#include <stdio.h>
#include <stdlib.h>
#include "morse.h"

void init(){
    char alf[]={"abcdefghijklmnopqrstuvwxyz0123456789!"};
    char tmp[]={".- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. -- -. --- .--. --.- .-. ... - ..- ...- .-- -..- -.-- --.. ----- .---- ..--- ...-- ....- ..... -.... --... ---.. ----.!"};
    int ite=0;
    for(int i=0; i<36; i++){
        int j=0;
        while(tmp[ite]!=' '){
				if(tmp[ite]=='!')break;
            szyfr[i][j++]=tmp[ite++];
			}
        szyfr[i][j]='\0';
        ite++;
    }
    T=(struct trie*)calloc(sizeof(struct trie),1);
    ite=0;
    int j=0;
    struct trie *g=T;
    while(tmp[j]!='\0'){
        if(tmp[j]==' '){
            j++;
            continue;
        }
        if(tmp[j]=='.'){
            if(T->dot==NULL)
                T->dot=(struct trie*)calloc(sizeof(struct trie),1);
            T=T->dot;
        }
        else{
            if(T->line==NULL)
                T->line=(struct trie*)calloc(sizeof(struct trie),1);
            T=T->line;
        }
        if(tmp[j+1]==' ' || tmp[j+1]=='\0'){
            T->l=alf[ite++];
            T=g;
        }
        j++;
    }
}

