#include "Block.h"
#include <conio.h>
#define blockNumX 4
#define blockNumY 4
#define BoardSize (blockNumX * blockSize + (blockNumX + 1) * lineSize),(blockNumY * blockSize + (blockNumY + 1) * lineSize)

/*	坐标结构：

	0,0 1,0 ...
	0,1 1,1 ...
	...	...	...  */

class Board
{
private:
	Block* blocks[blockNumX][blockNumY];
	int point;
public:
	void initBlocks(void)
	{
		std::cout << "initBlocks" << std::endl;
		for (int i = 0; i < blockNumX; i++)
			for (int j = 0; j < blockNumY; j++)
				blocks[i][j] = new Block(i, j, 0); // 在这里
	}

	void drawBlocks(void)
	{
		std::cout << "drawBlocks" << std::endl;
		for (int i = 0; i < blockNumX; i++)
			for (int j = 0; j < blockNumY; j++)
				blocks[i][j]->draw();
	}

	void drawBackground(void)
	{
		std::cout << "drawBackground" << std::endl;
		setfillcolor(cb);
		fillrectangle(0, 0, BoardSize);
	}

	bool determine(Direction dire)
	{
		std::cout << "determine" << std::endl;
		if (dire == Direction::left || dire == Direction::right)
		{
			//水平判定 (水平方向，不是level这个水平（雾

			/*
			每行进行blockNumX-1次判断
			每行的第n次判断第n个和第n+1个
			情况：
				1，两相邻块数字相同且均不为空 -> 左右都可以
				2，两相邻块数字不同且均不为空 -> 跳过
				3，两块中出现空块 -> 如下
				3.1，只有左边是空块 -> 向左可以移动
				3.2，只有右边是空块 -> 向右可以移动
				PS: 跳过意味着当前两块来看不能移动，
				但是整个判定是看是否存在有可以移动的块，（意思随便一个都可
				所以跳过进行下一次判定
			*/
			for (int line = 0; line < blockNumY; line++) // 每一行
				for (int n = 0; n < blockNumX - 1; n++)
				{
					Block* block1 = blocks[n][line];//左边的块
					Block* block2 = blocks[n + 1][line];//右边的块
					std::cout << "blockLeft::  " << "x:" << block1->x << " y:" << block1->y << std::endl;
					std::cout << "blockRight:: " << "x:" << block2->x << " y:" << block2->y << std::endl;
					if (block1->num == block2->num && block1->isNotEmpty()) // case 1
						return true;
					else if (block1->num != block2->num && block1->isNotEmpty() && block2->isNotEmpty()) // case 2
						; // pass
					else if (block1->isEmpty() && block2->isNotEmpty() && dire == Direction::left) // case 3.1
						return true;
					else if (block1->isNotEmpty() && block2->isEmpty() && dire == Direction::right) // case 3.2
						return true;
				}
			std::cout << "没有可以移动的方块" << std::endl;
			return false;
		}

		else if (dire == Direction::up || dire == Direction::down)
		{
			//竖直判定 

			/*
			每列进行blockNumY-1次判断
			每列的第n次判断第n个和第n+1个
			情况：
				1，两相邻块数字相同且均不为空 -> 上下都可以
				2，两相邻块数字不同且均不为空 -> 跳过
				3，两块中出现空块 -> 如下
				3.1，只有上面边是空块 -> 向上可以移动
				3.2，只有下边是空块 -> 向下可以移动
				PS: 跳过意味着当前两块来看不能移动，
				但是整个判定是看是否存在有可以移动的块，（意思随便一个都可
				所以跳过进行下一次判定
			*/

			for (int line = 0; line < blockNumX; line++)
				for (int n = 0; n < blockNumY - 1; n++)
				{
					Block* block1 = blocks[line][n];//上面的块
					Block* block2 = blocks[line][n + 1];
					std::cout << "blockLeft::  " << "x:" << block1->x << " y:" << block1->y << std::endl;
					std::cout << "blockRight:: " << "x:" << block2->x << " y:" << block2->y << std::endl;
					if (block1->num == block2->num && block1->isNotEmpty()) // case 1
						return true;
					else if (block1->num != block2->num && block1->isNotEmpty() && block2->isNotEmpty()) // case 2
						; // pass
					else if (block1->isEmpty() && block2->isNotEmpty() && dire == Direction::up) // case 3.1
						return true;
					else if (block1->isNotEmpty() && block2->isEmpty() && dire == Direction::down) // case 3.2
						return true;
				}
			std::cout << "没有可以移动的方块" << std::endl;
			return false;
		}
		return false;
	}

	void moveBlock(Block* block, Block* target)
	{
		std::cout << "moveBlock" << std::endl;
		/*
		* 移动：
		* 把块num赋给目标
		* 删除块num
		*/
		target->setNum(block->num);
		block->setNum(0);

		std::cout << "move block:" << std::endl << "Block:" << "x,y:" << block->x << "," << block->y << std::endl;
		std::cout << "Target:" << "x,y:" << target->x << "," << target->y << std::endl;
	}

	void mixBlock(Block* block, Block* target)
	{
		std::cout << "mixBlock" << std::endl;
		/*
		* 合并：
		* 把块num删除
		* 目标num *= 2；
		*/
		block->setNum(0);
		target->setNum((target->num) * 2);
		target->mixed = true;
		point += target->num;
	}

