struct letter {
	char *line[6];			/* What's on each of the six lines */
	char spcs[6];			/* Number of leading spaces for each */
	short tailin,tailout;		/* Where the lines in and out go */
	};

/* Special Codes for Character Table */

#define XX  -1		/* Ignore character */
#define SP  -2          /* Space */
#define ST  -3          /* Extend trailer */
#define SA  -4		/* Alternate font number */	/* Not used */
#define SN  -5		/* Numbers */			/* Not used */

#define CHARWIDTH 10

extern struct letter list[];
extern short code1[], code2[];
