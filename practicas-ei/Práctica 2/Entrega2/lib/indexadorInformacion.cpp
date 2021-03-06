/*
 * indexadorInformacion.cpp
 *
 *  Created on: 11 de abr. de 2016
 *      Author: Alejandro
 */

#include "indexadorInformacion.h"

//Clase InformacionTermino
InformacionTermino::InformacionTermino(const InformacionTermino& it) {
	ftc= it.ftc;
	l_docs=it.l_docs;
}

InformacionTermino::InformacionTermino() {
	ftc=0;
}

InformacionTermino::~InformacionTermino() {
	ftc=0;
	l_docs.clear();//Comprobar
}

InformacionTermino::InformacionTermino(int ftc,unordered_map<long int, InfTermDoc>& l_doc) {
	setFtc(ftc);
	setDocs(l_doc);
}

InformacionTermino& InformacionTermino::operator =(const InformacionTermino& it) {
	if(this!=&it){
		//(*this).~InformacionTermino();
		ftc=it.ftc;
//		l_docs.clear();
		l_docs=it.l_docs;
	}
	return *this;
}

ostream& operator<<(ostream& s, const InformacionTermino& p) {
	//unordered_map<long int, InfTermDoc>::iterator it;
	s << "Frecuencia total: " << p.ftc << "\tfd: " << p.l_docs.size();
	// A continuación se mostrarían todos los elementos de p.l_docs:
	for(auto it=p.l_docs.begin();it!=p.l_docs.end();++it){
		s << "\tId.Doc: " << it->first << "\t" << it->second;//Necesario preguntar tiene k mostrar el id del documento y la inftermdoc
	}
	return s;
}

//Clase InfTermDoc
InfTermDoc::InfTermDoc(const InfTermDoc& itd) {
	ft= itd.ft;
	posTerm= itd.posTerm;
}

InfTermDoc::InfTermDoc() {
	ft=0;
}

InfTermDoc::~InfTermDoc() {
	ft=0;
	posTerm.clear();
}

InfTermDoc::InfTermDoc(int ft, const list<int>& posTerm) {
	this->ft=ft;
	this->posTerm=posTerm;
}

InfTermDoc& InfTermDoc::operator =(const InfTermDoc& itd) {
	if(this!=&itd){
		//(*this).~InfTermDoc();
		ft=itd.ft;
		posTerm=itd.posTerm;
	}
	return *this;
}

ostream& operator<<(ostream& s, const InfTermDoc& p) {
	InfTermDoc itd =p;
	s << "ft: " << p.ft;
	// A continuación se mostrarían todos los elementos de p.posTerm ("posicion TAB posicion TAB ... posicion, es decir nunca finalizará en un TAB"):
	while(!itd.posTerm.empty()){
		s <<"\t" << itd.posTerm.front();
		itd.posTerm.pop_front();
	}
	return s;
}

//Clase InfDoc
InfDoc::InfDoc(const InfDoc& id) {
	fechaModificacion= id.fechaModificacion;
	idDoc=id.idDoc;
	numPal=id.numPal;
	numPalDiferentes=id.numPalDiferentes;
	numPalSinParada=id.numPalSinParada;
	tamBytes= id.tamBytes;
}

InfDoc::InfDoc() {
	fechaModificacion = Fecha();//Tiene que especificarlo el documento?
	idDoc=0;
	numPal=0;
	numPalDiferentes=0;
	numPalSinParada=0;
	tamBytes= 0;
}

InfDoc::~InfDoc() {
	idDoc=0;
	numPal=0;
	numPalDiferentes=0;
	numPalSinParada=0;
	tamBytes= 0;
	//fechaModificacion.~Fecha();
}

InfDoc::InfDoc(long int idDoc, int numPal, int numPalSinParada,int numPalDiferentes, int tamBytes, Fecha& fechaModificacion) {
	this->fechaModificacion=fechaModificacion;
	this->idDoc=idDoc;
	this->numPal=numPal;
	this->numPalDiferentes=numPalDiferentes;
	this->numPalSinParada=numPalSinParada;
	this->tamBytes=tamBytes;
}

InfDoc& InfDoc::operator =(const InfDoc& id) {
	if(this!=&id){
		//(*this).~InfDoc();
		fechaModificacion= id.fechaModificacion;
		idDoc=id.idDoc;
		numPal=id.numPal;
		numPalDiferentes=id.numPalDiferentes;
		numPalSinParada=id.numPalSinParada;
		tamBytes= id.tamBytes;
	}
	return *this;
}

