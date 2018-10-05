# ifndef __ffly__slurry__window__h
# define __ffly__slurry__window__h
# include "../ffint.h"
# include <X11/Xlib.h>
# include <GL/glx.h>
struct s_window* __s_window_new(Display*);
void __s_window_init(struct s_window*, ff_uint_t, ff_uint_t, char const*);
void __s_window_destroy(struct s_window*);
void __s_window_display(struct s_window*);
# endif /*__ffly__slurry__window__h*/
