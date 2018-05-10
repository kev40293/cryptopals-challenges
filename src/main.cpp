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

int main(int argc, char** argv) {

   /*
   unsigned char iv[16];
   memset(iv, 0, 16);
   unsigned char* key = (unsigned char*) "YELLOW SUBMARINE";
   decryptFileCBC(key, iv, argv[1]);
   */
   testECBCBC(10);

   return 0;
}
