//
// Created by sus on 25.10.18.
//

#include "modes.h"
#include "memory.h"
#include <malloc.h>
#include <stdlib.h>

#include "file.h"

void runCommandChain(char** tokens, char filename[]){
    struct line* from;
    int r;

    switch (tokens[0][0]){
        //show
        case 's':
            if (tokens[1][0] == 'f'){
                showFile();
            } else if (tokens[1][0] == 'l'){
                if ((from = findLine(atoi(tokens[2]))) != NULL){
                    if (tokens[3] == NULL){
                        printf("%s", from->content);
                    } else{
                        if (tokens[3][0] == 's'){
                            if(!printLines(atoi(tokens[2]), 50)){
                                printf("Line exceeding limit\n");
                            }
                        } else{
                            if(!printLines(atoi(tokens[2]), atoi(tokens[3]))){
                                printf("Line exceeding limit\n");
                            }
                        }
                    }
                } else{
                    printf("Line exceeding limit\n");
                }

            } else{
                printf("Wrong argument for show\n");
            }
            break;

            //clear
        case 'c':
            clear();
            break;

            //delete
        case 'd':
            if (tokens[1][0] == 'l'){
                if (tokens[3] == NULL){
                    deleteLine(atoi(tokens[2]));
                } else{
                    r = deleteLines(atoi(tokens[2]), atoi(tokens[3]));

                    if (r == 0){
                        printf("This line is the last one. Cannot delete more.\n");
                    } else if(r == -1){
                        printf("Line exceeds limit\n");
                    } else if(r == -2){
                        printf("Line could not be found\n");
                    }
                }
            }
            break;

            //write
        case 'w':
            if(tokens[1] == NULL){
                r = writeFile(filename);
            } else{
                r = writeFile(tokens[1]);
            }

            if (r == 0){
                printf("Write failed\n");
            } else if (r == -1){
                printf("File could not be opened.\n");
            }
            break;

            //append
        case 'a':
            if (tokens[1] != NULL){
                if (tokens[1][0] == 'l'){
                    if (tokens[2] != NULL){
                        appendAfterLine(atoi(tokens[2]), tokens[3]);
                    } else{
                        printf("Too few arguments given.\n");
                    }
                } else if (tokens[1][0] == 'w'){
                    //TODO: append after word
                } else{
                    printf("Wrong argument given.\n");
                }
            } else {
                printf("Too few arguments given.\n");
            }
            break;

            //modes
        case 'm':
            if (tokens[1] != NULL){
                if (tokens[1][0] == 'b'){
                    browseMode();
                } else if (tokens[1][0] == 'a'){
                    if (tokens[2] != NULL){
                        appendMode(atoi(tokens[2]));
                    } else{
                        appendMode(0);
                    }
                }
            } else{
                printf("Too few arguments given.\n");
            }
            break;

        default:
            printf("Function not supported.\n");
            break;
    }
}

void runBrowseMode(char** tokens, int* curpos){

    switch (tokens[0][0]){
        case 'd':
            *curpos = (*curpos) + MINSCROLL;
            printLines(*curpos, STDTERM);
            break;

        case 'D':
            *curpos = (*curpos) + MAXSCROLL;
            printLines(*curpos, STDTERM);
            break;

        case 'u':
            *curpos = (*curpos) - MINSCROLL;
            printLines(*curpos, STDTERM);
            break;

        case 'U':
            *curpos = (*curpos) - MAXSCROLL;
            printLines(*curpos, STDTERM);
            break;

        case 'j':
            (*curpos)++;
            printLines(*curpos, STDTERM);
            break;

        case 'k':
            (*curpos)--;
            printLines(*curpos, STDTERM);
            break;

        case 'c':
            clear();
            break;
    }
}

int browseMode(){
    char in[MAXIN];
    char** tokens;

    int curpos = 0;

    while (1){
        printf("$");
        scanf("%s", in);
        printf("\n");

        if (in[0] == 'q'){
            return 0;
        }

        tokens = tokenize(in);
        runBrowseMode(tokens, &curpos);
    }
}

int appendMode(int after){
    char in[MAXLEN];

    while (1){
        printf("%%");
        scanf("%s", in);
        printf("\n");

        if (in[0] == 'q'){
            return 0;
        }

        appendAfterLine(after, in);
        after++;
    }
}

void showFile(){
    struct line* l = first;

    for(int i = 0; l != NULL; i++){
        printf("%d\t%s", i, l->content);
        l = l->next;
    }
}

int deleteLine(int nr){
    struct line* l;

    if ((l = findLine(nr))){
        if (l == first){
            l->next->prev = NULL;
        } else if (l == last){
            l->prev->next = NULL;
        } else{
            l->prev->next = l->next;
            l->next->prev = l->prev;
        }

        free(l);
    } else{
        return 0;
    }

    return 1;
}

int deleteLines(int from, int howmany){
    struct line* f;
    struct line* t;

    if ((f = findLine(from))){
        if (f == last){
            return 0;
        }

        t = f;

        for (int i = 0; i < howmany; ++i) {
            if (t->next != NULL){
                t = t->next;

                if (f != t->prev){
                    free(t->prev);
                }
            } else{
                return -1;
            }
        }

        if (f == first){
            first = t;
            t->prev = NULL;
        } else{
            f->prev->next = t;
            t->prev = f->prev;
        }

        free(f);
    } else{
        return -2;
    }

    return 1;
}

void appendAfterLine(int nr, char toappend[]){
    struct line* prev = findLine(nr);
    struct line* next = prev->next;

    struct line* new = malloc(sizeof(struct line));
    new->content = malloc(sizeof(char)*strlen(toappend));
    strcpy(new->content, strcat(toappend, "\n"));

    new->prev = prev;
    new->next = next;

    prev->next = new;
    next->prev = new;
}

