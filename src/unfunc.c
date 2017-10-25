#include "../include/unfunc.h"


void err_doit(int errnoflag, int error, const char *fmt, va_list ap){
	char buf[MAXLINE];
	vsnprintf(buf, MAXLINE, fmt, ap);
	if(errnoflag)
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf), ": %s", strerror(error));
	strcat(buf, "\n");
	fflush(stdout);
	fputs(buf, stderr);
	fflush(NULL);
}

void err_sys(const char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	exit(1);
}

char *read_codefd(int fd){
	ssize_t stat;
	size_t buf_sz = 0;
	char *code = NULL;
	char buf[MAX_SIZE];
	
	while((stat = read(fd, buf, MAX_SIZE)) > 0){
		code = realloc(code, buf_sz + stat + 1);
		if(code == NULL)
			err_sys("Memory exchausted");
		memcpy(code + buf_sz, buf, stat);
		code[buf_sz += stat] = '\0';

		if(limit!=-1 && buf_sz>limit){
			error=LIMIT_ERROR;
			return NULL;
		}
			
	}
	
	if(stat==-1)
		err_sys("Read error");
	if(limit>0)
		code = realloc(code, limit+1);
	close(fd);
	return code;
}

int is_keyword(char *line){
	int i;
	const char keywords[93][5]={
		"add", "adi", "adc", "aci", "ana", "ani", "call", "cz", "cnz", "cp", 
		"cm", "cc", "cnc", "cpe", "cpo", "cma", "cmc", "cmp", "cpi", "daa", 
		"dad", "dcr", "dcx", "di", "ei", "inr", "inx", "jmp", "jz", "jnz", 
		"jp", "jm", "jc", "jnc", "jpe", "jpo", "lda", "ldax", "lhld", "lxi", 
		"mov", "mvi", "nop", "ora", "ori", "out", "pchl", "pop", "push", "ral", 
		"rar", "rlc", "rrc", "rim", "ret", "rz", "rnz", "rp", "rm", "rc", "rnc", 
		"rpe", "rpo", "rst", "sim", "shld", "sta", "stax", "stc", "sub", "sui", 
		"sbb", "sbi", "xthl", "xra", "xri", "hlt", "a", "b", "c", "d", "h", "l", "m", "sp",
		"pc", "psw", "bc", "de", "hl", "\0"
		};
		
	for(i=0; strcmp(keywords[i], line)!=0; i++)
		if(!strcmp(keywords[i], "\0"))
			return 0;
	return 1;
}

char *read_code(const char *arg){
	int fd;
	if((fd = open(arg, O_RDONLY)) < 0)
		err_sys("Can't open file %s", arg);
	
	return read_codefd(fd);
}


int strisdigit(char *str){
	while(*str){
		if(!(*str >= '0' && *str <= '9' ))
			return 0;
		str++;
	}
	return 1;
}

void strtolower(char *line){
	char *temp = (char *) malloc(strlen(line) + 1);
	int i;
	for(i=0; line[i]!='\0'; i++){
		temp[i]=tolower(line[i]);	
	}
	temp[i]='\0';
	strcpy(line, temp);
	free(temp);
}

void reverse(char s[]){
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
} 

int neg(int i){
	return i*-1;
}

void itoa(int n, char *s){
    int i, sign;

	if ((sign = n) < 0)
        n = -n;       
    i = 0;
    do{
        s[i++] = n % 10 + '0'; 
    }while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}
 
//Yes, i know about existing sizeof()
int bytes(int n){		
	int i=0;
	do{
		i++;
    }while ((n /= 10) > 0);
	return i;
}


void ch_append(char *str, char c){
	const char s[]={c, '\0'};
	str = realloc(str, strlen(str)+1);
	strcat(str, s);
}

void split(char line[], char com[], char arg[]){
	int i, j;
	for(j = i = 0; line[i]!=' ' ; i++, j++)
		com[j]=line[i];
	com[j]='\0';
	i++;
	for(j = 0; line[i]!='\0' ; i++, j++)
		arg[j]=line[i];
	arg[j]='\0';
	
}

