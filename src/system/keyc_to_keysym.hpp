# ifndef __keyc__to__keysym__hpp
# define __keyc__to__keysym__hpp

# if defined(__USING_X11) || defined(USING_X11)
# include "../graphics/window.hpp"
# include <X11/X.h>
# include <X11/XKBlib.h>
namespace mdl {
namespace firefly {
namespace system {
KeySym keyc_to_keysym(decltype(firefly::graphics::window::wd_handler) *__wd_handle, KeyCode __key_code)	{


}

KeySym keyc_to_keysym(Display *__display, KeyCode __key_code) {
	KeySym key_sym = NoSymbol;
	XkbDescPtr keyboard_map = XkbGetMap(__display, XkbAllClientInfoMask, XkbUseCoreKbd);
	int unsigned group = 0x00;
	if (keyboard_map != NULL) {
		//u8_t info = XkbKeyGroupInfo(keyboard_map, __key_code);
		int unsigned no_groups = XkbKeyNumGroups(keyboard_map, __key_code);

		XkbKeyTypePtr key_type = XkbKeyKeyType(keyboard_map, __key_code, group);

		int unsigned level = 0x00;
		for (int unsigned i = 0; i < key_type-> map_count; i++) {
			if (key_type-> map[i].active && key_type-> map[i].mods.mask == 0x00 & key_type-> mods.mask)
				level = key_type-> map[i].level;
		}

		key_sym = XkbKeySymEntry(keyboard_map, __key_code, level, group);
		XkbFreeClientMap(keyboard_map, XkbAllClientInfoMask, true);
	}
	return key_sym;
}
}
}
}
# endif

# endif /*__keyc__to__keysym__hpp*/
