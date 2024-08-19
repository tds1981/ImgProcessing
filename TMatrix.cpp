#include "TMatrix.h"

/*

int TMatrix::operator< (unsigned int x)
{
	return x; //at(x + y * SizeX)
}

int TMatrix::operator> (unsigned int y)
{
	return y; //at(x + y * SizeX)
}
*/
void TMatrix::HexOut(bool stop)
{
	for (unsigned int i = 0; i < size(); i++)
	{
		if (i % 16 == 0) printf("\n %d : ", i);
		printf(" %X ", at(i));
	}
	printf("\n");
	if(stop) std::cin.get();
}

void TMatrix::CopySubMatrix(TMatrix *SubMatrix, Area c)  //, TMatrix* src)
{
	//TMatrix src = new TMatrix(c.x2 - c.x1, c.y2 - c.y1);
	SubMatrix->ReSize(c.x2 - c.x1, c.y2 - c.y1);

	for (uint32_t y = c.y1; y < c.y2; y++)
		for (uint32_t x = c.x1; x < c.x2; x++)
			SubMatrix->set(x - c.x1, y - c.y1, v(x, y) & 0x01);
	
	return ;
}

void TMatrix::ScaleMatrix(TMatrix *ScaleMatrix, unsigned int Width, unsigned int Height)
{
	//TMatrix* src = new TMatrix(Width, Height);
	ScaleMatrix->ReSize(Width, Height);

	unsigned int winX; // = SizeX / Width;
	unsigned int winY; // = SizeY / Height;

		unsigned int tailX = SizeX % Width;
		unsigned int tailY = SizeY % Height;

		for (unsigned int y = 0; y < ScaleMatrix->SizeY; y++)
			for (unsigned int x = 0; x < ScaleMatrix->SizeX; x++)
			{ 			
				winY = (SizeY / Height) + ((y < tailY) ? 1 : 0);
				winX = (SizeX / Width) + ((x < tailX) ? 1 : 0);
				int value = 0;
				for (unsigned int i = y * winY + ((y >= tailY) ? tailY : 0); i < (y + 1) * winY + ((y >= tailY) ? tailY : 0); i++)
					for (unsigned int k = x * winX + ((x >= tailX) ? tailX : 0); k < (x + 1) * winX + ((x >= tailX) ? tailX : 0); k++)
						value += v(k, i);

				ScaleMatrix->set(x, y, value);
			}			
}