void split_two(char *line, char com[], char arg[]){
	int i, j, count=0;
	for(j = i = 0; line[i]!='\0'; i++, j++){
		if(line[i]==' ' && count)
			break;
		if(line[i]==' ')
			count++;
		com[j]=line[i];
	}
	com[j]='\0';
	i++;
	for(j = 0; line[i]!='\0' ; i++, j++)
		arg[j]=line[i];
	arg[j]='\0';
}


int com_bytes(int c){
	if(c==-1)
		return 0;
	if( 
	c == CMA    || c == CMC    || c == DAA    	|| c == DI    	|| c == EI     || 
	c == HLT    || c == NOP    || c == PCHL   	|| c == RAL   	|| c == RAR    || 
	c == RLC    || c == RRC    || c == RIM    	|| c == RET   	|| c == RZ     || 
	c == RNZ    || c == RP     || c == RM     	|| c == RC    	|| c == RNC    || 
	c == RPE    || c == RPO    || c == SIM    	|| c == SPHL  	|| c == STC    || 
	c == XCHG   || c == XTHL   || c == ADD_A  	|| c == ADD_B 	|| c == ADD_C  || 
	c == ADD_D  || c == ADD_E  || c == ADD_H  	|| c == ADD_L 	|| c == ADD_M  || 
	c == ADC_A  || c == ADC_B  || c == ADC_C  	|| c == ADC_D 	|| c == ADC_E  || 
	c == ADC_H  || c == ADC_L  || c == ADC_M  	|| c == ANA_A 	|| c == ANA_B  || 
	c == ANA_C  || c == ANA_D  || c == ANA_E  	|| c == ANA_H 	|| c == ANA_L  || 
	c == ANA_M  || c == CMP_A  || c == CMP_B  	|| c == CMP_C 	|| c == CMP_D  || 
	c == CMP_E  || c == CMP_H  || c == CMP_L  	|| c == CMP_M 	|| c == DAD_B  || 
	c == DAD_D  || c == DAD_H  || c == DAD_SP 	|| c == DCR_A 	|| c == DCR_B  || 
	c == DCR_C  || c == DCR_D  || c == DCR_E  	|| c == DCR_H 	|| c == DCR_L  || 
	c == DCR_M  || c == DCX_B  || c == DCX_D  	|| c == DCX_H 	|| c == DCX_SP || 
	c == INR_A  || c == INR_B  || c == INR_C  	|| c == INR_D 	|| c == INR_E  || 
	c == INR_H  || c == INR_L  || c == INR_M  	|| c == INX_B 	|| c == INX_D  || 
	c == INX_H  || c == INX_SP || c == ORA_A 	|| c == ORA_B 	|| c == ORA_C  ||
	c == ORA_D  || c == ORA_E  || c == ORA_H 	|| c == ORA_L 	|| c == ORA_M  || 
	c == POP_B  || c == POP_D  || c == POP_H 	|| c == POP_PSW || c == PUSH_B || 
	c == PUSH_D || c == PUSH_H || c == PUSH_PSW || c == SUB_A 	|| c == SUB_B  || 
	c == SUB_C  || c == SUB_D  || c == SUB_E  	|| c == SUB_H 	|| c == SUB_L  || 
	c == SUB_M  || c == SBB_A  || c == SBB_B  	|| c == SBB_C 	|| c == SBB_D  || 
	c == SBB_E  || c == SBB_H  || c == SBB_L  	|| c == SBB_M 	|| c == STAX_B || 
	c == STAX_D || c == LDAX_B || c == LDAX_D 	|| c == XRA_A 	|| c == XRA_B  || 
	c == XRA_C  || c == XRA_D  || c == XRA_E  	|| c == XRA_H 	|| c == XRA_L  || 
	c == XRA_M  || c == RST_0  || c == RST_1  	|| c == RST_2 	|| c == RST_3  || 
	c == RST_4  || c == RST_5  || c == RST_6  	|| c == RST_7	||
	c==MOV_A_A || c==MOV_A_B || c==MOV_A_C || c==MOV_A_D || c==MOV_A_E || c==MOV_A_H || c==MOV_A_L || c==MOV_A_M || 
	c==MOV_B_A || c==MOV_B_B || c==MOV_B_C || c==MOV_B_D || c==MOV_B_E || c==MOV_B_H || c==MOV_B_L || c==MOV_B_M || 
	c==MOV_C_A || c==MOV_C_B || c==MOV_C_C || c==MOV_C_D || c==MOV_C_E || c==MOV_C_H || c==MOV_C_L || c==MOV_C_M || 
	c==MOV_D_A || c==MOV_D_B || c==MOV_D_C || c==MOV_D_D || c==MOV_D_E || c==MOV_D_H || c==MOV_D_L || c==MOV_D_M || 
	c==MOV_E_A || c==MOV_E_B || c==MOV_E_C || c==MOV_E_D || c==MOV_E_E || c==MOV_E_H || c==MOV_E_L || c==MOV_E_M || 
	c==MOV_H_A || c==MOV_H_B || c==MOV_H_C || c==MOV_H_D || c==MOV_H_E || c==MOV_H_H || c==MOV_H_L || c==MOV_H_M || 
	c==MOV_L_A || c==MOV_L_B || c==MOV_L_C || c==MOV_L_D || c==MOV_L_E || c==MOV_L_H || c==MOV_L_L || c==MOV_L_M || 
	c==MOV_M_A || c==MOV_M_B || c==MOV_M_C || c==MOV_M_D || c==MOV_M_E || c==MOV_M_H || c==MOV_M_L
){
		return 1;
	} 
	if(
	c ==  ADI || c == ACI || c == ANI || c == CPI || c == IN || c == ORI || c == OUT || c == SUI || c == SBI || 
	c == XRI  || c==MVI_A || c==MVI_B || c==MVI_C || c==MVI_D || c==MVI_E || c==MVI_H || c==MVI_L || c==MVI_M
	){
		return 2;
	}
	if(
	c ==  CALL || c == CZ || c == CNZ || c == CP || c == CM || c == CC || c == CNC || c == CPE || c == CPO || 
	c == JMP   || c == JZ || c == JNZ || c == JP || c == JM || c == JC || c == JNC || c == JPE || c == JPO || 
	c == LDA || c == LHLD || c == SHLD || c == STA || c==LXI_B || c==LXI_H || c==LXI_SP){
		return 3;
	}
	return 0;
}

