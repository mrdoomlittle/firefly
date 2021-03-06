# ifndef __opencl__helper__hpp
# define __opencl__helper__hpp
# define CL_USE_DEPRECATED_OPENCL_1_2_APIS
# include <CL/cl.hpp>
# include <vector>
# include <string>
# include <fstream>
# include <strcmb.hpp>
# include "types/err_t.h"
# include "system/errno.h"
# define MAX_SOURCE_FILE_SIZE (0x100000)
namespace mdl {
namespace firefly {
class opencl_helper {
	public:
	types::err_t init();

	types::err_t load_source(char const *__file_path);

	types::err_t build_prog();

	cl_context context;
	cl_command_queue command_queue;

	cl_program program;


	char *prog_source = nullptr;
	std::size_t prog_size = 0;
	cl_platform_id platform_id;
	cl_uint platform_count;

	cl_device_id device_id;
	cl_uint device_count;
};
extern opencl_helper __opencl_helper__;
}
}

# endif /*__opencl__helper__hpp*/
