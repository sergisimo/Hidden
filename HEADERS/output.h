/********************************************************************
*
* @Arxiu : fat32.h
* @Finalitat : Conté les capçaleres de les funcions per informar l'usuari.
* @Autors : Esteve Genovard Ferriol - ls30742 & Sergi Simó Bosquet - ls30685
* @Data Creació: 26 d'abril del 2017
*
******************************************************************** */
#ifndef _OUTPUT_H_
  #define _OUTPUT_H_

  //Llibreries pròpies
  #include "../HEADERS/types.h"

  //Llibreries del sistema
  #include <stdio.h>
  #include <stdlib.h>
  #include <time.h>

  //CONSTANTS

  //Capçaleres
  void OUTPUT_searchFAT32 (ArchCercatFAT32 ACFAT32);

  void OUTPUT_searchEXT4 (unsigned long size, unsigned int creationTime);

  void OUTPUT_FAT32 (InfoFAT32 info);

  void OUTPUT_EXT4 ();

  void OUTPUT_otherFormats (const char * const SFD);

#endif
