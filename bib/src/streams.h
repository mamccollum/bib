/*
 *	streams.h - Stream processing function declarations
 */

#ifndef STREAMS_H
#define STREAMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Function prototypes */
long int nextrecord(FILE *stream, long int x);
long int recsize(FILE *stream, long int start);
long int nextline(FILE *stream, long int x);
void printline(FILE *stream);
void mygetline(FILE *stream, char *p);
void getword(FILE *stream, char *p, char *ignore, int *bolp);
void strreplace(char *subj, char *old, char *new);
int foldline(char *p);
int alpha_seek(FILE *stream, char *word, long int s_size, int fold);

# define  maxstr            256
# define  pos(x)            fseek(stream,x,0)

#endif /* STREAMS_H */
