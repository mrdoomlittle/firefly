# include "opencl_helper.hpp"

boost::int8_t mdl::firefly::opencl::init() {
	cl_int any_error = CL_SUCCESS;

	if ((any_error = clGetPlatformIDs(1, &this-> platform_id, &this-> platform_count)) != CL_SUCCESS) {
		fprintf(stderr, "there was a problem getting the platform ids, error id: %d\n", any_error);
		return -1;
	}

	if ((any_error = clGetDeviceIDs(this-> platform_id, CL_DEVICE_TYPE_GPU, 1, &this-> device_id, &this-> device_count)) != CL_SUCCESS) {
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

boost::int8_t mdl::firefly::opencl::load_source(char const *__file_path) {
	FILE *src_file = fopen(__file_path, "r");

	if (!src_file) {
		fprintf(stderr, "could not open the source file.\n");
		return -1;
	}

	if (this-> prog_source != nullptr) std::free(this-> prog_source);

	this-> prog_source = static_cast<char *>(malloc(MAX_SOURCE_FILE_SIZE));

	this-> prog_size = fread(this-> prog_source, 1, MAX_SOURCE_FILE_SIZE, src_file);
	fclose(src_file);

	return 0;
}

boost::int8_t mdl::firefly::opencl::build_prog() {
	cl_int any_error = CL_SUCCESS;

	this-> program = clCreateProgramWithSource(this-> context, 1, (char const **)&this-> prog_source, (std::size_t const *)&this-> prog_size, &any_error);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "failed to create opencl program from source, error code: %d\n", any_error);
		return -1;
	}

	if ((any_error = clBuildProgram(this-> program, 1, &device_id, "-DARC64 -DUSING_OPENCL -DUSE_CL_TYPES -I../src/types -I../eint_t/inc", NULL, NULL)) != CL_SUCCESS) {
		fprintf(stderr, "failed to build opencl program: %d\n", any_error);
		return -1;
	}

	return 0;
}
