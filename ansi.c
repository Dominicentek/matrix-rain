#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define ERASE_CURSOR_TO_END 0
#define ERASE_CURSOR_TO_START 1
#define ERASE_ALL 2

#define COLOR_BLACK 0
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_ORANGE 3
#define COLOR_BLUE 4
#define COLOR_PURPLE 5
#define COLOR_CYAN 6
#define COLOR_GRAY 7
#define COLOR_DARK_GRAY 8
#define COLOR_PINK 9
#define COLOR_LIME 10
#define COLOR_YELLOW 11
#define COLOR_LIGHT_BLUE 12
#define COLOR_MAGENTA 13
#define COLOR_AQUA 14
#define COLOR_WHITE 15
#define COLOR_DEFAULT (-1)

static void ansi_raw(char* code) {
	printf("\e[%s", code);
}
static void ansi(char letter, int paramcount, int* params) {
	char* string = (char*)malloc(1024);
	string[0] = 0;
	for (int i = 0; i < paramcount; i++) {
		char* buff = (char*)malloc(1024);
		sprintf(buff, "%d", params[i]);
		strcat(string, buff);
		if (i + 1 < paramcount) strcat(string, ";");
	}
	strncat(string, &letter, 1);
	ansi_raw(string);
}
static void ansi_formatting(int command, int paramcount, const int* params) {
	paramcount++;
	int* paramfinal = (int*)malloc(paramcount * sizeof(int));
	paramcount--;
	paramfinal[0] = command;
	for (int i = 0; i < paramcount; i++) {
		paramfinal[i + 1] = params[i];
	}
	ansi('m', paramcount + 1, paramfinal);
	free(paramfinal);
}
void scroll_up(int amount) {
	ansi('S', 1, &amount);
}
void scroll_down(int amount) {
	ansi('T', 1, &amount);
}
void move_absolute(int x, int y) {
	ansi('f', 2, (int[]){y + 1, x + 1});
}
void move_relative(int x, int y) {
	if (x < 0) ansi('D', 1, (int[]){-x});
	if (x > 0) ansi('C', 1, &x);
	if (y < 0) ansi('A', 1, (int[]){-y});
	if (y > 0) ansi('B', 1, &y);
}
void clear_screen(int erase_mode) {
	ansi('J', 1, &erase_mode);
}
void clear_line(int erase_mode) {
	ansi('K', 1, &erase_mode);
}
void show_cursor(bool show) {
	if (show) ansi_raw("?25h");
	else ansi_raw("?25l");
}
void reset_console() {
	ansi_formatting(0, 0, NULL);
}
void bold(bool bold) {
	if (bold) ansi_formatting(1, 0, NULL);
	else ansi_formatting(22, 0, NULL);
}
void italic(bool italic) {
	if (italic) ansi_formatting(3, 0, NULL);
	else ansi_formatting(23, 0, NULL);
}
void underline(bool underline) {
	if (underline) ansi_formatting(4, 0, NULL);
	else ansi_formatting(24, 0, NULL);
}
void strikethrough(bool strikethrough) {
	if (strikethrough) ansi_formatting(9, 0, NULL);
	else ansi_formatting(29, 0, NULL);
}
void show_text(bool show) {
	if (show) ansi_formatting(28, 0, NULL);
	else ansi_formatting(8, 0, NULL);
}
void blink(bool blink) {
	if (blink) ansi_formatting(5, 0, NULL);
	else ansi_formatting(25, 0, NULL);
}
void flip_colors() {
	ansi_formatting(7, 0, NULL);
}
void foreground_color_4(int color) {
	if (color == -1) ansi_formatting(39, 0, NULL);
	else if (color < 8) ansi_formatting(30 + color, 0, NULL);
	else ansi_formatting(82 + color, 0, NULL);
}
void background_color_4(int color) {
	if (color == -1) ansi_formatting(49, 0, NULL);
	else if (color < 8) ansi_formatting(40 + color, 0, NULL);
	else ansi_formatting(92 + color, 0, NULL);
}
void foreground_color_8(unsigned char color) {
	ansi_formatting(38, 2, (int[]){5, color});
}
void background_color_8(unsigned char color) {
	ansi_formatting(48, 2, (int[]){5, color});
}
void foreground_color_24(unsigned char r, unsigned char g, unsigned char b) {
	ansi_formatting(38, 4, (int[]){2, r, g, b});
}
void background_color_24(unsigned char r, unsigned char g, unsigned char b) {
	ansi_formatting(48, 4, (int[]){2, r, g, b});
}
void save_cursor_pos() {
	ansi('s', 0, NULL);
}
void restore_cursor_pos() {
	ansi('u', 0, NULL);
}