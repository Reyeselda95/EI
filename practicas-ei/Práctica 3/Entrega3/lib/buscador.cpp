/*
 * buscador.cpp
 *
 *  Created on: 6 de may. de 2016
 *      Author: Alejandro
 */

#include "buscador.h"

//Clase ResultadoRI
ResultadoRI::ResultadoRI(const double& kvSimilitud, const long int& kidDoc, const int& np) {
	vSimilitud = kvSimilitud;
	idDoc = kidDoc;
	numPregunta = np;
}

double ResultadoRI::VSimilitud() const {
	return vSimilitud;
}

long int ResultadoRI::IdDoc() const {
	return idDoc;
}

int ResultadoRI::NumPregunta() const {
	return numPregunta;
}

bool ResultadoRI::operator <(const ResultadoRI& lhs) const {
	if(numPregunta == lhs.numPregunta)
		return (vSimilitud < lhs.vSimilitud);
	else
		return (numPregunta > lhs.numPregunta);
}
ostream& operator<<(ostream &os, const ResultadoRI &res){
	os << res.vSimilitud << "\t\t" << res.idDoc << "\t" << res.numPregunta<< endl;
	return os;
}

//Clase Buscador
Buscador::Buscador(const string& directorioIndexacion, const int& f):IndexadorHash(directorioIndexacion) {
	formSimilitud=f;
	c = 2;
	k1 = 1.2;
	b = 0.75;
}

Buscador::Buscador(const Buscador& b):IndexadorHash(b.DevolverDirIndice()) {
	this->formSimilitud=b.formSimilitud;
	this->docsOrdenados=b.docsOrdenados;
	this->c = b.c;
	this->k1 = b.k1;
	this->b = b.b;
}

Buscador::~Buscador() {
	formSimilitud=0;
	c = 2;
	k1 = 1.2;
	b = 0.75;
}

Buscador& Buscador::operator =(const Buscador& b) {
	if(this!=&b){
		this->formSimilitud=b.formSimilitud;
		this->docsOrdenados=b.docsOrdenados;
		this->c = b.c;
		this->k1 = b.k1;
		this->b = b.b;
	}
	return *this;
}

bool Buscador::Buscar(const int& numDocumentos) {
	string question;
	priority_queue<ResultadoRI> aux;
	//Limpiamos el indice docsOrdenados
	while(docsOrdenados.size()!=0){
		docsOrdenados.pop();
	}

	if(!DevuelvePregunta(question)){
		return false;
	}

	for(unsigned int i=1;i<=getIndiceDocs().size();++i){//Para todos los documentos indexados
		//Ejecutar búsqueda
		if(formSimilitud==0){
			//DFR
			aux.push(ResultadoRI(DFR(i),i, 0));
		}
		else{//formSimilitud==1
			//BM25
			aux.push(ResultadoRI(BM25(i),i, 0));
		}
	}
	if(!aux.empty()){
		for(int i=numDocumentos;i>0;--i){//Se almacenan únicamente los numDocumentos mas relevantes
			docsOrdenados.push(aux.top());
			aux.pop();
		}
	}
	return true;
}

bool Buscador::Buscar(const string& dirPreguntas, const int& numDocumentos,const int& numPregInicio, const int& numPregFin) {
	struct stat dir;
	ifstream file;
	string filePregunta=dirPreguntas+"/";
	string abrir,lectura;
	stringstream ss;
	priority_queue<ResultadoRI> aux;
	// Compruebo la existencia del directorio
	int err=stat(dirPreguntas.c_str(), &dir);

	if(err==-1 || !S_ISDIR(dir.st_mode)){
		return false;
	}

	//Ahora que se que el directorio existe empiezo con la lectura
	for(int i=numPregInicio;i<=numPregFin;++i){
		ss.str("");
		ss << i;
		abrir=filePregunta+ss.str()+".txt";
		file.open(abrir.c_str());
		if(file.is_open()){
			//Hacer cosas
			getline(file, lectura);
			IndexarPregunta(lectura);//indexamos una pregunta
			file.close();
			//Realizar Búsqueda
			//Limpiamos el array con los documentos analizados previamente
			while(aux.size()!=0){
				aux.pop();
			}

			for(unsigned int j=1;j<=getIndiceDocs().size();++j){//Para todos los documentos indexados
				//Ejecutar búsqueda
				if(formSimilitud==0){
					//DFR
					aux.push(ResultadoRI(DFR(j),j, i));
				}
				else{//formSimilitud==1
					//BM25
					aux.push(ResultadoRI(BM25(j),j, i));
				}

			}

			for(int i=numDocumentos;i>0;--i){//Se almacenan únicamente los numDocumentos mas relevantes
				docsOrdenados.push(aux.top());
				aux.pop();
			}
		}
	}
	return true;
}

void Buscador::ImprimirResultadoBusqueda(const int& numDocumentos) const {
	priority_queue<ResultadoRI> DO(docsOrdenados);
	int anterior;
	while(!DO.empty()){
		anterior=DO.top().NumPregunta();
		for(int i=0;i<numDocumentos;++i){
			if(DO.top().NumPregunta()!=anterior){
				i=numDocumentos;
			}else{
				anterior=DO.top().NumPregunta();
				cout << anterior << " ";
				if(formSimilitud==0){
					cout << "DFR ";
				}
				else{
					cout << "BM25 ";
				}
				cout << SacaNombre(DO.top().IdDoc())<< " "  << i << " " << DO.top().VSimilitud() << " " ;
				if(DO.top().NumPregunta()==0){
					cout << getPregunta() << endl;
				}
				else{
					cout << "ConjuntoDePreguntas" << endl;
				}
				DO.pop();
			}
		}
	}
}

