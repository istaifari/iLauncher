#pragma once
#ifndef _VIDEO_H
#define _VIDEO_H

int get_cursor_pos();
void set_cursor_pos(int x,int y);
void printtext(char*,char,char);
void printchar(char,char,char);
void scroll();
void rm_char_in_pos(int);
void clear_screen();
void next_line();
#endif  