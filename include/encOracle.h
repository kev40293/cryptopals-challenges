#include <stdlib.h>
class encryptionOracle {
public:
   encryptionOracle() {
      for (int i = 0; i < 4; i++){
         ((int*)m_key)[i] = rand();
         ((int*)m_iv)[i] = rand();
      }
   }

   int encryptECB(char* input, int len, char** output);
   int encryptRandom(char *input, int len, char** output);
private:
   char m_key[16];
   char m_iv[16];
};

int encryptionOracleStandalone(char* input, int len, char** output);
void testECBCBC(int tries);
