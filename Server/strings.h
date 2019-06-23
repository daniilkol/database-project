#ifndef STRINGS_H
#define STRINGS_H

#include <cstdio>
#include <cstring>
#include <string>
#include <stack>

int parse (char *string, const char *space, char **words);
int change (char *string, const char *substr, const char *new_substr);
int like (const char *examplar, const char *string);

#endif // STRINGS_H
