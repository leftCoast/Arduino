#include "resizeBuff.h"

bool resizeBuff(int numBytes,uint8_t** buff) {

  if(*buff) {
    free(*buff);
    *buff = NULL;
  }
  if (numBytes) {
    *buff = (uint8_t*)malloc(numBytes);
    return *buff != NULL;
  }
  return true;
}
