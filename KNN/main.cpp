#define _CRT_SECURE_NO_WARNINGS

#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <conio.h>

// 初始化绘图窗口大小
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;

// 定义画笔大小
const int PEN_SIZE = 8;

#define MAX_DIGIT 1024
#define MAX_FEATURE 256
#define MAX_K 10

typedef struct
{
    int label;
    float feature[MAX_FEATURE];
} DATA;

/*
   定义一个用于比较两个数据结构中label值大小的比较函数
   参数a和b是指向DATA类型的指针，表示要比较的两个数据结构
   返回值为整型，表示a->label与b->label的差值
*/
int cmp(const void* a, const void* b)
{
    DATA* da = (DATA*)a;         // 将指针a强制转换为DATA类型指针
    DATA* db = (DATA*)b;         // 将指针b强制转换为DATA类型指针
    return da->label - db->label; // 返回两个数据结构中label值的差
}

/*
函数：Transform_Pixel
参数：
    - pMem：图像内存指针
    - i：行坐标
    - j：列坐标
功能：计算像素块的值
返回值：
    -0：白色
    -1：黑色
*/
int Transform_Pixel(int x, int y)
{
    int sum = 0;
    int color;
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            color = getpixel(x + i, y + j);
            if (color == 0xffffff)
            {
                sum += 0;
            }
            else if (color == 0x000000)
            {
                sum += 1;
            }
        }
    }
    double avg = (double)sum / 100;
    if (avg > 0.4)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
函数：Turn_Picture_to_txt
参数：
    - filename: png文件名
功能：读取绘图区数据处理成txt文件
*/
void Turn_Picture_to_txt()
{
    int Tarry[32][32];
    int i = 0;
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            Tarry[i][j] = Transform_Pixel(i * 20, j * 20);
        }
    }
    FILE* fp = fopen("target.txt", "w");
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            fprintf(fp, "%d", Tarry[j][i]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

/*
函数：read_data
参数：
    - filename: 文件名
    - data: 数据结构指针
    - num: 读取的数据数量指针
功能：读取数据并存储在指定的数据结构中
*/
void read_data(char* filename, DATA* data, int* num)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: cannot open file %s\n", filename);
        exit(1);
    }
    int i, j;
    char label[10];
    float feature[MAX_FEATURE];
    while (fgets(label, 10, fp) != NULL)
    {
        sscanf(label, "%d", &data[*num].label); // 从文件中读取标签并存储在数据结构中
        for (i = 0; i < MAX_FEATURE; i++)
        {
            if (fgets(label, 10, fp) == NULL)
                break;
            sscanf(label, "%f", &feature[i]); // 从文件中读取特征并存储在临时数组中
        }
        for (i = 0; i < MAX_FEATURE; i++)
        {
            data[*num].feature[i] = feature[i]; // 将临时数组中的特征存储在数据结构中
        }
        (*num)++; // 增加数据数量计数器
    }
    fclose(fp); // 关闭文件流
}

/*
   函数：knn
   参数：
        - data：包含待分类数据和标签的数组指针
        - num：数据集中的数据项数量
        - k：选择用于分类的最近邻居数
        - result：存储分类结果的数组指针
   功能：
        根据k最近邻算法对给定的数据集进行分类
*/
void KNN(DATA* data, int num, int k, float* result)
{
    int i, j, m, n;
    float dist, min_dist;
    DATA* train;
    int* label;
    train = (DATA*)malloc(num * sizeof(DATA)); // 分配内存以存储训练数据
    label = (int*)malloc(num * sizeof(int));   // 分配内存以存储标签
    for (i = 0; i < num; i++)
    {
        train[i] = data[i];       // 将数据复制到训练数据中
        label[i] = data[i].label; // 将标签复制到标签数组中
    }
    qsort(train, num, sizeof(DATA), cmp); // 对训练数据进行排序
    for (i = 0; i < num; i++)
    {
        min_dist = 1e10; // 初始化最小距离为一个较大的值
        for (j = 0; j < k; j++)
        {
            dist = 0.0; // 初始化距离为0
            for (m = 0; m < MAX_FEATURE; m++)
            {
                dist += (train[i].feature[m] - data[j].feature[m]) * (train[i].feature[m] - data[j].feature[m]); // 计算欧几里得距离的平方
            }
            if (dist < min_dist)
            {
                min_dist = dist;      // 更新最小距离
                result[i] = label[j]; // 将对应数据的标签作为此数据项的分类结果
            }
        }
    }
    free(train); // 释放训练数据的内存
    free(label); // 释放标签数组的内存
}

void Draw()
{
    bool drawing = false;
    int x = 0, y = 0;
    while (true)
    {
        // 获取鼠标状态
        MOUSEMSG msg = GetMouseMsg();

        switch (msg.uMsg)
        {
        case WM_LBUTTONDOWN:
            drawing = true;
            x = msg.x;
            y = msg.y;
            break;
        case WM_MOUSEMOVE:
            if (drawing)
            {
                // 绘制小圆点
                for (int i = -PEN_SIZE; i <= PEN_SIZE; ++i)
                {
                    for (int j = -PEN_SIZE; j <= PEN_SIZE; ++j)
                    {
                        putpixel(x + i, y + j, BLACK);
                    }
                }
                x = msg.x;
                y = msg.y;
            }
            break;
        case WM_LBUTTONUP:
            drawing = false;
            break;
        case WM_RBUTTONDOWN:
            goto Enddraw;
        }
    }
    Enddraw:BeginBatchDraw();
    EndBatchDraw();
}

int main()
{
    // 初始化图形窗口
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);

    // 设置背景颜色为白色
    setbkcolor(WHITE);
    cleardevice();

    // 设置画笔颜色为黑色
    setlinecolor(BLACK);
    setfillcolor(BLACK);

    ExMessage mskey;
    while (true)
    {
        // 检查是否有键盘输入
        mskey = getmessage(EX_KEY);
        switch (mskey.vkcode)
        {
        case 0x1B: // 按下ESC键退出程序
            printf("Program exited.");
            exit(0);
            break;
        case 0x43: // 按下C键清空画板
            cleardevice();
            break;
        case 0x53: //按下S键开始画图
            Draw();
            break;
        case 0x52: // 按下R键识别数字
            FlushBatchDraw();
            Turn_Picture_to_txt();
            break;
        }
        flushmessage(EX_KEY);
        Sleep(30);
    }

    // 关闭图形窗口
    closegraph();
    return 0;
}
