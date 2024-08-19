#include "imgCluster.h"
#include <fstream>
#include <iostream>


void imgCluster::Line(int x1, int y1, int x2, int y2, uint32_t color, char Type)
{
    int x = x1;  int y = y1;
    int dx=1; int dy=1;

    if (x1 > x2) dx = -1;
    if (y1 > y2) dy = -1;
    
    if (Type == 'l' or Type == 'x')
    {
        int k;
        if (x2 - x1) k = (y2 - y1) / (x2 - x1); else k=0;
        int b = y1-k*x1;
        for (x = x1; x <= x2; x += dx)
        {
            y = k * x + b;
            int n = x + y * Width;
            if (n < 0) n = 0;
            bitAraay.at(n).Red = color;
            bitAraay.at(n).Green = color >> 8;
            bitAraay.at(n).Blue = color >> 16;
        }
    }
    if (Type == 'r' or Type == 'x')
     for (x = x1; x<=x2; x+=dx)
         for (y = y1; y <= y2; y += dy)
         {
             if (x == x1 or x == x2 or y == y1 or y == y2)
             {
                 bitAraay.at(x + y * Width).Red = 0xff & color;
                 bitAraay.at(x + y * Width).Green = 0xff & (color >> 8);
                 bitAraay.at(x + y * Width).Blue = 0xff & (color >> 16);
             }
         }
};

//void imgCluster::Circle(int x1, int y1, int r, uint32_t color)
//{
  //  bitAraay.at(x1 + y1 * Width) = 0xff;
//};

void imgCluster::monoChrome(uint32_t level)
{
    if (BitCount == 24)
    {
        // lenStrMono = Width / 8 + (Width % 8 ? 1 : 0);//получили количество байт приходящихся на одну монохромную строку

        //monoImg.resize(lenStrMono * Height);
         //memset(monoImg.data(), 0, monoImg.size());

        Cls = new TMatrix(Width, Height);

        uint32_t m = 0; //счетчик байтов в monoImg
        uint32_t i = 0; // счётчик пикселей в монохромной строке
        for (int k = 0; k < Width * Height; k++)
        {
            uint32_t c = bitAraay.at(k).Red + bitAraay.at(k).Blue + bitAraay.at(k).Green;
            if (c < level)
            {
                //   monoImg.at(m) |= (1 << (7 - (i % 8)));
                Cls->at(k) = 1;
                bitAraay.at(k).Red = 0xff; bitAraay.at(k).Blue = 0xff;  bitAraay.at(k).Green = 0xff;
            }
            else
            {
                bitAraay.at(k).Red = 0; bitAraay.at(k).Blue = 0;  bitAraay.at(k).Green = 0;
            }//monoImg.at(m) &= ~(1 << (7 - (i%8)));

            i++;
            if (i % 8 == 0 or i % Width == 0) m++;
            if (i % Width == 0) i = 0;
        }
    }
    else printf("ERROR: This format immage not use");

}

vector<Area> imgCluster::FindClusters(uint32_t deep, uint32_t minCount)
{
    int Ncl = 1;
    vector<Area> cl;

    for (int k = Width * Height-1; k >= 0 ; k--)
    if (Cls->at(k)==1)
    {
            Ncl = (++Ncl)|1;
            uint32_t X0 = k % Width;
            uint32_t Y0 = k / Width;
            vector<Point> Pix;
            Pix.push_back({ X0,  Y0 });
            Area aCl = { X0, Y0, X0, Y0 };
            Cls->at(X0 + Y0 * Width) = Ncl;

            while (!Pix.empty())
            {
                // printf("Point: (%d , %d) \n", Pix.front().x, Pix.front().y);
                uint32_t Xbegin = (Pix.front().x >= deep) ? Pix.front().x - deep : 0;
                uint32_t Ybegin = (Pix.front().y >= deep) ? Pix.front().y - deep : 0;
                uint32_t Xend = (Pix.front().x + deep < Width) ? Pix.front().x + deep : Width;
                uint32_t Yend = (Pix.front().y + deep < Height) ? Pix.front().y + deep : Height;

                for (uint32_t y = Ybegin; y <= Yend; y++)
                    for (uint32_t x = Xbegin; x <= Xend; x++)
                        if (Cls->at(x + y * Width)==1) // 
                        {
                            Pix.push_back({ x, y });
                            Cls->at(x + y * Width) = Ncl;
                            if (x < aCl.x1) aCl.x1 = x;
                            if (y < aCl.y1) aCl.y1 = y;
                            if (x > aCl.x2) aCl.x2 = x;
                            if (y > aCl.y2) aCl.y2 = y;
                        }
                Pix.erase(Pix.cbegin());
            }
        aCl.y2++; aCl.x2++;

        if ((aCl.x2 - aCl.x1) * (aCl.y2 - aCl.y1) < minCount)
        {
             for (uint32_t y = aCl.y1; y <= aCl.y2; y++)
                for (uint32_t x = aCl.x1; x <= aCl.x2; x++)
                if (Cls->at(x + y * Width) == Ncl) Cls->at(x + y * Width) = 1;
        }
        else cl.push_back(aCl);
    }
    return cl;
}

