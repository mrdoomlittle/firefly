# ifndef __text__hpp
# define __text__hpp
# include <boost/cstdint.hpp>

# define SKELETON_XLEN 5
# define SKELETON_YLEN 6
constexpr boost::uint8_t LA_SKELETON[SKELETON_YLEN][SKELETON_XLEN] = {
	{0x0, 0x0, 0x1, 0x0, 0x0},
	{0x0, 0x1, 0x1, 0x1, 0x0},
	{0x1, 0x0, 0x0, 0x0, 0x1},
	{0x1, 0x1, 0x0, 0x1, 0x1},
	{0x1, 0x0, 0x0, 0x0, 0x1},
	{0x1, 0x0, 0x0, 0x0, 0x1}
};

constexpr boost::uint8_t LB_SKELETON[SKELETON_YLEN][SKELETON_XLEN] = {
	{0x1, 0x1, 0x1, 0x0},
	{0x1, 0x0, 0x0, 0x1},
	{0x1, 0x1, 0x1, 0x0},
	{0x1, 0x0, 0x0, 0x1},
	{0x1, 0x1, 0x1, 0x0}
};

# define LETTER_A 0
# define LETTER_B 1
# include <fstream>
# include <emu2d.hpp>
# include <initializer_list>
namespace mdl { class text
{
	private:
	mdl::uint_t skeleton_xlen = 0, skeleton_ylen = 0;
	boost::uint8_t *skeleton = nullptr;
	public:
	void load_skeleton(char const * __file) {
		if (this-> skeleton != nullptr) std::free(this-> skeleton);
		FILE *ifile = fopen(__file, "rb");

		fread(&skeleton_xlen, sizeof(mdl::uint_t), 1, ifile);
		fread(&skeleton_ylen, sizeof(mdl::uint_t), 1, ifile);
		printf("loading skeleton, size %dx%d.\n", skeleton_xlen, skeleton_ylen);
		this-> skeleton = static_cast<boost::uint8_t *>(malloc(skeleton_xlen * skeleton_ylen));
		bzero(this-> skeleton, skeleton_xlen * skeleton_ylen);
		fread(this-> skeleton, 1, skeleton_xlen * skeleton_ylen, ifile);

		fclose(ifile);
	}

	boost::uint8_t get_bstate(mdl::uint_t __x, mdl::uint_t __y, mdl::uint_t __id) {
		return this-> skeleton[emu2d(__x, __y, skeleton_xlen, skeleton_ylen, 1)];
	}

	boost::int8_t draw_letter(mdl::uint_t __id, boost::uint8_t *__pixmap, std::initializer_list<mdl::uint_t> __coords, std::initializer_list<mdl::uint_t> __pixmap_size, 
		mdl::uint_t __scale, std::initializer_list<boost::uint8_t> __colour) {
		if (__coords.size() != 2 || __pixmap_size.size() != 2 || __colour.size() != 4) return -1;
		mdl::uint_t xcoords = *__coords.begin(), ycoords = *(__coords.begin() + 1);
		mdl::uint_t pixmap_xlen = *__pixmap_size.begin(), pixmap_ylen = *(__pixmap_size.begin() + 1);
		mdl::uint_t scx = 0, scy = 0, sx = 0, sy = 0;
		boost::uint8_t r = *__colour.begin(), g = *(__colour.begin() + 1);
		boost::uint8_t b = *(__colour.begin() + 2), a = *(__colour.begin() + 3);

		for (std::size_t y = 0; y != this-> skeleton_ylen * __scale; y ++) {
			for (std::size_t x = 0; x != this-> skeleton_xlen * __scale; x ++) {
				mdl::uint_t coords = mdl::emu2d(x + xcoords, y + ycoords, pixmap_xlen, pixmap_ylen, 4);

				if (get_bstate(sx, sy, __id) == 0x1) {
					__pixmap[coords] = r;
					__pixmap[coords + 1] = g;
					__pixmap[coords + 2] = b;
					__pixmap[coords + 3] = a;
				} else {
					__pixmap[coords] = 0x0;
					__pixmap[coords + 1] = 0x0;
					__pixmap[coords + 2] = 0x0;
					__pixmap[coords + 3] = 0x0;
				}

				if (scx == __scale - 1) {
					sx ++;
					scx = 0;
				} else scx ++;
			}

			sx = 0;
			if (scy == __scale - 1) {
				sy ++;
				scy = 0;
			} else scy ++;
		}
	}
};

}

# endif /*__text__hpp*/
