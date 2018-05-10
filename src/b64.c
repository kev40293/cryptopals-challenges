#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "b64.h"
char char2byte(char byte) {
   switch(byte){
      case '1': return 1;
      case '2': return 2;
      case '3': return 3;
      case '4': return 4;
      case '5': return 5;
      case '6': return 6;
      case '7': return 7;
      case '8': return 8;
      case '9': return 9;
      case '0': return 0;
      case 'a': return 10;
      case 'b': return 11;
      case 'c': return 12;
      case 'd': return 13;
      case 'e': return 14;
      case 'f': return 15;
      case 'A': return 10;
      case 'B': return 11;
      case 'C': return 12;
      case 'D': return 13;
      case 'E': return 14;
      case 'F': return 15;
      default:
                return 0;
   }
}

char bits2b64(int bin) {
   if (bin < 26) {
      return bin + 'A';
   }
   else if (bin < 52) {
      return (bin - 26) + 'a';
   }
   else if (bin < 62 ){
      return (bin-52) + '0';
   }
   else {
      switch (bin) {
         case 62: return '+';
         case 63: return '/';
         default: return '\0';
      }
   }
}

char b64tobits(char b64) {
   switch (b64) {
      case '+': return 62;
      case '/': return 63;
      default:
                if (b64 >= 'a' ){
                   return b64 - 'a' + 26;
                }
                else if (b64 >= 'A') {
                   return b64 -'A';
                }
                else {
                   return b64-'0' + 52;
                }
   }
}

char * bin2b64(const char * bin, int len) {
   char * out = (char*) malloc(len*4/3 + 1);
   int pos = 0;
   for (int i = 0; i < len; i+=3) {
      out[pos] = bits2b64((bin[i] & 0xfc) >> 2 );
      if (i < len - 1) {
         out[pos+1] = bits2b64((bin[i] & 0x3) << 4) | ((bin[i+1] & 0xf0) >> 4);
      }
      else {
         out[pos+1] = bits2b64((bin[i] & 0x03) << 4);
         out[pos+2] = '=';
         out[pos+3] = '=';
         return out;
      }
      if (i < len - 2) {
         out[pos+2] = bits2b64(((bin[i+1] & 0x0f) << 2) | ((bin[i+2] & 0xc0) >> 6));
         out[pos+3] = bits2b64(((bin[i+2] & 0x3f)));
      }
      else {
         out[pos+2] = bits2b64((bin[i+1] & 0x0f) << 2);
         out[pos+3] = '=';
         return out;
      }
      pos+=4;
   }
   return out;
}

int b64tobin(const char * b64, int len, char ** binRes) {
   int bLen = len * 3 / 4;
   if (b64[len-2] == '=') bLen -= 2;
   else if (b64[len-1] == '=') bLen -= 1;

   char *bin = (char*) malloc(sizeof(char)* bLen);

   for (int i = 0; i < len/4; i++) {
      bin[i*3] = (b64tobits(b64[i*4]) << 2) | ((b64tobits(b64[i*4+1]) & 0x30) >> 4);
      if (bLen > i*3 +1)
         bin[i*3 + 1] = ((b64tobits(b64[i*4+1]) & 0x0f) << 4) | ((b64tobits(b64[i*4+2]) & 0x3c) >> 2);
      if (bLen > i*3 +2)
         bin[i*3 + 2] = ((b64tobits(b64[i*4+2]) & 0x03) << 6) | (b64tobits(b64[i*4+3]));
   }

   *binRes = bin;
   return bLen;
}

char * hex2bin(const char* hexString) {
   int len = strlen(hexString);
   char *out = (char*) malloc(len/2 + 1);
   for (int i = 0; i < len; i+=2) {
      out[i/2] = char2byte(hexString[i]) << 4 | char2byte(hexString[i+1]);
   }
   return out;
}
int b64encode (const char *bitString, int length) {
   char *buffer = hex2bin(bitString);
   char *b64 = bin2b64(buffer, length/2);
   free(buffer);
   free(b64);
   return 0;
}

void printBinHex(const char * bytes, int len) {
   for ( int i = 0; i < len; i++) {
      printf("%02x", (unsigned char)bytes[i]);
   }
   printf("\n");
}
