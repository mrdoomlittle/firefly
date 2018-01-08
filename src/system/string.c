# include "string.h"
mdl_u64_t ffly_stno(char *__s) {
    mdl_u8_t sign;
    if (sign = (*__s == '-')) __s++;

	mdl_u64_t no = 0;
	char *itr = __s;
	for(;*itr != '\0';itr++) {
        no = no*10;
		switch(*itr) {
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

mdl_i8_t ffly_islen(char *__s, mdl_uint_t __l) {
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

mdl_u64_t ffly_htint(char *__s) {
    char *itr = __s, c;
    mdl_u64_t ret = 0;
    while(*itr != '\0') {
        c = *(itr++);
        if (c >= 'A' && c <= 'F')
            c = ffly_tolow(c);
        if (c >= '0' && c <= '9')
            ret = (ret<<4)|((c-'0')&0xF);
        else if (c >= 'a' && c <= 'f')
            ret = (ret<<4)|(((c-'a')+10)&0xF);
    }
    return ret;
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

# include <stdio.h>
double ffly_stfloat(char *__s) {
    double ret;
    mdl_u8_t sign;
    if (sign = (*__s == '-')) __s++; 

    mdl_uint_t frac;
    char *p = __s, *end = NULL;
    while(*p != '.' && *p != '\0') p++;

    mdl_uint_t k = 0;
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
    mdl_u32_t v = 0;
    while(*p != '.' && *p != '\0') {
        v = v*10+(*p-'0');
        p++;
    }

    ret = (double)v+((double)frac*t[k-1]);
    if (sign)
        return -ret;  
    return ret;
}
