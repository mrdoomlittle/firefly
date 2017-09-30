# include <mdlint.h>
# include <stdio.h>
extern mdl_u32_t ffly_usleep(mdl_u64_t, mdl_u32_t);
int main() {
	ffly_usleep(0, 999999998);
}
