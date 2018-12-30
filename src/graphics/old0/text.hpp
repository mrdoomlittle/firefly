# ifndef __text__hpp
# define __text__hpp
# include <boost/cstdint.hpp>

# include <fstream>
# include <emu2d.hpp>
# include <initializer_list>
namespace mdl {
namespace firefly {
namespace graphics {

class text
{
	private:
	mdl::uint_t skeleton_xlen = 0, skeleton_ylen = 0;
	boost::uint8_t *skeleton = nullptr;
	public:
	void load_skeleton(char const * __file) {
		if (this-> skeleton != nullptr) { std::free(this-> skeleton); this-> skeleton = nullptr; }
		FILE *ifile = fopen(__file, "rb");

		fread(&skeleton_xlen, sizeof(mdl::uint_t), 1, ifile);
		fread(&skeleton_ylen, sizeof(mdl::uint_t), 1, ifile);
		//printf("loading skeleton, size %dx%d.\n", skeleton_xlen, skeleton_ylen);
		this-> skeleton = static_cast<boost::uint8_t *>(malloc(skeleton_xlen * skeleton_ylen));
		bzero(this-> skeleton, skeleton_xlen * skeleton_ylen);
		fread(this-> skeleton, 1, skeleton_xlen * skeleton_ylen, ifile);

		fclose(ifile);
	}

	char const *get_skel_file(char __letter) {
		switch(__letter) {
			// letter a
			case 'a':
				return "assets/letter_a.skel";
			break;
			case 'A':
				return "assets/letter_A.skel";
			break;

			// letter b
			case 'b':
				return "assets/letter_b.skel";
			break;
			case 'B':
				return "assets/letter_B.skel";
			break;

			// letter c
			case 'c':
				return "assets/letter_c.skel";
			break;
			case 'C':
				return "assets/letter_C.skel";
			break;

			// letter d
			case 'd':
				return "assets/letter_d.skel";
			break;
			case 'D':
				return "assets/letter_D.skel";
			break;

			// letter e
			case 'e':
				return "assets/letter_e.skel";
			break;
			case 'E':
				return "assets/letter_E.skel";
			break;

			// letter f
			case 'f':
				return "assets/letter_f.skel";
			break;
			case 'F':
				return "assets/letter_F.skel";
			break;

			// letter g
			case 'g':
				return "assets/letter_g.skel";
			break;
			case 'G':
				return "assets/letter_G.skel";
			break;

			// letter h
			case 'h':
				return "assets/letter_h.skel";
			break;
			case 'H':
				return "assets/letter_H.skel";
			break;

			case 'i':
				return "assets/letter_i.skel";
			break;
			case 'I':
				return "assets/letter_I.skel";
			break;

			case 'j':
				return "assets/letter_j.skel";
			break;
			case 'J':
				return "assets/letter_J.skel";
			break;

			case 'k':
				return "assets/letter_k.skel";
			break;
			case 'K':
				return "assets/letter_K.skel";
			break;

			case 'l':
				return "assets/letter_l.skel";
			break;
			case 'L':
				return "assets/letter_L.skel";
			break;

			case 'm':
				return "assets/letter_m.skel";
			break;
			case 'M':
				return "assets/letter_M.skel";
			break;

			case 'n':
				return "assets/letter_n.skel";
			break;
			case 'N':
				return "assets/letter_N.skel";
			break;

			case 'o':
				return "assets/letter_o.skel";
			break;
			case 'O':
				return "assets/letter_O.skel";
			break;

			case 'p':
				return "assets/letter_p.skel";
			break;
			case 'P':
				return "assets/letter_P.skel";
			break;

			case 'q':
				return "assets/letter_q.skel";
			break;
			case 'Q':
				return "assets/letter_Q.skel";
			break;

			case 'r':
				return "assets/letter_r.skel";
			break;
			case 'R':
				return "assets/letter_R.skel";
			break;

			case 's':
				return "assets/letter_s.skel";
			break;
			case 'S':
				return "assets/letter_S.skel";
			break;

			case 't':
				return "assets/letter_t.skel";
			break;
			case 'T':
				return "assets/letter_T.skel";
			break;

			case 'u':
				return "assets/letter_u.skel";
			break;
			case 'U':
				return "assets/letter_U.skel";
			break;

			case 'v':
				return "assets/letter_v.skel";
			break;
			case 'V':
				return "assets/letter_V.skel";
			break;

			case 'w':
				return "assets/letter_w.skel";
			break;
			case 'W':
				return "assets/letter_W.skel";
			break;

			case 'x':
				return "assets/letter_x.skel";
			break;
			case 'X':
				return "assets/letter_X.skel";
			break;

			case 'y':
				return "assets/letter_y.skel";
			break;
			case 'Y':
				return "assets/letter_Y.skel";
			break;

			case 'z':
				return "assets/letter_z.skel";
			break;
			case 'Z':
				return "assets/letter_Z.skel";
			break;
			// numbers
			case '0':
				return "assets/letter_0.skel";
			break;
			case '1':
				return "assets/letter_1.skel";
			break;
			case '2':
				return "assets/letter_2.skel";
			break;
			case '3':
				return "assets/letter_3.skel";
			break;
			case '4':
				return "assets/letter_4.skel";
			break;
			case '5':
				return "assets/letter_5.skel";
			break;
			case '6':
				return "assets/letter_6.skel";
			break;
			case '7':
				return "assets/letter_7.skel";
			break;
			case '8':
				return "assets/letter_8.skel";
			break;
			case '9':
				return "assets/letter_9.skel";
			break;
			case ' ':
				return "assets/letter_space.skel";
			break;
		}
		return nullptr;
	}

	void draw_string(char const *__string, boost::uint8_t *__pixdata, std::initializer_list<uint_t> __coords,
		std::initializer_list<uint_t> __pixmap_size, uint_t __scale, std::initializer_list<boost::uint8_t> __colour, uint_t __spacing) {
		uint_t str_len = strlen(__string);

		if (__coords.size() != 2 || __pixmap_size.size() != 2 || __colour.size() != 4) return;
		mdl::uint_t xcoords = *__coords.begin(), ycoords = *(__coords.begin() + 1);

		uint_t xoffset = xcoords;
		for (std::size_t o = 0; o != str_len; o ++) {
			char const *__file = get_skel_file(__string[o]);
			if (__file == nullptr) return;
			load_skeleton(__file);

			draw_letter(0, __pixdata, {xoffset, ycoords}, __pixmap_size, __scale, __colour);
			xoffset += (skeleton_xlen * __scale) + __spacing;
		}

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
}
}

# endif /*__text__hpp*/
