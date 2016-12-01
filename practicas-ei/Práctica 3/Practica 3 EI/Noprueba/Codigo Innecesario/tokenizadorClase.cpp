/*
 * TokenizadorClase.cpp
 *
 *  Created on: 29 de feb. de 2016
 *      Author: Alejandro
 */

#include "tokenizadorClase.h"

// cout << "DELIMITADORES: " << delimiters;
ostream& operator<<(ostream& o, const TokenizadorClase& tc){
	o<< "DELIMITADORES: " << tc.delimiters;
	return o;
}

TokenizadorClase::TokenizadorClase (const TokenizadorClase& tc){
	delimiters= tc.delimiters;
}

// Inicializa variable privada delimiters a delimitadoresPalabra
TokenizadorClase::TokenizadorClase (const string& delimitadoresPalabra){
	delimiters= delimitadoresPalabra;
}

// Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@"
TokenizadorClase::TokenizadorClase (){
   delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
}

// Pone delimiters=""
TokenizadorClase::~TokenizadorClase (){
	delimiters ="";
}

TokenizadorClase& TokenizadorClase::operator= (const TokenizadorClase& tc){
	if(this!=&tc){
		(*this).~TokenizadorClase();
		delimiters=tc.delimiters;
	}
	return *this;
}

// Tokeniza str devolviendo el resultado en tokens. La lista tokens se vaciará antes de almacenar el resultado de la tokenización.
// El código de esta función tal y como se ha visto en clase se muestra en este enunciado a continuación del prototipo de la clase TokenizadorClase
void TokenizadorClase::Tokenizar (const string& str, list<string>& tokens) const {
	if(tokens.size()!=0){
		tokens.clear();
	}
	string::size_type lastPos = str.find_first_not_of(delimiters,0);
	string::size_type pos = str.find_first_of(delimiters,lastPos);

	while(string::npos != pos || string::npos != lastPos) {
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}
}

// Tokeniza el fichero i guardando la salida en el fichero f (una palabra en cada línea del fichero).
// Devolverá true si se realiza la tokenización de forma correcta enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i)
bool TokenizadorClase::Tokenizar (const string& i, const string& f) const{
	TokenizadorClase aux = TokenizadorClase(*this);
	ifstream file;
	ofstream ofile;
	string linea;
	list<string> tokens;

	file.open(i.c_str());
	
	if(file.is_open()){
		ofile.open(f.c_str(), ios::out);
		//Recorre el fichero leyendo líneas y tokenizandolas
		while(!file.eof()){
			linea = "";
			getline(file, linea);
			if(linea.length()!=0){
				aux.Tokenizar(linea,tokens);
				if(ofile.is_open()){
					list<string>::iterator itS;
					for(itS= tokens.begin();itS!= tokens.end();++itS)
					{
						ofile << (*itS) << endl;
					}
					
				}
			}
		}
		ofile.close();
		file.close();
	}
	else{
		cerr << "ERROR: No existe el archivo: " << i << endl;
		return false;
	}
	return true;
}

// Tokeniza el fichero i guardando la salida en un fichero de nombre i añadiéndole extensión .tk (sin eliminar previamente la extensión de i por ejemplo, del archivo pp.txt se generaría el resultado en pp.txt.tk), y que contendrá una palabra en cada línea del fichero.
// Devolverá true si se realiza la tokenización de forma correcta enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i)
bool TokenizadorClase::Tokenizar (const string & i) const{
	return Tokenizar (i, i+".tk");
}

// Tokeniza el fichero i que contiene un nombre de fichero por línea guardando la salida en un fichero cuyo nombre será el de entrada añadiéndole extensión .tk, y que contendrá una palabra en cada línea del fichero.
// Devolverá true si se realiza la tokenización de forma correcta de todos los archivos que contiene i; devolverá false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i, o bien enviando a "cerr" los archivos de i que no existan)
bool TokenizadorClase::TokenizarListaFicheros (const string& i) const{
	ifstream file;
	ofstream ofile;
	string linea;
	file.open(i.c_str());
	struct stat dir;

	if(file.is_open()){
		//Recorre el fichero leyendo líneas y tokenizandolas
		while(!file.eof()){
			linea = "";
			getline(file, linea);
			int err=stat(linea.c_str(), &dir);

			if(err==-1 || !S_ISDIR(dir.st_mode)){
				if(linea.length()!=0){
					Tokenizar(linea);
				}
			}
		}
		file.close();
	}
	else{
		cerr << "ERROR: No existe el archivo: " << i << endl;
		return false;
	}
	return true;
}

// Tokeniza todos los archivos que contenga el directorio i, incluyendo los de los subdirectorios, guardando la salida en ficheros cuyo nombre será el de entrada añadiéndole extensión .tk, y que contendrá una palabra en cada línea del fichero.
// Devolverá true si se realiza la tokenización de forma correcta de todos los archivos; devolverá false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no exista el directorio i, o los ficheros que no se hayan podido tokenizar)
bool TokenizadorClase::TokenizarDirectorio (const string& i) const{
	struct stat dir;
	// Compruebo la existencia del directorio
	int err=stat(i.c_str(), &dir);

	if(err==-1 || !S_ISDIR(dir.st_mode))
		return false;
	else {
		// Hago una lista en un fichero con find>fich
		string cmd="find "+i+" -follow |sort > .lista_fich"; //Extrae todas las rutas de los ficheros a un archivo .lista_fich
		system(cmd.c_str()); //Ejecuta el comando cmd en el tablero de comandos
		return TokenizarListaFicheros(".lista_fich");
	}
}

// Cambia "delimiters" por "nuevoDelimiters"
void TokenizadorClase::DelimitadoresPalabra(const string& nuevoDelimiters){
		delimiters=nuevoDelimiters;
}

// Añade al final de "delimiters" los nuevos delimitadores que aparezcan en "nuevoDelimiters" (no se almacenarán caracteres repetidos)
void TokenizadorClase::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters){
		delimiters+=nuevoDelimiters;
}

// Devuelve "delimiters"
string TokenizadorClase::DelimitadoresPalabra() const{
	return delimiters;
}
