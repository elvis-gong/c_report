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
    string convert();    // ��int�ͱ���ת��Ϊ�ַ����鲢д���ļ�
    void display(int m); // չʾѧ����Ϣ
    string name, gender;
    int Class, StuNum, math, English, program;
};

class administrator : public student // ���м̳У����Է���student�ڵĹ��г�Ա
{
public:
    administrator();
    void show();   // չʾȫ��ѧ����Ϣ
    void add();    // ���ѧ����Ϣ
    void Erase();  // ɾ��ѧ����Ϣ
    void modify(); // �޸�ѧ����Ϣ
    void find();   // ����ѧ����Ϣ
    void Sort();   // ѧ���ɼ�����
    void readData(const string &fileName, char a[]);
    void writeData(const string &fileName);
    vector<student> Stu; // ����һ��student���͵Ķ�̬����,��С��������
    string TableHeader;
    string TableBody;
};

char buf[1024] = {0};
char a[1024] = {0};
string TableBody = " ";
string aimname;
student stu; // ����һ��ѧ������

student::student() : Class(0), StuNum(0), math(0), English(0), program(0)
{
}

student::student(const std::string &name, const std::string &gender, int Class, int StuNum, int math, int English, int program)
    : name(name), gender(gender), Class(Class), StuNum(StuNum), math(math), English(English), program(program)
{
}

string student::convert() // ��int�ͱ���ת��Ϊ�ַ����鲢д���ļ�
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
    string TableBody = ss.str(); // ����������ת��Ϊ�ַ���
    outtextxy(5, y, name.c_str());
    outtextxy(105, y, gender.c_str()); // �����
    outtextxy(190, y, TableBody.c_str());
    y += 30;
    if (y == (m + 2) * 30)
    { // m��Ŀǰ����ѧ����(Stu.size()),��ѭ��ʹÿ��������yֵ�ع���30,�´λ���ԭλ�������
        y = 30;
    }
}

// д���ļ����ݺ���
void administrator::writeData(const string &fileName)
{
    fstream write(fileName.c_str(), ios::out);
    if (!write.is_open())
    {
        cerr << " file open failed" << endl; // �쳣����
        return;
    }
    TableHeader += '\n';
    write.write(TableHeader.c_str(), TableHeader.size()); // д���ļ�
    for (size_t i = 0; i < Stu.size(); i++)
    {
        string inf = Stu[i].convert();
        write.write(inf.c_str(), inf.size());
    }
    write.close();
}

// ��ȡ�ļ����ݺ���
void administrator::readData(const string &fileName, char a[])
{
    fstream read(fileName.c_str(), ios::in); // ��������������
    if (!read.is_open())
    {
        cerr << " file open failed" << endl; // �쳣����
        return;
    }
    // ��ȡ��ͷ
    read.getline(buf, 1024); // ������
    TableHeader = buf;
    // ��ȡ����
    while (!read.eof())
    {
        memset(a, 0, sizeof(a));
        read.getline(a, 1024);
        if (strlen(a) == 0)
            break;
        stringstream ss(a); // ���ֱ�Ϊ�ַ����飬������ͷ�ļ�#include<sstream>��
        ss >> stu.name >> stu.gender >> stu.Class >> stu.StuNum >> stu.math >> stu.English >> stu.program;
        TableBody = ss.str();
        Stu.push_back(stu); // ����Ϣ���붯̬����
    }
    read.close(); // �ر�
}

administrator::administrator()
{
    readData("./Grade.txt", a);
}

// ����ȫ��ѧ����Ϣ����
void administrator::show()
{
    loadimage(NULL, "background2.jpg", 700, 700);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("����"), 0, 0, 5, false, false, false);
    outtextxy(5, 10, buf);
    for (auto &i : Stu)
    {
        i.display(Stu.size() - 1); // ����������ѧ������-1,��Ϊdisplay()��Ϊm+2,�˴�����Stu.size()-1
    }
}

// ���ѧ����Ϣ����
void administrator::add()
{
    char stud[100];
    InputBox(stud, 100, "��������������,�Ա�,�༶,ѧ��,��ѧ�ɼ�,Ӣ��ɼ�,������Ƴɼ���");
    fstream output("Grade.txt", ios::out | ios::ate | ios::app); // �����ļ�βд��
    output.write(stud, 30);
    output << '\n';
    output.close();
}

// ����ѧ����Ϣ����
void administrator::find()
{
    char aim[30];
    InputBox(aim, 7, "������Ҫ����ѧ����������");
    loadimage(NULL, "background2.jpg", 700, 700);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("����"), 0, 0, 5, false, false, false);
    int ap = 1; // �����
    for (size_t j = 0; j < Stu.size(); j++)
    {
        if (Stu[j].name == aim)
        {
            outtextxy(5, 10, buf); // ���ҳɹ��󽫸�����Ϣչʾ
            Stu[j].display(0);
            ap = 2;
            break;
        }
        else if (j == Stu.size() - 1 && ap == 1)
        {
            outtextxy(250, 300, "δ���ҵ�������Ϣ");
            break;
        }
    }
}

// ɾ��ѧ����Ϣ����

