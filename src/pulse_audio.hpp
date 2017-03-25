# ifndef __pulse__audio__hpp
# define __pulse__audio__hpp
# include <pulse/simple.h>
# include <pulse/error.h>
# include <errno.h>
# include <cstdio>
# include <strcmb.hpp>
# include <boost/cstdint.hpp>
# include <sys/stat.h>
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/errno.h"
# include <fcntl.h>
namespace mdl {
namespace firefly {
class pulse_audio {
	private:
	public:
	~pulse_audio() {
		if (this-> buff != nullptr) memory::mem_free(this-> buff);
	}

	boost::int8_t load_file(char const *__filedir, char const *__filename);
	boost::int8_t play_sample(char const *__name, char const *__stream_name, struct pa_sample_spec *__sample_spec = nullptr, boost::uint8_t *__buff = nullptr, std::size_t __file_size = 0);
	private:
	off_t file_size = 0;
	boost::uint8_t *buff = nullptr;
};

}
}

# endif /*__pulse__audio__hpp*/
