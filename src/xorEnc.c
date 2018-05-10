#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "b64.h"
#include "xorEnc.h"

char * xorEncrypt(char key, char* buffer, int len) {
   char *out = (char*) malloc(sizeof(char) * len);
   for (int i = 0; i < len; i ++) {
      out[i] = buffer[i] ^ key;
   }
   return out;
}

char * xorRepeatEncrypt(const char *key, int keyLen, const char* buffer, int len) {
   char *out = (char*) malloc(sizeof(char) * len);
   for (int i = 0; i < len; i++) {
      out[i] = buffer[i] ^ key[i%keyLen];
   }
   return out;
}
