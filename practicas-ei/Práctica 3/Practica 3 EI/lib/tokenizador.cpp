/*
 * Tokenizador.cpp
 *
 *  Created on: 29 de feb. de 2016
 *      Author: Alejandro
 */

#include "tokenizador.h"

ostream& operator<<(ostream& o, const Tokenizador& t){
	o<< "DELIMITADORES: " << t.delimiters << " TRATA CASOS ESPECIALES: " << t.casosEspeciales << " PASAR A MINUSCULAS Y SIN ACENTOS: " << t.pasarAminuscSinAcentos;
	return o;
}
//Constructor de copia
Tokenizador::Tokenizador(const Tokenizador &tc){
	delimiters=tc.delimiters;
	pasarAminuscSinAcentos=tc.pasarAminuscSinAcentos;
	casosEspeciales=tc.casosEspeciales;
}

// Inicializa delimiters a delimitadoresPalabra;
// casosEspeciales a kcasosEspeciales;
// pasarAminuscSinAcentos a minuscSinAcentos
Tokenizador::Tokenizador (const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos){
	delimiters=delimitadoresPalabra;
	casosEspeciales=kcasosEspeciales;
	pasarAminuscSinAcentos=minuscSinAcentos;
}

// Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>Â¡!Â¿?&#=\t\n\r@";
// casosEspeciales a true;
// pasarAminuscSinAcentos a false
Tokenizador::Tokenizador (){
	delimiters=",;:.-/+*\\ '\"{}[]()<>Â¡!Â¿?&#=\t\n\r@";
	casosEspeciales=true;
	pasarAminuscSinAcentos=false;
}

// Destructor de Tokenizador
Tokenizador::~Tokenizador (){
	delimiters ="";
	casosEspeciales=true;
	pasarAminuscSinAcentos=false;
}

// Operador asignación
Tokenizador& Tokenizador::operator= (const Tokenizador& t){
	if(this!=&t){
		//(*this).~Tokenizador();
		delimiters=t.delimiters;
		casosEspeciales=t.casosEspeciales;
		pasarAminuscSinAcentos=t.pasarAminuscSinAcentos;
	}
	return *this;
}

// Cambia la variable privada "casosEspeciales"
void Tokenizador::CasosEspeciales (const bool& nuevoCasosEspeciales){
	casosEspeciales=nuevoCasosEspeciales;
}

// Devuelve el contenido de la variable privada "casosEspeciales"
bool Tokenizador::CasosEspeciales (){
	return casosEspeciales;
}

// Cambia la variable privada "pasarAminuscSinAcentos".
// Atención al formato de codificación del corpus (comando "file" de Linux).
// Para la corrección de la práctica se utilizará el formato actual (ISO-8859).
void Tokenizador::PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos){
	pasarAminuscSinAcentos= nuevoPasarAminuscSinAcentos;
}

// Devuelve el contenido de la variable privada "pasarAminuscSinAcentos"
bool Tokenizador::PasarAminuscSinAcentos (){
	return pasarAminuscSinAcentos;
}


