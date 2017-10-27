#include "../include/unfunc.h"
#include "../include/cpu.h"
#include "../include/selector.h"


void init_varibles(){
	debug=NON_DEBUG; 
	ioutput=HUMAN_OUTPUT;
	output=BYTE_OUTPUT;
	mode=-1; 
	fd_in=NULL; 
	fd_out=-1; 
	bin_file=-1;
	error=0;
	limit=-1;
	REGS_TO_ZERO;
}	

char *machine_print(){
	static char str[120];
	char arg[6];
	
	//
	
	itoa(cf, arg);
	strcat(str, arg);
	strcat(str, " ");
	
	itoa(p, arg);
	strcat(str, arg);
	strcat(str, " ");
	
	itoa(ax, arg);
	strcat(str, arg);
	strcat(str, " ");
	
	itoa(z, arg);
	strcat(str, arg);
	strcat(str, " ");
	
	itoa(p, arg);
	strcat(str, arg);
	strcat(str, " ");
	
	//
	
	itoa(a, arg);
	strcat(str, arg);
	strcat(str, " ");
	
	itoa(b, arg);
	strcat(str, arg);
	strcat(str, " ");
	
	itoa(c, arg);
	strcat(str, arg);
	strcat(str, " ");
	
	itoa(d, arg);
	strcat(str, arg);
	strcat(str, " ");
	
	itoa(e, arg);
	strcat(str, arg);
	strcat(str, " ");
	
	itoa(h, arg);
	strcat(str, arg);
	strcat(str, " ");

	itoa(l, arg);
	strcat(str, arg);
	strcat(str, " ");
	
	//
	
	itoa(pc-begin, arg);
	strcat(str, arg);
	strcat(str, " ");
	
	itoa(sp-begin, arg);
	strcat(str, arg);
	strcat(str, "\n");
	
	return str;
}

char *human_print(){
	static char str[166];
	char arg[6];
	str[0]='\0';
	
	strcat(str, "Flags : cf = ");
	itoa(cf, arg);
	strcat(str, arg);
	
	strcat(str, ", p = ");
	itoa(p, arg);
	strcat(str, arg);
	
	strcat(str, ", ax = ");
	itoa(ax, arg);
	strcat(str, arg);
	
	strcat(str, ", z = ");
	itoa(z, arg);
	strcat(str, arg);
	
	strcat(str, ", s = ");
	itoa(s, arg);
	strcat(str, arg);
	strcat(str, "\n");
	
	
	
	strcat(str, "Registers : a = ");
	itoa(a, arg);
	strcat(str, arg);
	
	strcat(str, ", b = ");
	itoa(b, arg);
	strcat(str, arg);
	
	strcat(str, ", c = ");
	itoa(c, arg);
	strcat(str, arg);
	
	strcat(str, ", d = ");
	itoa(d, arg);
	strcat(str, arg);
	
	strcat(str, ", e = ");
	itoa(e, arg);
	strcat(str, arg);
	
	strcat(str, ", h = ");
	itoa(h, arg);
	strcat(str, arg);
	
	strcat(str, ", l = ");
	itoa(l, arg);
	strcat(str, arg);
	strcat(str, "\n");
	
	

	strcat(str, "Addresses : pc = ");
	itoa(pc-begin, arg);
	strcat(str, arg);

	strcat(str, ", sp = ");
	itoa(sp-begin, arg);
	strcat(str, arg);
	strcat(str, "\n");
	
	return str;
}


int dec_output(char *line, int code, int byte){
	int i=byte-bytes(code);
	char *str = (char *) malloc(bytes(code));
	line[0]='\0';
	
	if(!code && byte==1){
		strcpy(line, "000");
		return 0;
	}
	for(;i>0;i--){
		strcat(line, "0");
	}
	itoa(code, str);
	strcat(line, str);
	free(str);
	return 0;
}


int byte_output(char *line, int code, int byte){
	int j;
	char *str = (char *) malloc(strlen(line));
	char *com = (char *) malloc(strlen(line));
	dec_output(line, code, byte);
	
	if(byte==9){
		for(j=0; j!=3; j++){
			com[j]=line[j];
		}
		com[j]='\0';
		j=0;
		for(int i=3; line[i]!='\0'; i++, j++){
			str[j]=line[i];
		}
		str[j]='\0';
		
		j=atoi(str);
		j=itob(j);
		itoa(j, str);
		strcat(com, str);
		
		strcpy(line, com);
		free(str);
		free(com);
	
	}
	
	strcpy(line, spltob(line));
	return 0;
}

