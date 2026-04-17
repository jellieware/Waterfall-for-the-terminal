#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_DROPS 400

typedef struct {
    int x, y;
    int speed;
    int length;
} Drop;

int main() {
    // Initialize ncurses
    initscr();
    noecho();
    curs_set(0);
    start_color();
    timeout(20); // Frame delay in ms

    // Define blue shades using 256-color palette (16-231 range)
    // init_pair(id, foreground, background)
    init_pair(1, 17, 17);   // Darkest Blue
    init_pair(2, 18, 18);   // Deep Blue
    init_pair(3, 19, 19);   // Blue
    init_pair(4, 20, 20);   // Lighter Blue
    init_pair(5, 21, 21);   // Bright Blue
    init_pair(6, 33, 33);   // Electric Blue (Leading edge)

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int cols = max_x / 2; // Each pixel is 2 characters wide

    srand(time(NULL));
    Drop drops[MAX_DROPS];

    // Initial drop placement
    for (int i = 0; i < MAX_DROPS; i++) {
        drops[i].y = rand() % max_y;
        drops[i].x = rand() % cols;
        drops[i].speed = (rand() % 2) + 1;
        drops[i].length = (rand() % 12) + 6;
    }

    while (getch() == ERR) {
        erase();
        
        for (int i = 0; i < MAX_DROPS; i++) {
            for (int l = 0; l < drops[i].length; l++) {
                int curr_y = drops[i].y - l;
                
                if (curr_y >= 0 && curr_y < max_y) {
                    // Create shading/shadow effect based on position in trail
                    int color = 6 - (l * 6 / drops[i].length);
                    if (color < 1) color = 1;
                    
                    attron(COLOR_PAIR(color));
                    mvaddstr(curr_y, drops[i].x * 2, "  "); // Two solid blocks
                    attroff(COLOR_PAIR(color));
                }
            }

            // Move drop down
            drops[i].y += drops[i].speed;

            // Reset drop if it falls off screen
            if (drops[i].y - drops[i].length > max_y) {
                drops[i].y = 0;
                drops[i].x = rand() % cols;
                drops[i].speed = (rand() % 2) + 1;
            }
        }
        refresh();
    }

    endwin();
    return 0;
}
