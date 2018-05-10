#include "xorTest.h"
#include "xorEnc.h"
#include "b64.h"
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include "aes.h"
#include "detectECB.h"
#include "utils.h"
#include "encOracle.h"
#include "breakECB.h"

int main(int argc, char** argv) {

   /*
   unsigned char iv[16];
   memset(iv, 0, 16);
   unsigned char* key = (unsigned char*) "YELLOW SUBMARINE";
   decryptFileCBC(key, iv, argv[1]);
   */
   const char * message = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

   char * binMessage;
   int mlen = b64tobin(message, strlen(message), &binMessage);

   //char input[256];
   //memset (input, 'K', 256);
   revealMessage(binMessage, mlen);

   free(binMessage);

   return 0;
}
