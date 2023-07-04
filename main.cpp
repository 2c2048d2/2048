#include "Window.h"

int main(void)
{
	Window* window_ = new Window();
	window_->settingsInit();
	window_->windowInit();

	window_->startGameLoop();
	window_->close();
	return 0;
}
