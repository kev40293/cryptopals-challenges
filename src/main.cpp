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
#include <iostream>

using namespace std;

int main(int argc, char** argv) {

   /*
   unsigned char iv[16];
   memset(iv, 0, 16);
   unsigned char* key = (unsigned char*) "YELLOW SUBMARINE";
   decryptFileCBC(key, iv, argv[1]);
   */

   auto s = profile_for("foo@bar.com");
   cout << s << endl;

   auto tokens = parseToken(s);
   for (auto p : tokens) {
      cout << p.first << " " << p.second << endl;
   }

   unsigned char key [16];
   memset(key, 10, 16);
   unsigned char iv [16];
   memset(iv, 10, 16);

   char * result;
   int len = encryptAESCBC(key, iv, (const unsigned char*) s.c_str(), (int) s.size(), (unsigned char**) &result);

   printBinHex(result, len);

   return 0;
}
