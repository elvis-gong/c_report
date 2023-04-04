#include <iostream>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define row 9
#define col 9
#define Graphsize 45
IMAGE graph[12]; //我们需要的一些图片素材
int num = 0;

void show(int a[row + 2][col + 2]);
void set_mine(int a[row + 2][col + 2], int mine);
void assignment(int a[row + 2][col + 2]);
void loadgraph();
void putmap(int a[row + 2][col + 2]);
void openblank(int r, int c, int a[row + 2][col + 2]);
int mouse_action(int a[row + 2][col + 2]);
void initgame(int a[row + 2][col + 2], int mine, int &close);
void judge(int mine, int a[row + 2][col + 2], int &close);

int main()
{
    initgraph(row * Graphsize, col * Graphsize, SHOWCONSOLE);
    int menu[row + 2][col + 2] = {0}; //初始化游戏,这里行、列都加2是为了腾出数组空间
    int mine = 10;
    set_mine(menu, mine);
    show(menu);
    assignment(menu);
    loadgraph();
    putmap(menu);
    int close = 0;
    while (close == 0)
    {
        mouse_action(menu);
        judge(mine, menu, close);
    }

    closegraph();
    return 0;
}

void show(int a[row + 2][col + 2])
{
    for (int i = 1; i < row + 1; i++)
    {
        for (int j = 1; j < col + 1; j++)
            cout << a[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

void set_mine(int a[row + 2][col + 2], int mine) //埋地雷，令其值为-1
{
    srand((unsigned)time(NULL)); //随机种子，随机埋雷
    for (int i = 0; i < mine;)
    {
        int r = 1 + rand() % (row); //注意避开我们不用的数组边界,范围一定看仔细，错了好几次没看出来
        int c = 1 + rand() % (col);
        if (a[r][c] == 0)
        {
            a[r][c] = -1;
            i++; // i++放在这里，是为了防止两次埋雷埋在了同一个地方
        }
    }
}

void assignment(int a[row + 2][col + 2]) //对非地雷格进行赋值操作
{
    for (int i = 1; i < row + 1; i++)
    {
        for (int j = 1; j < col + 1; j++)
        {
            if (a[i][j] == -1) //越过地雷
                continue;
            else
            {
                for (int m = i - 1; m <= i + 1; m++)
                {
                    for (int n = j - 1; n <= j + 1; n++)
                    {
                        if (a[m][n] == -1)
                            a[i][j]++;
                    }
                }
            }
        }
    }

    for (int i = 1; i <= row; i++) //设置掩码，帮助我们界面的初始化
    {
        for (int j = 1; j <= col; j++)
        {
            a[i][j] += 10;
        }
    }
}

void loadgraph() //载入图片
{
    for (int i = 0; i < 12; i++)
    {
        string pictures = "./images/" + to_string(i) + ".jpg";
        loadimage(&graph[i], pictures.c_str(), Graphsize, Graphsize);
    }
}

void putmap(int a[row + 2][col + 2]) //根据代数值放置图片
{
    for (int i = 1; i <= row; i++)
    {
        for (int j = 1; j <= col; j++)
        {
            if (a[i][j] >= 0 && a[i][j] <= 8)
                putimage((j - 1) * Graphsize, (i - 1) * Graphsize, &graph[a[i][j]]);
            else if (a[i][j] == -1)
                putimage((j - 1) * Graphsize, (i - 1) * Graphsize, &graph[10]);
            else if (a[i][j] >= 9 && a[i][j] <= 18)
                putimage((j - 1) * Graphsize, (i - 1) * Graphsize, &graph[11]);
            else if (a[i][j] > 20)
                putimage((j - 1) * Graphsize, (i - 1) * Graphsize, &graph[9]); //标旗，如果标旗我们会将该格的代数加上20（只有没有点开的格子可以插旗）
        }
    }
}

void openblank(int r, int c, int a[row + 2][col + 2]) //晕开操作，打开空格周围为被标记且不是地雷的格子，减少需要点击的次数
{
    if (a[r][c] == 0)
    {
        for (int m = r - 1; m <= r + 1; m++)
        {
            for (int n = c - 1; n <= c + 1; n++)
            {
                if (a[m][n] != 9 && a[m][n] > 8)
                {
                    a[m][n] -= 10;
                    num++;
                    openblank(m, n, a); //递归打开(空格周围的空格)周围的格子
                }
            }
        }
    }
}

int mouse_action(int a[row + 2][col + 2]) //捕捉鼠标的行动
{
    if (MouseHit()) //鼠标有动作后才可以执行命令
    {
        MOUSEMSG msg = GetMouseMsg(); //获取当前鼠标信息，找到对应数组下标
        int r = msg.y / Graphsize + 1;
        int c = msg.x / Graphsize + 1;
        switch (msg.uMsg)
        {
        case WM_LBUTTONDOWN:                 //左击
            if (a[r][c] > 8 && a[r][c] < 19) //设定条件，只能打开关闭了的格子
            {
                a[r][c] -= 10;
                num++;
                openblank(r, c, a);
            }
            break;

        case WM_RBUTTONDOWN:                 //右击
            if (a[r][c] > 8 && a[r][c] < 19) //同上，标记关闭的格子
            {
                a[r][c] += 20;
            }
            else if (a[r][c] > 20) //再次右击已经插旗的格子（代数大于20的），会将该格对应代数降低20，取消标记
            {
                a[r][c] -= 20;
            }
            break;
        }
        return a[r][c]; //如果返回的值为-1，说明我们踩到了地雷，游戏失败
    }
    else
        return 0;
}

void judge(int mine, int a[row + 2][col + 2], int &close)
{
    putmap(a);
    if (num == row * col - mine) //挑战成功
    {
        putmap(a);
        int ok = MessageBox(GetHWnd(), _T("Congratulations on your success in the challenge! Do you want to try again?"), _T("Good job"), MB_OKCANCEL);
        if (ok == IDOK)
        {
            initgame(a, mine, close);
        }
        else
        {
            close = 1;
        }
    }

    if (mouse_action(a) == -1) //游戏失败
    {
        for (int i = 1; i < row + 1; i++) //显示所有地雷
        {
            for (int j = 1; j < col + 1; j++)
            {
                if (a[i][j] == 9)
                    a[i][j] -= 10;
                else if (a[i][j] == 29)
                    a[i][j] -= 30;
            }
        }
        putmap(a);
        int ok = MessageBox(GetHWnd(), _T("Wu ~ Wu ~ Wu, I'm sorry that you were bombed! Do you want to try again?"), _T("BOOM ~~"), MB_OKCANCEL);
        if (ok == IDOK)
        {
            initgame(a, mine, close);
        }
        else
        {
            close = 1;
        }
    }
}

void initgame(int a[row + 2][col + 2], int mine, int &close) //初始化游戏
{
    memset(a, 0, (row + 2) * (col + 2) * sizeof(int));
    num = 0;
    set_mine(a, mine);
    show(a);
    assignment(a);
    putmap(a);
    while (close == 0)
    {
        mouse_action(a);
        judge(mine, a, close);
    }
}