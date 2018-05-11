#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aes.h"
#include "breakECB.h"
#include "b64.h"
#include "detectECB.h"

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

int detectPrefixLength(encryptionOracle & oracle) {
   char testBuff[256];
   memset(testBuff, 10, 256);

   char *encrypted;
   int len = oracle.encryptECBprefixed(testBuff, 256, &encrypted);
   int rep = maxRepeats(encrypted, len);
   free(encrypted);
   int pGuess = 0;
   int nRep = rep;
   do {
      testBuff[pGuess] = '0';
      len = oracle.encryptECBprefixed(testBuff, 256, &encrypted);
      nRep = maxRepeats(encrypted, len);
      free (encrypted);
      pGuess++;
   } while( nRep == rep);
   return 16- (pGuess-1);
}

void breakECBbytes(encryptionOracle & oracle, unsigned char *message, int mLen, int pLen, int bSize) {
   int tBufLen = bSize*2-pLen;
   unsigned char tBuf[tBufLen];
   memset(tBuf, 10, tBufLen);

   char decMessage[tBufLen+mLen]; // Revealed message
   memcpy(decMessage, tBuf, tBufLen); // Set our known values

   char realMessage[tBufLen+mLen];
   memcpy(realMessage, tBuf, tBufLen);
   memcpy(realMessage+tBufLen, message, mLen);

   for (int mPos = 0; mPos < mLen; mPos++) { //Break each character
      for (int c = 0; c < 256; c++) {
         decMessage[tBufLen+mPos] = c; // Set our character guess

         char* encGuess;
         char* encReal;

         int gLen = oracle.encryptECBprefixed(decMessage + 1 + mPos, mLen+tBufLen - mPos, &encGuess);
         int rLen = oracle.encryptECBprefixed(realMessage + mPos, mLen+tBufLen-mPos, &encReal);

         if (memcmp(encGuess+bSize, encReal+bSize, bSize) == 0){
            free(encGuess);
            free(encReal);
            break;
         }
         free(encGuess);
         free(encReal);
      }
   }
   printf("%s\n", decMessage+tBufLen -1);
}

//Get the offset in case prefix is multiple blocks
int prefixBytes(encryptionOracle & oracle) {
   char buffer[256];
   memset(buffer, 'A', 256);

   char * base;
   int len = oracle.encryptECBprefixed(buffer, 256, &base);
   char * change;
   buffer[0]='B';
   oracle.encryptECBprefixed(buffer, 256, &change);

   int pBytes = 0;
   for (int i = 0; i < len; i++ ){
      if ( base[i] == change[i] ) {
         pBytes = i;
         break;
      }
   }
   free(base);
   free(change);
   return pBytes;
}

int detectBlockSize(encryptionOracle & oracle) {
   int initOffset = prefixBytes(oracle);
   char buffer[256];
   memset(buffer, 'A', 256);

   char * base;
   int len = oracle.encryptECBprefixed(buffer, 256, &base);
   char * change;
   buffer[0]='B';
   oracle.encryptECBprefixed(buffer, 256, &change);

   int bEnd = 0;
   for (int i = len; i >= initOffset; i-- ){
      if ( base[i] == change[i] ) {
         bEnd = 0;
         break;
      }
   }
   return bEnd = initOffset;

}

void breakECBoracle(encryptionOracle & oracle, unsigned char* message, int len) {
   int pLen = detectPrefixLength(oracle);
   printf("Detected prefix len %d\n", pLen);
   int bSize = detectBlockSize(oracle);
   printf("Block size %d\n", bSize);

   breakECBbytes(oracle, message, len, pLen, bSize);
}
