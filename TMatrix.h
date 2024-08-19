#pragma once
#include <stdio.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

#pragma pack(push,1)
typedef struct Area
{
	uint32_t   x1;
	uint32_t   y1;
	uint32_t   x2;
	uint32_t   y2;

	char name[8];
};

class TMatrix :
	public std::vector<int>
{
public:
	TMatrix(unsigned int Sx = 1, unsigned int Sy = 1, int mset=0)
	{
		SizeX = Sx; SizeY = Sy;
		resize(Sx * Sy);
		memset(data(), mset, size());
	}
	void ReSize(unsigned int Sx = 1, unsigned int Sy = 1, int mset = 0)
	{
		SizeX = Sx; SizeY = Sy;
		resize(Sx * Sy);
		memset(data(), mset, size());
	}
	//~TMatrix();
	unsigned int SizeX;
	unsigned int SizeY;
	string Name;
	
	void HexOut(bool stop = false);

	int v(unsigned int x, unsigned int y)
	{
		return at(x + y * SizeX);
	}

	void set(unsigned int x, unsigned int y, int value)
	{
		at(x + y * SizeX) = value;
	}

	void CopySubMatrix(TMatrix *SubMatrix, Area c);
	void ScaleMatrix(TMatrix *ScaleMatrix, unsigned int Width, unsigned int Height);

};

