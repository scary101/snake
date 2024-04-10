#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include<Windows.h>
#include<stralign.h>
#include<list>
#include<C:\Users\user\CppLibary\json.hpp>
#include <fstream>
#include <utility>

using json = nlohmann::json;
using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;
const char SNAKE_BODY = '*';
const char FOOD = '@';

enum Direction { UP, DOWN, LEFT, RIGHT };

class User {
private:
    string name;
    int score;
public:
    User(string name, int score) {
        this->name = name;
        this->score = score;
    }
    string getName() const {
        return this->name;
    }
    void setName(string name) {
        this->name = name;
    }
    int getScore() const {
        return this->score;
    }
    void setScore(int score) {
        this->score = score;
    }
};

class SerDeser {
private:
    string path = "C:\\Users\\user\\Desktop\\ReserveTable\\userScore.json";
public:
    void SerData(User user) {
        bool filecheck;
        vector<User> users = DeserData();
        users.push_back(user);
        ofstream file_out(path);



        if (file_out.is_open()) {
            for (const auto& user : users) {
                file_out << "{ \"name\": \"" << user.getName() << "\", \"score\": " << user.getScore() << " }" << endl;
            }
        }

    }
    vector<User> DeserData() {
        vector<User> users;
        ifstream file_in(path);
        string line;

        while (getline(file_in, line)) {
            string name;
            int score;
            json parced_data = json::parse(line);
            name = parced_data["name"].get<string>();
            score = parced_data["score"].get<int>();

            users.push_back(User(name, score));
        }
        return users;
    }
    bool fileNotEmpty(const std::string& filename) {
        ifstream file(path);
        string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return !content.empty();
    }

};

class Snake {
private:
    vector<pair<int, int>> body;
    Direction dir;

public:
    Snake() {
        body.push_back(make_pair(WIDTH / 2, HEIGHT / 2));
        dir = RIGHT;
    }

    void moving() {
        pair<int, int> nextPos = body[0];

        switch (dir) {
        case UP:
            nextPos.second--;
            break;
        case DOWN:
            nextPos.second++;
            break;
        case LEFT:
            nextPos.first--;
            break;
        case RIGHT:
            nextPos.first++;
            break;
        }

        body.insert(body.begin(), nextPos);
        body.pop_back();
    }

    bool checkCollision() {
        pair<int, int> head = body[0];

        if (head.first < 0 || head.first >= WIDTH || head.second < 0 || head.second >= HEIGHT)
            return true;

        for (size_t i = 1; i < body.size(); ++i) {
            if (head == body[i])
                return true;
        }

        return false;
    }

    void setDirection(Direction newDir) {
        if (dir == UP && newDir == DOWN || dir == DOWN && newDir == UP ||
            dir == LEFT && newDir == RIGHT || dir == RIGHT && newDir == LEFT)
            return;

        dir = newDir;
    }

    void eatFood() {
        body.push_back(body.back());
    }

    vector<pair<int, int>> getBody() {
        return body;
    }
};

class Game {
private:
    Snake snake;
    pair<int, int> food;
    int score;
    string name;
    bool gameOver;

public:
    Game() {
        srand(time(nullptr));
        score = 0;
        spawnFood();
    }

    void setname(string name) {
        this->name = name;
    }

    void spawnFood() {
        food = make_pair(rand() % WIDTH, rand() % HEIGHT);
    }

    void board() {
        system("cls");

        vector<pair<int, int>> snakeBody = snake.getBody();

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (x == food.first && y == food.second)
                    cout << FOOD;
                else {
                    bool isSnakeBody = false;
                    for (const auto& part : snakeBody) {
                        if (x == part.first && y == part.second) {
                            cout << SNAKE_BODY;
                            isSnakeBody = true;
                            break;
                        }
                    }
                    if (!isSnakeBody)
                        cout << ' ';
                }
            }
            cout << endl;
        }

        cout << "Яблок: " << score << endl;
    }

    void update() {
        snake.moving();

        if (snake.checkCollision()) {
            gameOver = true;

        }

        if (snake.getBody()[0] == food) {
            snake.eatFood();
            spawnFood();
            score++;
        }
    }

    void klavisha() {
        if (_kbhit()) {
            switch (_getch()) {
            case 'w':
                snake.setDirection(UP);
                break;
            case 's':
                snake.setDirection(DOWN);
                break;
            case 'a':
                snake.setDirection(LEFT);
                break;
            case 'd':
                snake.setDirection(RIGHT);
                break;
            case 'q':
                gameOver = true;
            }
        }
    }

    void SaveScore() {
        SerDeser save;
        User user = User(name, score);
        save.SerData(user);
    }

    void run() {
        while (true) {
            board();
            klavisha();
            update();
            if (gameOver) { 
                cout << "Вы проудли! Яблок схавано: " << score << endl;
                SaveScore();
                system("cls");
                break; 
            }
            Sleep(100);
        }
    }

};

int main() {
    setlocale(LC_ALL, "RUS");
    string userName;
    int action;
    bool isRun = false;
    char key;

    Game game;
    SerDeser load;
    vector<User> users;

    cout << "Введите ваше поганяло:" << endl;
    cin >> userName;
    system("cls");
    game.setname(userName);
    while (!isRun) {
        cout << "1. Начать игру\n" << "2. Посмотреть таблицу игроков\n" << "3. Выйти из игры" << endl;
        cin >> action;

        switch (action)
        {
        case 1:
            game.run();
            break;
        case 2:
            system("cls");
            users = load.DeserData();
            for (User user : users) {
                cout << user.getName() << "\t" << user.getScore() << endl;
            }
            cout << "Для того, чтобы выйти, введите \"ESC\"" << endl;
            while (true) {
                if (_kbhit()) {
                    key = _getch();
                    if (key == 27) {
                        system("cls");
                        break;
                    }
                }
            }
            break;
        case 3:
            isRun = true;
            break;
        default:
            break;
        }
    }
    return 0;
}