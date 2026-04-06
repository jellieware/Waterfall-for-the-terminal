#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Use 2 spaces to create one square "pixel"
#define PIXEL_WIDTH 2 
#define SHADES 30

int main() {
    initscr();
    noecho();
    curs_set(0);
    timeout(40); // Controls the flow speed (~25 FPS)
    start_color();
    srand(time(NULL));

    // 1. Initialize 30 shades of blue gradient
    // ncurses colors range 0-1000.
    for (int i = 0; i < SHADES; i++) {
        init_color(i + 1, 0, (i * 10), 400 + (i * 20)); 
        init_pair(i + 1, i + 1, i + 1);
    }
    // Turbulence color (White)
    init_pair(SHADES + 1, COLOR_WHITE, COLOR_WHITE);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int grid_w = cols / PIXEL_WIDTH;
    
    // Water grid stores color intensity (0-29)
    int grid[rows][grid_w];
    for(int y=0; y<rows; y++)
        for(int x=0; x<grid_w; x++) grid[y][x] = 0;

    while (getch() != 'q') {
        // 2. Physics: Generate new water at the top
        for (int x = 0; x < grid_w; x++) {
            grid[0][x] = rand() % SHADES;
        }

        // 3. Physics: Gravity & Diffusion
        for (int y = rows - 1; y > 0; y--) {
            for (int x = 0; x < grid_w; x++) {
                // Random horizontal drift creates "depth" and wave movement
                int drift = (rand() % 3) - 1; // -1, 0, 1
                int nx = x + drift;
                if (nx < 0) nx = 0;
                if (nx >= grid_w) nx = grid_w - 1;

                grid[y][nx] = grid[y-1][x];
            }
        }

        // 4. Render the grid
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < grid_w; x++) {
                int color_idx = grid[y][x];

                // Add white turbulence at the bottom for "foam"
                if (y > rows - 4 && color_idx > (SHADES / 2)) {
                    attron(COLOR_PAIR(SHADES + 1));
                } else {
                    attron(COLOR_PAIR(color_idx + 1));
                }

                mvaddstr(y, x * PIXEL_WIDTH, "  "); // The "Double Solid Block"
            }
        }
        refresh();
    }

    endwin();
    return 0;
}
