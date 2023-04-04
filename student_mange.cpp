#include <iostream>
#include <vector>
#include <string>
#include <graphics.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <conio.h>
using namespace std;

class student
{
public:
    student();
    student(const std::string &name, const std::string &gender, int Class, int StuNum, int math, int English, int program);
    string convert();    // 将int型变量转化为字符数组并写入文件
    void display(int m); // 展示学生信息
    string name, gender;
    int Class, StuNum, math, English, program;
};

class administrator : public student // 公有继承，可以访问student内的公有成员
{
public:
    administrator();
    void show();   // 展示全部学生信息
    void add();    // 添加学生信息
    void Erase();  // 删除学生信息
    void modify(); // 修改学生信息
    void find();   // 查找学生信息
    void Sort();   // 学生成绩排序
    void readData(const string &fileName, char a[]);
    void writeData(const string &fileName);
    vector<student> Stu; // 定义一个student类型的动态数组,大小不受限制
    string TableHeader;
    string TableBody;
};

char buf[1024] = {0};
char a[1024] = {0};
string TableBody = " ";
string aimname;
student stu; // 创建一个学生对象

student::student() : Class(0), StuNum(0), math(0), English(0), program(0)
{
}

student::student(const std::string &name, const std::string &gender, int Class, int StuNum, int math, int English, int program)
    : name(name), gender(gender), Class(Class), StuNum(StuNum), math(math), English(English), program(program)
{
}

string student::convert() // 将int型变量转化为字符数组并写入文件
{
    stringstream ss;
    ss << this->name << '\t' << this->gender << '\t' << this->Class << '\t' << this->StuNum << '\t' << this->math << '\t' << this->English << '\t' << this->program << endl;
    return ss.str();
}
int y = 30;

void student::display(int m)
{
    stringstream ss;
    ss << Class << "    " << StuNum << setw(6) << math << setw(7) << English << setw(8) << program << setw(10) << math + English + program;
    string TableBody = ss.str(); // 将表体数字转化为字符串
    outtextxy(5, y, name.c_str());
    outtextxy(105, y, gender.c_str()); // 输出表
    outtextxy(190, y, TableBody.c_str());
    y += 30;
    if (y == (m + 2) * 30)
    { // m是目前表中学生数(Stu.size()),该循环使每次输出表后y值重归于30,下次还在原位置输出表
        y = 30;
    }
}

// 写入文件数据函数
void administrator::writeData(const string &fileName)
{
    fstream write(fileName.c_str(), ios::out);
    if (!write.is_open())
    {
        cerr << " file open failed" << endl; // 异常处理
        return;
    }
    TableHeader += '\n';
    write.write(TableHeader.c_str(), TableHeader.size()); // 写入文件
    for (size_t i = 0; i < Stu.size(); i++)
    {
        string inf = Stu[i].convert();
        write.write(inf.c_str(), inf.size());
    }
    write.close();
}

// 读取文件数据函数
void administrator::readData(const string &fileName, char a[])
{
    fstream read(fileName.c_str(), ios::in); // 创建输入流对象
    if (!read.is_open())
    {
        cerr << " file open failed" << endl; // 异常处理
        return;
    }
    // 读取表头
    read.getline(buf, 1024); // 读汉字
    TableHeader = buf;
    // 读取数据
    while (!read.eof())
    {
        memset(a, 0, sizeof(a));
        read.getline(a, 1024);
        if (strlen(a) == 0)
            break;
        stringstream ss(a); // 数字变为字符数组，包含在头文件#include<sstream>中
        ss >> stu.name >> stu.gender >> stu.Class >> stu.StuNum >> stu.math >> stu.English >> stu.program;
        TableBody = ss.str();
        Stu.push_back(stu); // 将信息存入动态数组
    }
    read.close(); // 关闭
}

administrator::administrator()
{
    readData("./Grade.txt", a);
}

// 总览全部学生信息函数
void administrator::show()
{
    loadimage(NULL, "background2.jpg", 700, 700);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("楷体"), 0, 0, 5, false, false, false);
    outtextxy(5, 10, buf);
    for (auto &i : Stu)
    {
        i.display(Stu.size() - 1); // 传入数组中学生个数-1,因为display()中为m+2,此处传入Stu.size()-1
    }
}

