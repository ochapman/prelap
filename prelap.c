/*
 *       Filename:  prelap.c
 *    Description:  
 *         Author:  Chapman Ou <ochapman.cn@gmail.com>
 *        Created:  05/02/12 
 *        
 */
#define _GNU_SOURCE
#include	<stdio.h>
#include	<stdlib.h>
#include	<execinfo.h>
#include	<stdint.h>
#include	<string.h>
#include	<dlfcn.h>


static void parse_sym(const char *sym)
{
	const char *loc = NULL;
	/* TODO: size limited */
	char file[50];
	char addr[50];
	char cmd[100];
	size_t len;

	loc = strrchr(sym, '(');
	if (loc == NULL) {
		perror("'[' could not found in symbols\n");
		exit(EXIT_FAILURE);
	}

	len = (size_t)((uintptr_t)loc - (uintptr_t)sym);
	memcpy(file, sym, len);
	file[len] = '\0';
	loc = NULL;

	loc = strrchr(sym, '[');
	if (loc == NULL) {
		perror("'[' could not found in symbols\n");
		exit(EXIT_FAILURE);
	}

	len = (size_t)(strlen(sym) - ((uintptr_t)loc- (uintptr_t)sym)) - 2;
	memcpy(addr, loc + 1, len);
	addr[len] = '\0';
	/* last addr */
	addr[len - 1] -= 1;
	/* TODO: fork and exec, output redirect */
	snprintf(cmd,  sizeof(cmd),"addr2line -e %s %s", file, addr);
	printf("cmd: %s", cmd);
	system(cmd);
}

static void get_trace(void)
{
	void	*array[10];
	size_t	size;
	char	**strings;
	int	i;
	 
	size = backtrace(array, 10);
	strings = backtrace_symbols(array, size);

	for (i = 0; i < size; i++)
		fprintf(stderr, "%s\n", strings[i]);
	
	/* ONLY LAST SECOND SYMBOLS NEED TO BE PARSE */
	parse_sym(strings[2]);
	free(strings);
}

#define ORIG_PREFIX(func) orig_##func

#define ORIG_FUNCP_DEFINE(ret_type, func, ...)  \
	static ret_type (*ORIG_PREFIX(func))(__VA_ARGS__);

#define INIT_CONSTRUCTOR(func)  \
	void __attribute__((constructor)) init_##func() \
	{ \
		ORIG_PREFIX(func) = dlsym(RTLD_NEXT, #func); \
	} 

#define NEW_FUNC_DEFINE(ret_type, func, orig_func_exp, trap_cond, ...)  \
	ret_type func(__VA_ARGS__) \
	{ \
		if (trap_cond) { \
			get_trace(); \
		} \
	 \
		return orig_func_exp; \
	}

#define ORIG_FUNC_EXP(func, args) (*ORIG_PREFIX(func))args
#define STRCPY_COND(dest, src) ((size_t)((uintptr_t)src - (uintptr_t)dest) < strlen(dest))

/*
 * Common interface, for hook function.
 * ...: arguments of func
 */ 
#define NEW_FUNC_INIT(ret_type, func, orig_func_args, trap_cond, ...) \
	ORIG_FUNCP_DEFINE(ret_type, func, __VA_ARGS__) \
	INIT_CONSTRUCTOR(func) \
	NEW_FUNC_DEFINE(ret_type, func, ORIG_FUNC_EXP(func, orig_func_args), trap_cond, __VA_ARGS__)


NEW_FUNC_INIT(char *, strcpy, (dest, src), STRCPY_COND(dest, src), char *dest, const char *src);
NEW_FUNC_INIT(char *, strncpy, (dest, src, n), STRCPY_COND(dest, src), char *dest, const char *src, size_t n);

