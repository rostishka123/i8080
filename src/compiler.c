#include "../include/compiler.h"
#include "../include/unfunc.h"

int quantity_args(char *line){
	char c;
	int esc=0;
	while((c = *line++)!='\0')
		if(c==' ')
			esc++;
	
	if(esc >= 0 && esc<=2)
		return esc;
	return -1;
}


static int is_label(char *str){
	if(is_keyword(str) || !(NOT_KOMENT))
		return 0;
	return 1;
}

static int search_lab(char *arg, struct lab labels[]){
	for(int i=0; labels[i].label[0]!='\0'; i++)
		if(!strcmp(labels[i].label, arg))
			return labels[i].addr;
	
	return -1;	
}

static int label(char *arg, int option, int n){
	static struct lab labels[MAX_LAB];
	static int i=0;
	labels[i].label[0]='\0';
	
	if(!option){
		if(!is_label(arg))
			return -1;
		
		return search_lab(arg, labels);
	}else{
		if(!is_label(arg) || !(n>=MIN_ADDR && n<=MAX_ADDR) || search_lab(arg, labels)!=-1)
			return -1;
		strcpy(labels[i].label, arg);
		labels[i].addr=n;
		return i++;
	}
	
}

static char *escapes(char *line){
	if(!strcmp(line, "\n")){
		return NULL;
	}
	char *temp = (char *) malloc(strlen(line) + 1);
	int i, j, word;
	char c, begin, count, comma, colon, text, place, colon_d, comma_d;
	i = j = text = word = comma = begin = colon = count = place = colon_d = comma_d = 0;
	
	while(!(((c = line[i]))=='\0' || c==';' || c=='\n') ){
		if(!begin && (c==' ' || c=='\t'))       
			continue;
		else 
			begin=1;
		
		if(c=='\t')
			c=' ';
		if(c==','){
			if(comma_d){
				error=COMMA_ERROR;
				return NULL;
			}
			c=' ';
			comma=comma_d=1;
		}else if(c==':'){
			if(colon_d){
				error=COLON_ERROR;
				return NULL;
			}
			c=' ';
			colon=colon_d=1;	
		}
		
		if(text && c==' '){
			word++;
			text=0;
		}else if(c!=' '){
			if(!text){
				place++;
			}
			text=1;
		}
		
		if(colon_d && place!=0){
			error=COLON_ERROR;
			return NULL;
		}else
			colon=0;
		
		if(comma && word!=2 ){
			error=COMMA_ERROR;
			return NULL;
		}else{
			if(c!=' ')
				comma=0;
		}
		
		if(c==' '){ 					
			count++;
		}else{
			if(count){
				temp[j++]=' ';				
			}
			temp[j++]=c;
			count=0;
		}
		i++;	
	}
	
	if(comma){
		error=COMMA_ERROR;
		return NULL;	
	}
		
	temp[j]='\0';
	return temp;
}

static int zero_arg(char *line){
	const char command[28][5]={"cma", "cmc", "daa", "di", "ei", "hlt", "nop", "pchl", "ral", "rar", "rlc", "rrc", "rim", "ret", "rz", "rnz", "rp", "rm", "rc", "rnc", "rpe", "rpo", "sim", "sphl", "stx", "xchg", "xthl", "\0"};
	const int value[]={CMA, CMC, DAA, DI, EI, HLT, NOP, PCHL, RAL, RAR, RLC, RRC, RIM, RET, RZ, RNZ, RP, RM, RC, RNC, RPE, RPO, SIM, SPHL, STC, XCHG, XTHL};
	int i=0;
	
	while(strcmp(command[i], "\0")!=0){
		if(!strcmp(command[i], line))
			break;
		i++;
	}
	
	if(!strcmp(command[i], "\0"))
		return -1;
	
	return value[i];
}

