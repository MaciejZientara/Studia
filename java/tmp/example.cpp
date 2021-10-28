

template <class T>
T GetMax(T a, T b){
    T result;
    result = (a > b) ? a : b;
    return (result);
}

template <typename T>
T myMax(T x, T y){
    return (x > y) ? x : y;
}

// typename = class , można uzywać zamiennie

//bardziej do C
int main(int argc, char *argv[]){
}

//==========================================================    wskaźnik na funkcję

#include <stdio.h>
void my_int_func(int x){
    printf( "%d\n", x );
}
 
int main(){
    void (*foo)(int);
    /* the ampersand is actually optional */
    foo = &my_int_func;
 
    return 0;
}

//==========================================================

class Foo : public parent_class
{
        Foo() : parent_class( "arg" ) // sample initialization list
        {
                // you must include a body, even if it's merely empty
        }
};

class Quux
{
        public:
                Quux() : _my_int( 5 )  // sets _my_int to 5
                { }
 
        private:
                int _my_int;
};