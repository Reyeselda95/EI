/*
 * Tokenizador.h
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

#ifndef TOKENIZADOR_H_
#define TOKENIZADOR_H_

using namespace std;

class Tokenizador {
	friend ostream& operator<<(ostream&, const Tokenizador&);
	// cout << "DELIMITADORES: " << delimiters << " TRATA CASOS ESPECIALES: " << casosEspeciales << " PASAR A MINUSCULAS Y SIN ACENTOS: " << pasarAminuscSinAcentos;
	// Aunque se modifique el almacenamiento de los delimitadores por temas de eficiencia, el campo delimiters se imprimir� con el string con el que se inicializ� el tokenizador
	public:
		Tokenizador (const Tokenizador &tc);
		//Constructor de copia
		Tokenizador (const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos);
		// Inicializa delimiters a delimitadoresPalabra; casosEspeciales a kcasosEspeciales; pasarAminuscSinAcentos a minuscSinAcentos
		Tokenizador ();
		// Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>�!�?&#=\t\n\r@"; casosEspeciales a true; pasarAminuscSinAcentos a false
		~Tokenizador ();
		//Destructor de Tookenizador
		Tokenizador& operator= (const Tokenizador& t);
		void CasosEspeciales (const bool& nuevoCasosEspeciales);
		// Cambia la variable privada "casosEspeciales"
		bool CasosEspeciales ();
		// Devuelve el contenido de la variable privada "casosEspeciales"
		void PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos);
		// Cambia la variable privada "pasarAminuscSinAcentos". Atenci�n al formato de codificaci�n del corpus (comando "file" de Linux).
		// Para la correcci�n de la pr�ctica se utilizar� el formato actual (ISO-8859).
		bool PasarAminuscSinAcentos ();
		// Devuelve el contenido de la variable privada "pasarAminuscSinAcentos"
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
		bool esNumero(const string& string)const;
		//Comprueba si el string pasao
		int Count(const string& str, const string& str2)const;
		//Cuenta cuantas veces aparece un caracter (guardado en un string), en otro
		string aMinusSinAcentos(const string& str)const;
		//Pasa el string a minusculas sin acentos
		void URL(string::size_type& pos, string::size_type& lastPos,const string& aux, const string& delimitadores, list<string>& tokens)const;
		//realiza el caso de la URL
		void Numeros(string::size_type& pos, string::size_type& lastPos,string::size_type& posAcron,const string& aux, const string& delimitadores, list<string>& tokens)const;
		//realiza el caso de los Numeros
		void Email(string::size_type& pos, string::size_type& lastPos,const string& aux, const string& delimitadores, list<string>& tokens)const;
		//realiza el caso del Email
		void Acronimos(string::size_type& pos, string::size_type& lastPos,string::size_type& posAcron,const string& aux, const string& delimitadores, list<string>& tokens)const;
		//realiza el caso de los Acronimos
		void Guiones(string::size_type& pos, string::size_type& lastPos,const string& aux, const string& delimitadores, list<string>& tokens)const;
		//realiza el caso de la URL
	private:
		string delimiters;
		// Delimitadores de t�rminos. Aunque se modifique la forma de almacenamiento interna para mejorar la eficiencia, este campo debe permanecer para indicar el orden en que se introdujeron los delimitadores
		bool casosEspeciales;
		// Si true detectar� palabras compuestas y casos especiales
		bool pasarAminuscSinAcentos;
		// Si true pasar� el token a min�sculas y quitar� acentos, antes de realizar la tokenizaci�n
};

#endif /* TOKENIZADOR_H_ */

