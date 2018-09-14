#include "candy.h"

#ifdef _WIN32
#endif

    unsigned char buf[256];
    unsigned char buf_start = 255;
    unsigned char buf_len = 0;
    unsigned char is_preprocessor_line = (0);
    unsigned char is_single_quoted = (0);
    unsigned char is_double_quoted = (0);
    unsigned char literal = (0);
    FILE* in;
    FILE* out;

int in_regular_code() {
    return ! is_double_quoted && 
           ! is_single_quoted && 
           ! is_preprocessor_line;
    }

void fill_buffer() {
    buf_len = fread(buf, 1, 254, in);
}

unsigned char get_byte(unsigned char i) {
    return buf[(unsigned char)(buf_start + i)];
}

void set_byte(unsigned char i, unsigned char val) {
    buf[(unsigned char)(buf_start + i)] = val;
}

unsigned char read_next_byte() {
    unsigned char tmp[2];

    if ( ! buf_len) {
        return (0);
    }
    
    buf_len--;
    
    if ( fread(tmp, 1, 1, in)) {
        set_byte(-1, tmp[0]);
        buf_len++;
    }
    
    buf_start++;

    return (1);
}

int next_indent() {
    for (unsigned char i = 0; i < 254; i++) {
        if ( (get_byte(i) == '\n' && get_byte(i+1) != '\n')) {
            unsigned char j;
            
            for (j = 0; get_byte(i + j + 1) == ' '; j++);
            return j >> 2;
        }
    }

    return (0);
}

int is_valid_name_char(unsigned char c) {
    return (c >= 48 && c <= 57) || 
           (c >= 65 && c <= 90) || 
           (c >= 97 && c <= 122) || 
           (c == '_');
    }

int is_separator(unsigned char c) {
    return ( c == ';' ||
             c == ',' ||
             c == '&' ||
             //c == '+' ||
             //c == '-' ||
             c == '*' ||
             c == '/' ||
             c == '>' ||
             c == '<' ||
             c == '|' ||
             c == '{' ||
             c == '}' ||
             c == ' ' ||
             c == '\n' ||
             c == ':');
        }

int buf_starts_with(char* keyword) {
    for (int i = 0; i < strlen(keyword); i++) {
        if ( get_byte(i) != keyword[i]) {
            return (0);
        }
        
    }
    return (1);
}

int replace_keyword(char* keyword, char* replacement) {
    if ( ! in_regular_code()) {
        return (0);
    }
    
    if ( is_valid_name_char(get_byte(-1))) {
        return (0);
    }
    
    if ( is_valid_name_char(get_byte(strlen(keyword)))) {
        return (0);
    }
    
    if ( !buf_starts_with(keyword)) {
        return (0);
        
    }
    fprintf(out, ("%s"), replacement);
    
    for (int i = 0; i < strlen(keyword) - 1; i++) {
        read_next_byte();
    }
    
    set_byte(0, replacement[strlen(replacement) - 1]);
    
    return (1);
}

void automatic_semicolon() {
    if ( (get_byte(0) == '\n' && 
        ! is_preprocessor_line &&
        ! is_separator(get_byte(-1)))) {
        fprintf(out, (";"));
    }
}

int do_replacements() {
    return (replace_keyword((" pass"), (" ")) ||
            replace_keyword(("not"), ("!")) ||
            replace_keyword(("and"), ("&&")) ||
            replace_keyword(("true"), ("(1)")) ||
            replace_keyword(("false"), ("(0)")) ||
            replace_keyword(("or"), ("||")) ||
            replace_keyword(("def"), ("void")) ||
            replace_keyword(("byte"), ("unsigned char")));
        }

