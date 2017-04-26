all: Hidden

H=./HEADERS/
S=./SOURCES/
OBJ=main.o error.o
FLAGS=-Wall -Wextra

main.o: $Smain.c $Htypes.h
	gcc $Smain.c -c $(FLAGS)

error.o: $Serror.c $Herror.h
	gcc $Serror.c -c $(FLAGS)

Hidden: $(OBJ)
	gcc $(OBJ) -o Hidden $(FLAGS)

clean:
	rm *.o Hidden
