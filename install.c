#include <stdio.h>

int main() {
	system("feh --bg-scale ~/Pictures/wallpaper.png");
	system("make");
	system("sudo make clean install");
	system("echo 'exec uwm' >> ~/.xinitrc");
	system("sudo cp uwm.desktop /usr/share/xsessions/");
	system("echo 'feh --bg-scale ~/Pictures/wallpaper.png' >> ~/.bashrc");
	system("sudo chmod +x clock.sh");
	return 0;
}
