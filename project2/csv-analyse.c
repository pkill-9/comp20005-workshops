/* Skeleton "interpreter" for numeric processing on CSV files
 
   Written by Alistair Moffat, May 2015

   Modifications by Matt Signorini, May 2015
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

/**
 *  Structure to contain data about categories. To compute the average, we
 *  need to know how many items in the category and what they sum to.
 */
typedef struct {
    double id;
    double sum;
    int count;
} category_t;

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
int get_scaling_factor (const bucket_t *buckets);
void print_bar (int length, int scaling_factor);
double getmax (const csv_t *data, int col);
double getmin (const csv_t *data, int col);

int insert_into_categories (category_t *c, int numcats, double id, 
  double value);
int get_category (category_t *c, int *numcats, double id);
void print_categories (category_t *c, int numcats);
void swap_categories (category_t *c1, category_t *c2);

int isconcordant (const csv_t *data, int row1, int row2, int col1, 
  int col2);

int get_bucket_index (double lower_limit, double bucket_range, 
  double value);
char bucket_to_char (int bucket_count);

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
            if (j >= 0) {
                line[j--] = '\0';
            }
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
    bucket_t buckets [GRAPHROWS];
    int row;

    /* col index starts at 1 for users. */
    col -= 1;

    init_buckets (D, col, buckets);

    for (row = 0; row < D->nrows; row ++) {
        add_to_buckets (buckets, D->vals [row] [col]);
    }

    print_histogram (D, buckets, col);

	return;
}

/****************************************************************/

/**
 *  Compute the average of the values in multiple categories. The cat
 *  parameter is the column index of the category labels, and col is the
 *  column of the data to be averaged.
 */
void
do_catavg(csv_t *d, int cat, int col) {
    category_t counts [MAXCATS];
    int row, numcats = 0;

    cat -= 1;
    col -= 1;

    for (row = 0; row < d->nrows; row ++) {
        numcats = insert_into_categories (counts, numcats, 
          d->vals [row] [cat], d->vals [row] [col]);
    }

    /* print header for stage 3 */
    printf ("%s     average %s\n", d->labs [cat], d->labs [col]);
    print_categories (counts, numcats);
}

/****************************************************************/

/**
 *  Calculates Kendall's tau correlation for two given columns in the
 *  data.
 */
void   
do_kndall(csv_t *d, int col1, int col2) {
    int concordant = 0, disconcordant = 0;
    int i, j;
    double tau;

    col1 --;
    col2 --;

    for (i = 0; i < d->nrows; i ++) {
        for (j = i + 1; j < d->nrows; j ++) {
            if (isconcordant (d, i, j, col1, col2)) {
                concordant ++;
            } else {
                disconcordant ++;
            }
        }
    }

    tau = 2.0 * (concordant - disconcordant) / 
        (d->nrows * (d->nrows - 1));

    printf ("tau coefficient between %s and %s = %6.2f\n", d->labs [col1],
      d->labs [col2], tau);
}

/****************************************************************/

/**
 *  Generates a 2 dimensional version of the stage 2 plot, illustrating
 *  the correlation between two columns.
 */
void   
do_graph2(csv_t *d, int col1, int col2) {
    int buckets [GRAPHROWS] [GRAPHCOLS] = {{0}};
    int row, col, xbin, ybin;

    /* map columns to zero origin */
    col1 --; col2 --;

    /* get max and min for both columns to calculate the range to be
     * divided into buckets */
    double ymin = getmin (d, col1) - EPSILON; 
    double ymax = getmax (d, col1) + EPSILON;
    double xmin = getmin (d, col2) - EPSILON; 
    double xmax = getmax (d, col2) + EPSILON;

    /* calculate the range of the buckets for the x and y dimensions. */
    double xinterval = (xmax - xmin) / GRAPHCOLS;
    double yinterval = (ymax - ymin) / GRAPHROWS;

    for (row = 0; row < d->nrows; row ++) {
        ybin = get_bucket_index (ymin, yinterval, d->vals [row] [col1]);
        xbin = get_bucket_index (xmin, xinterval, d->vals [row] [col2]);
        buckets [ybin] [xbin] += 1;
    }

    for (row = GRAPHROWS - 1; row >= 0; row --) {
        printf ("%6.2f--%6.2f: ", yinterval * row + ymin, 
          yinterval * (row + 1) + ymin);

        for (col = 0; col < GRAPHCOLS; col ++) {
            printf ("%c", bucket_to_char (buckets [row] [col]));
        }

        printf ("\n");
    }
}

/****************************************************************/

/**
 *  Return the index of the bucket that a given value belongs in, given
 *  the lower limit of the range and the size of the interval covered by
 *  each bucket.
 */
int
get_bucket_index (double lower_limit, double bucket_range, double value) {
    return (value - lower_limit) / bucket_range;
}

/****************************************************************/

/**
 *  Returns the character that should be printed out, given the number of
 *  values in a bucket. The density value must be only 1 char wide,
 *  otherwise our plot would get mangled.
 */
