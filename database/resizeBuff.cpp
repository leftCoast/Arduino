#include "resizeBuff.h"

bool resizeBuff(int numBytes,uint8_t** buff) {

  if(*buff) {
    free(*buff);
    *buff = NULL;
  }
  if (numBytes) {
    *buff = malloc(numBytres);
    return *buff != NULL;
  }
  return true;
}
