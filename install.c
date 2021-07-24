#include <stdio.h>

int main() {
	system("feh --bg-scale ~/Pictures/wallpaper.png");
	system("make");
	system("sudo make clean install");
	system("echo 'exec dwm' >> ~/.xinitrc");
	system("sudo cp dwm.desktop /usr/share/xsessions/");
	system("sudo chmod +x clock.sh");
	return 0;
}
