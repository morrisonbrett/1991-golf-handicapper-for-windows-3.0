/*      CURSIVE SIGNATURE PROGRAM       Version 0.10
 *              (c) 1985 - Jan Wolter
 *
 *  Purpose:    This program translates text into crude cursive script.
 *      It works on any terminal with a fairly normal character
 *      set (must have backslashes and underscores and suchlike).
 *      The font includes all upper and lower case letters, most
 *      punctuation, and a few symbols.  No numbers are included
 *      (It's difficult to make a nice italicized set of numbers).
 *      Cursive was originally written to generate fancy signatures
 *      for electronic mail messages, but other uses may occur to
 *      you.  The attractiveness of the font varies greatly with
 *      the display device.
 *
 *  Usage:  If no command line arguments are given, cursive reads the
 *      text to translate from standard input.  Otherwise the args
 *      are translated (e.g. "cursive Jan Wolter" prints my name).
 *      Output is always to standard output.  A couple command line
 *      arguments are recognized:
 *
 *        -in   Sets the amount of space to insert between letters.
 *          The default is "-i1".  "-i0" gives interesting
 *          results.
 *        -tn   Sets the length of the trailing line off the end
 *          of each word.  The default is "-t1".
 *
 *      One character in the text is treated in a special way:
 *
 *        '_'   Can be inserted in text to slightly lengthen the
 *          the spacing between two letters, or add a longer
 *          tail to the end of a word.
 *
 *  Internals:  Unfortunately, the program is a kludge and the font is
 *      somewhat difficult to edit.  It should be easy to port
 *      though.  Systems with short integers or unsigned characters
 *      should be no problem.  You should probably run "xstr" on the
 *      font.c file, but if you haven't got "xstr", just compiling it
 *      the usual way works fine.
 *
 *  Copyright:  Both the cursive program and the font it generates are
 *      copyrighted by yours truly.  However, both may be used
 *      and distributed freely.  You even have my permission to
 *      sell it, or include in it a system that you sell.  I only
 *      ask that my name be retained on this program, or any direct
 *      decendents of this program with approximately the same
 *      visibility as in this posting.
 *
 *  Mail:   I'm interested in any comments you have on this program.
 *      I can be mailed at "janc@crim.eecs.umich.edu".  Better yet,
 *      if you are into teleconferencing, call M-Net at (313) 994-6333
 *      and type "newuser" at the "login:" prompt to give yourself an
 *      account.  Send mail to janc, or join the "ugames" conference.
 *
 *      Have fun,
 *                ___                _     __  _
 *               (   >              ' )   /   // _/_
 *                __/___.  ____      / / / __|/  /  _  __
 *               / / (_/|_/ / <__   (_(_/ (_) \_<__</_/ (__
 *              <_/
 */

#include "a:stdio.h"
#include "cursive.h"

char *buffer[6];    /* memory buffers to build up line in */
int c[6];       /* current index in each of the buffer lines */
int tail[6];        /* which buffer lines have tails in them */
int lasttail;       /* which line was the last letter's tail */
int space;      /* how much white space before the next letter */
int interspace = 1; /* how much to spread adjacent letters out */
int taillen = 1;    /* how long the tails on ends of words should be */
char firstletter;   /* is this the first letter on the line? */
char message[256] = ""; /* message to print */

char *malloc();

/* These are used by atoi */

#define TAB             '\t'
#define SP              ' '
#define PLUS            '+'
#define MINUS           '-'     /* Redundant, but clearer */
#define LEASTN          '0'
#define MOSTN           '9'


main(argc,argv)
int argc;
char **argv;
{
int i;
char *s,*m;
char ch;

    m = message;
    for (i = 1; i < argc; i++)
    {
        if (*argv[i] == '-')
            switch(ch = argv[i][1])
            {
            case 'i':
            case 't':
                s = argv[i]+2;
                if (*s == '\000')
                    if (++i < argc)
                        s = argv[i];
                if (*s < '0' || *s > '9')
                {
                    printf("%s: Illegal value %s\n",
                        argv[0],s);
                    exit(1);
                }
                if (ch == 'i')
                    interspace = atoi(s);
                else
                    taillen = atoi(s);
                break;
            default:
                printf("usage: %s [-tn] [-in] message\n",
                    argv[0]);
                exit(1);
            }
        else
        {
            if (m != message)
                *(m++) = ' ';
            for (s=argv[i]; *s != '\000'; s++)
                *(m++) = *s;
        }
    }
    /* Do the deed */
    if (m != message)
    {
        /* Message from the arg list */
        *(m++) = 0;
        prline(message);
    }
    else
    {
        /* Message from standard input */
        while (gets(message) != NULL)
            prline(message);
    }
}


