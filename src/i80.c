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
#include "../include/logic.h"


int main(int argc, char **argv){
	char *code;
	char *ccode;
	int l;
	init_varibles();
	set_options(argc, argv);
	
	switch(mode){
		
		case INTERACTIVE_MODE:
			printf("To clear the terminal enter : \"clear\"\n"
			"To quit enter : \"q\" or \"quit\"\n");
			
			while(1){
				printf(">> ");
				code=get_line();
				if(!strcmp(code, "q\n") || !strcmp(code, "quit\n"))
					break;
				if(!strcmp(code, "clear\n")){
					system("clear");
					continue;
				}
					
				if((ccode=compiler(code, &l))==NULL || !strcmp(code, "\n")){
					if(error){
						printf("Error : ");
						error_selector();
					} 
					continue;
				}else{
					execute_str(ccode);
					printf("\n");
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