static int check_without_num(char *line){
	int i;
	const char commands[113][8]={
	"add a", "add b", "add c", "add d", "add e", "add h", "add l", "add m",
	"adc a", "adc b", "adc c", "adc d", "adc e", "adc h", "adc l", "adc m",
	"ana a", "ana b", "ana c", "ana d", "ana e", "ana h", "ana l", "ana m",
	"cmp a", "cmp b", "cmp c", "cmp d", "cmp e", "cmp h", "cmp l", "cmp m",
	"dad b", "dad d", "dad h", "dad sp",
	"dcr a", "dcr b", "dcr c", "dcr d", "dcr e", "dcr h", "dcr l", "dcr m", 
	"dcx b", "dcx d", "dcx h", "dcx sp", 
	"inr a", "inr b", "inr c", "inr d", "inr e", "inr h", "inr l", "inr m", 
	"inx b", "inx d", "inx h", "inx sp", 
	"ora a", "ora b", "ora c", "ora d", "ora e", "ora h", "ora l", "ora m", 
	"pop b", "pop d", "pop h", "pop psw", 
	"push b", "push d", "push h", "push psw",
	"sub a", "sub b", "sub c", "sub d", "sub e", "sub h", "sub l", "sub m", 
	"sbb a", "sbb b", "sbb c", "sbb d", "sbb e", "sbb h", "sbb l", "sbb m", 
	"xra a", "xra b", "xra c", "xra d", "xra e", "xra h", "xra l", "xra m",
	"rst 1", "rst 2", "rst 3", "rst 4", "rst 5", "rst 6", "rst 7", "rst 0", 
	"stax b", "stax d",
	"ldax b", "ldax d",
	"\0"};
	
	const int values[]={
	ADD_A, ADD_B, ADD_C, ADD_D, ADD_E, ADD_H, ADD_L, ADD_M,
	ADC_A, ADC_B, ADC_C, ADC_D, ADC_E, ADC_H, ADC_L, ADC_M,
	ANA_A, ANA_B, ANA_C, ANA_D, ANA_E, ANA_H, ANA_L, ANA_M,
	CMP_A, CMP_B, CMP_C, CMP_D, CMP_E, CMP_H, CMP_L, CMP_M,
	DAD_B, DAD_D, DAD_H, DAD_SP,
	DCR_A, DCR_B, DCR_C, DCR_D, DCR_E, DCR_H, DCR_L, DCR_M,
	DCX_B, DCX_D, DCX_H, DCX_SP,
	INR_A, INR_B, INR_C, INR_D, INR_E, INR_H, INR_L, INR_M,
	INX_B, INX_D, INX_H, INX_SP,
	ORA_A, ORA_B, ORA_C, ORA_D, ORA_E, ORA_H, ORA_L, ORA_M,
	POP_B, POP_D, POP_H, POP_PSW,
	PUSH_B, PUSH_D, PUSH_H, PUSH_PSW,
	SUB_A, SUB_B, SUB_C, SUB_D, SUB_E, SUB_H, SUB_L, SUB_M, 
	SBB_A, SBB_B, SBB_C, SBB_D, SBB_E, SBB_H, SBB_L, SBB_M, 
	XRA_A, XRA_B, XRA_C, XRA_D, XRA_E, XRA_H, XRA_L, XRA_M, 
	RST_1, RST_2, RST_3, RST_4, RST_5, RST_6, RST_7, RST_0, 
	STAX_B, STAX_D, 
	LDAX_B, LDAX_D 
	};
	
	for(i=0; strcmp(line, commands[i]); i++)
		if(!strcmp(commands[i], "\0"))
			return -1;
	return values[i];
}	

static int check_num_com(char *line){
	int i;
	const char commands[11][4]={"adi", "aci", "ani", "cpi", "in", "ori", "out", "sui", "sbi", "xri", "\0"};
	const int values[]={ADI, ACI, ANI, CPI, IN, ORI, OUT, SUI, SBI, XRI};
	for(i=0; strcmp(line, commands[i]); i++)
		if(!strcmp(commands[i], "\0"))
			return -1;
	return values[i];
}

