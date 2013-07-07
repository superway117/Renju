#include "Arithmetic.h"
#include <queue>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

#define CELL_EMPTY (int)0xFFFFFFFF
#define CELL_LEFT  (int)0xF1000000
#define CELL_RIGHT (int)0xF2000000
#define CELL_UP    (int)0xF3000000
#define CELL_DOWN  (int)0xF4000000

typedef struct
{
    int x;
    int y;
    int direction;
}QueueItem_T;

typedef struct
{
    int *board;
    int width;
    int height;
    int from_x;
    int from_y;
    int to_x;
    int to_y;
}Board_T;

static Board_T g_board;

static bool board_init(int width, int height)
{
    if(width < 5 || width > 255 || height < 5 || height > 255)
        return false;

    g_board.board = (int*)malloc(width * height * sizeof(int));

    if(g_board.board != NULL)
    {
        g_board.width = width;
        g_board.height = height;
        return true;
    }

    return false;
}

static void board_deinit(void)
{
    free(g_board.board);
}

static bool board_blankinit(Bead** board, Point& target)
{
    if(target.x < 0 || target.x >= g_board.width || target.y < 0 || target.y >= g_board.height)
        return false;

    g_board.to_x = target.x;
    g_board.to_y = target.y;
    memset(g_board.board, -1, g_board.width * g_board.height * sizeof(int));
    for(int i = 0; i < g_board.width * g_board.height; i++)
    {
         if(board[i] != NULL)
            g_board.board[i] = board[i]->getBeadColor();
    }

    return true;
}

static void board_blankdeinit(void)
{

}

static bool board_horizontalblank(vector<Point>& path)
{
    int color = g_board.board[g_board.to_y*g_board.width+g_board.to_x];
    int x;
    CCLog("board_horizontalblank targe x=%d,y=%d",g_board.to_x,g_board.to_y);
    if(color == CELL_EMPTY)
        return false;

    x = g_board.to_x-1;
    while(x >= 0)
    {

        if(g_board.board[g_board.to_y*g_board.width+x] == color)
        {
            path.push_back(Point(x, g_board.to_y));
            CCLog("board_horizontalblank x=%d y=%d color=%d",x,g_board.to_y,color);
        }
        else
            break;
        --x;
    }

    reverse(path.begin(), path.end());

    x = g_board.to_x+1;
    while(x < g_board.width)
    {
        if(g_board.board[g_board.to_y*g_board.width+x] == color)
        {
            path.push_back(Point(x, g_board.to_y));
            CCLog("board_horizontalblank x=%d y=%d color=%d",x,g_board.to_y,color);
        }
        else
            break;
        ++x;
    }

    return (path.size() >= 4);
}

static bool board_verticalblank(vector<Point>& path)
{
    int color = g_board.board[g_board.to_y*g_board.width+g_board.to_x];
    int y;
    CCLog("board_verticalblank targe x=%d,y=%d",g_board.to_x,g_board.to_y);
    if(color == CELL_EMPTY)
        return false;

    y = g_board.to_y-1;
    while(y >= 0)
    {


        if(g_board.board[y*g_board.width+g_board.to_x] == color)
        {
            path.push_back(Point(g_board.to_x, y));
            CCLog("board_verticalblank x=%d y=%d color=%d",g_board.to_x,y,color);
        }

        else
            break;
        --y;
    }

    reverse(path.begin(), path.end());

    y = g_board.to_y+1;
    while(y < g_board.height)
    {

        if(g_board.board[y*g_board.width+g_board.to_x] == color)
        {
            path.push_back(Point(g_board.to_x, y));
            CCLog("board_verticalblank x=%d y=%d color=%d",g_board.to_x,y,color);
        }
        else
            break;
        ++y;

    }

    return (path.size() >= 4);
}

