/*
 * indexadorInformacion.h
 *
 *  Created on: 11 de abr. de 2016
 *      Author: Alejandro
 */

#ifndef INDEXADORINFORMACION_H_
#define INDEXADORINFORMACION_H_

#include "tokenizador.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "time.h"

using namespace std;

//Clase Fecha
class Fecha{
	friend ostream& operator<<(ostream &s, const Fecha& p);
	public:
		Fecha(const Fecha& f);
		Fecha(int dia,int mes,int anyo,int hora,int min,int seg);
		Fecha(const time_t& t);
		Fecha();//Inicializa la fecha a la actual del sistema
		~Fecha();//Pone la fecha al 1/1/1900 00:00:00
		Fecha& operator= (const Fecha& f);
		bool Posterior(const Fecha& a) const;//Devuelve true si a es posterior a this
		int getAnyo() const {
			return anyo;
		}
		void setAnyo(int anyo) {
			if(anyo>=2000){
				this->anyo = anyo;
			}
			else{
				cerr<< "El anyo debe ser mayor o igual a 2000" << endl;
			}
		}
		int getDia() const {
			return dia;
		}
		void setDia(int dia) {
			if(dia>=1 && dia <= 31){
				switch(this->mes){
					case 2:
						if(anyo%400==0){
							if(dia<=29){ this->dia=dia;}
							else{
								cerr<< "Los dias deben ir dentro del rango [1-29]" << endl;
							}
						}
						else if(anyo%4==0 && anyo%100!=0){
							if(dia<=29){ this->dia=dia;}
							else{
								cerr<< "Los dias deben ir dentro del rango [1-29]" << endl;
							}
						}
						else{
							if(dia<=28){ this->dia=dia;}
							else{
								cerr<< "Los dias deben ir dentro del rango [1-28]" << endl;
							}
						}
						break;
					case 4:
						if(dia<=30){this->dia=dia;}
						else{
							cerr<< "Los dias deben ir dentro del rango [1-30]" << endl;
						}
						break;
					case 6:
						if(dia<=30){this->dia=dia;}
						else{
							cerr<< "Los dias deben ir dentro del rango [1-30]" << endl;
						}
						break;
					case 9:
						if(dia<=30){this->dia=dia;}
						else{
							cerr<< "Los dias deben ir dentro del rango [1-30]" << endl;
						}
						break;
					case 11:
						if(dia<=30){this->dia=dia;}
						else{
							cerr<< "Los dias deben ir dentro del rango [1-30]" << endl;
						}
						break;
					default:
						this->dia=dia;
						break;
				}
			}
			else{
				cerr<< "Los dias deben ir dentro del rango [1-31]" << endl;
			}
		}
		int getHora() const {
			return hora;
		}
		void setHora(int hora) {
			if(hora>=0 && hora<=23){
				this->hora = hora;
			}
			else{
				cerr<< "Las horas tienen que estar en el rango [0-23]" << endl;
			}
		}
		int getMes() const {
			return mes;
		}
		void setMes(int mes) {
			if(mes>=1 && mes<=12){
				this->mes = mes;
			}
			else{
				cerr<< "Los meses tienen que estar en el rango [1-12]" << endl;
			}
		}
		int getMin() const {
			return min;
		}
		void setMin(int min) {
			if(min>=0 && min <=59){
				this->min = min;
			}
			else{
				cerr<< "Los minutos tienen que estar en el rango [0-59]" << endl;
			}
		}
		int getSeg() const {
			return seg;
		}
		void setSeg(int seg) {
			if(seg>=0 && seg <=59){
				this->seg = seg;
			}
			else{
				cerr<< "Los segundos tienen que estar en el rango [0-59]" << endl;
			}
		}
	private:
		int dia;//Dia
		int mes;//Mes
		int anyo;//Año
		int hora;//Hora
		int min;//minuto
		int seg;//Segundo
};