static int check_jump_com(char *line){
	int i;
	const char commands[23][5]={
	"call", "cz", "cnz", "cp", "cm", "cc", "cnc", "cpe", "cpo", 
	"jmp", "jz", "jnz", "jp", "jm", "jc", "jnc", "jpe", "jpo",
	"lda", "lhld", "shld", "sta", "\0"
	};
	const int values[]={
	CALL, CZ, CNZ, CP, CM, CC, CNC, CPE, CPO,
	JMP, JZ, JNZ, JP, JM, JC, JNC, JPE, JPO, 
	LDA, LHLD, SHLD, STA
	};
	for(i=0; strcmp(line, commands[i]); i++)
		if(!strcmp(commands[i], "\0"))
			return -1;
	return values[i];
}

static int make_command(int com, int arg, unsigned int count){
	return count * com + arg;
}
	
static int is_num(char *num){
	while(*num){
		if(!(*num>='0' && *num<='9'))
			return 0;
		num++;
	}
	return 1;
}

static int ntod(char *arg){		//number to dec
	
	char num[MAXLINE], c;
	int h=0;

	if(arg==NULL)
		return 0;
		
	int i=0, j=0;
	if(arg[0]=='0' && arg[1]=='x'){
		h=1;
		i=2;
	}
	
	while((c=arg[i])!='\0'){
		if(c=='b' || c=='h')
			break;
		num[j]=c;
		j++;
		i++;
	}
	num[j]='\0';
	
	if(arg[i]=='h' || h)
		return hex(num);
	else if(arg[i]=='b')
		return bin(num);
	else if(arg[i]=='\0' && is_num(arg))
		return atoi(num);
	else return -1;
}

static int one_arg(char *line, int *byte){
	char com[MAXLINE], arg[MAXLINE];
	int i, a;
	split(line, com, arg);

	if((a=ntod(arg))==-1){
		if(!error)
			error = NOT_RIGHT_NUMBER;
		return -1;
	}
	
	a=atoi(arg);	
	
	*byte=0;
	if((i=check_without_num(line))!=-1){
		*byte=com_bytes(i);
		return i;
	}else if((i=check_num_com(com))!=-1){
		if(a<0)
			a = MAX_NUM + neg(a);
		if(i==IN || i==OUT)
			if(a>=MIN_PORT && a<=MAX_PORT){ 
				*byte=com_bytes(i);
				return make_command(i, a, NUM_COUNT);
			return -1;
		}
		if(!(a>=MIN_REAL_NUM && a<=MAX_REAL_NUM))
			return -1;
		*byte=com_bytes(i);
		return make_command(i, a, NUM_COUNT);
			
	}else if((i=check_jump_com(com))!=-1){
		if(strisdigit(arg) && a>=MIN_ADDR && a<=MAX_ADDR){
			*byte=com_bytes(i);
			return make_command(i, a, ADDR_COUNT);
		}else if(!strisdigit(arg))
			if((a=label(arg, 0, 0))!=-1){
				*byte=com_bytes(i);
				return make_command(i, a, ADDR_COUNT);
			}
	}
	return -1;
}

static int movc(char *line){
	int i;
	char mov[][8]={
	"mov a a", "mov a b", "mov a c", "mov a d", "mov a e", "mov a h", "mov a l", "mov a m", 
	"mov b a", "mov b b", "mov b c", "mov b d", "mov b e", "mov b h", "mov b l", "mov b m", 
	"mov c a", "mov c b", "mov c c", "mov c d", "mov c e", "mov c h", "mov c l", "mov c m", 
	"mov d a", "mov d b", "mov d c", "mov d d", "mov d e", "mov d h", "mov d l", "mov d m", 
	"mov e a", "mov e b", "mov e c", "mov e d", "mov e e", "mov e h", "mov e l", "mov e m", 
	"mov h a", "mov h b", "mov h c", "mov h d", "mov h e", "mov h h", "mov h l", "mov h m", 
	"mov l a", "mov l b", "mov l c", "mov l d", "mov l e", "mov l h", "mov l l", "mov l m", 
	"mov m a", "mov m b", "mov m c", "mov m d", "mov m e", "mov m h", "mov m l", "\0"
	};
	
	const int value[]={
	0x7f, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e,
	0x47, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 
	0x4f, 0x48, 0x49, 0x4a, 0x4b, 0x5c, 0x5d, 0x4e,
	0x57, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56,
	0x5f, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e,
	0x67, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
	0x6f, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
	0x77, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
	};
	
	for(i=0; strcmp(mov[i], line); i++)
		if(!strcmp(mov[i], "\0")!=0)
			return -1;
	return value[i];
}

