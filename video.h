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

typedef struct {
    char **lines;      // array di stringhe
    int line_count;    // numero di righe
} AsciiFrame;

typedef struct FrameNode {
    AsciiFrame frame;
    struct FrameNode *next;
} FrameNode;

#ifdef __cplusplus
extern "C" {
#endif

char* read_line_dynamic(FILE *);

int compare_filenames(const void *, const void *);

DWORD WINAPI spinner_thread(LPVOID param);

FrameNode* load_frames(const char */*, int **/);

void play_frames(FrameNode *, int);

void free_frames(FrameNode *);

#ifdef __cplusplus
}
#endif

#endif //_WIN32

#endif
