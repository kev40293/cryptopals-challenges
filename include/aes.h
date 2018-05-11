#ifndef AES_H
#define AES_H

int encryptAES(const unsigned char * key, const unsigned char * buffer, unsigned char *out );
int decryptAES(const unsigned char * key, const unsigned char * buffer, unsigned char *out );

int encryptAESCBC(const unsigned char * key, const unsigned char* iv, const unsigned char * buffer, int inputLength, unsigned char **out);
int decryptAESCBC(const unsigned char * key, const unsigned char* iv, const unsigned char * buffer, int inputLength, unsigned char **out);

int encryptAESECB(const unsigned char * key, const unsigned char * buffer, int inputLength, unsigned char **out);
int decryptAESECB(const unsigned char * key, const unsigned char * buffer, int inputLength, unsigned char **out);

void padBlock(char* block, int bSize, int inputLength);
unsigned char * encryptECB(const unsigned char * key, const unsigned char* buffer, int len);

void decryptFileECB(const char* key, char* filename);
void decryptFileCBC(const unsigned char* key, const unsigned char* iv, char* filename);

bool validPadding(const unsigned char* message, int len);

void testOps(unsigned char* buffer);

#endif
