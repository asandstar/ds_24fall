#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stack>
#include <vector>
#include <queue>
#include <utility>
#include <string>

// 定义常量
const int WINDOW_WIDTH = 1024;  // 窗口宽度
const int WINDOW_HEIGHT = 1074; // 窗口高度（包含调色板和工具区）
const int CANVAS_SIZE = 256;    // 逻辑画板尺寸
const int PIXEL_SIZE = 4;       // 每个像素块的显示尺寸
const int PALETTE_SIZE = 50;    // 调色板尺寸
const int TOOL_SIZE = 50;       // 工具按钮尺寸

// 定义不同程度的灰色
const COLORREF GRAY_COLORS[] = {
    RGB(192, 192, 192), // 浅灰色
    RGB(160, 160, 160), // 中灰色
    RGB(128, 128, 128), // 深灰色
    RGB(96, 96, 96)     // 更深的灰色
};

// 记录每个像素的颜色
COLORREF canvas[CANVAS_SIZE][CANVAS_SIZE];
COLORREF currentColor = BLACK; // 当前选择的颜色

// 操作栈用于撤销功能
std::stack<std::vector<std::vector<COLORREF>>> history;

// 工具类型枚举
enum Tool
{
    PEN,
    BUCKET,
    ERASER
};
Tool currentTool = PEN; // 当前选择的工具

// 保存当前画布状态
void saveState()
{
    std::vector<std::vector<COLORREF>> state(CANVAS_SIZE, std::vector<COLORREF>(CANVAS_SIZE));
    for (int i = 0; i < CANVAS_SIZE; ++i)
    {
        for (int j = 0; j < CANVAS_SIZE; ++j)
        {
            state[i][j] = canvas[i][j];
        }
    }
    history.push(state);
}

// 绘制调色板
void drawPalette()
{
    // 定义调色板颜色
    const COLORREF colors[] = {
        RGB(255, 0, 0),     // 红色
        RGB(255, 165, 0),   // 橙色
        RGB(255, 255, 0),   // 黄色
        RGB(0, 128, 0),     // 绿色
        RGB(0, 0, 255),     // 蓝色
        RGB(75, 0, 130),    // 靛色
        RGB(238, 130, 238), // 紫色
        RGB(0, 0, 0),       // 黑色
        RGB(255, 255, 255)  // 白色
    };
    // 绘制调色板
    for (int i = 0; i < 9; ++i)
    {
        setfillcolor(colors[i]);
        bar(i * PALETTE_SIZE, 0, (i + 1) * PALETTE_SIZE, PALETTE_SIZE);
    }
}

// 绘制工具区
void drawTools()
{
    // 定义工具按钮标签和颜色
    const char *toolNames[] = {"Pen", "Bucket", "Eraser", "Undo"};
    for (int i = 0; i < 4; ++i)
    {
        setfillcolor(GRAY_COLORS[i]);
        bar(i * TOOL_SIZE, PALETTE_SIZE, (i + 1) * TOOL_SIZE, PALETTE_SIZE + TOOL_SIZE);
    }
    // for (int i = 0; i < 4; ++i) {
    //     setfillcolor(LIGHTGRAY);
    //     bar(i * TOOL_SIZE, PALETTE_SIZE, (i + 1) * TOOL_SIZE, PALETTE_SIZE + TOOL_SIZE);
    //     settextcolor(DARKGRAY);
    //     //outtextxy(i * TOOL_SIZE + 10, PALETTE_SIZE + 15, toolNames[i]);
    // }
}

// 绘制画板
void drawCanvas()
{
    // 绘制画板边框
    rectangle(TOOL_SIZE * 4, PALETTE_SIZE, TOOL_SIZE * 4 + CANVAS_SIZE * PIXEL_SIZE, PALETTE_SIZE + CANVAS_SIZE * PIXEL_SIZE);
    // 遍历画板每个像素，放大绘制
    for (int i = 0; i < CANVAS_SIZE; ++i)
    {
        for (int j = 0; j < CANVAS_SIZE; ++j)
        {
            setfillcolor(canvas[i][j]);
            bar(TOOL_SIZE * 4 + i * PIXEL_SIZE, PALETTE_SIZE + j * PIXEL_SIZE, TOOL_SIZE * 4 + (i + 1) * PIXEL_SIZE, PALETTE_SIZE + (j + 1) * PIXEL_SIZE);
        }
    }
}

