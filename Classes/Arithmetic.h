#ifndef  __Arithmetic_H_
#define  __Arithmetic_H_
#include "Bead.h"


using namespace std;


class Point
{
public:
    Point(int px, int py):x(px),y(py) {}
    Point(const Point& p){x=p.x;y=p.y;}
    int x;
    int y;
};


class  Arithmetic
{
public:
    static bool calcPath(Bead** board,int board_widht,int board_height,Point& from,Point& target,vector<Point>& path);

    static bool rightSlantingScan(Bead** board,int board_widht,int board_height,Point& target,vector<Point>& path);
    static bool leftSlantingScan(Bead** board,int board_widht,int board_height,Point& target,vector<Point>& path);
    static bool verticalScan(Bead** board,int board_widht,int board_height,Point& target,vector<Point>& path);
    static bool horizontalScan(Bead** board,int board_widht,int board_height,Point& target,vector<Point>& path);
    static int calcScore(int h_num, int v_num, int left_slant_num, int right_slant_num);
    static int calcScore(int num);
};

#endif // __Arithmetic_H_

