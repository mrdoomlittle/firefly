# include "text.h"
void _nt_pixdraw(ff_u32_t, ff_u32_t, ff_u8_t*, ff_uint_t, ff_uint_t);

ff_u64_t static cmap[0x100] = {
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

void nt_text(ff_uint_t __x, ff_uint_t __y, char const *__str) {
	ff_u8_t buf[8*8*4];
	ff_uint_t x, y;
	ff_uint_t xoff;
	xoff = 0;
	ff_u64_t bm;
	char const *c;
	c = __str;
	while(*c != '\0') {
		bm = cmap[*c];
		y = 0;
		for(;y != 8; y++) {
			x = 0;
			for(;x != 8;x++) {
				*((ff_u32_t*)(buf+((x+(y*8))*4))) = !((bm>>(63-(x+(y*8))))&0x01)?~0:0xff<<24;
			}
		}
		_nt_pixdraw(__x+xoff, __y, buf, 8, 8);
		xoff+=8;
		c++;
	}
}
