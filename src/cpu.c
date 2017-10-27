#include "../include/unfunc.h"
#include "../include/cpu.h"

static void jmp_n(int adr){
	if(pc==begin+adr)
		new_addr = (pc-begin) + 1;
	else 	
		new_addr=adr;
}

static void jmp(){
	jmp_n((*(pc+1)<<8)+*(pc+2));
}


static void flags(){
	int i=0, count=0;
	if(a>=MIN_REAL_NUM && nn){
		cf=1;
		a=MIN_REAL_NUM;
	}else if(pp && a>MAX_NUM){
		cf=1;
		a=MAX_NUM;
	}else{
		if(a<0)
			a= a*-1 + MAX_NUM;
	}
	pp=nn=0;
	
	if(a>=0 && a<=MAX_NUM){
		s=0;
	}else{ 
		s=1;
	}
		
	if(a==0)
		z=1;
	else 
		z=0;
		
	if((a>=8 && a<16) || (a>=MAX_NUM+9 && a<MAX_NUM+17))
		ax=1;
	else 
		ax=0;

	i = MAX_REAL_NUM + 1;
	int n = a;
	while(i!=0){
		if(i & n)
			count++;
		i>>=1;
	}
	if(!(count%2))
		p=1;
	else 
		p=0;
}





static void add_r(int *r, int n){
	if((n>MAX_NUM && n<=MAX_REAL_NUM) && (*r>MAX_NUM && *r<=MAX_REAL_NUM)){
			*r+=n-MAX_NUM;
			nn=1;			
	}else if((n>MAX_NUM && n<=MAX_REAL_NUM) && (*r>=0 && *r<=MAX_NUM)){
			*r-=n-MAX_NUM;
	}else if((n>=0 && n<=MAX_NUM) && (*r>MAX_NUM && *r<=MAX_REAL_NUM)){
			*r = *r - n - MAX_NUM;
	}else if((n>=0 && n<=MAX_NUM) && (*r>=0 && *r<=MAX_NUM)){
			*r+=n;
			pp=1;
	}
}

static void add(int n){
	add_r(&a, n);
}

static void adc_r(int *reg, int n){
	if(cf==1)
		add_r(reg, n+1);
	else 
		add_r(reg, n);
}

static void adc(int n){
	if(cf==1)
		add(n+1);
	else
		add(n);
	
}

static void adi(){
	add(*(pc+1));
}

static void aci(int num){
	adc(num);
}

static void sub_r(int *r, int n){
	if(n>MAX_NUM && n<=MAX_REAL_NUM)
		n-=MAX_NUM;
	else if(n>0 && n<=MAX_NUM)
		n+=MAX_NUM;
	
	add_r(r, n);
	
}

static void sub(int n){
	sub_r(&a, n);
}

static void sbb_r(int *r, int n){
	if(cf)
		sub_r(r, n-1);
	else
		sbb_r(r, n);
}

static void sbb(int reg){
	if(cf)
		sub(reg-1);
	else
		sbb(reg);
}

static void sui(int n){
	sub(n);
}

static void sbi(int num){
	sbb(num);
}

static void ana(int reg){
	a&=reg;
}
	

static void ani(int num){
	a&=num;
}

static int prog(int i){
	static int pr=0;
	if(i==0)
		return pr;
	if(i==-1)
		return pr--;
	if(i==i)
		return pr++;
	return -1;
}

static int call(){
	int i;
	if((i=prog(1))>255 || i<0)
		return -1;
		
	i=prog(0)-1;
	progs[i]=*(pc+1);
	jmp_n((*(pc+1)<<8)+*(pc+2));
	return 0;
}


static int ret(){
	int i;
	if(prog(-1)<0);
		return -1;
	
	i=prog(0)+1;
	if(progs[i]==*pc)
		pc++;
	
	*pc=progs[i];
	return 0;
}

static void cz(){
	if(z)
		call();
}

static void cnz(){
	if(!z)
		call();
}

static void cp(){
	if(!s)
		call();
}

static void cm(){
	if(s)
		call();
}

static void cc(){
	if(cf)
		call();
}

static void cnc(){
	if(!cf)
		call();
}

static void dad(int reg_1, int reg_2){
	add_r(&h, reg_2);
	adc_r(&l, reg_1);
}

