# Compilatori
CC = gcc
CXX = g++
WINDRES = windres

# Flags di compilazione
CFLAGS = -Wall -Wextra
CXXFLAGS = -Wall -Wextra

# Directory di inclusione librerie SDL2
# Il percorso specificato è per Windows, se le librerie SDL2 e SDL2_mixer si trovano nella stessa cartella.
# Per esempio, SDL2_mixer.h si troverà in ./SDL2/include/SDL2/SDL2_mixer.h
# Stesso discorso valido per le librerie compilate.
# Su linux è necessario modificare questo percorso, affinché il compilatore trovi nella cartella di installazione corretta le librerie.
INCLUDES = -I./SDL2/include/
LIBS = -L./SDL2/lib/ -lSDL2 -lSDL2_mixer

ifeq ($(OS), Linux)
	LIBS += -pthread
endif

# File oggetto
OBJS = video.o audio.o main.o resources.o

# Nome dell'eseguibile finale
TARGET = ASCIIVideoPlayer

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(INCLUDES) $(LIBS) $(CXXFLAGS)

# Regola per compilare le risorse
resources.o: icon/resources.rc icon/icon.png
	$(WINDRES) $< -o $@

video.o: video.c video.h
	$(CC) -c $< -o $@ $(INCLUDES) $(LIBS) $(CFLAGS)

audio.o: audio.cpp audio.h
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS) $(CXXFLAGS)

main.o: main.cpp audio.h video.h
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS) $(CXXFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)