int wropen(char *arg){
	int fd;
	if((fd = unlink(arg))==-1)
		if(errno!=ENOENT) 
			err_sys("Error by cleaning file %s", arg);
	
	if((fd = open(arg, O_WRONLY | O_CREAT, FILE_MODE)) < 0)
		err_sys("Can't open file %s", arg);
		
	return fd;
}

int rdopen(char *arg){
	int fd;
	if((fd = open(arg, O_RDONLY)) < 0)
		err_sys("Can't open file %s", arg);
	
	return fd;	
}

int powi(int num, int n){
	int ret=1;
	if(num==1)
		return 1;
	for(int i=1; i<=n; i++){
		ret*=num;
	}
	return ret;
}

int bin(char *num){
	int ret=0, c;
	int j=strlen(num)-1;
	for(int i = 0; i<strlen(num); i++, j--){
		if((c=(num[i]-'0'))!=0 && c!=1){
			error=NOT_BIN_NUN;
			return -1;
		}
		ret += powi(2, j) * c;
	}
	
	return ret;
}

int ch_hex(char n){
	if(n>='0' && n<='9')
		return n-'0';
	if(n>='a' && n<='z')
		return n - 'a' + 10; 
	return -1; 
}

int hex(char *num){
	int ret=0,c;
	int j=strlen(num)-1;
	for(int i = 0; i<strlen(num); i++, j--){
		if((c=ch_hex(num[i]))==-1){
			error=NOT_HEX_NUM;
			return -1;
		}
		ret += powi(16, j) * c;
	}
	return ret;
}