int main (int argc, char **argv) {
    int line_pos = (0);
    unsigned char open_braces = (0);
    unsigned char indent_level = (0);
    unsigned char needs_closing_paren = (0);
    unsigned char needs_closing_imp = (0);
    unsigned char needs_semi = (0);
    unsigned char needs_newline = (1);
    in = fopen(argv[argc-2], ("r"));
    out = fopen(argv[argc-1], ("wb"));

    fill_buffer();

    if ( argv[argc-1][strlen(argv[argc-1]) - 1] == 'h') {
        while (read_next_byte()) {
            if ( get_byte(0) == '\n') {
                if ( needs_closing_imp) {
                    fprintf(out, (".h\""));
                    needs_closing_imp = (0);
                }
                if ( ! is_preprocessor_line && needs_semi) {
                    fprintf(out, (";"));
                    needs_semi = (0);
                }
                line_pos = -1;
                indent_level = (0);
                is_preprocessor_line = (0);
            }

            if ( get_byte(0) == ' ' && line_pos == 0) {
                indent_level = (1);
            }
            else {
                line_pos += 1;
            }

            if ( (get_byte(0) == '#' && in_regular_code())) {
                is_preprocessor_line = (1);
            }
            
            if ( ! indent_level) {
                if ( do_replacements()) {
                    needs_semi = (1);
                }
                else if ( replace_keyword((":"), (""))) {
                    needs_semi = (1);
                }
                else if ( replace_keyword(("import"), ("#include"))) {
                    fprintf(out, (" \""));
                    read_next_byte();
                    needs_closing_imp = (1);
                    is_preprocessor_line = (1);
                }
                else {
                    if ( get_byte(0) != '\n' || needs_newline) {
                        fprintf(out, ("%c"), get_byte(0));
                        needs_newline = (1);
                    }
                    if ( get_byte(0) == '\n') {
                        needs_newline = (0);
                    }
                }
            }
        }
    }
    else {
        char * header_name = argv[argc-1];
        header_name[strlen(header_name) - 1] = 'h';
        fprintf(out, ("#include \"%s\"\n"), header_name);
        while (read_next_byte()) {
            automatic_semicolon();

            if ( get_byte(0) == ':' && get_byte(1) == '\n') {
                if ( needs_closing_paren) {
                    fprintf(out, (")"));
                    needs_closing_paren = (0);
                }
                fprintf(out, (" {"));
                open_braces++;
            }
            else if ( get_byte(0) == '\n') {
                if ( needs_closing_imp) {
                    fprintf(out, (".h\""));
                    needs_closing_imp = (0);
                }
                fprintf(out, ("\n"));
            }
            else if ( replace_keyword(("if"), ("if ("))) {
                needs_closing_paren = (1);
            }
            else if ( buf_starts_with(("import")) && in_regular_code()) {
                while(get_byte(0) != '\n') {
                    read_next_byte();
                }
            }
            else if ( get_byte(0) == '"' && in_regular_code()) {
                fprintf(out, ("(\""));
            }
            else if ( do_replacements()) {
                
            }
            else {
                fprintf(out, ("%c"), get_byte(0));
            }
            
            if ( ! is_preprocessor_line) {
                // Handle closing brace insertion;
                if ( get_byte(0) == '\n') {
                    while (open_braces && indent_level > next_indent()) {
                        for (unsigned char i = 4; i < indent_level << 2; i++) {
                            fprintf(out, (" "));
                        }

                        fprintf(out, ("}\n"));
                        indent_level--;
                        open_braces--;
                    }

                    indent_level = next_indent();
                }

                if ( get_byte(0) == '"' && ! (literal || is_single_quoted)) {
                    if ( is_double_quoted) {
                        fprintf(out, (")"));
                    }
                }
                
            }
            if ( get_byte(0) == '\n') {
                is_preprocessor_line = (0);
            }

            if ( get_byte(0) == '#' && in_regular_code()) {
                is_preprocessor_line = (1);
            }

            if ( get_byte(0) == '\'' && ! (literal || is_double_quoted)) {
                is_single_quoted = ! is_single_quoted;
            }

            if ( get_byte(0) == '"' && ! (literal || is_single_quoted)) {
                is_double_quoted = ! is_double_quoted;
            }
            
            if ( get_byte(0) == '\\' && ! literal) {
                literal = (1);
            }
            else {
                literal = (0);
            }
            
            line_pos +=1;
        }
    }
    
    fclose(in);
    fclose(out);
    
    return (0);
}
