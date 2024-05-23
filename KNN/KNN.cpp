// KNNʵ����д����ʶ��
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <graphics.h>
#include <conio.h>

#define MAX_DIGIT 1000
#define MAX_FEATURE 256
#define MAX_K 10
#define png_sig_size = 8; // PNG �ļ�ͷ�ĳ���
#define GraphicWigth 480;
#define GraphicHeigth 480;

typedef struct
{
    int label;
    float feature[MAX_FEATURE];
} DATA;

/*
   ����һ�����ڱȽ��������ݽṹ��labelֵ��С�ıȽϺ���
   ����a��b��ָ��DATA���͵�ָ�룬��ʾҪ�Ƚϵ��������ݽṹ
   ����ֵΪ���ͣ���ʾa->label��b->label�Ĳ�ֵ
*/
int cmp(const void *a, const void *b)
{
    DATA *da = (DATA *)a;         // ��ָ��aǿ��ת��ΪDATA����ָ��
    DATA *db = (DATA *)b;         // ��ָ��bǿ��ת��ΪDATA����ָ��
    return da->label - db->label; // �����������ݽṹ��labelֵ�Ĳ�
}

/*
������getpixel
������
    - pMem��ͼ���ڴ�ָ��
    - i��������
    - j��������
���ܣ��������ؿ��ֵ
����ֵ��
    -0����ɫ
    -1����ɫ
*/
int Transform_Pixel(int x, int y)
{
    int sum = 0;
    int color;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
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
    if (avg > 0.7)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
������Turn_Picture_to_txt
������
    - filename: png�ļ���
���ܣ���ȡ��ͼ�����ݴ����txt�ļ�
*/
void Turn_Picture_to_txt()
{
    int Tarry[48][48];
    int i = 0;
    for (int i = 0; i < 48; i++)
    {
        for (int j = 0; j < 48; j++)
        {
            Tarry[i][j] = Transform_Pixel(i * 10, j * 10);
        }
    }
    FILE *fp = fopen("target.txt", "w");
    for (int i = 0; i < 48; i++)
    {
        for (int j = 0; j < 48; j++)
        {
            fprintf(fp, "%d", Tarry[i][j]);
        }
    }
    fclose(fp);
}

/*
������read_data
������
    - filename: �ļ���
    - data: ���ݽṹָ��
    - num: ��ȡ����������ָ��
���ܣ���ȡ���ݲ��洢��ָ�������ݽṹ��
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
        sscanf(label, "%d", &data[*num].label); // ���ļ��ж�ȡ��ǩ���洢�����ݽṹ��
        for (i = 0; i < MAX_FEATURE; i++)
        {
            if (fgets(label, 10, fp) == NULL)
                break;
            sscanf(label, "%f", &feature[i]); // ���ļ��ж�ȡ�������洢����ʱ������
        }
        for (i = 0; i < MAX_FEATURE; i++)
        {
            data[*num].feature[i] = feature[i]; // ����ʱ�����е������洢�����ݽṹ��
        }
        (*num)++; // ������������������
    }
    fclose(fp); // �ر��ļ���
}

/*
   ������knn
   ������
        - data���������������ݺͱ�ǩ������ָ��
        - num�����ݼ��е�����������
        - k��ѡ�����ڷ��������ھ���
        - result���洢������������ָ��
   ���ܣ�
        ����k������㷨�Ը��������ݼ����з���
*/
void KNN(DATA *data, int num, int k, float *result)
{
    int i, j, m, n;
    float dist, min_dist;
    DATA *train;
    int *label;
    train = (DATA *)malloc(num * sizeof(DATA)); // �����ڴ��Դ洢ѵ������
    label = (int *)malloc(num * sizeof(int));   // �����ڴ��Դ洢��ǩ
    for (i = 0; i < num; i++)
    {
        train[i] = data[i];       // �����ݸ��Ƶ�ѵ��������
        label[i] = data[i].label; // ����ǩ���Ƶ���ǩ������
    }
    qsort(train, num, sizeof(DATA), cmp); // ��ѵ�����ݽ�������
    for (i = 0; i < num; i++)
    {
        min_dist = 1e10; // ��ʼ����С����Ϊһ���ϴ��ֵ
        for (j = 0; j < k; j++)
        {
            dist = 0.0; // ��ʼ������Ϊ0
            for (m = 0; m < MAX_FEATURE; m++)
            {
                dist += (train[i].feature[m] - data[j].feature[m]) * (train[i].feature[m] - data[j].feature[m]); // ����ŷ����þ����ƽ��
            }
            if (dist < min_dist)
            {
                min_dist = dist;      // ������С����
                result[i] = label[j]; // ����Ӧ���ݵı�ǩ��Ϊ��������ķ�����
            }
        }
    }
    free(train); // �ͷ�ѵ�����ݵ��ڴ�
    free(label); // �ͷű�ǩ������ڴ�
}

int main()
{
    DATA data[MAX_DIGIT];
    int num = 0;
    read_data("d:/homework/digits.txt", data, &num);
    int k = 3;
    float result[MAX_DIGIT];
    KNN(data, num, k, result);
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