int itob(unsigned short int num){  				//int to byte
	return ((num>>8) *1000) + (num & 0xff);
}

char *spltob(char *line){
		char *str = (char *) malloc(4);
		char *ret=NULL;
		
		int j=0, y=0, i=0;
		char c;
		while(1){
			if(j==3){
				str[j]='\0';
				ret = realloc(ret, y+1);
				ret[y++]=atoi(str);
				free(str);
				str = (char *) malloc(4);
				j=0;
			}else
				str[j++] = (c=line[i++]);
			
			if(c=='\0')
				break;
		}
		ret[y]='\0';
		return ret;
}


char *num_to_com(int n){
	char *kom = (char *) malloc(5);
	
	switch(n){
		case 0x2f:
			strcpy(kom ,"cma");
		break;
		case 0x3f:
			strcpy(kom ,"cmc");
		break;
		case 0x27:
			strcpy(kom ,"daa");
		break;
		case 0xf3:
			strcpy(kom ,"di");
		break;
		case 0xfb:
			strcpy(kom ,"ei");
		break;
		case 0x76:
			strcpy(kom ,"hlt");
		break;
		case 0:
			strcpy(kom ,"nop");
		break;
		case 0xe9:
			strcpy(kom ,"pchl");
		break;
		case 0x17:
			strcpy(kom ,"ral");
		break;
		case 0x1f:
			strcpy(kom ,"rar");
		break;
		case 0x07:
			strcpy(kom ,"rlc");
		break;
		case 0x0f:
			strcpy(kom ,"rrc");
		break;
		case 0x20:
			strcpy(kom ,"rim");
		break;
		case 0xc9:
			strcpy(kom ,"ret");
		break;
		case 0xc8:
			strcpy(kom ,"rz");
		break;
		case 0xc0:
			strcpy(kom ,"rnc");
		break;
		case 0xf0:
			strcpy(kom ,"rp");
		break;
		case 0xf8:
			strcpy(kom ,"rm");
		break;
		case 0xd8:
			strcpy(kom ,"rc");
		break;
		case 0xd0:
			strcpy(kom ,"rnc");
		break;
		case 0xe8:
			strcpy(kom ,"rpe");
		break;
		case 0xe0:
			strcpy(kom ,"rpo");
		break;
		case 0x30:
			strcpy(kom ,"sim");
		break;
		case 0xf9:
			strcpy(kom ,"sphl");
		break;
		case 0x37:
			strcpy(kom ,"stc");
		break;
		case 0xeb:
			strcpy(kom ,"xchg");
		break;
		case 0xe3:
			strcpy(kom ,"xthl");
		break;
		//add
		case 0x87:
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
			strcpy(kom ,"add");
		break;
		
		//adc
		case 0x8f:
		case 0x88:
		case 0x89:
		case 0x8a:
		case 0x8b:
		case 0x8c:
		case 0x8d:
		case 0x8e:
			strcpy(kom ,"adc");
		break;
		
		//ana
		case 0xa7:
		case 0xa0:
		case 0xa1:
		case 0xa2:
		case 0xa3:
		case 0xa4:
		case 0xa5:
		case 0xa6:
			strcpy(kom ,"ana");
		break;
		
		//cmp
		case 0xbf:
		case 0xb8:
		case 0xb9:
		case 0xba:
		case 0xbb:
		case 0xbc:
		case 0xbd:
		case 0xbe:
			strcpy(kom ,"cmp");
		break;
		
		//mov
		case 0x7f:
		case 0x78:
		case 0x79:
		case 0x7a:
		case 0x7b:
		case 0x7c:
		case 0x7d:
		case 0x7e:
		case 0x47:
		case 0x40:
		case 0x41:
		case 0x42:
		case 0x43:
		case 0x44:
		case 0x45:
		case 0x46:
		case 0x4f:
		case 0x48:
		case 0x49:
		case 0x4a:
		case 0x4b:
		case 0x4c:
		case 0x4d:
		case 0x4e:
		case 0x57:
		case 0x50:
		case 0x51:
		case 0x52:
		case 0x53:
		case 0x54:
		case 0x55:
		case 0x56:
		case 0x5f:
		case 0x58:
		case 0x59:
		case 0x5a:
		case 0x5b:
		case 0x5c:
		case 0x5d:
		case 0x5e:
		case 0x67:
		case 0x60:
		case 0x61:
		case 0x62:
		case 0x63:
		case 0x64:
		case 0x65:
		case 0x66:
		case 0x6f:
		case 0x68:
		case 0x69:
		case 0x6a:
		case 0x6b:
		case 0x6c:
		case 0x6d:
		case 0x6e:
		case 0x77:
		case 0x70:
		case 0x71:
		case 0x72:
		case 0x73:
		case 0x74:
		case 0x75:
			strcpy(kom ,"mov");
		break;
		//sub
		case 0x97:
		case 0x90:
		case 0x91:
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:
		case 0x96:
			strcpy(kom ,"sub");
		break;
		//sbb
		case 0x9f:
		case 0x98:
		case 0x99:
		case 0x9a:
		case 0x9b:
		case 0x9c:
		case 0x9d:
		case 0x9e:
			strcpy(kom ,"sbb");
		break;
		//xra
		case 0xaf:
		case 0xa8:
		case 0xa9:
		case 0xaa:
		case 0xab:
		case 0xac:
		case 0xad:
		case 0xae:
			strcpy(kom ,"xra");
		break;	
		//pop
		case 0xc1:
		case 0xd1:
		case 0xe1:
		case 0xf1:
			strcpy(kom ,"pop");
		break;
		//push
		case 0xc5:
		case 0xd5:
		case 0xe5:
		case 0xf5:
			strcpy(kom ,"push");
		break;
		//dad
		case 0x9:
		case 0x19:
		case 0x29:
		case 0x39:
			strcpy(kom ,"dad");
		break;
		
		//dcr 
		case 0x3d:
		case 0x5:
		case 0xd:
		case 0x15:
		case 0x1d:
		case 0x25:
		case 0x2d:
		case 0x35:
			strcpy(kom ,"dcr");
		break;
		//dcx
		case 0xb:
		case 0x1b:
		case 0x2b:
		case 0x3b:
			strcpy(kom ,"dcx");
		break;
		//inr
		case 0x3c:
		case 0x4:
		case 0xc:
		case 0x14:
		case 0x1c:
		case 0x24:
		case 0x2c:
		case 0x34:
			strcpy(kom ,"inr");
		break;
		//inx
		case 0x3:
		case 0x13:
		case 0x23:
		case 0x33:
			strcpy(kom ,"inx");
		break;
		//ora
		case 0xb7:
		case 0xb0:
		case 0xb1:
		case 0xb2:
		case 0xb3:
		case 0xb4:
		case 0xb5:
		case 0xb6:
			strcpy(kom ,"ora");
		break;
		//ldax
		case 0xa:
		case 0x1a:
			strcpy(kom ,"ldax");
		break;
		//stax
		case 0x2:
		case 0x12:
			strcpy(kom ,"stax");
		break;
		//rst
		case 0xc7:
		case 0xcf:
		case 0xd7:
		case 0xdf:
		case 0xe7:
		case 0xef:
		case 0xf7:
		case 0xff:
			strcpy(kom ,"rst");
		break;
		
		
		case 0xc6: //adi
			strcpy(kom ,"adi");
		break;
		
		case 0xce: //aci
			strcpy(kom ,"aci");
		break;
		
		case 0xe6: //ani
			strcpy(kom ,"ani");
		break;
		case 0xf6: //ori
			strcpy(kom ,"ori");
		break;
		
		case 0xd3: //out
			strcpy(kom ,"out");
		break;		
		case 0xd6: //sui
			strcpy(kom ,"sui");
		break;
		
		case 0xde: //sbi
			strcpy(kom ,"sbi");
		break;
		
		case 0xee: //xri
			strcpy(kom ,"xri");
		break;
		//mvi
		case 0x3e:
		case 0x6:
		case 0xe:
		case 0x16:
		case 0x1e:
		case 0x26:
		case 0x2e:
		case 0x36:
			strcpy(kom ,"mvi");
		break;
		case 0xfe: //cpi
			strcpy(kom ,"cpi");
		break;
		
		case 0xdb: //in
			strcpy(kom ,"in");
		break;
		
		case 0x3a: //lda
			strcpy(kom ,"lda");
		break;
		case 0x2a: //lhld
			strcpy(kom ,"lhld");
		break;
		case 0x22: //shld
			strcpy(kom ,"shld");
		break;
		
		case 0x32: //sta
			strcpy(kom ,"sta");
		break;
		case 0xcd: //call
			strcpy(kom ,"call");
		break;
		
		case 0xcc: //cz
			strcpy(kom ,"cz");
		break;
		
		case 0xc4: //cnz
			strcpy(kom ,"cnz");
		break;
		
		case 0xf4: //cp
			strcpy(kom ,"cp");
		break;
		
		case 0xfc: //cm
			strcpy(kom ,"cm");
		break;
		
		case 0xdc: //cc
			strcpy(kom ,"cc");
		break;
		
		case 0xd4: //cnc
			strcpy(kom ,"cnc");
		break;
		
		case 0xec: //cpe
			strcpy(kom ,"cpe");
		break;
		
		case 0xe4: //cpo	
			strcpy(kom ,"cpo");
		break;
		
		case 0xc3: //jmp
			strcpy(kom ,"jmp");
		break;
		
		case 0xca: //jz
			strcpy(kom ,"jz");
		break;
		
		case 0xc2: //jnz
			strcpy(kom ,"jnz");
		break;
		
		case 0xf2: //jp
			strcpy(kom ,"jp");
		break;
		
		case 0xfa: //jm
			strcpy(kom ,"jm");
		break;
		
		case 0xda: //jc
			strcpy(kom ,"jc");
		break;
		
		case 0xd2: //jnc
			strcpy(kom ,"jnc");
		break;
		
		case 0xea: //jpe
			strcpy(kom ,"jpe");
		break;
		
		case 0xe2: //jpo
			strcpy(kom ,"jpo");
		break;
		
		//lxi
		case 1:
		case 0x21:
		case 0x31:
			strcpy(kom ,"lxi");
		break;
		default:
			kom = realloc(kom, 19);
			strcpy(kom, "Undefined command");
		
	}
	return kom;
}

