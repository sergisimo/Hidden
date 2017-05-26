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

  typedef struct InfoFAT32{
    char sysName[8];
    short secSize;
    char secXClou;
    short resSec;
    char numFat;
    short rootE;
    int secXFat;
    int RFCluster;
    char label[11];
  }InfoFAT32;

  typedef struct ArchCercatFAT32{
    char NomArch[150];
    unsigned long direccio;
    int cluster;
    int mida;
    short data;
    int soloLectura;
    int oculto;
    unsigned long direccioEntry;
  }ArchCercatFAT32;

  typedef struct EntryFAT32{
    char DName[8];
    char ext[3];
    char Attrb;
    char palla [2];
    char CTime [2];
    short CDate;
    char LADate [2];
    unsigned short FCHI;
    char LMTime [2];
    char LMDate [2];
    unsigned short FCLW;
    int FSize;
  }EntryFAT32;

  typedef struct ArchiuFAT{
    int trobat;
    char * file;
    char paraula[150];
    int VFATLongNameFlag;
    int longitud;
  }ArchiuFAT;

  typedef struct EstructuraFAT32{
    unsigned long dataRegionMesCluster;
    unsigned long dataRegion;
    unsigned long fatTable;
    unsigned int nextCluster;
  }EstructuraFAT32;

  typedef struct InfoEXT4{
    short inoSize;
    int numIno;
    int firstIno;
    int inoGrp;
    int freeIno;
    unsigned int blkSize;
    unsigned long rsvBlk;
    int freeBlk;
    int totBlk;
    int firstBlk;
    int blkGrp;
    int frgGrp;
    char vlName[16];
    int lstCheck;
    int lstMount;
    int lstWritten;
    int offset;
  }InfoEXT4;

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

  typedef enum {

    FAT32,
    FAT16,
    FAT12,
    EXT4,
    EXT3,
    EXT2,
    NOFORMAT
  } Fitxer;

  static const char* const SFDisp[] = { "FAT32", "FAT16", "FAT12", "EXT4", "EXT3", "EXT2", "NOFORMAT"};

#endif
