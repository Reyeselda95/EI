/*
 * buscador.h
 *
 *  Created on: 6 de may. de 2016
 *      Author: Alejandro
 */

#ifndef BUSCADOR_H_
#define BUSCADOR_H_

#include "indexadorHash.h"
#include <iostream>
#include <queue>
#include "stdio.h"
#include "math.h"
#include "stdlib.h"

using namespace std;

class ResultadoRI {
	friend ostream& operator<<(ostream&, const ResultadoRI&);
	public:
		ResultadoRI(const double& kvSimilitud, const long int& kidDoc, const
		int& np);
		double VSimilitud() const;
		long int IdDoc() const;
		int NumPregunta() const;
		bool operator< (const ResultadoRI& lhs) const;
	private:
		double vSimilitud;
		long int idDoc;
		int numPregunta;
};

class Buscador: public IndexadorHash {
	friend ostream& operator<<(ostream& s, const Buscador& p) {
		string preg;
		s << "Buscador: " << endl;
		if(p.DevuelvePregunta(preg))
			s << "\tPregunta indexada: " << preg << endl;
		else
			s << "\tNo hay ninguna pregunta indexada" << endl;
		s << "\tDatos del indexador: " << endl << (IndexadorHash) p;
		// Invoca a la sobrecarga de la salida del Indexador
		return s;
	}
	public:
		/** Constructor para inicializar Buscador a partir de la indexación
		 * generada previamente y almacenada en "directorioIndexacion". En caso que
		 * no exista el directorio o que no contenga los datos de la indexación se
		 * enviará a cerr la excepción correspondiente y se continuará la ejecución
		 * del programa manteniendo los índices vacíos
		 */
		/** Inicializará la variable privada "formSimilitud" a "f" y las
	 * constantes de cada modelo: "c = 2; k1 = 1.2; b = 0.75;"
	 */
		Buscador(const string& directorioIndexacion, const int& f);
		Buscador(const Buscador& b);
		~Buscador();
		Buscador& operator= (const Buscador& b);

