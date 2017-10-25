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

char *get_code(int *fd){
	limit=-1;
	char *code=NULL, *temp;
	while(*fd!=-1){
		temp = read_codefd(*fd);
		code = realloc(code, strlen(temp));
		strcat(code, temp);
		free(temp);
		close(*fd++);
	}
	
	ch_append(code, '\n');
	return code;
}

char *get_line(){
	int i=0;
	char c;
	char *str = (char *) malloc(20);
	while((c=getchar())!=EOF){
		i++;
		if(c=='\n')
			break;
		else
			*str++=c;
		
		if(i==20){
			str = realloc(str, strlen(str)+i);
		}
	}
	if(!(strlen(str)%20))
		str = realloc(str, strlen(str)+1);
	*str='\0';
	return str;
}


int compiler(char *code){
	char *line, *text, *ccode;
	int l, i, n, byte, li, len;
	text=NULL;
	text = (char *) malloc(strlen(code));
	strcpy(text, code);
	
	ccode = genschtalt(text, &l);
	if(error)
		return l;
	
	free(text);
	
	code=NULL;
	l=1;
	
	while(1){
		line = NULL;
		li=len=0;
		
		while(*ccode!='\n'){							//bug in get_line
			if(*ccode=='\0')
				goto end;
			line = realloc(line, len+1);
			line[li++]=*ccode++;
			len++;
		}
		ccode++;
		line = realloc(line, len+1);
		line[li]='\0';
		
		if(!len){
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
		
		code = realloc(code, len+1);	
		strcat(code, line);
		
		l++;
		free(line);
	}
	end:
	if(error)
		return l;
	if(fd_out==STDOUT){
		code = realloc(code, strlen(code)+1);
		strcat(code, "\n");
	}
	free(ccode);
	return 0;
	
}

int execute(char *bin){
	int ret;
	int *code;
	char *reg_str, *com;
	error=0;
	
	code = order_to_int(bin);
	free(bin);
	
	code = realloc(code, (MAX_ADDR+1)*sizeof(int));

	begin=pc=code;
	end=sp=code+MAX_ADDR;

	while(1){
		if((ret = exec(*pc))==-1){
			printf("Error by executintg instruction number %i. Command: %s\n", prev_com, (com=num_to_com(*(begin+prev_com))));
			free(com);
			break;
		}else if(!ret)
			break;
		
		if(debug==DEBUG){
			if((reg_str=write_regs())!=NULL)
				if(write(fd_out, reg_str, strlen(reg_str)) < 0)
					err_sys("Write error");
			free(reg_str);	
		}
	}
	
	if((reg_str=write_regs())!=NULL && debug==NON_DEBUG)
		if(write(fd_out, reg_str, strlen(reg_str)) < 0)
			err_sys("Write error");
	
	free(code);	
	return 0;
}


int main(int argc, char **argv){
	char *code=NULL;
	int l;
	//init_varibles();
	set_options(argc, argv);
	
	switch(mode){
		case INTERACTIVE_MODE:
			while(1){
				code=get_line();
				if(!strcmp(code, "q") || !strcmp(code, "quit"))
					break;
				if(compiler(code)){
					printf("Error : ");
					error_selector();
				}else
					execute(code);
				free(code);
			}
		break;
		
		case COMPILER_MODE:
			code = get_code(fd_in);
			l=compiler(code);
			errorpl(l);
			free(code);
		break;
		
		case EMULATOR_MODE:
			limit=MAX_ADDR+1;
			code = read_codefd(bin_file);
			errorp();
			execute(code);
			free(code);
		break;
		
		case COMPILE_AND_EXECUTE_MODE:
			code = get_code(fd_in);
			compiler(code);
			execute(code);
			free(code);
		break;
	}
	return 0;
}
