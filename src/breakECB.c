#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aes.h"
#include "breakECB.h"
#include "b64.h"

int detectBlockSize(char *input, int len) {
   unsigned char testBuffer[len];
   memcpy(testBuffer, input, len);

   unsigned char key[16];
   memset(key, 'A', 16);
   unsigned char* testEnc;
   int elen = encryptAESECB(key, (unsigned char*)testBuffer, len, &testEnc); // baseline with no changes ie bs = 0

   int bs = 1;
   do {
      unsigned char * nextEnc;
      memset(testBuffer, 0, bs); // set the first block to something known
      int e2len = encryptAESECB(key, testBuffer, len, &nextEnc); // Encrypt with next block size guess
      if (bs > 1 && memcmp(testEnc, nextEnc, bs-1) == 0) {
         free(testEnc);
         free(nextEnc);
         break;
      }
      free(testEnc);
      testEnc = nextEnc;

      bs ++;
   } while (true);

   return bs-1;
}

char discoverByte(char* known, char* input, int bSize) {
   unsigned char key[bSize];
   memset(key, 1, bSize);
   unsigned char eKnown[bSize];
   unsigned char pGuess[bSize];
   memcpy(pGuess, known, bSize-1);
   pGuess[bSize-1] = *input;
   unsigned char eGuess[bSize];
   printBinHex((const char*) pGuess, bSize);
   for (int c = 0; c < 256; c++) {
      known[bSize-1] = c;
      encryptAES(key, (unsigned char*)known, eKnown);
      encryptAES(key, (unsigned char*)pGuess, eGuess);
      if (memcmp(eKnown, eGuess, bSize) == 0){
         printf("Found char %c\n", c);
         break;
      }
   }
   return 0;
}

void revealMessage(char* input, int len) {
   // Detect block size
   int bSize = detectBlockSize(input, len);
   printf("%d\n", bSize);

   // Create space for message + the known prefix
   char* message = (char*) malloc(sizeof(char) * (len + bSize));
   memset(message, 'A', bSize);
   memset(message+bSize, 0, len);

   for (int i = 0; i < len; i++ ) {
      discoverByte(message+1+i, input+i, bSize);
      printBinHex(message, len+bSize);
   }
   printf("%s", message+bSize);

   free(message);
}

void breakChallenge() {
   const char * message = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

   char * binMessage;
   int mlen = b64tobin(message, strlen(message), &binMessage);

   //char input[256];
   //memset (input, 'K', 256);
   revealMessage(binMessage, mlen);

   free(binMessage);
}