void administrator::Erase()
{
    int po = 1;
    char dis[30];
    InputBox(dis, 7, "������Ҫɾ��ѧ��������");
    loadimage(NULL, "background2.jpg", 700, 700);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("����"), 0, 0, 5, false, false, false);
    for (size_t k = 0; k < Stu.size(); k++)
    {

        if (Stu[k].name == dis)
        {
            po = 2;
            Stu.erase(Stu.begin() + k, Stu.begin() + k + 1); // ���ҳɹ�����vector��ɾ����ֵ
            show();
            writeData("Grade.txt");
            break;
        }
        else if (k == Stu.size() - 1 && po == 1)
        {
            outtextxy(100, 200, "δ�ҵ�������Ϣ");
        }
    }
}

// �޸�ѧ����Ϣ����
void administrator::modify()
{
    int flag = 1;
    char old[30];
    char New[50];
    InputBox(old, 7, "������Ҫ�޸�ѧ����������");
    loadimage(NULL, "background2.jpg", 700, 700);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("����"), 0, 0, 5, false, false, false);
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
            outtextxy(10, 65, "�޸�");
            outtextxy(90, 65, "�޸�");
            outtextxy(170, 65, "�޸�");
            outtextxy(270, 65, "�޸�");
            outtextxy(365, 65, "�޸�");
            outtextxy(435, 65, "�޸�");
            outtextxy(525, 65, "�޸�");
            outtextxy(255, 315, "������Esc�����ط���");
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
                        InputBox(New, 10, "�������޸ĺ��������");
                        Stu[i].name = New;
                        break;
                    }
                    if (Ms.x > 80 && Ms.x < 140 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "�������޸ĺ���Ա�");
                        Stu[i].gender = New;
                        break;
                    }
                    if (Ms.x > 160 && Ms.x < 220 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "�������޸ĺ�İ༶��");
                        Stu[i].Class = atoi(New);
                        break;
                    }
                    if (Ms.x > 260 && Ms.x < 320 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "�������޸ĺ��ѧ�ţ�");
                        Stu[i].StuNum = atoi(New); // atoi()������#include<cstdlib>�ļ���,�ɽ��ַ�����ת��Ϊint
                        break;
                    }
                    if (Ms.x > 360 && Ms.x < 410 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "�������޸ĺ����ѧ�ɼ���");
                        Stu[i].math = atoi(New);
                        break;
                    }
                    if (Ms.x > 430 && Ms.x < 480 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "�������޸ĺ��Ӣ��ɼ���");
                        Stu[i].English = atoi(New);
                        break;
                    }
                    if (Ms.x > 520 && Ms.x < 570 && Ms.y > 60 && Ms.y < 90)
                    {
                        InputBox(New, 10, "�������޸ĺ�ĳ�����Ƴɼ���");
                        Stu[i].program = atoi(New);
                        break;
                    }
                case WM_KEYDOWN:
                    if (Ms.vkcode = VK_ESCAPE) // ��Esc�����ط���
                    {
                        op = 0;
                        break;
                    }
                }
                if (op == 0)
                    break; // ����whileѭ��

            } while (1);
        }
        if (i == (Stu.size() - 1) && flag == 1)
        {
            outtextxy(250, 300, "δ���ҵ���ѧ����Ϣ");
            fillrectangle(200, 400, 500, 450);
            outtextxy(255, 415, "������Esc�����س�ʼ����");
            break;
        }
    }
    writeData("Grade.txt"); //
}

bool cmp(student a, student b) // ��������ʽ
{
    return a.math + a.English + a.program > b.math + b.English + b.program;
}

// �ɼ�������
void administrator::Sort()
{
    sort(Stu.begin(), Stu.end(), cmp); // sort()����������ɼ�
    show();
}

void init()
{
    loadimage(NULL, "./background1.jpg", 700, 700); // ���ط���ͼƬ
    setbkmode(TRANSPARENT);
    settextcolor(BLACK); // ����������ɫ
    settextstyle(60, 0, _T("����"), 0, 0, 5, false, false, false);
    outtextxy(110, 80, "ѧ���ɼ�����ϵͳ");
    setfillcolor(LIGHTCYAN);
    fillrectangle(250, 180, 440, 230);
    fillrectangle(250, 230, 440, 280);
    fillrectangle(250, 280, 440, 330);
    fillrectangle(250, 330, 440, 380); // ����߸���ť
    fillrectangle(250, 380, 440, 430);
    fillrectangle(250, 430, 440, 480);
    fillrectangle(250, 480, 440, 530);
    settextstyle(30, 0, _T("����"), 0, 0, 5, false, false, false);
    outtextxy(285, 190, "�鿴ȫ��");
    outtextxy(285, 240, "���ѧ��");
    outtextxy(285, 290, "ɾ��ѧ��");
    outtextxy(285, 340, "����ѧ��");
    outtextxy(285, 390, "�޸���Ϣ");
    outtextxy(285, 440, "�ɼ�����");
    outtextxy(285, 490, "�˳�����");
    settextstyle(20, 0, _T("����"), 0, 0, 50, false, false, false);
}

void Button()
{
    ExMessage msg;
    administrator m;                     // ����һ��Management����
    msg = getmessage(EM_MOUSE | EM_KEY); // ��꽻���ͼ��̽���
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
            m.writeData("Grade.txt"); // ���沢�˳�
            exit(0);
            break;
        }
    case WM_KEYDOWN:
        if (msg.vkcode = VK_ESCAPE) // ��Esc�����ط���
        {
            init();
            break;
        }
    }
}

int main()
{
    initgraph(700, 700); // ���ô��ڴ�С
    init();
    while (1)
    {
        Button();
    }
    _getch(); // ������������ֹ��������
    closegraph();
    return 0;
}