struct cand {
   char val;
   int score;
   char * decrypted;
   struct cand * next;
};

struct cand * findMostLikely(char * line, int length);
void findMostLikelyInFile(char* fileName);
void findMostLikelyInList(struct cand * first);
void freeList(struct cand * first);
