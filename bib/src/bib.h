/*
 *	bib.h	
 */
/*   various arguments for bib and listrefs processors */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>

/* constants */

# define true  1
# define false 0
# define bool unsigned char
# define err  -1
# define REFSIZE 2048                /* maximum size of reference string    */
# define MAXFIELD 512                /* maximum size of any field in referece*/

/* reference citation marker genrated in pass 1 */

# define CITEMARK (char) 02
# define CITEEND  (char) 03
# define FMTSTART (char) 04
# define FMTEND   (char) 05

# define HUNTSIZE 512                /* maximum size of hunt string         */
# define MAXREFS  300                /* maximum number of references        */
# define MAXATONCE 35                /* maximum references at one location  */


/* file names */

        /* output of invert, input file for references */
# define INDXFILE "INDEX"
        /* pass1 reference collection file */
# define TMPREFFILE  "/tmp/bibrXXXXXX"
        /* pass2 text collection file */
# define TMPTEXTFILE "/tmp/bibpXXXXXX"
        /* temp file used in invert */
# define INVTEMPFILE "/tmp/invertXXXXXX"
# define SYSINDEX "/usr/dict/papers/INDEX"	/* default system dictionary */

#ifndef BASEDIR
# define BASEDIR   ""
# endif

# define N_BMACLIB "/lib/bmac"       /* where macro libraries live */
# define N_COMFILE "/lib/bmac/common"	/* common words */
# define N_DEFSTYLE "/lib/bmac/bib.stdsn" /* default style of refs */

# define InitDirectory(arr,str) \
   strcpy(arr,BASEDIR); strcat(arr,str);

extern char BMACLIB[100], COMFILE[100], DEFSTYLE[100];

/* size limits */

	/* maximum number of characters in common file */
# define MAXCOMM 1000

/* fix needed for systems where open [w]+ doesn't work */
# ifdef READWRITE

# define READ 1
# define WRITE 0

#endif
   /*
    *	Reference information
    */
   struct refinfo{
	char	*ri_ref;	/* actual value, base value */
	char	*ri_cite;	/* citation string */
	char	ri_disambig[2];	/* disambiguation string */
	int	ri_length;	/* length of reference string, plus null */
	long int ri_pos;	/* reference seek position */
	int	ri_n;		/* number of citation in pass1 */
	struct	refinfo	*ri_hp;	/* hash chain */
   };
   struct wordinfo{
	char	*wi_word;	/* actual word */
	char	*wi_def;	/* actual definition */
	int	wi_length;	/* word length */
	bool	wi_expanding;	/* is it being expanded? */
	struct wordinfo *wi_hp;	/* hash chain */
   };
#define reg register

int	strhash(reg char *str2);
void	rdref(struct refinfo *p, char ref[]);
void	wrref(struct refinfo *p, char ref[]);
void	dumpref(int i, FILE *ofd);
void	bldcite(char *cp, int i, char ref[]);
void	expand(char *line);
void	cleanup(int val);
void	error(char *str, ...);
void	bibwarning(char *msg, ...);
void	rdtext(FILE *fd);
void	rdcite(FILE *fd, char ch);
void	makecites(void);
void	disambiguate(void);
void	pass2(FILE *ifd, FILE *ofd);
void	incfile(char *np);
void	wordstuff(char *word, char *def);
void	wordrestuff(char *word, char *def);
struct wordinfo *wordsearch(char *word);
char	*walloc(char *word);
char	*hunt(char huntstr[]);
int	getref(char huntstr[]);
struct refinfo *refssearch(char *rf);
void	makekey(char *p, int max_klen, char *common);
void	load_comm(char *common);
int	lookup(char *list, char *p);
int	doargs(int argc, char **argv, char defstyle[]);
int newbibdir(char *name);
int citemark(char *info, char *huntstr, char *tail);
int addc(char huntstr[HUNTSIZE], char c);
int changefmt(FILE *ifd);

#define HASHSIZE	509

