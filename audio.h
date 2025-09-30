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
#include <cstdbool>

bool startAudio(const std::string&);

#endif //AUDIO_H
