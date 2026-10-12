#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#define SIZE 20

struct Segment {
    int x;
    int y;
    Segment* next;
};

class Snake {
private:
    Segment* head;
    Segment* tail;
    int length;
    int foodX;
    int foodY;
    int napr;
    int score;
    bool game;
public:
    Snake() {
        head = nullptr;
        tail = nullptr;
        length = 0;
        foodX = 0;
        foodY = 0;
        napr = 1;
        score = 0;
        game = true;
    }

    ~Snake(){
        clear();
    }

    void clear() {
        Segment* cur = head;
        while (cur != nullptr) {
            Segment* temp = cur;
            cur = cur->next;
            delete temp;
        }
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    void addHead(int x, int y) {
        Segment* s = new Segment;
        s->x = x;
        s->y = y;
        s->next = head;
        head = s;
        if (tail == nullptr) {
            tail = s;
        }
        length++;
    }

    bool isSegIn(int x, int y, bool skipHead) {
        Segment* cur = head;
        if (skipHead && cur != nullptr) {
            cur = cur->next;
        }
        while (cur != nullptr) {
            if (cur->x == x && cur->y == y) return true;
            cur = cur->next;
        }
        return false;
    }
    void newGame() {
        clear();
        score = 0;
        napr = 1;
        game = true;
        int cx = SIZE / 2;
        int cy = SIZE / 2;
        addHead(cx - 2, cy);
        addHead(cx - 1, cy);
        addHead(cx, cy);
        generateFood();
    }
    void generateFood() {
        bool ok = false;
        while (!ok) {
            foodX = rand() % SIZE;
            foodY = rand() % SIZE;
            if (!isSegIn(foodX, foodY, false)) {
                ok = true;
            }
        }
    }
    void print() {
        system("cls");
        std::cout << "Счет: " << score << std::endl;
        std::cout << "WASD - движение, E - сохранить, Q - выход\n\n";
        for (int i = 0; i < SIZE + 2; i++) std::cout << "-";
        std::cout << std::endl;

        for (int y = 0; y < SIZE; y++) {
            std::cout << "-";
            for (int x = 0; x < SIZE; x++) {
                if (head != nullptr && head->x == x && head->y == y) {
                    std::cout << "O";
                }
                else if (isSegIn(x, y, true)) {
                    std::cout << "o";
                }
                else if (x == foodX && y == foodY) {
                    std::cout << "$";
                }
                else {
                    std::cout << " ";
                }
            }
            std::cout << "-\n";
        }
        for (int i = 0; i < SIZE + 2; i++) std::cout << "-";
        std::cout << std::endl;
    }

    char Upr() {
        if (!_kbhit()) return 0;

        char c = _getch();

        if ((c == 'w' || c == 'W') && napr != 2)napr = 0;
        else if ((c == 'd' || c == 'D') && napr != 3) napr = 1;
        else if ((c == 's' || c == 'S') && napr != 0) napr = 2;
        else if ((c == 'a' || c == 'A') && napr != 1) napr = 3;

        if (c == 'q' || c == 'Q') return 'Q';
        if (c == 'p' || c == 'P') return 'S';
        return 0;
    }


};


int main()
{

    int answ;
    char pole[20][20];
    std::ofstream file("data.bin", std::ios::binary);
    std::ofstream rec("rec.bin", std::ios::binary);
    Snake snake;
    
    

    while (answ != 4) {
        std::cout << "1. Новая игра" << std::endl;
        std::cout << "2. Загрузить игру" << std::endl;
        std::cout << "3. Таблица рекордов" << std::endl;
        std::cout << "4. Выход" << std::endl;
        std::cin >> answ;
        

    }
}

