# include "opencl_helper.hpp"
mdl::firefly::types::err_t mdl::firefly::opencl_helper::init() {
	cl_int any_error = CL_SUCCESS;

	if ((any_error = clGetPlatformIDs(1, &this-> platform_id, &this-> platform_count)) != CL_SUCCESS) {
		fprintf(stderr, "there was a problem getting the platform ids, error id: %d\n", any_error);
		return FFLY_FAILURE;
	}

	if ((any_error = clGetDeviceIDs(this-> platform_id, CL_DEVICE_TYPE_GPU, 1, &this-> device_id, &this-> device_count)) != CL_SUCCESS) {
		fprintf(stderr, "there was a problem getting the device ids, error id: %d\n", any_error);
		return FFLY_FAILURE;
	}

	this-> context = clCreateContext(NULL, 1, &this-> device_id, NULL, NULL, &any_error);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "failed to create the context for opencl, error code: %d\n", any_error);
		return FFLY_FAILURE;
	}

	this-> command_queue = clCreateCommandQueue(this-> context, this-> device_id, 0, &any_error);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "failed to create opencl command queue, error code: %d\n", any_error);
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::opencl_helper::load_source(char const *__file_path) {
	FILE *src_file = fopen(__file_path, "r");

	if (!src_file) {
		fprintf(stderr, "could not open the source file.\n");
		return FFLY_FAILURE;
	}

	if (this-> prog_source != nullptr) std::free(this-> prog_source);

	this-> prog_source = static_cast<char *>(malloc(MAX_SOURCE_FILE_SIZE));

	this-> prog_size = fread(this-> prog_source, 1, MAX_SOURCE_FILE_SIZE, src_file);
	fclose(src_file);

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::opencl_helper::build_prog() {
	cl_int any_error = CL_SUCCESS;

	this-> program = clCreateProgramWithSource(this-> context, 1, (char const **)&this-> prog_source, (std::size_t const *)&this-> prog_size, &any_error);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "failed to create opencl program from source, error code: %d\n", any_error);
		return FFLY_FAILURE;
	}

# ifdef ARC64
	char const *arc = "-D__ARC64";
# elif ARC32
	char const *arc = "-D__ARC32";
# elif ARC16
	char const *arc = "-D__ARC16";
# elif ARC8
	char const *arc = "-D__ARC8";
# endif
	char *c_options = strcmb(const_cast<char *>(arc), "-D__USING_OPENCL -D__OPENCL_LNG_TYPES -Itypes -I../eint_t/inc", STRCMB_FREE_NONE);
	if ((any_error = clBuildProgram(this-> program, 1, &device_id, c_options, NULL, NULL)) != CL_SUCCESS) {
		fprintf(stderr, "failed to build opencl program: %d\n", any_error);
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}

mdl::firefly::opencl_helper mdl::firefly::__opencl_helper__;