	void moveBlocksLoop(int lineStart, int lineEnd, int nEnd, bool xMove)
	{
		/*
		* 移动的步骤：
		* 参数dire是移动方向，将它想象成重力方向
		* 单位时间内每个方块都向重力方向移动一个单位
		*
		* 命名上我作如下定义：
		* 块：将要被移动的块
		* 目标：将要被移动到的位置
		* 下落：整个移动事件
		*
		* 移动策略类似冒泡排序
		* 进入一个While true循环，每个循环记为一个时间单位
		* 每个时间单位内每个块做一次距离为一的移动
		* 如果存在被移动了的块则需要进行下一个时间单位
		* 如果没有块被移动说明下落过程结束，跳出循环
		*
		*
		* 每个时间单位内对每组块进行判定
		* 而对于判定的顺序，应当向重力方向进行判定
		* 相同高度的判定完后再判定下一高度
		* 一组块中有这么几种情况：
		* 1，块是空块 -> 无需移动
		* 2，块不是空块，目标是空块 -> 移动
		* 3，块不是空块，目标不是空块
		* -> 3.1 块与目标数字相同且块与目标在当前回合均未被合并 -> 移动，合并
		*	 3.2 块与目标数字不同或者块与目标在当前回合被合并过了 -> 无需移动
		*
		* 移动：
		* 把块num赋给目标
		* 删除块num
		*
		* 合并：
		* 把块num删除
		* 目标num *= 2；
		*
		*/
		std::cout << "moveBlocksLoop" << std::endl;
		bool isMovedBlock = true;
		Block* block;
		Block* target;
		int lineDelta;
		lineDelta = lineStart > lineEnd ? -1 : 1;
		while (isMovedBlock)
		{
			isMovedBlock = false;
			for (int line = lineStart; line != lineEnd; line += lineDelta)
				for (int n = 0; n < nEnd; n++)
				{
					block = xMove ? blocks[line + lineDelta][n] : blocks[n][line + lineDelta];
					target = xMove ? blocks[line][n] : blocks[n][line];
					std::cout << "block:: " << "x:" << block->x << " y:" << block->y << std::endl;
					std::cout << "target:: " << "x:" << target->x << " y:" << target->y << std::endl;
					if (block->isEmpty()) //case 1
						std::cout << "case1" << std::endl;
					else if (block->isNotEmpty() && target->isEmpty()) //case 2
					{
						std::cout << "case2" << std::endl;
						moveBlock(block, target);
						isMovedBlock = true;
					}
					else if (block->isNotEmpty() && target->isNotEmpty()) //case 3
						if (block->num == target->num && block->mixed == false && target->mixed == false) //case 3.1
						{
							std::cout << "case3.1" << std::endl;
							mixBlock(block, target);
							isMovedBlock = true;
						}

						else // case3.2
							std::cout << "case3.2" << std::endl;;
				}
		}
		for (int i = 0; i < blockNumX; i++)
			for (int j = 0; j < blockNumY; j++)
				blocks[i][j]->mixed = false;
	}

	void moveBlocks(Direction dire)
	{
		switch (dire)
		{
		case left:
			moveBlocksLoop(0, blockNumX - 1, blockNumY, true);
			break;
		case right:
			moveBlocksLoop(blockNumX - 1, 0, blockNumY, true);
			break;
		case up:
			moveBlocksLoop(0, blockNumY - 1, blockNumX, false);
			break;
		case down:
			moveBlocksLoop(blockNumY - 1, 0, blockNumX, false);
			break;
		default:
			break;
		}
	}

	void geneBlock(void)
	{
		std::cout << "geneBlock" << std::endl;
		int zeroBlockNum = 0;
		Block* zeroBlocks[(blockNumX * blockNumY)]; //这里没有分配内存是因为他获取到的block都已经创建过了，只需要保存指针
		if (zeroBlocks)
		{
			for (int x = 0; x < blockNumX; x++)
				for (int y = 0; y < blockNumY; y++)
					if (blocks[x][y]->num == 0)
						zeroBlocks[zeroBlockNum++] = blocks[x][y];
			int n = std::rand() % zeroBlockNum;
			int b = std::rand() % 8;
			int num = b == 0 ? 4 : 2;
			zeroBlocks[n]->setNum(num);
		}
	}

	void gameloop(void)
	{
		int n = 0;
		while (true)
		{
			std::cout << "循环次数：" << ++n << std::endl;
			char input = _getch();
			std::cout << input << std::endl;
			switch (input)
			{
			case'w':
			case'W':
			case 72:
				if (determine(Direction::up))
				{
					moveBlocks(Direction::up);
					geneBlock();
					drawBlocks();
				}

				break;
			case's':
			case'S':
			case 80:
				if (determine(Direction::down))
				{
					moveBlocks(Direction::down);
					geneBlock();
					drawBlocks();
				}
				break;
			case'a':
			case'A':
			case 75:
				if (determine(Direction::left))
				{
					moveBlocks(Direction::left);
					geneBlock();
					drawBlocks();
				}
				break;
			case'd':
			case'D':
			case 77:
				if (determine(Direction::right))
				{
					moveBlocks(Direction::right);
					geneBlock();
					drawBlocks();
				}
				break;

			}
			std::cout << "当前分数：" << point << std::endl;
		}
	}

	void gameInit(void) 
	{
		srand((unsigned)time(0));
		initBlocks();
		geneBlock();
		geneBlock();
		drawBlocks();
	}
};