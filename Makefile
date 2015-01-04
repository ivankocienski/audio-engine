
EXE=test
GPP=g++
OBJ=main.o audio-exception.o audio-service.o audio-channel.o audio-pattern.o audio-oscillator-base.o audio-sine-oscillator.o
CFLAGS=-Wall -g
LIBS=-lportaudio

all: $(OBJ)
	$(GPP) $(OBJ) -o $(EXE) $(CFLAGS) $(LIBS)

.cc.o:
	$(GPP) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXE)