void error_selector(){
	if(error){
		switch(error){
			case LABEL_NOT_FOUND:
				printf("Label not found\n");
			break;
			case TOO_MANY_WORDS:
				printf("Too many words\n");
			break;
			case COMMA_ERROR:
				printf("Comma error\n");
			break;
			case LIMIT_ERROR:
				printf("File is too big\n");
			break;
			case STRING_TOO_LONG:
				printf("The string is too long");
			break;
			case ERROR:
				printf("Error\n");
			break;
			case COMMAND_NOT_FOUND:
				printf("Command not found\n");
			break;
			case NOT_RIGHT_NUMBER:
				printf("Not right number\n");
			break;
			case ARGUMENT_ERROR:
				printf("Programm argument error\n");
			break;
			case MODE_ERROR:
				printf("Unidentified mode\n");
			break;
			case ONE_ARG_TWO_TIMES:
				printf("You have one argument two tymes\n");
			break;
			case IS_NOT_SOURCE_CODE:
				printf("The source code files must ending on .as\n");
			break;
			case NOT_RIGHT_ARGUMENT_FOR_THIS_MODE:
				printf("Not right argument for this mode\n");
			break;
			case ARGUMEN_NOT_FOUND:
				printf("Argument not found\n");
			break;
			default:
				printf("Unidentified error number : %i\n", error);
			break;
		}
	}
}

void errorp(){
	error_selector();
	if(error)
		exit(1);
}

void errorpl(int line){
	if(error){
		printf("Error on line %i : ", line);
		errorp();
	}
}

