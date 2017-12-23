# include "string.h"
mdl_u64_t ffly_stno(char *__s) {
	mdl_u64_t plv = 1, no = 0;
	mdl_uint_t i = 0;
	for (;;i++) {
		if (__s[i+1] == '\0') break;
		plv = plv*10;
	}

	char *itr = __s;
	for(;*itr != '\0';itr++) {
		switch(*itr) {
			case '0': break;
			case '1': no += 1*plv; break;
			case '2': no += 2*plv; break;
			case '3': no += 3*plv; break;
			case '4': no += 4*plv; break;
			case '5': no += 5*plv; break;
			case '6': no += 6*plv; break;
			case '7': no += 7*plv; break;
			case '8': no += 8*plv; break;
			case '9': no += 9*plv; break;
			default: return 0;
		}
		if (plv != 1) plv = plv/10;
	}
	return no;
}
