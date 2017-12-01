# include <mdlint.h>
# include <stdio.h>
void extern ffly_nanosleep(mdl_u64_t, mdl_u64_t);

int main() {
	ffly_nanosleep(1, 1);
}
