#include <iostream>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define Graphsize 45

class MineClearance
{
public:
    MineClearance();
    void show(const MineClearance &L);
    friend int getrow(const MineClearance &L);
    friend int getcol(const MineClearance &L);
    void set_mine(MineClearance &L);
    void assignment(MineClearance &L);
    void putmap(MineClearance &L);
    void openblank(int r, int c, MineClearance &L);
    int mouse_action(MineClearance &L);
    void initgame(MineClearance &L, int &close);
    void judge(MineClearance &L, int &close);

private:
    static const int row = 9;
    static const int col = 9;
    IMAGE graph[12]; //我们需要的一些图片素材
    int menu[row + 2][col + 2];
    int num;
    int mine;
};

int main()
{
    MineClearance game;
    initgraph(getrow(game) * Graphsize, getcol(game) * Graphsize, SHOWCONSOLE);
    game.set_mine(game);
    game.show(game);
    game.assignment(game);
    game.putmap(game);
    int close = 0;
    while (close == 0)
    {
        game.mouse_action(game);
        game.judge(game, close);
    }

    closegraph();
    return 0;
}

MineClearance ::MineClearance() : num(0), mine(10)
{
    memset(menu, 0, (row + 2) * (col + 2) * sizeof(int));
    menu[row + 2][col + 2] = {0}; // //初始化游戏布局,这里行、列都加2是为了腾出数组空间
    for (int i = 0; i < 12; i++)  //载入图片
    {
        string pictures = "./images/" + to_string(i) + ".jpg";
        loadimage(&graph[i], pictures.c_str(), Graphsize, Graphsize);
    }
}

