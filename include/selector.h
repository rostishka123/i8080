#ifndef _SELECTOR_H_
#define _SELECTOR_H_


#define INTERACTIVE_MODE 0
#define COMPILER_MODE 1
#define EMULATOR_MODE 2
#define COMPILE_AND_EXECUTE_MODE 3

#define BYTE_OUTPUT 0
#define DEC_OUTPUT 1

#define NON_DEBUG 0
#define DEBUG 1

#define HUMAN_OUTPUT 0
#define MACHINE_OUTPUT 1

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <fcntl.h>
#include <stdbool.h>

bool debug, ioutput, output;
int mode;
int *fd_in;
int fd_out;
int bin_file;
int (*out_order)(char*, int, int);
char *(*write_regs)(void);


void init_varibles();
char *machine_print();
char *human_print();
int dec_output(char *, int, int);
int byte_output(char *, int, int);
void set_options(int, char **);


#endif