		/** Devuelve true si en IndexadorHash.pregunta hay indexada una pregunta
		 * no vacía con algún término con contenido, y si sobre esa pregunta se
		 * finaliza la búsqueda correctamente con la fórmula de similitud indicada
		 * en la variable privada "formSimilitud".
		 */
		/** Por ejemplo, devuelve falso si no finaliza la búsqueda por falta de
		 * memoria, mostrando el mensaje de error correspondiente, e indicando el
		 * documento y término en el que se ha quedado.
		 */
		/** Se guardarán los primeros "numDocumentos" documentos más relevantes
		 * en la variable privada "docsOrdenados" en orden decreciente según la
		 * relevancia sobre la pregunta (se vaciará previamente el contenido de
		 * esta variable antes de realizar la búsqueda). Como número de pregunta en
		 * "ResultadoRI.numPregunta" se almacenará el valor 0
		 */
		bool Buscar(const int& numDocumentos);
		/** Realizará la búsqueda entre el número de pregunta "numPregInicio" y
		 * "numPregFin", ambas preguntas incluidas. El corpus de preguntas estará en el directorio "dirPreguntas", y tendrá la estructura de cada pregunta
		 * en un fichero independiente, de nombre el número de pregunta, y
		 * extensión ".txt" (p.ej. 1.txt 2.txt 3.txt ... 83.txt). Esto significa
		 * que habrá que indexar cada pregunta por separado y ejecutar una búsqueda
		 * por cada pregunta añadiendo los resultados de cada pregunta (junto con
		 * su número de pregunta) en la variable privada "docsOrdenados".
		 */
		/** Se guardarán los primeros "numDocumentos" documentos más relevantes
		 * para cada pregunta en la variable privada "docsOrdenados"
		 */
		/** La búsqueda se realiza con la fórmula de similitud indicada en la
		 * variable privada "formSimilitud".
		 */
		/** Devuelve falso si no finaliza la búsqueda (p.ej. por falta de
		 * memoria), mostrando el mensaje de error correspondiente, indicando el
		 * documento, pregunta y término en el que se ha quedado.
		 */
		bool Buscar(const string& dirPreguntas, const int& numDocumentos, const int& numPregInicio, const int& numPregFin);
		/** Imprimirá por pantalla los resultados de la última búsqueda (un
		 * número máximo de "numDocumentos" por cada pregunta), los cuales estarán
		 * almacenados en la variable privada "docsOrdenados" en orden decreciente
		 * según la relevancia sobre la pregunta, en el siguiente formato (una
		 * línea por cada documento):
		 * NumPregunta FormulaSimilitud NomDocumento Posicion PuntuacionDoc
		 * PreguntaIndexada
		 */
		/** Donde:
		 * NumPregunta sería el número de pregunta almacenado en
		 * "ResultadoRI.numPregunta"
		 * FormulaSimilitud sería: "DFR" si la variable privada
		 * "formSimilitud == 0"; "BM25" si es 1.
		 * NomDocumento sería el nombre del documento almacenado
		 * en la indexación (habrá que extraer el nombre del documento
		 * a partir de "ResultadoRI.idDoc", pero sin el directorio
		 * donde esté almacenado ni la extensión del archivo)
		 * Posicion empezaría desde 0 (indicando el documento más
		 * relevante para la pregunta) incrementándose por cada
		 * documento (ordenado por relevancia). Se imprimirá un máximo
		 * de líneas de "numDocumentos" (es decir, el máximo valor de
		 * este campo será "numDocumentos - 1")
		 * PuntuacionDoc sería el valor numérico de la fórmula de
		 * similitud empleada almacenado en "ResultadoRI.vSimilitud".
		 * Se mostrarán los decimales con el punto en lugar de con la coma.
		 * PreguntaIndexada se corresponde con IndexadorHash.pregunta
		 * si "ResultadoRI.numPregunta == 0". En caso contrario se
		 * imprimirá "ConjuntoDePreguntas"
		 */
		/** Por ejemplo:
		 * 1 BM25 EFE19950609-05926 0 64.7059 ConjuntoDePreguntas
		 * 1 BM25 EFE19950614-08956 1 63.9759 ConjuntoDePreguntas
		 * 1 BM25 EFE19950610-06424 2 62.6695 ConjuntoDePreguntas
		 * 2 BM25 EFE19950610-00234 0 0.11656233535972 ConjuntoDePreguntas
		 * 2 BM25 EFE19950610-06000 1 0.10667871616613 ConjuntoDePreguntas
		 */
		/** Este archivo debería usarse con la utilidad
		 * "trec_eval -q -o
		 * frelevancia_trec_eval_TIME.txt fich_salida_buscador.txt > fich_salida_trec_eval.res"",
		 * para obtener los datos de precisión y cobertura
		 */
		void ImprimirResultadoBusqueda(const int& numDocumentos) const;
		/** Lo mismo que "ImprimirResultadoBusqueda()" pero guardando la salida
		 * en el fichero de nombre "nombreFichero"
		 */
		// Devolverá false si no consigue crear correctamente el archivo
		bool ImprimirResultadoBusqueda(const int& numDocumentos, const string& nombreFichero) const;
		// Devuelve el valor del campo privado "formSimilitud"
		int DevolverFormulaSimilitud() const;
		// Cambia el valor de "formSimilitud" a "f" si contiene un valor correcto (f == 0 || f == 1);
		// Devolverá false si "f" no contiene un valor correcto, en cuyo caso no cambiaría el valor anterior de "formSimilitud"
		bool CambiarFormulaSimilitud(const int& f);
		// Cambia el valor de "c = kc"
		void CambiarParametrosDFR(const double& kc);
		// Devuelve el valor de "c"
		double DevolverParametrosDFR() const;
		// Cambia el valor de "k1 = kk1; b = kb;"
		void CambiarParametrosBM25(const double& kk1, const double& kb);
		// Devuelve el valor de "k1" y "b"
		void DevolverParametrosBM25(double& kk1, double& kb) const;
		//Devuelve el valor de la similitud DFR
		double DFR(int Doc);
		//Devuelve el valor de la similitud BM25
		double BM25(int Doc);
		//Devuelve el nombre de un documento sin la ruta ni la extensión
		string SacaNombre(long int id) const;
	private:
		/** Este constructor se pone en la parte privada porque no se permitirá
		crear un buscador sin inicializarlo convenientemente a partir de una
		indexación.*/
		/** Se inicializará con todos los campos vacíos y la variable privada
		* "formSimilitud" con valor 0 y las constantes de cada modelo:
		* "c = 2; k1 = 1.2; b = 0.75"
		*/
		Buscador();
		/** Contendrá los resultados de la última búsqueda realizada en orden
		* decreciente según la relevancia sobre la pregunta. El tipo
		* "priority_queue" podrá modificarse por cuestiones de eficiencia. La
		* clase "ResultadoRI" aparece en la sección "Ejemplo de modo de uso de la
		* cola de prioridad de STL"
		*/
		priority_queue< ResultadoRI > docsOrdenados;
		// 0: DFR, 1: BM25
		int formSimilitud;
		// Constante del modelo DFR
		double c;
		// Constante del modelo BM25
		double k1;
		// Constante del modelo BM25
		double b;
};

#endif /* BUSCADOR_H_ */