bool Buscador::ImprimirResultadoBusqueda(const int& numDocumentos, const string& nombreFichero) const {
	priority_queue<ResultadoRI> DO(docsOrdenados);
	int anterior;
	ofstream ofile;
	ofile.open(nombreFichero.c_str());
	if(ofile.is_open()){
		while(!DO.empty()){
			anterior=DO.top().NumPregunta();
			for(int i=0;i<numDocumentos;++i){
				if(DO.top().NumPregunta()!=anterior){
					i=numDocumentos;
				}
				else{
					anterior=DO.top().NumPregunta();
					ofile << anterior << " ";
					if(formSimilitud==0){
						ofile << "DFR ";
					}
					else{
						ofile << "BM25 ";
					}
					ofile << SacaNombre(DO.top().IdDoc())<< " "  << i << " " << DO.top().VSimilitud() << " " ;
					if(DO.top().NumPregunta()==0){
						ofile << getPregunta() << endl;
					}
					else{
						ofile << "ConjuntoDePreguntas" << endl;
					}
					DO.pop();
				}
			}
		}
		ofile.close();
	}
	else{
		return false;
	}
	return true;
}

int Buscador::DevolverFormulaSimilitud() const {
	return formSimilitud;
}

bool Buscador::CambiarFormulaSimilitud(const int& f) {
	if(f==1 || f==0){
		formSimilitud=f;
		return true;
	}
	else{
		return false;
	}
}

void Buscador::CambiarParametrosDFR(const double& kc) {
	c=kc;
}

double Buscador::DevolverParametrosDFR() const {
	return c;
}

void Buscador::CambiarParametrosBM25(const double& kk1, const double& kb) {
	k1=kk1;
	b=kb;
}

void Buscador::DevolverParametrosBM25(double& kk1, double& kb) const {
	kk1=k1;
	kb=b;
}

double Buscador::DFR(int Doc) {
	double sim=0;
	double wiq=0.1,wid=0,ft=0,nt=0,ftd=0,freq=0,ld=0,avr_ld=0;
	InformacionTermino it;
	for(auto i=getIndiceDocs().begin();i!=getIndiceDocs().end();++i){
		if(i->second.getIdDoc()==Doc){
			ld=i->second.getTamBytes();//tamaño del documento
		}
		avr_ld+=i->second.getTamBytes();
	}
	avr_ld=avr_ld/getIndiceDocs().size();//media de los tamaños de los documentos de la coleccion


	for(auto i=getIndicePregunta().begin();i!=getIndicePregunta().end();++i){//Para cada término de la pregunta
		ft=0;
		nt=0;
		if(getIndice().find(i->first)!=getIndice().end()){
			for(auto j=getIndice().find(i->first)->second.getDocs().begin();j!=getIndice().find(i->first)->second.getDocs().end();++j){
				if(j->first==Doc){//Si es el documento
					freq=j->second.getFt();//Veces que aparece el termino en el documento
				}
				ft+=j->second.getFt();//Frecuencia del término en la coleccion
				++nt;
			}
		}
		else{
			freq=0;
			ft=0;
		}

		ftd=freq*((log10(1+((c*avr_ld)/ld))/log10(2)));

		if(nt!=0){
			wid=((log10(1+(ft/getIndiceDocs().size()))/log10(2)) + (ftd*(log10((1+(ft/getIndiceDocs().size()))/(ft/getIndiceDocs().size()))/log10(2)))) * ((ft+1)/(nt*(ftd+1)));
		}
		else{
			wid=0;
		}
		wiq=(double)i->second.getFt()/getIndicePregunta().size();
		sim+=wiq*wid;
	}
	return sim;
}

double Buscador::BM25(int Doc) {
	double sim=0;
	double idf,numpal,medianumpal;

	for(auto i=getIndiceDocs().begin();i!=getIndiceDocs().end();++i){
		if(i->second.getIdDoc()==Doc){
			numpal=i->second.getNumPalSinParada();
		}
		medianumpal+=i->second.getNumPalSinParada();
	}
	medianumpal=medianumpal/getIndiceDocs().size();

	InformacionTermino it;
	for(auto i=getIndicePregunta().begin();i!=getIndicePregunta().end();++i){//Para cada término de la pregunta
		if(getIndice().find(i->first)!=getIndice().end()){
			it=getIndice().find(i->first)->second;
			idf=log10((getIndiceDocs().size()-(it.getDocs().size()+0.5))/(it.getDocs().size()+0.5));
			if(it.getDocs().find(Doc)!=it.getDocs().end()){
				sim+=fabs(idf*((it.getDocs().find(Doc)->second.getFt()*(k1+1))/(it.getDocs().find(Doc)->second.getFt()+(k1*(1-b+(b*(numpal/medianumpal)))))));
			}
			else{
				//Numeros absolutos --> fabs(num);
				sim+=fabs(idf*(0*(k1+1))/(0+(k1*(1-b+(b*(numpal/medianumpal))))));
			}
		}

	}
	return sim;
}

string Buscador::SacaNombre(long int id) const {//Devuelve el nombre sin extension de un documento a partir de una id
	string name;
	list<string> tokens;

	for(auto it=getIndiceDocs().begin();it!=getIndiceDocs().end();++it){//Obtengo ruta+nombre+extension
		if(it->second.getIdDoc()==id){
			name=it->first;
		}
	}
	Tokenizador tokenizer("\\/",false,false);
    tokenizer.Tokenizar(name, tokens);
    auto nombre=tokens.end();//Obtiene el último elemento de la ruta
	--nombre;
	name=*nombre;
	tokenizer.DelimitadoresPalabra(".");
	tokenizer.Tokenizar(name,tokens);
	nombre=tokens.begin();
	return *nombre;
}

Buscador::Buscador() {
	formSimilitud=0;
	c=2;
	k1=1.2;
	b=0.75;
}
