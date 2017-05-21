all: Hidden

H=./HEADERS/
S=./SOURCES/
OBJ=main.o fat.o ext.o error.o output.o -lm
FLAGS=-Wall -Wextra

main.o: $Smain.c $Hext.h $Hext.h
	gcc $Smain.c -c $(FLAGS)

fat.o: $Sfat.c $Hfat.h $Htypes.h $Herror.h
	gcc $Sfat.c -c $(FLAGS)

ext.o: $Sext.c $Hext.h $Htypes.h $Herror.h
		gcc $Sext.c -c $(FLAGS)

error.o: $Serror.c $Herror.h
	gcc $Serror.c -c $(FLAGS)

output.o: $Soutput.c $Houtput.h $Htypes.h
	gcc $Soutput.c -c $(FLAGS)

Hidden: $(OBJ)
	gcc $(OBJ) -o Hidden $(FLAGS)

clean:
	rm *.o Hidden
