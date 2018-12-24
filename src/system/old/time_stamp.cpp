# include "time_stamp.hpp"
char *mdl::firefly::system::time_stamp() {
	static time_t raw_time;
	static struct tm *time_info;

	time(&raw_time);
	time_info = localtime(&raw_time);
	char *time_stmp = asctime(time_info);

	time_stmp[strlen(time_stmp) - 1] = '\0';
	return time_stmp;
}