// 绘制单个像素
void drawPixel(int x, int y)
{
    setfillcolor(canvas[x][y]);
    bar(TOOL_SIZE * 4 + x * PIXEL_SIZE, PALETTE_SIZE + y * PIXEL_SIZE, TOOL_SIZE * 4 + (x + 1) * PIXEL_SIZE, PALETTE_SIZE + (y + 1) * PIXEL_SIZE);
}

// 绘制工具和颜色状态
void drawStatus()
{
    setfillcolor(currentColor);
    bar(0, WINDOW_HEIGHT - PALETTE_SIZE, PALETTE_SIZE, WINDOW_HEIGHT);
    settextcolor(DARKGRAY);
    setbkmode(TRANSPARENT);
    std::string toolText = "Tool: ";
    if (currentTool == PEN)
        toolText += "Pen";
    else if (currentTool == BUCKET)
        toolText += "Bucket";
    else if (currentTool == ERASER)
        toolText += "Eraser";
    // outtextxy(PALETTE_SIZE + 10, WINDOW_HEIGHT - PALETTE_SIZE + 10, toolText.c_str());
}

// 油漆桶填充算法（使用广度优先搜索）
void bucketFill(int x, int y, COLORREF oldColor)
{
    // 边界检查和颜色检查
    if (x < 0 || x >= CANVAS_SIZE || y < 0 || y >= CANVAS_SIZE || canvas[x][y] != oldColor || canvas[x][y] == currentColor)
    {
        return;
    }

    std::queue<std::pair<int, int>> q;
    q.push(std::make_pair(x, y));
    canvas[x][y] = currentColor;
    drawPixel(x, y); // 绘制填充的像素

    // 广度优先搜索
    while (!q.empty())
    {
        std::pair<int, int> current = q.front();
        q.pop();
        int cx = current.first;
        int cy = current.second;

        // 检查上下左右四个方向
        std::vector<std::pair<int, int>> directions = {
            std::make_pair(1, 0),  // 向右
            std::make_pair(-1, 0), // 向左
            std::make_pair(0, 1),  // 向下
            std::make_pair(0, -1)  // 向上
        };
        for (std::pair<int, int> direction : directions)
        {
            int nx = cx + direction.first;
            int ny = cy + direction.second;
            if (nx >= 0 && nx < CANVAS_SIZE && ny >= 0 && ny < CANVAS_SIZE && canvas[nx][ny] == oldColor)
            {
                canvas[nx][ny] = currentColor;
                drawPixel(nx, ny); // 绘制填充的像素
                q.push(std::make_pair(nx, ny));
            }
        }
    }
}

