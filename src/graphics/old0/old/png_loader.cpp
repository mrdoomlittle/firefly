# include "png_loader.hpp"
mdl::firefly::types::err_t mdl::firefly::graphics::load_png_file(char const *__dir, char const *__name, types::pixmap_t&__pixmap, uint_t *__pm_size) {
	char *pth = str_cmb(const_cast<char*>(__dir), str_cmb(const_cast<char*>(__name), ".png", 0), _mdl_stc_free_rhs);
	FILE *file = fopen(pth, "rb");

	if (file == NULL) {
		system::io::fprintf(ffly_err, "png_loader, error can't open png file at '%s', errno: %d\n", pth, errno);
		return FFLY_FAILURE;
	}

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) return FFLY_FAILURE;

	png_infop info = png_create_info_struct(png);
	if (!info) return FFLY_FAILURE;

	if(setjmp(png_jmpbuf(png))) return FFLY_FAILURE;

	png_init_io(png, file);
	png_read_info(png, info);

	__pm_size[0] = png_get_image_width(png, info);
	__pm_size[1] = png_get_image_height(png, info);
	system::io::fprintf(ffly_log, "loading img. size %dx%d\n", __pm_size[0], __pm_size[1]);
	png_byte color_type = png_get_color_type(png, info);
	png_byte bit_depth = png_get_bit_depth(png, info);

	if (bit_depth != 8 || color_type != PNG_COLOR_TYPE_RGB_ALPHA) return FFLY_FAILURE;

	types::pixmap_t pixmap = static_cast<types::pixmap_t>(memory::mem_alloc((__pm_size[0]*__pm_size[1])*4));
	bzero(pixmap, (__pm_size[0]*__pm_size[1])*4);

	for (std::size_t ya{}; ya != __pm_size[1]; ya++)
		png_read_row(png, pixmap+(ya*(__pm_size[0]*4)), NULL);

	fclose(file);
	std::free(pth);

	__pixmap = pixmap;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::load_png_file(char const *__dir, char const *__name, types::pixmap_t&__pixmap, types::_2d_dsize_t<>& __pm_size) {
	uint_t pm_size[2] = {0};
	i8_t result = load_png_file(__dir, __name, __pixmap, pm_size);
	__pm_size.xaxis_len = pm_size[0];
	__pm_size.yaxis_len = pm_size[1];
	return result;
}
