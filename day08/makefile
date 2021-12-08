CC=gcc
CFLAGS=-O3 -lm
DEPS=
OBJ=main.o
OUT=main.out

all: $(OUT)
	./$(OUT)

time: $(OUT)
	time ./$(OUT)

$(OUT): $(OBJ) $(DEPS)
	$(CC) -o $(OUT) $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) -Wall

.PHONY: clean
clean: 
	rm *.o *.out