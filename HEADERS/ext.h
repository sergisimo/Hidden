/********************************************************************
*
* @Arxiu : fat32.h
* @Finalitat : Conté les capçaleres de les funcions que permeten la gestió dels sistemes de fitchers EXT.
* @Autors : Esteve Genovard Ferriol - ls30742 & Sergi Simó Bosquet - ls30685
* @Data Creació: 26 d'abril del 2017
*
******************************************************************** */
#ifndef _EXT_H_
  #define _EXT_H_

  //Llibreries pròpies
  #include "../HEADERS/types.h"
  #include "../HEADERS/error.h"

  //Llibreries del sistema
  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>

  //CONSTANTS

  //Capçaleres
  InfoEXT4 EXT_getInfoEXT4 (char * fitxer);

  Fitxer EXT_format(FILE * f);

  int EXT_magicNumber(FILE * f);

  Fitxer EXT_journalFlag(FILE * f);

  Fitxer EXT_extendFlag(FILE * f);

#endif
