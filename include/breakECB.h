#ifndef BREAK_ECB_H
#define BREAK_ECB_H
#include "encOracle.h"
void revealMessage(char* input, int len);

void breakECBoracle(encryptionOracle & oracle, unsigned char* message, int len);

#endif