static void dad_sp(){
	char reg_1, reg_2;
	reg_1=*sp<<24;
	reg_1>>=24;
	reg_2=*sp>>8;
	add_r(&l, reg_2);
	adc_r(&h, reg_1);
	sp+=(reg_1<<8) + reg_2;
}

static void dcx(int *reg_1, int *reg_2){
	sub_r(reg_2, 1);
	sbb_r(reg_1, 0);
}

static void dcx_sp(){
	int reg_1, reg_2;
	reg_1=*sp<<24;
	reg_1>>=24;
	reg_2=*sp>>8;
	sub_r(&reg_2, 1);
	sbb_r(&reg_1, 0);
	sp+=(reg_1<<8) + reg_2;
}

static void in(int pp){
	a=port[pp];
}

static void inr(int *reg){
	*reg-=1;
}

static void inx(int *reg_1, int *reg_2){
	add_r(reg_2, 1);
	adc_r(reg_1, 0);
}

static void inx_sp(){
	int reg_1, reg_2;
	reg_1=*sp<<24;
	reg_1>>=24;
	reg_2=*sp>>8;
	add_r(&reg_2, 1);
	adc_r(&reg_1, 0);
	sp+=(reg_1<<8) + reg_2;
}

static void mov(int *reg_1, int reg_2){
	*reg_1=reg_2;
}

static void mvi_n(int *reg, int num){
	if(num<MIN_REAL_NUM)
		num=MIN_REAL_NUM;
	else if(num>MAX_REAL_NUM)	
		num=MAX_REAL_NUM;
	
	*reg=num;
}

static void mvi(int *reg){
	mvi_n(reg, *(pc+1));
}

static void lda(){
	a=*(pc+((*(pc+1)<<8) + *(pc+2)));
}

static void ldax(int reg_1, int reg_2){
	int n=reg_1<<8;
	n+=reg_2;
	a=*(pc+n);
}

static void lhld(){
	h=*(pc+((*(pc+1)<<8) + *(pc+2)));
	l=*(pc+((*(pc+1)<<8) + *(pc+2))+1);
		
}

static void lxi(int *reg_1, int *reg_2){
	*reg_1=*(pc) >> 8;
	*reg_2=*(pc) & 0xff;
}

static void out(){
	port[*(pc+1)]=a;
}

static void pchl(){
	int num=h<<8;
	*pc=num+l;
}


static void pop(int *reg_1, int *reg_2){
	*reg_1=*sp++;
	*reg_2=*sp++;
}

static void push(int reg_1, int reg_2){
	*sp--=reg_2;
	*sp--=reg_1;
}

static void ral(){
	a>>=1;
	add_r(&a, 0);
}

static void rar(){
	a<<=1;
	add_r(&a, 0);
}

static void rlc(){
	ral();
	adc_r(&a, 0);
}

static void rrc(){
	rar();
	adc_r(&a, 0);
}

static void rim(){
	;
}

static void rst(int m){
	jmp_n(m*8);		
}

static void sim(){
	;//the same like rim
}

static void sphl(){
	int n=h<<8;
	n+=l;
	*sp=n;
}

static void shld(){
	*(pc+( (*(pc+1)<<8) + *(pc+2)) )=h;
	*(pc+( (*(pc+1)<<8) + *(pc+2)) + 1)=l;
}

static void sta(){
	*(pc+( (*(pc+1)<<8) + *(pc+2)))=a;
}

static void stax(int reg_1, int reg_2){
	*(pc+((reg_1<<8)+reg_2))=a;
}

static void xthl(){
	char temp;
	temp=h;
	h=*sp;
	*sp=temp;
	temp=l;
	l=*sp;
	*sp=temp;
}

static void xra(int reg){
	a^=reg;
}

static void xri(int n){
	xra(n);
}

static void cma(){
	a=~a;
}

static void cmc(){
	c=1-c;
}

static void cmp(int reg){
	sub(0);
}

static void cpi(int num){
	cmp(num);
}

static void daa(){
	;//десятичная коррекция аккумулятора, а по другому, какая то невнятноя херня
}

static void dcr(int *reg){
	*reg-=1;
}

static void di(){
	inter_perm=0;
}

static void ei(){
	inter_perm=1;
}


