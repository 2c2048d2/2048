#include "Block.h"
#include <conio.h>
#define blockNumX 4
#define blockNumY 4
#define BoardSize (blockNumX * blockSize + (blockNumX + 1) * lineSize),(blockNumY * blockSize + (blockNumY + 1) * lineSize)

/*	����ṹ��

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
				blocks[i][j] = new Block(i, j, 0); // ������
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
			//ˮƽ�ж� (ˮƽ���򣬲���level���ˮƽ����

			/*
			ÿ�н���blockNumX-1���ж�
			ÿ�еĵ�n���жϵ�n���͵�n+1��
			�����
				1�������ڿ�������ͬ�Ҿ���Ϊ�� -> ���Ҷ�����
				2�������ڿ����ֲ�ͬ�Ҿ���Ϊ�� -> ����
				3�������г��ֿտ� -> ����
				3.1��ֻ������ǿտ� -> ��������ƶ�
				3.2��ֻ���ұ��ǿտ� -> ���ҿ����ƶ�
				PS: ������ζ�ŵ�ǰ�������������ƶ���
				���������ж��ǿ��Ƿ�����п����ƶ��Ŀ飬����˼���һ������
				��������������һ���ж�
			*/
			for (int line = 0; line < blockNumY; line++) // ÿһ��
				for (int n = 0; n < blockNumX - 1; n++)
				{
					Block* block1 = blocks[n][line];//��ߵĿ�
					Block* block2 = blocks[n + 1][line];//�ұߵĿ�
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
			std::cout << "û�п����ƶ��ķ���" << std::endl;
			return false;
		}

		else if (dire == Direction::up || dire == Direction::down)
		{
			//��ֱ�ж� 

			/*
			ÿ�н���blockNumY-1���ж�
			ÿ�еĵ�n���жϵ�n���͵�n+1��
			�����
				1�������ڿ�������ͬ�Ҿ���Ϊ�� -> ���¶�����
				2�������ڿ����ֲ�ͬ�Ҿ���Ϊ�� -> ����
				3�������г��ֿտ� -> ����
				3.1��ֻ��������ǿտ� -> ���Ͽ����ƶ�
				3.2��ֻ���±��ǿտ� -> ���¿����ƶ�
				PS: ������ζ�ŵ�ǰ�������������ƶ���
				���������ж��ǿ��Ƿ�����п����ƶ��Ŀ飬����˼���һ������
				��������������һ���ж�
			*/

			for (int line = 0; line < blockNumX; line++)
				for (int n = 0; n < blockNumY - 1; n++)
				{
					Block* block1 = blocks[line][n];//����Ŀ�
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
			std::cout << "û�п����ƶ��ķ���" << std::endl;
			return false;
		}
		return false;
	}

	void moveBlock(Block* block, Block* target)
	{
		std::cout << "moveBlock" << std::endl;
		/*
		* �ƶ���
		* �ѿ�num����Ŀ��
		* ɾ����num
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
		* �ϲ���
		* �ѿ�numɾ��
		* Ŀ��num *= 2��
		*/
		block->setNum(0);
		target->setNum((target->num) * 2);
		target->mixed = true;
		point += target->num;
	}

	void moveBlocksLoop(int lineStart, int lineEnd, int nEnd, bool xMove)
	{
		/*
		* �ƶ��Ĳ��裺
		* ����dire���ƶ����򣬽����������������
		* ��λʱ����ÿ�����鶼�����������ƶ�һ����λ
		*
		* �������������¶��壺
		* �飺��Ҫ���ƶ��Ŀ�
		* Ŀ�꣺��Ҫ���ƶ�����λ��
		* ���䣺�����ƶ��¼�
		*
		* �ƶ���������ð������
		* ����һ��While trueѭ����ÿ��ѭ����Ϊһ��ʱ�䵥λ
		* ÿ��ʱ�䵥λ��ÿ������һ�ξ���Ϊһ���ƶ�
		* ������ڱ��ƶ��˵Ŀ�����Ҫ������һ��ʱ�䵥λ
		* ���û�п鱻�ƶ�˵��������̽���������ѭ��
		*
		*
		* ÿ��ʱ�䵥λ�ڶ�ÿ�������ж�
		* �������ж���˳��Ӧ����������������ж�
		* ��ͬ�߶ȵ��ж�������ж���һ�߶�
		* һ���������ô���������
		* 1�����ǿտ� -> �����ƶ�
		* 2���鲻�ǿտ飬Ŀ���ǿտ� -> �ƶ�
		* 3���鲻�ǿտ飬Ŀ�겻�ǿտ�
		* -> 3.1 ����Ŀ��������ͬ�ҿ���Ŀ���ڵ�ǰ�غϾ�δ���ϲ� -> �ƶ����ϲ�
		*	 3.2 ����Ŀ�����ֲ�ͬ���߿���Ŀ���ڵ�ǰ�غϱ��ϲ����� -> �����ƶ�
		*
		* �ƶ���
		* �ѿ�num����Ŀ��
		* ɾ����num
		*
		* �ϲ���
		* �ѿ�numɾ��
		* Ŀ��num *= 2��
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
		Block* zeroBlocks[(blockNumX * blockNumY)]; //����û�з����ڴ�����Ϊ����ȡ����block���Ѿ��������ˣ�ֻ��Ҫ����ָ��
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
			std::cout << "ѭ��������" << ++n << std::endl;
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
			std::cout << "��ǰ������" << point << std::endl;
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