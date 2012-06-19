#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define _GNU_SOURCE
#include <getopt.h>

extern char *optarg;
extern int optind, opterr, optopt;

int analyse_chunk(char *buffer, FILE *f, size_t length);
int analyse_file(FILE *f, size_t *state, size_t state_len, size_t chunk_size);
void print_analysis(const char *filename, size_t *state, size_t state_len, size_t chunk_size, size_t filesize);
size_t multiplier(char *num_string);


#define F_NONE 0x00
#define F_CONTIGUOUS 0x01
#define F_QUIET 0x02
#define F_VERBOSE 0x04
#define F_BLOCK_SET 0x08
#define F_NUMBER_SET 0x10

char flags; /* Ugly global variable. Eugh! I'm lazy, though. */

int main(int argc, char **argv)
{
	size_t block_size = 0;
	flags = F_NONE;
	unsigned int interval;
	size_t number = 100;
	
	if ( argc > 1 )
	{
		/* Do getopt stuff */
		struct option options[] = {
			{"block-size", 1, NULL, 'b'},
			{"contiguous", 0, NULL, 'c'},
			{"interval", 1, NULL, 'i'},
			{"number", 1, NULL, 'n'},
			{"quiet", 0, NULL, 'q'},
			{"verbose", 0, NULL, 'v'},
			{0, 0, 0, 0}
		};
		
		int val;
		while( (val = getopt_long(argc, argv,
						"b:ci:n:qv", options, NULL)) != -1 )
		{
			switch(val)
			{
				case 'b':
					/* optarg points to a string representing block size */
					if ( flags & F_NUMBER_SET )
						fprintf(stderr, "Warning: Ignoring custom block size. Number of blocks already set.\n");
					else
					{
						block_size = multiplier(optarg);
						block_size *= strtoul(optarg, NULL, 0);
						flags |= F_BLOCK_SET;
					}
					break;
				case 'c':
					/* Treat multiple files as one contiguous file */
					flags |= F_CONTIGUOUS;
					break;
				case 'i':
					/* Set interval for percentage cutoffs */
					interval = atoi(optarg);
					break;
				case 'n':
					/* Set number of blocks */
					if ( flags & F_BLOCK_SET )
						fprintf(stderr, "Warning: Ignoring custom block number. Block size already set.\n");
					else
					{
						number = multiplier(optarg);
						number *= strtoul(optarg, NULL, 0);
						flags |= F_NUMBER_SET;
					}
					break;
				case 'q':
					/* Set quiet flag */
					if ( flags & F_VERBOSE )
						fprintf(stderr, "Warning: Ignoring quiet flag. Verbose mode already set.\n");
					else
						flags |= F_QUIET;
					break;
				case 'v':
					/* Set verbose flag */
					if ( flags & F_QUIET )
						fprintf(stderr, "Warning: Ignoring verbose flag. Quiet mode already set.\n");
					else
						flags |= F_VERBOSE;
					break;
				case '?':
				default:
					/* Display usage info */
					exit(1);
					break;
			}
		}
	}
	/* optind should now give the first non-option argument */
	if ( optind > argc - 1 )
	{
		/* Print usage */
		exit(1);
	}
	
	FILE *f = fopen(argv[optind], "r");
	struct stat st;
	fstat(fileno(f), &st);
	size_t filesize = st.st_size;
	if ( flags & F_BLOCK_SET )
	{
		/* Calculate number of blocks from file size */
		if ( block_size < 1 )
		{
			fprintf(stderr, "Error: Block size must be at least 1 byte.\n");
			exit(1);
		}
		number = filesize / block_size;
	}
	else
	{
		/* Calculate block size from file size */
		block_size = filesize / number;
		if ( block_size < 1 )
		{
			fprintf(stderr, "Error: Block size is less than 1 byte. The results will be meaningless.\n");
			exit(1);
		}
	}

	size_t *state = malloc(number*sizeof(size_t));
	memset(state, 0x00, number*sizeof(size_t));
	if ( analyse_file(f, state, number, block_size) != 0 )
	{
		fprintf(stderr, "Error.\n");
		exit(1);
	}

	print_analysis(argv[optind], state, number, block_size, filesize);
	free(state);

	return 0;
}

int analyse_chunk(char *buffer, FILE *f, size_t length)
{
	/*
	 * Returns number of non-zero bytes in a chunk
	 * or -1 on error
	 */
	size_t n_read = 0;
	size_t non_zero_count = 0;
	size_t i = 0;

	memset(buffer, 0x00, length);

	n_read = fread(buffer, 1, length, f);
	if ( n_read != length && ! feof(f) )
		return -1;

	for(i = 0; i < n_read; ++i)
	{
		if ( buffer[i] != 0x00 )
			++non_zero_count;
	}

	return non_zero_count;
}

int analyse_file(FILE *f, size_t *state, size_t state_len, size_t chunk_size)
{
	/* Analyse empty space in a file. Returns 0 on success,
	 * or -1 on error.
	 */
	size_t i;
	
	char *buffer = malloc(chunk_size);
	if ( ! buffer )
		return -1;

	for(i = 0; i < state_len; ++i)
	{
		state[i] = analyse_chunk(buffer, f, chunk_size);
	}
	
	free(buffer);
	return 0;
}

void print_analysis(const char *filename, size_t *state, size_t state_len, size_t chunk_size, size_t filesize)
{
	size_t i;
	double *fractions = malloc(state_len*sizeof(double));
	size_t n_empty, n_25, n_50, n_75, n_full;
	n_empty = n_25 = n_50 = n_75 = n_full = 0;
	size_t total_usage = 0;
	double percentage = 0;

	if ( ! (flags & F_QUIET) )
		printf("Usage analysis for file: %s\n", filename);

	for(i = 0; i < state_len; ++i)
	{
		if ( i % 25 == 0 )
			printf("\n");
		total_usage += state[i];
		fractions[i] = (1.0*state[i]) / (1.0*chunk_size);
		if ( state[i] == 0 )
		{
			printf("_");
			++n_empty;
		}
		else if ( fractions[i] < 0.25 )
		{
			printf("-");
			++n_25;
		}
		else if ( fractions[i] < 0.50 )
		{
			printf("+");
			++n_50;
		}
		else if ( fractions[i] < 0.75 )
		{
			printf("*");
			++n_75;
		}
		else
		{
			printf("#");
			++n_full;
		}
	}
	printf("\n\n");

	if ( ! (flags & F_QUIET) )
	{
	
		printf("Block size:\t%u\nNumber:\t\t%u\n\n", chunk_size, state_len);

		printf("_ Empty\t\t%u\n- Under 25%%\t%u\n+ 25%% to 50%%\t%u\n* 50%% to 75%%\t%u\n# Over 75%%\t%u\n",
			n_empty, n_25, n_50, n_75, n_full);

		percentage = (1.0*total_usage)/(1.0*filesize)*100.0;
		printf("\nFile length\t%u\nNon-null\t%u\nPercentage\t%lf\n", filesize, total_usage, percentage);
	}
}

size_t multiplier(char *num_string)
{
	char *p = num_string;

	/* Search for non-digits */
	while( *p != '\0' && isdigit(*p) )
		++p;

	switch(*p)
	{
		case '\0':
			return 1;
		case 'c':
			return 1;
		case 'b':
			return 512;
		case 'k':
		case 'K':
			return 1024;
		case 'M':
			return 1024*1024;
		case 'G':
			return 1024*1024*1024;
		default:
			return 1;
	}
}