static int mvic(char *com, char *arg){
	int i, a;
	const char mvi[][6]={"mvi a", "mvi b", "ana c", "mvi d", "mvi e", "mvi h", "mvi l", "mvi m", "\0"};
	const int value[]={0x3e, 0x6, 0xe, 0x16, 0x1e, 0x26, 0x2e, 0x36};
	for(i=0; strcmp(mvi[i], com)!=0; i++)
		if(!strcmp(mvi[i], "\0"))
			return -1;
	i=value[i];
	a=atoi(arg);
	if(a>=MIN_NUM && a<=MAX_NUM)
		return make_command(i, a, NUM_COUNT);
	return -1;
}

static int lxic(char *com, char *arg){
	int i, a;
	const char lxi[][7]={"lxi b", "lxi h", "lxi sp", "\0"};
	const int value[]={0x1, 0x21, 0x31};
	
	for(i=0; strcmp(lxi[i], com)!=0; i++)
		if(!strcmp(lxi[i], "\0"))
			return -1;

	i=value[i];
	a=atoi(arg);
	if(a>=MIN_WORD && a<=MAX_WORD)
		return make_command(i, a, ADDR_COUNT);		
	return -1;
}

static int two_arg(char *line, int *byte){
	char com[MAXLINE], arg[MAXLINE];
	int i;
	

	split_two(line, com, arg);	
	
	if((i=movc(line))!=-1){
		*byte=1;
		return i;
	}else if((i=mvic(com, arg))!=-1){
		*byte=2;
		return i;
	}else if((i=lxic(com, arg))!=-1){
		*byte=3;
		return i;
	}
	return -1;
}

int compile(char *line, int n, int *byte){
	int ret=0;
	
	switch(n){
		case 0:
			ret = zero_arg(line);
			*byte=1;
		break;
		case 1:
			if((ret = one_arg(line, byte))== -1)
				if(!error)
					error=ERROR;
			
		break;
		case 2:
			ret = two_arg(line, byte);
		break;
		default :
			error=TOO_MANY_WORDS;
			*byte=0;
			return -1;
	}	
	return ret;
}

static int bytes_com(char *line, int n){
	int byte, ret;
	switch(n){
		case 0:
			if((ret = zero_arg(line))== -1)
				return ret;		
			return 1;
		break;
		case 1:
			if((ret = one_arg(line, &byte))== -1)
				return ret;	
		break;
		case 2:
			if((ret = two_arg(line, &byte))== -1)
				return ret;	
		break;
		default :
			return -1;		
	}
	return byte;
}

char *genschtalt(char *text, int *lines){
	char *code = (char *) malloc(strlen(text));
	char *line;
	int i, l=1, addr=0, n, li, len;

	while(1){
		line = NULL;
		li=len=0;
		if(*text=='\0')
			break;
		while(*text!='\n'){							//bug in get_line
			
			line = realloc(line, len+1);
			line[li++]=*text++;
			len++;
		}

		line = realloc(line, len+1);
		line[li]='\0';
		text++;
		
		if(!len){
			l++;
			continue;
		}
		
		if(strlen(line)>MAXLINE){
			error=STRING_TOO_LONG;
			return NULL;
		}
		
		strtolower(line);
		
		if((line=escapes(line))==NULL){
			if(error)
				return NULL;
			l++;
			continue;
		}else if(i==-1){
			error=ERROR;
			if(lines!=NULL)
				*lines=l;
			return NULL;
		}
		
		n = quantity_args(line);	
		if(n == 0 && is_label(line)){
			label(line, 0, addr);
			continue;
		}
		
		if((i=bytes_com(line, n))==-1){
			error=COMMAND_NOT_FOUND;
			if(lines!=NULL)
				*lines=l;
			return NULL;
		}
		strcat(code, line);
		strcat(code, "\n");
		
		addr+=i;
		free(line);
		l++;
		
	}
	strcat(code, "\n");
	if(lines!=NULL)
		*lines=0;

	return code;
}


