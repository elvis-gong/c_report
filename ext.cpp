#include <iostream>
#include <graphics.h>

using namespace std;

bool main()
{
    SeqStack S;
    ElementType p;
    int str = 1, length1 = 0, int length2 = 0;
    InitStack(&S);
    cin >> p;
    while (p != '&' && str == 1)
    {
        Push(&S, p);
        length1++;
        cin >> p;
    }
    if (p == '&')
        str = 2;
    if (str == 2)
    {
        cin >> p;
        length2++;
        while (p != '@' && S.top != S.base)
        {
            if (p != Pop(&S))
            {
                return 0;
            }
            else
                cin >> p;
            length2++;
        }
    }
    if (length1 == length2) // str2读入与str1相同的字符数仍然没有不同的字符
        return true;
    return false;
}

typedef int Elemtype;

struct Queue //带头结点的循环单链表队列,只有一个尾指针
{
    Elemtype date;
    Queue *next;
};

typedef struct
{
    Queue *rear;
} SeqQueue;

int InitQueue(SeqQueue &p)
{
    p.rear = (Queue *)malloc(sizeof(Queue));
    p.rear->next = p.rear; //尾指针指向自己,空队列自循环
}

void EnQueue(SeqQueue &p, Elemtype e)
{
    Queue *q;
    q = (Queue *)malloc(sizeof(Queue));
    q->date = e;
    q->next = p.rear->next; // q的下一个是队列头，构成循环
    p.rear->next = q;       //入队,插入到表尾之后
    p.rear = q;             //更新表尾指针为q指针
}

void DeQueue(SeqQueue &p, Elemtype &e)
{
    Queue *q;
    q = p.rear->next->next;       // rear.next是头节点,头结点下一个节点才是首数据地址
    p.rear->next->next = q->next; //删除q节点
    e = q->date;

    if (q == p.rear) //队列只有一个元素
        p.rear = p.rear->next;
    free(q);
}

int main()
{
    initgraph(400, 400, SHOWCONSOLE);
    while (1)
    {
        ;
    }
}