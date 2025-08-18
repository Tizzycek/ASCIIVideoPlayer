# Compilatori
CC = gcc
CXX = g++

# Flags di compilazione
CFLAGS = -Wall -Wextra
CXXFLAGS = -Wall -Wextra

# Directory di inclusione e librerie SDL2
INCLUDES = -I./SDL2/include/
LIBS = -L./SDL2/lib/ -lSDL2 -lSDL2_mixer

# File oggetto
OBJS = video.o audio.o main.o

# Nome dell'eseguibile finale
TARGET = ASCIIVideoPlayer

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(INCLUDES) $(LIBS) $(CXXFLAGS)

video.o: video.c video.h
	$(CC) -c $< -o $@ $(INCLUDES) $(LIBS) $(CFLAGS)

audio.o: audio.cpp audio.h
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS) $(CXXFLAGS)

main.o: main.cpp audio.h video.h
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS) $(CXXFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)