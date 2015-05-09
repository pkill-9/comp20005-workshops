/* Skeleton "interpreter" for numeric processing on CSV files
 
   Written by Alistair Moffat, May 2015

   Modifications by XXXXX, May 2015
   (Add your name and login name)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#define MAXROWS 10000
#define MAXCOLS 50

#define LINELEN	500	/* maximum length of any input line */

#define ERROR	(-1)	/* error return value from some functions */
#define COMMA	','	/* separator for CSV files */

#define INDEXX	'i'	/* command to list the column headings */
#define DATDMP	'd'	/* command to dump out the data */
#define AVERGE	'a'	/* command to average a column */
#define CATAVG	'c'	/* command to category-average a column */
#define GRAPH1	'g'	/* command to graph a column */
#define GRAPH2	'p'	/* command to plot a 2d correlation */
#define KNDALL	'k'	/* command to compute Kendall's tau */
#define ALLOPS  "acdgikp"
			/* list of all valid commands */

#define ARGS_0	"di"	/* commands that take no arguments */
#define ARGS_1	"ag"	/* commands that take one argument */
#define ARGS_2	"ckp"	/* commands that take two arguments */
#define MAXARGS  2      /* maximum number of arguments to any command */

#define GRAPHROWS 20	/* number of rows in graph */
#define GRAPHCOLS 60	/* number of columns in graph */
#define EPSILON   1e-6	/* small adjustment for graph computation */

#define MAXCATS	1000	/* maximum number of categories to be handled */

#define FILINP	1	/* indicates command input coming from a file */
#define PROMPT	"> "

#define NUMBUCKETS 20   /* number of divisions for the graph in stage 2 */

/****************************************************************/

/* structure declarations -- probably no need to change these,
   but you can if you wish... */

typedef char input_line_t[LINELEN+1];

typedef struct {
	input_line_t labelstring;
	char *labs[MAXCOLS+1];
	int nrows;
	int ncols;
	double vals[MAXROWS][MAXCOLS];
} csv_t;

typedef struct {
	char command;
	int nargs;
	int argvals[MAXARGS];
} command_t;

/**
 *  This structure is used to build the histogram for stage 2.
 */
typedef struct {
    double upper;
    double lower;
    int nitems;
} bucket_t;

/****************************************************************/

/* function prototypes */

void	read_csv_file(char *fname, csv_t *D);
void	reassign_input(char *fname);
void    print_prompt();
int     read_command(command_t *comd, int fileinput, int ncols);
void	process_line(command_t *comd, csv_t *D);
void    do_datdmp(csv_t *D);
void    do_indexx(csv_t *D);
void    do_averge(csv_t *D, int col);
void    do_graph1(csv_t *D, int col);
void    do_catavg(csv_t *D, int cat, int col);
void    do_kndall(csv_t *D, int col1, int col2);
void    do_graph2(csv_t *D, int col1, int col2);

void init_buckets (const csv_t *data, int col, bucket_t *buckets);
void add_to_buckets (bucket_t *buckets, double value);
void print_histogram (const csv_t *data, const bucket_t *buckets, int col);
void print_bar (int length);
double getmax (const csv_t *data, int col);
double getmin (const csv_t *data, int col);

/****************************************************************/

/* main program controls all the action
 */
int
main(int argc, char *argv[]) {
	int fileinput=0;
	command_t comd;
	csv_t D;

	/* first argument on commandline is the data file name */
	read_csv_file(argv[1], &D);

	/* second argument, if it exists, is file of input commands */
	if (argc==3) {
		fileinput = 1;
		reassign_input(argv[2]);
	}

	/* start the main execution loop */
	print_prompt();
	while (read_command(&comd, fileinput, D.ncols)) {
		process_line(&comd, &D);
		/* then round we go */
		print_prompt();
	}

	/* all done, so pack up and go home */
	printf("bye\n");
	return 0;
}

/****************************************************************/

