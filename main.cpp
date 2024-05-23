#include <graphics.h>
#include <conio.h>
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
        // 检查是否有键盘输入
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 27) 
            { // ESC键退出
                break;
            }
            else if (ch == 'c' || ch == 'C') 
            { // 'c'键清除窗口
                cleardevice();
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
                for (int i = -PEN_SIZE; i <= PEN_SIZE; ++i) {
                    for (int j = -PEN_SIZE; j <= PEN_SIZE; ++j) {
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
