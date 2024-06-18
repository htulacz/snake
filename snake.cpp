#include <ncurses.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <algorithm>
#include "options.h"
bool run = true;

using namespace std;

int main(){

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
        if (menuOption.length() > maxOptionLength){
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
    while (run){
        wclear(stdscr);
        mvprintw(1, 12, "Menu");
        for (size_t i = 0; i < menuOption.size(); i++) {
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
            if (selected == 3){
                run = false;
            } else {

            }
        }
    }
    endwin();
    return 0;
}