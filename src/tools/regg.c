# include <mdlint.h>
# include <stdio.h>
# define is_bit(__bits, __bit) \
	(((__bits)&(__bit))==(__bit))
int main() {
	mdl_u8_t i = 0;
	while(i != 32) {
		printf("%c%c%c%c%c = %u\n", is_bit(i, 1)?'r':'-', is_bit(i, 2)?'a':'-',
			is_bit(i, 4)?'x':'-', is_bit(i, 8)?'e':'-', is_bit(i, 16)?'l':'-', 0x1f^i);
		i++;
	}
}
