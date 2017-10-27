#ifndef _CPU_H_
#define _CPU_H_

#include <stdlib.h>
#include <string.h>

#define REGS_TO_ZERO (a=b=c=d=e=l=m=f=cf=p=z=s=nn=pp=0)

int a, b, c, d, e, h, l, f, m, psw, new_addr, prev_com;
int *pc, *sp;
int progs[MAX_NUM], port[MAX_NUM];
int *begin, *end; 
char cf, p, ax, z, s;
char inter_perm;
char nn, pp;

int *order_to_int(char *);
int exec();


#endif
