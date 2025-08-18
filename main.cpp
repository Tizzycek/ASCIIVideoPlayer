//
// Created by Tiziano on 21.07.2025.
//

#ifdef _WIN32

#define SDL_MAIN_HANDLED

#include <iostream>
#include "audio.h"
#include "video.h"
#include <thread>
#include <fstream>

#define FILE ".\\config"

using namespace std;

bool load_info(string& frame_path, string& music_path, int& fps) {
    ifstream file(FILE);

    // 1. Controllo apertura file
    if (!file.is_open()) {
        cerr << "Errore: Impossibile aprire il file di configurazione" << endl;
        return false;
    }

    // 2. Lettura frame_path (prima riga)
    if (!getline(file, frame_path)) {
        cerr << "Errore: Lettura del percorso dei frame fallita" << endl;
        return false;
    }

    // 3. Lettura music_path (seconda riga)
    if (!getline(file, music_path)) {
        cerr << "Errore: Lettura del percorso della musica fallita" << endl;
        return false;
    }

    // 4. Lettura FPS (terza riga) con controllo numerico
    string fps_str;
    if (!getline(file, fps_str)) {
        cerr << "Errore: Lettura degli FPS fallita" << endl;
        return false;
    }

    try {
        fps = stoi(fps_str);  // Converte stringa a intero
    } catch (const invalid_argument&) {
        cerr << "Errore: Gli FPS devono essere un numero valido (got '" << fps_str << "')" << endl;
        return false;
    } catch (const out_of_range&) {
        cerr << "Errore: Valore FPS fuori dal range consentito" << endl;
        return false;
    }

    // 5. Controllo valori sensati
    if (fps <= 0) {
        cerr << "Errore: Gli FPS devono essere > 0 (got " << fps << ")" << endl;
        return false;
    }

    // 6. Verifica che il file non abbia altre righe (opzionale)
    if (string extra_line; getline(file, extra_line)) {
        cerr << "Warning: Il file di configurazione contiene righe extra (ignorate)" << endl;
    }

    return true;  // Tutto ok!
}

int main() {
    string frame_path, music_path;
    int fps;

    if (!load_info(frame_path, music_path, fps)) {
        cerr << "Errore nel caricamento delle configurazioni!" << endl;
        return 3;
    }

    AsciiFrame* frames = load_frames(frame_path.c_str());

    if (frames == nullptr) {
        cerr << "Errore allocazione memoria!" << endl;
        return 1;
    }

    thread first(play_frames, frames, fps);
    thread second(startAudio, music_path);

    first.join();
    second.join();

    free_frames(frames);

    delete[] frames;
    return 0;
}

#else

#include <iostream>

int main() {
    std::cerr << "To be implemented!" << std::endl;
    abort();
}

#endif