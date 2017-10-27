#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h> 

#include "../include/unfunc.h"     //universal functions for compiler and cpu 
#include "../include/compiler.h"
#include "../include/cpu.h"
#include "../include/selector.h"


char* compiler(char *text, int *lines){
	char line[MAXLINE+1], *ret, *code=NULL;
	int l, i, n, byte, li, len;
	

	ret = (char *) malloc(strlen(text));
	strcpy(ret, text);
	code = genschtalt(ret, &l);
	
	if(error){
		*lines=l;
		return NULL;
	}
	free(ret);
	ret=NULL;
	
	l=1;
	len=0;
	
	while(1){
		
		if(*code=='\0')
			break;
		
		li=0;
		while(*code!='\n')
			line[li++]=*code++;
		
		line[li]='\0';	
		code++;
		
		if(!li){
			l++;
			continue;
		}
		
		n = quantity_args(line);
		if((i=compile(line, n, &byte))==-1){
			if(error)
				break;
			continue;
		}
		
		out_order(line, i, byte*3);
		if(line!=NULL){
			len+=strlen(line);
			ret = realloc(ret, len+1);
			strcat(ret, line);
		}	
		l++;
	}
	
	if(error){
		return NULL;
		*lines = l;
	}
	return ret;
	
}

int execute_str(char *bin){
	int *code;
	char *reg_str;
	
	code = order_to_int(bin);
	code = realloc(code, (MAX_ADDR+1)*sizeof(int));
		
	if(sizeof(code)>MAX_ADDR){
		error=THE_PROGRAMM_IS_TOO_BIG;
		return -1;
	}	
	
	begin=pc=code;
	end=sp=code+MAX_ADDR;
		
	
	if(exec(*pc)==-1){
		if(ioutput==HUMAN_OUTPUT)
			printf("Error by executintg instruction number %i. Command: %s\n", prev_com+1, num_to_com(*(begin+prev_com)));
	}
			
	if(pc>end || pc<begin)
		pc=begin;
	
	reg_str=write_regs();
	if(write(fd_out, reg_str, strlen(reg_str)) < 0)
		err_sys("Write error");
				
	free(code);	
	return 0;
}

int execute(char *bin){
	int *code;
	char *reg_str;
	int ret;	
	
	code = order_to_int(bin);
	code = realloc(code, (MAX_ADDR+1)*sizeof(int));
		
	if(sizeof(code)>MAX_ADDR){
		error=THE_PROGRAMM_IS_TOO_BIG;
		return -1;
	}	
	
	begin=pc=code;
	end=sp=code+MAX_ADDR;

	while(1){
		if((ret = exec(*pc))==-1){
			if(ioutput==HUMAN_OUTPUT)
				printf("Error by executintg instruction number %i. Command: %s\n", prev_com+1, num_to_com(*(begin+prev_com)));
		}else if(!ret)
			break;
			
		if(pc>end || pc<begin)
			pc=begin;
				
		if(debug==DEBUG){
			reg_str=write_regs();
			if(write(fd_out, reg_str, strlen(reg_str)) < 0)
				err_sys("Write error");	
		}
	}

	reg_str=write_regs();
	if(write(fd_out, reg_str, strlen(reg_str)) < 0)
		err_sys("Write error");
	
	free(code);	
	
	return 0;
}



int main(int argc, char **argv){
	char *code;
	char *ccode;
	int l;
	init_varibles();
	set_options(argc, argv);
	
	switch(mode){
		case INTERACTIVE_MODE:
			while(1){
				code=get_line();
				if(!strcmp(code, "q\n") || !strcmp(code, "quit\n"))
					break;
				
				if((ccode=compiler(code, &l))==NULL || !strcmp(code, "\n")){
					if(error){
						printf("Error : ");
						error_selector();
					} 
					continue;
				}else{
					execute_str(ccode);
					free(ccode);
				}
			}
		break;
		
		case COMPILER_MODE:
			code = get_code(fd_in);
			ch_append(code, '\n');
			ccode=compiler(code, &l);
			free(code);
			errorpl(l);
			
			if(fd_out==STDOUT)
				ch_append(ccode, '\n');
			if(write(fd_out, ccode, strlen(ccode)) < 0)
				err_sys("Write error");
			free(ccode);
		break;
		
		case EMULATOR_MODE:
			limit=MAX_ADDR+1;
			ccode = read_codefd(bin_file);
			errorp();
			execute(ccode);
			free(ccode);
		break;
		
		case COMPILE_AND_EXECUTE_MODE:
			code = get_code(fd_in);
			ch_append(code, '\n');
			ccode=compiler(code, &l);
			free(code);
			errorpl(l);
			execute(ccode);
		break;
	}

	
	return 0;
}