ostream& operator<<(ostream& s, const InfDoc& p) {
	s << "idDoc: " << p.idDoc << "\tnumPal: " << p.numPal <<
	"\tnumPalSinParada: " << p.numPalSinParada << "\tnumPalDiferentes: " <<
	p.numPalDiferentes << "\ttamBytes: " << p.tamBytes;
	return s;
}

//Clase InfColeccionDocs
InfColeccionDocs::InfColeccionDocs(const InfColeccionDocs& icd) {
	numDocs= icd.numDocs;
	numTotalPal=icd.numTotalPal;
	numTotalPalDiferentes=icd.numTotalPalDiferentes;
	numTotalPalSinParada=icd.numTotalPalSinParada;
	tamBytes=icd.tamBytes;
}

InfColeccionDocs::InfColeccionDocs() {
	numDocs= 0;
	numTotalPal=0;
	numTotalPalDiferentes=0;
	numTotalPalSinParada=0;
	tamBytes=0;
}

InfColeccionDocs::~InfColeccionDocs() {
	numDocs=0;
	numTotalPal=0;
	numTotalPalDiferentes=0;
	numTotalPalSinParada=0;
	tamBytes=0;
}

InfColeccionDocs::InfColeccionDocs(long int numDocs, long int numTotalPal,long int numTotalPalSinParada, long int numTotalPalDiferentes,long int tamBytes) {
	this->numDocs=numDocs;
	this->numTotalPal=numTotalPal;
	this->numTotalPalDiferentes=numTotalPalDiferentes;
	this->numTotalPalSinParada=numTotalPalSinParada;
	this->tamBytes=tamBytes;
}

InfColeccionDocs& InfColeccionDocs::operator =(const InfColeccionDocs& icd) {
	if(this!=&icd){
		//(*this).~InfColeccionDocs();
		numDocs= icd.numDocs;
		numTotalPal=icd.numTotalPal;
		numTotalPalDiferentes=icd.numTotalPalDiferentes;
		numTotalPalSinParada=icd.numTotalPalSinParada;
		tamBytes=icd.tamBytes;
	}
	return *this;
}

ostream& operator<<(ostream& s, const InfColeccionDocs& p){
	s << "numDocs: " << p.numDocs << "\tnumTotalPal: " << p.numTotalPal <<
	"\tnumTotalPalSinParada: " << p.numTotalPalSinParada <<
	"\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes << "\ttamBytes: " <<
	p.tamBytes;
	return s;
}

//Clase InformacionTerminoPregunta
InformacionTerminoPregunta::InformacionTerminoPregunta(const InformacionTerminoPregunta& itp) {
	ft=itp.ft;
	posTerm=itp.posTerm;
}

InformacionTerminoPregunta::InformacionTerminoPregunta() {
	ft=0;
}

InformacionTerminoPregunta::~InformacionTerminoPregunta() {
	ft=0;
	posTerm.clear();
}

InformacionTerminoPregunta::InformacionTerminoPregunta(int ft,list<int>& posTerm) {
	this->ft=ft;
	this->posTerm=posTerm;
}

InformacionTerminoPregunta& InformacionTerminoPregunta::operator =(const InformacionTerminoPregunta& itp) {
	if(this!=&itp){
		//(*this).~InformacionTerminoPregunta();
		ft=itp.ft;
		posTerm=itp.posTerm;
	}
	return *this;
}

ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p) {
	InformacionTerminoPregunta itp=p;
	s << "ft: " << p.ft;
	// A continuación se mostrarían todos los elementos de p.posTerm
	// ("posicion TAB posicion TAB ... posicion, es decir nunca finalizará en un TAB"):
	while(!itp.posTerm.empty()){
		s <<"\t" << itp.posTerm.front();
		itp.posTerm.pop_front();
	}
	return s;
}

//Clase InformacionPregunta
InformacionPregunta::InformacionPregunta(const InformacionPregunta& ip) {
	numTotalPal= ip.numTotalPal;
	numTotalPalDiferentes=ip.numTotalPalDiferentes;
	numTotalPalSinParada=ip.numTotalPalSinParada;
}

InformacionPregunta::InformacionPregunta() {
	numTotalPal=0;
	numTotalPalDiferentes=0;
	numTotalPalSinParada=0;
}

InformacionPregunta::~InformacionPregunta() {
	numTotalPal=0;
	numTotalPalDiferentes=0;
	numTotalPalSinParada=0;
}

InformacionPregunta::InformacionPregunta(long int numTotalPal,	long int numTotalPalSinParada, long int numTotalPalDiferentes) {
	this->numTotalPal=numTotalPal;
	this->numTotalPalDiferentes=numTotalPalDiferentes;
	this->numTotalPalSinParada=numTotalPalSinParada;
}

