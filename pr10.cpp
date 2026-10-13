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
        for (int i = 0; i < SIZE + 2; i++) std::cout << "#";
        std::cout << std::endl;

        for (int y = 0; y < SIZE; y++) {
            std::cout << "#";
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
            std::cout << "#\n";
        }
        for (int i = 0; i < SIZE + 2; i++) std::cout << "#";
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
        if (c == 'e' || c == 'E') return 'S';
        return 0;
    }

    void moveTailToHead(int x, int y) {
        if (head == nullptr) return;

        if (head == tail) {
            head->x = x;
            head->y = y;
            return;
        }

        Segment* beforeTail = head;
        while (beforeTail->next != tail) {
            beforeTail = beforeTail->next;
        }

        Segment* oldTail = tail;
        beforeTail->next = nullptr;
        tail = beforeTail;

        oldTail->x = x;
        oldTail->y = y;
        oldTail->next = head;
        head = oldTail;
    }

    void step() {
        int x = head->x;
        int y = head->y;
        switch (napr) {
        case 0: {
            y--;
            break;
        }
        case 1: {
            x++;
            break;
        }
        case 2: {
            y++;
            break;
        }
        case 3: {
            x--;
            break;
        }
        }
        
        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
            game = false;
            return;
        }
        if (isSegIn(x, y, false)) {
            game = false;
            return;
        }
        if (x == foodX && y == foodY) {
            addHead(x, y);
            score += 10;
            generateFood();
        }
        else {
            moveTailToHead(x, y);
        }
        
    }
    bool run() {
        while (game) {
            print();
            char action = Upr();

            if (action == 'Q') return false;
            if (action == 'S') {
                saveGame("save.bin");
                std::cout << "\nИгра сохранена\n";
                std::cout << "Нажмите любую клавишу...\n";
                _getch();
                return true;
            }
            step();
            Sleep(200);
        }
        print();
        std::cout << "\nИГРА ОКОНЧЕНА\n";
        std::cout << "Ваш счёт: " << score << "\n";

        std::cout << "Введите имя: ";
        char name[50];
        std::cin >> name;
        addRecord(name, score);

        std::cout << "Нажмите любую клавишу...";
        _getch();
        return false;
    }

    void saveGame(const char* fileN) {
        std::ofstream file(fileN, std::ios::binary);
        if (!file) {
            std::cout << "Ошибка открытия файла";
            return;
        }
        file.write(reinterpret_cast<const char*>(&score), sizeof(score));
        file.write(reinterpret_cast<const char*>(&napr), sizeof(napr));
        file.write(reinterpret_cast<const char*>(&foodX), sizeof(foodX));
        file.write(reinterpret_cast<const char*>(&foodY), sizeof(foodY));
        file.write(reinterpret_cast<const char*>(&length), sizeof(length));
        Segment* cur = head;
        while (cur != nullptr) {
            file.write(reinterpret_cast<const char*>(&cur->x), sizeof(cur->x));
            file.write(reinterpret_cast<const char*>(&cur->y), sizeof(cur->y));
            cur = cur->next;
        }
        file.close();
    }

    bool loadGame(const char* fileN) {
        std::ifstream file(fileN, std::ios::binary);
        if (!file)return false;
        clear();

        file.read(reinterpret_cast<char*>(&score), sizeof(score));
        file.read(reinterpret_cast< char*>(&napr), sizeof(napr));
        file.read(reinterpret_cast< char*>(&foodX), sizeof(foodX));
        file.read(reinterpret_cast< char*>(&foodY), sizeof(foodY));
        file.read(reinterpret_cast< char*>(&length), sizeof(length));

        for (int i = 0; i < length; i++) {
            int x;
            int y;
            file.read(reinterpret_cast<char*>(&x), sizeof(x));
            file.read(reinterpret_cast<char*>(&y), sizeof(y));
            
            Segment* s = new Segment;
            s->x = x;
            s->y = y;
            s->next = nullptr;

            if (head == nullptr) {
                head = s;
                tail = s;
            }
            else {
                tail->next = s;
                tail = s;
            }
        }
        file.close();
        game = true;
        return true;
    }

    struct Record {
        char name[50];
        int score;
    };

    void addRecord(const char* name, int sc) {
        Record rec[10];
        int count = 0;

        std::ifstream in("records.bin", std::ios::binary);
        if (in) {
            in.seekg(0, std::ios::end);
            std::streamsize fileSize = in.tellg();
            in.seekg(0, std::ios::beg);
            count = fileSize / sizeof(Record);
            if (count > 10)count = 10;

            for (int i = 0; i < count; i++) {
                in.read(reinterpret_cast<char*>(&rec[i]), sizeof(Record));
            }
            in.close();
        }
        Record newR;
        int i = 0;
        while (name[i] != '\0' && i < 49) {
            newR.name[i] = name[i];
            i++;
        }
        newR.name[i] = '\0';
        newR.score = sc;
        rec[count] = newR;
        count++;

        std::ofstream out("records.bin", std::ios::binary);
        if (!out) return;
        for (int j = 0; j < count; j++) {
            out.write(reinterpret_cast<const char*>(&rec[j]), sizeof(Record));
        }
        out.close();
    }

    void showRec() {
        std::ifstream in("records.bin", std::ios::binary);
        if (!in) {
            std::cout << "Таблица рекордов пуста\n";
            return;
        }
        in.seekg(0, std::ios::end);
        std::streamsize fileSize = in.tellg();
        in.seekg(0, std::ios::beg);
        int count = fileSize / sizeof(Record);

        if (count == 0) {
            std::cout << "Таблица рекордов пуста\n";
            in.close();
            return;
        }

        std::cout << "\nТАБЛИЦА РЕКОРДОВ\n";
        for (int i = 0; i < count; i++) {
            Record r;
            in.read(reinterpret_cast<char*>(&r), sizeof(Record));
            std::cout << i + 1 << ". " << r.name << " - " << r.score << "\n";
        }
        in.close();
    }
};


int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));

    Snake game;
    int answ = 0;

    while (answ != 4) {
        system("cls");
        std::cout << "1. Новая игра" << std::endl;
        std::cout << "2. Загрузить игру" << std::endl;
        std::cout << "3. Таблица рекордов" << std::endl;
        std::cout << "4. Выход" << std::endl;
        std::cin >> answ;
        
        switch (answ) {
            case 1:{
                game.newGame();
                game.run();
                break;
            }
            case 2: {
                if (game.loadGame("save.bin")) {
                    std::cout << "Игра загружена, нажмите любую клавишу...\n";
                    _getch();
                    game.run();
                }
                else {
                    std::cout << "Файл сохранения не найден, нажмите любую клавишу...\n";
                    _getch();
                }
                break;
            }
            case 3: {
                game.showRec();
                std::cout << "Нажмите любую клавишу...";
                _getch();
                break;
            }
            case 4: {
                std::cout << "Выход\n";
                break;
            }
            default: {
                std::cout << "Неверный ответ\nНажмите любую клавишу...";
                _getch();
            }
        }
    }
}