static bool board_slashblank(vector<Point>& path)
{
   int color = g_board.board[g_board.to_y*g_board.width+g_board.to_x];
   int x, y;

   if(color == CELL_EMPTY)
       return false;

   x = g_board.to_x-1;
   y = g_board.to_y-1;

   while(x >= 0 && y >= 0)
   {


        if(g_board.board[y*g_board.width+x] == color)
        {
            path.push_back(Point(x, y));
            CCLog("board_slashblank x=%d color=%d",x,color);
        }
        else
            break;
        --x;
        --y;
   }

   reverse(path.begin(), path.end());

   x = g_board.to_x+1;
   y = g_board.to_y+1;

   while(x < g_board.width && y < g_board.height)
   {

        if(g_board.board[y*g_board.width+x] == color)
        {
            path.push_back(Point(x, y));
            CCLog("board_slashblank x=%d color=%d",x,color);
        }
        else
            break;
        ++x;
        ++y;

   }

   return (path.size() >= 4);
}

static bool board_backslashblank(vector<Point>& path)
{
   int color = g_board.board[g_board.to_y*g_board.width+g_board.to_x];
   int x, y;

   if(color == CELL_EMPTY)
       return false;

   x = g_board.to_x-1;
   y = g_board.to_y+1;

   while(x >= 0 && y < g_board.height)
   {


        if(g_board.board[y*g_board.width+x] == color)
        {
            path.push_back(Point(x, y));
            CCLog("board_backslashblank x=%d color=%d",x,color);
        }
        else
            break;
        --x;
        ++y;
   }

   reverse(path.begin(), path.end());

   x = g_board.to_x+1;
   y = g_board.to_y-1;

   while(x < g_board.width && y >= 0 )
   {

        if(g_board.board[y*g_board.width+x] == color)
        {
            path.push_back(Point(x, y));
            CCLog("board_backslashblank x=%d color=%d",x,color);
        }
        else
            break;
        ++x;
        --y;

   }

   return (path.size() >= 4);
}

static bool board_searchinit(Bead** board, Point& from, Point& to)
{
    if(from.x < 0 || from.x >= g_board.width || from.y < 0  || from.y >= g_board.height)
        return false;

    if(to.x < 0 || to.x >= g_board.width || to.y < 0 || to.y >= g_board.height)
        return false;

    if(from.x == to.x && from.y == to.y)
        return false;

    if(board[from.y * g_board.width + from.x] == NULL || board[to.y * g_board.width + to.x] != NULL)
        return false;

    g_board.from_x = from.x;
    g_board.from_y = from.y;
    g_board.to_x = to.x;
    g_board.to_y = to.y;

    memset(g_board.board, -1, g_board.width * g_board.height * sizeof(int));

    for(int i = 0; i < g_board.width * g_board.height; i++)
    {
         if(board[i] != NULL)
            g_board.board[i] = board[i]->getBeadColor();
    }

    return true;
}

static void board_searchdeinit(void)
{

}

static bool board_search(void)
{
    int x, y;
    QueueItem_T openitem;
    queue<QueueItem_T> openitems;

    openitem.x = g_board.from_x;
    openitem.y = g_board.from_y;
    // direction of "from" is not used.
    openitems.push(openitem);

    while(!openitems.empty())
    {
        openitem = openitems.front();
        x = openitem.x;
        y = openitem.y;

        // CELL_EMPTY except for "from"
        if(g_board.board[y*g_board.width+x] == CELL_EMPTY)
            g_board.board[y*g_board.width+x] = openitem.direction;

        // reach "to"
        if(x == g_board.to_x && y == g_board.to_y)
            break;

        openitems.pop();

        // push adjacent empty cells
        if(x > 0 && g_board.board[y*g_board.width+x-1] == CELL_EMPTY)
        {
            openitem.x = x - 1;
            openitem.y = y;
            openitem.direction = CELL_RIGHT;
            openitems.push(openitem);
        }

        if(x + 1 < g_board.width && g_board.board[y*g_board.width+x+1] == CELL_EMPTY)
        {
            openitem.x = x + 1;
            openitem.y = y;
            openitem.direction = CELL_LEFT;
            openitems.push(openitem);
        }

        if(y > 0 && g_board.board[(y-1)*g_board.width+x] == CELL_EMPTY)
        {
            openitem.x = x;
            openitem.y = y - 1;
            openitem.direction = CELL_UP;
            openitems.push(openitem);
        }

        if(y + 1 < g_board.height && g_board.board[(y+1)*g_board.width+x] == CELL_EMPTY)
        {
            openitem.x = x;
            openitem.y = y + 1;
            openitem.direction = CELL_DOWN;
            openitems.push(openitem);
        }
    }

    return !openitems.empty();
}

