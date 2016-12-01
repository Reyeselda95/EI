/*
 * indexadorHash.h
 *
 *  Created on: 14 de abr. de 2016
 *      Author: Alejandro
 */

#ifndef INDEXADORHASH_H_
#define INDEXADORHASH_H_

#include "indexadorInformacion.h"
#include "stemmer.h"
#include <sys/stat.h>

using namespace std;

class IndexadorHash {
	friend ostream& operator<<(ostream& s, const IndexadorHash& p) {
		s << "Fichero con el listado de palabras de parada: " << p.ficheroStopWords << endl;
		s << "Tokenizador: " << p.tok << endl;
		s << "Directorio donde se almacenara el indice generado: " << p.directorioIndice << endl;
		s << "Stemmer utilizado: " << p.tipoStemmer << endl;
		s << "Informacion de la coleccion indexada: " << p.informacionColeccionDocs << endl;
		s << "Se almacenara parte del indice en disco duro: " << p.almacenarEnDisco << endl;
		s << "Se almacenaran las posiciones de los terminos: " << p.almacenarPosTerm;
		return s;
	}
	public:
		/** "fichStopWords" ser� el nombre del archivo que contendr� todas las palabras de parada
		 *  (una palabra por cada l�nea del fichero) y se almacenar� en el campo privado "ficheroStopWords".
		 *  Asimismo, almacenar� todas las palabras de parada que contenga el archivo en el campo privado
		 *  "stopWords", el �ndice de palabras de parada.
		 */
		/** "delimitadores" ser� el string que contiene todos los delimitadores utilizados
		 * por el tokenizador (campo privado "tok")
		 */
		/** "dirIndice" ser� el directorio del disco duro donde se almacenar� el �ndice (campo privado "directorioIndice").
		 * Si dirIndice="" entonces se almacenar� en el directorio donde se ejecute el programa
		 */
		// "tStemmer" inicializar� la variable privada "tipoStemmer":
			// 0 = no se aplica stemmer: se indexa el t�rmino tal y como aparece tokenizado
			// 1 = stemmer de Porter para espa�ol
			// 2 = stemmer de Porter para ingl�s
		// "almEnDisco" inicializar� la variable privada "almacenarEnDisco"
		// "almPosTerm" inicializar� la variable privada "almacenarPosTerm"
		// Los �ndices (p.ej. �ndice, indiceDocs e informacionColeccionDocs)quedar�n vac�os
		IndexadorHash(const string& fichStopWords, const string& delimitadores,const bool& detectComp, const bool& minuscSinAcentos, const string&
		dirIndice, const int& tStemmer, const bool& almEnDisco, const bool& almPosTerm);
		/** Constructor para inicializar IndexadorHash a partir de una indexaci�n previamente realizada
		 * que habr� sido almacenada en "directorioIndexacion" mediante el m�todo "bool GuardarIndexacion()".
		 * Con ello toda la parte privada se inicializar� convenientemente, igual que si se acabase de indexar la colecci�n de documentos.
		 * En caso que no exista el directorio o que no contenga los datos de la indexaci�n se tratar� la excepci�n correspondiente
		 */
		IndexadorHash(const string& directorioIndexacion);
		IndexadorHash(const IndexadorHash& ih);
		~IndexadorHash();
		IndexadorHash& operator= (const IndexadorHash& ih);
		/** Devuelve true si consigue crear el �ndice para la colecci�n de documentos detallada
		 *  en ficheroDocumentos, el cual contendr� un nombre de documento por l�nea.
		 */
		/** Devuelve falso si no finaliza la indexaci�n (p.ej. por falta de memoria),mostrando el
		 * mensaje de error correspondiente, indicando el documento y t�rmino en el que se ha quedado.
		 */
		/** En el caso que aparezcan documentos repetidos o que ya estuviesen previamente indexados
		 * (ha de coincidir el nombre del documento y el directorio en que se encuentre),
		 * se mostrar� el mensaje de excepci�n correspondiente, y se re-indexar�n
		 * (borrar el documento previamente indexado e indexar el nuevo) en caso que la fecha de
		 * modificaci�n del documento sea m�s reciente que la almacenada previamente (class "InfDoc"
		 * campo "fechaModificacion"). Los casos de reindexaci�n mantendr�n el mismo idDoc.
		 */
		bool Indexar(const string& ficheroDocumentos);
		/** Devuelve true si consigue crear el �ndice para la colecci�n de documentos que se
		 * encuentra en el directorio (y subdirectorios que contenga) dirAIndexar
		 * (independientemente de la extensi�n de los mismos).
		 * Se considerar� que todos los documentos del directorio ser�n ficheros de texto.
		 */
		/** Devuelve falso si no finaliza la indexaci�n (p.ej. por falta de memoria o porque no
		 * exista "dirAIndexar"), mostrando el mensaje de error correspondiente,
		 * indicando el documento y t�rmino en el que se ha quedado.
		 */
		/** En el caso que aparezcan documentos repetidos o que ya estuviesen previamente indexados
		 * (ha de coincidir el nombre del documento y el directorio en que se encuentre),
		 * se mostrar� el mensaje de excepci�n correspondiente, y se re-indexar�n (borrar el documento previamente
		 * indexado e indexar el nuevo) en caso que la fecha de modificaci�n del documento sea m�s
		 * reciente que la almacenada previamente (class "InfDoc" campo "fechaModificacion").
		 * Los casos de reindexaci�n mantendr�n el mismo idDoc.
		 */
		bool IndexarDirectorio(const string& dirAIndexar);
		/** Se guardar� en disco duro (directorio contenido en la variable privada "directorioIndice")
		 *  la indexaci�n actualmente en memoria (incluidos todos los par�metros de la parte privada).
		 *  La forma de almacenamiento la determinar� el alumno.
		 *  El objetivo es que esta indexaci�n se pueda recuperar posteriormente mediante el
		 *  constructor "IndexadorHash(const string& directorioIndexacion)".
		 *  Por ejemplo, supongamos que se ejecuta esta secuencia de comandos: "IndexadorHash
		 *  a("./fichStopWords.txt", "[ ,.", "./dirIndexPrueba", 0, false);
		 *  a.Indexar("./fichConDocsAIndexar.txt");
		 *  a.GuardarIndexacion();
		 *  Entonces mediante el comando:
		 *  IndexadorHash b("./dirIndexPrueba");
		 *  se recuperar� la indexaci�n realizada en la secuencia anterior, carg�ndola en "b"
		 */
		/** Devuelve falso si no finaliza la operaci�n (p.ej. por falta de memoria,
		 * o el nombre del directorio contenido en "directorioIndice" no es correcto),
		 * mostrando el mensaje de error correspondiente
		 */
		// En caso que no existiese el directorio directorioIndice, habr�a que crearlo previamente
		bool GuardarIndexacion() const;
		/** Vac�a la indexaci�n que tuviese en ese momento e inicializa IndexadorHash a partir de
		 * una indexaci�n previamente realizada que habr� sido almacenada en "directorioIndexacion"
		 * mediante el m�todo "bool GuardarIndexacion()".
		 * Con ello toda la parte privada se inicializar� convenientemente, igual que si se acabase
		 * de indexar la colecci�n de documentos.
		 * En caso que no exista el directorio o que no contenga los datos de la indexaci�n se
		 * tratar� la excepci�n correspondiente, y se devolver� false
		 */
		bool RecuperarIndexacion (const string& directorioIndexacion);
		void ImprimirIndexacion() const {
			cout << "Terminos indexados: " << endl;
			/** A continuaci�n aparecer� un listado del contenido del campo privado "�ndice"
			 * donde para cada t�rmino indexado se imprimir�:
			 */
			ListarTerminos();
			cout << "Documentos indexados: " << endl;
			/** A continuaci�n aparecer� un listado del contenido del campo privado "indiceDocs"
			 * donde para cada documento indexado se imprimir�:
			 */
			ListarDocs();
		}
		/** Devuelve true si consigue crear el �ndice para la pregunta "preg".
		 * Antes de realizar la indexaci�n vaciar� los campos privados indicePregunta e infPregunta
		 */
		/** Generar� la misma informaci�n que en la indexaci�n de documentos, pero dej�ndola toda
		 * accesible en memoria principal (mediante las variables privadas "pregunta,
		 * indicePregunta, infPregunta")
		 */
		/** Devuelve falso si no finaliza la operaci�n (p.ej. por falta de memoria o bien si la
		 * pregunta no contiene ning�n t�rmino con contenido), mostrando el mensaje de error
		 * correspondiente
		 */
		bool IndexarPregunta(const string& preg);
		/** Devuelve true si hay una pregunta indexada (con al menos un t�rmino que no sea palabra
		 * de parada, o sea, que haya alg�n t�rmino indexado en indicePregunta),
		 * devolvi�ndola en "preg"
		 */
		bool DevuelvePregunta(string& preg) const;
		/** Devuelve true si word est� indexado en la pregunta, devolviendo su informaci�n almacenada "inf".
		 *  En caso que no est�, devolver�a "inf" vac�o
		 */
		bool DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf)const;
		/** Devuelve true si hay una pregunta indexada, devolviendo su informaci�n almacenada
		 *  (campo privado "infPregunta") en "inf".
		 *  En caso que no est�, devolver�a "inf" vac�o
		 */
		bool DevuelvePregunta(InformacionPregunta& inf) const;
		void ImprimirIndexacionPregunta() {
			cout << "Pregunta indexada: " << pregunta << endl;
			cout << "Terminos indexados en la pregunta: " << endl;
			/** A continuaci�n aparecer� un listado del contenido de "indicePregunta"
			 * donde para cada t�rmino indexado se imprimir�:
			 */
			//  unordered_map<string, InformacionPregunta>::iterator it;
			for(auto it=indicePregunta.begin();it!=indicePregunta.end();++it){
				cout << it->first << '\t' << it->second << endl;
			}
			cout << "Informacion de la pregunta: " << infPregunta << endl;
		}
		void ImprimirPregunta() {
			cout << "Pregunta indexada: " << pregunta << endl;
			cout << "Informacion de la pregunta: " << infPregunta << endl;
		}
		/** Devuelve true si word est� indexado, devolviendo su informaci�n almacenada "inf".
		 * En caso que no est�, devolver�a "inf" vac�o
		 */
		bool Devuelve(const string& word, InformacionTermino& inf) const;
		/** Devuelve true si word est� indexado y aparece en el documento de nombre nomDoc,
		 * en cuyo caso devuelve la informaci�n almacenada para word en el documento.
		 * En caso que no est�, devolver�a "InfDoc" vac�o
		 */
		bool Devuelve(const string& word, const string& nomDoc, InfTermDoc& InfDoc)const;
		// Devuelve true si word aparece como t�rmino indexado
		bool Existe(const string& word) const;
		// Devuelve true si se realiza el borrado (p.ej. si word aparece como t�rmino indexado)
		bool Borra(const string& word);
		/** Devuelve true si nomDoc est� indexado y se realiza el borrado de todos los t�rminos del
		 * documento y del documento en los campos privados"indiceDocs" e "informacionColeccionDocs"
		 */
		bool BorraDoc(const string& nomDoc);
		// Borra todos los t�rminos de todos los �ndices
		void VaciarIndice();
		// Ser� true si word est� indexado, sustituyendo la informaci�n almacenada por "inf"
		bool Actualiza(const string& word, const InformacionTermino& inf);
		// Ser� true si se realiza la inserci�n (p.ej. si word no estaba previamente indexado)
		bool Inserta(const string& word, const InformacionTermino& inf);
		// Devolver� el n�mero de t�rminos diferentes indexados (cardinalidad de campo privado "�ndice")
		int NumPalIndexadas() const;
		// Devuelve el contenido del campo privado "ficheroStopWords"
		string DevolverFichPalParada () const;
		/** Mostrar� por pantalla las palabras de parada almacenadas: una palabra por l�nea
		 * (salto de l�nea al final de cada palabra)
		 */
		void ListarPalParada() const;
		// Devolver� el n�mero de palabras de parada almacenadas
		int NumPalParada() const;
		// Devuelve los delimitadores utilizados por el tokenizador
		string DevolverDelimitadores () const;
		// Devuelve si el tokenizador analiza los casos especiales
		bool DevolverCasosEspeciales () const;
		// Devuelve si el tokenizador pasa a min�sculas y sin acentos
		bool DevolverPasarAminuscSinAcentos () const;
		// Devuelve el valor de almacenarPosTerm
		bool DevolverAlmacenarPosTerm () const;
		// Devuelve "directorioIndice" (el directorio del disco duro donde se almacenar� el �ndice)
		string DevolverDirIndice () const;
		/** Devolver� el tipo de stemming realizado en la indexaci�n de acuerdo con el valor
		 * indicado en la variable privada "tipoStemmer"
		 */
		int DevolverTipoStemming () const;
		// Devolver� el valor indicado en la variable privada "almacenarEnDisco"
		bool DevolverAlmEnDisco () const;
		// Mostrar por pantalla: cout << informacionColeccionDocs << endl;
		void ListarInfColeccDocs() const;
		/** Mostrar por pantalla el contenido del campo privado "�ndice":
		 *  cout << termino << '\t' << InformacionTermino << endl;
		 */
		void ListarTerminos() const;
		/** Devuelve true si nomDoc existe en la colecci�n y muestra por pantalla todos los t�rminos
		 * indexados del documento con nombre "nomDoc":
		 * cout << termino << '\t' << InformacionTermino << endl;
		 * Si no existe no se muestra nada
		 */
		bool ListarTerminos(const string& nomDoc) const;
		/** Mostrar por pantalla el contenido el contenido del campo privado "indiceDocs":
		 * cout << nomDoc << '\t' << InfDoc << endl;
		 */
		void ListarDocs() const;
		/** Devuelve true si nomDoc existe en la colecci�n y muestra por pantalla el contenido del
		 * campo privado "indiceDocs" para el documento con nombre "nomDoc":
		 * cout << nomDoc << '\t' << InfDoc << endl;
		 * Si no existe no se muestra nada
		 */
		bool ListarDocs(const string& nomDoc) const;

		//Implementadas por mi
		//////////////////////////////////////////////////////////////////
		//Devuelve un entero con el n�mero de palabras distintas indexadas para un documento sin contar las stopwords
		int PalDistintasDoc( const string& nomDoc)const;
		//////////////////////////////////////////////////////////////////
		IndexadorHash();

	const unordered_map<string, InformacionTermino>& getIndice() const {
		return indice;
	}

	const unordered_map<string, InfDoc>& getIndiceDocs() const {
		return indiceDocs;
	}

	const unordered_map<string, InformacionTerminoPregunta>& getIndicePregunta() const {
		return indicePregunta;
	}

	const string& getPregunta() const {
		return pregunta;
	}

	private:
		/** Este constructor se pone en la parte privada porque no se permitir� crear un indexador
		 * sin inicializarlo convenientemente. La inicializaci�n la decidir� el alumno
		 */

		// �ndice de t�rminos indexados accesible por el t�rmino
		unordered_map<string, InformacionTermino> indice;
		// �ndice de documentos indexados accesible por el nombre del documento
		unordered_map<string, InfDoc> indiceDocs;
		// Informaci�n recogida de la colecci�n de documentos indexada
		InfColeccionDocs informacionColeccionDocs;
		// Pregunta indexada actualmente. Si no hay ninguna indexada, contendr�a el valor ""
		string pregunta;
		// �ndice de t�rminos indexados en una pregunta. Se almacenar� en memoria principal
		unordered_map<string, InformacionTerminoPregunta> indicePregunta;
		// Informaci�n recogida de la pregunta indexada. Se almacenar� en memoria principal
		InformacionPregunta infPregunta;
		// Palabras de parada. Se almacenar� en memoria principal
		unordered_set<string> stopWords;
		// Nombre del fichero que contiene las palabras de parada
		string ficheroStopWords;
		// Tokenizador que se usar� en la indexaci�n
		Tokenizador tok;
		/** "directorioIndice" ser� el directorio del disco duro donde se almacenar� el �ndice.
		 * En caso que contenga la cadena vac�a se crear� en el directorio donde se ejecute
		 * el indexador
		 */
		string directorioIndice;
		// 0 = no se aplica stemmer: se indexa el t�rmino tal y como aparece tokenizado
		// 1 = stemmer de Porter para espa�ol
		// 2 = stemmer de Porter para ingl�s
		/** Para el stemmer de Porter se utilizar�n los archivos stemmer.cpp y stemmer.h,
		 *  concretamente las funciones de nombre "stemmer"
		 */
		int tipoStemmer;
		/** Si es true se almacenar� la m�nima parte de los �ndices de los documentos en memoria
		 *  principal (solo parte de "indice", el resto en memoria secundaria, creando las
		 *  estructuras para acceder a esta memoria secundaria de la forma m�s eficiente posible).
		 *  En caso contrario se almacenar� toda la info del �ndice en memoria principal
		 */
		bool almacenarEnDisco;
		// Si es true se almacenar� la posici�n en la que aparecen los t�rminos dentro del documento en la clase InfTermDoc
		bool almacenarPosTerm;
};



#endif /* INDEXADORHASH_H_ */
