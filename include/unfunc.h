#ifndef _UNFUNC_H_
#define _UNFUNC_H_

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


int error;
int limit;

#define LABEL_NOT_FOUND 1
#define TOO_MANY_WORDS 2
#define COMMA_ERROR 3
#define COLON_ERROR 4
#define LIMIT_ERROR 5
#define STRING_TOO_LONG 6
#define COMMAND_NOT_FOUND 7
#define NOT_RIGHT_NUMBER 8
#define NOT_BIN_NUN 9 
#define NOT_HEX_NUM 10
#define ARGUMENT_ERROR 11
#define MODE_ERROR 12
#define ONE_ARG_TWO_TIMES 13
#define IS_NOT_SOURCE_CODE 14
#define NOT_RIGHT_ARGUMENT_FOR_THIS_MODE 15
#define ARGUMEN_NOT_FOUND 16
#define NOT_INDIFICATET_COMMAND 17
#define THE_PROGRAMM_IS_TOO_BIG 18
#define ERROR 255

#define NOT_KOMENT (!(str[0]>='0' && str[0]<='9'))
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define MAXLINE 251
#define MAX_SIZE 4096
#define STDOUT 1
#define MIN_NUM -128
#define MAX_NUM 127
#define MIN_PORT 0
#define MAX_PORT 255
#define MAX_REAL_NUM MAX_PORT
#define MIN_REAL_NUM MIN_PORT
#define MIN_ADDR 0
#define MAX_ADDR 65535
#define MIN_WORD (-1*(MAX_ADDR+1)/2)
#define MAX_WORD (((MAX_ADDR+1)/2)-1)
#define MAX_LAB (MAX_ADDR+1)/2
#define COM_COUNT 100
#define NUM_COUNT 1000
#define ADDR_COUNT 100000
#define NEW_LINE '\n'


#define CMA  0x2f
#define CMC  0x3f
#define DAA  0x27
#define DI   0xf3
#define EI   0xfb
#define HLT  0x76
#define NOP  0x00
#define PCHL 0xe9
#define RAL  0x17
#define RAR  0x1f
#define RLC  0x07
#define RRC  0x0f
#define RIM  0x20
#define RET  0xc9
#define RZ   0xc8
#define RNZ  0xc0
#define RP   0xf0
#define RM   0xf8
#define RC   0xd8
#define RNC  0xd0
#define RPE  0xe8
#define RPO  0xe0
#define SIM  0x30
#define SPHL 0xf9
#define STC  0x37
#define XCHG 0xeb
#define XTHL 0xe3

#define ADD_A 0x87
#define ADD_B 0x80
#define ADD_C 0x81
#define ADD_D 0x82
#define ADD_E 0x83
#define ADD_H 0x84
#define ADD_L 0x85
#define ADD_M 0x86
#define ADC_A 0x8f
#define ADC_B 0x88
#define ADC_C 0x89
#define ADC_D 0x8a	
#define ADC_E 0x8b
#define ADC_H 0x8c
#define ADC_L 0x8d
#define ADC_M 0x8e
#define ANA_A 0xa7
#define ANA_B 0xa0
#define ANA_C 0xa1
#define ANA_D 0xa2
#define ANA_E 0xa3
#define ANA_H 0xa4
#define ANA_L 0xa5
#define ANA_M 0xa6
#define CMP_A 0xbf
#define CMP_B 0xb8
#define CMP_C 0xb9
#define CMP_D 0xba
#define CMP_E 0xbb
#define CMP_H 0xbc
#define CMP_L 0xbd
#define CMP_M 0xbe
#define DAD_B 0x09
#define DAD_D 0x19
#define DAD_H 0x29
#define DAD_SP 0x39
#define DCR_A 0x3d
#define DCR_B 0x05
#define DCR_C 0x0d
#define DCR_D 0x15
#define DCR_E 0x1d
#define DCR_H 0x25
#define DCR_L 0x2d
#define DCR_M 0x35
#define DCX_B 0x0B
#define DCX_D 0x1B
#define DCX_H 0x2B
#define DCX_SP 0x3B
#define INR_A 0x3C
#define INR_B 0x04
#define INR_C 0x0C
#define INR_D 0x14
#define INR_E 0x1C
#define INR_H 0x24
#define INR_L 0x2C
#define INR_M 0x34
#define INX_B 0x01
#define INX_D 0x13
#define INX_H 0x26
#define INX_SP 0x33
#define ORA_A 0xb7
#define ORA_B 0xb0
#define ORA_C 0xb1
#define ORA_D 0xb2
#define ORA_E 0xb3
#define ORA_H 0xb4
#define ORA_L 0xb5
#define ORA_M 0xb6
#define POP_B 0xc1
#define POP_D 0xd1
#define POP_H 0xe1
#define POP_PSW 0xf1
#define PUSH_B 0xc5
#define PUSH_D 0xd5
#define PUSH_H 0xe5
#define PUSH_PSW 0xf5
#define SUB_A 0x97
#define SUB_B 0x90
#define SUB_C 0x91
#define SUB_D 0x92
#define SUB_E 0x93
#define SUB_H 0x94
#define SUB_L 0x95
#define SUB_M 0x96
#define SBB_A 0x9f
#define SBB_B 0x98
#define SBB_C 0x99
#define SBB_D 0x9a
#define SBB_E 0x9b
#define SBB_H 0x9c
#define SBB_L 0x9d
#define SBB_M 0x9e
#define STAX_B 0x02
#define STAX_D 0x12
#define LDAX_B 0x0a
#define LDAX_D 0x1a
#define XRA_A 0xaf
#define XRA_B 0xa8
#define XRA_C 0xa9
#define XRA_D 0xaa
#define XRA_E 0xab
#define XRA_H 0xac
#define XRA_L 0xad
#define XRA_M 0xae
#define RST_0 0xc7
#define RST_1 0xcf
#define RST_2 0xd7
#define RST_3 0xdf
#define RST_4 0xe7
#define RST_5 0xef
#define RST_6 0xf7
#define RST_7 0xff

