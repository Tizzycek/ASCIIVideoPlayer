//
// Created by Tiziano on 21.07.2025.
//
#pragma once

#ifndef VIDEO_H
#define VIDEO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#ifdef _WIN32

#include <windows.h>

DWORD WINAPI spinner_thread(LPVOID param);

FrameNode* load_frames(const char */*, int **/);

void play_frames(FrameNode *, int);

#else

#include <dirent.h>
#include <unistd.h>
#include <pthread.h>

void* spinner_thread(void *);

FrameNode* load_frames(const char */*, int **/);

void play_frames(FrameNode *, int);

#endif //_WIN32

void free_frames(FrameNode *);

#ifdef __cplusplus
}
#endif

#endif
