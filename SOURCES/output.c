/********************************************************************
*
* @Arxiu : fat.c
* @Finalitat : Conté les capçaleres de les funcions per informar l'usuari.
* @Autors : Esteve Genovard Ferriol - ls30742 & Sergi Simó Bosquet - ls30685
* @Data Creació: 26 d'abril del 2017
*
******************************************************************** */
#include "../HEADERS/output.h"

void OUTPUT_searchFAT32 (ArchCercatFAT32 ACFAT32) {

    short dia = ACFAT32.data;
    short mes = ACFAT32.data;
    short any = ACFAT32.data;

    dia = dia & 0x1F;
    mes = mes >> 5;
    mes = mes & 0x0F;
    any = any >> 9;
    any = any & 0x7F;
    any = any + 1980;

    printf("\nFile found! Size: %d Bytes.  Created on: %d/%d/%d\n", ACFAT32.mida, dia, mes, any);
}

void OUTPUT_searchEXT4 (char * fileName, unsigned long size, unsigned int creationTime) {

  time_t timeSeconds = creationTime;
  struct tm finalTime;
  char buffer[80];

  finalTime = *localtime(&timeSeconds);
  strftime(buffer, 80, "%d/%m/%Y", &finalTime);

  printf("\nFile %s found! Size: %lu Bytes.  Created on: %s\n", fileName, size, buffer);
}

void OUTPUT_FAT32 (InfoFAT32 info) {

  printf("\n---- Filesystem Information ----\n\n");
  printf("Filesystem: FAT32\n\n");
  printf("System Name: %s\n", info.sysName);
  printf("Sector Size: %d\n", info.secSize);
  printf("Sectors per Cluster: %d\n", info.secXClou);
  printf("Reserved Sectors: %d\n", info.resSec);
  printf("Number of FATs: %d\n", info.numFat);
  printf("Maximum Root Entries: %d\n", info.rootE);
  printf("Sectors per FAT: %d\n", info.secXFat);
  printf("Label: %s\n\n", info.label);

}

void OUTPUT_EXT4 (InfoEXT4 info) {

  time_t timeSeconds;
  struct tm finalTime;
  char buffer[80];

  printf("\n---- Filesystem Information ----\n\n");
  printf("Filesystem: EXT4\n\n");
  printf("INODE INFO\n");
  printf("Inode Size: %d\n", info.inoSize);
  printf("Number of Inodes: %d\n", info.numIno);
  printf("First Inode: %d\n", info.firstIno);
  printf("Inodes Group: %d\n", info.inoGrp);
  printf("Free Innodes: %d\n\n", info.freeIno);

  printf("BLOCK INFO\n");
  printf("Block Size: %d\n", info.blkSize);
  printf("Reserved Blocks: %lu\n", info.rsvBlk);
  printf("Free Blocks: %d\n", info.freeBlk);
  printf("Total Blocks: %d\n", info.totBlk);
  printf("First Block: %d\n", info.firstBlk);
  printf("Block Group: %d\n", info.blkGrp);
  printf("Frags Group: %d\n\n", info.frgGrp);

  printf("VOLUME INFO\n");
  printf("Volume Name: %s\n", info.vlName);
  timeSeconds = info.lstCheck;
  finalTime = *localtime(&timeSeconds);
  strftime(buffer, 80, "%a %b %d %H:%M:%S %Y", &finalTime);
  printf("Last check: %s\n", buffer);
  timeSeconds = info.lstMount;
  finalTime = *localtime(&timeSeconds);
  strftime(buffer, 80, "%a %b %d %H:%M:%S %Y", &finalTime);
  printf("Last mount: %s\n", buffer);
  timeSeconds = info.lstWritten;
  finalTime = *localtime(&timeSeconds);
  strftime(buffer, 80, "%a %b %d %H:%M:%S %Y", &finalTime);
  printf("Last written: %s\n\n", buffer);
}

void OUTPUT_otherFormats (const char * const SFD) {

  printf("\n---- Filesystem Information ----\n\n");
  printf("Filesystem: %s\n\n", SFD);

}