/* reads a csv file in to the defined structure, with empty or non-numeric
   values replaced by 0.0/0.0 = nan so as to prevent downstream arithmetic
   being interpreted incorrectly. Probably best to just leave this function
   alone, you don't need to edit it to complete the project
*/

void
read_csv_file(char *fname, csv_t *D) {
	FILE *fp;	/* used to read from a named file */
	input_line_t line;
	int cols=0, rows=0, bytes=0;
	int c, i, j, chr, ncommas, empties=0;
	double x;
	double nan = 0.0/0.0;

	/* first argument on commandline should the data file name */
	if (fname==NULL) {
		/* and it wasn't there... */
		printf("No csv file specified on commandline\n");
		exit(EXIT_FAILURE);
	}

	/* try and open the named file for reading */
	if ((fp=fopen(fname,"r")) == NULL) {
		printf("Error: unable to open %s\n", fname);
		exit(EXIT_FAILURE);
	}

	/* file is open, can now use fp to access CSV data,
	   start by reading the bytes of the header row */
	while ((c=getc(fp)) != '\n') {
		D->labelstring[bytes++] = c;
	}
	D->labelstring[bytes] = '\0';

	/* now process line again, breaking in to separate labels by
	   replacing commas by nulls, and tracking the start of each of
	   the column headings */
	D->labs[cols++] = D->labelstring;
	for (i=1; i<bytes; i++) {
		if (D->labelstring[i]==COMMA) {
			D->labelstring[i] = '\0';
			D->labs[cols++] = D->labelstring+i+1;
		}
		if (cols>MAXCOLS && i<bytes) {
			printf("Too many columns, limit is %d\n",
				MAXCOLS);
			exit(EXIT_FAILURE);
		}
	}
	D->labs[cols] = NULL;

	/* ok, that's the labels sorted, now for the data */
	while ((chr=getc(fp)) != EOF) {

		/* there is another row, because first character of it
		   just got read, next step is to get the rest of them */
		i = 0;
		line[i++] = chr;
		ncommas = (chr==COMMA) ;
		while (((chr=getc(fp))!=EOF) && (chr!='\n')) {
			line[i++] = chr;
			ncommas += (chr==COMMA) ;
		}
		line[i] = '\0';
		if (ncommas!=cols-1) {
			printf("Data input error line %d\n", rows+2);
			exit(EXIT_FAILURE);
		}
		/* then process the line from the right end */
		j = i-1;
		for (c=cols-1; c>=0; c--) {
			/* look for next previous comma */
			while (j>=0 && line[j]!=COMMA) {
				j--;
			}
			/* access the value */
			if (sscanf(line+j+1, "%lf", &x) == 1) {
				D->vals[rows][c] = x;
			} else {
				D->vals[rows][c] = nan;
				empties++;
			}
			/* mark the new end of the string */
			line[j--] = '\0';
		}
		rows++;
		/* check to make sure don't overflow array */
		if (rows==MAXROWS) {
			/* time to stop reading data */
			printf("Too many rows, truncated at %d\n", MAXROWS);
			break;
		}
		/* if not full, go round and see if there is another data row */
	}

	/* either input has all been read or array is full */
	printf("file %s:\n    %d columns and %d rows of data\n",
			fname, cols, rows);
	if (empties) {
		printf("    %d entries were empty or non-numeric\n",
			empties);
	}
	/* finish building the structure */
	D->nrows = rows;
	D->ncols = cols;
	return;
}

/****************************************************************/

/* if there is a valid filename on the commandline, redirect stdin
   so that the file is read, and return FILINP to show that input
   input lines should be echoed to the output when they are read
*/
void
reassign_input(char *fname) {
	if (freopen(fname, "r", stdin)==NULL) {
		printf("Unable to open \"%s\"\n", fname);
		exit(EXIT_FAILURE);
	}
	/* stdin successfully reopened to the named file */
	printf("Input file: %s\n", fname);
	return;
}

/****************************************************************/

/* print the "ready for input" prompt
 */
void
print_prompt() {
	printf(PROMPT);
	return;
}

/****************************************************************/

