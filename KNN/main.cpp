#define _CRT_SECURE_NO_WARNINGS
#include <graphics.h>
#include "KNN.cpp"

// 初始化绘图窗口大小
const int WINDOW_WIDTH = 480;
const int WINDOW_HEIGHT = 480;

// 定义画笔大小
const int PEN_SIZE = 2;

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

    bool drawing = false;
    int x = 0, y = 0;

    while (true)
    {
        ExMessage msKey;
        // 检查是否有键盘输入
        if (peekmessage(&msKey, EX_KEY,true))
        {
            switch (msKey.vkcode)
            {
            case 0x1B: // 按下ESC键退出程序
                printf("Program exited.");
                exit(0);
                break;
            case 0x43: // 按下C键清空画板
                cleardevice();
                break;
            case 0x53: // 按下S键识别数字
                Turn_Picture_to_txt();
                break;
            }
        }

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
        }
    }

    // 关闭图形窗口
    closegraph();
    return 0;
}
