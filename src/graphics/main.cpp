# include "window.hpp"
# include "../system/thread.h"

int main() {
	mdl::firefly::graphics::window wd;
	wd.init(640, 480, "Hello");
	wd.begin();
	while(1);
}
