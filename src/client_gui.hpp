# ifndef __client__gui__hpp
# define __client__gui__hpp
# include <boost/cstdint.hpp>
# include <boost/numeric/ublas/vector.hpp>
# include <boost/array.hpp>
# include "draw.hpp"
# include "types.hpp"
# include "../eint_t/inc/eint_t.hpp"
namespace ublas = boost::numeric::ublas;
namespace mdl { class client_gui
{
	public:
	client_gui(boost::uint8_t *__wpixmap, mouse_coords_t *__mouse_coords, bool *__button_press, int *__button_code, uint_t __window_xlen, uint_t __window_ylen);
	boost::uint8_t create_button(boost::uint8_t *__pixmap, boost::uint8_t *__hpixmap, void (* __hover_action)(), void (* __press_action)(int), uint_t __xaxis, uint_t __yaxis, uint_t __xlen, uint_t __ylen);

	void draw_buttons();
	bool is_mouse_inside(uint_t __xaxis, uint_t __yaxis, uint_t __xlen, uint_t __ylen);

	private:
	boost::uint8_t *wpixmap = nullptr;

	typedef struct {
		boost::array<int unsigned, 2> coords;
		boost::array<int unsigned, 2> size;
		bool do_hover_action = false;
		bool already_drawn = false;
		bool already_pressed = false;
		boost::uint8_t pixmap_id = 0;
		void (* press_action)(int) = nullptr;
		void (* hover_action)() = nullptr;
		boost::uint8_t *pixmap = nullptr;
		boost::uint8_t *hpixmap = nullptr;
	} button_info_t;

	bool *button_press;
 	int *button_code;

	mouse_coords_t *mouse_coords;
	uint_t window_xlen, window_ylen;

	ublas::vector<button_info_t> button_index;
} ;

}

# endif /*__client__gui__hpp*/
