#pragma once

#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "TMatrix.h"
using namespace std;

#pragma pack(push,1)
typedef struct tagBITMAPFILEHEADER
{
	uint16_t   bfType;
	uint32_t   bfSize;
	uint16_t   bfReserved1;
	uint16_t   bfReserved2;
	uint32_t   bfOffBits; //bfOffBits ����������, ��� ���������� ��� ������� ������ ������������ ������ �����
} BITMAPFILEHEADER;

#pragma pack(push,1)
typedef struct tagBITMAPINFOHEADER // � ����������� �� ������� ��������� �������� ������� ����� ??? biWidth � biHeight  ����� ���� 4 ��� 8 ������
{
	uint32_t   biSize;	//biSize - ��� ������ ����� ���������
	uint32_t   biWidth; // biWidth � biHeight ������ �������������� ������ � ������ �������� � ��������
	uint32_t   biHeight;
	uint16_t   biPlanes;		//biPlanes ������ ���������� ����������. ���� ��� ������ ��������������� � 1.
	uint16_t   biBitCount;		//biBitCount - ���������� ��� �� ���� �������.
	uint32_t  biCompression;	//���������� ��� ������
	uint32_t  biSizeImage;		// biSizeImage ���������� ������ �������� � ������.
	uint32_t   biXPelsPerMeter;
	uint32_t  biYPelsPerMeter;
	uint32_t  biClrUsed;
	uint32_t  biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(push,1)
typedef struct ColorPix
{
	uint8_t   Blue;
	uint8_t   Green;
	uint8_t   Red;
};

BITMAPINFOHEADER Load_Bmp24(string NameFile, vector<ColorPix>* bitAraay, bool printParametrs = true);
void Save_Bmp24b(string NameFile, vector<ColorPix>bitAraay, unsigned int  Width, unsigned int Height); // 24-������ ����
void Save_Bmp_mono(string NameFile, TMatrix* bufA, bool print = true); //  ����������� �����������
unsigned int  openWin(TMatrix* b, unsigned int x, unsigned int y, unsigned int color);
