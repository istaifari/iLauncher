#pragma once
#ifndef _VIDEO_H
#define _VIDEO_H

unsigned char port_byte_read(unsigned short port );
void port_byte_write(unsigned short port ,unsigned char data );
int get_cursor_pos();
void set_cursor_pos(int x,int y);
void printtext(char*,char,char);
void printchar(char,char,char);
void scroll();
void rm_char_in_pos(int);
void clear_screen();
void next_line();
#endif  