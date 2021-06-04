/*************************************
* Lab 1 Exercise 3
* Name     :
* Matric No:
* Lab Group:
*************************************/

#include <stdio.h>
#include <math.h>

//Datatype Declarations
typedef int (*arithFuncPtr)(int, int);


//Function Prototypes
int add(int x, int y);
int lumos(int x, int y);
int alohomora(int x, int y);
int expelliarmus(int x, int y);
int sonorus(int x, int y);

int main()
{
    int spell, a, b, optype, res;

    //The code below is to show the usage of a function pointer only
    //You can remove/comment them off 

    //arithFuncPtr is a function pointer datatype
    //   - it points to afunction that takes (int, int) as parameter
    //   - and returns int as return result

    arithFuncPtr spell1, spell2, spell3, spell4, spell5;
    arithFuncPtr spellArray[5] = {&add, &lumos, &alohomora, &expelliarmus, &sonorus};
    const char *spellNameArray[] = {"add", "lumos", "alohomora", "expelliarmus", "sonorus"};
    //ptr points to the function add

    while (scanf("%i %i %i", &spell, &a, &b) == 3){
        res = (*(spellArray[spell]))(a, b);
        printf("%s %i\n", spellNameArray[spell], res);
    } 

    //Defereference a function pointer === call the function
    //if you take (*ptr) as some function f
    // then below is the same as
    //   res = f(a, b);

    // res = (*ptr)(a, b);
    return 0;
}

int add(int x, int y)
{
    return x+y;
}

int lumos(int x, int y)
{
    int a = y - x;
    int z = (a + 1) * x + a * (a + 1) / 2;
    return z;
}

int alohomora(int x, int y)
{
    int temp;
    while (y != 0)
    {
        temp = x % y;
        x = y;
        y = temp;
    }
    return x;
}

int expelliarmus(int x, int y)
{
    int z = x;
    while(z % y == 0) // while x can be divided by y
    {
        z = z / y;
    }
    return z;
}

int sonorus(int x, int y)
{
    int z = pow(x, y);
    return z;
}
