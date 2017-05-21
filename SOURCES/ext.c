/********************************************************************
*
* @Arxiu : fat.c
* @Finalitat : Conté la implementació de les funcions que permeten la gestió dels sistemes de fitxers EXT.
* @Autors : Esteve Genovard Ferriol - ls30742 & Sergi Simó Bosquet - ls30685
* @Data Creació: 26 d'abril del 2017
*
******************************************************************** */
#include "../HEADERS/ext.h"

InfoEXT4 EXT_getInfoEXT4 (char * fitxer) {

  InfoEXT4 info;
  FILE * f;

  f = fopen(fitxer, "rb");
  if (f == NULL) {
    ERROR_print(ERROR_OPENING_SYSTEM_FILE);
    exit(-1);
  } else{

    fseek(f, 1112, SEEK_SET);
    fread (&info.inoSize, 2, 1, f);
    fseek(f, 1024, SEEK_SET);
    fread (&info.numIno, 4, 1, f);
    fseek(f, 1108, SEEK_SET);
    fread (&info.firstIno, 4, 1, f);
    fseek(f, 1064, SEEK_SET);
    fread (&info.inoGrp, 4, 1, f);
    fseek(f, 1040, SEEK_SET);
    fread (&info.freeIno, 4, 1, f);
    fseek(f, 1048, SEEK_SET);
    fread (&info.blkSize, 4, 1, f);
    info.blkSize = pow (2, 10 + info.blkSize);

    //Revisar aquest
    fseek(f, 1364, SEEK_SET);
    fread (&info.rsvBlk, 4, 1, f);

    fseek(f, 1036, SEEK_SET);
    fread (&info.freeBlk, 4, 1, f);
    fseek(f, 1028, SEEK_SET);
    fread (&info.totBlk, 4, 1, f);
    fseek(f, 1044, SEEK_SET);
    fread (&info.firstBlk, 4, 1, f);
    fseek(f, 1056, SEEK_SET);
    fread (&info.blkGrp, 4, 1, f);

    //Revisar aquest
    fseek(f, 1056, SEEK_SET);
    fread (&info.frgGrp, 4, 1, f);

    fseek(f, 1144, SEEK_SET);
    fread (&info.vlName, 16, 1, f);
    fseek(f, 1088, SEEK_SET);
    fread (&info.lstCheck, 4, 1, f);
    fseek(f, 1068, SEEK_SET);
    fread (&info.lstMount, 4, 1, f);
    fseek(f, 1072, SEEK_SET);
    fread (&info.lstWritten, 4, 1, f);
    fclose(f);
  }

  return info;
}

int EXT_magicNumber(FILE * f) {

  unsigned short ext;

  fseek(f, 1080, SEEK_SET);
  fread (&ext, 2, 1, f);
  if ( ext == 61267 ) return 1;

  return 0;
}

Fitxer EXT_journalFlag(FILE * f) {

  unsigned int flags;

  fseek(f, 1116, SEEK_SET);
  fread (&flags, 4, 1, f);
  flags = flags & 4;
  if (flags == 0) return EXT2;

  return NOFORMAT;
}

Fitxer EXT_extendFlag(FILE * f) {

  unsigned int flags;

  fseek(f, 1120, SEEK_SET);
  fread (&flags, 4, 1, f);
  flags = flags & 64;
  if (flags == 0) return EXT3;

  return NOFORMAT;
}

Fitxer EXT_format(FILE * f) {

  //Miram el flag de journal
  if (EXT_journalFlag(f) == 5){
    return EXT2;
  } else {

    //Miram el flag de extend
    if (EXT_extendFlag(f) == 4){
      return EXT3;
    } else {
      return EXT4;
    }
  }

  return NOFORMAT;
}
