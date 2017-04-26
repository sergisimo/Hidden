/********************************************************************
*
* @Arxiu : tipus.h
* @Finalitat : Conté els tipus propis que es faran servir en els diferents mòduls de la aplicació Hidden.
* @Autors : Esteve Genovard Ferriol - ls30742 & Sergi Simó Bosquet - ls30685
* @Data Creació: 26 d'abril del 2017
*
******************************************************************** */
#ifndef _TIPUS_H_
  #define _TIPUS_H_

  //Llibreries del sistema
  #include <stdio.h>
  #include <string.h>

  typedef enum {

    INFO,
    SEARCH,
    SHOW,
    ACTIVATE_READ,
    DESACTIVATE_READ,
    ACTIVATE_HIDDEN,
    DESACTIVATE_HIDDEN,
    CHANGE_DATE
  } Option;

#endif
