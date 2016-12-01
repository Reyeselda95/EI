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
		/** Constructor para inicializar Buscador a partir de la indexaci�n
		 * generada previamente y almacenada en "directorioIndexacion". En caso que
		 * no exista el directorio o que no contenga los datos de la indexaci�n se
		 * enviar� a cerr la excepci�n correspondiente y se continuar� la ejecuci�n
		 * del programa manteniendo los �ndices vac�os
		 */
		/** Inicializar� la variable privada "formSimilitud" a "f" y las
	 * constantes de cada modelo: "c = 2; k1 = 1.2; b = 0.75;"
	 */
		Buscador(const string& directorioIndexacion, const int& f);
		Buscador(const Buscador& b);
		~Buscador();
		Buscador& operator= (const Buscador& b);

		/** Devuelve true si en IndexadorHash.pregunta hay indexada una pregunta
		 * no vac�a con alg�n t�rmino con contenido, y si sobre esa pregunta se
		 * finaliza la b�squeda correctamente con la f�rmula de similitud indicada
		 * en la variable privada "formSimilitud".
		 */
		/** Por ejemplo, devuelve falso si no finaliza la b�squeda por falta de
		 * memoria, mostrando el mensaje de error correspondiente, e indicando el
		 * documento y t�rmino en el que se ha quedado.
		 */
		/** Se guardar�n los primeros "numDocumentos" documentos m�s relevantes
		 * en la variable privada "docsOrdenados" en orden decreciente seg�n la
		 * relevancia sobre la pregunta (se vaciar� previamente el contenido de
		 * esta variable antes de realizar la b�squeda). Como n�mero de pregunta en
		 * "ResultadoRI.numPregunta" se almacenar� el valor 0
		 */
		bool Buscar(const int& numDocumentos);
		/** Realizar� la b�squeda entre el n�mero de pregunta "numPregInicio" y
		 * "numPregFin", ambas preguntas incluidas. El corpus de preguntas estar� en el directorio "dirPreguntas", y tendr� la estructura de cada pregunta
		 * en un fichero independiente, de nombre el n�mero de pregunta, y
		 * extensi�n ".txt" (p.ej. 1.txt 2.txt 3.txt ... 83.txt). Esto significa
		 * que habr� que indexar cada pregunta por separado y ejecutar una b�squeda
		 * por cada pregunta a�adiendo los resultados de cada pregunta (junto con
		 * su n�mero de pregunta) en la variable privada "docsOrdenados".
		 */
		/** Se guardar�n los primeros "numDocumentos" documentos m�s relevantes
		 * para cada pregunta en la variable privada "docsOrdenados"
		 */
		/** La b�squeda se realiza con la f�rmula de similitud indicada en la
		 * variable privada "formSimilitud".
		 */
		/** Devuelve falso si no finaliza la b�squeda (p.ej. por falta de
		 * memoria), mostrando el mensaje de error correspondiente, indicando el
		 * documento, pregunta y t�rmino en el que se ha quedado.
		 */
		bool Buscar(const string& dirPreguntas, const int& numDocumentos, const int& numPregInicio, const int& numPregFin);
		/** Imprimir� por pantalla los resultados de la �ltima b�squeda (un
		 * n�mero m�ximo de "numDocumentos" por cada pregunta), los cuales estar�n
		 * almacenados en la variable privada "docsOrdenados" en orden decreciente
		 * seg�n la relevancia sobre la pregunta, en el siguiente formato (una
		 * l�nea por cada documento):
		 * NumPregunta FormulaSimilitud NomDocumento Posicion PuntuacionDoc
		 * PreguntaIndexada
		 */
		/** Donde:
		 * NumPregunta ser�a el n�mero de pregunta almacenado en
		 * "ResultadoRI.numPregunta"
		 * FormulaSimilitud ser�a: "DFR" si la variable privada
		 * "formSimilitud == 0"; "BM25" si es 1.
		 * NomDocumento ser�a el nombre del documento almacenado
		 * en la indexaci�n (habr� que extraer el nombre del documento
		 * a partir de "ResultadoRI.idDoc", pero sin el directorio
		 * donde est� almacenado ni la extensi�n del archivo)
		 * Posicion empezar�a desde 0 (indicando el documento m�s
		 * relevante para la pregunta) increment�ndose por cada
		 * documento (ordenado por relevancia). Se imprimir� un m�ximo
		 * de l�neas de "numDocumentos" (es decir, el m�ximo valor de
		 * este campo ser� "numDocumentos - 1")
		 * PuntuacionDoc ser�a el valor num�rico de la f�rmula de
		 * similitud empleada almacenado en "ResultadoRI.vSimilitud".
		 * Se mostrar�n los decimales con el punto en lugar de con la coma.
		 * PreguntaIndexada se corresponde con IndexadorHash.pregunta
		 * si "ResultadoRI.numPregunta == 0". En caso contrario se
		 * imprimir� "ConjuntoDePreguntas"
		 */
		/** Por ejemplo:
		 * 1 BM25 EFE19950609-05926 0 64.7059 ConjuntoDePreguntas
		 * 1 BM25 EFE19950614-08956 1 63.9759 ConjuntoDePreguntas
		 * 1 BM25 EFE19950610-06424 2 62.6695 ConjuntoDePreguntas
		 * 2 BM25 EFE19950610-00234 0 0.11656233535972 ConjuntoDePreguntas
		 * 2 BM25 EFE19950610-06000 1 0.10667871616613 ConjuntoDePreguntas
		 */
		/** Este archivo deber�a usarse con la utilidad
		 * "trec_eval -q -o
		 * frelevancia_trec_eval_TIME.txt fich_salida_buscador.txt > fich_salida_trec_eval.res"",
		 * para obtener los datos de precisi�n y cobertura
		 */
		void ImprimirResultadoBusqueda(const int& numDocumentos) const;
		/** Lo mismo que "ImprimirResultadoBusqueda()" pero guardando la salida
		 * en el fichero de nombre "nombreFichero"
		 */
		// Devolver� false si no consigue crear correctamente el archivo
		bool ImprimirResultadoBusqueda(const int& numDocumentos, const string& nombreFichero) const;
		// Devuelve el valor del campo privado "formSimilitud"
		int DevolverFormulaSimilitud() const;
		// Cambia el valor de "formSimilitud" a "f" si contiene un valor correcto (f == 0 || f == 1);
		// Devolver� false si "f" no contiene un valor correcto, en cuyo caso no cambiar�a el valor anterior de "formSimilitud"
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
		//Devuelve el nombre de un documento sin la ruta ni la extensi�n
		string SacaNombre(long int id) const;
	private:
		/** Este constructor se pone en la parte privada porque no se permitir�
		crear un buscador sin inicializarlo convenientemente a partir de una
		indexaci�n.*/
		/** Se inicializar� con todos los campos vac�os y la variable privada
		* "formSimilitud" con valor 0 y las constantes de cada modelo:
		* "c = 2; k1 = 1.2; b = 0.75"
		*/
		Buscador();
		/** Contendr� los resultados de la �ltima b�squeda realizada en orden
		* decreciente seg�n la relevancia sobre la pregunta. El tipo
		* "priority_queue" podr� modificarse por cuestiones de eficiencia. La
		* clase "ResultadoRI" aparece en la secci�n "Ejemplo de modo de uso de la
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
