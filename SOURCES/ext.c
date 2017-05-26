/********************************************************************
*
* @Arxiu : ext.c
* @Finalitat : Conté la implementació de les funcions que permeten la gestió dels sistemes de fitxers EXT.
* @Autors : Esteve Genovard Ferriol - ls30742 & Sergi Simó Bosquet - ls30685
* @Data Creació: 26 d'abril del 2017
*
******************************************************************** */
#include "../HEADERS/ext.h"

InfoEXT4 EXT_getInfoEXT4 (char * fitxer) {

  InfoEXT4 info;
  unsigned int reservedBlocksLow;
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
    info.offset = 0;
    if(info.blkSize == 1024) info.offset = 1024;

    //Revisar aquest
    fseek(f, 1032, SEEK_SET);
    fread (&reservedBlocksLow, 4, 1, f);
    fseek(f, 1364, SEEK_SET);
    fread (&info.rsvBlk, 4, 1, f);
    info.rsvBlk = createAdressExt(info.rsvBlk, reservedBlocksLow);

    fseek(f, 1036, SEEK_SET);
    fread (&info.freeBlk, 4, 1, f);
    fseek(f, 1028, SEEK_SET);
    fread (&info.totBlk, 4, 1, f);
    fseek(f, 1044, SEEK_SET);
    fread (&info.firstBlk, 4, 1, f);
    fseek(f, 1056, SEEK_SET);
    fread (&info.blkGrp, 4, 1, f);
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

void EXT_findFile(char * volume, char * fileName, short optionAux, char * date) {

  unsigned int inodeTableAdressLow;
  unsigned long inodeTableAdress, direccio;
  short rootDirectory;
  InfoEXT4 info = EXT_getInfoEXT4(volume);
  FILE * f;

  option = optionAux;
  newDate = date;

  f = fopen(volume, "rb+");
  if (f == NULL) {

    ERROR_print(ERROR_OPENING_SYSTEM_FILE);
    ERROR_exit(-1);
  } else {

    direccio = info.offset + info.blkSize + 8;
    fseek(f, direccio, SEEK_SET);
    fread(&inodeTableAdressLow, 4, 1, f);
    fseek(f, 28, SEEK_CUR);
    fread(&inodeTableAdress, 4, 1, f);

    inodeTableAdress = createAdressExt(inodeTableAdress, inodeTableAdressLow);
    direccio =  inodeTableAdress * info.blkSize + info.inoSize;

    fseek(f, direccio, SEEK_SET);
    fread(&rootDirectory, 2, 1, f);

    if (rootDirectory & 0x4000) {
      fseek(f, 38, SEEK_CUR); //Ens movem al cap de l'arbre extend.
      flagTrobat = printing = 0;
      recorreArbre(f, fileName, info);
      if (!flagTrobat) ERROR_print(ERROR_FILE_NOT_FOUND);
    } else {
      fclose(f);
      ERROR_print(ERROR_ROOT_DIRECTORY);
      ERROR_exit(-1);
    }

    fclose(f);
  }
}

void recorreArbre(FILE * f, char * fileName, InfoEXT4 info) {

  unsigned short entries, depth;

  fseek(f, 2, SEEK_CUR); //Saltem informació no necessaria.
  fread(&entries, 2, 1, f); //Llegim número entrades.
  fseek(f, 2, SEEK_CUR); //Saltem informació no necessaria.
  fread(&depth, 2, 1, f); //Llegim profunditat que ens indica si és fulla.
  fseek(f, 4, SEEK_CUR); //Saltem informació no necessaria.

  if (!depth) llegeixNodesFulla(f, fileName, info, entries);
  else recorreNodesInterns(f, fileName, info, entries);
}

void recorreNodesInterns(FILE * f, char * fileName, InfoEXT4 info, unsigned short entries) {
  int i;
  unsigned long childAdress = 0, parentAdress;
  unsigned int childAdressLow;

  for (i = 0; i < entries; i++) {

    fseek(f, 4, SEEK_CUR); //Saltem informació no necessaria.
    fread(&childAdressLow, 4, 1, f); //Llegim adreça següent nivell del arbre.
    fread(&childAdress, 2, 1, f); //Llegim adreça següent nivell del arbre.
    fseek(f, 2, SEEK_CUR); //Saltem informació no necessaria.

    parentAdress = ftell(f);
    childAdress = createAdressExt(childAdress, childAdressLow);

    fseek(f, info.blkSize * childAdress, SEEK_SET); //Saltem al fill

    recorreArbre(f, fileName, info);

    fseek(f, parentAdress, SEEK_SET); //Tornem al pare
  }
}

void llegeixNodesFulla(FILE * f, char * fileName, InfoEXT4 info, unsigned short entries) {

  unsigned long dataBlockAdress = 0;
  unsigned long actualAdress = 0;
  unsigned int dataBlockAdressLow;
  short blockExtent, i;

  for (i = 0; i < entries; i++) {
    fseek(f, 4, SEEK_CUR); //Saltem el el blockNumber
    fread(&blockExtent, 2, 1, f);
    fread(&dataBlockAdress, 2, 1, f);
    fread(&dataBlockAdressLow, 4, 1, f);

    dataBlockAdress = createAdressExt(dataBlockAdress, dataBlockAdressLow);
    actualAdress = ftell(f);

    if (!printing) readDataBlock(f, fileName, info, dataBlockAdress);
    else showFileContent(f, info, dataBlockAdress);
    fseek(f, actualAdress, SEEK_SET);
  }
}

void readDataBlock(FILE * f, char * fileName, InfoEXT4 info, unsigned long dataBlockAdress) {

  unsigned int inode;
  unsigned long parentAdress;
  unsigned short length;
  unsigned char nameLength, entryType;
  char * name;

  fseek(f, info.blkSize * dataBlockAdress, SEEK_SET);

   do {

     fread(&inode, 4, 1, f);
     fread(&length, 2, 1, f);
     fread(&nameLength, 1, 1, f);
     fread(&entryType, 1, 1, f);

     name = (char *) malloc (sizeof(char) * (nameLength + 1));
     fread(name, 1, nameLength, f);
     name[nameLength] = '\0';
     //printf("%s\n", name);

     fseek(f, length - 8 - nameLength, SEEK_CUR);
     parentAdress = ftell(f);

     if (inode != 0 && entryType == 2 && strcmp(name, ".") && strcmp(name, "..")) recorreDirectori(f, fileName, info, inode);
     if (inode != 0 && entryType == 1 && !strcmp(name, fileName)) {
       manageFile(f, fileName, info, inode);
       flagTrobat = 1;
     }

     fseek(f, parentAdress, SEEK_SET);
   } while (inode != 0);
}

void recorreDirectori(FILE * f, char * fileName, InfoEXT4 info, unsigned int inodeNumber) {

  unsigned int groupNumber = (inodeNumber - 1) / info.inoGrp;
  unsigned int inodeOffset = ((inodeNumber - 1) % info.inoGrp) * info.inoSize;
  unsigned int inodeTableAdressLow;
  unsigned long inodeTableAdress;

  fseek(f, info.offset + info.blkSize + 8 + (groupNumber * 64), SEEK_SET);
  fread(&inodeTableAdressLow, 4, 1, f);

  fseek(f, 28, SEEK_CUR);
  fread(&inodeTableAdress, 4, 1, f);
  inodeTableAdress = createAdressExt(inodeTableAdress, inodeTableAdressLow);

  fseek(f, inodeTableAdress * info.blkSize + inodeOffset + 0x28, SEEK_SET);

  recorreArbre(f, fileName, info);
}

void manageFile(FILE * f, char * fileName, InfoEXT4 info, unsigned int inodeNumber) {

  unsigned int groupNumber = (inodeNumber - 1) / info.inoGrp;
  unsigned int inodeOffset = ((inodeNumber - 1) % info.inoGrp) * info.inoSize;
  unsigned int inodeTableAdressLow, fileSizeLow, creationTime;
  unsigned long inodeTableAdress, fileSize;
  unsigned short auxProperties;

  fseek(f, info.offset + info.blkSize + 8 + (groupNumber * 64), SEEK_SET);
  fread(&inodeTableAdressLow, 4, 1, f);

  fseek(f, 28, SEEK_CUR);
  fread(&inodeTableAdress, 4, 1, f);
  inodeTableAdress = createAdressExt(inodeTableAdress, inodeTableAdressLow);

  fseek(f, inodeTableAdress * info.blkSize + inodeOffset + 0x4, SEEK_SET);
  fread(&fileSizeLow, 4, 1, f);
  fseek(f, 100, SEEK_CUR);
  fread(&fileSize, 4, 1, f);
  fileSize = createAdressExt(fileSize, fileSizeLow);
  inodeSize = fileSize;

  fseek(f, 32, SEEK_CUR);
  fread(&creationTime, 4, 1, f);
  fseek(f, -108, SEEK_CUR);

  switch (option) {
    case 0:
      OUTPUT_searchEXT4(fileName, fileSize, creationTime);
      break;

    case 6:
      printing = 1;
      printf("File found! Showing content...\n\n");
      recorreArbre(f, fileName, info);
      printing = 0;
      break;

    case 1:
      fseek(f, -40, SEEK_CUR);
      fread(&auxProperties, 2, 1, f);
      auxProperties = (auxProperties & 0xFF00) | 0x124;
      fseek(f, -2, SEEK_CUR);
      fwrite(&auxProperties, 2, 1, f);

      printf("\nThe properties of %s have been modified!\n", fileName);
      break;

    case 2:
      fseek(f, -40, SEEK_CUR);
      fread(&auxProperties, 2, 1, f);
      auxProperties = auxProperties | 0xDB;
      fseek(f, -2, SEEK_CUR);
      fwrite(&auxProperties, 2, 1, f);

      printf("\nThe properties of %s have been modified!\n", fileName);
      break;

    case 5:
      fseek(f, 104, SEEK_CUR);
      creationTime = createDate();
      fwrite(&creationTime, 4, 1, f);
      printf("\nThe properties of %s have been modified!\n", fileName);
      break;
  }
}

void showFileContent(FILE * f, InfoEXT4 info, unsigned long dataBlockAdress) {

  unsigned int length = 0;
  char aux;

  fseek(f, info.blkSize * dataBlockAdress, SEEK_SET);

  while(length < info.blkSize && length < inodeSize) {
    fread(&aux, 1, 1, f);
    printf("%c", aux);
    length++;
  }
}

unsigned int createDate() {

  char dia[3], mes[3], any[5];
  short i, j = 0;
  struct tm timeStruct;
  time_t finalTime;

  for (i = 0; i < 2; i ++) dia[i] = newDate[i];
  dia[2] = '\0';
  for (i = 2; i < 4; i ++) {
    mes[j] = newDate[i];
    j++;
  }
  mes[2] = '\0';
  j = 0;
  for (i = 4; i < 9; i ++) {
    any[j] = newDate[i];
    j++;
  }
  any[4] = '\0';

  printf("%d, %d, %d\n", atoi(dia), atoi(mes), atoi(any));
  timeStruct.tm_year = atoi(any) - 1900;
  timeStruct.tm_mon = atoi(mes) - 1;
  timeStruct.tm_mday = atoi(dia);
  timeStruct.tm_min = 0;
  timeStruct.tm_sec = 0;
  timeStruct.tm_hour = 0;
  timeStruct.tm_isdst = -1;
  finalTime = mktime(&timeStruct);
  printf("%li\n", (long) finalTime);

  return (unsigned int) finalTime;
}

unsigned long createAdressExt(unsigned long adressHigh, unsigned int adressLow) {

  adressHigh = adressHigh << 32;
  adressHigh &= 0xFFFFFFFF00000000;
  adressHigh |= adressLow;
  return adressHigh;
}
