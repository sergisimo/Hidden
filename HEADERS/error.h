/********************************************************************
*
* @Arxiu : error.h
* @Finalitat : Conté les capçaleres de les funcions que permeten la gestió dels diferents errors de la aplicació Hidden.
* @Autors : Esteve Genovard Ferriol - ls30742 & Sergi Simó Bosquet - ls30685
* @Data Creació: 26 d'abril del 2017
*
******************************************************************** */
#ifndef _ERROR_H_
  #define _ERROR_H_

  //Llibreries del sistema
  #include <stdio.h>
  #include <stdlib.h>

  //CONSTANTS
  #define ERROR_NOT_ENOUGH_ARGUMENTS "\nError: Incorrect parameter number.\n"
  #define ERROR_INVALID_OPTION "\nError! La opció introduida no existeix.\n"
  #define ERROR_OPENING_SYSTEM_FILE "\nError. Filesystem not found.\n"
  #define ERROR_FILE_FORMAT "\nError. The volume is neither FAT32 or EXT4.\n"
  #define ERROR_FILE_NOT_RECOGNIZED "\nFile System not recognized\n"
  #define ERROR_FILE_NOT_FOUND "\nError. File not found.\n"

  //Capçaleres
  void ERROR_print(char * error);

  void ERROR_exit(int errorCode);

#endif
