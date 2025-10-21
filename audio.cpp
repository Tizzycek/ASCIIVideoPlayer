#include "audio.h"

void startAudio(const std::string &path, std::promise<void>& prom) {
    using namespace std;
    try {
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
            throw runtime_error("Impossibile inizializzare SDL! SDL_Error: " + string(SDL_GetError()));

        // Inizializza SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            throw runtime_error("Impossibile inizializzare SDL_mixer! Mix_Error: " + string(Mix_GetError()));

        // Carica il file audio
        Mix_Music* music = Mix_LoadMUS(path.c_str());
        if (!music)
            throw runtime_error("Caricamento audio fallito! Mix_Error: " + string(Mix_GetError()));

        // Riproduci l'audio
        if (Mix_PlayMusic(music, 0) < 0)
            throw runtime_error("Riproduzione audio fallita! Mix_Error: " + string(Mix_GetError()));

        cout << "Playing audio" << endl;

        // Attendi la fine della riproduzione
        while (Mix_PlayingMusic())
            SDL_Delay(100);

        // Pulizia
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();

        prom.set_value();
    } catch (...) {
        prom.set_exception(current_exception());
    }
}
