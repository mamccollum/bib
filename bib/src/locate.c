/*
 * locate.c - Find references in bibliographic database
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "streams.h"
#include "ctype.h"

#define maxrefs 1000

struct reftype {
    char reffile[maxstr];
    long int start, length;
};

/* char *calloc(); */
char *rindex();
char *stripkeys();
int fetchref();

/*  locate(keys, name, max_klen, common):
        Returns a string containing all references pointed to by name
        that contain all keys in keys.  Common is name of common word file.
    Pointer returned comes from calloc.  Use free to return storage.
    NB A zero length string returned if nothing is found.
       A NULL pointer indicates an error accessing the file "name".
*/
extern int fflag;  /* set if want the reference string to have the file name*/

char *
locate(char *keys, char *name, int max_klen, char *common)
{
    static char oldname[maxstr] = "";  /* oldname is name of stream index */
    static FILE *index = NULL;
    static long int i_size;            /* size of index                   */
    static char oldtext[maxstr];       /* oldtext is the path to stream   */
    static FILE *text = NULL;          /*  text.  if it is a relative     */
    static int pathlen;                /*  path, it is relative to index  */
                                       /*  directory.                     */
                                       /* oldname[0..pathlen-1] is index  */
                                       /*  directory                      */
    int len;
    char key[maxstr];                  /* refs[i] is a line of index for  */
    struct reftype refs[maxrefs];      /* all keys up to key              */

    int refcnt, copied, comp;          /* refcnt = # of refs               */
                                       /* copied = # of refs copied        */
                                       /* comp   = # of refs compared      */
    struct reftype ref;
    char str[maxstr];
    int more;

    long int ans;
    int i, j;
    unsigned total;
    char *allrefs, *next;              /* all refs (separated by null line)*/
    char *p;

    /*  open index */
    if (strcmp(oldname, name) != 0) {
        if (index)
            fclose(index);
        if (text)
            fclose(text);
        strcpy(oldname, name);
        strcpy(oldtext, "");
        /*  determine pathlen   */
        p = rindex(oldname, '/');
        if (p != NULL)
            pathlen = p - oldname + 1;
        else
            pathlen = 0;

        index = fopen(oldname, "r");
        if (index == NULL) {
            fprintf(stderr, "locate: cannot open %s\n", oldname);
            strcpy(oldname, "");
            return (NULL);
        } else {
            fseek(index, 0L, 2); /*  seeks last newline      */
            i_size = ftell(index);
        }
    }

    /*  load references to first key  */
    keys = stripkeys(keys, key, max_klen, common);
    if (*key == NULL) {
        fprintf(stderr, "locate: no keys for citation: %s\n", keys);
        allrefs = (char *)calloc(1, sizeof(char));
        if (allrefs == NULL) {
            fprintf(stderr, "locate: insufficient space for references\n");
            exit(1);
        }
        *allrefs = NULL;
        return (allrefs);
    }
    len = strlen(key);
    strcat(key, " ");
    alpha_seek(index, key, i_size, 0);
    key[len] = NULL; /*  strip blank off */

    refcnt = 0;
    fscanf(index, "%s ", str);
    if (strcmp(str, key) == 0) {
        str[0] = NULL;
        while (refcnt < maxrefs && fetchref(index, str, &ref)) {
            refs[refcnt] = ref;
            refcnt++;
        }
    }

    if (refcnt == maxrefs)
        fprintf(stderr,
            "locate: first key (%s) matched too many refs\n", key);

    /*  intersect the reference sets for remaining keys with first set */
    while (*keys != NULL) {
        keys = stripkeys(keys, key, max_klen, common);
        if (*key == NULL)
            continue;

        len = strlen(key);
        strcat(key, " ");
        alpha_seek(index, key, i_size, 0);
        key[len] = NULL;

        fscanf(index, "%s ", str);
        if (strcmp(str, key) != 0)
            refcnt = 0; /*  no matching refs */

        copied = 0;
        comp = 0;
        more = fetchref(index, str, &ref);
        while (comp < refcnt && more) {
            /*  ans= ref-refs[comp]    */
            ans = strcmp(ref.reffile, refs[comp].reffile);
            if (ans == 0)
                ans = ref.start - refs[comp].start;
            if (ans == 0)
                ans = ref.length - refs[comp].length;
            if (ans < 0)
                more = fetchref(index, str, &ref);
            if (ans == 0) {
                refs[copied] = refs[comp];
                comp++;
                copied++;
                more = fetchref(index, str, &ref);
            }
            if (ans > 0)
                comp++;
        }

        refcnt = copied;
    }

    total = 0;
    for (i = 0; i < refcnt; i++)
        total += refs[i].length;

    allrefs = (char *)calloc(total + 10 + refcnt, sizeof(char));
    if (allrefs == NULL) {
        fprintf(stderr, "locate: insufficient space for references\n");
        exit(1);
    }

    next = allrefs;
    for (i = 0; i < refcnt; i++) {
        if (fflag)
            sprintf(next, " %s %ld %ld", refs[i].reffile, refs[i].start,
                refs[i].length);
        fseek(text, refs[i].start, 0);
        for (j = 0; j < refs[i].length; j++) {
            *next = getc(text);
            next++;
        }
        *next = '\n';
        next++;
        *next = '\n';
        next++;
    }
    *next = NULL;
    return (allrefs);
}

char *
stripkeys(char *p, char *r, int max_klen, char *common)
{
    extern void makekey(char *p, int max_klen, char *common);
    for (; *p && (isspace(*p)); p++)
        ;
    for (; *p && (!isspace(*p)); )
        *r++ = *p++;
    *r = NULL;
    makekey(r = r - strlen(r), max_klen, common);
    return (p);
}

int
fetchref(FILE *stream, char *str, struct reftype *ref)
{
    char oldfile[maxstr];
    fscanf(stream, " :%s %ld/%ld", ref->reffile, &ref->start, &ref->length);
    return (1);
}
