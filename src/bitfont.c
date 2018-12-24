# include "bitfont.h"

/*
	i went to ahead of myself with typo and is to lazy to make fontforge in open gl so
	this option build fontforge for workshop then move over to typo and use bitfont for now.
*/

ff_u64_t static stuff[0x100] = {
	['0'] = 0x3c242424243c00,
	['1'] = 0x38080808080800,
	['2'] = 0x3c040438203c00,
	['3'] = 0x3c040804043c00,
	['4'] = 0x40c141c040000,
	['5'] = 0x3c203c043c0000,
	['6'] = 0x3c203c243c0000,
	['7'] = 0x3c040c04040400,
	['8'] = 0x3c242c34243c00,
	['9'] = 0x3c243c04043800,
	['a'] = 0x1824243c242400,
	['b'] = 0x3c22223c22221c,
	['c'] = 0x7c444040447c00,
	['d'] = 0x70484848487000,
	['e'] = 0x3c203c20203c00,
	['f'] = 0x3c202038202000,
	['g'] = 0x38404058443800,
	['h'] = 0x44447c44444400,
	['i'] = 0x3c080808083c00,
	['j'] = 0x3c080808281000,
	['k'] = 0x24242830282400,
	['l'] = 0x40404040487800,
	['m'] = 0x42665a424200,
	['n'] = 0x4464544c444400,
	['o'] = 0x3c424242423c00,
	['p'] = 0x70484870404000,
	['q'] = 0x38444444380c00,
	['r'] = 0x78444c70484400,
	['s'] = 0x3c407008087000,
	['t'] = 0x7c101010101000,
	['u'] = 0x44444444443800,
	['v'] = 0x42422424140800,
	['w'] = 0x525252522c0000,
	['x'] = 0x28281028280000,
	['y'] = 0x28282810100000,
	['z'] = 0x3c0810203c0000
};

ff_u8_t static bm[4096];
ff_u8_t static scale = 0;
#define width	(8<<scale)
#define height	(8<<scale)

void ffly_bitfont_info(ff_u8_t __what, long long __arg) {
	switch(__what) {
		case 0x00:
			*(ff_uint_t*)__arg = width;
		break;
		case 0x01:
			*(ff_uint_t*)__arg = height;
		break;
	}
}

void ffly_bitfont_scalefactor(ff_u8_t __factor) {
	scale = __factor;
}

ff_u8_t* ffly_bitfont_char(ff_u8_t __id) {
	ff_u64_t ch;
	ch = stuff[__id];

	ff_uint_t x, y;
	y = 0;
	for(;y != height; y++) {
		x = 0;
		for(;x != width;x++) {
			bm[x+(y*width)] = (ch>>(63-((x>>scale)+((y>>scale)*8))))&0x01;
		}
	}

	return bm;
}
