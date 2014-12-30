
EXE=test
GPP=g++
OBJ=main.o audio.o
CFLAGS=-Wall -g
LIBS=-lportaudio

all: $(OBJ)
	$(GPP) $(OBJ) -o $(EXE) $(CFLAGS) $(LIBS)

.cc.o:
	$(GPP) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXE)
