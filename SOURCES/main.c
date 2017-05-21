/********************************************************************
*
* @Arxiu : main.c
* @Finalitat : Conté el procediment principal i les funcions més generals de la aplicació Hidden.
* @Autors : Esteve Genovard Ferriol - ls30742 & Sergi Simó Bosquet - ls30685
* @Data Creació: 26 d'abril del 2017
*
******************************************************************** */
//Llibreries pròpies
#include "../HEADERS/error.h"
#include "../HEADERS/fat.h"
#include "../HEADERS/ext.h"
#include "../HEADERS/output.h"

//CONSTANTS
#define INFO_OPTION "-info"
#define SEARCH_OPTION "-search"
#define SHOW_OPTION "-show"
#define ACTIVATE_READ_OPTION "-r"
#define DESACTIVATE_READ_OPTION "-w"
#define ACTIVATE_HIDDEN_OPTION "-h"
#define DESACTIVATE_HIDDEN_OPTION "-s"
#define CHANGE_DATE_OPTION "-d"

//VARIABLES
InfoFAT32 infoF32;
InfoEXT4 infoE4;

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

Fitxer MAIN_checkForFormat (char * fitxer) {

  Fitxer format = NOFORMAT;
  FILE * f;

  f = fopen(fitxer, "rb");
  if (f == NULL) {
    ERROR_print(ERROR_OPENING_SYSTEM_FILE);
    exit(-1);
  } else{
    if (EXT_magicNumber(f)){
        format = EXT_format(f);
    } else {
      format = FAT_format(f);
    }
    fclose(f);
  }
  return format;
}

void MAIN_goForFormatInfo (char * fitxer) {

  int format = MAIN_checkForFormat(fitxer);
  switch (format) {

    case FAT32:
      infoF32 = FAT_getInfoFAT32(fitxer);
      OUTPUT_FAT32 (infoF32);
      break;

    case EXT4:
      infoE4 = EXT_getInfoEXT4(fitxer);
      OUTPUT_EXT4 (infoE4);
      break;

    case FAT16:
    case FAT12:
    case EXT3:
    case EXT2:
      OUTPUT_otherFormats(SFDisp[format]);
      break;

    case NOFORMAT:
      ERROR_print(ERROR_FILE_NOT_RECOGNIZED);
      exit (-1);
      break;

  }
}

void MAIN_searchFile(char * fitxer, char * file, int fase) {

  int format = MAIN_checkForFormat(fitxer);
  switch (format) {

    case FAT32:
      FAT_findFileOnRoot(fitxer, file);

      if (fase) {
        FAT_showInfo(fitxer);
      }
      else {
         OUTPUT_searchFAT32 (ACFAT32);
      }

      break;

    case EXT4:
      break;

    case FAT16:
    case FAT12:
    case EXT3:
    case EXT2:
    case NOFORMAT:
      ERROR_print(ERROR_FILE_FORMAT);
      exit (-1);
      break;

  }

}


int main (int argc, char ** argv) {

  if (argc < 3) {
    ERROR_print(ERROR_NOT_ENOUGH_ARGUMENTS);
    exit (-1);
  } else {

    switch (MAIN_checkForOption(argv[1])) {

      case INFO:
        MAIN_goForFormatInfo(argv[2]);
        break;

      case SEARCH:
        if (argc < 4) {
          ERROR_print(ERROR_NOT_ENOUGH_ARGUMENTS);
          exit (-1);
        }
        MAIN_searchFile(argv[3], argv[2], 0);
        break;

      case SHOW:
        if (argc < 4) {
          ERROR_print(ERROR_NOT_ENOUGH_ARGUMENTS);
          exit (-1);
        }
        MAIN_searchFile(argv[3], argv[2], 1);
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
