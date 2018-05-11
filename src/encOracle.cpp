#include "aes.h"
#include "encOracle.h"
#include "b64.h"
#include "detectECB.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

char * fuzzInput(char *input, int length, int *newLen) {
   int front = 5 + (rand() % 5);
   int back = 5 + (rand() % 5);
   *newLen = length + front + back;
   char * newInput = (char*) malloc(sizeof(char)* (*newLen));
   memcpy(newInput+front, input, length);
   return newInput;
}

int encryptionOracle::encryptECB(char * input, int len, char** output ) {
   int newLen;
   char* fuzzedInput = fuzzInput(input, len, &newLen);
   return encryptAESCBC((unsigned char*) m_key, (unsigned char*) m_iv, (unsigned char*)fuzzedInput, newLen, (unsigned char**) output);
}

int encryptionOracle::encryptRandom(char* input, int len, char** output) {
   int r = rand();
   int alg = r % 2;

   int length;
   int newLen;
   char* fuzzedInput = fuzzInput(input, len, &newLen);
   switch(alg) {
      case 0:
         printf("Encrypting CBC\n");
         length = encryptAESCBC((unsigned char*) m_key, (unsigned char*) m_iv, (unsigned char*)fuzzedInput, newLen, (unsigned char**) output);
         break;
      case 1:
         printf("Encrypting ECB\n");
         length = encryptAESECB((unsigned char*) m_key, (unsigned char*) fuzzedInput, newLen, (unsigned char**) output);
         break;
   }
   return length;
}


int encryptionOracleStandalone(char* input, int len, char** output) {
   encryptionOracle oracle;
   return oracle.encryptRandom(input, len, output);
}

void testECBCBC(int tries) {
   srand(time(0));
   char test[256];
   for (int i=0; i<256; i++) {
      test[i] = i%16;
   }

   for (int i = 0; i < tries; i ++) {
      char *out;
      int len =  encryptionOracleStandalone(test, 256, &out);

      int nrep = maxRepeats (out, len);
      if (nrep > 2) printf("ECB detected\n");
      else printf("CBC detected\n");

      free (out);
   }
}
