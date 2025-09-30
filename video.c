//
// Updated video.c
//
#include "video.h"

char* read_line_dynamic(FILE *file) {
    size_t size = 128;
    size_t len = 0;
    char *buffer = malloc(size);
    if (!buffer) return NULL;

    int c;
    while ((c = fgetc(file)) != EOF && c != '\n') {
        if (len + 1 >= size) {
            size *= 2;
            char *new_buf = realloc(buffer, size);
            if (!new_buf) {
                free(buffer);
                return NULL;
            }
            buffer = new_buf;
        }
        buffer[len++] = (char)c;
    }

    if (len == 0 && c == EOF) {
        free(buffer);
        return NULL;
    }

    buffer[len] = '\0';
    return buffer;
}

int compare_filenames(const void *a, const void *b) {
    const char *fa = *(const char **)a;
    const char *fb = *(const char **)b;
    return strcmp(fa, fb);
}

#ifdef _WIN32

DWORD WINAPI spinner_thread(LPVOID param) {
    HANDLE hEvent = (HANDLE)param;
    int i = 0;

    printf("Caricamento in corso. Non chiudere questa finestra.  ");

    while (WaitForSingleObject(hEvent, 100) == WAIT_TIMEOUT) {
        const char signs[4] = {'|', '/', '-', '\\'};
        printf("\b%c", signs[i % 4]);
        fflush(stdout);
        i++;
    }
    return 0;
}

FrameNode* load_frames(const char *dir_path/*, int *out_frame_count*/) {
    WIN32_FIND_DATA find_file_data;
    HANDLE hFind;
    char search_path[MAX_PATH];
    char **file_list = NULL;
    int file_count = 0;

    // spinner
    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE hThread = CreateThread(NULL, 0, spinner_thread, hEvent, 0, NULL);

    snprintf(search_path, sizeof(search_path), "%s\\*", dir_path);
    hFind = FindFirstFile(search_path, &find_file_data);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Directory non trovata o errore di accesso\n");
        SetEvent(hEvent);
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
        CloseHandle(hEvent);
        return NULL;
    }

    // Ricerca dei nomi dei vari frames
    do {
        if (strcmp(find_file_data.cFileName, ".") == 0 ||
            strcmp(find_file_data.cFileName, "..") == 0) {
            continue;
        }
        file_list = realloc(file_list, sizeof(char*) * (file_count + 1));
        file_list[file_count] = _strdup(find_file_data.cFileName);
        file_count++;
    } while (FindNextFile(hFind, &find_file_data) != 0);
    FindClose(hFind);

    if (file_count == 0) {
        printf("Nessun frame trovato\n");
        SetEvent(hEvent);
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
        CloseHandle(hEvent);
        return NULL;
    }

    // Funzione per riordinarli
    qsort(file_list, file_count, sizeof(char*), compare_filenames);

    FrameNode *head = NULL, *tail = NULL;
    for (int i = 0; i < file_count; i++) {
        char filepath[MAX_PATH];
        snprintf(filepath, sizeof(filepath), "%s\\%s", dir_path, file_list[i]);

        FILE *file = fopen(filepath, "r");
        if (!file) {
            printf("Impossibile aprire %s\n", filepath);
            continue;
        }

        AsciiFrame frame;
        frame.lines = NULL;
        frame.line_count = 0;

        char *line;
        while ((line = read_line_dynamic(file)) != NULL) {
            frame.lines = realloc(frame.lines, sizeof(char*) * (frame.line_count + 1));
            frame.lines[frame.line_count] = line;
            frame.line_count++;
        }
        fclose(file);
        free(line);

        FrameNode *node = malloc(sizeof(FrameNode));
        node->frame = frame;
        node->next = NULL;

        if (!head) head = node;
        else tail->next = node;
        tail = node;
    }

    for (int i = 0; i < file_count; i++)
        free(file_list[i]);

    free(file_list);

    //if (out_frame_count) *out_frame_count = file_count;

    // Ferma spinner
    SetEvent(hEvent);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    CloseHandle(hEvent);

    //printf("\nTotale frame caricati: %d\n", file_count);
    return head;
}

