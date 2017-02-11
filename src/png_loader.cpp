# include "png_loader.hpp"
boost::uint8_t mdl::load_png_file(char const *__filename, boost::uint8_t *&__pixmap, int unsigned *__pixmap_size) {
	FILE *file = fopen(__filename, "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) return 1;

	png_infop info = png_create_info_struct(png);
	if (!info) return 1;

	if(setjmp(png_jmpbuf(png))) return 1;

	png_init_io(png, file);
	png_read_info(png, info);

	__pixmap_size[0] = png_get_image_width(png, info);
	__pixmap_size[1] = png_get_image_height(png, info);
	png_byte color_type = png_get_color_type(png, info);
	png_byte bit_depth = png_get_bit_depth(png, info);

	if (bit_depth != 8 || color_type != PNG_COLOR_TYPE_RGB_ALPHA) return 1;

	boost::uint8_t *pixmap = static_cast<boost::uint8_t *>(malloc((__pixmap_size[0] * __pixmap_size[1]) * 4));
	bzero(pixmap, ((__pixmap_size[0] * __pixmap_size[1]) * 4));

	for (std::size_t y = 0; y != __pixmap_size[1]; y ++)
		png_read_row(png, pixmap + (y * (__pixmap_size[0] * 4)), NULL);

	fclose(file);

	__pixmap = pixmap;

	return 0;
}
