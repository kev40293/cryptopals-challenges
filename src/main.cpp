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
#include "padOracle.h"
#include <iostream>
#include "misc.h"
#include <assert.h>

using namespace std;

int main(int argc, char** argv) {

   printf("%d\n", ctrStrLen);

   encryptionOracle oracle;
   for (int i = 0; i < ctrStrLen; i++) {
      char *bin;
      char * output;
      char * orig;
      int blen = b64tobin(ctrStrings[i], strlen(ctrStrings[i]), &bin);
      int elen = oracle.encryptCTR(bin, blen, &output);
      int dlen = oracle.decryptCTR(output, elen, &orig);
      assert(blen == elen);
      assert(dlen == elen);
      assert (blen == dlen);
      printBinHex(output, elen);
      printBinHex(bin, blen);
      printBinHex(orig, dlen);
      free (bin);
      free(output);
   }
   return 0;
   padAttack();

   /*
   unsigned char iv[16];
   memset(iv, 0, 16);
   unsigned char* key = (unsigned char*) "YELLOW SUBMARINE";
   decryptFileCBC(key, iv, argv[1]);
   */
   srand(time(0));

   auto s = profile_for("foo@bar.com");
   cout << s << endl;

   auto tokens = parseToken(s, "&");
   for (auto p : tokens) {
      cout << p.first << " " << p.second << endl;
   }

   unsigned char key [16];
   memset(key, 10, 16);
   unsigned char iv [16];
   memset(iv, 10, 16);

   char * result;

   int len = oracle.encryptECBprefixed(s.c_str(), (int)s.size(), &result);

   printBinHex(result, len);

   free(result);
   breakECBoracle(oracle, (unsigned char*)s.c_str(), (int) s.size());

   cbcBitflip();

   return 0;
}
