#include "stdio.h"
#include "string.h"
#include "fcntl.h"
#ifdef _WIN32
#include "io.h"
#endif
int in_regular_code();
void fill_buffer();
unsigned char get_byte(unsigned char i);
void set_byte(unsigned char i, unsigned char val);
unsigned char read_next_byte();
int next_indent();
int is_valid_name_char(unsigned char c);
int is_separator(unsigned char c);
int buf_starts_with(char* keyword);
int replace_keyword(char* keyword, char* replacement);
void automatic_semicolon();
int do_replacements();
int main (int argc, char **argv);
