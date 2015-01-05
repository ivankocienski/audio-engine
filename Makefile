
EXE=test
GPP=g++
CFLAGS=-Wall -g
LIBS=-lportaudio -lSDL -lSDL_image

OBJ=\
	main.o \
	audio-exception.o \
	audio-service.o \
	audio-channel.o \
	audio-pattern.o \
	audio-oscillator-base.o \
	audio-sine-oscillator.o \
	window.o

all: $(OBJ)
	$(GPP) $(OBJ) -o $(EXE) $(CFLAGS) $(LIBS)

.cc.o:
	$(GPP) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXE)
