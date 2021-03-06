# ifndef __combstr__hpp
# define __combstr__hpp
# include <mdlint.h>
# include <string.h>
namespace mdl {
char * combstr(char const *__str0, char const *__str1) {
	uint_t str0_len = strlen(__str0);
	uint_t str1_len = strlen(__str1);

	uint_t new_strlen = strlen(__str0) + strlen(__str1) + 1;

	char *strout = static_cast<char *>(malloc(new_strlen * sizeof(char)));
	memset(strout, '\0', new_strlen * sizeof(char));

	for (std::size_t o = 0; o != str0_len; o ++)
		strout[o] = __str0[o];

	std::size_t i = 0;
	for (std::size_t o = str0_len; o != str0_len + str1_len; o ++, i ++)
		strout[o] = __str1[i];

	return strout;
}
}

# endif /*__combstr__hpp*/
