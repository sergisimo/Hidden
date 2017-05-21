/********************************************************************
*
* @Arxiu : fat.c
* @Finalitat : Conté la implementació de les funcions que permeten la gestió dels sistemes de fitxers FAT.
* @Autors : Esteve Genovard Ferriol - ls30742 & Sergi Simó Bosquet - ls30685
* @Data Creació: 26 d'abril del 2017
*
******************************************************************** */
#include "../HEADERS/fat.h"

Fitxer FAT_formatFat32 (FILE * f){

  unsigned int fat;
  unsigned short valueFat;

  fseek(f, 82, SEEK_SET);
  fread (&fat, 3, 1, f);

  if (fat == 5521734) {
    fread (&valueFat, 2, 1, f);

    if (valueFat == 12851) return FAT32;
  }

  return NOFORMAT;
}

EstructuraFAT32 FAT_getEstructura (FILE * f, InfoFAT32 info) {

  EstructuraFAT32 FAT32;

  FAT32.dataRegionMesCluster = (info.resSec + (info.numFat * info.secXFat)) * info.secSize;
  FAT32.dataRegion = (info.resSec + (info.numFat * info.secXFat)) * info.secSize;
  FAT32.fatTable = info.resSec * info.secSize;
  FAT32.nextCluster = 8;

  fseek(f,FAT32.fatTable + FAT32.nextCluster, SEEK_SET);
  fread (&FAT32.nextCluster, 4, 1, f);

  return FAT32;
}

void FAT_showInfo(char * fitxer) {

  char lletra;
  int i = 0;
  FILE * f;
  InfoFAT32 info;
  EstructuraFAT32 EFAT32;

  info = FAT_getInfoFAT32 (fitxer);
  int sizeCluster = info.secSize * info.secXClou;
  f = fopen(fitxer, "rb");

  if (f == NULL) {
    ERROR_print(ERROR_OPENING_SYSTEM_FILE);
    exit(-1);
  }
  else {
    printf("\nFile found! Showing content…\n\n");
    EFAT32 = FAT_getEstructura (f, info);
    //ens posicionem al Cluster que ens interesa.
    while (ACFAT32.cluster != 268435448 && ACFAT32.cluster != 268435455) {
    fseek(f, ACFAT32.direccio, SEEK_SET);

      do {

        fread (&lletra, 1, 1, f);
        printf("%c", lletra);
        i++;

      } while ( i < sizeCluster);

      //Llegir següent Cluster
      fseek(f,EFAT32.fatTable + ACFAT32.cluster * 4, SEEK_SET);
      fread (&ACFAT32.cluster, 4, 1, f);

      if (ACFAT32.cluster != 268435448 && ACFAT32.cluster != 268435455) { //Comprovem que el cluster tingui més entrades
        ACFAT32.direccio = ((ACFAT32.cluster - 2) * info.secXClou * info.secSize) + EFAT32.dataRegion;
      }

      i = 0;
    }

    printf("\n");
    fclose(f);
  }
}

void FAT_nomArchiuSenseVFATLN (EntryFAT32 entry, char paraula[150]) {

  int fi = strlen(entry.DName);
  int compt, index = 0;
  int flag = 1;

  do {
    if (entry.DName[compt] != ' ') {
      paraula[index] = entry.DName[compt];
      index++;
    }
    else {
      if (flag) {
        paraula[index] = '.';
        index++;
        flag = 0;
      }
    }

    compt++;

  } while (fi != compt);

  paraula[strlen(paraula)] = '\0';
}

