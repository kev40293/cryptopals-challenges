#include "detectECB.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "b64.h"

int maxRepeats(char * data, int len) {
   int max = 1;
   for (int i = 0; i < len/16; i++) {
      char * d = data+ (i*16);
      int count = 1;
      for (int j = i + 1; j < len/16; j++) {
         if (memcmp(d, data+(j*16), 16) == 0) {
            count++;
         }
      }
      if (count > max) {
         max = count;
      }
   }
   return max;
}

char * detectECB(char *filename) {

   FILE * dfile = fopen(filename, "r");
   char* buffer = NULL;
   size_t size = 0;
   ssize_t nread;
   int line = 0;
   while ((nread = getline(&buffer, &size, dfile)) > 0) {
      buffer[nread-1] = 0;
      char* bin = hex2bin(buffer);
      int m = maxRepeats(bin, (nread-1)/2);
      printf("%d on line %d\n", m, line);
      line++;
   }

   return NULL;
}
