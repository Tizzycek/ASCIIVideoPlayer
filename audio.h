//
// Created by Tiziano on 21.07.2025.
//

#ifndef AUDIO_H
#define AUDIO_H

#ifdef _WIN32
#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_mixer.h"
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif
#include <iostream>
#include <future>

void startAudio(const std::string&, std::promise<void>& prom);

#endif //AUDIO_H