int main()
{
    // 初始化图形窗口
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    cleardevice();

    // 初始化画布为白色
    for (int i = 0; i < CANVAS_SIZE; ++i)
    {
        for (int j = 0; j < CANVAS_SIZE; ++j)
        {
            canvas[i][j] = WHITE;
        }
    }

    saveState(); // 保存初始状态

    drawPalette(); // 初始绘制调色板
    drawTools();   // 初始绘制工具区
    drawCanvas();  // 初始绘制画板
    drawStatus();  // 初始绘制当前工具和颜色状态

    while (true)
    {
        ExMessage msg = getmessage(EX_MOUSE | EX_KEY);

        if (msg.message == WM_LBUTTONDOWN)
        {
            int x = msg.x, y = msg.y;
            if (y < PALETTE_SIZE)
            {
                // 选择调色板颜色
                int colorIndex = x / PALETTE_SIZE;
                const COLORREF colors[] = {
                    RGB(255, 0, 0),     // 红色
                    RGB(255, 165, 0),   // 橙色
                    RGB(255, 255, 0),   // 黄色
                    RGB(0, 128, 0),     // 绿色
                    RGB(0, 0, 255),     // 蓝色
                    RGB(75, 0, 130),    // 靛色
                    RGB(238, 130, 238), // 紫色
                    RGB(0, 0, 0),       // 黑色
                    RGB(255, 255, 255)  // 白色
                };
                currentColor = colors[colorIndex];
                drawStatus(); // 更新状态显示
            }
            else if (y >= PALETTE_SIZE && y < PALETTE_SIZE + TOOL_SIZE)
            {
                // 选择工具
                int toolIndex = x / TOOL_SIZE;
                if (toolIndex == 0)
                    currentTool = PEN;
                else if (toolIndex == 1)
                    currentTool = BUCKET;
                else if (toolIndex == 2)
                    currentTool = ERASER;
                else if (toolIndex == 3 && !history.empty())
                {
                    // 撤销上一步操作
                    auto lastState = history.top();
                    history.pop();
                    for (int i = 0; i < CANVAS_SIZE; ++i)
                    {
                        for (int j = 0; j < CANVAS_SIZE; ++j)
                        {
                            canvas[i][j] = lastState[i][j];
                        }
                    }
                    drawCanvas(); // 重绘整个画板
                }
                drawStatus(); // 更新状态显示
            }
            else
            {
                // 在画板上绘制
                x = (x - TOOL_SIZE * 4) / PIXEL_SIZE;
                y = (y - PALETTE_SIZE) / PIXEL_SIZE;
                if (x < CANVAS_SIZE && y < CANVAS_SIZE && x >= 0 && y >= 0)
                {
                    saveState(); // 保存当前状态
                    if (currentTool == PEN)
                    {
                        canvas[x][y] = currentColor;
                        drawPixel(x, y); // 绘制单个像素
                    }
                    else if (currentTool == BUCKET)
                    {
                        COLORREF oldColor = canvas[x][y];
                        bucketFill(x, y, oldColor);
                    }
                    else if (currentTool == ERASER)
                    {
                        canvas[x][y] = WHITE;
                        drawPixel(x, y); // 绘制单个像素
                    }
                }
            }
        }
        else if (msg.message == WM_MOUSEMOVE && (msg.lbutton & MK_LBUTTON))
        {
            int x = msg.x, y = msg.y;
            x = (x - TOOL_SIZE * 4) / PIXEL_SIZE;
            y = (y - PALETTE_SIZE) / PIXEL_SIZE;
            if (x < CANVAS_SIZE && y < CANVAS_SIZE && x >= 0 && y >= 0)
            {
                if (currentTool == PEN)
                {
                    canvas[x][y] = currentColor;
                    drawPixel(x, y); // 绘制单个像素
                }
                else if (currentTool == ERASER)
                {
                    canvas[x][y] = WHITE;
                    drawPixel(x, y); // 绘制单个像素
                }
            }
        }
        FlushBatchDraw();
    }

    closegraph();
    return 0;
}

