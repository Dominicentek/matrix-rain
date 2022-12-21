#include "console_utils.c"

#define HEIGHT 16

typedef struct Letter Letter;
struct Letter {
    char letter;
    int x;
    int y;
    int prevY;
};

int main() {
    setup_console();
    printf("Resize your terminal then press any key");
    getch();
    int width = console_width();
    int height = console_height();
    Letter* letters = (Letter*)malloc(sizeof(Letter) * width);
    for (int i = 0; i < width; i++) {
        letters[i].x = i;
        letters[i].y = height + HEIGHT;
    }
    background_color_24(0, 0, 0);
    clear_screen(ERASE_ALL);
    while (1) {
        for (int i = 0; i < width; i++) {
            letters[i].prevY = letters[i].y;
            if (letters[i].y == height + HEIGHT) {
                if (rand() % height == 0) {
                    letters[i].letter = (char)((rand() % (126 - 32 + 1)) + 32);
                    letters[i].y = 0;
                }
            }
            else letters[i].y++;
        }
        for (int i = 0; i < width; i++) {
            for (int j = 0; j <= HEIGHT; j++) {
                int y = letters[i].prevY - HEIGHT + j;
                if (y < 0 || y >= height) continue;
                move_absolute(letters[i].x, letters[i].prevY - HEIGHT + j);
                foreground_color_24(0, (char)((float)j / (float)HEIGHT * 255), 0);
                printf("%c", letters[i].letter);
            }
        }
    }
}