/* Add the given letter to the end of the current line */

place(let)
struct letter *let;
{
int i,j,n;
int col;
int maxx= -10000;
char pad;
char *l;

    if (firstletter)
    {
        col = space;        /* Leading spaces */
        firstletter = 0;
    }
    else
    {
        /* Find the furthest left position we can place this letter */
        for(i=0; i<6; i++)
        {
            if (let->line[i][0] != '\000' &&
                (col = c[i] - let->spcs[i]) > maxx)
                maxx = col;
        }

        /* Insert some spaces between letters */
        col = maxx + space + interspace;
    }

    for(i=0; i<6; i++)

        /* If Nothing on this Line, Skip It */
        if (let->line[i][0] != '\000')
        {
            /* Number of Spaces to Put on this Line? */
            n = col - c[i] + let->spcs[i];

            /* Flyers off Ends of Letters */
            if (tail[i])
                for (j = 0;
                     j < 5 && n > space + 2; j++,n--)
                    buffer[i][c[i]++] = '_';

            /* Connecting Lines Between Letters */
            pad = (lasttail == i && let->tailin == i) ? '_' : ' ';

            /* In the middle of the intersection of South and */
            /* East University, someone is playing the piano. */
            for ( ; n > 0; n--)
                buffer[i][c[i]++] = pad;

            /* Copy in the letter text */
            for (l = let->line[i]; *l != '\000'; l++)
                buffer[i][c[i]++] = *l;

            tail[i] = (i == let->tailout);
        }

    lasttail = let->tailout;
    space = 0;
}

/* Lengthen the last trailer by n */
tailer(n)
int n;
{
int j;
    if (lasttail >= 0)
        for (j = 0; j < n; j++)
            buffer[lasttail][c[lasttail]++] = '_';
}

/* Handle a line */
prline(s)
char *s;
{
int l,i;
char *ch;
short lcode;

    lasttail = -1;
    firstletter = 1;
    space = 0;
    /* Get some memory to put the output into */
    l = strlen(s) * (CHARWIDTH + interspace);
    for (i=0;i<6;i++)
    {
        buffer[i] = malloc((unsigned)l);
        tail[i] = c[i] = 0;
    }

    /* do each letter */
    for (ch=s; *ch != '\000'; ch++)
    {

        *ch &= '\177';
        /* Find the letter */
        lcode = (lasttail != 2) ? code1[*ch] : code2[*ch];
        if (lcode >= 0)
            place(&list[lcode]);
        else
            /* Various Special characters */
            switch(lcode)
            {
            case SP:
                /* Insert white space before next letter */
                tailer(taillen);
                space += 3;
                lasttail = -1;
                break;
            case ST:
                /* Lengthen the last tail */
                if (lasttail >= 0)
                    buffer[lasttail][c[lasttail]++] = '_';
                break;
            }
    }
    tailer(taillen);

    /* print the line and release the memory */
    for (i=0;i<6;i++)
    {
        buffer[i][c[i]++] = '\n';
        write(1,buffer[i],c[i]);
        free(buffer[i]);
    }
}

/*
**      atoi
**
**      Convert an ASCII string to an integer
**
*/


int atoi( ap )
char *ap;
{
        register int n, c;
        register char *p;
        int f;

        p = ap;
        n = 0;
        f = 1;

        /* Scan the string for spaces or signs */
        for( ; ;p++ ){
                if( *p == SP || *p == TAB || *p == PLUS )
                        continue;
                if( *p == MINUS ){
                        f = -f;
                        continue;
                }
                break;
        }

        /* Now assemble the number */
        while( *p >= LEASTN && *p <= MOSTN )
                n = n*10 + *p++ - LEASTN;
        if( f < 0 )
                n = -n;

        return( n );
}

