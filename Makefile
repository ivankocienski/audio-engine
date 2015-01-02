
EXE=test
GPP=g++
OBJ=main.o audio-exception.o audio-tone.o audio-service.o audio-channel.o audio-pattern.o
CFLAGS=-Wall -g
LIBS=-lportaudio

all: $(OBJ)
	$(GPP) $(OBJ) -o $(EXE) $(CFLAGS) $(LIBS)

.cc.o:
	$(GPP) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXE)
