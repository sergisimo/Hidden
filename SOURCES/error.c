/********************************************************************
*
* @Arxiu : error.c
* @Finalitat : Conté la implementació de les funcions que permeten la gestió dels diferents errors de la aplicació Hidden.
* @Autors : Esteve Genovard Ferriol - ls30742 & Sergi Simó Bosquet - ls30685
* @Data Creació: 26 d'abril del 2017
*
******************************************************************** */
#include "../HEADERS/error.h"

void ERROR_print(char * error) {

  printf("%s", error);
}

void ERROR_exit(int errorCode) {

  exit(errorCode);
}