char
bucket_to_char (int bucket_count) {
    const char *alphabet = "0123456789abcdef";

    /* apply the formula specified in the project spec */
    bucket_count = log ((double) bucket_count + 1.0) / log (2.0);

    /* buckets with no values are displayed as "." */
    if (bucket_count == 0)
        return '.';

    /* otherwise, lookup the character from the alphabet */
    assert (bucket_count < strlen (alphabet));

    return alphabet [bucket_count];
}

/****************************************************************/

/**
 *  This function is used in the process of calculating Kendall's 
 *  coefficient. For the given dataset, this function returns true if
 *  row and row+1 are concordant. Rows are concordant if the values in the
 *  specified two columns are in the same order in both rows.
 */
int
isconcordant (const csv_t *data, int row1, int row2, int col1, int col2) {
    /* either col1 is less than col2 in both rows, or col1 is greater than
     * col2 in both rows. This can be expressed using the exclusive or
     * operation. */
    return ! (data->vals [row1] [col1] > data->vals [row2] [col1]) ^
        (data->vals [row1] [col2] > data->vals [row2] [col2]);
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
    double increment = (max - min) / GRAPHROWS;
    int i;

    for (i = 0; i < GRAPHROWS; i ++) {
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

    for (i = 0; i < GRAPHROWS; i ++) {
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
print_histogram (const csv_t *data, const bucket_t *buckets, int col) {
    int row, scaling = get_scaling_factor (buckets);

    printf ("graph of %s scaled by a factor of %d\n", data->labs [col],
      scaling);

    for (row = 0; row < GRAPHROWS; row ++) {
        printf ("%5.2f -- %5.2f [%4d]:", buckets [row].lower, 
          buckets [row].upper, buckets [row].nitems);
        print_bar (buckets [row].nitems, scaling);
    }
}

/****************************************************************/

/**
 *  Prints a bar of the histogram for stage 2, with a newline at the end.
 */
void
print_bar (int length, int scaling_factor) {
    for (; length > 0; length -= scaling_factor)
        printf ("*");

    printf ("\n");
}

/****************************************************************/

/**
 *  Returns the number of units that each star should represent such that
 *  the longest bar of the graph will not go over the 80 column margin.
 *
 *  For example, if this function returns 4, each star should represent 4
 *  items in the bucket.
 */
int
get_scaling_factor (const bucket_t *buckets) {
    int maxitems = buckets [0].nitems, i;

    /** find the bucket with the most items in it. That will be the longest
     *  bar in the graph. */
    for (i = 0; i < GRAPHROWS; i ++) {
        if (buckets [i].nitems > maxitems)
            maxitems = buckets [i].nitems;
    }

    /** allow up to 60 stars in a bar of the graph */
    return maxitems / 60 + 1;
}

/****************************************************************/

/**
 *  Get the maximum down a given row of the data.
 */
double
getmax (const csv_t *data, int col) {
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
getmin (const csv_t *data, int col) {
    double min = data->vals [0] [col];
    int row;

    for (row = 0; row < data->nrows; row ++) {
        if (data->vals [row] [col] < min) {
            min = data->vals [row] [col];
        }
    }

    return min;
}

/****************************************************************/

/**
 *  Search through the category array for a category matching id, and if
 *  found, add the specified value to its sum. If the category does not
 *  exist, this function will create a new category, setting its initial
 *  sum to zero.
 *
 *  Return value is the length of the category array, which may be 1 unit
 *  larger if a new category is added.
 */
int
insert_into_categories (category_t *c, int numcats, double id, double val) {
    int index = get_category (c, &numcats, id);
    int previndex = index - 1;

    c [index].sum += val;
    c [index].count += 1;

    /* keep the category array sorted. */
    while (previndex >= 0 && c [index].id < c [previndex].id) {
        swap_categories (c + index, c + previndex);
        index --;
        previndex --;
    }

    return numcats;
}

/****************************************************************/

/**
 *  Searches the category array for a matching id. If found, this function
 *  returns the index of the matching id. Otherwise, a new category struct
 *  will be initialised (sum = 0, count = 0) and the index of the new
 *  category will be returned.
 */
int
get_category (category_t *c, int *numcats, double id) {
    int i, found = 0;

    for (i = 0; i < *numcats; i ++) {
        if (c [i].id == id) {
            found = 1;
            break;
        }
    }

    /* check to see if we found a matching category */
    if (found == 0) {
        c [i].sum = 0;
        c [i].count = 0;
        c [i].id = id;

        /* we have added a category, so increase the length of the array */
        *numcats += 1;
    }

    return i;
}

/****************************************************************/

/**
 *  Print output for stage 3, consisting of each category value, and the
 *  average of the values in that category.
 */
void
print_categories (category_t *cats, int numcats) {
    int i;

    for (i = 0; i < numcats; i ++) {
        printf ("%8.2f %10.2f (over %d values)\n", cats [i].id,
          cats [i].sum / cats [i].count, cats [i].count);
    }
}

/****************************************************************/

/**
 *  Swap two category structs.
 */
void
swap_categories (category_t *c1, category_t *c2) {
    category_t temp = *c1;
    *c1 = *c2;
    *c2 = temp;
}

/****************************************************************/

/** vim: set ts=4 sw=4 et : */
