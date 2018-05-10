extern "C" {

int b64tobin(const char* b64, int len, char** bin);

char * bin2b64(const char * bin, int len);

char * hex2bin(const char * hexString);

int b64encode(const char *bitString, int length);

void printBinHex(const char* bytes, int len);

}