// Tokeniza str devolviendo el resultado en tokens. La lista tokens se vaciará antes de almacenar el resultado de la tokenización.
void Tokenizador::Tokenizar (const string& str, list<string>& tokens) const {
	if(tokens.size()!=0){
		tokens.clear();
	}
	string aux= str;//String a tokenizar
	string delimaux=delimiters;
	//Pasa a minusculas sin Acentos
	if(this->pasarAminuscSinAcentos){
		aux=aMinusSinAcentos(aux);
	}

	if(this->casosEspeciales){
		delimaux+=" ";
	}
	string::size_type lastPos = aux.find_first_not_of(delimaux,0);//primer caracter que no esté en la lista de delimitadoes
	string::size_type pos = aux.find_first_of(delimaux,lastPos);//primer delimitador existente en el string desde la posición del primer caracter que no es delimitador
	string::size_type posAcron=aux.find_first_of(delimaux+".",lastPos);
	//Mientras que no se acabe el string principal
	while(string::npos != pos || string::npos != lastPos) {
		if(this->casosEspeciales){
			if (string::npos == pos && string::npos!=lastPos){//Al iniciar lastPos será 0, pero si pos es npos dará error, por lo que lo cambiamos al length del string
				pos=str.length();
			}
			if (string::npos == posAcron && string::npos!=lastPos){//Al iniciar lastPos será 0, pero si pos es npos dará error, por lo que lo cambiamos al length del string
				posAcron=str.length();
			}
			// 1. URL: los almacenaría como un solo término aunque esté definido como delimitador el ":/.?&-=@"
			if(aux.substr(lastPos, (1+pos) - lastPos).find("http:")==0 || aux.substr(lastPos, (1+pos) - lastPos).find("https:")==0 || aux.substr(lastPos, (1+pos) - lastPos).find("ftp:")==0){
				URL(pos, lastPos, aux, delimaux, tokens);
			}

			// 2. Números decimales: .103 5,6 1,000,000 (los almacenaría como un solo término aunque esté definido como delimitador el ",.")
			else if(esNumero(aux.substr(lastPos, aux.find_first_of(delimaux+".,$%ºª"+(char)164,1+lastPos) - lastPos))){//Si es un número
				Numeros(pos, lastPos, posAcron, aux, delimaux, tokens);
			}


			// 3. E-mail: los almacenarí­a como un solo término aunque esté definido como delimitador el "._@"
			else if(aux.substr(pos, aux.find_first_not_of(delimaux+"@",pos)-pos)=="@"){
				Email(pos, lastPos, aux, delimaux, tokens);
			}

			// 4. Detección de acrónimos (U.S.A): los almacenaría como un solo término aunque esté definido como delimitador el "."
			else if(aux.substr(posAcron, aux.find_first_not_of(delimaux+".",posAcron)- posAcron)=="."){
				Acronimos(pos, lastPos, posAcron, aux, delimaux, tokens);
			}

			// 5. Guiones: por ejemplo "MS-DOS" lo almacenaría como "MS-DOS" aunque esté definido como delimitador el "-"
			else if(aux.substr(pos, aux.find_first_not_of(delimaux+"-",pos)- pos)=="-"){//si detecta 1 guión entra y mira a ver si hay más que cumplan con la condición
				Guiones(pos, lastPos, aux, delimaux, tokens);
			}
			else{
				tokens.push_back(aux.substr(lastPos, pos - lastPos));
			}
			lastPos = aux.find_first_not_of(delimaux, pos);
			pos = aux.find_first_of(delimaux, lastPos);
			posAcron= aux.find_first_of(delimaux+".",lastPos);
		}


		else{//En el caso de que no se contemplen casos especiales
			tokens.push_back(aux.substr(lastPos, pos - lastPos));
			lastPos = aux.find_first_not_of(delimiters, pos);
			pos = aux.find_first_of(delimiters, lastPos);
		}
	}
}


