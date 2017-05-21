/********************************************************************
*
* @Arxiu : fat32.h
* @Finalitat : Conté les capçaleres de les funcions que permeten la gestió dels sistemes de fitchers FAT.
* @Autors : Esteve Genovard Ferriol - ls30742 & Sergi Simó Bosquet - ls30685
* @Data Creació: 26 d'abril del 2017
*
******************************************************************** */
#ifndef _FAT_H_
  #define _FAT_H_

  //Llibreries pròpies
  #include "../HEADERS/types.h"
  #include "../HEADERS/error.h"

  //Llibreries del sistema
  #include <stdio.h>
  #include <stdlib.h>

  //CONSTANTS

  //VARIABLES
  ArchCercatFAT32 ACFAT32;

  //Capçaleres
  InfoFAT32 FAT_getInfoFAT32 (char * fitxer);

  EstructuraFAT32 FAT_getEstructura (FILE * f, InfoFAT32 info);

  int FAT_CercaRootFile(FILE * f, InfoFAT32 info, EntryFAT32 entry, EstructuraFAT32 FAT32, ArchiuFAT AFAT, int finalEntry);

  ArchiuFAT FAT_inicialitzaValors(char * file);

  void FAT_nomArchiuSenseVFATLN (EntryFAT32 entry, char paraula[150]);

  ArchiuFAT FAT_nomArchiuVFATLN (EntryFAT32 entry, ArchiuFAT AFAT, FILE * f);

  EstructuraFAT32 FAT_subdirectory (EntryFAT32 entry, InfoFAT32 info, EstructuraFAT32 EFAT32, FILE * f);

  Fitxer FAT_format(FILE * f);

  void FAT_showInfo(char * fitxer);

  Fitxer FAT_formatFat32 (FILE * f);

  Fitxer FAT_formatFat1216 (FILE * f);

  int FAT_findFileOnRoot(char * fitxer, char * file);

#endif
