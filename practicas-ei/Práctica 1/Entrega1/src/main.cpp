#include <iostream> 
#include <string>
#include <list> 
#include "tokenizadorClase.h"
#include "tokenizador.h"

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

	Tokenizador a;
	cout << a << endl;
	string inicio, final1;
	a.TokenizarDirectorio("./src/corpus");
	//a.TokenizarDirectorio("./src/corpus");
	imprimirListaSTL(lt1);
}
