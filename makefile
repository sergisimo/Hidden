all: Hidden

H=./HEADERS/
S=./SOURCES/
OBJ=main.o
FLAGS=-Wall -Wextra

main.o: $Smain.c
	gcc $Smain.c -c $(FLAGS)

Hidden: $(OBJ)
	gcc $(OBJ) -o Hidden $(FLAGS)

clean:
	rm *.o Hidden
