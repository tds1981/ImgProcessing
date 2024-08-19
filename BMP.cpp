#include "BMP.h"

#include <GLFW/glfw3.h>
#include <math.h>

unsigned int finish = 0;

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    finish = codepoint;
}

unsigned int openWin(TMatrix* b, unsigned int x, unsigned int y, unsigned int color)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(300, 300, "Hello World", NULL, NULL);

    glfwSetWindowPos(window, x, y);

    if (!window )
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    finish = 0;
    glfwSetCharCallback(window, character_callback);

    /* Loop until the user closes the window */

    while (!glfwWindowShouldClose(window) && finish==0)
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3b(150,100,70);
        //glColor3b((16 >> color) & 0xff, (8>>color) & 0xff, color&0xff);
        glBegin(GL_POINTS);
     
        for (int x = 0; x < b->SizeX; x++)
            for (int y = 0; y < b->SizeY; y++)
                if (b->v(x, y))
                {
                    double Xd = (double(x) / b->SizeX)-0.5;
                    double Yd = (double(y) / b->SizeY)-0.5;
                    glVertex2f(Xd, Yd);       // Вывод битового массива
                }        
        glEnd(); 
                
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return finish;
}

BITMAPINFOHEADER Load_Bmp24(string NameFile, vector<ColorPix> *bitAraay, bool printParametrs)
{
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih; memset(&bih, 0, sizeof(bih));
 
    ifstream f;
    f.open(NameFile, ios::binary);
    if (!f.is_open()) { printf("ERROR: File not open \n"); return bih; }
    else printf("Open File: %s \n", NameFile.c_str());

    f.read(reinterpret_cast<char*> (&bfh), sizeof(bfh));
    
    if (printParametrs)
    {
        printf("bfType: %c %c \n", bfh.bfType, bfh.bfType >> 8);
        printf("bfSize: %lu \n", bfh.bfSize);
        printf("bfOffBits: %lu \n", bfh.bfOffBits);
    }

    f.read(reinterpret_cast<char*> (&bih), sizeof(bih));
    if (printParametrs)
    {
        printf("biSize: %ld \n", bih.biSize);
        printf("biWidth: %ld \n", bih.biWidth);
        printf("biHeight: %ld \n", bih.biHeight);
        printf("biPlanes: %ld \n", bih.biPlanes);
        printf("biBitCount: %d \n", bih.biBitCount);
        printf("biCompression: %d \n", bih.biCompression);
        printf("biSizeImage: %d \n", bih.biSizeImage);
        printf("biXPelsPerMeter: %d \n", bih.biXPelsPerMeter);
        printf("biYPelsPerMeter: %d \n", bih.biYPelsPerMeter);
        printf("biClrUsed: %d \n", bih.biClrUsed);
        printf("biClrImportant: %d \n", bih.biClrImportant);
        printf("bfh: %d,  bih: %d \n", sizeof(bfh), sizeof(bih));
    }

    vector<char>* buf = reinterpret_cast<vector<char>*>(bitAraay);
    buf->resize(bfh.bfSize - bfh.bfOffBits); //Width * Height

    f.read(buf->data(), buf->size());
    if (printParametrs) printf("bitAraay.size(): pixel: %d, byte: %d \n", bitAraay->size(), buf->size());

    unsigned int lenStr = bih.biWidth * (bih.biBitCount / 8);

    if (lenStr % 4)
    {
        unsigned int nulByte = 4 * (1 + lenStr / 4) - lenStr;
        auto iter = buf->cbegin() + lenStr;
        while (iter < buf->end() - 1)
        {
            iter = buf->erase(iter, iter + nulByte);
            if (iter < buf->end() - 1) iter += lenStr;
        }
    }
    return bih;
}


