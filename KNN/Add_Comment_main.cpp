#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <sys/types.h>
#include <dirent.h>
#include <direct.h>
#include <math.h>

// 初始化绘图窗口大小
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;

// 定义画笔大小
const int PEN_SIZE = 15;

#define MAX_DIGIT 1024
#define MAX_K 10
#define PATH_LENTH 256

// 定义训练集路径
char path[] = "E:/KNN-HandWritting-Project/KNN/trainingDigits/";

// 定义数据结构
typedef struct data
{
    int label;
    int feature[MAX_DIGIT];
    struct data *next = NULL;
} DATA;

/*
   定义一个用于比较两个数据结构中label值大小的比较函数
   参数a和b是指向DATA类型的指针，表示要比较的两个数据结构
   返回值为整型，表示a->label与b->label的差值
*/

int cmp(const void* a, const void* b)
{
    // 将指针a强制转换为DATA类型指针
    DATA* da = (DATA*)a;  
    // 将指针b强制转换为DATA类型指针       
    DATA* db = (DATA*)b;     
    // 返回两个数据结构中label值的差    
    return da->label - db->label; 
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
    int sum = 0; // 存储像素颜色转换后的累加值
    int color; // 存储获取的像素颜色值

    // 遍历小区域像素并进行颜色转换
    for (int i = 0; i < 20; i++) // 遍历行
    {
        for (int j = 0; j < 20; j++) // 遍历列
        {
            color = getpixel(x + i, y + j); // 获取坐标 (x+i, y+j) 处的像素颜色值
            if (color == 0xffffff) // 如果像素为白色
            {
                sum += 0; // 累加值加 0
            }
            else if (color == 0x000000) // 如果像素为黑色
            {
                sum += 1; // 累加值加 1
            }
        }
    }

    double avg = (double)sum / 100; // 计算颜色比例的平均值

    // 根据平均值判断并返回像素值
    if (avg > 0.4)
    {
        return 1; // 平均值大于 0.4 则返回 1
    }
    else
    {
        return 0; // 否则返回 0
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
    int Tarry[32][32]; // 存储转换后的像素数据的二维数组
    int i = 0;

    // 将图片转换为文本像素矩阵
    for (int i = 0; i < 32; i++) // 遍历行
    {
        for (int j = 0; j < 32; j++) // 遍历列
        {
            Tarry[i][j] = Transform_Pixel(i * 20, j * 20); // 调用函数转换像素并存储在数组中
        }
    }

    FILE* fp = fopen("target.txt", "w"); // 打开文件用于写入
    if (fp == NULL)
    {
        perror("Can not open target.txt file"); // 打开文件失败时输出错误信息
        return;
    }

    // 将像素矩阵写入文件
    for (int i = 0; i < 32; i++) // 遍历行
    {
        for (int j = 0; j < 32; j++) // 遍历列
        {
            fprintf(fp, "%d", Tarry[j][i]); // 将像素值写入文件
        }
        fprintf(fp, "\n"); // 每行结束后换行
    }

    fclose(fp); // 关闭文件
}


/*
函数：Get_FileLable
参数：-filename[]
功能：从文件名中获取标签值
返回值:int lable
*/

int Get_FileLable(char* filename)
{
    char tamp = filename[0]; // 从文件名中取第一个字符
    int label = tamp - '0'; // 将字符转换为整数标签（假设文件名第一个字符为数字字符）
    return label; // 返回文件名对应的标签
}


/*
函数：txt2vector
参数：-filename
功能：把txt文件向量化
返回：-DATA
*/

void txt2vector(struct dirent *ptr, DATA *pdata)
{
    char filename[PATH_LENTH]; // 存储文件路径的字符数组
    memset(filename, 0, sizeof(char)); // 初始化文件名数组为空

    // 拼接文件路径
    strcat(filename, path); // 将全局变量 path（存储目录路径）连接到 filename 中
    strcat(filename, ptr->d_name); // 将文件名 ptr->d_name（目标文件名）连接到 filename 中

    FILE *fp; // 文件指针
    fp = fopen(filename, "r"); // 打开文件
    if (fp == NULL)
    {
        perror("Can not open target file"); // 如果文件打开失败，打印错误信息
    }

    int ch; // 存储读取的字符
    int cur = 0; // 特征数组的索引

    ch = fgetc(fp); // 读取文件中的一个字符
    while (ch != EOF) // 循环直到文件结束
    {
        if (ch != '\n') // 如果字符不是换行符
        {
            pdata->feature[cur] = ch - '0'; // 将字符转换为数字并存储在 pdata 的特征数组中
            cur++; // 移动到下一个特征
        }
        ch = fgetc(fp); // 继续读取文件中的下一个字符
    }

    fclose(fp); // 关闭文件
}


/*
函数：Train_DataSet
参数：无
功能：训练数据并且返回
返回值：
*/

DATA *Train_DataSet(DATA *phead)
{
    printf("Start training\n"); // 输出训练开始信息
    DATA *pcur = phead; // 当前训练数据指针，初始指向训练数据集的头部
    DATA *pdata = NULL; // 用于存储每个读取的数据项
    DIR *dir = opendir("E:\\KNN-HandWritting-Project\\KNN\\trainingDigits"); // 打开训练数据所在的目录
    struct dirent *ptr; // dirent 结构体指针，用于遍历目录中的文件项
    ptr = readdir(dir); // 读取目录中的第一个文件项
    int count = 1; // 计数器，用于跟踪读取的文件数量

    while (ptr->d_name[0] == '.') // 跳过当前目录和父目录的文件项
    {
        ptr = readdir(dir);
    }

    while (ptr != NULL) // 遍历目录中的每一个文件项
    {
        pdata = (DATA *)malloc(sizeof(DATA)); // 分配内存存储数据
        pdata->label = Get_FileLable(ptr->d_name); // 获取文件名对应的标签
        txt2vector(ptr, pdata); // 将文件内容转换为特征向量存储在 pdata 中

        if (phead == NULL) // 如果训练数据集为空
        {
            phead = pdata; // 将当前数据项作为训练数据集的头部
            pdata->next = NULL; // 下一节点为空
            pcur = phead; // 更新当前数据指针为头部
        }
        else // 如果训练数据集不为空
        {
            pcur->next = pdata; // 将当前数据项连接到训练数据集的尾部
            pcur = pdata; // 更新当前数据指针为新加入的数据项
            pcur->next = NULL; // 下一节点为空
        }

        ptr = readdir(dir); // 继续读取下一个文件项
    }

    closedir(dir); // 关闭目录
    printf("Training success\n"); // 输出训练成功信息
    return phead; // 返回训练完成后的训练数据集头部指针
}


/*
函数：Len_DataSet
参数：DATA *phead
功能：获得训练集的长度
返回值：int lenth
*/
int Len_DataSet(DATA* phead)
{
    DATA* cur = phead; // 当前指针指向数据集的头部
    int count = 0;     // 计数器，用于统计数据集中的元素个数

    while (cur != NULL) // 遍历数据集，直到当前指针为空（即到达数据集末尾）
    {
        count++;       // 每遍历一个元素，计数器加1
        cur = cur->next; // 移动到下一个元素
    }

    return count; // 返回数据集中元素的个数
}


/*
函数：Euclidean_Distance
参数：
功能：计算识别数据和测试数据之间的欧式距离
返回值：distance
*/

float Eucliden_Distance(DATA *pdata, DATA *ptrain)
{
    int i = 0;
    float distance = -1; // 初始化距离为-1

    while (i < MAX_DIGIT) // 遍历特征数组的每个元素
    {
        distance += pow(pdata->feature[i] - ptrain->feature[i], 2); // 计算特征之间的差的平方并累加
        i++; // 移动到下一个特征
    }

    distance = sqrt(distance); // 对累加的平方和进行平方根计算，得到欧几里得距离

    return distance; // 返回计算得到的距离
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

int KNN(DATA* pTrain, int k)
{
    // 先把目标数据向量化
    DATA* pData = (DATA*)malloc(sizeof(DATA)); // 为目标数据分配内存
    pData->label = -1;                         // 初始化目标数据标签为-1
    pData->next = NULL;                        // 初始化下一节点指针为空
    FILE* fp = fopen("E:/KNN-HandWritting-Project/KNN/target.txt", "r"); // 打开目标文件
    if (fp == NULL) // 如果文件打开失败
    {
        perror("Can not open target.txt file"); // 打印错误信息
        exit(0); // 退出程序
    }
    int ch;
    int cur = 0;
    ch = fgetc(fp); // 从文件中读取一个字符
    while (ch != EOF) // 直到文件末尾
    {
        if (ch != '\n') // 如果不是换行符
        {
            pData->feature[cur] = ch - '0'; // 将字符转换为数字并存储在特征数组中
            cur++;
        }
        ch = fgetc(fp); // 继续读取下一个字符
    }
    fclose(fp); // 关闭文件

    int DataSetLenth = Len_DataSet(pTrain); // 获取训练数据集的长度
    float All_distance[2000]; // 存储所有距离
    memset(All_distance, -1, sizeof(All_distance)); // 初始化所有距离为-1
    int all_lable[2000]; // 存储所有标签
    memset(all_lable, -1, sizeof(all_lable)); // 初始化所有标签为-1

    cur = 0;
    DATA* pcur = pTrain;
    while (cur < DataSetLenth && pcur != NULL) // 遍历训练数据集
    {
        All_distance[cur] = Eucliden_Distance(pData, pcur); // 计算目标数据与当前训练数据的欧几里得距离
        all_lable[cur] = pcur->label; // 存储当前训练数据的标签
        pcur = pcur->next; // 移动到下一个训练数据
        cur++;
        //printf("prograssing...... %d/%d\n", cur,DataSetLenth); // 打印进度
    }

    float minDistance[3] = {1024, 1024, 1024};  // 最小距离数组初始化，假设 k=3
    int minlable[3] = { -1, -1, -1 }; // 最小标签数组初始化
    cur = 0;
    while (cur < DataSetLenth) // 遍历所有距离
    {
        for (int i = 0; i < 3; i++) // 找到前三个最小距离
        {
            if (All_distance[cur] < minDistance[i])
            {
                minDistance[i] = All_distance[cur];
                minlable[i] = all_lable[cur];
                break;
            }
        }
        cur++;
    }
    free(pData); // 释放目标数据内存
    printf("The 3 nearest neighbour:\n");
    for (int i = 0; i < 3; i++) // 打印前三个最近邻居的距离和标签
    {
        printf("Distance:%.4f Lable:%d\n", minDistance[i], minlable[i]);
    }
    return minlable[0]; // 返回第一个最近邻居的标签
}


/*
函数：Draw
功能：打开绘图版，绘制数字
返回值：无
*/

void Draw()
{
    bool drawing = false;  // 标记是否正在绘图
    int x = 0, y = 0;      // 记录鼠标当前位置

    while (true)
    {
        // 获取鼠标状态
        MOUSEMSG msg = GetMouseMsg();

        // 根据鼠标消息类型进行处理
        switch (msg.uMsg)
        {
        case WM_LBUTTONDOWN: // 当左键按下时
            drawing = true;  // 开始绘图
            x = msg.x;       // 记录当前鼠标位置
            y = msg.y;
            break;

        case WM_MOUSEMOVE: // 当鼠标移动时
            if (drawing)   // 如果正在绘图
            {
                // 绘制小圆点
                for (int i = -PEN_SIZE; i <= PEN_SIZE; ++i)
                {
                    for (int j = -PEN_SIZE; j <= PEN_SIZE; ++j)
                    {
                        putpixel(x + i, y + j, BLACK);  // 在当前位置绘制黑色像素
                    }
                }
                x = msg.x;  // 更新鼠标位置
                y = msg.y;
            }
            break;

        case WM_LBUTTONUP: // 当左键松开时
            drawing = false; // 停止绘图
            break;

        case WM_RBUTTONDOWN: // 当右键按下时
            goto Enddraw;    // 跳出循环，结束绘图
        }
    }

Enddraw: // 绘图结束
    BeginBatchDraw();  // 开始批量绘图
    EndBatchDraw();    // 结束批量绘图
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
    
    DATA *pTrain = NULL;
    ExMessage mskey;
    
    pTrain=Train_DataSet(pTrain);

    while (true)
    {
        // 检查是否有键盘输入
        mskey = getmessage(EX_KEY);
        switch (mskey.vkcode)
        {
        case 0x1B: 
        // 按下ESC键退出程序
            printf("Program exited.");
            exit(0);
            break;
        case 0x43: 
        // 按下C键清空画板
            cleardevice();
            break;
        case 0x53: 
        //按下S键开始画图
            Draw();
            break;
        case 0x52:
         // 按下R键识别数字
            FlushBatchDraw();
            Turn_Picture_to_txt();
            int classfiyresult = KNN(pTrain,3);
            printf("The Digitis is %d\n", classfiyresult);
            break;
        }
        flushmessage(EX_KEY);
        Sleep(30);
    }

    // 关闭图形窗口
    closegraph();
    return 0;
}
