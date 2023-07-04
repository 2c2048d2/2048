#include "Board.h"
class Window
{
private:
	Board* board_ = new Board();

public:
	void settingsInit()
	{
		initgraph(BoardSize);
		drawBackground();
	}

	void windowInit()
	{
		initgraph(BoardSize);
		drawBackground();

		board_->gameInit();
		SetWindowPos(GetHWnd(), NULL,
			(GetSystemMetrics(SM_CXSCREEN) - (blockNumX * blockSize + (blockNumX + 1) * lineSize)) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - (blockNumY * blockSize + (blockNumY + 1) * lineSize)) / 2,
			BoardSize, SWP_NOSIZE);

	}

	void drawBackground(void)
	{
		std::cout << "drawBackground" << std::endl;
		setfillcolor(cb);
		fillrectangle(0, 0, BoardSize);
	}

	void close(void)
	{
		closegraph();
	}

	void startGameLoop(void)
	{
		board_->gameloop();
	}
};