static void jz(){
	if(z)
		jmp();
}

static void jnz(){
	if(!z)
		jmp();
}

static void jp(){
	if(!s)
		jmp();
}

static void jm(){
	if(s)
		jmp();
}

static void jc(){
	if(cf)
		jmp();
}

static void jnc(){
	if(!c)
		jmp();
}

static void jpe(){
	;
}

static void jpo(){
	;
}

static void lxi_sp(){
	*sp=(*(pc+1)<<8)+*(pc+2);	
}

static void nop(){
	;
}

static void ora(int reg){
	a |= reg;
}

static void ori(int num){
	ora(num);
}

static void rz(){
	if(z)
		ret();
}

static void rnz(){
	if(!z)
		ret();
}

static void rp(){
	if(p)
		ret();
}

static void rm(){
	if(!p)
		ret();
}

static void rc(){
	if(c)
		ret();
}

static void rnc(){
	if(!c)
		ret();
}

static void rpe(){
	;
}

static void rpo(){
	;
}

static void stc(){
	c=1;	
}

static void cpo(){
	if(!p)
		call();
	
}

static void cpe(){
	if(p)
		call();
}


static int init_flag_register(){
	int fl=0;
	fl+=cf;
	fl+=p*4;
	fl+=ax*16;
	fl+=z*64;
	fl+=s*128;
	return fl;
}

static void init_flags(int fl){
	cf=fl%2;
	fl/=2;
	
	fl/=2;
	
	p=fl%2;
	fl/=2;
	
	fl/=2;
	
	ax=fl%2;
	fl/=2;
	
	fl/=2;
	
	z=fl%2;
	fl/=2;
	
	s=fl%2;
	fl/=2;
}

static void xchg(){
	char temp;
	temp=h;
	h=d;
	d=temp;
	
	temp=l;
	l=e;
	e=temp;
	
}

