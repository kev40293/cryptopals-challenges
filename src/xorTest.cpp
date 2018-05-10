#include "b64.h"
#include "xorEnc.h"
#include "xorTest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include "utils.h"
#include "sys/stat.h"

const char LETTERS[] = {
   'E', 'T', 'A', 'O', 'I', 'N', ' ', 'S', 'H', 'R', 'D', 'L', 'U' };
const char LETTERS_LOWER[] = {
   'e', 't', 'a', 'o', 'i', 'n', ' ', 's', 'h', 'r', 'd', 'l', 'u' };
#define NCHARS 13
//const char LETTERS[] = {
   //'E', 'T', 'A', 'O', 'I', 'N', 'S', 'H', 'R', 'D', 'L', 'U' };
//const char LETTERS_LOWER[] = {
   //'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd', 'l', 'u' };
//#define NCHARS 12

int score(char * buffer, int len) {
   int sc = 0;
   for (int i = 0; i < len; i ++) {
      if (buffer[i] >= 'a' && buffer[i] <= 'z') sc++;
      if (buffer[i] >= 'A' && buffer[i] <= 'Z') sc++;
      if (buffer[i] >= '0' && buffer[i] <= '9') sc++;
      if (buffer[i] == ' ') sc++;
   }
   return sc;
}

int score2(char* buffer, int len){
   int sc = 0;
   for (int i = 0; i < len; i ++) {
      for (int s = 0; s < NCHARS; s ++) {
         if (buffer[i] == LETTERS[s]) sc += NCHARS-s;
         else if (buffer[i] == LETTERS_LOWER[s]) sc += NCHARS-s;
      }
   }
   return sc;
}

struct cand* findMostLikely(char * buffer, int len) {
   char * candidate;
   int max = 0;
   struct cand* res = (struct cand*) malloc(sizeof(struct cand));
   res->decrypted = NULL;
   for (int i = 0; i < 256; i++) {
      char *c = xorEncrypt((char)i, buffer, len);
      int tmp = score2(c, len);// + score(c, len);
      if (tmp > max) {
         max = tmp;
         candidate = c;
         res->val = i;
         if (res->decrypted != NULL) {
            free(res->decrypted);
         }
         res->decrypted = c;
         res->score = tmp;
         res->next = NULL;
      }
      else {
         free(c);
      }
   }
   //printf("%d :%s\n", max, candidate);
   //free(candidate);
   return res;
}

struct cand* tryLine(char* line, int length) {
   char *b1 = hex2bin(line);
   struct cand* res = findMostLikely(b1,length/2);
   free(b1);
   return res;
}

void findMostLikelyInList(struct cand* first) {
   struct cand* cur = first;
   struct cand* bestCand = first;
   while (cur != NULL) {
      if (cur->score > bestCand->score) {
         bestCand = cur;
      }
      cur = cur->next;
   }
   printf("Best score %d result %s with key %02x\n", bestCand->score, bestCand->decrypted, bestCand->val);
}

void freeList(struct cand * first) {
   while (first != NULL){
      struct cand* old = first;
      first = first->next;
      free(old->decrypted);
      free(old);
   }
}

void findMostLikelyInFile (char* fileName) {
   FILE *inputs = fopen(fileName, "r");
   char *buffer = NULL;
   size_t size = 0;
   struct cand ** all = (struct cand**) malloc(sizeof(struct cand*));
   *all = NULL;
   while ((getline(&buffer, &size, inputs)) > 0) {
      int end = strlen(buffer);
      struct cand * lineCand = tryLine(buffer, end);
      lineCand->next = *all;
      *all = lineCand;
      size = 0;
      free (buffer);
      buffer = NULL;
   }
   findMostLikelyInList(*all);
   freeList(*all);
   free(all);
}

std::vector<std::pair<int,double>> deriveKeySizes(const char * filename, int min, int max) {
   std::vector<std::pair<int,double>> keySizeScores;
   int size;
   char *fileData = base64readFile(filename, &size);
   for (int k = min; k <= max; k++) {
      char buff1[k];
      char buff2[k];

      double sum = 0;
      int samples = 4;
      for (int s = 0; s < samples; s++) {
         memcpy (buff1, fileData+(k*s), k);
         memcpy (buff2, fileData+(k*(s+1)), k);

         sum += editDistance(buff1, buff2, k);
      }
      double avg = sum / samples / k;
      keySizeScores.push_back(std::pair<int, double>(k, avg));
   }
   std::sort(keySizeScores.begin(), keySizeScores.end(), [] (std::pair<int, double> a, std::pair<int,double> b) {
         return a.second < b.second;
         });
   free(fileData);
   return keySizeScores ;
}

// kSize is the number of rows to break
// nBlocks is the length of each row
char ** createChunks(const char * data, int kSize, int nBlocks, int totalSize) {
   char ** blocks = (char**) malloc(sizeof(char*) * (kSize));
   for (int i = 0; i < kSize; i++) {
      blocks[i] = (char*) malloc (sizeof(char)*nBlocks);
   }
   for (int i = 0; i < nBlocks; i++) {
      for (int j = 0; j < kSize; j++) {
         // get the i'th byte from the j'th block
         // j is key index, i is block index
         if (i*kSize+j >= totalSize)
            blocks[j][i] = 0;
         else
            blocks[j][i] = data[ (i*kSize) +j];
      }
   }
   return blocks;
}

void printreverse(struct cand* list, int pos) {
   if (list == NULL) return;
   printreverse(list->next, pos);
   printf("%c", list->decrypted[pos]);
}
char ** interpolateChunks(const char * filename, int blockSize) {
   struct stat fs;
   stat(filename, &fs);
   int filesize = fs.st_size;
   int nBlocks = filesize * 3/4 / blockSize;
   int nread;
   char * filedata = base64readFile(filename, &nread);
   printf("filesize = %d\n", filesize) ;
   printf("nBlocks = %f\n", filesize *3 / 4.0 / blockSize);
   auto blocks = createChunks(filedata, blockSize, nBlocks, nread);
   struct cand ** all = (struct cand**) malloc(sizeof(struct cand*));
   *all = NULL;
   for (int q = 0; q < blockSize; q++ ){
      struct cand * res = findMostLikely(blocks[q], nBlocks);
      printf("%s\n", res->decrypted);
      res->next = *all;
      *all = res;
   }
   struct cand* cur = *all;
   printf("Key: ");
   while (cur != NULL) {
      printf("%02x", cur->val);
      cur = cur->next;
   }
   printf("\n");
   for (int i = 0 ; i < nBlocks; i ++ ) {
      struct cand* cur = *all;
      printreverse(cur, i);
   }
   free(filedata);
   freeList(*all);
   free(all);
   return blocks;
}

void breakRepeatXOR(char* filename) {
   printf("%d\n", editDistance("this is a test", "wokka wokka!!!", 15));
   auto keyOrder = deriveKeySizes(filename, 2, 40);
   for (auto p : keyOrder) {
      printf("%d %f\n", p.first, p.second);
   }
   printf("Guessed keySize = %d\n", keyOrder.front().first);
   for (int x = 0 ; x < 3; x ++) {
      printf("Key size: %d\n", keyOrder.at(x).first);
      char ** blocks = interpolateChunks(filename, keyOrder.at(x).first);
      for (int k = 0; k < keyOrder.at(x).first; k++) {
         free(blocks[k]);
      }
      free (blocks);
   }
}
