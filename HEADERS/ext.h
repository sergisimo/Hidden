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
  #include "../HEADERS/output.h"

  //Llibreries del sistema
  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>

  //CONSTANTS

  //VARIABLES GLOBALS
  short flagTrobat, option, printing;
  unsigned long inodeSize;
  char * newDate;

  //Capçaleres
  InfoEXT4 EXT_getInfoEXT4 (char * fitxer);

  Fitxer EXT_format(FILE * f);

  int EXT_magicNumber(FILE * f);

  Fitxer EXT_journalFlag(FILE * f);

  Fitxer EXT_extendFlag(FILE * f);

  void EXT_findFile(char * volume, char * fileName, short optionAux, char * date);

  void recorreArbre(FILE * f, char * fileName, InfoEXT4 info);

  void recorreNodesInterns(FILE * f, char * fileName, InfoEXT4 info, unsigned short entries);

  void llegeixNodesFulla(FILE * f, char * fileName, InfoEXT4 info, unsigned short entries);

  void readDataBlock(FILE * f, char * fileName, InfoEXT4 info, unsigned long dataBlockAdress);

  void recorreDirectori(FILE * f, char * fileName, InfoEXT4 info, unsigned int inodeNumber);

  void manageFile(FILE * f, char * fileName, InfoEXT4 info, unsigned int inodeNumber);

  void showFileContent(FILE * f, InfoEXT4 info, unsigned long dataBlockAdress);

  unsigned int createDate();

  unsigned long createAdressExt(unsigned long adressHigh,unsigned int adressLow);

#endif