/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————实现基本功能*/
// #include <graphics.h>
// #include <conio.h>
// #include <stdio.h>
// #include <stack>
// #include <vector>
// #include <queue>
// #include <utility>
//
//// 定义常量
// const int WINDOW_WIDTH = 1024; // 窗口宽度
// const int WINDOW_HEIGHT = 1074; // 窗口高度（包含调色板）
// const int CANVAS_SIZE = 256; // 逻辑画板尺寸
// const int PIXEL_SIZE = 4; // 每个像素块的显示尺寸
// const int PALETTE_SIZE = 50; // 调色板尺寸
// const int TOOL_SIZE = 50;//工具按键尺寸
//
//// 记录每个像素的颜色
// COLORREF canvas[CANVAS_SIZE][CANVAS_SIZE];
// COLORREF currentColor = BLACK; // 当前选择的颜色
//
//// 操作栈用于撤销功能
// std::stack<std::vector<std::vector<COLORREF>>> history;
//
//// 工具类型枚举
// enum Tool { PEN, BUCKET, ERASER };
// Tool currentTool = PEN; // 当前选择的工具
//
//// 保存当前画布状态
// void saveState() {
//     std::vector<std::vector<COLORREF>> state(CANVAS_SIZE, std::vector<COLORREF>(CANVAS_SIZE));
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             state[i][j] = canvas[i][j];
//         }
//     }
//     history.push(state);
// }
//
//// 绘制调色板
// void drawPalette() {
//     const COLORREF colors[] = {
//         RGB(255, 0, 0), // 红色
//         RGB(255, 165, 0), // 橙色
//         RGB(255, 255, 0), // 黄色
//         RGB(0, 128, 0), // 绿色
//         RGB(0, 0, 255), // 蓝色
//         RGB(75, 0, 130), // 靛色
//         RGB(238, 130, 238), // 紫色
//         RGB(0, 0, 0), // 黑色
//         RGB(255, 255, 255) // 白色
//     };
//     // 绘制调色板
//     for (int i = 0; i < 9; ++i) {
//         setfillcolor(colors[i]);
//         bar(i * PALETTE_SIZE, 0, (i + 1) * PALETTE_SIZE, PALETTE_SIZE);
//     }
// }
//
//// 绘制画板
// void drawCanvas() {
//     // 遍历画板每个像素，放大绘制
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             setfillcolor(canvas[i][j]);
//             bar(i * PIXEL_SIZE, j * PIXEL_SIZE + PALETTE_SIZE, (i + 1) * PIXEL_SIZE, (j + 1) * PIXEL_SIZE + PALETTE_SIZE);
//         }
//     }
// }
//
//// 油漆桶填充算法（使用广度优先搜索）
// void bucketFill(int x, int y, COLORREF oldColor) {
//     // 边界检查和颜色检查
//     if (x < 0 || x >= CANVAS_SIZE || y < 0 || y >= CANVAS_SIZE || canvas[x][y] != oldColor || canvas[x][y] == currentColor) {
//         return;
//     }
//
//     std::queue<std::pair<int, int>> q;
//     q.push(std::make_pair(x, y));
//     canvas[x][y] = currentColor;
//
//     // 广度优先搜索
//     while (!q.empty()) {
//         std::pair<int, int> current = q.front();
//         q.pop();
//         int cx = current.first;
//         int cy = current.second;
//
//         // 检查上下左右四个方向
//         std::vector<std::pair<int, int>> directions = {
//             std::make_pair(1, 0), // 向右
//             std::make_pair(-1, 0), // 向左
//             std::make_pair(0, 1), // 向下
//             std::make_pair(0, -1) // 向上
//         };
//         for (std::pair<int, int> direction : directions) {
//             int nx = cx + direction.first;
//             int ny = cy + direction.second;
//             if (nx >= 0 && nx < CANVAS_SIZE && ny >= 0 && ny < CANVAS_SIZE && canvas[nx][ny] == oldColor) {
//                 canvas[nx][ny] = currentColor;
//                 q.push(std::make_pair(nx, ny));
//             }
//         }
//     }
// }
//
// int main() {
//     // 初始化图形窗口
//     initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
//     cleardevice();
//
//     // 初始化画布为白色
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             canvas[i][j] = WHITE;
//         }
//     }
//
//     saveState(); // 保存初始状态
//
//     while (true) {
//         drawPalette(); // 绘制调色板
//         drawCanvas(); // 绘制画板
//         ExMessage msg = getmessage(EX_MOUSE | EX_KEY);
//
//         if (msg.message == WM_LBUTTONDOWN) {
//             int x = msg.x, y = msg.y;
//             if (y < PALETTE_SIZE) {
//                 // 选择调色板颜色
//                 int colorIndex = x / PALETTE_SIZE;
//                 const COLORREF colors[] = {
//                     RGB(255, 0, 0), // 红色
//                     RGB(255, 165, 0), // 橙色
//                     RGB(255, 255, 0), // 黄色
//                     RGB(0, 128, 0), // 绿色
//                     RGB(0, 0, 255), // 蓝色
//                     RGB(75, 0, 130), // 靛色
//                     RGB(238, 130, 238), // 紫色
//                     RGB(0, 0, 0), // 黑色
//                     RGB(255, 255, 255) // 白色
//                 };
//                 currentColor = colors[colorIndex];
//             }
//             else {
//                 // 在画板上绘制
//                 x /= PIXEL_SIZE;
//                 y = (y - PALETTE_SIZE) / PIXEL_SIZE;
//                 if (x < CANVAS_SIZE && y < CANVAS_SIZE) {
//                     saveState(); // 保存当前状态
//                     if (currentTool == PEN) {
//                         canvas[x][y] = currentColor;
//                     }
//                     else if (currentTool == BUCKET) {
//                         COLORREF oldColor = canvas[x][y];
//                         bucketFill(x, y, oldColor);
//                     }
//                     else if (currentTool == ERASER) {
//                         canvas[x][y] = WHITE;
//                     }
//                 }
//             }
//         }
//         else if (msg.message == WM_MOUSEMOVE && (msg.lbutton & MK_LBUTTON)) {
//             int x = msg.x, y = msg.y;
//             x /= PIXEL_SIZE;
//             y = (y - PALETTE_SIZE) / PIXEL_SIZE;
//             if (x < CANVAS_SIZE && y < CANVAS_SIZE) {
//                 if (currentTool == PEN) {
//                     canvas[x][y] = currentColor;
//                 }
//                 else if (currentTool == ERASER) {
//                     canvas[x][y] = WHITE;
//                 }
//             }
//         }
//         else if (msg.message == WM_KEYDOWN) {
//             if (msg.vkcode == 'P') {
//                 currentTool = PEN; // 切换为画笔工具
//             }
//             else if (msg.vkcode == 'B') {
//                 currentTool = BUCKET; // 切换为油漆桶工具
//             }
//             else if (msg.vkcode == 'E') {
//                 currentTool = ERASER; // 切换为橡皮擦工具
//             }
//             else if (msg.vkcode == 'Z' && !history.empty()) {
//                 // 撤销上一步操作
//                 auto lastState = history.top();
//                 history.pop();
//                 for (int i = 0; i < CANVAS_SIZE; ++i) {
//                     for (int j = 0; j < CANVAS_SIZE; ++j) {
//                         canvas[i][j] = lastState[i][j];
//                     }
//                 }
//             }
//         }
//         FlushBatchDraw();
//     }
//
//     closegraph();
//     return 0;
// }
/*——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————下面这版还有点问题*/
// #include <graphics.h>
// #include <conio.h>
// #include <stdio.h>
// #include <stack>
// #include <vector>
// #include <queue>
// #include <utility>
//
//// 定义常量
// const int WINDOW_WIDTH = 1024; // 窗口宽度
// const int WINDOW_HEIGHT = 1074; // 窗口高度（包含调色板）
// const int CANVAS_SIZE = 256; // 逻辑画板尺寸
// const int PIXEL_SIZE = 4; // 每个像素块的显示尺寸
// const int PALETTE_SIZE = 50; // 调色板尺寸
// const int TOOL_SIZE = 50;//工具按键尺寸
//
//// 记录每个像素的颜色
// COLORREF canvas[CANVAS_SIZE][CANVAS_SIZE];
// COLORREF currentColor = BLACK; // 当前选择的颜色
//
//// 操作栈用于撤销功能
// std::stack<std::vector<std::vector<COLORREF>>> history;
//
//// 工具类型枚举
// enum Tool { PEN, BUCKET, ERASER };
// Tool currentTool = PEN; // 当前选择的工具
//
//// 保存当前画布状态
// void saveState() {
//     std::vector<std::vector<COLORREF>> state(CANVAS_SIZE, std::vector<COLORREF>(CANVAS_SIZE));
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             state[i][j] = canvas[i][j];
//         }
//     }
//     history.push(state);
// }
//
//// 绘制调色板
// void drawPalette() {
//     const COLORREF colors[] = {
//         RGB(255, 0, 0), // 红色
//         RGB(255, 165, 0), // 橙色
//         RGB(255, 255, 0), // 黄色
//         RGB(0, 128, 0), // 绿色
//         RGB(0, 0, 255), // 蓝色
//         RGB(75, 0, 130), // 靛色
//         RGB(238, 130, 238), // 紫色
//         RGB(0, 0, 0), // 黑色
//         RGB(255, 255, 255) // 白色
//     };
//     // 绘制调色板
//     for (int i = 0; i < 9; ++i) {
//         setfillcolor(colors[i]);
//         bar(i * PALETTE_SIZE, 0, (i + 1) * PALETTE_SIZE, PALETTE_SIZE);
//     }
// }
//
//// 绘制画板
// void drawCanvas() {
//     // 遍历画板每个像素，放大绘制
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             setfillcolor(canvas[i][j]);
//             bar(i * PIXEL_SIZE, j * PIXEL_SIZE + PALETTE_SIZE, (i + 1) * PIXEL_SIZE, (j + 1) * PIXEL_SIZE + PALETTE_SIZE);
//         }
//     }
// }
//
//// 油漆桶填充算法（使用广度优先搜索）
// void bucketFill(int x, int y, COLORREF oldColor) {
//     // 边界检查和颜色检查
//     if (x < 0 || x >= CANVAS_SIZE || y < 0 || y >= CANVAS_SIZE || canvas[x][y] != oldColor || canvas[x][y] == currentColor) {
//         return;
//     }
//
//     std::queue<std::pair<int, int>> q;
//     q.push(std::make_pair(x, y));
//     canvas[x][y] = currentColor;
//
//     // 广度优先搜索
//     while (!q.empty()) {
//         std::pair<int, int> current = q.front();
//         q.pop();
//         int cx = current.first;
//         int cy = current.second;
//
//         // 检查上下左右四个方向
//         std::vector<std::pair<int, int>> directions = {
//             std::make_pair(1, 0), // 向右
//             std::make_pair(-1, 0), // 向左
//             std::make_pair(0, 1), // 向下
//             std::make_pair(0, -1) // 向上
//         };
//         for (std::pair<int, int> direction : directions) {
//             int nx = cx + direction.first;
//             int ny = cy + direction.second;
//             if (nx >= 0 && nx < CANVAS_SIZE && ny >= 0 && ny < CANVAS_SIZE && canvas[nx][ny] == oldColor) {
//                 canvas[nx][ny] = currentColor;
//                 q.push(std::make_pair(nx, ny));
//             }
//         }
//     }
// }
//
// int main() {
//     // 初始化图形窗口
//     initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
//     cleardevice();
//
//     // 初始化画布为白色
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             canvas[i][j] = WHITE;
//         }
//     }
//
//     saveState(); // 保存初始状态
//
//     while (true) {
//         drawPalette(); // 绘制调色板
//         drawCanvas(); // 绘制画板
//         ExMessage msg = getmessage(EX_MOUSE | EX_KEY);
//
//         if (msg.message == WM_LBUTTONDOWN) {
//             int x = msg.x, y = msg.y;
//             if (y < PALETTE_SIZE) {
//                 // 选择调色板颜色
//                 int colorIndex = x / PALETTE_SIZE;
//                 const COLORREF colors[] = {
//                     RGB(255, 0, 0), // 红色
//                     RGB(255, 165, 0), // 橙色
//                     RGB(255, 255, 0), // 黄色
//                     RGB(0, 128, 0), // 绿色
//                     RGB(0, 0, 255), // 蓝色
//                     RGB(75, 0, 130), // 靛色
//                     RGB(238, 130, 238), // 紫色
//                     RGB(0, 0, 0), // 黑色
//                     RGB(255, 255, 255) // 白色
//                 };
//                 currentColor = colors[colorIndex];
//             }
//             else {
//                 // 在画板上绘制
//                 x /= PIXEL_SIZE;
//                 y = (y - PALETTE_SIZE) / PIXEL_SIZE;
//                 if (x < CANVAS_SIZE && y < CANVAS_SIZE) {
//                     saveState(); // 保存当前状态
//                     if (currentTool == PEN) {
//                         canvas[x][y] = currentColor;
//                     }
//                     else if (currentTool == BUCKET) {
//                         COLORREF oldColor = canvas[x][y];
//                         bucketFill(x, y, oldColor);
//                     }
//                     else if (currentTool == ERASER) {
//                         canvas[x][y] = WHITE;
//                     }
//                 }
//             }
//         }
//         else if (msg.message == WM_MOUSEMOVE && (msg.lbutton & MK_LBUTTON)) {
//             int x = msg.x, y = msg.y;
//             x /= PIXEL_SIZE;
//             y = (y - PALETTE_SIZE) / PIXEL_SIZE;
//             if (x < CANVAS_SIZE && y < CANVAS_SIZE) {
//                 if (currentTool == PEN) {
//                     canvas[x][y] = currentColor;
//                 }
//                 else if (currentTool == ERASER) {
//                     canvas[x][y] = WHITE;
//                 }
//             }
//         }
//         else if (msg.message == WM_KEYDOWN) {
//             if (msg.vkcode == 'P') {
//                 currentTool = PEN; // 切换为画笔工具
//             }
//             else if (msg.vkcode == 'B') {
//                 currentTool = BUCKET; // 切换为油漆桶工具
//             }
//             else if (msg.vkcode == 'E') {
//                 currentTool = ERASER; // 切换为橡皮擦工具
//             }
//             else if (msg.vkcode == 'Z' && !history.empty()) {
//                 // 撤销上一步操作
//                 auto lastState = history.top();
//                 history.pop();
//                 for (int i = 0; i < CANVAS_SIZE; ++i) {
//                     for (int j = 0; j < CANVAS_SIZE; ++j) {
//                         canvas[i][j] = lastState[i][j];
//                     }
//                 }
//             }
//         }
//         FlushBatchDraw();
//     }
//
//     closegraph();
//     return 0;
// }

