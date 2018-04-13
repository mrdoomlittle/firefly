# include "string.h"
# include "../ffly_def.h"
ff_u64_t static powof10[] = {
	1,
	10,
	100,
	1000,
	10000,
	100000,
	1000000,
	10000000,
	100000000,
	1000000000,
	10000000000,
	100000000000,
	1000000000000,
	10000000000000,
	100000000000000,
	1000000000000000,
	10000000000000000,
	100000000000000000,
	1000000000000000000
};

ff_uint_t ffly_nots(ff_u64_t __no, char *__buf) {
	if (!__no) {
		*(__buf++) = '0';
		*__buf = '\0';
		return 1;
	}

	ff_uint_t l = 0;
	ff_u64_t ret = 0, pl = 1;
	for (;pl <= __no;pl*=10,l++);

	ff_u64_t g = 0, i = l-1, r;
	char *p = __buf;
	while(p != __buf+l) {
		switch(r = ((__no-g)/powof10[i])) { 
			case 0: *p = '0';break;
			case 1: *p = '1';break;
			case 2: *p = '2';break;
			case 3: *p = '3';break;
			case 4: *p = '4';break;
			case 5: *p = '5';break;
			case 6: *p = '6';break;
			case 7: *p = '7';break;
			case 8: *p = '8';break;
			case 9: *p = '9';break;
		}
		g += r*powof10[i];
		p++;
		i--;
	}
	*p = '\0';
	return l;
}

ff_u64_t ffly_stno(char *__s) {
	if(*__s == '0' && *(__s+1) == '\0')
		return 0;

	ff_u8_t sign;
	if (sign = (*__s == '-')) __s++;

	ff_u64_t no = 0;
	char *p = __s;
	for(;*p != '\0';p++) {
		no = no*10;
		switch(*p) {
			case '0': break;
			case '1': no += 1; break;
			case '2': no += 2; break;
			case '3': no += 3; break;
			case '4': no += 4; break;
			case '5': no += 5; break;
			case '6': no += 6; break;
			case '7': no += 7; break;
			case '8': no += 8; break;
			case '9': no += 9; break;
			default: {
				// error
				return 0;
			}
		}
	}

	if (sign)
		return -no;
	return no;
}

char ffly_tolow(char __c) {
	if (__c >= 'a' && __c <= 'z') return __c;
	return 'a'+(__c-'A');
}

ff_i8_t ffly_islen(char *__s, ff_uint_t __l) {
	while(*__s != '\0') {
		if (!(__l--)) { 
			if (*(__s+1) == '\0')
				return 1;  
			else
				return -1;	   
		}
		__s++;
	}	 
	return 0;
}

ff_u64_t ffly_htint(char *__s) {
	char *p = __s, c;
	ff_u64_t ret = 0;
	while(*p != '\0') {
		c = *(p++);
		if (c >= 'A' && c <= 'F')
			c = ffly_tolow(c);
		if (c >= '0' && c <= '9')
			ret = (ret<<4)|((c-'0')&0xf);
		else if (c >= 'a' && c <= 'f')
			ret = (ret<<4)|(((c-'a')+10)&0xf);
	}
	return ret;
}

//# include <stdio.h>
// needs testing
ff_uint_t ffly_floatts(double __no, char *__buf) {
	ff_u8_t i = 0;
	double v = __no;
	while(i != 9) {
		if (v<1) break; 
		v *= 0.1;
		i++;	   
	}

	ff_u64_t no, s = 0;
	ff_uint_t l;
	char *p;
_bk:
	if (__no<1) {
		while(__no<1) {
			__buf[s++] = '0';			
			__no*=10;
		}
		s--; 
		no = (ff_u64_t)__no;
		goto _bk;
	} else
		no = (ff_u64_t)(__no/0.00000001); // needs to be rounded
	l = ffly_nots(no, __buf+s);
	p = __buf+l+s;
	while((p-__buf) >= i) {
		*(p+1) = *p;
		p--;
	}
	*(__buf+i) = '.';
	*(__buf+s+l+1) = '\0';
	return l+s+1;
}

ff_uint_t ffly_noths(ff_u64_t __no, char *__buf) {
	if (!__no) {
		*(__buf++) = '0';
		*__buf = '\0';
		return 1;
	}

	char *p = __buf;
	ff_u8_t offset = 0;
	ff_u8_t sk = 1;
	while(!((offset++)>>4)) {
		ff_u8_t b = __no>>60&0xf;
		if (b>0 && sk) // skip until somthing other then zero pops up
			sk = 0;
		if (!sk) {
			if (b>=0 && b<10)
				*(p++) = '0'+b;
			else if (b>=10 && b <=15)
				*(p++) = 'a'+(b-10); 
		}
		__no<<=4;
	}
	*p = '\0';
	return p-__buf;
}

// needs testing
static double t[] = {
	0.1,
	0.01,
	0.001,
	0.0001,
	0.00001,
	0.000001,
	0.0000001,
	0.00000001,
	0.000000001,
	0.0000000001
};

/*
# include <stdio.h>
int main() {
	char buf[200];

	ff_u64_t i = 800;
	ffly_noths(i, buf);
	printf("%s, %u, %u\n", buf, i, ffly_htint(buf));
}*/

/*
# include "../data/str_len.h"
# include <stdio.h>
int main() {
	char s[1000];
//	  ff_uint_t ii = ffly_floatts(21.22, s);
	//ffly_nots(2129900000000, s);
//	  ff_uint_t i = ffly_str_len(s);
	ffly_noths(101987, s);
	printf("'%s'\n", s);


//	  ff_u8_t i = 0;
  //  while(i != 4) {
	//	  printf("%lf\n", 0.01/t[i]);
	 //  i++;
	//}

	//printf("%u\n", 21299/1000);
//	  ffly_floatts(4.2332, s);
}
*/

/*
# include <stdio.h>
int main() {
	char s[1220];
	ff_uint_t len = ffly_floatts(1.00397972, s);

	printf("%s:%u\n", s, len);
}*/

double ffly_stfloat(char *__s) {
	double ret;
	ff_u8_t sign;
	if (sign = (*__s == '-')) __s++; 

	ff_uint_t frac;
	char *p = __s, *end = NULL;
	while(*p != '.' && *p != '\0') p++;

	ff_uint_t k = 0;
	if (*p == '.') {
		p++;
		frac = 0;
		while(*p != '\0') {
			frac = frac*10+(*p-'0');
			k++;
			p++;
		}
	}

	p = __s;
	ff_u32_t v = 0;
	while(*p != '.' && *p != '\0') {
		v = v*10+(*p-'0');
		p++;
	}

	ret = (double)v+((double)frac*t[k-1]);
	if (sign)
		return -ret;  
	return ret;
}
