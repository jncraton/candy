#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
#endif

unsigned char buf[256]
unsigned char buf_start = 255

void fillBuf():
    if (!fread(buf, 1, 255, stdin)):
        exit(0)

unsigned char ithByte(unsigned char i):
    return buf[(unsigned char)(buf_start + i)]

unsigned char getNextByte():
    unsigned char byte[2]

    buf_start++

    if (!fread(byte, 1, 1, stdin)):
        buf[(unsigned char)(buf_start - 1)] = 0x00
     else:
        buf[(unsigned char)(buf_start - 1)] = byte[0]

    if (!buf[buf_start]):
        return 0
     else:
        return 1

int nextLineIndent():
    for (unsigned char i = 0; i < 255; i++):
        if (ithByte(i) == '\n' && ithByte(i+1) != '\n'):
            unsigned char count
            for (count = 0; ithByte(i + count + 1) == ' '; count++)
            return count >> 2

    return 0

int main (int argc, char **argv):
    int line_pos = 0
    unsigned char open_braces = 0
    unsigned char previous_indent = 0
    unsigned char is_preprocessor_line = 0
    unsigned char is_single_quoted = 0
    unsigned char is_double_quoted = 0
    unsigned char literal = 0

    fillBuf()

    #ifdef _WIN32
    // Work around newline replacement on Windows
    extern int fileno(FILE*)
    setmode(fileno(stdout), O_BINARY)
    #endif

    while (getNextByte()):
        if (ithByte(0) == ':' && ithByte(1) == '\n'):
            printf(" {")
            open_braces++
         else:
            printf("%c", ithByte(0))

        if (!is_preprocessor_line):
            // Handle closing brace insertion
            if (ithByte(0) == '\n' && !is_preprocessor_line):
                while (open_braces && previous_indent > nextLineIndent()):
                    for (unsigned char i = 4; i < previous_indent << 2; i++):
                        printf(" ")

                    printf("}\n")
                    previous_indent--
                    open_braces--

                previous_indent = nextLineIndent()
                #ifdef debug
                printf("// Next indent level: %d\n", previous_indent)
                #endif

            // Handle semicolon insertions
            if (ithByte(1) == '\n' &&
                ithByte(0) != ';' &&
                ithByte(0) != ',' &&
                ithByte(0) != '&' &&
                //                ithByte(0) != '+' &&
                //                ithByte(0) != '-' &&
                ithByte(0) != '*' &&
                ithByte(0) != '/' &&
                ithByte(0) != '>' &&
                ithByte(0) != '<' &&
                ithByte(0) != '"' &&
                ithByte(0) != '\'' &&
                ithByte(0) != '|' &&
                ithByte(0) != '{' &&
                ithByte(0) != '}' &&
                ithByte(0) != ' ' &&
                ithByte(0) != '\n' &&
                ithByte(0) != ':'):
                printf(";")
        
        if (ithByte(0) == '\n'):
            is_preprocessor_line = 0

        if (ithByte(0) == '#' && !(is_single_quoted || is_double_quoted)):
            is_preprocessor_line = 1

        if (ithByte(0) == '\'' && !literal):
            is_single_quoted = !is_single_quoted

        if (ithByte(0) == '\'' && !literal):
            is_double_quoted = !is_double_quoted
        
        if (ithByte(0) == '\\'):
            literal = 1
        else:
            literal = 0
        
        line_pos +=1

    return 0
