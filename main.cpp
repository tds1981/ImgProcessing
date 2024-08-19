#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include "imgCluster.h"
#include "BMP.h"
using namespace std;


std::string GetPath(string NameFile) 
{
	return NameFile.substr(0, NameFile.find_last_of("/\\"));
}

int main(int argc, char* argv[])
{
	string InPath = GetPath(string(argv[0])) + "/" + "IN_data" + "/";
	string OutPath = GetPath(string(argv[0])) + "/" + "Out_data" + "/";
	//printf("IN: %s \n", InPath.c_str());
	printf("OUT DATA: %s \n", OutPath.c_str());
	
	imgCluster img1(OutPath);
	printf("\n LOAD SHABLONS: \n");
	img1.Load_Bmp(InPath +"TNR.bmp"); //Загрузка шаблона
	img1.GetShablons(1, 30);
	img1.Save_Bmp_24b();
	cin.get();

	imgCluster img2(OutPath);
	printf("\n Example 1 - Digit detecting : \n");
	img2.Load_Bmp(InPath + "Test_num.bmp");
	img2.GetSegments(1, 30);
	
	string res = img2.CompareSegments(img1.Segments); // сравнение сегментов
	printf("\n RESULT COMPARE: \n");
	printf("%s \n", res.c_str()); // результат распознавания сегментов
	img2.OutSegments();
	//img2.Save_Bmp_m();
	img2.Save_Bmp_24b();
	cin.get();
	
	imgCluster img3(OutPath);;
	printf("\n Example 2 - Detection letters of different sizes: \n");
	img3.Load_Bmp(InPath + "A.bmp");
	img3.GetSegments(1, 30);
	img3.Save_Bmp_24b();

	res = img3.CompareSegments(img1.Segments); // сравнение сегментов
	printf("\n RESULT COMPARE: \n");
	printf("%s \n", res.c_str()); // результат распознавания сегментов
	img3.OutSegments();
	cin.get();

	imgCluster img4(OutPath);;
	printf("\n Example 3 - Detection  color letters: \n");
	img4.Load_Bmp(InPath + "color_letters.bmp");
	//img4.monoChrome(280);
	img4.GetSegments(1, 30, 450);
	img4.Save_Bmp_m();
	img4.Save_Bmp_24b();

	res = img4.CompareSegments(img1.Segments); // сравнение сегментов
	printf("\n RESULT COMPARE: \n");
	printf("%s \n", res.c_str()); // результат распознавания сегментов
	img4.OutSegments();

	cin.get();
	

}