void play_frames(FrameNode *head, int fps) {
    if (fps <= 0) fps = 1;
    int delay_ms = 1000 / fps;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (head != NULL) {
        // pulizia
        COORD coord = {0, 0};
        DWORD written;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        int console_size = csbi.dwSize.X * csbi.dwSize.Y;
        FillConsoleOutputCharacter(hConsole, ' ', console_size, coord, &written);
        SetConsoleCursorPosition(hConsole, coord);

        // stampa frame
        for (int i = 0; i < head->frame.line_count; i++) {
            DWORD w;
            WriteConsoleA(hConsole, head->frame.lines[i],
                          (DWORD)strlen(head->frame.lines[i]), &w, NULL);
            WriteConsoleA(hConsole, "\n", 1, &w, NULL);
        }

        Sleep(delay_ms);
        head = head->next;
    }
}

#else

// Spinner
static int spinner_running = 1;
void* spinner_thread(void *arg) {
    const char signs[4] = {'|', '/', '-', '\\'};
    int i = 0;
    printf("Caricamento in corso. Non chiudere questa finestra.  ");
    fflush(stdout);
    while (spinner_running) {
        printf("\b%c", signs[i % 4]);
        fflush(stdout);
        i++;
        usleep(100000); // 100 ms
    }
    return NULL;
}

// Caricamento frame (Linux)
FrameNode* load_frames(const char *dir_path/*, int *out_frame_count*/) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Errore apertura directory");
        return NULL;
    }

    pthread_t spinner;
    pthread_create(&spinner, NULL, spinner_thread, NULL);

    char **file_list = NULL;
    int file_count = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        file_list = realloc(file_list, sizeof(char*) * (file_count + 1));
        file_list[file_count] = strdup(entry->d_name);
        file_count++;
    }
    closedir(dir);

    if (file_count == 0) {
        printf("Nessun frame trovato\n");
        spinner_running = 0;
        pthread_join(spinner, NULL);
        return NULL;
    }

    qsort(file_list, file_count, sizeof(char*), compare_filenames);

    FrameNode *head = NULL, *tail = NULL;
    for (int i = 0; i < file_count; i++) {
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", dir_path, file_list[i]);

        FILE *file = fopen(filepath, "r");
        if (!file) continue;

        AsciiFrame frame = {NULL, 0};
        char *line;
        while ((line = read_line_dynamic(file)) != NULL) {
            frame.lines = realloc(frame.lines, sizeof(char*) * (frame.line_count + 1));
            frame.lines[frame.line_count++] = line;
        }
        fclose(file);

        FrameNode *node = malloc(sizeof(FrameNode));
        node->frame = frame;
        node->next = NULL;

        if (!head) head = node;
        else tail->next = node;
        tail = node;
    }

    for (int i = 0; i < file_count; i++) free(file_list[i]);
    free(file_list);

    //if (out_frame_count) *out_frame_count = file_count;

    spinner_running = 0;
    pthread_join(spinner, NULL);

    printf("\nTotale frame caricati: %d\n", file_count);
    return head;
}

// Riproduzione (Linux, usa printf e ANSI escape)
void play_frames(FrameNode *head, int fps) {
    if (fps <= 0) fps = 1;
    int delay_us = 1000000 / fps;

    while (head != NULL) {
        printf("\033[H\033[J"); // clear screen ANSI

        for (int i = 0; i < head->frame.line_count; i++) {
            printf("%s\n", head->frame.lines[i]);
        }

        fflush(stdout);
        usleep(delay_us);
        head = head->next;
    }
}
#endif

void free_frames(FrameNode *head) {
    while (head != NULL) {
        for (int i = 0; i < head->frame.line_count; i++) {
            free(head->frame.lines[i]);
        }
        free(head->frame.lines);

        FrameNode *next = head->next;
        free(head);
        head = next;
    }
}

