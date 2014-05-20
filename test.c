#include "stdlib.h"
#include "stdio.h"

void main() {
    printf ("Hello, world!");
    
    unsigned char dummy = '"';
    unsigned char dummy2 = ("\"");
    unsigned char dummy3 = ("'");
    unsigned char dummy3 = ("\\\"");
    
    int this_is_false = (0);
    int true_this_is = (1);
    
    if ( do_something || do_something_else) {
        
    }

    if ( (some_condition && 
        another_condition ||
        ! some_other_condition)) {
        do_something();
    }
    
    do_something_else();
    
    while (1) {
        do_this_a_lot();
    }
}
