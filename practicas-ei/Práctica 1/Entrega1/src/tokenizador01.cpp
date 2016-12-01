#include <iostream> 
#include <string>
#include <list> 
#include "tokenizadorClase.h"

using namespace std;

void imprimirListaSTL(const list<string>& cadena)
{
        list<string>::const_iterator itCadena;
        for(itCadena=cadena.begin();itCadena!=cadena.end();itCadena++)
        {
                cout << (*itCadena) << ", ";
        }
        cout << endl;
}

int
main(void)
{
list<string> lt1, lt2, lt3, lt4;

TokenizadorClase a(". /");
cout << a << endl;
a.Tokenizar("MS DOS OS 2 high low", lt1);
imprimirListaSTL(lt1);
}