//Clase InfTermDoc
class InfTermDoc {
	friend ostream& operator<<(ostream& s, const InfTermDoc& p);
	public:
		InfTermDoc (const InfTermDoc& itd);
		InfTermDoc (int ft,const list<int>& posTerm);
		InfTermDoc (); // Inicializa ft = 0
		~InfTermDoc (); // Pone ft = 0
		InfTermDoc& operator= (const InfTermDoc& itd);
		void Append(list<int>& lista);
		int getFt() const {
			return ft;
		}
		void setFt(int ft) {
			this->ft = ft;
		}
		const list<int>& getPosTerm() const {
			return posTerm;
		}
		void setPosTerm(const list<int>& posTerm) {
			this->posTerm = posTerm;
		}
		// Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
	private:
		int ft; // Frecuencia del término en el documento
		list<int> posTerm;
		// Solo se almacenará esta información si el campo privado del indexador almacenarPosTerm == true
		// Lista de números de palabra en los que aparece el término en el documento.
		//Los números de palabra comenzarán desde cero (la primera palabra del documento).
		// Se numerarán las palabras de parada. Estará ordenada de menor a mayor posición.
};

//Clase InformacionTermino
class InformacionTermino {
	friend ostream& operator<<(ostream& s, const InformacionTermino& p);
	public:
		InformacionTermino (const InformacionTermino& it);
		InformacionTermino (int ftc, unordered_map<long int,InfTermDoc>& l_doc);
		InformacionTermino (); // Inicializa ftc = 0
		~InformacionTermino (); // Pone ftc = 0 y vacía l_docs
		InformacionTermino& operator= (const InformacionTermino& it);
		void AppendDocs(const unordered_map<long int,InfTermDoc>& l_docs_local);
		int getFtc() const {
			return ftc;
		}
		void setFtc(int ftc) {
			this->ftc = ftc;
		}
		const unordered_map<long int, InfTermDoc>& getDocs() const {
			return l_docs;
		}
		void setDocs(const unordered_map<long int, InfTermDoc>& docs) {
			this->l_docs=docs;
			/*l_docs.clear();
			for(auto i=docs.begin();i!=docs.end();++i){
				l_docs[i->first]=i->second;
			}*/
		}
		// Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
	private:
		int ftc; // Frecuencia total del término en la colección
		unordered_map<long int, InfTermDoc> l_docs;
		// Tabla Hash que se accederá por el id del documento, devolviendo un objeto de
		// la clase InfTermDoc que contiene toda la información de aparición del término en el documento
};

//Clase InfDoc
class InfDoc {
	friend ostream& operator<<(ostream& s, const InfDoc& p);
	public:
		InfDoc (const InfDoc& id);
		InfDoc (long int idDoc, int numPal, int numPalSinParada, int numPalDiferentes, int tamBytes, Fecha& fechaModificacion);
		InfDoc ();
		~InfDoc ();
		InfDoc& operator= (const InfDoc& id);
		const Fecha& getFechaModificacion() const {
			return fechaModificacion;
		}
		void setFechaModificacion(const Fecha& fechaModificacion) {
			this->fechaModificacion = fechaModificacion;
		}
		long int getIdDoc() const {
			return idDoc;
		}
		void setIdDoc(long int idDoc) {
			this->idDoc = idDoc;
		}
		int getNumPal() const {
			return numPal;
		}
		void setNumPal(int numPal) {
			this->numPal = numPal;
		}
		int getNumPalDiferentes() const {
			return numPalDiferentes;
		}
		void setNumPalDiferentes(int numPalDiferentes) {
			this->numPalDiferentes = numPalDiferentes;
		}
		int getNumPalSinParada() const {
			return numPalSinParada;
		}
		void setNumPalSinParada(int numPalSinParada) {
			this->numPalSinParada = numPalSinParada;
		}
		int getTamBytes() const {
			return tamBytes;
		}
		void setTamBytes(int tamBytes) {
			this->tamBytes = tamBytes;
		}
		// Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
	private:
		long int idDoc;
		// Identificador del documento. El primer documento indexado en la colección será
		// el identificador 1
		int numPal; // Nº total de palabras del documento
		int numPalSinParada; // Nº total de palabras sin stop-words del documento
		int numPalDiferentes;
		// Nº total de palabras diferentes que no sean stop-words (sin acumular la frecuencia
		// de cada una de ellas)
		int tamBytes; // Tamaño en bytes del documento
		Fecha fechaModificacion;
		// Atributo correspondiente a la fecha y hora de modificación del documento.
		// El tipo "Fecha/hora" lo elegirá/implementará el alumno
};

