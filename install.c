#include <stdio.h>

int main() {
	system("feh --bg-scale wallpaper.png");
	system("make");
	system("sudo make clean install");
	system("echo 'exec uwm' >> ~/.xinitrc");
	system("sudo cp uwm.desktop /usr/share/xsessions/");
	system("echo 'feh --bg-scale ~/uwm/wallpaper.png' >> ~/.bashrc");
	system("sudo chmod +x clock.sh");
	return 0;
}