void imgCluster::GetShablons(uint32_t deep, uint32_t minCount)
{
    monoChrome(380); //получение монохромного изображения
    uint32_t nc = 0;
    
    string phFile = InPath + "/" + NameImg + ".dat";
    ifstream f(phFile);

    if (f.is_open())
    {
        while (!f.eof())
        {
            Area cl;
            f.read(reinterpret_cast<char*>(&cl), sizeof(cl));
            printf("Read Cluster %s: (%d , %d)  (%d , %d)    \n", cl.name,  cl.x1, cl.y1, cl.x2, cl.y2);
            Line(cl.x1, cl.y1, cl.x2, cl.y2, 0x00ff, 'r');
            TMatrix src;
            Cls->CopySubMatrix(&src, cl);
            src.Name[0] = cl.name[0];
            Segments.push_back(src);
        }
    }
    else
    {
        vector<Area> clasters = FindClusters(deep, minCount);
        ofstream out(phFile);
        for (auto c = clasters.begin(); c < clasters.end(); c++)
        {
                Area cl = *c;
                printf("Cluster N %d : (%d , %d)  (%d , %d)  dX = %d   dY= %d\n", ++nc, cl.x1, cl.y1, cl.x2, cl.y2, cl.x2 - cl.x1, cl.y2 - cl.y1);
                Line(cl.x1, cl.y1, cl.x2, cl.y2, 0x00ff, 'r');

                TMatrix src;
                Cls->CopySubMatrix(&src, cl);

                char ch = (char)openWin(&src, 1500, 500, 0xff00);
                cl.name[0] = ch;
                if (out.is_open()) out.write(reinterpret_cast<char*>(&cl), sizeof(cl));
                src.Name[0] = cl.name[0];
                Segments.push_back(src);
        }
        out.close();
    }
    OutSegments();
}

vector<Area> imgCluster::SortingInLine(vector<Area> &clasters)
{
    vector<Area> out;
    Area min; //memset(&min, 0, sizeof(min));
    auto middleY{ [](Area cl) { return cl.y1 + (cl.y2 - cl.y1); } };
    auto middleX{ [](Area cl) { return cl.x1 + (cl.x2 - cl.x1); } };
    
    bool sortEnd = true;
    while (sortEnd)
    {
        sortEnd = false;
        for (auto cl = clasters.begin(); cl < clasters.end(); cl++)
        {
            if ((cl + 1 < clasters.end()) && (middleY(*cl) > middleY(*(cl + 1))))
            {
                Area buf = *cl;
                *cl = *(cl + 1);
                *(cl + 1) = buf;
                sortEnd = true;
            }

        }
    }

    sortEnd = true;
    while (sortEnd)
    {
        sortEnd = false;
        for (auto cl = clasters.begin(); cl < clasters.end(); cl++)
        {
            if ((cl + 1 < clasters.end()) && (middleX(*cl) > middleX(*(cl + 1))))
            {
                Area buf = *cl;
                *cl = *(cl + 1);
                *(cl + 1) = buf;
                sortEnd = true;
            }

        }
    }
    return out;
}

void imgCluster::GetSegments(uint32_t deep, uint32_t minCount, uint32_t monohromLevel)
{
    monoChrome(monohromLevel); // получаем монохромное изображение
    vector<Area> clasters = FindClusters(deep, minCount); // делим изображение на сегменты
    SortingInLine(clasters); 
    for (auto cl = clasters.begin(); cl < clasters.end(); cl++)
    {
        Line(cl->x1, cl->y1, cl->x2, cl->y2, 0x00ff, 'r');
        Area claster = *cl;
        TMatrix src;
        Cls->CopySubMatrix(&src, claster);
        Segments.push_back(src);
    }
}

void imgCluster::OutSegments()
{
    string NameP = OutPath + "/" + NameImg + "/";
    int r = _mkdir(NameP.c_str());
    int nc = 0;
    for (auto a = Segments.begin(); a < Segments.end(); a++)
    {
        string NameF;
        if (a->Name[0]) NameF.push_back(a->Name[0]);

        NameF.push_back('_');
        NameF.append(to_string(++nc));
        NameF.append(".bmp"); //NameFile.append(to_string(nc));
        Save_Bmp_mono(NameP + NameF, &(*a), false);
    }
}


