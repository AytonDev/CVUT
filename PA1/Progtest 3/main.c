#ifndef __PROGTEST__

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define TRIANG_NONE       0
#define TRIANG_ACUTE      1 //je konstanta definovaná v testovacím prostředí. Její použití přidá do výběru ostroúhlé trojúhelníky.
#define TRIANG_RIGHT      2 //je konstanta definovaná v testovacím prostředí. Její použití přidá do výběru pravoúhlé trojúhelníky.
#define TRIANG_OBTUSE     4 //je konstanta definovaná v testovacím prostředí. Její použití přidá do výběru tupoúhlé trojúhelníky.
#define TRIANG_DUPLICATES 8 //je konstanta definovaná v testovacím prostředí, která řídí započítávání shodných trojúhelníků:
#define TRIANG_ALL       ( TRIANG_ACUTE | TRIANG_RIGHT | TRIANG_OBTUSE )
#endif /* __PROGTEST__ */


int countTriangles(int lo, int hi, int filter) {
    /* todo */
    int i, j, k, l,m, acute, right, obtuse;
    acute = 0;
    right = 0;
    obtuse = 0;
    l = 0;
    if(lo > hi || lo <1 || hi <1 || filter> 15 || filter <0)
        return -1;
    for (i = lo; i <= hi; ++i) {
        for (j = lo + l; j <= hi; ++j) {
            if(filter < 9)
                m =j;
            else
                m=lo;
            for (k = m; k <= hi; ++k) {
                if (i + j > k && j+k > i && k+i>j) {
                    if (i * i + j * j == k * k || j * j + k * k == i * i || (i * i + k * k) == j * j)
                        right++;

                    else if (i * i + j * j > k * k && j * j + k * k > i * i && (i * i + k * k) > j * j)

                        acute++;
                    else
                        obtuse++;
                }
            }
        }
        if(filter < 9)
        ++l;
    }
    if (filter == 0|| filter==8)
        return 0;
    else if (filter == 1 || filter ==9)
        return acute;
    else if (filter == 2|| filter==10)
        return right;
    else if (filter == 3 || filter==11)
        return right+acute;
    else if (filter == 4 || filter==12)
        return obtuse;
    else if (filter == 5|| filter ==13)
        return obtuse+acute;
    else if (filter == 6 || filter==14)
        return obtuse + right;
    else if (filter == 7 || filter ==15)
        return acute+obtuse+right;
    else if (filter > 15){
        return -1;
    }else {
        return -1;
    }

}

#ifndef __PROGTEST__

int main(int argc, char *argv[]) {
    assert (countTriangles(27, 83, TRIANG_OBTUSE) == 12045);
    assert (countTriangles(16, 38, TRIANG_RIGHT | TRIANG_OBTUSE) == 683);
    assert (countTriangles(31, 39, TRIANG_ACUTE) == 165);
    assert ( countTriangles ( 31, 39, TRIANG_ACUTE | TRIANG_DUPLICATES ) == 729 );
    assert ( countTriangles ( 84, 128, TRIANG_ACUTE | TRIANG_OBTUSE | TRIANG_DUPLICATES ) == 91125 );
    assert (countTriangles(10, 50, TRIANG_NONE) == 0);
    assert (countTriangles(10, 50, TRIANG_ACUTE) == 4955);
    assert (countTriangles(10, 50, TRIANG_RIGHT) == 13);
    assert (countTriangles(10, 50, TRIANG_OBTUSE) == 4517);
    assert (countTriangles(10, 50, TRIANG_ACUTE | TRIANG_RIGHT) == 4968);
    assert (countTriangles(10, 50, TRIANG_ACUTE | TRIANG_OBTUSE) == 9472);
    assert (countTriangles(10, 50, TRIANG_RIGHT | TRIANG_OBTUSE) == 4530);
    assert (countTriangles(10, 50, TRIANG_ACUTE | TRIANG_RIGHT | TRIANG_OBTUSE) == 9485);
    assert ( countTriangles ( 10, 50, TRIANG_DUPLICATES ) == 0 );
    assert ( countTriangles ( 10, 50, TRIANG_ACUTE | TRIANG_DUPLICATES ) == 26060 );
    assert ( countTriangles ( 10, 50, TRIANG_RIGHT | TRIANG_DUPLICATES ) == 78 );
    assert ( countTriangles ( 10, 50, TRIANG_OBTUSE | TRIANG_DUPLICATES ) == 26415 );
    assert ( countTriangles ( 10, 50, TRIANG_ACUTE | TRIANG_RIGHT | TRIANG_DUPLICATES ) == 26138 );
    assert ( countTriangles ( 10, 50, TRIANG_ACUTE | TRIANG_OBTUSE | TRIANG_DUPLICATES ) == 52475 );
    assert ( countTriangles ( 10, 50, TRIANG_RIGHT | TRIANG_OBTUSE | TRIANG_DUPLICATES ) == 26493 );
    assert ( countTriangles ( 10, 50, TRIANG_ACUTE | TRIANG_RIGHT | TRIANG_OBTUSE | TRIANG_DUPLICATES ) == 52553 );
    assert ( countTriangles ( 10, 50, 15 ) == 52553 );
    assert ( countTriangles ( 10, 50, 42 ) == -1 );
    assert ( countTriangles ( 10, 9, TRIANG_ACUTE ) == -1 );
    return 0;
}

#endif /* __PROGTEST__ */
