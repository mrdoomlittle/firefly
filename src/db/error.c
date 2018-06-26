# include "../db.h"
char const* ff_db_errst(ff_db_err __err) {
	switch(__err) {
		case _ff_err_null:	return "no error";
		case _ff_err_lci:	return "login credentials incorrect";
		case _ff_err_nsu:	return "no such user";
		case _ff_err_ali:	return "already logged in";
		case _ff_err_prd:	return "permission denied";
	}
	return "unknown";
}
