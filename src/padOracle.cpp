const char *strings[] = {"MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc=" ,
"MDAwMDAxV2l0aCB0aGUgYmFzcyBraWNrZWQgaW4gYW5kIHRoZSBWZWdhJ3MgYXJlIHB1bXBpbic=" ,
"MDAwMDAyUXVpY2sgdG8gdGhlIHBvaW50LCB0byB0aGUgcG9pbnQsIG5vIGZha2luZw==" ,
"MDAwMDAzQ29va2luZyBNQydzIGxpa2UgYSBwb3VuZCBvZiBiYWNvbg==" ,
"MDAwMDA0QnVybmluZyAnZW0sIGlmIHlvdSBhaW4ndCBxdWljayBhbmQgbmltYmxl" ,
"MDAwMDA1SSBnbyBjcmF6eSB3aGVuIEkgaGVhciBhIGN5bWJhbA==" ,
"MDAwMDA2QW5kIGEgaGlnaCBoYXQgd2l0aCBhIHNvdXBlZCB1cCB0ZW1wbw==" ,
"MDAwMDA3SSdtIG9uIGEgcm9sbCwgaXQncyB0aW1lIHRvIGdvIHNvbG8=" ,
"MDAwMDA4b2xsaW4nIGluIG15IGZpdmUgcG9pbnQgb2g=" ,
"MDAwMDA5aXRoIG15IHJhZy10b3AgZG93biBzbyBteSBoYWlyIGNhbiBibG93" };
#define nStrings 10

#include <stdlib.h>
#include <string.h>
#include "encOracle.h"
#include "b64.h"
#include "aes.h"
#include "padOracle.h"
#include <time.h>

const char * randomString() {
   srand(time(0));
   int r = rand() % 10;
   return strings[r];
}

bool testDecrypt(encryptionOracle & oracle, char * message, int eLen) {
   char * output;
   int l = oracle.decryptCBC(message, eLen, &output);
   bool padding = validPadding((unsigned char*)output, l);
   free(output);
   return padding;
}

void padAttack() {
   const char * str = randomString();
   char * binstr;
   int len = b64tobin(str, strlen(str), &binstr);
   encryptionOracle oracle;
   char * encrypted;
   int eLen = oracle.encryptCBC(binstr, len, &encrypted);

   int nBlocks = eLen / 16;

   char * decrypted = (char *) malloc (sizeof(char) * eLen); // space for decrypted message
   memset(decrypted, 0, eLen);
   char * teststr = (char *) malloc ((sizeof(char) * eLen) + 16); // space for decrypted message
   const char * iv = oracle.iv();
   //printBinHex(encrypted, eLen);

   for (int n = 0; n < nBlocks; n++) {
      memcpy(teststr, iv, 16);
      memcpy(teststr+16, encrypted, eLen);
      for (int c = 1; c <= 16; c++) {
         bool yes = false;
         for (int i = 1; i < 256; i++) {
            if (n+1 == nBlocks) {
               for (int q = 1; q < c; q++) {teststr[eLen-q-(16*n)] = decrypted[eLen-q-(16*n)] ^ c ^ iv[16-q];
               }
               teststr[eLen-c-(16*n)] = iv[16-c] ^ i;
            }
            else {
               for (int q = 1; q < c; q++) {teststr[eLen-q-(16*n)] = decrypted[eLen-q-(16*n)] ^ c ^ encrypted[eLen-q-(16*n)-16];
               }
               teststr[eLen-c-(16*n)] = encrypted[eLen-16-(n*16)-c] ^ i;
            }
            yes = testDecrypt(oracle, teststr, eLen+16 - (n*16));
            if (yes) {
               printf("padding byte = %02x\n", i);
               printf("guess = %02x\n", c ^ i);
               printf("real = %02x\n", binstr[eLen - c - (n*16)]);
               decrypted[eLen-c-(16*n)] = c ^ i;
               break;
            }
         }
         if (!yes) {
            decrypted[eLen-c-(16*n)] = c;
         }
         printBinHex(decrypted, eLen);
      }
   }
   printBinHex(binstr, len);

   free(encrypted);
   free(binstr);
   free(decrypted);
   free(teststr);
}
