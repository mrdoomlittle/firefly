# ifndef __ffly__pkg__h
# define __ffly__pkg__h
# include "ffint.h"
#define IG_FILE     0x00
#define IG_DIR      0x01
#define IG_UNKNOWN  0x02
#define IG_ISFILE(__type) \
	(__type == IG_FILE)
#define IG_ISDIR(__type) \
	(__type == IG_DIR)
extern char const *pkc_plan;
extern ff_uint_t pkc_pfl;
/*
	NOTE:
		packages are done in a linear fashion
		there is not to be any magic, e.g. test/a/file.txt
		<- dir 'a' will not be created and will attempt to open file at "test/a/file.txt"
		but as directory 'a' does not exit error <- i think i dont actily know if open creates the path to the file along the way.
		plan file should include

		test/a				<- register dir
		test/a/file.txt		<- register file

		so when deconstruction occurs it will be done in the right order, and ... idfk read pkg/construct.c

	TODO:
		remove text plan file and go for a binary method

		soo
		dest <- exec-file(THIS) <- binary-file <- converter <- text-file

*/

/*
	do clean up and naming
*/
struct pkc_header {
	ff_u32_t grain, glen;
	ff_u32_t it;
	ff_uint_t ic;

};

struct __pkc_ingot {
	ff_u8_t type;
	ff_u32_t path;
	ff_u32_t mode;
	ff_uint_t plen;
	ff_u32_t tract;
	ff_uint_t ts;
};

struct pkc_ingot {
    ff_u8_t type;
    char const *path;
    ff_uint_t plen;
	ff_u32_t tract;
};

struct pkc_plan {
    struct pkc_ingot **i;
    ff_uint_t n;
};

void ff_pkc_init(void);
struct pkc_plan* pkc_get_plan(void);
void ff_pkc_construct(char const*, char const*);
void ff_pkc_deconstruct(char const*, char const*);
void ff_pkc_de_init(void);
# endif /*__ffly__pkg__h*/
