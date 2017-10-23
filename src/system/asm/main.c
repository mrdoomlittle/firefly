# include <mdlint.h>
# include <stdio.h>
extern mdl_u32_t ffly_usleep(mdl_u64_t, mdl_u32_t);

void ffly_usleepa(mdl_u64_t __a, mdl_u32_t __b) {}
int main() {
	printf("%u\n", ffly_usleep(1, 1));
}