// 添加学生信息函数
void administrator::add()
{
    char stud[100];
    InputBox(stud, 100, "请依次输入姓名,性别,班级,学号,数学成绩,英语成绩,程序设计成绩：");
    fstream output("Grade.txt", ios::out | ios::ate | ios::app); // 接着文件尾写入
    output.write(stud, 30);
    output << '\n';
    output.close();
}

// 查找学生信息函数
void administrator::find()
{
    char aim[30];
    InputBox(aim, 7, "请输入要查找学生的姓名：");
    loadimage(NULL, "background2.jpg", 700, 700);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("楷体"), 0, 0, 5, false, false, false);
    int ap = 1; // 标记量
    for (size_t j = 0; j < Stu.size(); j++)
    {
        if (Stu[j].name == aim)
        {
            outtextxy(5, 10, buf); // 查找成功后将该生信息展示
            Stu[j].display(0);
            ap = 2;
            break;
        }
        else if (j == Stu.size() - 1 && ap == 1)
        {
            outtextxy(250, 300, "未查找到此人信息");
            break;
        }
    }
}

// 删除学生信息函数

void administrator::Erase()
{
    int po = 1;
    char dis[30];
    InputBox(dis, 7, "请输入要删除学生的姓名");
    loadimage(NULL, "background2.jpg", 700, 700);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("楷体"), 0, 0, 5, false, false, false);
    for (size_t k = 0; k < Stu.size(); k++)
    {

        if (Stu[k].name == dis)
        {
            po = 2;
            Stu.erase(Stu.begin() + k, Stu.begin() + k + 1); // 查找成功，从vector中删除该值
            show();
            writeData("Grade.txt");
            break;
        }
        else if (k == Stu.size() - 1 && po == 1)
        {
            outtextxy(100, 200, "未找到此人信息");
        }
    }
}

// 修改学生信息函数
void administrator::modify()
{
    int flag = 1;
    char old[30];
    char New[50];
    InputBox(old, 7, "请输入要修改学生的姓名：");
    loadimage(NULL, "background2.jpg", 700, 700);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("楷体"), 0, 0, 5, false, false, false);
    setfillcolor(LIGHTGRAY);
    for (size_t i = 0; i < Stu.size(); i++)
    {
        if (Stu[i].name == old)
        {
            outtextxy(5, 10, buf);
            Stu[i].display(0);
            fillrectangle(0, 60, 60, 90);
            fillrectangle(80, 60, 140, 90);
            fillrectangle(160, 60, 220, 90);
            fillrectangle(260, 60, 320, 90);
            fillrectangle(360, 60, 410, 90);
            fillrectangle(430, 60, 480, 90);
            fillrectangle(520, 60, 570, 90);
            fillrectangle(200, 300, 500, 350);
            outtextxy(10, 65, "修改");
            outtextxy(90, 65, "修改");
            outtextxy(170, 65, "修改");
            outtextxy(270, 65, "修改");
            outtextxy(365, 65, "修改");
            outtextxy(435, 65, "修改");
            outtextxy(525, 65, "修改");
            outtextxy(255, 315, "按两次Esc键返回封面");
            ExMessage Ms;
            int op = 1;
            do
            {
                flag = 2;
                Ms = getmessage(EM_MOUSE | EM_KEY);
                switch (Ms.message)
                {
                case WM_LBUTTONDOWN:
                    if (Ms.x > 0 && Ms.x < 60 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "请输入修改后的姓名：");
                        Stu[i].name = New;
                        break;
                    }
                    if (Ms.x > 80 && Ms.x < 140 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "请输入修改后的性别：");
                        Stu[i].gender = New;
                        break;
                    }
                    if (Ms.x > 160 && Ms.x < 220 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "请输入修改后的班级：");
                        Stu[i].Class = atoi(New);
                        break;
                    }
                    if (Ms.x > 260 && Ms.x < 320 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "请输入修改后的学号：");
                        Stu[i].StuNum = atoi(New); // atoi()定义在#include<cstdlib>文件内,可将字符数组转化为int
                        break;
                    }
                    if (Ms.x > 360 && Ms.x < 410 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "请输入修改后的数学成绩：");
                        Stu[i].math = atoi(New);
                        break;
                    }
                    if (Ms.x > 430 && Ms.x < 480 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "请输入修改后的英语成绩：");
                        Stu[i].English = atoi(New);
                        break;
                    }
                    if (Ms.x > 520 && Ms.x < 570 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "请输入修改后的程序设计成绩：");
                        Stu[i].program = atoi(New);
                        break;
                    }
                case WM_KEYDOWN:
                    if (Ms.vkcode = VK_ESCAPE) // 按Esc键返回封面
                    {
                        op = 0;
                        break;
                    }
                }
                if (op == 0)
                    break; // 跳出while循环

            } while (1);
        }
        if (i == (Stu.size() - 1) && flag == 1)
        {
            outtextxy(250, 300, "未查找到该学生信息");
            fillrectangle(200, 400, 500, 450);
            outtextxy(255, 415, "按两次Esc键返回初始界面");
            break;
        }
    }
    writeData("Grade.txt"); //
}