//Clase InfColeccionDocs
class InfColeccionDocs {
	friend ostream& operator<<(ostream& s, const InfColeccionDocs& p);
	public:
		InfColeccionDocs (const InfColeccionDocs& icd);
		InfColeccionDocs (long int numDocs, long int numTotalPal, long int numTotalPalSinParada,long int numTotalPalDiferentes,long int tamBytes);
		InfColeccionDocs ();
		~InfColeccionDocs ();
		InfColeccionDocs& operator= (const InfColeccionDocs& icd);
		long int getNumDocs() const {
			return numDocs;
		}
		void setNumDocs(long int numDocs) {
			this->numDocs = numDocs;
		}
		long int getNumTotalPal() const {
			return numTotalPal;
		}
		void setNumTotalPal(long int numTotalPal) {
			this->numTotalPal = numTotalPal;
		}
		long int getNumTotalPalDiferentes() const {
			return numTotalPalDiferentes;
		}
		void setNumTotalPalDiferentes(long int numTotalPalDiferentes) {
			this->numTotalPalDiferentes = numTotalPalDiferentes;
		}
		long int getNumTotalPalSinParada() const {
			return numTotalPalSinParada;
		}
		void setNumTotalPalSinParada(long int numTotalPalSinParada) {
			this->numTotalPalSinParada = numTotalPalSinParada;
		}
		long int getTamBytes() const {
			return tamBytes;
		}
		void setTamBytes(long int tamBytes) {
			this->tamBytes = tamBytes;
		}
		// Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
	private:
		long int numDocs; // Nº total de documentos en la colección
		long int numTotalPal;
		// Nº total de palabras en la colección
		long int numTotalPalSinParada;
		// Nº total de palabras sin stop-words en la colección
		long int numTotalPalDiferentes;
		// Nº total de palabras diferentes en la colección que no sean stopwords
		// (sin acumular la frecuencia de cada una de ellas)
		long int tamBytes; // Tamaño total en bytes de la colección
};

//Clase InformacionTerminoPregunta
class InformacionTerminoPregunta {
	friend ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p);
	public:
		InformacionTerminoPregunta (const InformacionTerminoPregunta& itp);
		InformacionTerminoPregunta (int ft, list<int>& posTerm);
		InformacionTerminoPregunta ();
		~InformacionTerminoPregunta ();
		InformacionTerminoPregunta& operator= (const InformacionTerminoPregunta& itp);
		int getFt() const {
			return ft;
		}
		void setFt(int ft) {
			this->ft = ft;
		}
		const list<int>& getPosTerm() const {
			return posTerm;
		}
		void setPosTerm(const list<int>& posTerm) {
			this->posTerm = posTerm;
		}
		// Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
	private:
		int ft; // Frecuencia total del término en la pregunta
		list<int> posTerm;
		// Solo se almacenará esta información si el campo privado del indexador almacenarPosTerm == true
		// Lista de números de palabra en los que aparece el término en la pregunta.
		// Los números de palabra comenzarán desde cero (la primera palabra de la pregunta).
		// Se numerarán las palabras de parada. Estará ordenada de menor a mayor posición.
};

//Clase InformacionPregunta
class InformacionPregunta {
	friend ostream& operator<<(ostream& s, const InformacionPregunta& p);
	public:
		InformacionPregunta (const InformacionPregunta& ip);
		InformacionPregunta (long int numTotalPal, long int numTotalPalSinParada, long int numTotalPalDiferentes);
		InformacionPregunta ();
		~InformacionPregunta ();
		InformacionPregunta& operator= (const InformacionPregunta& ip);
		long int getNumTotalPal() const {
			return numTotalPal;
		}
		void setNumTotalPal(long int numTotalPal) {
			this->numTotalPal = numTotalPal;
		}
		long int getNumTotalPalDiferentes() const {
			return numTotalPalDiferentes;
		}
		void setNumTotalPalDiferentes(long int numTotalPalDiferentes) {
			this->numTotalPalDiferentes = numTotalPalDiferentes;
		}
		long int getNumTotalPalSinParada() const {
			return numTotalPalSinParada;
		}
		void setNumTotalPalSinParada(long int numTotalPalSinParada) {
			this->numTotalPalSinParada = numTotalPalSinParada;
		}
		// Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
	private:
		long int numTotalPal;
		// Nº total de palabras en la pregunta
		long int numTotalPalSinParada;
		// Nº total de palabras sin stop-words en la pregunta
		long int numTotalPalDiferentes;
		// Nº total de palabras diferentes en la pregunta que no sean stop-words
		// (sin acumular la frecuencia de cada una de ellas)
};


#endif /* INDEXADORINFORMACION_H_ */

