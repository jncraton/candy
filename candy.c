#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "fcntl.h"

#ifdef _WIN32
#include <io.h>
#endif

unsigned char buf[256];
unsigned char buf_start = 255;
unsigned char buf_len = 0;
unsigned char is_preprocessor_line = (0);
unsigned char is_single_quoted = (0);
unsigned char is_double_quoted = (0);
unsigned char literal = (0);

int in_regular_code() {
    return !is_double_quoted && !is_single_quoted && !is_preprocessor_line;
}

void fill_buffer() {
    if (!(buf_len = fread(buf, 1, 254, stdin))) {
        exit(0);
    }
}

unsigned char get_byte(unsigned char i) {
    return buf[(unsigned char)(buf_start + i)];
}

unsigned char read_next_byte() {
    unsigned char byte[2];

    buf_start++;

    if (!buf_len) {
        return (0);
    }
    
    buf_len--;
    
    if (fread(byte, 1, 1, stdin)) {
        buf[(unsigned char)(buf_start - 2)] = byte[0];
        buf_len++;
    }
    
    return (1);
}

int next_line_indent() {
    for (unsigned char i = 0; i < 255; i++) {
        if ((get_byte(i) == '\n' && get_byte(i+1) != '\n')) {
            unsigned char count;
            for (count = 0; get_byte(i + count + 1) == ' '; count++);
            return count >> 2;
        }
    }

    return (0);
}

int is_valid_name_char(unsigned char c) {
    return (c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == '_';
}

int replace_keyword(const char* keyword, const char* replacement) {
    if (!in_regular_code()) {
        return (0);
    }
    
    if (is_valid_name_char(get_byte(-1))) {
        return (0);
    }
    
    if (is_valid_name_char(get_byte(strlen(keyword)))) {
        return (0);
    }
    
    for (int i = 0; i < strlen(keyword); i++) {
        if (get_byte(i) != keyword[i]) {
            return (0);
        }
    }
    
    printf(replacement);
    
    for (int i = 0; i < strlen(keyword) - 1; i++) {
        read_next_byte();
    }
    
    buf[buf_start] = replacement[strlen(replacement) - 1];
    
    return (1);
}

int main (int argc, char **argv) {
    int line_pos = (0);
    unsigned char open_braces = (0);
    unsigned char previous_indent = (0);
    unsigned char needs_closing_paren = (0);
    unsigned char needs_closing_imp = (0);

    fill_buffer();

    #ifdef _WIN32
    // Work around newline replacement on Windows;
    extern int fileno(FILE*);
    setmode(fileno(stdout), O_BINARY);
    #endif

    while (read_next_byte()) {
        if (!is_preprocessor_line) {
            // Handle semicolon insertions;
            if ((get_byte(0) == '\n' &&
                get_byte(-1) != ';' &&
                get_byte(-1) != ',' &&
                get_byte(-1) != '&' &&
                //get_byte(-1) != '+' &&
                //get_byte(-1) != '-' &&
                get_byte(-1) != '*' &&
                get_byte(-1) != '/' &&
                get_byte(-1) != '>' &&
                get_byte(-1) != '<' &&
                get_byte(-1) != '|' &&
                get_byte(-1) != '{' &&
                get_byte(-1) != '}' &&
                get_byte(-1) != ' ' &&
                get_byte(-1) != '\n' &&
                get_byte(-1) != ':')) {
                printf((";"));
            }
        }

        if (get_byte(0) == ':' && get_byte(1) == '\n') {
            if (needs_closing_paren) {
                printf((")"));
                needs_closing_paren = (0);
            }
            printf((" {"));
            open_braces++;
        }
        else if (get_byte(0) == '\n') {
            if (needs_closing_imp) {
                printf((".h\""));
                needs_closing_imp = (0);
            }
            printf(("\n"));
        }
        else if (replace_keyword(("if"), ("if ("))) {
            needs_closing_paren = (1);
        }
        else if (replace_keyword(("import"), ("#include"))) {
            printf((" \""));
            read_next_byte();
            needs_closing_imp = (1);
            is_preprocessor_line = (1);
        }
        else if (get_byte(0) == '"' && !literal && !is_single_quoted && !is_double_quoted) {
            printf(("(\""));
        }
        else if (replace_keyword((" pass"), (" "))) {
        
        }
        else if (replace_keyword(("not"), ("!"))) {
        
        }
        else if (replace_keyword(("and"), ("&&"))) {
        
        }
        else if (replace_keyword(("true"), ("(1)"))) {
        
        }
        else if (replace_keyword(("false"), ("(0)"))) {
        
        }
        else if (replace_keyword(("or"), ("||"))) {
        
        }
        else {
            printf(("%c"), get_byte(0));
        }
        
        if (!is_preprocessor_line) {
            // Handle closing brace insertion;
            if (get_byte(0) == '\n') {
                while (open_braces && previous_indent > next_line_indent()) {
                    for (unsigned char i = 4; i < previous_indent << 2; i++) {
                        printf((" "));
                    }

                    printf(("}\n"));
                    previous_indent--;
                    open_braces--;
                }

                previous_indent = next_line_indent();
                #ifdef debug
                printf(("// Next indent level: %d\n"), previous_indent);
                #endif

            }
            if ((get_byte(0) == '"' && !literal && !is_single_quoted)) {
                if (is_double_quoted) {
                    printf((")"));
                }
            }
            
        }
        if (get_byte(0) == '\n') {
            is_preprocessor_line = (0);
        }

        if ((get_byte(0) == '#' && !(is_single_quoted || is_double_quoted))) {
            is_preprocessor_line = (1);
        }

        if ((get_byte(0) == '\'' && !literal && !is_double_quoted)) {
            is_single_quoted = !is_single_quoted;
        }

        if ((get_byte(0) == '"' && !literal && !is_single_quoted)) {
            is_double_quoted = !is_double_quoted;
        }
        
        if (get_byte(0) == '\\' && !literal) {
            literal = (1);
        }
        else {
            literal = (0);
        }
        
        line_pos +=1;
    }

    return (0);
}
