#ifndef ENC_ORACLE_H
#define ENC_ORACLE_H

#include <stdlib.h>
#include <stdio.h>
class encryptionOracle {
public:
   encryptionOracle() {
      for (int i = 0; i < 4; i++){
         ((int*)m_key)[i] = rand();
         ((int*)m_iv)[i] = rand();
      }
      m_prefixLen = rand() % 16;
      printf("plen %d\n", m_prefixLen);
      m_prefix = new char[m_prefixLen];
      for (int i = 0; i < m_prefixLen; i++){
         m_prefix[i] = rand() % 256;
      }
   }

   ~encryptionOracle() {
      delete [] m_prefix;
   }

   int encryptECBfuzzed(const char* input, int len, char** output);
   int encryptECBprefixed(const char* input, int len, char** output);
   int encryptRandom(const char *input, int len, char** output);
   int encryptCBCcomment(const char* input, int len, char** output);
   int decryptCBC(const char* input, int len, char** output);
private:
   char m_key[16];
   char m_iv[16];

   char * m_prefix;
   int m_prefixLen;

   char* prefixInput(const char* input, int len, int* newLen);
};

int encryptionOracleStandalone(char* input, int len, char** output);
void testECBCBC(int tries);

#endif