static void write_help()
{
	static const char help_msg[] =
		"Usage: i80 [OPTION]... [FILE]...\n"
		" -h  --help           output this message\n"
		"\n"
		"i80 have four modes :\n"
		"1) -i --inter-mode            interactive mode\n"
		"\n"
		"2) -c --compile [FILE.as]     compiler\n"
		"  * -o --out [FILE]     out file. Without this option compiler will write in \"out\"\n"
		"  * -dec --dec-output   write bin file in deciminal\n"
		"  * -s --stdout         write in stdout\n"
		"\n"
		"3) -e --exec [BIN_FILE]       execute a i8080 bin file\n"
		"  * -o --out [FILE]     write result in a file\n"
		"  * -d --debug          after each command write registers\n"
		"  * -m --machine-print  print out file for others apps\n"
		"\n"
		"4) [FILE.as]                  compile and execute\n"
		"  *arguments like in execute mode\n"
		"\n"
		"Report bugs to <jakushev.rus@gmail.com>\n";
	printf("%s", help_msg);
		
}


static int is_as(char *arg){
	int count=0, i=0;
	while(arg[i]!='\0'){
		if(!count && arg[i]=='.')
			count++;
		else if(count==1 && arg[i]=='a')
			count++;
		else if(count==2 && arg[i]=='s')
			count++;
		else 
			count=0;			
		i++;
	}
	if(count==3)
		return 1;
	return 0;
}

static int is_arg(char *argv){
	if((
		(!strcmp(argv,"-w")   || !strcmp(argv,"--write")) || 
		(!strcmp(argv,"-d")   || !strcmp(argv,"--debug")) ||
		(!strcmp(argv,"-dec") || !strcmp(argv,"--dec-output")) ||
		(!strcmp(argv,"-e")   || !strcmp(argv,"--exec")) ||
		(!strcmp(argv,"-i")   || !strcmp(argv,"--inter-mode")) ||
		(!strcmp(argv,"-s")   || !strcmp(argv,"--stdout")) ||
		(!strcmp(argv,"-o")   || !strcmp(argv,"--out")) ||
		(!strcmp(argv,"-m")   || !strcmp(argv,"--machine-print")) ||
		(!strcmp(argv,"-c")   || !strcmp(argv,"--compile"))
		
	)){
		return 1;
	}
	return 0;
}

