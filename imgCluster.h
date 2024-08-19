#pragma once
#include <string>
#include <stdio.h>
#include <vector>
#include "TMatrix.h"
#include "BMP.h"
#include <cmath>
#include <map>
#include <direct.h>
//#include <stdint.h>
using namespace std;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t LONG;

void HexOut(uint32_t* ptr, uint32_t size);

#pragma pack(push,1)
typedef struct Point
{
	uint32_t   x;
	uint32_t   y;
};

class imgCluster
{
public:
	imgCluster(string oPath) {
					OutPath = oPath;
					int r = _mkdir(OutPath.c_str());
					Width = 1000; Height = 1000; BitCount = 24; 
					bitAraay.resize(Width * Height);
					lenStr = Width * (BitCount / 8);
				 }
	~imgCluster() {
		bitAraay.clear(); 
		delete Cls;
	}
	//void Circle(int x1, int y1, int r, uint32_t color);
	void Line(int x1, int y1, int x2, int y2, uint32_t color, char Type);
	void Load_Bmp(string NameFile, bool printParametrs = false)
	{ 
		BITMAPINFOHEADER bih = Load_Bmp24(NameFile, &bitAraay, printParametrs);
		Width = bih.biWidth; Height = bih.biHeight; BitCount = bih.biBitCount;
		string NameF = NameFile.substr(NameFile.find_last_of("/\\") + 1);
		NameImg = NameF.substr(0, NameF.rfind("."));
			//NameFile.substr(NameFile.rfind("/")+1, NameFile.size()- NameFile.rfind(".")-1);
		InPath = NameFile.substr(0, NameFile.rfind("/"));
		if (printParametrs) printf(" Name Path: %s ,   NAME IMG: %s \n", OutPath.c_str(), NameImg.c_str());
	}
	void Save_Bmp_m()
	{
		string nf = OutPath + NameImg + "_m.bmp";
		Save_Bmp_mono(nf, Cls);
	}
	void Save_Bmp_24b()
	{
		string nf = OutPath + "/" + NameImg + "_24b.bmp";
		Save_Bmp24b(nf, bitAraay, Width, Height);
	}

	void monoChrome(uint32_t level);	//получение монохромного изображени€
	vector<Area> FindClusters(uint32_t deep,		// глубина обхода точки
							 uint32_t minCount); // минималиный размер кластера
	void GetSegments(uint32_t deep, uint32_t minCount, uint32_t monohromLevel = 380);	// сегменты без имени
	void GetShablons(uint32_t deep, uint32_t minCount); // шаблон - поименованный сегмент
	vector<Area> SortingInLine(vector<Area> &clasters);
	//TMatrix HashVector(uint8_t divX, uint8_t divY, Area cl);
	string CompareSegments(vector<TMatrix> Shablons);
	void OutSegments();
	vector<TMatrix> Segments;
private:
	uint32_t   Width; 
	uint32_t   Height;
	uint16_t   BitCount; //BitCount -  оличество бит на один пиксель.
	vector<ColorPix> bitAraay;
	unsigned int lenStr; //длина строки в байтах дл€ массива цветных пикселей
	string NameImg;
	string OutPath;
	string InPath;
	TMatrix *Cls;
};
