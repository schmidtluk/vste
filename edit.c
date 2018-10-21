//
// Created by tyroc on 21.10.2018.
//

#include <stdio.h>
#include <mem.h>
#include <malloc.h>

#include "edit.h"

void insert(int nr, char buf[]){
    struct line* l = (struct line*) malloc(sizeof(struct line));
    l->content = malloc(sizeof(char)*strlen(buf));
    strcpy(l->content, buf);

    if (first == NULL){
        l->prev = NULL;
        l->next = NULL;
        first = l;
        last = l;

    } else{
        l->prev = last;
        l->next = NULL;
        last->next = l;
        last = l;
    }
}

void readFile(char* filename){
    FILE *fp = fopen(filename, "r");
    char buf[MAXLEN];

    for (int i = 0; fgets(buf, BUFSIZ, fp); i++){
        insert(i, buf);
    }
}

void showFile(){
    struct line* l = first;

    for(int i = 0; l != NULL; i++){
        printf("%d %s", i, l->content);
        l = l->next;
    }
}

