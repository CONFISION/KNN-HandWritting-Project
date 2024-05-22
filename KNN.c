// KNN实现手写数字识别

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_DIGIT 1000
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
int cmp(const void *a, const void *b)
{
    DATA *da = (DATA *)a;         // 将指针a强制转换为DATA类型指针
    DATA *db = (DATA *)b;         // 将指针b强制转换为DATA类型指针
    return da->label - db->label; // 返回两个数据结构中label值的差
}

/*
函数：read_data
参数：
    - filename: 文件名
    - data: 数据结构指针
    - num: 读取的数据数量指针
功能：读取数据并存储在指定的数据结构中
*/
void read_data(char *filename, DATA *data, int *num)
{
    FILE *fp = fopen(filename, "r");
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
void knn(DATA *data, int num, int k, float *result)
{
    int i, j, m, n;
    float dist, min_dist;
    DATA *train;
    int *label;
    train = (DATA *)malloc(num * sizeof(DATA)); // 分配内存以存储训练数据
    label = (int *)malloc(num * sizeof(int));   // 分配内存以存储标签
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

int main()
{
    DATA data[MAX_DIGIT];
    int num = 0;
    read_data("d:/homework/digits.txt", data, &num);
    int k = 3;
    float result[MAX_DIGIT];
    knn(data, num, k, result);
    int correct = 0;
    for (int i = 0; i < num; i++)
    {
        if (result[i] == data[i].label)
        {
            correct++;
        }
    }
    printf("Accuracy: %.2f%%\n", 100.0 * correct / num);
    return 0;
}