bool Tokenizador::Tokenizar (const string& i, const string& f) const{
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
				Tokenizar(linea,tokens);
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

// Tokeniza el fichero i guardando la salida en el fichero f (una palabra en cada línea del fichero).
// Devolverá true si se realiza la tokenización de forma correcta enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i)
bool Tokenizador::TokenizarDoc (const string& i, list<string>& tokens) const{
	ifstream file;
	string linea;
	list<string>::iterator itS;
	list<string> tok;
	if(tokens.size()!=0){
		tokens.clear();
	}
	file.open(i.c_str());
	if(file.is_open()){
		//Recorre el fichero leyendo líneas y tokenizandolas
		while(!file.eof()){
			linea = "";
			getline(file, linea);
			if(linea.length()!=0){
				Tokenizar(linea,tok);
				for(itS= tok.begin();itS!= tok.end();++itS)
				{
					tokens.push_back(*itS);
				}
			}
		}
		file.close();
		return true;
	}
	else{
		cerr << "ERROR: No existe el archivo: " << i << endl;
		return false;
	}
}

// Tokeniza el fichero i guardando la salida en un fichero de nombre i añadiéndole extensión .tk (sin eliminar previamente la extensión de i por ejemplo, del archivo pp.txt se generaría el resultado en pp.txt.tk), y que contendrá una palabra en cada línea del fichero.
// Devolverá true si se realiza la tokenización de forma correcta enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i)
bool Tokenizador::Tokenizar (const string & i) const{
	return Tokenizar (i, i+".tk");
}

// Tokeniza el fichero i que contiene un nombre de fichero por línea guardando la salida en un fichero cuyo nombre será el de entrada añadiéndole extensión .tk, y que contendrá una palabra en cada línea del fichero.
// Devolverá true si se realiza la tokenización de forma correcta de todos los archivos que contiene i; devolverá false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i, o bien enviando a "cerr" los archivos de i que no existan)
bool Tokenizador::TokenizarListaFicheros (const string& i) const{
	ifstream file;
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
bool Tokenizador::TokenizarDirectorio (const string& i) const{
	struct stat dir;
	// Compruebo la existencia del directorio
	int err=stat(i.c_str(), &dir);

	if(err==-1 || !S_ISDIR(dir.st_mode)){
		cerr << "El directorio no existe" << endl;
		return false;
	}
	else {
		// Hago una lista en un fichero con find>fich
		string cmd="find "+i+" -follow |sort > .lista_fich"; //Extrae todas las rutas de los ficheros a un archivo .lista_fich
		system(cmd.c_str()); //Ejecuta el comando cmd en el tablero de comandos
		return TokenizarListaFicheros(".lista_fich");
	}
}

// Cambia "delimiters" por "nuevoDelimiters"
void Tokenizador::DelimitadoresPalabra(const string& nuevoDelimiters){
		delimiters=nuevoDelimiters;
}

// Añade al final de "delimiters" los nuevos delimitadores que aparezcan en "nuevoDelimiters" (no se almacenarán caracteres repetidos)
void Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters){
		delimiters+=nuevoDelimiters;
}

// Devuelve "delimiters"
string Tokenizador::DelimitadoresPalabra() const{
	return delimiters;
}

//Devuelve true si el string pasado es un número
bool Tokenizador::esNumero(const string& str) const{
	for(unsigned int i=str.length()-1;i>=0 && i!=string::npos;--i){
		if(!isdigit(str[i])){
			return false;
		}
	}
	return true;
}

//Devuelve el número (positivo) de veces que aparece un caracter y -1 si no se pasa un único caracter
int Tokenizador::Count(const string& str, const string& str2)const{
	int cuenta=0;
	if(str2.length()>1){
		cerr << "El string pasado no es un caracter, sino un conjunto de ellos" << endl;
		return -1;
	}
	for(unsigned int i= str.length()-1;i>=0 && i!=string::npos;--i){
		if(str[i]==str2[0]){
			++cuenta;
		}
	}
	return cuenta;
}

string Tokenizador::aMinusSinAcentos(const string& str)const{
	string aux=str;
	for (unsigned int i=str.length()-1;i>=0 && i!=string::npos;--i){

		//cout << (char)164 << endl;//Caracter del euro

		if(aux[i]==(char)192 || aux[i]==(char)193 || aux[i]==(char)194 || aux[i]==(char)195 || aux[i]==(char)196 || aux[i]==(char)197 || aux[i]==(char)224 || aux[i]==(char)225 || aux[i]==(char)226 || aux[i]==(char)227 || aux[i]==(char)228 || aux[i]==(char)229 ){
			aux[i]='a';
		}
		else if(aux[i]==(char)200 || aux[i]==(char)201 || aux[i]==(char)202 || aux[i]==(char)203 || aux[i]==(char)232 || aux[i]==(char)233 || aux[i]==(char)234 || aux[i]==(char)235){
			aux[i]='e';
		}
		else if(aux[i]==(char)204 || aux[i]==(char)205 || aux[i]==(char)206 || aux[i]==(char)207 || aux[i]==(char)236 || aux[i]==(char)237 || aux[i]==(char)238 || aux[i]==(char)239){
			aux[i]='i';
		}
		else if(aux[i]==(char)210 || aux[i]==(char)211 || aux[i]==(char)212 || aux[i]==(char)213 || aux[i]==(char)214 || aux[i]==(char)242 || aux[i]==(char)243 || aux[i]==(char)244 || aux[i]==(char)245 || aux[i]==(char)246){
			aux[i]='o';
		}
		else if(aux[i]==(char)217 || aux[i]==(char)218 || aux[i]==(char)219 || aux[i]==(char)220 || aux[i]==(char)249 || aux[i]==(char)250 || aux[i]==(char)251 || aux[i]==(char)252){
			aux[i]='u';
		}
		else if(aux[i]==(char)221 || aux[i]==(char)253 || aux[i]==(char)255){
			aux[i]='y';
		}
		else if(aux[i]==(char)209){
			aux[i]='ñ';
		}
		else if(aux[i]==(char)231 || aux[i]==(char)199){
			aux[i]='ç';
		}

		else{
			//Pasar a minúsculas
			aux[i]=(char)tolower(aux[i]);
		}
	}
	return aux;
}

bool Tokenizador::URL(string::size_type& pos, string::size_type& lastPos,const string& aux, const string& delimitadores,list<string>& tokens) const{
	string delimaux=delimitadores;
	string sinDelimURL; //Array sin delimitadores de URL
	for(unsigned int i=delimaux.length()-1;i>=0 && i!=string::npos;--i){
		switch(delimaux[i]){
			case ':':
				break;
			case '/':
				break;
			case '.':
				break;
			case '?':
				break;
			case '&':
				break;
			case '-':
				break;
			case '=':
				break;
			case '@':
				break;
			default:
				sinDelimURL+= delimaux[i];
				break;
		}

	}
	if(aux.find_first_of(sinDelimURL, lastPos)!=string::npos){
		pos = aux.find_first_of(sinDelimURL, lastPos); //Obtenemos el espacio completo de la URL
	}
	tokens.push_back(aux.substr(lastPos, pos - lastPos)); //Metemos la URL en el array de TOKENS
	return true;
}

bool Tokenizador::Numeros(string::size_type& pos, string::size_type& lastPos,string::size_type& posAcron,const string& aux, const string& delimitadores, list<string>& tokens)const{
	string delimaux=delimitadores;
	bool isNumber=true;
	bool decimal=false;

	//Calculamos el final del token
	string::size_type nuevoini=aux.find_first_not_of(delimaux+".,",lastPos);
	string::size_type auxpos=pos;
	string atokenizar=aux.substr(nuevoini, auxpos-nuevoini);
	string especial="";//Especial es el caso con el simbolo del euro y el espacio
	especial+=(char)164;

	//Mientras sea decimal
	while(aux.substr(pos, aux.find_first_not_of(delimaux+".,",pos)-pos)=="." || aux.substr(pos, aux.find_first_not_of(delimaux+".,",pos)-pos)==","){
		decimal=true;
		nuevoini = aux.find_first_not_of(delimaux+".,", pos);//Nueva posición de inicio auxiliar
		auxpos=aux.find_first_of(delimaux+".,",nuevoini);
		if(auxpos==string::npos){//Si se va fuera del string
			auxpos=aux.length();
			if(aux.substr(pos,auxpos-pos)=="." ||aux.substr(pos,auxpos-pos)==","){
				break;
			}
		}
		if(nuevoini==string::npos){//Si se va fuera del string
			nuevoini=aux.length();
		}

		atokenizar=aux.substr(nuevoini, auxpos-nuevoini);
		if(esNumero(atokenizar)){
			if(auxpos!=string::npos){
				pos=auxpos;
			}
			else{
				pos=aux.length();
			}
		}
		else{
			auxpos=aux.find_first_not_of(delimaux+".,",auxpos);
			if(auxpos==string::npos){//Si se va fuera del string
				auxpos=aux.length();
				atokenizar=aux.substr(nuevoini, auxpos-nuevoini);
				if((atokenizar.find("%")>0 && atokenizar.find("%")!= string::npos)|| (atokenizar.find("$")>0 && atokenizar.find("$")!= string::npos)|| (atokenizar.find("º")>0&& atokenizar.find("º")!= string::npos) ||	(atokenizar.find("ª")>0 && atokenizar.find("ª")!= string::npos)|| (atokenizar.find(especial)>0 && atokenizar.find(especial)!= string::npos)){//Debemos asegurarnos de que no coge a los npos
					pos=aux.find_first_of(delimaux+".,$%ºª"+(char)164,nuevoini);
				}
			}
			atokenizar=aux.substr(nuevoini, auxpos-nuevoini);
			//En el caso en el que terminen en "%$(euro)ºª" seguidos de un espacio
			if((atokenizar.find("% ")>0 && atokenizar.find("% ")!= string::npos)|| (atokenizar.find("$ ")>0 && atokenizar.find("$ ")!= string::npos)|| (atokenizar.find("º ")>0&& atokenizar.find("º ")!= string::npos) ||	(atokenizar.find("ª ")>0 && atokenizar.find("ª ")!= string::npos)|| (atokenizar.find(especial+" ")>0 && atokenizar.find(especial+" ")!= string::npos)){//Debemos asegurarnos de que no coge a los npos
				pos=aux.find_first_of(delimaux+".,$%ºª"+(char)164,nuevoini);
			}else{
				isNumber=false;
			}
			break;
		}
	}

	if(!decimal){
		auxpos=aux.find_first_not_of(delimaux,auxpos);
		if(auxpos==string::npos){//Si se va fuera del string
			auxpos=aux.length();
			atokenizar=aux.substr(nuevoini, auxpos-nuevoini);
			if((atokenizar.find("%")>0 && atokenizar.find("%")!= string::npos)|| (atokenizar.find("$")>0 && atokenizar.find("$")!= string::npos)|| (atokenizar.find("º")>0&& atokenizar.find("º")!= string::npos) ||	(atokenizar.find("ª")>0 && atokenizar.find("ª")!= string::npos)|| (atokenizar.find(especial)>0 && atokenizar.find(especial)!= string::npos)){//Debemos asegurarnos de que no coge a los npos
				pos=aux.find_first_of(delimaux+".,$%ºª"+(char)164,nuevoini);
			}
		}
		else{
			atokenizar=aux.substr(nuevoini, auxpos-nuevoini);
			//En el caso en el que terminen en "%$(euro)ºª" seguidos de un espacio
			if((atokenizar.find("% ")>0 && atokenizar.find("% ")!= string::npos)|| (atokenizar.find("$ ")>0 && atokenizar.find("$ ")!= string::npos)|| (atokenizar.find("º ")>0&& atokenizar.find("º ")!= string::npos) ||	(atokenizar.find("ª ")>0 && atokenizar.find("ª ")!= string::npos)|| (atokenizar.find(especial+" ")>0 && atokenizar.find(especial+" ")!= string::npos)){//Debemos asegurarnos de que no coge a los npos
				pos=aux.find_first_of(delimaux+".,$%ºª"+(char)164,nuevoini);
			}
		}
	}

	//Si no es un número se tratará como un Acronimo
	if(!isNumber){
		Acronimos(pos, lastPos, posAcron, aux, delimaux, tokens);
	}
	else{
		lastPos=aux.find_first_not_of(delimaux+".,",lastPos);//Nos seguramos de que el primer token se obtenga sin el punto o la coma en el caso de ser .5165
		if(lastPos>=1){
			//si es un decimal .5815
			if(aux.substr(lastPos-1, lastPos-(lastPos-1))=="." || (aux.substr(lastPos-1, lastPos-(lastPos-1))==",")){
				tokens.push_back("0"+aux.substr(lastPos-1,pos-(lastPos-1)));//Decimal que empieza por "." o ","
			}
			//si es un decimal 6485165.684516541
			else{
				tokens.push_back(aux.substr(lastPos,pos-lastPos));
			}
		}
		else{
			tokens.push_back(aux.substr(lastPos,pos-lastPos));
		}
	}
	return true;

}

bool Tokenizador::Email(string::size_type& pos, string::size_type& lastPos,const string& aux, const string& delimitadores, list<string>& tokens)const{
	string delimaux=delimitadores;
	string sinDelimURL; //Array sin delimitadores de URL

	for(unsigned int i=delimaux.length();i>=0 && i!=string::npos ;--i){
		if(!(delimaux[i]=='.' || delimaux[i]=='_' || delimaux[i]=='@')){
			sinDelimURL+= delimaux[i];
		}
	}

	string comprueba=aux.substr(lastPos, aux.find_first_of(sinDelimURL, lastPos) - lastPos);

	while(Count(comprueba,"@")>1){//Mientras haya más de una arroba irá tokenizando las palabras hasta la arroba
		tokens.push_back(aux.substr(lastPos, pos - lastPos)); //Metemos el e-mail en tokens
		lastPos = aux.find_first_not_of(delimaux, pos);
		pos = aux.find_first_of(delimaux, lastPos);
		comprueba=aux.substr(lastPos, aux.find_first_of(sinDelimURL, lastPos) - lastPos);
	}

	pos = aux.find_first_of(sinDelimURL, lastPos); //Obtenemos el espacio completo de la URL
	tokens.push_back(aux.substr(lastPos, pos - lastPos)); //Metemos el e-mail en tokens

	return true;
}

bool Tokenizador::Acronimos(string::size_type& pos, string::size_type& lastPos,string::size_type& posAcron,const string& aux, const string& delimitadores, list<string>& tokens)const{
	string delimaux=delimitadores;
	string::size_type final=posAcron;
	string::size_type prepunto= aux.find_first_of(delimaux+".",1+posAcron);//Si hay un punto antes del acrónimo
	if(posAcron==lastPos){
		if(aux.substr(prepunto,aux.find_first_not_of(delimaux+".",prepunto)-prepunto)=="."){
			lastPos = aux.find_first_not_of(delimaux+".", lastPos);
		}
	}
	//Cuerpo principal del acrónimo
	while(aux.substr(final, aux.find_first_not_of(delimaux+".",final)-final)=="."){
		final=aux.find_first_of(delimaux+".",aux.find_first_not_of(delimaux+".",final));
		if (string::npos == final && string::npos!=lastPos){//Al iniciar lastPos será 0, pero si pos es npos dará error, por lo que lo cambiamos al length del string
			final=aux.length();
		}
	}
	//Metemos en la lista de tokens el acrónimo
	tokens.push_back(aux.substr(lastPos, final - lastPos));//Introduce palabra.palabra.palabra ..... mientras cumpla con la condición
	//quitamos los puntos finales
	final=aux.find_first_not_of(".",final);

	pos=final;
	return true;

}

bool Tokenizador::Guiones(string::size_type& pos, string::size_type& lastPos,const string& aux, const string& delimitadores, list<string>& tokens)const{
	string delimaux=delimitadores;

	string::size_type final=pos;
	while(aux.substr(final, aux.find_first_not_of(delimaux+"-",final)-final)=="-"){
		final=aux.find_first_of(delimaux,aux.find_first_not_of(delimaux+"-",final));
		if (string::npos == final && string::npos!=lastPos){//Al iniciar lastPos será 0, pero si pos es npos dará error, por lo que lo cambiamos al length del string
			final=aux.length();
		}
	}
	tokens.push_back(aux.substr(lastPos, final - lastPos));//Introduce palabra-palabra-palabra ..... mientras cumpla con la condición
	pos=final;

	return true;

}

