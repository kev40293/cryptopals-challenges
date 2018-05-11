#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
int editDistance(const char* first, const char* second, int length);

char * base64readFile(const char * filename, int *totalSize);

std::vector<std::pair<std::string, std::string>> parseToken(const std::string & token);
std::string profile_for(const std::string & profile);
std::string escape(const std::string & input);

#endif
