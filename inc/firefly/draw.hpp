# ifndef __draw__hpp
# define __draw__hpp
# include <boost/cstdint.hpp>
//void _draw_hud_background(int unsigned *__xoffset, int unsigned *__yoffset, boost::uint8_t *__pixels, boost::uint8_t *__img);
void draw_pixmap(int unsigned __xoffset, int unsigned __yoffset, boost::uint8_t *__pixels, boost::uint8_t *__pixmap, int unsigned __sizes[2], int unsigned __imgsize[2], int unsigned __xlen);
void reset_pixmap(boost::uint8_t *__pixmap, int unsigned __sizes[2]);
# endif /*__draw__hpp*/
