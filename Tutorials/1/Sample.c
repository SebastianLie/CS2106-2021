#include <stdio.h>

int global = 0;

int function( int parameter )
{
    int local;

    local = parameter + 123;
    global = local + 456;

    return local;
}

int main()
{
    int result;

    result = function( 999 );
}
