// KNNʵ����д����ʶ��

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
void knn(DATA *data, int num, int k, float *result)
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