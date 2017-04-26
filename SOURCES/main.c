/********************************************************************
*
* @Arxiu : main.c
* @Finalitat : Conté el procediment principal i les funcions més generals de la aplicació Hidden.
* @Autors : Esteve Genovard Ferriol - ls30742 & Sergi Simó Bosquet - ls30685
* @Data Creació: 26 d'abril del 2017
*
******************************************************************** */
//Llibreries pròpies
#include "../HEADERS/types.h"
#include "../HEADERS/error.h"

//CONSTANTS
#define INFO_OPTION "-info"
#define SEARCH_OPTION "-search"
#define SHOW_OPTION "-show"
#define ACTIVATE_READ_OPTION "-r"
#define DESACTIVATE_READ_OPTION "-w"
#define ACTIVATE_HIDDEN_OPTION "-h"
#define DESACTIVATE_HIDDEN_OPTION "-s"
#define CHANGE_DATE_OPTION "-d"

Option MAIN_checkForOption (char * option) {

    if (!strcmp(option, INFO_OPTION)) return INFO;
    else if (!strcmp(option, SEARCH_OPTION)) return SEARCH;
    else if (!strcmp(option, SHOW_OPTION)) return SHOW;
    else if (!strcmp(option, ACTIVATE_READ_OPTION)) return ACTIVATE_READ;
    else if (!strcmp(option, DESACTIVATE_READ_OPTION)) return DESACTIVATE_READ;
    else if (!strcmp(option, ACTIVATE_HIDDEN_OPTION)) return ACTIVATE_HIDDEN;
    else if (!strcmp(option, DESACTIVATE_HIDDEN_OPTION)) return DESACTIVATE_HIDDEN;
    else if (!strcmp(option, CHANGE_DATE_OPTION)) return CHANGE_DATE;
    else {
      ERROR_print(ERROR_INVALID_OPTION);
      exit(-1);
    }
}

int main (int argc, char ** argv) {

  if (argc < 2) ERROR_print(ERROR_NOT_ENOUGH_ARGUMENTS);
  else {

    switch (MAIN_checkForOption(argv[1])) {

      case INFO:
        break;

      case SEARCH:
        break;

      case SHOW:
        break;

      case ACTIVATE_READ:
        break;

      case DESACTIVATE_READ:
        break;

      case ACTIVATE_HIDDEN:
        break;

      case DESACTIVATE_HIDDEN:
        break;

      case CHANGE_DATE:
        break;
    }
  }

  return 0;
}
