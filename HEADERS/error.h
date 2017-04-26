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
  #define ERROR_NOT_ENOUGH_ARGUMENTS "Error! Falten arguments en la crida del programa. Ex: \"./Hidden <opcio> <nom_del_volum>\"\n"
  #define ERROR_INVALID_OPTION "Error! La opció introduida no existeix.\n"

  //Capçaleres
  void ERROR_print(char * error);

  void ERROR_exit(int errorCode);

#endif
