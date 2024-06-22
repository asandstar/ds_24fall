#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stack>
#include <vector>
#include <queue>
#include <utility>
#include <string>

// ���峣��
const int WINDOW_WIDTH = 1024;  // ���ڿ��
const int WINDOW_HEIGHT = 1074; // ���ڸ߶ȣ�������ɫ��͹�������
const int CANVAS_SIZE = 256;    // �߼�����ߴ�
const int PIXEL_SIZE = 4;       // ÿ�����ؿ����ʾ�ߴ�
const int PALETTE_SIZE = 50;    // ��ɫ��ߴ�
const int TOOL_SIZE = 50;       // ���߰�ť�ߴ�

// ���岻ͬ�̶ȵĻ�ɫ
const COLORREF GRAY_COLORS[] = {
    RGB(192, 192, 192), // ǳ��ɫ
    RGB(160, 160, 160), // �л�ɫ
    RGB(128, 128, 128), // ���ɫ
    RGB(96, 96, 96)     // ����Ļ�ɫ
};

// ��¼ÿ�����ص���ɫ
COLORREF canvas[CANVAS_SIZE][CANVAS_SIZE];
COLORREF currentColor = BLACK; // ��ǰѡ�����ɫ

// ����ջ���ڳ�������
std::stack<std::vector<std::vector<COLORREF>>> history;

// ��������ö��
enum Tool
{
    PEN,
    BUCKET,
    ERASER
};
Tool currentTool = PEN; // ��ǰѡ��Ĺ���

// ���浱ǰ����״̬
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

// ���Ƶ�ɫ��
void drawPalette()
{
    // �����ɫ����ɫ
    const COLORREF colors[] = {
        RGB(255, 0, 0),     // ��ɫ
        RGB(255, 165, 0),   // ��ɫ
        RGB(255, 255, 0),   // ��ɫ
        RGB(0, 128, 0),     // ��ɫ
        RGB(0, 0, 255),     // ��ɫ
        RGB(75, 0, 130),    // ��ɫ
        RGB(238, 130, 238), // ��ɫ
        RGB(0, 0, 0),       // ��ɫ
        RGB(255, 255, 255)  // ��ɫ
    };
    // ���Ƶ�ɫ��
    for (int i = 0; i < 9; ++i)
    {
        setfillcolor(colors[i]);
        bar(i * PALETTE_SIZE, 0, (i + 1) * PALETTE_SIZE, PALETTE_SIZE);
    }
}