static int one_byte(){
	switch(*pc){
		case 0x2f:
			cma();
		break;
		case 0x3f:
			cmc();
		break;
		case 0x27:
			daa();
		break;
		case 0xf3:
			di();
		break;
		case 0xfb:
			ei();
		break;
		case 0x76:
			return 0;
		break;
		case 0:
			nop();
		break;
		case 0xe9:
			pchl();
		break;
		case 0x17:
			ral();
		break;
		case 0x1f:
			rar();
		break;
		case 0x07:
			rlc();
		break;
		case 0x0f:
			rrc();
		break;
		case 0x20:
			rim();
		break;
		case 0xc9:
			ret();
		break;
		case 0xc8:
			rz();
		break;
		case 0xc0:
			rnz();
		break;
		case 0xf0:
			rp();
		break;
		case 0xf8:
			rm();
		break;
		case 0xd8:
			rc();
		break;
		case 0xd0:
			rnc();
		break;
		case 0xe8:
			rpe();
		break;
		case 0xe0:
			rpo();
		break;
		case 0x30:
			sim();
		break;
		case 0xf9:
			sphl();
		break;
		case 0x37:
			stc();
		break;
		case 0xeb:
			xchg();
		break;
		case 0xe3:
			xthl();
		break;
		//add
		case 0x87:
			add(a);
		break;
		case 0x80:
			add(b);
		break;
		case 0x81:
			add(c);
		break;
		case 0x82:
			add(d);
		break;
		case 0x83:
			add(e);
		break;
		case 0x84:
			add(h);
		break;
		case 0x85:
		case 0x86:
			add(l);
		break;
		
		//adc
		case 0x8f:
			adc(a);
		break;
		case 0x88:
			adc(b);
		break;
		case 0x89:
			adc(c);
		break;
		case 0x8a:
			adc(d);
		break;
		case 0x8b:
			adc(e);
		break;
		case 0x8c:
			adc(h);
		break;
		case 0x8d:
			adc(l);
		break;
		case 0x8e:
			adc(h);
		break;
		
		//ana
		case 0xa7:
			ana(a);
		break;
		case 0xa0:
			ana(b);
		break;
		case 0xa1:
			ana(c);
		break;
		case 0xa2:
			ana(d);
		break;
		case 0xa3:
			ana(e);
		break;
		case 0xa4:
			ana(h);
		break;
		case 0xa5:
		case 0xa6:
			ana(l);
		break;
		
		//cmp
		case 0xbf:
			cmp(a);
		break;
		case 0xb8:
			cmp(b);
		break;
		case 0xb9:
			cmp(c);
		break;
		case 0xba:
			cmp(d);
		break;
		case 0xbb:
			cmp(e);
		break;
		case 0xbc:
			cmp(h);
		break;
		case 0xbd:
			cmp(l);
		break;
		case 0xbe:
			cmp(l);
		break;
		
		//mov
		case 0x7f:
			mov(&a, a);
		break;
		case 0x78:
			mov(&a, b);
		break;
		case 0x79:
			mov(&a, c);
		break;
		case 0x7a:
			mov(&a, d);
		break;
		case 0x7b:
			mov(&a, e);
		break;
		case 0x7c:
			mov(&a, h);
		break;
		case 0x7d:
		case 0x7e:
			mov(&a, l);
		break;
		

		case 0x47:
			
			mov(&b, a);
		break;
		case 0x40:
			mov(&b, b);
		break;
		case 0x41:
			mov(&b, c);
		break;
		case 0x42:
			mov(&b, d);
		break;
		case 0x43:
			mov(&b, e);
		break;
		case 0x44:
			mov(&b, h);
		break;
		case 0x45:
		case 0x46:
			mov(&b, l);
		break;
		
		case 0x4f:
			mov(&c, a);
		break;
		case 0x48:
			mov(&c, b);
		break;
		case 0x49:
			mov(&c, c);
		break;
		case 0x4a:
			mov(&c, d);
		break;
		case 0x4b:
			mov(&c, e);
		break;
		case 0x4c:
			mov(&c, h);
		break;
		case 0x4d:
		case 0x4e:
			mov(&c, l);
		break;
		
			
		case 0x57:
			mov(&d, a);
		break;
		case 0x50:
			mov(&d, b);
		break;
		case 0x51:
			mov(&d, c);
		break;
		case 0x52:
			mov(&d, d);
		break;
		case 0x53:
			mov(&d, e);
		break;
		case 0x54:
			mov(&d, h);
		break;
		case 0x55:
		case 0x56:
			mov(&d, l);
		break;
		
		case 0x5f:
			mov(&e, a);
		break;
		case 0x58:
			mov(&e, b);
		break;
		case 0x59:
			mov(&e, c);
		break;
		case 0x5a:
			mov(&e, d);
		break;
		case 0x5b:
			mov(&e, e);
		break;
		case 0x5c:
			mov(&e, h);
		break;
		case 0x5d:
		case 0x5e:
			mov(&e, l);
		break;
		
		
		
		case 0x67:
			mov(&h, a);
		break;
		case 0x60:
			mov(&h, b);
		break;
		case 0x61:
			mov(&h, c);
		break;
		case 0x62:
			mov(&h, d);
		break;
		case 0x63:
			mov(&h, e);
		break;
		case 0x64:
			mov(&h, h);
		break;
		case 0x65:
		case 0x66:
			mov(&h, l);
		break;
		
		
		
		case 0x6f:
			mov(&l, a);
		break;
		case 0x68:
			mov(&l, b);
		break;
		case 0x69:
			mov(&l, c);
		break;
		case 0x6a:
			mov(&l, d);
		break;
		case 0x6b:
			mov(&l, e);
		break;
		case 0x6c:
			mov(&l, h);
		break;
		case 0x6d:
		case 0x6e:
			mov(&l, l);
		break;
		
		
		
		//mov m, x
		case 0x77:
			mov(&h, a);
			mov(&l, a);
		break;
		case 0x70:
			mov(&h, b);
			mov(&l, b);
		break;
		case 0x71:
			mov(&h, c);
			mov(&l, c);
		break;
		case 0x72:
			mov(&h, d);
			mov(&l, d);
		break;
		case 0x73:
			mov(&h, e);
			mov(&l, e);
		break;
		case 0x74:
			mov(&h, h);
			mov(&l, h);
		break;
		case 0x75:
			mov(&h, l);
			mov(&l, l);
		break;
		//sub
		case 0x97:
			sub(a);
		break;
		case 0x90:
			sub(b);
		break;
		case 0x91:
			sub(c);
		break;
		case 0x92:
			sub(d);
		break;
		case 0x93:
			sub(e);
		break;
		case 0x94:
			sub(h);
		break;
		case 0x95:
			sub(l);
		break;
		case 0x96:
			sub(l);
		break;
		//sbb
		case 0x9f:
			sbb(a);
		break;
		case 0x98:
			sbb(b);
		break;
		case 0x99:
			sbb(c);
		break;
		case 0x9a:
			sbb(d);
		break;
		case 0x9b:
			sbb(e);
		break;
		case 0x9c:
			sbb(h);
		break;
		case 0x9d:
			sbb(l);
		break;
		case 0x9e:
			sbb(l);
		break;
		//xra
		case 0xaf:
			xra(a);
		break;
		case 0xa8:
			xra(b);
		break;
		case 0xa9:
			xra(c);
		break;
		case 0xaa:
			xra(d);
		break;
		case 0xab:
			xra(e);
		break;
		case 0xac:
			xra(h);
		break;
		case 0xad:
			xra(l);
		break;
		case 0xae:
			xra(l);
		break;	
		//pop
		case 0xc1:
			pop(&b, &c);
		break;
		case 0xd1:
			pop(&d, &e);
		break;
		case 0xe1:
			pop(&h, &l);
		break;
		case 0xf1:
			pop(&a, &f);
		break;
		//push
		case 0xc5:
			push(b, c);
		break;
		case 0xd5:
			push(d, e);
		break;
		case 0xe5:
			push(h, l);
		break;
		case 0xf5:
			f=init_flag_register();
			push(a, f);
		break;
		//dad
		case 0x9:
			dad(b, c);
		break;
		case 0x19:
			dad(d, e);
		break;
		case 0x29:
			dad(h, l);
		break;
		case 0x39:
			dad_sp();
		break;
		
		//dcr 
		case 0x3d:
			dcr(&a);
		break;
		case 0x5:
			dcr(&b);
		break;
		case 0xd:
			dcr(&c);
		break;
		case 0x15:
			dcr(&d);
		break;
		case 0x1d:
			dcr(&e);
		break;
		case 0x25:
			dcr(&h);
		break;
		case 0x2d:
			dcr(&l);
		break;
		case 0x35:
			dcx(&h, &l);
		break;
		//dcx
		case 0xb:
			dcx(&b, &c);
		break;
		case 0x1b:
			dcx(&d, &e);
		break;
		case 0x2b:
			dcx(&h, &l);
		break;
		case 0x3b:
			dcx_sp();
		break;
		//inr
		case 0x3c:
			inr(&a);
		break;
		case 0x4:
			inr(&b);
		break;
		case 0xc:
			inr(&c);
		break;
		case 0x14:
			inr(&d);
		break;
		case 0x1c:
			inr(&e);
		break;
		case 0x24:
			inr(&h);
		break;
		case 0x2c:
			inr(&l);
		break;
		case 0x34:
			inr(&l);
			inr(&h);
		break;
		//inx
		case 0x3:
			inx(&b, &c);
		break;
		case 0x13:
			inx(&d, &e);
		break;
		case 0x23:
			inx(&h, &l);
		break;
		case 0x33:
			inx_sp();
		break;
		//ora
		case 0xb7:
			ora(a);
		break;
		case 0xb0:
			ora(b);
		break;
		case 0xb1:
			ora(c);
		break;
		case 0xb2:
			ora(d);
		break;
		case 0xb3:
			ora(e);
		break;
		case 0xb4:
			ora(h);
		break;
		case 0xb5:
			ora(l);
		break;
		case 0xb6:
			ora(l);
		break;
		//ldax
		case 0xa:
			ldax(b, c);
		break;
		case 0x1a:
			ldax(d, e);
		break;
		//stax
		case 0x2:
			stax(d, c);
		break;
		case 0x12:
			stax(d, e);
		break;
		//rst
		case 0xc7:
			rst(0);
		break;
		case 0xcf:
			rst(1);
		break;
		case 0xd7:
			rst(2);
		break;
		case 0xdf:
			rst(3);
		break;
		case 0xe7:
			rst(4);
		break;
		case 0xef:
			rst(5);
		break;
		case 0xf7:
			rst(6);
		break;
		case 0xff:
			rst(7);
		break;
		
		default :	
			return -1;
		break;
	}
	
	return 1;

}