bool cmp(student a, student b) // 定义排序方式
{
    return a.math + a.English + a.program > b.math + b.English + b.program;
}

// 成绩排序函数
void administrator::Sort()
{
    sort(Stu.begin(), Stu.end(), cmp); // sort()函数，排序成绩
    show();
}

void init()
{
    loadimage(NULL, "./background1.jpg", 700, 700); // 加载封面图片
    setbkmode(TRANSPARENT);
    settextcolor(BLACK); // 设置字体颜色
    settextstyle(60, 0, _T("楷体"), 0, 0, 5, false, false, false);
    outtextxy(110, 80, "学生成绩管理系统");
    setfillcolor(LIGHTCYAN);
    fillrectangle(250, 180, 440, 230);
    fillrectangle(250, 230, 440, 280);
    fillrectangle(250, 280, 440, 330);
    fillrectangle(250, 330, 440, 380); // 设计七个按钮
    fillrectangle(250, 380, 440, 430);
    fillrectangle(250, 430, 440, 480);
    fillrectangle(250, 480, 440, 530);
    settextstyle(30, 0, _T("楷体"), 0, 0, 5, false, false, false);
    outtextxy(285, 190, "查看全部");
    outtextxy(285, 240, "添加学生");
    outtextxy(285, 290, "删除学生");
    outtextxy(285, 340, "查找学生");
    outtextxy(285, 390, "修改信息");
    outtextxy(285, 440, "成绩排名");
    outtextxy(285, 490, "退出程序");
    settextstyle(20, 0, _T("楷体"), 0, 0, 50, false, false, false);
}

void Button()
{
    ExMessage msg;
    administrator m;                     // 定义一个Management对象
    msg = getmessage(EM_MOUSE | EM_KEY); // 鼠标交互和键盘交互
    switch (msg.message)
    {
    case WM_LBUTTONDOWN:
        if (msg.x > 250 && msg.x < 440 && msg.y > 180 && msg.y < 230)
        {
            m.show();
            break;
        }
        if (msg.x > 250 && msg.x < 440 && msg.y > 230 && msg.y < 280)
        {
            m.add();
            break;
        }
        if (msg.x > 250 && msg.x < 440 && msg.y > 280 && msg.y < 330)
        {
            m.Erase();
            break;
        }
        if (msg.x > 250 && msg.x < 440 && msg.y > 330 && msg.y < 380)
        {
            m.find();
            break;
        }
        if (msg.x > 250 && msg.x < 440 && msg.y > 380 && msg.y < 430)
        {
            m.modify();
            break;
        }
        if (msg.x > 250 && msg.x < 440 && msg.y > 430 && msg.y < 480)
        {
            m.Sort();
            break;
        }
        if (msg.x > 250 && msg.x < 440 && msg.y > 480 && msg.y < 530)
        {
            m.writeData("Grade.txt"); // 保存并退出
            exit(0);
            break;
        }
    case WM_KEYDOWN:
        if (msg.vkcode = VK_ESCAPE) // 按Esc键返回封面
        {
            init();
            break;
        }
    }
}

int main()
{
    initgraph(700, 700); // 设置窗口大小
    init();
    while (1)
    {
        Button();
    }
    _getch(); // 阻塞函数，防止窗口闪退
    closegraph();
    return 0;
}