#ifndef _COMPILER_H_
#define _COMPILER_H_

struct lab{
	char label[250];
	unsigned int addr;
};

char *genschtalt(char *, int *);
int compile(char *, int, int *);
int quantity_args(char *);


#endif