static int two_bytes(){
	switch(*pc){		
		case 0xc6: //adi
			adi();
		break;
		
		case 0xce: //aci
			aci(*(pc+1));
		break;
		
		case 0xe6: //ani
			ani(*(pc+1));
		break;
		case 0xf6: //ori
			ori(*(pc+1));
		break;
		
		case 0xd3: //out
			out(*(pc+1));
		break;		
		case 0xd6: //sui
			sui(*(pc+1));
		break;
		
		case 0xde: //sbi
			sbi(*(pc+1));
		break;
		
		case 0xee: //xri
			xri(*(pc+1));
		break;
		//mvi
		case 0x3e:
			mvi(&a);
		break;
		case 0x6:
			mvi(&b);
		break;
		case 0xe:
			mvi(&c);
		break;
		case 0x16:
			mvi(&d);
		break;
		case 0x1e:
			mvi(&e);
		break;
		case 0x26:
			mvi(&h);
		break;
		case 0x2e:
			mvi(&l);
		break;
		case 0x36:
			mvi(&h);
			mvi_n(&l, 0);
		break;
		case 0xfe: //cpi
			cpi(*(pc+1));
		break;
		
		case 0xdb: //in
			in(*(pc+1));
		break;
		
		default:
			return -1;
		break;
	}
	
	return 1;
}

