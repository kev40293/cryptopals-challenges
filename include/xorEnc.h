extern "C" {

char * xorEncrypt(char key, char* buffer, int len);
char * xorRepeatEncrypt(const char* key, int keyLen, const char* buffer, int len);
void breakRepeatXOR(char* filename);

}
