#include <iostream> 
#include <string>
#include <list> 
#include "tokenizador.h"
#include "indexadorHash.h"

using namespace std;

int main(void)
{
	IndexadorHash b(".//StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, true);
	/*Tokenizador Tok;
	Tok.DelimitadoresPalabra(" \t");
	Tok.TokenizarListaFicheros("./listaFicheros.txt");*/
	b.IndexarDirectorio("./corpus");
	cout << "Tamaño del indice: "<<b.NumPalIndexadas()<< endl;
	b.ListarInfColeccDocs();
	//b.ListarTerminos();
	//b.ImprimirIndexacion();

/*	b.IndexarPregunta("pal1 yo pal2 pal1. pal3 el  ");
	b.GuardarIndexacion();

	IndexadorHash a(".//StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, true);
	a.RecuperarIndexacion("./indicePrueba");

	//IndexadorHash a("./indicePrueba");

	cout << "Tamaño del indice: "<<a.NumPalIndexadas()<< endl;
	a.ListarInfColeccDocs();*/

	cout << "------------------------" << endl << "Info B" <<endl<< "------------------------"<< endl;
	cout << b.DevolverDelimitadores () << endl;
	cout << b.DevolverDirIndice () << endl;
	cout << b.DevolverTipoStemming () << endl;
	cout << b.DevolverAlmEnDisco () << endl;
/*	b.ImprimirIndexacion();
	b.ImprimirIndexacionPregunta();
	b.ImprimirPregunta();*/
	/*cout << "------------------------" << endl << "Info A" <<endl<< "------------------------"<< endl;
	cout << a.DevolverDelimitadores () << endl;
	cout << a.DevolverDirIndice () << endl;
	cout << a.DevolverTipoStemming () << endl;
	cout << a.DevolverAlmEnDisco () << endl;
	a.ImprimirIndexacion();
	a.ImprimirIndexacionPregunta();
	a.ImprimirPregunta();*/


}
