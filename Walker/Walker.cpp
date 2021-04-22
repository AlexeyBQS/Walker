#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>

using namespace std;


// Список методов
void Setup(); // Установка параметров
void SettingConsoleWindow(); // Настройка окна консоли
void SettingConsoleFont(); // Настройка шрифта консоли
void DisableVisibleConsoleCursor(); // Отключение видимости курсора консоли
void SetPositionCursor(int x, int y); // Установка позиции курсора для отрисоки элементов
void ClearMap(); // Зачистка карты он "мусора"
void GenMap(); // Генерация карты
void Logic(); // Логика
void DrawObject(); // Отрисовка объектов
void Draw(); // Отрисовка карты
void Input(); // Получение нажатий с клавиатуры
char CheckInputChar(char inputChar); // Поддержка всех символов при нажатии (QqЙй)
void StartGame(); // Запуск игры


const LPCWSTR ConsoleTitle = L"Бродилка v0.03 Build 1"; // Наименование окна консоли приложения
const LPCWSTR FontFamily = L"Lucida Console"; // Шрифт отрисовки
const short FontSize = 25; // Размер шрифта отрисовки

short delay = 45; // Задержка отрисовки
bool gameOver = false; // Выход из приложения

const short width = 120; // Ширина окна
const short height = 15; // Высота окна

char visibleMap[width][height]; // Видимая часть карты

bool jumpBlock; // Блокировка возможности прыжка
short jumpHeight; // Высота прыжка

bool jump; // Состояние персонажа (прыжок)
short x; // Позиция персонажа по горизонтали
short y; // Позиция персонажа по вертикали
short xOld; // Предыдущая позиция персонажа по горизонтали
short yOld; // Предыдущая позиция персонажа по вертикали

char direction[] = "wasd"; // Передвижение
short choiseDirection; // Поиск нажатой клавиши

const char imageEmpty = ' '; // Изображение пустоты
const char imageBlock = 'Z'; // Изображение блока земли
const char imagePerson = '0'; // Изображение персонажа

// Установка параметров
void Setup() { 
    jump = false;
    jumpBlock = true;
    x = width / 2;
    y = 0;
    jumpHeight = 6;

    DisableVisibleConsoleCursor();
    SettingConsoleWindow();
    SettingConsoleFont();
    ClearMap();
    GenMap();
    Draw();
}

// Настройка окна консоли
void SettingConsoleWindow() {
    SetConsoleTitleW(ConsoleTitle);
    HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD crd = { width, height + 1 };
    SMALL_RECT src = { 0, 0, crd.X - 1, crd.Y - 1 };
    SetConsoleWindowInfo(out_handle, true, &src);
    SetConsoleScreenBufferSize(out_handle, crd);
}

// Настройка шрифта консоли
void SettingConsoleFont() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(fontInfo);
    GetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);
    wcscpy(fontInfo.FaceName, FontFamily);
    fontInfo.dwFontSize.Y = FontSize;
    SetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);
}

// Отключение видимости курсора консоли
void DisableVisibleConsoleCursor() {
    CONSOLE_CURSOR_INFO curs = { 0 };
    curs.dwSize = sizeof(curs);
    curs.bVisible = FALSE;
    ::SetConsoleCursorInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &curs);
}

// Установка позиции курсора для отрисоки элементов
void SetPositionCursor(int x, int y) {
    COORD position;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(hConsole, position);
}

// Зачистка карты он "мусора"
void ClearMap() {
    for (int w = 0; w < width; w++) {
        for (int h = 0; h < height; h++) {
            visibleMap[w][h] = imageEmpty;
        }
    }
}

// Генерация карты
void GenMap() {
    int gen = 0;
    //int genOld = 0;
    for (int w = 0; w < width; w++) {
        gen = 1 + rand() % (height / 2 - 1);

        for (int h = 0; h < gen; h++) {
            visibleMap[w][height - 1 - h] = imageBlock;
        }
    }
}

// Логика
void Logic() {
    if (jump && !jumpBlock) {
        y -= jumpHeight;
        jump = false;
        jumpBlock = true;
    }
    else {
        if (visibleMap[x][y + 1] != imageBlock) {
            y++;
        }
        else {
            jumpBlock = false;
        }
    }
}

// Отрисовка объектов
void DrawObject() {
    if (xOld != x || yOld != y) {
        visibleMap[xOld][yOld] = imageEmpty;
    }

    xOld = x;
    yOld = y;

    visibleMap[x][y] = imagePerson;
}

// Отрисовка карты
void Draw() {
    DrawObject();

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            SetPositionCursor(w, h);
            cout << visibleMap[w][h];
        }
    }
}

// Получение нажатий с клавиатуры
void Input() {
    if (_kbhit()) {
        char inputKey = CheckInputChar(_getch());

        int length = strlen(direction);
        for (int i = 0; i < length && choiseDirection == NULL; i++) {
            if (direction[i] == inputKey) {
                choiseDirection = i;
            }
        }

        if (choiseDirection != NULL)
        {
            switch (direction[choiseDirection]) {
            case 's':

                break;
            case 'a':
                if (x >= 0 && visibleMap[x - 1][y] != imageBlock) {
                    x--;
                }
                break;
            case 'd':
                if (x < width - 1 && visibleMap[x + 1][y] != imageBlock) {
                    x++;
                }
                break;
            }

            if (!_kbhit()) choiseDirection = NULL;
        }

        switch (inputKey) {
        case 'w':
            jump = true;
            break;

        case ' ':

            break;

        case 'q':
            gameOver = true;
            break;
        }
    }
}

// Поддержка всех символов при нажатии (QqЙй)
char CheckInputChar(char iChar) {
    const short size = 35;
    const char keyUS_Low[] = "qwertyuiop[]asdfghjkl;'\\zxcvbnm,./";
    const char keyUS_Up[] = "QWERTYUIOP[]ASDFGHJKL;'\\ZXCVBNM,./";
    const char keyRUS_Low[] = "йцукенгшщзхъфывапролджэ\\ячсмитьбю.";
    const char keyRUS_Up[] = "ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭ\\ЯЧСМИТЬБЮ.";

    for (int i = 0; i < size; i++) {
        if (iChar == keyUS_Low[i] || iChar == keyUS_Up[i] || iChar == keyRUS_Low[i] || iChar == keyRUS_Up[i]) {
            return keyUS_Low[i];
        }
    }

    return iChar;
}

// Запуск игры
void StartGame() {
    Setup();

    while (!gameOver) {
        Input();
        Logic();
        DrawObject();
        Draw();
        Sleep(delay);
    }
}

// Точка входа в программу
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(NULL));

    StartGame();

    system("pause");

    return 0;
}