// ���ƹ�����
void drawTools()
{
    // ���幤�߰�ť��ǩ����ɫ
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

// ���ƻ���
void drawCanvas()
{
    // ���ƻ���߿�
    rectangle(TOOL_SIZE * 4, PALETTE_SIZE, TOOL_SIZE * 4 + CANVAS_SIZE * PIXEL_SIZE, PALETTE_SIZE + CANVAS_SIZE * PIXEL_SIZE);
    // ��������ÿ�����أ��Ŵ����
    for (int i = 0; i < CANVAS_SIZE; ++i)
    {
        for (int j = 0; j < CANVAS_SIZE; ++j)
        {
            setfillcolor(canvas[i][j]);
            bar(TOOL_SIZE * 4 + i * PIXEL_SIZE, PALETTE_SIZE + j * PIXEL_SIZE, TOOL_SIZE * 4 + (i + 1) * PIXEL_SIZE, PALETTE_SIZE + (j + 1) * PIXEL_SIZE);
        }
    }
}

// ���Ƶ�������
void drawPixel(int x, int y)
{
    setfillcolor(canvas[x][y]);
    bar(TOOL_SIZE * 4 + x * PIXEL_SIZE, PALETTE_SIZE + y * PIXEL_SIZE, TOOL_SIZE * 4 + (x + 1) * PIXEL_SIZE, PALETTE_SIZE + (y + 1) * PIXEL_SIZE);
}

// ���ƹ��ߺ���ɫ״̬
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

// ����Ͱ����㷨��ʹ�ù������������
void bucketFill(int x, int y, COLORREF oldColor)
{
    // �߽������ɫ���
    if (x < 0 || x >= CANVAS_SIZE || y < 0 || y >= CANVAS_SIZE || canvas[x][y] != oldColor || canvas[x][y] == currentColor)
    {
        return;
    }

    std::queue<std::pair<int, int>> q;
    q.push(std::make_pair(x, y));
    canvas[x][y] = currentColor;
    drawPixel(x, y); // ������������

    // �����������
    while (!q.empty())
    {
        std::pair<int, int> current = q.front();
        q.pop();
        int cx = current.first;
        int cy = current.second;

        // ������������ĸ�����
        std::vector<std::pair<int, int>> directions = {
            std::make_pair(1, 0),  // ����
            std::make_pair(-1, 0), // ����
            std::make_pair(0, 1),  // ����
            std::make_pair(0, -1)  // ����
        };
        for (std::pair<int, int> direction : directions)
        {
            int nx = cx + direction.first;
            int ny = cy + direction.second;
            if (nx >= 0 && nx < CANVAS_SIZE && ny >= 0 && ny < CANVAS_SIZE && canvas[nx][ny] == oldColor)
            {
                canvas[nx][ny] = currentColor;
                drawPixel(nx, ny); // ������������
                q.push(std::make_pair(nx, ny));
            }
        }
    }
}

int main()
{
    // ��ʼ��ͼ�δ���
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    cleardevice();

    // ��ʼ������Ϊ��ɫ
    for (int i = 0; i < CANVAS_SIZE; ++i)
    {
        for (int j = 0; j < CANVAS_SIZE; ++j)
        {
            canvas[i][j] = WHITE;
        }
    }

    saveState(); // �����ʼ״̬

    drawPalette(); // ��ʼ���Ƶ�ɫ��
    drawTools();   // ��ʼ���ƹ�����
    drawCanvas();  // ��ʼ���ƻ���
    drawStatus();  // ��ʼ���Ƶ�ǰ���ߺ���ɫ״̬

    while (true)
    {
        ExMessage msg = getmessage(EX_MOUSE | EX_KEY);

        if (msg.message == WM_LBUTTONDOWN)
        {
            int x = msg.x, y = msg.y;
            if (y < PALETTE_SIZE)
            {
                // ѡ���ɫ����ɫ
                int colorIndex = x / PALETTE_SIZE;
                const COLORREF colors[] = {
                    RGB(255, 0, 0),     // ��ɫ
                    RGB(255, 165, 0),   // ��ɫ
                    RGB(255, 255, 0),   // ��ɫ
                    RGB(0, 128, 0),     // ��ɫ
                    RGB(0, 0, 255),     // ��ɫ
                    RGB(75, 0, 130),    // ��ɫ
                    RGB(238, 130, 238), // ��ɫ
                    RGB(0, 0, 0),       // ��ɫ
                    RGB(255, 255, 255)  // ��ɫ
                };
                currentColor = colors[colorIndex];
                drawStatus(); // ����״̬��ʾ
            }
            else if (y >= PALETTE_SIZE && y < PALETTE_SIZE + TOOL_SIZE)
            {
                // ѡ�񹤾�
                int toolIndex = x / TOOL_SIZE;
                if (toolIndex == 0)
                    currentTool = PEN;
                else if (toolIndex == 1)
                    currentTool = BUCKET;
                else if (toolIndex == 2)
                    currentTool = ERASER;
                else if (toolIndex == 3 && !history.empty())
                {
                    // ������һ������
                    auto lastState = history.top();
                    history.pop();
                    for (int i = 0; i < CANVAS_SIZE; ++i)
                    {
                        for (int j = 0; j < CANVAS_SIZE; ++j)
                        {
                            canvas[i][j] = lastState[i][j];
                        }
                    }
                    drawCanvas(); // �ػ���������
                }
                drawStatus(); // ����״̬��ʾ
            }
            else
            {
                // �ڻ����ϻ���
                x = (x - TOOL_SIZE * 4) / PIXEL_SIZE;
                y = (y - PALETTE_SIZE) / PIXEL_SIZE;
                if (x < CANVAS_SIZE && y < CANVAS_SIZE && x >= 0 && y >= 0)
                {
                    saveState(); // ���浱ǰ״̬
                    if (currentTool == PEN)
                    {
                        canvas[x][y] = currentColor;
                        drawPixel(x, y); // ���Ƶ�������
                    }
                    else if (currentTool == BUCKET)
                    {
                        COLORREF oldColor = canvas[x][y];
                        bucketFill(x, y, oldColor);
                    }
                    else if (currentTool == ERASER)
                    {
                        canvas[x][y] = WHITE;
                        drawPixel(x, y); // ���Ƶ�������
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
                    drawPixel(x, y); // ���Ƶ�������
                }
                else if (currentTool == ERASER)
                {
                    canvas[x][y] = WHITE;
                    drawPixel(x, y); // ���Ƶ�������
                }
            }
        }
        FlushBatchDraw();
    }

    closegraph();
    return 0;
}

/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������ʵ�ֻ�������*/
// #include <graphics.h>
// #include <conio.h>
// #include <stdio.h>
// #include <stack>
// #include <vector>
// #include <queue>
// #include <utility>
//
//// ���峣��
// const int WINDOW_WIDTH = 1024; // ���ڿ��
// const int WINDOW_HEIGHT = 1074; // ���ڸ߶ȣ�������ɫ�壩
// const int CANVAS_SIZE = 256; // �߼�����ߴ�
// const int PIXEL_SIZE = 4; // ÿ�����ؿ����ʾ�ߴ�
// const int PALETTE_SIZE = 50; // ��ɫ��ߴ�
// const int TOOL_SIZE = 50;//���߰����ߴ�
//
//// ��¼ÿ�����ص���ɫ
// COLORREF canvas[CANVAS_SIZE][CANVAS_SIZE];
// COLORREF currentColor = BLACK; // ��ǰѡ�����ɫ
//
//// ����ջ���ڳ�������
// std::stack<std::vector<std::vector<COLORREF>>> history;
//
//// ��������ö��
// enum Tool { PEN, BUCKET, ERASER };
// Tool currentTool = PEN; // ��ǰѡ��Ĺ���
//
//// ���浱ǰ����״̬
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
//// ���Ƶ�ɫ��
// void drawPalette() {
//     const COLORREF colors[] = {
//         RGB(255, 0, 0), // ��ɫ
//         RGB(255, 165, 0), // ��ɫ
//         RGB(255, 255, 0), // ��ɫ
//         RGB(0, 128, 0), // ��ɫ
//         RGB(0, 0, 255), // ��ɫ
//         RGB(75, 0, 130), // ��ɫ
//         RGB(238, 130, 238), // ��ɫ
//         RGB(0, 0, 0), // ��ɫ
//         RGB(255, 255, 255) // ��ɫ
//     };
//     // ���Ƶ�ɫ��
//     for (int i = 0; i < 9; ++i) {
//         setfillcolor(colors[i]);
//         bar(i * PALETTE_SIZE, 0, (i + 1) * PALETTE_SIZE, PALETTE_SIZE);
//     }
// }
//
//// ���ƻ���
// void drawCanvas() {
//     // ��������ÿ�����أ��Ŵ����
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             setfillcolor(canvas[i][j]);
//             bar(i * PIXEL_SIZE, j * PIXEL_SIZE + PALETTE_SIZE, (i + 1) * PIXEL_SIZE, (j + 1) * PIXEL_SIZE + PALETTE_SIZE);
//         }
//     }
// }
//
//// ����Ͱ����㷨��ʹ�ù������������
// void bucketFill(int x, int y, COLORREF oldColor) {
//     // �߽������ɫ���
//     if (x < 0 || x >= CANVAS_SIZE || y < 0 || y >= CANVAS_SIZE || canvas[x][y] != oldColor || canvas[x][y] == currentColor) {
//         return;
//     }
//
//     std::queue<std::pair<int, int>> q;
//     q.push(std::make_pair(x, y));
//     canvas[x][y] = currentColor;
//
//     // �����������
//     while (!q.empty()) {
//         std::pair<int, int> current = q.front();
//         q.pop();
//         int cx = current.first;
//         int cy = current.second;
//
//         // ������������ĸ�����
//         std::vector<std::pair<int, int>> directions = {
//             std::make_pair(1, 0), // ����
//             std::make_pair(-1, 0), // ����
//             std::make_pair(0, 1), // ����
//             std::make_pair(0, -1) // ����
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
//     // ��ʼ��ͼ�δ���
//     initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
//     cleardevice();
//
//     // ��ʼ������Ϊ��ɫ
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             canvas[i][j] = WHITE;
//         }
//     }
//
//     saveState(); // �����ʼ״̬
//
//     while (true) {
//         drawPalette(); // ���Ƶ�ɫ��
//         drawCanvas(); // ���ƻ���
//         ExMessage msg = getmessage(EX_MOUSE | EX_KEY);
//
//         if (msg.message == WM_LBUTTONDOWN) {
//             int x = msg.x, y = msg.y;
//             if (y < PALETTE_SIZE) {
//                 // ѡ���ɫ����ɫ
//                 int colorIndex = x / PALETTE_SIZE;
//                 const COLORREF colors[] = {
//                     RGB(255, 0, 0), // ��ɫ
//                     RGB(255, 165, 0), // ��ɫ
//                     RGB(255, 255, 0), // ��ɫ
//                     RGB(0, 128, 0), // ��ɫ
//                     RGB(0, 0, 255), // ��ɫ
//                     RGB(75, 0, 130), // ��ɫ
//                     RGB(238, 130, 238), // ��ɫ
//                     RGB(0, 0, 0), // ��ɫ
//                     RGB(255, 255, 255) // ��ɫ
//                 };
//                 currentColor = colors[colorIndex];
//             }
//             else {
//                 // �ڻ����ϻ���
//                 x /= PIXEL_SIZE;
//                 y = (y - PALETTE_SIZE) / PIXEL_SIZE;
//                 if (x < CANVAS_SIZE && y < CANVAS_SIZE) {
//                     saveState(); // ���浱ǰ״̬
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
//                 currentTool = PEN; // �л�Ϊ���ʹ���
//             }
//             else if (msg.vkcode == 'B') {
//                 currentTool = BUCKET; // �л�Ϊ����Ͱ����
//             }
//             else if (msg.vkcode == 'E') {
//                 currentTool = ERASER; // �л�Ϊ��Ƥ������
//             }
//             else if (msg.vkcode == 'Z' && !history.empty()) {
//                 // ������һ������
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
/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������滹�е�����*/
// #include <graphics.h>
// #include <conio.h>
// #include <stdio.h>
// #include <stack>
// #include <vector>
// #include <queue>
// #include <utility>
//
//// ���峣��
// const int WINDOW_WIDTH = 1024; // ���ڿ��
// const int WINDOW_HEIGHT = 1074; // ���ڸ߶ȣ�������ɫ�壩
// const int CANVAS_SIZE = 256; // �߼�����ߴ�
// const int PIXEL_SIZE = 4; // ÿ�����ؿ����ʾ�ߴ�
// const int PALETTE_SIZE = 50; // ��ɫ��ߴ�
// const int TOOL_SIZE = 50;//���߰����ߴ�
//
//// ��¼ÿ�����ص���ɫ
// COLORREF canvas[CANVAS_SIZE][CANVAS_SIZE];
// COLORREF currentColor = BLACK; // ��ǰѡ�����ɫ
//
//// ����ջ���ڳ�������
// std::stack<std::vector<std::vector<COLORREF>>> history;
//
//// ��������ö��
// enum Tool { PEN, BUCKET, ERASER };
// Tool currentTool = PEN; // ��ǰѡ��Ĺ���
//
//// ���浱ǰ����״̬
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
//// ���Ƶ�ɫ��
// void drawPalette() {
//     const COLORREF colors[] = {
//         RGB(255, 0, 0), // ��ɫ
//         RGB(255, 165, 0), // ��ɫ
//         RGB(255, 255, 0), // ��ɫ
//         RGB(0, 128, 0), // ��ɫ
//         RGB(0, 0, 255), // ��ɫ
//         RGB(75, 0, 130), // ��ɫ
//         RGB(238, 130, 238), // ��ɫ
//         RGB(0, 0, 0), // ��ɫ
//         RGB(255, 255, 255) // ��ɫ
//     };
//     // ���Ƶ�ɫ��
//     for (int i = 0; i < 9; ++i) {
//         setfillcolor(colors[i]);
//         bar(i * PALETTE_SIZE, 0, (i + 1) * PALETTE_SIZE, PALETTE_SIZE);
//     }
// }
//
//// ���ƻ���
// void drawCanvas() {
//     // ��������ÿ�����أ��Ŵ����
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             setfillcolor(canvas[i][j]);
//             bar(i * PIXEL_SIZE, j * PIXEL_SIZE + PALETTE_SIZE, (i + 1) * PIXEL_SIZE, (j + 1) * PIXEL_SIZE + PALETTE_SIZE);
//         }
//     }
// }
//
//// ����Ͱ����㷨��ʹ�ù������������
// void bucketFill(int x, int y, COLORREF oldColor) {
//     // �߽������ɫ���
//     if (x < 0 || x >= CANVAS_SIZE || y < 0 || y >= CANVAS_SIZE || canvas[x][y] != oldColor || canvas[x][y] == currentColor) {
//         return;
//     }
//
//     std::queue<std::pair<int, int>> q;
//     q.push(std::make_pair(x, y));
//     canvas[x][y] = currentColor;
//
//     // �����������
//     while (!q.empty()) {
//         std::pair<int, int> current = q.front();
//         q.pop();
//         int cx = current.first;
//         int cy = current.second;
//
//         // ������������ĸ�����
//         std::vector<std::pair<int, int>> directions = {
//             std::make_pair(1, 0), // ����
//             std::make_pair(-1, 0), // ����
//             std::make_pair(0, 1), // ����
//             std::make_pair(0, -1) // ����
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
//     // ��ʼ��ͼ�δ���
//     initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
//     cleardevice();
//
//     // ��ʼ������Ϊ��ɫ
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             canvas[i][j] = WHITE;
//         }
//     }
//
//     saveState(); // �����ʼ״̬
//
//     while (true) {
//         drawPalette(); // ���Ƶ�ɫ��
//         drawCanvas(); // ���ƻ���
//         ExMessage msg = getmessage(EX_MOUSE | EX_KEY);
//
//         if (msg.message == WM_LBUTTONDOWN) {
//             int x = msg.x, y = msg.y;
//             if (y < PALETTE_SIZE) {
//                 // ѡ���ɫ����ɫ
//                 int colorIndex = x / PALETTE_SIZE;
//                 const COLORREF colors[] = {
//                     RGB(255, 0, 0), // ��ɫ
//                     RGB(255, 165, 0), // ��ɫ
//                     RGB(255, 255, 0), // ��ɫ
//                     RGB(0, 128, 0), // ��ɫ
//                     RGB(0, 0, 255), // ��ɫ
//                     RGB(75, 0, 130), // ��ɫ
//                     RGB(238, 130, 238), // ��ɫ
//                     RGB(0, 0, 0), // ��ɫ
//                     RGB(255, 255, 255) // ��ɫ
//                 };
//                 currentColor = colors[colorIndex];
//             }
//             else {
//                 // �ڻ����ϻ���
//                 x /= PIXEL_SIZE;
//                 y = (y - PALETTE_SIZE) / PIXEL_SIZE;
//                 if (x < CANVAS_SIZE && y < CANVAS_SIZE) {
//                     saveState(); // ���浱ǰ״̬
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
//                 currentTool = PEN; // �л�Ϊ���ʹ���
//             }
//             else if (msg.vkcode == 'B') {
//                 currentTool = BUCKET; // �л�Ϊ����Ͱ����
//             }
//             else if (msg.vkcode == 'E') {
//                 currentTool = ERASER; // �л�Ϊ��Ƥ������
//             }
//             else if (msg.vkcode == 'Z' && !history.empty()) {
//                 // ������һ������
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

/*�������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������汾�Ļ�ͼ*/
// #include <graphics.h>
// #include <conio.h>
// #include <stdio.h>
// #include <stack>
// #include <vector>
// #include <queue>
// #include <utility>
//
//// ����һЩ����
// const int WINDOW_WIDTH = 600;
// const int WINDOW_HEIGHT = 400;
// const int CANVAS_SIZE = 256;
// const int PALETTE_SIZE = 50;
//
//// ��¼ÿ�����ص���ɫ
// COLORREF canvas[CANVAS_SIZE][CANVAS_SIZE];
// COLORREF currentColor = BLACK;
//
//// ����ջ���ڳ�������
// std::stack<std::vector<std::vector<COLORREF>>> history;
//
//// ��¼��ǰ��������
// enum Tool { PEN, BUCKET, ERASER };
// Tool currentTool = PEN;
//
//// ���浱ǰ����״̬
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
//// ���Ƶ�ɫ��
// void drawPalette() {
//     for (int i = 0; i < 10; ++i) {
//         setfillcolor(RGB(i * 25, 0, 0));
//         bar(i * PALETTE_SIZE, 0, (i + 1) * PALETTE_SIZE, PALETTE_SIZE);
//     }
// }
//
//// ���ƻ���
// void drawCanvas() {
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             putpixel(i, j, canvas[i][j]);
//         }
//     }
// }
//
//// ����Ͱ����㷨��ʹ�ù������������
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
//         // ������������ĸ�����
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
//     // ��ʼ������Ϊ��ɫ
//     for (int i = 0; i < CANVAS_SIZE; ++i) {
//         for (int j = 0; j < CANVAS_SIZE; ++j) {
//             canvas[i][j] = WHITE;
//         }
//     }
//
//     saveState(); // ��ʼ״̬����
//
//     while (true) {
//         drawPalette();
//         drawCanvas();
//         ExMessage msg = getmessage(EX_MOUSE | EX_KEY);
//         if (msg.message == WM_LBUTTONDOWN) {
//             int x = msg.x, y = msg.y;
//             if (y < PALETTE_SIZE) {
//                 // ѡ���ɫ����ɫ
//                 int colorIndex = x / PALETTE_SIZE;
//                 currentColor = RGB(colorIndex * 25, 0, 0);
//             }
//             else if (x < CANVAS_SIZE && y < CANVAS_SIZE) {
//                 saveState(); // ���浱ǰ״̬
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
//                 // ������һ������
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
//  /*��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������easy��*/
////// ���뻷����Visual C++ 6.0~2022��EasyX_2023�����
////// https://easyx.cn
//////
////#include <graphics.h>
////#include <conio.h>
////#include <stdio.h>
////
/////*
////��1�� ���壺һ��256 �� 256 ���ػ����Ļ���
////��2�� ��ɫ��ͻ��ʣ���������ѡ��Ӧ��ǰ����ɫ��
////��3�� ���ʹ��ߣ����������ʵ���ڻ�������Ӧ������ʵ�ֶ���Ӧ������ɫ��
////��4�� ����Ͱ���ߣ�ʹ��ǰ��ɫ�����ɫ���������ʵ�ֶ�������������ʵ��һ������ɫ����
////��5�� ��Ƥ��������Ӧ�����ص���ɫ������
////��6�� ������������һ��������
////*/
////
////
////int main()
////{
////	// ��ʼ��ͼ�δ���
////	initgraph(640, 480);
////
////	ExMessage m;		// ������Ϣ����
////
////	while (true)
////	{
////		// ��ȡһ�����򰴼���Ϣ
////		m = getmessage(EX_MOUSE | EX_KEY);
////
////		switch (m.message)
////		{
////		case WM_MOUSEMOVE:
////			// ����ƶ���ʱ�򻭺�ɫ��С��
////			putpixel(m.x, m.y, RED);
////			break;
////
////		case WM_LBUTTONDOWN:
////			// ����������ͬʱ������ Ctrl ��
////			if (m.ctrl)
////				// ��һ���󷽿�
////				rectangle(m.x - 10, m.y - 10, m.x + 10, m.y + 10);
////			else
////				// ��һ��С����
////				rectangle(m.x - 5, m.y - 5, m.x + 5, m.y + 5);
////			break;
////
////		case WM_KEYDOWN:
////			if (m.vkcode == VK_ESCAPE)
////				return 0;	// �� ESC ���˳�����
////		}
////	}
////
////	// �ر�ͼ�δ���
////	closegraph();
////	return 0;
////}