static void board_search_result(vector<Point>& path)
{
    int x, y;

    x = g_board.to_x;
    y = g_board.to_y;

    while(x != g_board.from_x || y != g_board.from_y)
    {
        path.push_back(Point(x, y));

        switch(g_board.board[y*g_board.width+x])
        {
        case CELL_LEFT:
            --x;
            break;

        case CELL_RIGHT:
            ++x;
            break;

        case CELL_UP:
            ++y;
            break;

        case CELL_DOWN:
            --y;
            break;

        // should not come here
        default:
            x = g_board.from_x;
            y = g_board.from_y;
            break;
        }
    }

    path.push_back(Point(g_board.from_x, g_board.from_y));
    reverse(path.begin(), path.end());
}

bool Arithmetic::calcPath(Bead** board,int board_width,int board_height,Point& from,Point& target,vector<Point>& path)
{
    bool ret;

    if(!board_init(board_width, board_height))
        return false;

    ret = board_searchinit(board, from, target);

    if(ret)
        ret = board_search();

    if(ret)
        board_search_result(path);

    board_searchdeinit();
    board_deinit();

    return ret;
}
bool Arithmetic::rightSlantingScan(Bead** board,int board_width,int board_height,Point& target,vector<Point>& path)
{
    bool ret;

    if(!board_init(board_width, board_height))
        return false;

    ret = board_blankinit(board, target);

    if(ret)
       ret = board_slashblank(path);

    board_blankdeinit();
    board_deinit();

    return ret;
}
bool Arithmetic::leftSlantingScan(Bead** board,int board_width,int board_height,Point& target,vector<Point>& path)
{
    bool ret;

    if(!board_init(board_width, board_height))
        return false;

    ret = board_blankinit(board, target);

    if(ret)
        ret=board_backslashblank(path);

    board_blankdeinit();
    board_deinit();

    return ret;
}

bool Arithmetic::verticalScan(Bead** board,int board_width,int board_height,Point& target,vector<Point>& path)
{
    bool ret;

    if(!board_init(board_width, board_height))
        return false;

    ret = board_blankinit(board, target);

    if(ret)
        ret=board_verticalblank(path);

    board_blankdeinit();
    board_deinit();

    return ret;
}
bool Arithmetic::horizontalScan(Bead** board,int board_width,int board_height,Point& target,vector<Point>& path)
{
    bool ret;

    if(!board_init(board_width, board_height))
        return false;

    ret = board_blankinit(board, target);

    if(ret)
        ret=board_horizontalblank(path);

    board_blankdeinit();
    board_deinit();

    return ret;
}

int Arithmetic::calcScore(int h_num, int v_num, int left_slant_num, int right_slant_num)
{
    int arr[4];
    arr[0] = h_num;
    arr[1] = v_num;
    arr[2] = left_slant_num;
    arr[3] = right_slant_num;
    int max_idx = 0;
    for(int i=1;i<4;i++)
        if(arr[i]>arr[max_idx])
            max_idx = i;
    arr[max_idx]++;
    int score =0;
    int line =0;
    for(int i=0;i<4;i++)
    {
        if(arr[i]>=4)
        {
            score+=calcScore(arr[i]);
            line++;
        }
    }
    score+=((line-1)*50);
    return score;
}

int Arithmetic::calcScore(int num)
{
    int score = num * 2;
    if(num > 6)
        score+=(num-6)*10;
    return score;
}