/*——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————初版本的画图*/
// #include <graphics.h>
// #include <conio.h>
// #include <stdio.h>
// #include <stack>
// #include <vector>
// #include <queue>
// #include <utility>
//
//// 定义一些常量
// const int WINDOW_WIDTH = 600;
// const int WINDOW_HEIGHT = 400;
// const int CANVAS_SIZE = 256;
// const int PALETTE_SIZE = 50;
//
//// 记录每个像素的颜色
// COLORREF canvas[CANVAS_SIZE][CANVAS_SIZE];
// COLORREF currentColor = BLACK;
//
//// 操作栈用于撤销功能
// std::stack<std::vector<std::vector<COLORREF>>> history;
//
//// 记录当前工具类型
// enum Tool { PEN, BUCKET, ERASER };
// Tool currentTool = PEN;
//
//// 保存当前画布状态
// void saveState() {
//     std::vector<std::vector<COLORREF>> state(CANVAS_SIZE, std::vector<COLORREF>(CANVAS_SIZE));
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             state[i][j] = canvas[i][j];
//         }
//     }
//     history.push(state);
// }
//
//// 绘制调色板
// void drawPalette() {
//     for (int i = 0; i < 10; ++i) {
//         setfillcolor(RGB(i * 25, 0, 0));
//         bar(i * PALETTE_SIZE, 0, (i + 1) * PALETTE_SIZE, PALETTE_SIZE);
//     }
// }
//
//// 绘制画板
// void drawCanvas() {
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             putpixel(i, j, canvas[i][j]);
//         }
//     }
// }
//
//// 油漆桶填充算法（使用广度优先搜索）
// void bucketFill(int x, int y, COLORREF oldColor) {
//     if (x < 0 || x >= CANVAS_SIZE || y < 0 || y >= CANVAS_SIZE || canvas[x][y] != oldColor || canvas[x][y] == currentColor) {
//         return;
//     }
//
//     std::queue<std::pair<int, int>> q;
//     q.push(std::make_pair(x, y));
//     canvas[x][y] = currentColor;
//
//     while (!q.empty()) {
//         std::pair<int, int> current = q.front();
//         q.pop();
//         int cx = current.first;
//         int cy = current.second;
//
//         // 检查上下左右四个方向
//         std::vector<std::pair<int, int>> directions = { std::make_pair(1, 0), std::make_pair(-1, 0), std::make_pair(0, 1), std::make_pair(0, -1) };
//         for (std::pair<int, int> direction : directions) {
//             int nx = cx + direction.first;
//             int ny = cy + direction.second;
//             if (nx >= 0 && nx < CANVAS_SIZE && ny >= 0 && ny < CANVAS_SIZE && canvas[nx][ny] == oldColor) {
//                 canvas[nx][ny] = currentColor;
//                 q.push(std::make_pair(nx, ny));
//             }
//         }
//     }
// }
//
// int main() {
//     initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
//     cleardevice();
//
//     // 初始化画布为白色
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             canvas[i][j] = WHITE;
//         }
//     }
//
//     saveState(); // 初始状态保存
//
//     while (true) {
//         drawPalette();
//         drawCanvas();
//         ExMessage msg = getmessage(EX_MOUSE | EX_KEY);
//         if (msg.message == WM_LBUTTONDOWN) {
//             int x = msg.x, y = msg.y;
//             if (y < PALETTE_SIZE) {
//                 // 选择调色板颜色
//                 int colorIndex = x / PALETTE_SIZE;
//                 currentColor = RGB(colorIndex * 25, 0, 0);
//             }
//             else if (x < CANVAS_SIZE && y < CANVAS_SIZE) {
//                 saveState(); // 保存当前状态
//                 if (currentTool == PEN) {
//                     canvas[x][y] = currentColor;
//                 }
//                 else if (currentTool == BUCKET) {
//                     COLORREF oldColor = canvas[x][y];
//                     bucketFill(x, y, oldColor);
//                 }
//                 else if (currentTool == ERASER) {
//                     canvas[x][y] = WHITE;
//                 }
//             }
//         }
//         else if (msg.message == WM_MOUSEMOVE && (msg.lbutton & MK_LBUTTON)) {
//             int x = msg.x, y = msg.y;
//             if (x < CANVAS_SIZE && y < CANVAS_SIZE) {
//                 if (currentTool == PEN) {
//                     canvas[x][y] = currentColor;
//                 }
//                 else if (currentTool == ERASER) {
//                     canvas[x][y] = WHITE;
//                 }
//             }
//         }
//         else if (msg.message == WM_KEYDOWN) {
//             if (msg.vkcode == 'P') {
//                 currentTool = PEN;
//             }
//             else if (msg.vkcode == 'B') {
//                 currentTool = BUCKET;
//             }
//             else if (msg.vkcode == 'E') {
//                 currentTool = ERASER;
//             }
//             else if (msg.vkcode == 'Z' && !history.empty()) {
//                 // 撤销上一步操作
//                 auto lastState = history.top();
//                 history.pop();
//                 for (int i = 0; i < CANVAS_SIZE; ++i) {
//                     for (int j = 0; j < CANVAS_SIZE; ++j) {
//                         canvas[i][j] = lastState[i][j];
//                     }
//                 }
//             }
//         }
//         FlushBatchDraw();
//     }
//
//     closegraph();
//     return 0;
// }
//
//
//  /*——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————测试easy库*/
////// 编译环境：Visual C++ 6.0~2022，EasyX_2023大暑版
////// https://easyx.cn
//////
////#include <graphics.h>
////#include <conio.h>
////#include <stdio.h>
////
/////*
////（1） 画板：一个256 × 256 像素画幅的画板
////（2） 调色板和画笔：鼠标左键点选相应的前景颜色。
////（3） 画笔工具：鼠标左键点击实现在画板上相应的像素实现对相应像素上色。
////（4） 油漆桶工具：使用前景色填充颜色相近的区域（实现对于连续区域内实现一次性上色）。
////（5） 橡皮擦：将相应的像素的颜色擦除。
////（6） 撤销：撤销上一步操作。
////*/
////
////
////int main()
////{
////	// 初始化图形窗口
////	initgraph(640, 480);
////
////	ExMessage m;		// 定义消息变量
////
////	while (true)
////	{
////		// 获取一条鼠标或按键消息
////		m = getmessage(EX_MOUSE | EX_KEY);
////
////		switch (m.message)
////		{
////		case WM_MOUSEMOVE:
////			// 鼠标移动的时候画红色的小点
////			putpixel(m.x, m.y, RED);
////			break;
////
////		case WM_LBUTTONDOWN:
////			// 如果点左键的同时按下了 Ctrl 键
////			if (m.ctrl)
////				// 画一个大方块
////				rectangle(m.x - 10, m.y - 10, m.x + 10, m.y + 10);
////			else
////				// 画一个小方块
////				rectangle(m.x - 5, m.y - 5, m.x + 5, m.y + 5);
////			break;
////
////		case WM_KEYDOWN:
////			if (m.vkcode == VK_ESCAPE)
////				return 0;	// 按 ESC 键退出程序
////		}
////	}
////
////	// 关闭图形窗口
////	closegraph();
////	return 0;
////}
