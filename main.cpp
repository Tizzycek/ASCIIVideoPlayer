//
// Created by Tiziano on 21.07.2025.
//

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#endif


#include <iostream>
#include "audio.h"
#include "video.h"
#include <thread>
#include <fstream>

#ifdef _WIN32
#define FILE ".\\config"
#else
#define FILE "./config"
#endif

using namespace std;

// Controllo del file
void load_info(string& frame_path, string& music_path, unsigned int& fps) {
    ifstream file(FILE);

    // 1. Apertura
    if (!file.is_open())
        throw runtime_error("Impossibile aprire il file di configurazione");

    // 2. Lettura frame_path (prima riga)
    if (!getline(file, frame_path))
        throw runtime_error("Lettura del percorso dei frame fallita");

    // 3. Lettura music_path (seconda riga)
    if (!getline(file, music_path))
        throw runtime_error("Lettura del percorso della musica fallita");

    // 4. Lettura FPS (terza riga) con controllo numerico
    string fps_str;
    if (!getline(file, fps_str))
        throw runtime_error("Lettura degli FPS fallita");

    try {
        fps = stoi(fps_str);  // Converte stringa a intero
    } catch (const invalid_argument&) {
        throw runtime_error("Gli FPS devono essere un numero valido (rilevato '" + fps_str + "')");
    } catch (const out_of_range&) {
        throw runtime_error("Valore FPS fuori dal range consentito");
    }

    // 5. Controllo valori sensati
    if (fps <= 0)
        throw runtime_error("Gli FPS devono essere > 0 (rilevato " + to_string(fps) + ")");

    // 6. Verifica righe extra (opzionale)
    if (string extra_line; getline(file, extra_line))
        cout << "Attenzione: il file di configurazione contiene righe extra (ignorate)" << endl;
}

int main() {
    string frame_path;
    string music_path;
    unsigned int fps;

    try {
        load_info(frame_path, music_path, fps);
    } catch (exception& e) {
        cerr << "Errore: " << e.what() << endl;
        return 2;
    }

    FrameNode* frames = load_frames(frame_path.c_str());

    if (frames == nullptr)
        return 1;

    thread first(play_frames, frames, fps);
    thread second(startAudio, music_path);

    first.join();
    second.join();

    free_frames(frames);

    return 0;
}