static int three_bytes(){
	switch(*pc){
		case 0x3a: //lda
			lda();
		break;
		case 0x2a: //lhld
			lhld();
		break;
		case 0x22: //shld
			shld();
		break;
		
		case 0x32: //sta
			sta();
		break;
		case 0xcd: //call
			if((call())==-1)
				return -1;
		break;
		
		case 0xcc: //cz
			cz();
		break;
		
		case 0xc4: //cnz
			cnz();
		break;
		
		case 0xf4: //cp
			cp();
		break;
		
		case 0xfc: //cm
			cm();
		break;
		
		case 0xdc: //cc
			cc();
		break;
		
		case 0xd4: //cnc
			cnc();
		break;
		
		case 0xec: //cpe
			cpe();
		break;
		
		case 0xe4: //cpo	
			cpo();
		break;
		
		case 0xc3: //jmp
			jmp();
		break;
		
		case 0xca: //jz
			jz();
		break;
		
		case 0xc2: //jnz
			jnz();
		break;
		
		case 0xf2: //jp
			jp();
		break;
		
		case 0xfa: //jm
			jm();
		break;
		
		case 0xda: //jc
			jc();
		break;
		
		case 0xd2: //jnc
			jnc();
		break;
		
		case 0xea: //jpe
			jpe();
		break;
		
		case 0xe2: //jpo
			jpo();
		break;
		
		//lxi
		case 1:
			lxi(&b, &c);
		break;
		case 0x21:
			lxi(&h, &l);
		break;
		case 0x31:
			lxi_sp();
		break;
		
		default:
			return -1;
		break;
	}
	return 1;
}

int *order_to_int(char *code){
	int *ret = (int *) malloc(sizeof(int) * strlen(code)-1);
	int i=0;
	while(*code!='\0'){
		if(*code<0)
			ret[i]=MAX_REAL_NUM+1+*code;
		else 
			ret[i]=*code;

		code++;i++;
	}
	
	return ret;
}

static int three_byte_correcting(int *byte){
	static int i=0;
	if(i==3){
		i = 0;
		return 0;
	}else{
		if(*byte<0)
			*byte=0;
		else if(*byte>MAX_REAL_NUM)
			*byte=MAX_REAL_NUM;
		i++;	
		return three_byte_correcting(byte+1);
	}
}

int exec(){
	int byte, ret;
	byte = com_bytes(*pc);

	prev_com=pc-begin;
	new_addr=-1;
	flags();
	
	three_byte_correcting(pc);

	switch(byte){
		case 1:
			ret = one_byte();
		break;
		case 2:
			ret = two_bytes();
		break;
		case 3:
			ret = three_bytes();		
		break;
		default:
			error=NOT_INDIFICATET_COMMAND;
			return -1;
	}

	if(new_addr!=-1)
		pc = begin + new_addr;
	else 
		pc += byte;
	
	return ret;
}
