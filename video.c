//
// Created by Tiziano on 11.07.2025.
//
#include "video.h"
#ifdef _WIN32

void clear_screen() {
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD written, cells;

    GetConsoleScreenBufferInfo(console, &csbi);
    cells = csbi.dwSize.X * csbi.dwSize.Y;

    // Riempie lo schermo con spazi e resetta gli attributi
    FillConsoleOutputCharacter(console, ' ', cells, topLeft, &written);
    FillConsoleOutputAttribute(console, csbi.wAttributes, cells, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

void add_line_to_frame(AsciiFrame *frame, const char *line) {
    AsciiLine *new_line = (AsciiLine *)malloc(sizeof(AsciiLine));
    new_line->content = _strdup(line);
    new_line->next = NULL;

    if (frame->lines == NULL) {
        frame->lines = new_line;
    } else {
        AsciiLine *current = frame->lines;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_line;
    }
    frame->line_count++;
}

// Carica tutti i frame dalla directory
AsciiFrame* load_frames(const char *dir_path) {
    WIN32_FIND_DATA find_file_data;
    HANDLE hFind;
    char search_path[MAX_PATH];
    AsciiFrame *head = NULL;
    AsciiFrame *tail = NULL;
    int total_frames = 0;

    snprintf(search_path, sizeof(search_path), "%s\\*", dir_path);
    hFind = FindFirstFile(search_path, &find_file_data);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Directory non trovata o errore di accesso\n");
        return NULL;
    }

    do {
        if (strcmp(find_file_data.cFileName, ".") == 0 ||
            strcmp(find_file_data.cFileName, "..") == 0) {
            continue;
        }

        char filepath[MAX_PATH];
        snprintf(filepath, sizeof(filepath), "%s\\%s", dir_path, find_file_data.cFileName);

        FILE *file = fopen(filepath, "r");
        if (file == NULL) {
            printf("Impossibile aprire %s\n", filepath);
            continue;
        }

        // Crea nuovo frame
        AsciiFrame *new_frame = (AsciiFrame *)malloc(sizeof(AsciiFrame));
        new_frame->lines = NULL;
        new_frame->line_count = 0;
        new_frame->next = NULL;

        // Leggi tutte le righe
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            // Rimuovi newline
            buffer[strcspn(buffer, "\r\n")] = '\0';
            add_line_to_frame(new_frame, buffer);
        }

        fclose(file);

        // Aggiungi alla lista
        if (head == NULL) {
            head = new_frame;
            tail = new_frame;
        } else {
            tail->next = new_frame;
            tail = new_frame;
        }

        total_frames++;
        printf("Caricato frame %d (%d linee)\n", total_frames, new_frame->line_count);

    } while (FindNextFile(hFind, &find_file_data) != 0);

    FindClose(hFind);
    printf("Totale frame caricati: %d\n", total_frames);
    return head;
}

// Riproduci tutti i frame
void play_frames(AsciiFrame *head, int fps) {
    if (fps <= 0) fps = 1;
    int delay_ms = 1000 / fps;
    int frame_num = 1;

    while (head != NULL) {
        clear_screen();
        //printf("Frame %d\n", frame_num++);

        AsciiLine *current_line = head->lines;
        while (current_line != NULL) {
            printf("%s\n", current_line->content);
            current_line = current_line->next;
        }

        Sleep(delay_ms);
        head = head->next;
    }
}

// Libera la memoria
void free_frames(AsciiFrame *head) {
    while (head != NULL) {
        AsciiLine *line = head->lines;
        while (line != NULL) {
            AsciiLine *next_line = line->next;
            free(line->content);
            free(line);
            line = next_line;
        }
        AsciiFrame *next_frame = head->next;
        free(head);
        head = next_frame;
    }
}

bool load_all(const char*frames_dir, const int fps, const int frame_count, const AsciiFrame*frames) {

    if (fps <= 0) {
         printf("FPS deve essere un numero positivo\n");
         return false;
    }

    if (frames==NULL) {
        perror("Allocazione fallita");
        return false;
    }

    if (frame_count <= 0) {
        printf("Nessun frame trovato nella directory %s\n", frames_dir);
        return false;
    }

    return true;
}

#endif