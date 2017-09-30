# include "window.hpp"
int main() {
	mdl::firefly::graphics::window wd;
	wd.init(640, 480, "Hello");
	wd.begin();
	while(1) {
		if (!wd.is_event_buff_empty()) {
			wd.dump_event_buff();
			printf("dumped event buff.\n");
		}
	}
}
