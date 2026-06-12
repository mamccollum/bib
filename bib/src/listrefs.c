/*
 * listrefs.c - List references for bib system
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "bib.h"
#include "streams.h"

#define MAXLIST 2000  /* maximum number of references that can be listed */
#define getch(c, fd) (c = getc(fd))

char BMACLIB[100], COMFILE[100], DEFSTYLE[100];

FILE *tfd;

#ifndef INCORE
FILE *rfd;                      /* reference file position */
char reffile[] = TMPREFFILE;    /* temporary file (see bib.h) */
#endif
struct refinfo refinfo[MAXLIST];      /* references temporary file, seek positions */
struct refinfo *refshash[HASHSIZE];
long int rend = 1;              /* last used position in reference file */
int numrefs = 0;               /* number of references */
extern int sort;                /* see if things are to be sorted */
extern char bibfname[];
extern int biblineno;
char *programName;

int rcomp(const void *ap, const void *bp);
void intr(void);

int
main(int argc, char **argv)
{
    char defult[120];
    int i;

    InitDirectory(BMACLIB, N_BMACLIB);
    InitDirectory(COMFILE, N_COMFILE);
    InitDirectory(DEFSTYLE, N_DEFSTYLE);

    signal(SIGINT, (void (*)(int))intr);
    programName = argv[0];
    tfd = stdout;
    strcpy(defult, BMACLIB);
    strcat(defult, "/bib.list");
#ifndef INCORE
    mktemp(reffile);
    rfd = fopen(reffile, "w+");
    if (rfd == NULL)
        error("can't open temporary reference file");
    putc('x', rfd);      /* put garbage in first position */
#endif

    doargs(argc, argv, defult);

    if (sort)
        qsort(refinfo, numrefs, sizeof(struct refinfo), rcomp);
    makecites();
    disambiguate();

    for (i = 0; i < numrefs; i++)
        dumpref(i, stdout);

    cleanup(0);
    return 0;
}

void
intr(void)
{
    cleanup(1);
}

void
cleanup(int val)
{
#ifndef INCORE
    fclose(rfd);
    unlink(reffile);
#endif
    exit(val);
}

/* rdtext - process a file */
void
rdtext(FILE *ifile)
{
    int c;
    char *p, rec[REFSIZE];
    int i;
    int hash, lg;

    biblineno = 1;
    for (;;) {
        getch(c, ifile);
        for (;;) {
            /* skip leading newlines and comments */
            if (c == '\n')
                getch(c, ifile);
            else if (c == '#')
                while (getch(c, ifile) != '\n' && c != EOF)
                    ;
            else
                break;
            biblineno++;
        }
        if (c == EOF)
            return;

        p = rec;          /* read a reference */
        for (;;) {
            for (*p++ = c; getch(c, ifile) != '\n';)
                if (c == EOF)
                    error("ill formed reference file");
                else
                    *p++ = c;
            /* at end-of-line */
            while (getch(c, ifile) == '#')
                while (getch(c, ifile) != '\n' && c != EOF)
                    ;
            if (c == '\n' || c == EOF) { /* if empty or eof */
                biblineno++;
                *p++ = '\n';
                break;
            }
            if (c == '.' || c == '%')
                *p++ = '\n';
            else
                *p++ = ' ';
        }

        *p = 0;
        expand(rec);

        /* didn't match any existing reference, create new one */
        if (numrefs >= MAXLIST)
            error("too many references, max of %d", MAXLIST);
        hash = strhash(rec);
        lg = strlen(rec) + 1;
        refinfo[numrefs].ri_pos = rend;
        refinfo[numrefs].ri_length = lg;
        refinfo[numrefs].ri_hp = refshash[hash];
        refinfo[numrefs].ri_n = numrefs;
        refshash[hash] = &refinfo[numrefs];
        wrref(&refinfo[numrefs], rec);
        numrefs++;
    }
}