#define ADI	0xc6
#define ACI 0xce
#define ANI	0xe6
#define CPI  0xfe
#define IN  0xdb
#define ORI  0xf6
#define OUT  0xd3
#define SUI  0xd6
#define SBI  0xde
#define XRI  0xee

#define CALL 0xcd
#define CZ   0xcc
#define CNZ	 0xc4
#define CP   0xf4
#define CM   0xfc
#define CC   0xdc
#define CNC  0xd4
#define CPE  0xec
#define CPO  0xe4
#define JMP 0xc3
#define JZ  0xca
#define JNZ 0xc2
#define JP  0xf2
#define JM  0xfa
#define JC  0xda
#define JNC 0xd2
#define JPE 0xea
#define JPO 0xe2
#define LDA  0x3a
#define LHLD 0x2a
#define SHLD 0x22
#define STA  0x32

#define MOV_A_A 0x7f
#define MOV_A_B 0x78
#define MOV_A_C 0x79
#define MOV_A_D 0x7a
#define MOV_A_E 0x7b
#define MOV_A_H 0x7c
#define MOV_A_L 0x7d
#define MOV_A_M 0x7e
#define MOV_B_A 0x47
#define MOV_B_B 0x40
#define MOV_B_C 0x41
#define MOV_B_D 0x42
#define MOV_B_E 0x43
#define MOV_B_H 0x44
#define MOV_B_L 0x45
#define MOV_B_M 0x46
#define MOV_C_A 0x4f
#define MOV_C_B 0x48
#define MOV_C_C 0x49
#define MOV_C_D 0x4a
#define MOV_C_E 0x4b
#define MOV_C_H 0x5c
#define MOV_C_L 0x5d
#define MOV_C_M 0x4e
#define MOV_D_A 0x57
#define MOV_D_B 0x50
#define MOV_D_C 0x51
#define MOV_D_D 0x52
#define MOV_D_E 0x53
#define MOV_D_H 0x54
#define MOV_D_L 0x55
#define MOV_D_M 0x56
#define MOV_E_A 0x5f
#define MOV_E_B 0x58
#define MOV_E_C 0x59
#define MOV_E_D 0x5a
#define MOV_E_E 0x5b
#define MOV_E_H 0x5c
#define MOV_E_L 0x5d
#define MOV_E_M 0x5e
#define MOV_H_A 0x67
#define MOV_H_B 0x60
#define MOV_H_C 0x61
#define MOV_H_D 0x62
#define MOV_H_E 0x63
#define MOV_H_H 0x64
#define MOV_H_L 0x65
#define MOV_H_M 0x66
#define MOV_L_A 0x6f
#define MOV_L_B 0x68
#define MOV_L_C 0x69
#define MOV_L_D 0x6a
#define MOV_L_E 0x6b
#define MOV_L_H 0x6c
#define MOV_L_L 0x6d
#define MOV_L_M 0x6e
#define MOV_M_A 0x77
#define MOV_M_B 0x70
#define MOV_M_C 0x71
#define MOV_M_D 0x72
#define MOV_M_E 0x73
#define MOV_M_H 0x74
#define MOV_M_L 0x75

#define LXI_B 0x1
#define LXI_H 0x21
#define LXI_SP 0x31

#define MVI_A 0x3e
#define MVI_B 0x6
#define MVI_C 0xe
#define MVI_D 0x16
#define MVI_E 0x1e
#define MVI_H 0x26
#define MVI_L 0x2e
#define MVI_M 0x36

void err_doit(int, int, const char *, va_list );
void err_sys(const char *, ...);
char *read_codefd(int);
char *get_line();
char *get_code(int *);
int is_keyword(char *);
char *read_code(const char *);
int strisdigit(char *);
void strtolower(char *);
void reverse(char *);
int neg(int);
void itoa(int , char *);
int bytes(int);
void ch_append(char *, char);
void split(char *, char *, char *);
void split_two(char *, char *, char *);
int com_bytes(int );
int wropen(char *);
int rdopen(char *);
int powi(int, int);
int bin(char *);
int ch_hex(char);
int hex(char *);
int itob(unsigned short int);
char *spltob(char *);
char *num_to_com(int);
void error_selector();
void errorp();
void errorpl(int);


#endif
