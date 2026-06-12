/*
 * makekey.c - Extract and process keywords for inverted indices
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bib.h"

char commlist[MAXCOMM] = "";
int firsttime = 1;

/*  makekey(p,max_klen,common):  compresses *p into a key
        folds upper to lower case.  ignores non-alphanumeric
        drops keys of length <= 1.
        drops words in common (name of file of words, one per line)
            (first call determines common for all later calls)
*/
void
makekey(char *p, int max_klen, char *common)
{
    register char *from, *to, *stop;

    if (firsttime) {
        firsttime = 0;
        load_comm(common);
    }

    from = p;
    to = p;
    stop = max_klen + p;
    while (*from != '\0' && to < stop) {
        if (islower(*from))
            *to++ = *from++;
        else if (isdigit(*from))
            *to++ = *from++;
        else if (isupper(*from)) {
            *to++ = tolower(*from);
            from++;
        } else
            from++;
    }
    *to = '\0';

    if (to <= p + 1 || lookup(commlist, p))
        *p = '\0';
}

/*  list is a string of null terminated strings, final string is null.
    p is a null terminated string.
    return 1 if p is a string in list, 0 ow.
*/
int
lookup(char *list, char *p)
{
    int len;
    len = strlen(list);
    while (len != 0 && strcmp(list, p) != 0) {
        list += (len + 1);
        len = strlen(list);
    }
    return (len != 0);
}

/*  read file common into commlist
*/
void
load_comm(char *common)
{
    FILE *commfile;
    char *p, *stop;
    commfile = fopen(common, "r");
    if (commfile == NULL)
        fprintf(stderr, "cannot open '%s'\n", common);
    else {
        /* read commfile into commlist  */
        p = commlist;
        stop = commlist + MAXCOMM - 1;
        while (p < stop && fgets(p, stop - p, commfile) != NULL) {
            while (*p != '\0')
                p++;
            p[-1] = '\0'; /* erase newline */
            p++;
        }
        *p = '\0';
        fclose(commfile);
    }
}