string imgCluster::CompareSegments(vector<TMatrix> Shablons) 
{
    string res;
    for (auto a = Segments.begin(); a < Segments.end(); a++)
    {
        int last_R = 0;
        map<vector<TMatrix>::iterator, int> Rating;

        for (auto b = Shablons.begin(); b < Shablons.end(); b++)
        {
            unsigned int Width;
            unsigned int Height;
            TMatrix scaleA;
            TMatrix scaleB;
            if (a->SizeX <= b->SizeX && a->SizeY <= b->SizeY) 
            {
                Width = a->SizeX; Height = a->SizeY;
                scaleA = *a;
                b->ScaleMatrix(&scaleB, Width, Height);
            }
            else 
                if (a->SizeX >= b->SizeX && a->SizeY >= b->SizeY)
                {
                    Width = b->SizeX; Height = b->SizeY;
                    scaleB = *b;
                    a->ScaleMatrix(&scaleA, Width, Height);
                }
                else
                {
                   // printf("Error Scale \n");
                    Width = 20; Height = 20;
                    a->ScaleMatrix(&scaleA, Width, Height);
                    b->ScaleMatrix(&scaleB, Width, Height);
                }

            int ComFl = 0;
            for (unsigned int i = 0; i < Width*Height; i++)
                    if ((scaleA.at(i) ? 1 : 0) == (scaleB.at(i) ? 1 : 0)) ComFl++;
            
            unsigned int  R = ComFl*10000 / (Width * Height);
            if (Width>10 && Height>10 && R > last_R) 
                { last_R = R; Rating[b] = R; 
                  //printf("%d --> %d x %d--> %d --> %c   ", ComFl, Width, Height, R, b->Name[0]);
                 //openWin(&scaleA, 10, 500, 0xff00);
                 //openWin(&scaleB, 1500, 500, 0xff00);
            }

        }
        
        for (auto it = Rating.begin(); it != Rating.end(); ++it)
            if (last_R - it->second < 50) a->Name.push_back(it->first->Name[0]);
                
        //printf("%d --> %c   ", it->second, it->first->Name[0]);
        //printf("%s \n ------------------------ \n", a->Name.c_str());
    }
    for (auto a = Segments.begin(); a < Segments.end(); a++)
    {
        int next = (a!=Segments.end()-1) ? (a+1)->Name[0] : 0;
        char ch = a->Name[0];
        if (a->Name.size() > 1)
           for (int i = 1; i < a->Name.size(); i++)
                if (abs(a->Name[i - 1] - next) > abs(a->Name[i] - next)) ch = a->Name[i];
        res.push_back(ch);
    }
    return res;
}
/*
TMatrix imgCluster::HashVector(uint8_t divX, uint8_t divY, Area cl)
{
    //TMatrix src(cl.x2 - cl.x1, cl.y2 - cl.y1);
   // unsigned int SizeX = cl.x2 - cl.x1;
   // unsigned int SizeY = cl.y2 - cl.y1;
    unsigned int dX = ((cl.x2 - cl.x1) / divX) ? ((cl.x2 - cl.x1) / divX) : 1;
    unsigned int dY = ((cl.y2 - cl.y1) / divY) ? ((cl.y2 - cl.y1) / divY) : 1;

    //TMatrix Hash(divX + (((cl.x2 - cl.x1) % divX) ? 1 : 0), divY + (((cl.y2 - cl.y1) % divY) ? 1 : 0));
      TMatrix Hash( divX + 1, divY + 1 );
    //printf("SizeX %d, SizeY %d, Size %d", Hash.SizeX, Hash.SizeY, Hash.size());
    //printf("  dX %d, dY %d", dX, dY);

    uint8_t HX=0; 
    uint8_t HY=0;
    int b = 0;

    while (true)
    {
        for (int y = cl.y1 + HY * dY; y < cl.y1 + HY * dY + dY; y++)
            for (int x = cl.x1 + HX*dX; x < cl.x1 + HX*dX + dX; x++)
        {
                if(x<cl.x2 and y<cl.y2) 
                    if (Cls->v(x, y)) b++;
                //printf("\n  HX %d, HY %d, x %d, y %d", HX, HY, x, y);
        }
        Hash.set(HX, HY, b*1000/(dX*dY));
        b = 0; 
        if (++HX >= Hash.SizeX) { HX = 0; if (++HY >= Hash.SizeY) break; }
    }
    return Hash;
}
*/