/* read a line of input into the array passed as argument
   returns false if there is no input available
   all whitespace characters are removed
*/
int    
read_command(command_t *comd, int fileinput, int ncols) {
	int i=0, c;
	int col;
	input_line_t line;
	/* get a whole input line, single blank of multiples */
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (i<LINELEN) {
			line[i] = c;
			if (i==0 || (isspace(line[i-1])*isspace(line[i])==0)) {
				i++;
			}
		}
	}
	line[i] = '\0';
	if (fileinput) {
		/* print out the input command */
		printf("%s\n", line);
	}
	/* nothing typed? straight back to caller */
	if (i==0 && c==EOF) {
		return 0;
	}
	if (i==0) {
		return 1;
	}
	/* something typed? parse into parts needed */
	comd->command = line[0];
	comd->nargs = 0;
	for (i=1; line[i]; i++) {
		if (!isdigit(line[i]) && !isspace(line[i])) {
			printf("Invalid input character\n");
			return 1;
		}
		if (line[i-1]==' ' && line[i]!=' ') {
			col = atoi(line+i);
			comd->argvals[comd->nargs++] = col;
		}
	}
	return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* process a command by parsing the input line into parts and
   carrying out the specified action
 */
void
process_line(command_t *comd, csv_t *D) {
	int optype, col1=0, col2=0;

	/* determine the operation to be performed, it
	   must be first character in line
	 */
	optype = comd->command;
	if (strchr(ALLOPS, optype) == NULL) {
		printf("Unknown operator\n");
		return;
	}

	/* determine the string argument (if one is required),
	   it must start in second character of line
	 */
	if (strchr(ARGS_0, optype)) {
		if (comd->nargs!=0) {
			printf("No argument required for '%c'\n", optype);
			return;
		}
	} else if (strchr(ARGS_1, optype)) {
		if (comd->nargs!=1) {
			printf("One argument required for '%c'\n", optype);
			return;
		}
		col1 = comd->argvals[0];
		if (col1>D->ncols) {
			printf("Invalid column number, ");
			printf("max is %d\n", D->ncols);
			return;
		}
	} else if (strchr(ARGS_2, optype)) {
		if (comd->nargs!=2) {
			printf("Two arguments required for '%c'\n", optype);
			return;
		}
		col1 = comd->argvals[0];
		col2 = comd->argvals[1];
		if (col1>D->ncols || col2>D->ncols) {
			printf("Invalid column number, ");
			printf("max is %d\n", D->ncols);
			return;
		}
	}

	/* finally, do the actual operation
	 */
	if (optype == INDEXX) {
		do_indexx(D);
	} else if (optype == DATDMP) {
		do_datdmp(D);
	} else if (optype == AVERGE) {
		do_averge(D, col1);
	} else if (optype == GRAPH1) {
		do_graph1(D, col1);
	} else if (optype == CATAVG) {
		do_catavg(D, col1, col2);
	} else if (optype == KNDALL) {
		do_kndall(D, col1, col2);
	} else if (optype == GRAPH2) {
		do_graph2(D, col1, col2);
	}
	return;
}

/****************************************************************/

/* provide an index list of the column headings
*/
void
do_indexx(csv_t *D) {
	int c;
	printf("      col  data\n");
	for (c=0; c<D->ncols; c++) {
		printf("      %3d  %s\n", c+1, D->labs[c]);
	}
	return;
}

/****************************************************************/

/* dump out the data in the CSV structure D
*/
void
do_datdmp(csv_t *D) {
	int r, c;
	/* first the header labels */
	printf("      ");
	for (c=0; c<D->ncols; c++) {
		printf("%10s ", D->labs[c]);
	}
	printf("\n");
	/* now the values in the data rows */
	for (r=0; r<D->nrows; r++) {
		printf("%4d: ", r+1);
		for (c=0; c<D->ncols; c++) {
			printf("%10.2f ", D->vals[r][c]);
		}
		printf("\n");
	}
	return;
}

/****************************************************************/

/**
 *  Computes the average down a given column of the data.
 */
