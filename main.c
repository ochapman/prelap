/*
 *       Filename:  main.c
 *    Description:  
 *         Author:  Chapman Ou <ochapman.cn@gmail.com>
 *        Created:  05/03/12 
 *        
 */

#include	<stdio.h>
#include	<getopt.h>

#define	FUNC_MAX_LEN	50
#define	FILE_NAME_MAX	128
#define	COND_MAX	128
#define	CMD_MAX		128

typedef struct _opt {
	char *output;
	char *func;
	char *cond;
	char *prog;
}opt_t;

static void usage(const char *appname)
{
	printf( "Usage: %s -f func -o output.txt "
		"\t-d ((src - dest) < strlen(dest)) -p PROGRAM\n"
		"\t-o --output	output to file, stdout default\n"
		"\t-f --function	function that want to be hook\n"
		"\t-c --condition	condition trigger\n"
		"\t-p --program	program that want to check\n"
		"\t-h --help	this message\n", appname);
}

static int parse_options(int argc, char *argv[], opt_t *opt)
{
	const char *short_options = "o:f:c:p:h";
	int next_opt;
	int digit_optind = 0;
	const struct option long_options[] = {
		{"output", 1, NULL, 'o'},
		{"function", 1, NULL, 'f'},
		{"condition", 1, NULL, 'c'},
		{"program", 1, NULL, 'p'},
		{"help", 0, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	if (argc < 2) {
		return -1;
	}

	do {
		int this_option_optind = optind ? optind : 1;
		next_opt = getopt_long(argc, argv, short_options, 
				long_options, NULL);

		switch (next_opt) {
			case 'o':
				opt->output = optarg;
				break;
			case 'f':
				opt->func = optarg;
				break;
			case 'c':
				opt->cond = optarg;
				break;
			case 'p':
				opt->prog = optarg;
				break;
			case 'h':
			case '?':
			case ':':
				return -1;
			case -1:
				break;
			case '0':
			case '1':
			case '2':
				 if (digit_optind != 0 && 
					digit_optind != this_option_optind)
					printf("digits occur in two different argv-elements.\n");
				digit_optind = this_option_optind;
				return -1;
			default:
				return -1;
		}
	} while (next_opt != -1);
	
	if (optind < argc) {
		fprintf(stderr, "non-option ARGV-elements: ");
		while (optind < argc)
			fprintf(stderr, "%s ", argv[optind++]);
		fprintf(stderr, "\n");
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	opt_t	opt;
	int	ret;

	ret = parse_options(argc, argv, &opt);
	if (ret != 0) {
		usage(argv[0]);
		return -1;
	}

	printf("output: %s function: %s cond %s prog %s\n",
			opt.output, opt.func, opt.cond, opt.prog);

	return 0;
}
