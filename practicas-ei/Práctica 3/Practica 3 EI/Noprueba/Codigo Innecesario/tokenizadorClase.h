/*
 * TokenizadorClase.h
 *
 *  Created on: 29 de feb. de 2016
 *      Author: Alejandro
 */
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <string>
#include <list>
#include <fstream>

#ifndef TOKENIZADORCLASE_H_
#define TOKENIZADORCLASE_H_

using namespace std;

class TokenizadorClase {
	 friend ostream& operator<<(ostream& o, const TokenizadorClase& tc);
	 // cout << "DELIMITADORES: " << delimiters;

	 public:
		TokenizadorClase (const TokenizadorClase&);
		TokenizadorClase (const string& delimitadoresPalabra);
		// Inicializa variable privada delimiters a delimitadoresPalabra
		TokenizadorClase ();
		// Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>�!�?&#=\t\n\r@"
		~TokenizadorClase (); // Pone delimiters=""

		TokenizadorClase& operator= (const TokenizadorClase&);

		void Tokenizar (const string& str, list<string>& tokens) const;
		// Tokeniza str devolviendo el resultado en tokens. La lista tokens se vaciar� antes de almacenar el resultado de la tokenizaci�n.
		// El c�digo de esta funci�n tal y como se ha visto en clase se muestra en este enunciado a continuaci�n del prototipo de la clase TokenizadorClase
		bool Tokenizar (const string& i, const string& f) const;
		// Tokeniza el fichero i guardando la salida en el fichero f (una palabra en cada l�nea del fichero).
		// Devolver� true si se realiza la tokenizaci�n de forma correcta enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i)
		bool Tokenizar (const string & i) const;
		// Tokeniza el fichero i guardando la salida en un fichero de nombre i a�adi�ndole extensi�n .tk (sin eliminar previamente la extensi�n de i por ejemplo, del archivo pp.txt se generar�a el resultado en pp.txt.tk), y que contendr� una palabra en cada l�nea del fichero.
		// Devolver� true si se realiza la tokenizaci�n de forma correcta enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i)
		bool TokenizarListaFicheros (const string& i) const;
		// Tokeniza el fichero i que contiene un nombre de fichero por l�nea guardando la salida en un fichero cuyo nombre ser� el de entrada a�adi�ndole extensi�n .tk, y que contendr� una palabra en cada l�nea del fichero.
		// Devolver� true si se realiza la tokenizaci�n de forma correctade todos los archivos que contiene i; devolver� false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i, o bien enviando a "cerr" los archivos de i que no existan)
		bool TokenizarDirectorio (const string& i) const;
		// Tokeniza todos los archivos que contenga el directorio i, incluyendo los de los subdirectorios, guardando la salida en ficheros cuyo nombre ser� el de entrada a�adi�ndole extensi�n .tk, y que contendr� una palabra en cada l�nea del fichero.
		// Devolver� true si se realiza la tokenizaci�n de forma correcta de todos los archivos; devolver� false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no exista el directorio i, o los ficheros que no se hayan podido tokenizar)
		void DelimitadoresPalabra(const string& nuevoDelimiters);
		// Cambia "delimiters" por "nuevoDelimiters"
		void AnyadirDelimitadoresPalabra(const string& nuevoDelimiters); //
		// A�ade al final de "delimiters" los nuevos delimitadores que aparezcan en "nuevoDelimiters" (no se almacenar�n caracteres repetidos)
		string DelimitadoresPalabra() const;
		// Devuelve "delimiters"
	private:
		string delimiters;
		// Delimitadores de t�rminos. Aunque se modifique la forma de almacenamiento interna para mejorar la eficiencia, este campo debe permanecer para indicar el orden en que se introdujeron los delimitadores
};

#endif /* TOKENIZADORCLASE_H_ */
