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

int main(int argc, char** argv) {

   /*
   unsigned char input[16];
   memcpy(input, argv[1], strlen(argv[1]));
   padBlock((char*)input, 16, strlen(argv[1]));
   printBinHex((char*)input, 16);
   unsigned char block[16];
   encryptAES((unsigned char*) "YELLOW SUBMARINE", input, block);
   printBinHex((char*)block, 16);
   */

   unsigned char iv[16];
   memset(iv, 0, 16);
   unsigned char* key = (unsigned char*) "YELLOW SUBMARINE";
   /*
   unsigned char* out;
   int eLen = encryptAESCBC((unsigned char*) "YELLOW SUBMARINE", iv, (unsigned char*)argv[1], strlen(argv[1]), &out);
   printBinHex((char*)out, eLen);

   unsigned char* dec;
   int dLen = decryptAESCBC((unsigned char*) "YELLOW SUBMARINE", iv, out, eLen, &dec);
   printf("%s\n", dec);
   free(out);
   free(dec);
   */
   decryptFileCBC(key, iv, argv[1]);

   return 0;
}
