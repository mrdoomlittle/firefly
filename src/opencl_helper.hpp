# ifndef __opencl__helper__hpp
# define __opencl__helper__hpp
# define CL_USE_DEPRECATED_OPENCL_1_2_APIS
# include <CL/cl.hpp>
# include <vector>
# include <string>
# include <fstream>
# define MAX_SOURCE_FILE_SIZE (0x100000)
namespace mdl {
namespace firefly {
class opencl {
	public:
	boost::int8_t init() {
		cl_int any_error = 0;

		any_error = clGetPlatformIDs(1, &this-> platform_id, &this-> platform_count);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "there was a problem getting the platform ids, error id: %d\n", any_error);
			return -1;
		}

		any_error = clGetDeviceIDs(this-> platform_id, CL_DEVICE_TYPE_GPU, 1, &this-> device_id, &this-> device_count);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "there was a problem getting the device ids, error id: %d\n", any_error);
			return -1;
		}

		this-> context = clCreateContext(NULL, 1, &this-> device_id, NULL, NULL, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "failed to create the context for opencl, error code: %d\n", any_error);
			return -1;
		}

		this-> command_queue = clCreateCommandQueue(this-> context, this-> device_id, 0, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "failed to create opencl command queue, error code: %d\n", any_error);
			return -1;
		}

		return 0;
	}

	boost::int8_t load_source(char const *__file) {
		FILE *src_file = fopen(__file, "r");

		if (!src_file) {
			fprintf(stderr, "could not open the source file.\n");
			return -1;
		}

		this-> prog_source = static_cast<char *>(malloc(MAX_SOURCE_FILE_SIZE));
		//memset(this-> prog_source, '\0', MAX_SOURCE_FILE_SIZE);

		this-> prog_size = fread(this-> prog_source, 1, MAX_SOURCE_FILE_SIZE, src_file);

		fclose(src_file);

		return 0;
	}

	boost::int8_t build_prog() {
		cl_int any_error;
		this-> program = clCreateProgramWithSource(this-> context, 1, (char const **)&this-> prog_source, (std::size_t const *)&this-> prog_size, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "failed to create opencl program from source, error code: %d\n", any_error);
			return -1;
		}

		any_error = clBuildProgram(this-> program, 1, &device_id, NULL, NULL, NULL);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "failed to build openc program: %d\n", any_error);
			return -1;
		}

		return 0;
	}

	cl_context context;
	cl_command_queue command_queue;

	cl_program program;

	char *prog_source;
	std::size_t prog_size = 0;
	cl_platform_id platform_id;
	cl_uint platform_count;

	cl_device_id device_id;
	cl_uint device_count;
};

}
}

# endif /*__opencl__helper__hpp*/
