#include <stdio.h>
#include <stdlib.h>

char szyfr[36][6];

struct trie{
    char l;
    struct trie *dot;
    struct trie *line;
}f;

struct trie *T;

void init();