static int init_options(int argc, char **argv){
	int one=0;
	int in_files=0;
	if(argc==1){
		printf("The programm have no arguments\n");
		printf("For help, type %s -h\n", argv[0]);
		exit(1);
	}
		
	
	for(int i=1; i<argc; i++){
		
		if(!strcmp(argv[i],"-i") || !strcmp(argv[i],"--interactive")){
			if(mode==-1){
				mode=INTERACTIVE_MODE;
			}else{
				error=MODE_ERROR;
				return -1;
			}
				
		}else if(!strcmp(argv[i],"-m") || !strcmp(argv[i],"--machine-print")){
			if(ioutput!=MACHINE_OUTPUT)
				ioutput=MACHINE_OUTPUT;
			else{
				error=ONE_ARG_TWO_TIMES;
				return -1;
			}
		}else if(!strcmp(argv[i],"-dec") || !strcmp(argv[i],"--dec-output")){
			if(output!=DEC_OUTPUT)
				output=DEC_OUTPUT;
			else{
				error=ONE_ARG_TWO_TIMES;
				return -1;
			}
		}else if(!strcmp(argv[i],"-d") || !strcmp(argv[i],"--debug")){
			if(debug!=DEBUG)
				debug=DEBUG;
			else{
				error=ONE_ARG_TWO_TIMES;
				return -1;
			}
		}else if(!strcmp(argv[i],"-s") || !strcmp(argv[i],"--stdout")){
			if(fd_out==-1)
				fd_out=STDOUT;
			else{
				error=ONE_ARG_TWO_TIMES;
				return -1;
			}	
		}else if(!strcmp(argv[i],"-dm") || !strcmp(argv[i],"-md")){ 
			if(debug!=DEBUG)
				debug=DEBUG;
			else{
				error=ONE_ARG_TWO_TIMES;
				return -1;
			}
			if(ioutput!=MACHINE_OUTPUT)
				ioutput=MACHINE_OUTPUT;
			else{
				error=ONE_ARG_TWO_TIMES;
				return -1;
			}
		}else if(!strcmp(argv[i],"-sdec") || !strcmp(argv[i],"-decs")){
			if(fd_out==-1)
				fd_out=STDOUT;
			else{
				error=ONE_ARG_TWO_TIMES;
				return -1;
			}	
			if(output!=DEC_OUTPUT)
				output=DEC_OUTPUT;
			else{
				error=ONE_ARG_TWO_TIMES;
				return -1;
			}
		}else if(!strcmp(argv[i], "-o") || !strcmp(argv[i], "--out")){
			i++;
			if(argc<=i || fd_out!=-1){
				error = ARGUMENT_ERROR;
				return -1;
			}
			fd_out = wropen(argv[i]);
		}else if(!strcmp(argv[i],"-e") || !strcmp(argv[i],"--exec")){
			if(mode==-1)
				mode=EMULATOR_MODE;
			else{
				error=MODE_ERROR;
				return -1;
			}
			i++;

			if(argc<=i){
				error = ARGUMENT_ERROR;
				return -1;
			}else if(bin_file!=-1){
				error=ONE_ARG_TWO_TIMES;
				return -1;
			}
			bin_file = rdopen(argv[i]);
		}else if(!strcmp(argv[i],"-h") || !strcmp(argv[i],"--help")){
			write_help();
			exit(1);
		}else{										// *.as
			
			int m;
			if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--compile")){
				m=COMPILER_MODE;
				i++;
			}else{ 
				if(argv[i][0]=='-'){
					error=ARGUMEN_NOT_FOUND;
					return -1;
				}	
				m=COMPILE_AND_EXECUTE_MODE;
			}
			if(mode==-1){
				mode=m;
			}else{
				error=MODE_ERROR;
				return -1;
			}	
			
			while(i<argc){
				if(is_arg(argv[i])){
					i--;
					break;
				}
				if(!is_as(argv[i])){
					error=IS_NOT_SOURCE_CODE;
					return -1;
				}
				fd_in = realloc(fd_in, (in_files+1)*4);
				fd_in[in_files] = rdopen(argv[i]);
				
				one++;
				i++;
				in_files++;
			}
			fd_in = realloc(fd_in, (in_files+1)*4);
			fd_in[in_files]=-1;
			
			if(!one){
				error=ARGUMENT_ERROR;
				return -1;
			}
			one=0;
		
		}
	}
	return 0;
}


static int order_options(){
	
	switch(mode){
		case INTERACTIVE_MODE:
			if(fd_out!=-1 || fd_in!=NULL || 
			bin_file!=-1 || debug!=NON_DEBUG || 
			ioutput!=HUMAN_OUTPUT || output!=BYTE_OUTPUT){
				error=NOT_RIGHT_ARGUMENT_FOR_THIS_MODE;
				return -1;
			}
			fd_out=STDOUT;
			out_order=byte_output;
			write_regs = human_print;
			
		break;
		case COMPILER_MODE:
			if(fd_in==NULL || bin_file!=-1 
			|| debug!=NON_DEBUG || ioutput!=HUMAN_OUTPUT){
				
				error=NOT_RIGHT_ARGUMENT_FOR_THIS_MODE;
				return -1;
			}
			if(fd_out==-1)
				if((fd_out = open("out.r", O_RDONLY | O_CREAT, FILE_MODE)) < 0)
					err_sys("Can't open file out.r");
			
			if(output==DEC_OUTPUT)
				out_order=dec_output;
			else 
				out_order=byte_output;
		break;
		case EMULATOR_MODE:
		case COMPILE_AND_EXECUTE_MODE:
			if((bin_file == -1 && mode==EMULATOR_MODE) || 
			(fd_in==NULL && mode==COMPILE_AND_EXECUTE_MODE) ||
			output!=BYTE_OUTPUT || fd_out==STDOUT){
				error=NOT_RIGHT_ARGUMENT_FOR_THIS_MODE;
				return -1;
			}
			
			if(fd_out==-1)
				fd_out=STDOUT;
			
			if(ioutput == MACHINE_OUTPUT)
				write_regs = machine_print;
			else 
				write_regs = human_print;
			
			if(mode==COMPILE_AND_EXECUTE_MODE){
				if(output==DEC_OUTPUT)
					out_order=dec_output;
				else 
					out_order=byte_output;
			}
		break;
		default :
			error=MODE_ERROR;
			return -1;
		break;
	}
	return 0;
}

void set_options(int argc, char **argv){
	int i=0;
	if((i=init_options(argc, argv))!=-1){
		order_options();
		
	}

	errorp();
}
