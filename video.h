//
// Created by Tiziano on 21.07.2025.
//
#pragma once

#ifndef VIDEO_H
#ifdef _WIN32
#define VIDEO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <windows.h>
#include <io.h>  // Per _access()
#include <direct.h>  // Per _mkdir()

typedef struct AsciiLine {
    char *content;
    struct AsciiLine *next;
} AsciiLine;

typedef struct AsciiFrame {
    AsciiLine *lines;
    int line_count;
    struct AsciiFrame *next;
} AsciiFrame;

#ifdef __cplusplus
extern "C" {
#endif

void clear_screen();

void add_line_to_frame(AsciiFrame *, const char *);

AsciiFrame* load_frames(const char *);

void play_frames(AsciiFrame *, int );

void free_frames(AsciiFrame *);

bool load_all(const char*, const int, const int, const AsciiFrame*);

void*loading_spinner();

#ifdef __cplusplus
}
#endif

#endif //VIDEO_H

#endif