void
do_averge(csv_t *D, int col) {
    int row;
    double sum = 0;

    /* column index begins at 1 from the users perspective */
    col -= 1;

    for (row = 0; row < D->nrows; row ++)
        sum += D->vals [row] [col];

    printf ("average %s is %5.2f (over %d values)\n", D->labs [col], 
      sum / row, row);

	return;
}

/****************************************************************/

/**
 *  Implement the 'g' graphing command. Plots a histogram of the data.
 */
void
do_graph1(csv_t *D, int col) {
    bucket_t buckets [NUMBUCKETS];
    int row;

    init_buckets (D, col, buckets);

    for (row = 0; row < D->nrows; row ++) {
        add_to_buckets (buckets, D->vals [row] [col]);
    }

    print_histogram (D, buckets, col);

	return;
}

/****************************************************************/

/* implement the 'c' category average command
*/
void
do_catavg(csv_t *D, int cat, int col) {
	return;
}

/****************************************************************/

/* implement the 'k' command to compute Kendall's tau-b
   coefficient for similarity between two sets of paired values
*/
void   
do_kndall(csv_t *D, int col1, int col2) {
	return;
}

/****************************************************************/

/* implement the 'p' plot command to generate
   a 2d graph showing correlation between two columns
*/
void   
do_graph2(csv_t *D, int col1, int col2) {
	return;
}

/****************************************************************/

/**
 *  Initialise the buckets array which is used to produce the histogram in
 *  stage 2. This function will set the upper and lower limits of each
 *  bucket in the array to cover the range of the data, and will set all
 *  the counts to zero.
 */
void
init_buckets (const csv_t *data, int col, bucket_t *buckets) {
    double min = getmin (data, col) - EPSILON;
    double max = getmax (data, col) + EPSILON;
    double increment = (max - min) / NUMBUCKETS;
    int i;

    for (i = 0; i < NUMBUCKETS; i ++) {
        buckets [i].lower = min + increment * i;
        buckets [i].upper = buckets [i].lower + increment;
        buckets [i].nitems = 0;
    }
}

/****************************************************************/

/**
 *  Increments the items count for the bucket corresponding to the given
 *  value.
 */
void
add_to_buckets (bucket_t *buckets, double value) {
    int i;

    for (i = 0; i < NUMBUCKETS; i ++) {
        if (value > buckets [i].lower && value < buckets [i].upper) {
            buckets [i].nitems += 1;
            return;
        }
    }
}

/****************************************************************/

/**
 *  Prints the output for stage 2; a histogram of the data down a given
 *  column.
 */
void
print_histogram (const csv_t *data, const bucket_t *buckets, int col)
{
    int row;

    printf ("graph of %s scaled by a factor of 1\n", data->labs [col]);

    for (row = 0; row < NUMBUCKETS; row ++) {
        printf ("%5.2f -- %5.2f [%4d]:", buckets [row].lower, 
          buckets [row].upper, buckets [row].nitems);
        print_bar (buckets [row].nitems);
    }
}

/****************************************************************/

/**
 *  Prints a bar of the histogram for stage 2, with a newline at the end.
 */
void
print_bar (int length)
{
    for (; length > 0; length -= 1)
        printf ("*");

    printf ("\n");
}

/****************************************************************/

/**
 *  Get the maximum down a given row of the data.
 */
double
getmax (const csv_t *data, int col)
{
    double max = data->vals [0] [col];
    int row;

    for (row = 0; row < data->nrows; row ++) {
        if (data->vals [row] [col] > max) {
            max = data->vals [row] [col];
        }
    }

    return max;
}

/****************************************************************/

/**
 *  Similarly for the min down a row.
 */
double
getmin (const csv_t *data, int col)
{
    double min = data->vals [0] [col];
    int row;

    for (row = 0; row < data->nrows; row ++) {
        if (data->vals [row] [col] > min) {
            min = data->vals [row] [col];
        }
    }

    return min;
}

/****************************************************************/

/** vim: set ts=4 sw=4 et : */
