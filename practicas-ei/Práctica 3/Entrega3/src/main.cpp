#include <iostream> 
#include <string>
#include "buscador.h"
#include "indexadorHash.h"

using namespace std;

////////////////////////////////////////////
//   HABRIA QUE CAMBIAR EN tad02.cpp.sal LOS PESOS DEL DOCUMENTO
////////////////////////////////////////////

int main() {
//IndexadorHash b("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, false);

//b.IndexarDirectorio("./materiales_buscador/CorpusTime/Documentos");
//b.GuardarIndexacion();
Buscador a("./indicePrueba", 0);

//a.Buscar("./materiales_buscador/CorpusTime/Preguntas", 423, 1, 83);
//a.ImprimirResultadoBusqueda(423, "fich_salida_buscador_alumnoDFR.txt");

a.CambiarFormulaSimilitud(1);
a.Buscar("./materiales_buscador/CorpusTime/Preguntas", 423, 1, 83);
a.ImprimirResultadoBusqueda(423, "fich_salida_buscador_alumnoBM25.txt");

}
