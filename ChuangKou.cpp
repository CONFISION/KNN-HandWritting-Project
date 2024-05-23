#include <graphics.h>
#include <conio.h>

// 初始化绘图窗口大小
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 480;

// 定义画笔大小
const int PEN_SIZE = 2;

// 定义分区线位置
const int DIVIDER_X = WINDOW_WIDTH / 2;

int main() {
    // 初始化图形窗口
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);

    // 设置背景颜色为白色
    setbkcolor(WHITE);
    cleardevice();

    // 画分割线
    setlinecolor(BLACK);
    line(DIVIDER_X, 0, DIVIDER_X, WINDOW_HEIGHT);

    // 设置画笔颜色为黑色
    setlinecolor(BLACK);
    setfillcolor(BLACK);

    bool drawing = false;
    int x = 0, y = 0;

    while (true) {
        // 检查是否有键盘输入，ESC键退出
        if (_kbhit()) {
            if (_getch() == 27) {
                break;
            }
        }

        // 获取鼠标状态
        MOUSEMSG msg = GetMouseMsg();

        switch (msg.uMsg) {
        case WM_LBUTTONDOWN:
            // 只允许在右边区域绘图
            if (msg.x >= DIVIDER_X) {
                drawing = true;
                x = msg.x;
                y = msg.y;
            }
            break;
        case WM_MOUSEMOVE:
            if (drawing) {
                // 绘制小圆点，只在右边区域绘图
                if (x >= DIVIDER_X) {
                    for (int i = -PEN_SIZE; i <= PEN_SIZE; ++i) {
                        for (int j = -PEN_SIZE; j <= PEN_SIZE; ++j) {
                            putpixel(x + i, y + j, BLACK);
                        }
                    }
                    x = msg.x;
                    y = msg.y;
                }
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