ArchiuFAT FAT_nomArchiuVFATLN (EntryFAT32 entry, ArchiuFAT AFAT, FILE * f) {

    char lletra;

    int extensio = entry.DName[0] - 64;
    int fiVFAT = entry.DName[0];
    int i = 0;

    fseek(f, - 32, SEEK_CUR);

    for (i = 0; i < 32; i++) {
        fread (&lletra, 1, 1, f);

        if (i !=0 && i != 11 && i != 12 && i != 13 && i != 26 && i != 27) {
          if (lletra != (char) 0 && lletra != -1) {
            AFAT.paraula[AFAT.longitud] = lletra;
            AFAT.longitud++;
          }
        }
    }

    if (extensio == 1 || fiVFAT == 1) {
      AFAT.paraula[AFAT.longitud]='\0';
      AFAT.VFATLongNameFlag = 1;
      AFAT.longitud = 0;
    }

    return AFAT;
}

EstructuraFAT32 FAT_subdirectory (EntryFAT32 entry, InfoFAT32 info, EstructuraFAT32 EFAT32, FILE * f) {

  EstructuraFAT32 EFAT33;

  EFAT33.nextCluster = entry.FCHI + entry.FCLW;
  EFAT33.fatTable = EFAT32.fatTable;
  EFAT33.dataRegion = EFAT32.dataRegion;
  EFAT33.dataRegionMesCluster = ((EFAT33.nextCluster - 2) * info.secXClou * info.secSize) + EFAT33.dataRegion + 64;

  fseek(f,EFAT32.fatTable + EFAT33.nextCluster * 4, SEEK_SET);
  fread (&EFAT33.nextCluster, 4, 1, f);

  return EFAT33;
}

int FAT_CercaRootFile(FILE * f, InfoFAT32 info, EntryFAT32 entry, EstructuraFAT32 EFAT32, ArchiuFAT AFAT, int finalEntry) {

  unsigned char num = 0xE5;
  int t = 0;

  //ens posicionem al Cluster que ens interesa.
  fseek(f, EFAT32.dataRegionMesCluster, SEEK_SET);
  fread (&entry, 32, 1, f);

  do {
      if (entry.DName[0] == 0x05 || entry.DName[0] == num) {  //Entrada Eliminada
        printf("Entrada eliminada\n");
      }
      else {
        if (entry.Attrb == 0x0f) { //VFAT LONG NAME
          AFAT = FAT_nomArchiuVFATLN (entry, AFAT, f);
        }

        if (entry.Attrb == 0x10) { //SUBDIRECTORI
          EstructuraFAT32 EFAT33;

          EFAT33 = FAT_subdirectory(entry, info, EFAT32, f);
          AFAT.trobat = FAT_CercaRootFile(f, info, entry, EFAT33, AFAT, 13);

          fseek(f, EFAT32.dataRegionMesCluster + t * 32 + 32, SEEK_SET);
        }

        if (entry.Attrb == 0x20 && AFAT.VFATLongNameFlag == 0) { //ARCHIU sense VAFT
          char nomArchiu[150] = "";

          FAT_nomArchiuSenseVFATLN (entry, nomArchiu);


          if (!strcmp(nomArchiu,AFAT.file)) {
            strcpy(ACFAT32.NomArch,nomArchiu);
            ACFAT32.cluster = entry.FCHI + entry.FCLW;
            ACFAT32.direccio = ((ACFAT32.cluster - 2) * info.secXClou * info.secSize) + EFAT32.dataRegion;
            ACFAT32.mida =entry.FSize;
            ACFAT32.data = entry.CDate;
            //printf("\nFile found! Size: %d.  Created on: %d\n", entry.FSize, entry.CDate);
            AFAT.trobat = 1;
          }
        }

        if (entry.Attrb == 0x20 && AFAT.VFATLongNameFlag == 1) { //ARCHIU VFAT
          if (!strcmp(AFAT.paraula,AFAT.file) && entry.Attrb == 0x20) {
            strcpy(ACFAT32.NomArch,AFAT.paraula);
            ACFAT32.cluster = entry.FCHI + entry.FCLW;
            ACFAT32.direccio = ((ACFAT32.cluster - 2) * info.secXClou * info.secSize) + EFAT32.dataRegion;
            ACFAT32.mida =entry.FSize;
            ACFAT32.data = entry.CDate;
            //printf("\nFile found! Size: %d.  Created on: %d\n", entry.FSize, entry.CDate);
            AFAT.trobat = 1;
          }

          AFAT.VFATLongNameFlag = 0;
        }
      }

      fread (&entry, 32, 1, f);
      t++;
    } while (t <= finalEntry);

    if (EFAT32.nextCluster != 268435448 && EFAT32.nextCluster != 268435455) { //Comprovem que el cluster tingui més entrades
      EFAT32.dataRegionMesCluster = ((EFAT32.nextCluster - 2) * info.secXClou * info.secSize) + EFAT32.dataRegion;

      fseek(f,EFAT32.fatTable + EFAT32.nextCluster * 4, SEEK_SET);
      fread (&EFAT32.nextCluster, 4, 1, f);

      AFAT.trobat = FAT_CercaRootFile(f, info, entry, EFAT32, AFAT, 15);
    }

    return AFAT.trobat;
}

