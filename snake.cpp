#include <ncurses.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <unistd.h>
#include <time.h>

bool run = true;
bool snakeRun = true;

using namespace std;

struct Position {
    int x, y;

    bool operator==(const Position &other) const {
        return x == other.x && y == other.y;
    }
};

void drawBorder(int width, int height) {
    for (int i = 0; i < width; ++i) {
        mvprintw(0, i, "#");
        mvprintw(height - 1, i, "#");
    }
    for (int i = 0; i < height; ++i) {
        mvprintw(i, 0, "#");
        mvprintw(i, width - 1, "#");
    }
}

void generateFood(Position &food, int width, int height, const deque<Position> &snake) {
    bool valid = false;
    while (!valid) {
        valid = true;
        food.x = rand() % (width - 2) + 1;
        food.y = rand() % (height - 2) + 1;
        for (const auto &pos : snake) {
            if (pos == food) {
                valid = false;
                break;
            }
        }
    }
}

int main() {
    srand(time(0));

    ifstream menuFile("MenuOptions.txt");
    if (!menuFile) {
        cerr << "Unable to open MenuOptions.txt" << endl;
        return 1;
    }

    vector<string> menuOptions;
    string menuOption;
    size_t maxOptionLength = 0;
    while (getline(menuFile, menuOption)) {
        menuOptions.push_back(menuOption);
        if (menuOption.length() > maxOptionLength) {
            maxOptionLength = menuOption.length();
        }
    }
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    int input = 0;
    int selected = 1;
    while (run) {
        wclear(stdscr);
        mvprintw(1, 12, "Menu");
        for (size_t i = 0; i < menuOptions.size(); i++) {
            mvprintw(i + 2, 15, "%zu. %s", i + 1, menuOptions[i].c_str());
        }
        if (selected > 0 && selected <= menuOptions.size()) {
            mvchgat(selected + 1, 15, maxOptionLength + 3, A_STANDOUT, 1, NULL);
        }
        refresh();

        input = getch();
        if (input == KEY_DOWN && selected < menuOptions.size()) selected++;
        if (input == KEY_UP && selected > 1) selected--;

        if (input == 10) {
            if (selected == menuOptions.size()) {
                run = false;
            } else {
                int width = 50;
                int height = 20;
                Position food;
                deque<Position> snake = {{width / 2, height / 2}};
                Position direction = {0, 1};
                generateFood(food, width, height, snake);

                struct timespec sleepTime;
                sleepTime.tv_sec = 0;
                sleepTime.tv_nsec = 500000000L;

                time_t startTime = time(nullptr);

                snakeRun = true;
                while (snakeRun) {
                    nodelay(stdscr, TRUE);
                    input = getch();
                    switch (input) {
                        case KEY_UP: if (direction.y != 1) direction = {0, -1}; break;
                        case KEY_DOWN: if (direction.y != -1) direction = {0, 1}; break;
                        case KEY_LEFT: if (direction.x != 1) direction = {-1, 0}; break;
                        case KEY_RIGHT: if (direction.x != -1) direction = {1, 0}; break;
                    }

                    Position newHead = {snake.front().x + direction.x, snake.front().y + direction.y};

                    if (newHead.x <= 0 || newHead.x >= width - 1 || newHead.y <= 0 || newHead.y >= height - 1 ||
                        find(snake.begin(), snake.end(), newHead) != snake.end()) {
                        snakeRun = false;
                        break;
                    }

                    snake.push_front(newHead);
                    if (newHead == food) {
                        generateFood(food, width, height, snake);
                    } else {
                        snake.pop_back();
                    }

                    wclear(stdscr);
                    drawBorder(width, height);
                    mvprintw(food.y, food.x, "*");

                    for (const auto &pos : snake) {
                        mvprintw(pos.y, pos.x, "O");
                    }

                    time_t currentTime = time(nullptr);
                    int elapsedTime = static_cast<int>(difftime(currentTime, startTime));
                    mvprintw(0, width + 2, "Time: %d sec", elapsedTime);

                    refresh();

                    nanosleep(&sleepTime, nullptr);
                }
                timeout(-1);
                nodelay(stdscr, FALSE);
            }
        }
    }
    endwin();
    return 0;
}
