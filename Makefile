# Compilatori
CC = gcc
CXX = g++

ifeq ($(OS), Windows_NT)
	WINDRES = windres
	RM = del
else
	WINDRES = 
	RM = rm
endif

# Flags di compilazione
CFLAGS = -Wall -Wextra
CXXFLAGS = -Wall -Wextra

# Directory di inclusione librerie SDL2
# Su Linux, i percorsi vanno modificati solo se le librerie SDL2 non sono state installate globalmente nel sistema.
# Su Windows, i percorsi vanno modificati solo se le librerie si trovano in un percorso diverso da quello già specificato. 
INCLUDES = -I./SDL2/include/
LIBS = -L./SDL2/lib/ -lSDL2 -lSDL2_mixer

ifeq ($(OS), Linux)
	LIBS += -pthread
endif

# File oggetto
OBJS = video.o audio.o main.o
ifeq ($(OS), Windows_NT)
	OBJS += resources.o
endif

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
	$(CC) -c $< -o $@ $(CFLAGS)

audio.o: audio.cpp audio.h
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS) $(CXXFLAGS)

main.o: main.cpp audio.h video.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	$(RM) -f $(OBJS) $(TARGET)