void Save_Bmp24b(string NameFile, vector<ColorPix>bitAraay, unsigned int  Width, unsigned int Height) // c - 24-битный цвет, 
{
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    memset(&bfh, 0, sizeof(bfh));
    memset(&bih, 0, sizeof(bih));
    bfh.bfType = 0x4D42;                           // Обозначим, что это bmp 'BM'
    bfh.bfOffBits = sizeof(bfh) + sizeof(bih);    // начало изображения относительно начала файла
    bih.biSize = sizeof(bih);                      // Так положено
    bih.biClrUsed = 0;
    // bih.biCompression = BI_RGB;                    // Без сжатия
    bih.biHeight = Height;
    bih.biWidth = Width;
    bih.biPlanes = 1;     // Должно быть 1
    bih.biBitCount = 24; //BitCount;

    vector<char>* buf = reinterpret_cast<vector<char>*>(&bitAraay);
    unsigned int lstr = Width * sizeof(ColorPix);
    unsigned int nulByte = (lstr % 4) ? 4 - lstr % 4 : 0;  //nulByte = 4 * (1 + lenStr / 4) - lenStr;
   
    bfh.bfSize = bfh.bfOffBits + Height * (lstr + nulByte);
    bih.biSizeImage = Height * (lstr + nulByte);

    printf("Save file: %s \n", NameFile.c_str());
   // printf("lstr %d, nulByte %d \n", lstr, nulByte);

    ofstream out;
    out.open(NameFile, ios::binary);
    out.write(reinterpret_cast<char*>(&bfh), sizeof(bfh));
    out.write(reinterpret_cast<char*>(&bih), sizeof(bih));

    char* nulstr = new char[nulByte]; memset(nulstr, 0, sizeof(nulstr));
    for (unsigned int i = 0; i < Height; i++)
    {
        out.write(&buf->data()[i * lstr], lstr);
        if (nulByte) out.write(nulstr, nulByte);
    }

    out.close();
}


void Save_Bmp_mono(string NameFile, TMatrix* bufA, bool print) // монохромное изображение
{
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    //unsigned int lstr;
    unsigned int nulByte;

    memset(&bfh, 0, sizeof(bfh));
    memset(&bih, 0, sizeof(bih));
    bfh.bfType = 0x4D42;                           // Обозначим, что это bmp 'BM'
    bfh.bfOffBits = sizeof(bfh) + sizeof(bih);    // начало изображения относительно начала файла

    bih.biSize = sizeof(bih);                      // Так положено
    bih.biClrUsed = 0;
    // bih.biCompression = BI_RGB;                    // Без сжатия
    bih.biHeight = bufA->SizeY;
    bih.biWidth = bufA->SizeX;
    bih.biPlanes = 1;     // Должно быть 1

    vector<char> monoImg;
    unsigned int lenStrMono = bih.biWidth / 8 + (bih.biWidth % 8 ? 1 : 0);//получили количество байт приходящихся на одну монохромную строку
    nulByte = (lenStrMono % 4) ? 4 - lenStrMono % 4 : 0;
   
    monoImg.resize((lenStrMono + nulByte) * bih.biHeight);
    memset(monoImg.data(), 0, monoImg.size());

    uint32_t m = 0; //счетчик байтов в monoImg
    uint32_t i = 0; // счётчик пикселей в монохромной строке
    for (auto iter = bufA->begin(); iter != bufA->end(); iter++)
    {
           // uint32_t c = bitAraay.at(k).Red + bitAraay.at(k).Blue + bitAraay.at(k).Green;
            if (*iter)  
            {
                monoImg.at(m) |= (1 << (7 - (i % 8)));
            }
            else monoImg.at(m) &= ~(1 << (7 - (i%8)));

            i++;
            if (i % 8 == 0) m++;
            else if (i % bih.biWidth == 0) m += 1 + nulByte;

            if (i%bih.biWidth == 0) i = 0;
    }
   
    bfh.bfOffBits += 8;  // резервируем место под палитру 2 цвета
    bih.biBitCount = 1;  // один бит на пиксель
    
    bfh.bfSize = bfh.bfOffBits + bih.biHeight * (lenStrMono + nulByte);
    bih.biSizeImage = bih.biHeight * (lenStrMono + nulByte);

    if (print) printf("Save file: %s \n", NameFile.c_str());
  //  printf("lstr %d, nulByte %d \n", lenStrMono, nulByte);

    ofstream out;
    out.open(NameFile, ios::binary);
    out.write(reinterpret_cast<char*>(&bfh), sizeof(bfh));
    out.write(reinterpret_cast<char*>(&bih), sizeof(bih));


    char Palette[8] = { 0, 0, 0, 0, 0xff, 0xff, 0xff, 0 };
    out.write(reinterpret_cast<char*>(Palette), sizeof(Palette));
    out.write(monoImg.data(), monoImg.size());

    out.close();

}