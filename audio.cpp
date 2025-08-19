#include "audio.h"

bool startAudio(const std::string &path) {
    // Inizializza SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    // Inizializza SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << "\n";
        return false;
    }

    // Carica il file audio
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music) {
        std::cerr << "Failed to load music! Mix_Error: " << Mix_GetError() << "\n";
        return false;
    }

    // Riproduci l'audio
    if (Mix_PlayMusic(music, 0) < 0) {
        std::cerr << "Failed to play music! Mix_Error: " << Mix_GetError() << "\n";
        return false;
    }

    std::cout << "Playing audio... Press Enter to stop.\n";

    // Attendi la fine della riproduzione o input utente
    while (Mix_PlayingMusic())
        SDL_Delay(100);

    // Pulizia
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();

    return true;
}