ArchiuFAT FAT_inicialitzaValors(char * file) {
  ArchiuFAT AFAT;
  AFAT.trobat = 0;
  AFAT.VFATLongNameFlag = 0;
  AFAT.file = file;
  AFAT.longitud = 0;

  return AFAT;
}

int FAT_findFileOnRoot(char * fitxer, char * file) {

  FILE * f;
  InfoFAT32 info;
  EntryFAT32 entry;
  ArchiuFAT AFAT;
  EstructuraFAT32 EFAT32;

  info = FAT_getInfoFAT32 (fitxer);
  AFAT = FAT_inicialitzaValors(file);

  f = fopen(fitxer, "rb");

  if (f == NULL) {
    ERROR_print(ERROR_OPENING_SYSTEM_FILE);
    exit(-1);
  }
  else {
    EFAT32 = FAT_getEstructura (f, info);

    if (!FAT_CercaRootFile(f, info, entry, EFAT32, AFAT, 15)) {
      fclose(f);
      ERROR_print(ERROR_FILE_NOT_FOUND);
      exit(-1);
    }
    fclose(f);
  }

  return 0;
}

InfoFAT32 FAT_getInfoFAT32 (char * fitxer) {

  InfoFAT32 info;
  FILE * f;

  f = fopen(fitxer, "rb");

  if (f == NULL) {
    ERROR_print(ERROR_OPENING_SYSTEM_FILE);
    exit(-1);
  }
  else {
    fseek(f, 3, SEEK_SET);
    fread (&info.sysName, 8, 1, f);
    fseek(f, 11, SEEK_SET);
    fread (&info.secSize, 2, 1, f);
    fseek(f, 13, SEEK_SET);
    fread (&info.secXClou, 1, 1, f);
    fseek(f, 14, SEEK_SET);
    fread (&info.resSec, 2, 1, f);
    fseek(f, 16, SEEK_SET);
    fread (&info.numFat, 1, 1, f);
    fseek(f, 17, SEEK_SET);
    fread (&info.rootE, 2, 1, f);
    fseek(f, 36, SEEK_SET);
    fread (&info.secXFat, 4, 1, f);
    fseek(f, 44, SEEK_SET);
    fread (&info.RFCluster, 4, 1, f);
    fread (&info.label, 11, 1, f);

    fclose(f);
  }

  return info;
}

Fitxer FAT_formatFat1216 (FILE * f) {

  unsigned int fat;
  unsigned short valueFat;

  fseek(f, 54, SEEK_SET);
  fread (&fat, 3, 1, f);

  if ( fat == 5521734) {
    fread (&valueFat, 2, 1, f);

    if (valueFat == 13873) return FAT16;
    if (valueFat == 12849) return FAT12;
  }

  return NOFORMAT;
}

Fitxer FAT_format(FILE * f) {

  if (FAT_formatFat32(f) == 0 ) return FAT32;
  else if (FAT_formatFat1216(f) == 1 ) return FAT16;
  else if (FAT_formatFat1216(f) == 2 ) return FAT12;

  return NOFORMAT;
}