InformacionPregunta& InformacionPregunta::operator =(const InformacionPregunta& ip) {
	if(this!=&ip){
		//(*this).~InformacionPregunta();
		numTotalPal= ip.numTotalPal;
		numTotalPalDiferentes=ip.numTotalPalDiferentes;
		numTotalPalSinParada=ip.numTotalPalSinParada;
	}
	return *this;
}

ostream& operator<<(ostream& s, const InformacionPregunta& p){
	s << "numTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: "<< p.numTotalPalSinParada <<
	"\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes;
	return s;
}

//Clase Fecha
Fecha::Fecha(const Fecha& f) {
	hora=f.hora;
	min=f.min;
	seg=f.seg;
	dia=f.dia;
	mes=f.mes;
	anyo=f.anyo;
}

Fecha::Fecha() {
	time_t a;
	tm * tiempo;
	time(&a);
	tiempo= localtime(&a);
	dia=tiempo->tm_mday;
	mes=tiempo->tm_mon;
	++mes;
	anyo=tiempo->tm_year;
	anyo+=1900;
	hora=tiempo->tm_hour;
	min=tiempo->tm_min;
	seg=tiempo->tm_sec;
}
Fecha::Fecha(const time_t& t) {
	time_t a=t;
	tm * tiempo;
	time(&a);
	tiempo= localtime(&a);
	dia=tiempo->tm_mday;
	mes=tiempo->tm_mon;
	++mes;
	anyo=tiempo->tm_year;
	anyo+=1900;
	hora=tiempo->tm_hour;
	min=tiempo->tm_min;
	seg=tiempo->tm_sec;
}

Fecha::~Fecha() {
	hora=0;
	min=0;
	seg=0;
	dia=1;
	mes=1;
	anyo=1900;
}

Fecha::Fecha(int dia, int mes, int anyo, int hora, int min, int seg) {//Necesito algo para poder hacer try-catch
	setDia(dia);
	setMes(mes);
	setAnyo(anyo);
	setHora(hora);
	setMin(min);
	setSeg(seg);
}

Fecha& Fecha::operator =(const Fecha& f) {
	if(this!=&f){
		//(*this).~Fecha();
		hora=f.hora;
		min=f.min;
		seg=f.seg;
		dia=f.dia;
		mes=f.mes;
		anyo=f.anyo;
	}
	return *this;
}

ostream& operator <<(ostream& s, const Fecha& p){
	//Muestra la fecha
	s << "Fecha: " << p.dia << "/"<< p.mes << "/" <<p.anyo <<" ";
	//Muestra la hora
	if (p.hora<10){
		s << "0" << p.hora <<":";
	}else{s << p.hora << ":";}
	//Muestra los minutos
	if (p.min<10){
		s << "0" << p.min <<":";
	}else{s << p.min << ":";}
	//Muestra los segundos
	if (p.seg<10){
		s << "0" << p.seg;
	}else{s << p.seg;}

	return s;
}

//Devuelve true si a es posterior a this
bool Fecha::Posterior(const Fecha& a) const {
	//Fecha b(*this);
	bool posterior=false;
	if(a.anyo==anyo){//Si a es mayor o igual a b
		if(a.mes==mes){
			if(a.dia==dia){
				if(a.hora==hora){
					if(a.min==min){
						if(a.seg>seg){
							posterior= true;
						}
					}
					else if(a.min>min){
						posterior= true;
					}
				}
				else if(a.hora>hora){
					posterior= true;
				}
			}
			else if(a.dia>dia){
				posterior= true;
			}
		}
		else if(a.mes>mes){
			posterior= true;
		}
	}
	else if(a.anyo>anyo){
		posterior= true;
	}
	return posterior;
}

void InfTermDoc::Append(list<int>& lista) {
	while(!lista.empty()){
		posTerm.push_back(lista.front());
		lista.pop_front();
	}
}

void InformacionTermino::AppendDocs(const unordered_map<long int, InfTermDoc>& l_docs_local) {
	list<int> lista;
	for(auto i=l_docs_local.begin();i!=l_docs_local.end();++i){
		//Si ya existe lo actualizas
		if(l_docs.find(i->first)!=l_docs.end()){//Si ya existía el documento
			lista=i->second.getPosTerm();
			l_docs.find(i->first)->second.Append(lista);
			l_docs.find(i->first)->second.setFt(l_docs.find(i->first)->second.getFt()+ i->second.getFt());
		}
		else{
			l_docs.insert({i->first,i->second});
		}
	}
}