void MineClearance ::show(const MineClearance &L)
{
    for (int i = 1; i < getrow(L) + 1; i++)
    {
        for (int j = 1; j < getcol(L) + 1; j++)
            cout << L.menu[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

int getrow(const MineClearance &L)
{
    return L.row;
}

int getcol(const MineClearance &L)
{
    return L.col;
}

void MineClearance ::set_mine(MineClearance &L) //埋地雷，令其值为-1
{
    srand((unsigned)time(NULL)); //随机种子，随机埋雷
    for (int i = 0; i < L.mine;)
    {
        int r = 1 + rand() % (row); //注意避开我们不用的数组边界,范围一定看仔细，错了好几次没看出来
        int c = 1 + rand() % (col);
        if (L.menu[r][c] == 0)
        {
            L.menu[r][c] = -1;
            i++; // i++放在这里，是为了防止两次埋雷埋在了同一个地方
        }
    }
}

void MineClearance ::assignment(MineClearance &L) //对非地雷格进行赋值操作
{
    for (int i = 1; i < getrow(L) + 1; i++)
    {
        for (int j = 1; j < getcol(L) + 1; j++)
        {
            if (L.menu[i][j] == -1) //越过地雷
                continue;
            else
            {
                for (int m = i - 1; m <= i + 1; m++)
                {
                    for (int n = j - 1; n <= j + 1; n++)
                    {
                        if (L.menu[m][n] == -1)
                            L.menu[i][j]++;
                    }
                }
            }
        }
    }

    for (int i = 1; i <= getrow(L); i++) //设置掩码，帮助我们界面的初始化
    {
        for (int j = 1; j <= getcol(L); j++)
        {
            L.menu[i][j] += 10;
        }
    }
}

void MineClearance ::putmap(MineClearance &L) //根据代数值放置图片
{
    for (int i = 1; i <= getrow(L); i++)
    {
        for (int j = 1; j <= getcol(L); j++)
        {
            if (L.menu[i][j] >= 0 && L.menu[i][j] <= 8)
                putimage((j - 1) * Graphsize, (i - 1) * Graphsize, &graph[L.menu[i][j]]);
            else if (L.menu[i][j] == -1)
                putimage((j - 1) * Graphsize, (i - 1) * Graphsize, &graph[10]);
            else if (L.menu[i][j] >= 9 && L.menu[i][j] <= 18)
                putimage((j - 1) * Graphsize, (i - 1) * Graphsize, &graph[11]);
            else if (L.menu[i][j] > 20)
                putimage((j - 1) * Graphsize, (i - 1) * Graphsize, &graph[9]); //标旗，如果标旗我们会将该格的代数加上20
        }
    }
}

void MineClearance ::openblank(int r, int c, MineClearance &L) //晕开操作
{
    if (L.menu[r][c] == 0)
    {
        for (int m = r - 1; m <= r + 1; m++)
        {
            for (int n = c - 1; n <= c + 1; n++)
            {
                if (L.menu[m][n] != 9 && L.menu[m][n] > 8)
                {
                    L.menu[m][n] -= 10;
                    num++;
                    L.openblank(m, n, L); //递归打开(空格周围的空格)周围的格子
                }
            }
        }
    }
}

int MineClearance ::mouse_action(MineClearance &L) //捕捉鼠标的行动
{
    if (MouseHit()) //鼠标有动作后才可以执行命令
    {
        MOUSEMSG msg = GetMouseMsg(); //获取当前鼠标信息，找到对应数组下标
        int r = msg.y / Graphsize + 1;
        int c = msg.x / Graphsize + 1;
        switch (msg.uMsg)
        {
        case WM_LBUTTONDOWN:                           //左击
            if (L.menu[r][c] > 8 && L.menu[r][c] < 19) //设定条件，只能打开关闭了的格子
            {
                L.menu[r][c] -= 10;
                num++;
                L.openblank(r, c, L);
            }
            break;

        case WM_RBUTTONDOWN:                           //右击
            if (L.menu[r][c] > 8 && L.menu[r][c] < 19) //同上，标记关闭的格子
            {
                L.menu[r][c] += 20;
            }
            else if (L.menu[r][c] > 20) //再次右击已经插旗的格子（代数大于20的），会将该格对应代数降低20，取消标记
            {
                L.menu[r][c] -= 20;
            }
            break;
        }
        return L.menu[r][c]; //如果返回的值为-1，说明我们踩到了地雷，游戏失败
    }
    else
        return 0;
}

void MineClearance ::judge(MineClearance &L, int &close)
{
    L.putmap(L);
    if (num == getrow(L) * getcol(L) - mine) //挑战成功
    {
        L.putmap(L);
        int ok = MessageBox(GetHWnd(), _T("Congratulations on your success in the challenge! Do you want to try again?"), _T("Good job"), MB_OKCANCEL);
        if (ok == IDOK)
        {
            L.initgame(L, close);
        }
        else
        {
            close = 1;
        }
    }

    if (mouse_action(L) == -1) //游戏失败
    {
        for (int i = 1; i < getrow(L) + 1; i++) //显示所有地雷
        {
            for (int j = 1; j < getcol(L) + 1; j++)
            {
                if (L.menu[i][j] == 9)
                    L.menu[i][j] -= 10;
                else if (L.menu[i][j] == 29)
                    L.menu[i][j] -= 30;
            }
        }
        L.putmap(L);
        int ok = MessageBox(GetHWnd(), _T("Wu ~ Wu ~ Wu, I'm sorry that you were bombed! Do you want to try again?"), _T("BOOM ~~"), MB_OKCANCEL);
        if (ok == IDOK)
        {
            L.initgame(L, close);
        }
        else
        {
            close = 1;
        }
    }
}

void MineClearance ::initgame(MineClearance &L, int &close) //初始化游戏
{
    memset(L.menu, 0, (getrow(L) + 2) * (getcol(L) + 2) * sizeof(int));
    num = 0;
    L.set_mine(L);
    L.show(L);
    L.assignment(L);
    L.putmap(L);
    while (close == 0)
    {
        L.mouse_action(L);
        L.judge(L, close);
    }
}