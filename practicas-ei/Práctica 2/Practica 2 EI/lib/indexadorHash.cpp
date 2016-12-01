/*
 * indexadorHash.cpp
 *
 *  Created on: 14 de abr. de 2016
 *      Author: Alejandro
 */

#include "indexadorHash.h"


IndexadorHash::IndexadorHash(const string& fichStopWords,const string& delimitadores, const bool& detectComp,const bool& minuscSinAcentos, const string& dirIndice,const int& tStemmer, const bool& almEnDisco, const bool& almPosTerm) {
	ficheroStopWords=fichStopWords;
	ifstream file;
	string linea;
	struct stat dir;
	file.open(ficheroStopWords.c_str());
	if(file.is_open()){
		while(!file.eof()){
			linea = "";
			getline(file, linea);
			int err=stat(linea.c_str(), &dir);

			if(err==-1 || !S_ISDIR(dir.st_mode)){
				if(linea.length()!=0){
					stopWords.insert(linea);
				}
			}
		}
		file.close();
	}
	tok.DelimitadoresPalabra(delimitadores);
	tok.PasarAminuscSinAcentos(minuscSinAcentos);
	tok.CasosEspeciales(detectComp);
	if(dirIndice==""){
		directorioIndice="./";
	}
	else{
		directorioIndice=dirIndice;
	}
	tipoStemmer=tStemmer;
	almacenarEnDisco=almEnDisco;
	almacenarPosTerm=almPosTerm;
	//Los indices quedarán vacíos
	indice.clear();
	indiceDocs.clear();
	indicePregunta.clear();
	//informacionColeccionDocs.~InfColeccionDocs();
}

IndexadorHash::IndexadorHash(const string& directorioIndexacion) {
	struct stat dir;
	list<string> tokens;
	ifstream file;
	//Para el indice
	/////////////////////////////////////////////
	list<string>::iterator itAux;
	int idDoc,ft;
	string termino;
	list<int> lista;
	unordered_map<long int,InfTermDoc> l_docs_local;
	InfTermDoc itd;
	InformacionTermino it;
	////////////////////////////////////////////

	//Para el indiceDocs
	////////////////////////////////////////////
	InfDoc id;
	////////////////////////////////////////////

	//Para el indiceDocs
	////////////////////////////////////////////
	InformacionTerminoPregunta itp;
	////////////////////////////////////////////

	// Compruebo la existencia del directorio
	int err=stat(directorioIndexacion.c_str(), &dir);

	if(err==-1 || !S_ISDIR(dir.st_mode)){//Si no existe el directorio
		cerr << "El directorio no existe" << endl;
		return;
	}
	else {
		directorioIndice=directorioIndexacion;
		string fichero[7]= {{directorioIndice+"/indice.txt"},{directorioIndice+"/indiceDocs.txt"},{directorioIndice+"/InfColeccionDoc.txt"},{directorioIndice+"/indicePregunta.txt"},{directorioIndice+"/InformacionPregunta.txt"},{directorioIndice+"/tokenizador.txt"},{directorioIndice+"/Variables.txt"}};
		//Si existe el directorioo
		string linea;
		tok.DelimitadoresPalabra("\t ");//Para tokenizar los indices
		tok.CasosEspeciales(false);
		tok.PasarAminuscSinAcentos(false);
		int MAX=7;
		for(int i=0;i<MAX;++i){
			file.open(fichero[i].c_str());
			//comprobamos si el fichero se puede abrir, si no error
			if(file.is_open()){
				linea = "";
				while(!file.eof()){
					switch (i){
						case 0: //indice.txt
							if(file.is_open()){
								while(!file.eof()){
									getline(file, linea);
									//Limpiamos las variables que usamos
									////////////////////////////////////
									l_docs_local.clear();
									lista.clear();
									////////////////////////////////////
									tok.Tokenizar(linea,tokens);//Tokenizamos la linea
									for(list<string>::iterator itS= tokens.begin();itS!= tokens.end();++itS){//Iteramos entre los diferentes tokens
										//pal2 Frecuencia total: 3	fd: 2	Id.Doc: 1	ft: 1	2	Id.Doc: 2	ft: 2	0	2
										if(itS==tokens.begin()){//Si es el primer termino del token es la palabra indexada
											termino=(*itS);
											++itS;//Frecuencia
											++itS;//total:
											++itS;//numero a leer
											it.setFtc((atoi((*itS).c_str())));
											++itS;//fd:
											++itS;//l_docs.size()
										}
										else if((*itS)=="Id.Doc:"){
											lista.clear();
											++itS;
											idDoc=(atoi((*itS).c_str()));
										}
										else if((*itS)=="ft:"){
											++itS;
											ft=(atoi((*itS).c_str()));
										}
										else{
											lista.push_back((atoi((*itS).c_str())));
											itAux=itS;
											++itAux;
											if((*itAux)=="Id.Doc:" || itAux== tokens.end()){
												itd.setFt(ft);
												itd.setPosTerm(lista);
												l_docs_local.insert({idDoc,itd});
											}
										}
									}
									it.setDocs(l_docs_local);
									Inserta(termino,it);
								}
							}
							break;
						case 1: //indiceDocs.txt
							if(file.is_open()){
								linea="";
								getline(file, linea);
								tok.Tokenizar(linea,tokens);//Tokenizamos la linea
								for(list<string>::iterator itS= tokens.begin();itS!= tokens.end();++itS){//Iteramos entre los diferentes tokens
									//corpus_corto//fichero2.txt idDoc: 2	numPal: 5	numPalSinParada: 3	numPalDiferentes: 2	tamBytes: 23
									termino=(*itS);
									++itS;//idDoc:
									++itS;//numero a guardar
									id.setIdDoc((atoi((*itS).c_str())));
									++itS;//numPal:
									++itS;//numero a guardar
									id.setNumPal((atoi((*itS).c_str())));
									++itS;//numPalSinParada:
									++itS;//numero a guardar
									id.setNumPalSinParada((atoi((*itS).c_str())));
									++itS;//numPalDiferentes:
									++itS;//numero a guardar
									id.setNumPalDiferentes((atoi((*itS).c_str())));
									++itS;//tamBytes:
									++itS;//numero a guardar
									id.setTamBytes((atoi((*itS).c_str())));
								}
								indiceDocs.insert({termino,id});
							}
							break;
						case 2: //InfColeccionDoc.txt
							if(file.is_open()){
								while(!file.eof()){
									linea="";
									getline(file, linea);
									if(!file.eof()){
										informacionColeccionDocs.setNumDocs((atoi(linea.c_str())));
										getline(file, linea);
										informacionColeccionDocs.setNumTotalPal((atoi(linea.c_str())));
										getline(file, linea);
										informacionColeccionDocs.setNumTotalPalSinParada((atoi(linea.c_str())));
										getline(file, linea);
										informacionColeccionDocs.setNumTotalPalDiferentes((atoi(linea.c_str())));
										getline(file, linea);
										informacionColeccionDocs.setTamBytes((atoi(linea.c_str())));
									}
								}
							}
							break;
						case 3: //indicePregunta.txt
							if(file.is_open()){
								linea="";
								getline(file, linea);
								tok.Tokenizar(linea,tokens);//Tokenizamos la linea
								lista.clear();
								for(list<string>::iterator itS= tokens.begin();itS!= tokens.end();++itS){//Iteramos entre los diferentes tokens
									//pal1	ft: 2	0	3
									if(itS==tokens.begin()){
										termino=(*itS);
									}
									else if((*itS)=="ft:"){
										++itS;
										itp.setFt((atoi((*itS).c_str())));
									}
									else{
										lista.push_back((atoi((*itS).c_str())));
									}
								}
								itp.setPosTerm(lista);
								indicePregunta.insert({termino,itp});
							}
							break;
						case 4: //InformacionPregunta.txt
							if(file.is_open()){
								linea="";
								getline(file, linea);
								if(!file.eof()){
									infPregunta.setNumTotalPal((atoi(linea.c_str())));
									getline(file, linea);
									infPregunta.setNumTotalPalSinParada((atoi(linea.c_str())));
									getline(file, linea);
									infPregunta.setNumTotalPalDiferentes((atoi(linea.c_str())));
								}
							}
							break;
						case 5: //tokenizador.txt
							while(!file.eof()){
								linea="";
								getline(file, linea);
								tok.CasosEspeciales(atoi(linea.c_str()));
								if(!file.eof()){
									getline(file, linea);
									tok.DelimitadoresPalabra(linea);
									if(!file.eof()){
										getline(file, linea);
										tok.PasarAminuscSinAcentos(atoi(linea.c_str()));
									}
								}
							}
							break;

						case 6: //Variables.txt
							while(!file.eof()){
								linea="";
								getline(file, pregunta);
								if(!file.eof()){
									getline(file, ficheroStopWords);
									if(!file.eof()){
										getline(file, linea);
										tipoStemmer=atoi(linea.c_str());
										if(!file.eof()){
											getline(file, linea);
											almacenarEnDisco=atoi(linea.c_str());
											if(!file.eof()){
												getline(file, linea);
												almacenarPosTerm=atoi(linea.c_str());
											}
										}
									}
								}
							}
							break;
						default:
							break;
					}
				}
				file.close();
			}
			else{
				cerr << "ERROR: No existe el archivo: " << fichero[i] << endl;
			}

		}
	}
}

IndexadorHash::IndexadorHash(const IndexadorHash& ih) {
	almacenarEnDisco=ih.almacenarEnDisco;
	almacenarPosTerm=ih.almacenarPosTerm;
	directorioIndice=ih.directorioIndice;
	ficheroStopWords=ih.ficheroStopWords;
	indice=ih.indice;
	indiceDocs=ih.indiceDocs;
	indicePregunta=ih.indicePregunta;
	infPregunta=ih.infPregunta;
	informacionColeccionDocs=ih.informacionColeccionDocs;
	pregunta=ih.pregunta;
	stopWords=ih.stopWords;
	tipoStemmer=ih.tipoStemmer;
	tok=ih.tok;
}

IndexadorHash::~IndexadorHash() {
	almacenarEnDisco=false;
	almacenarPosTerm=false;
	directorioIndice="";
	ficheroStopWords="";
	indice.clear();
	indiceDocs.clear();
	indicePregunta.clear();
	//infPregunta.~InformacionPregunta();
	//informacionColeccionDocs.~InfColeccionDocs();
	pregunta="";
	stopWords.clear();
	tipoStemmer=0;
	//tok.~Tokenizador();
}

IndexadorHash& IndexadorHash::operator =(const IndexadorHash& ih) {
	if(this!=&ih){
		//(*this).~IndexadorHash();
		almacenarEnDisco=ih.almacenarEnDisco;
		almacenarPosTerm=ih.almacenarPosTerm;
		directorioIndice=ih.directorioIndice;
		ficheroStopWords=ih.ficheroStopWords;
		indice=ih.indice;
		indiceDocs=ih.indiceDocs;
		indicePregunta=ih.indicePregunta;
		infPregunta=ih.infPregunta;
		informacionColeccionDocs=ih.informacionColeccionDocs;
		pregunta=ih.pregunta;
		stopWords=ih.stopWords;
		tipoStemmer=ih.tipoStemmer;
		tok=ih.tok;
	}
	return *this;
}

bool IndexadorHash::Indexar(const string& ficheroDocumentos) {
	ifstream file;
	bool indexe=true;
	int numPal, numPalParada, id;//Necesarias para contabilizar
	//Para almacenar las lineas de los ficheros a tokenizar y las palabras a indexar
	string linea, lineaTokenizada;
	list<string>::iterator itS;//Para iterar entre tokens

	//Elementos auxiliares para evitar problemas de accceso
	///////////////////////////////////////////////////////
	//Para indice
	InformacionTermino infter;
	unordered_map<long int, InfTermDoc> l_docs_local;
	InfTermDoc infopaldoc;
	list<int> lista;//Lista de posiciones de palabra en el documento
	Fecha aux;
	list<string> tokens;
	//Para indiceDocs
	InfDoc infdoc;
	///////////////////////////////////////////////////////

	//Para obtener datos metricos del fichero y saber si es un directorio o no
	struct stat attrib;

	file.open(ficheroDocumentos.c_str());//Abrimos el documento de documentos
	if(file.is_open()){//si esta abierto
		while(!file.eof()){//Mientras no sea el fin del documento
			linea="";
			indexe=true;
			getline(file, linea);//Leemos documento
			//cout <<linea<< endl ;
			int err=stat(linea.c_str(), &attrib);

			if(err==-1 || !S_ISDIR(attrib.st_mode)){//Si la linea no es un directorio
				//cout << linea << endl;//Imprime el nombre del documento que va a tokenizar
				if(linea.length()!=0){//si la  linea no esta vacía
					//Limpiamos las variables auxiliares para no meter mierda de indexaciones anteriores
					//////////////////////////////////////////////////////////////
					//Auxiliares para indiceDocs
					//infdoc.~InfDoc();
					//////////////////////////////////////////////////////////////

					//Obtener info del documento
					//stat(linea.c_str(), &attrib);
					//attrib.st_size;//Tamaño del fichero en bytes
					//attrib.st_mtime;//Fecha de la ultima modificacion
					Fecha fecha(attrib.st_mtime);
					id=indiceDocs.size()+1;

					//////////Comprobar si el documento está indexado ya
					if(indiceDocs.find(linea) != indiceDocs.end()){//Si ya esta indexado el documento
						//Si la fecha del documento actual es posterior al anteriormente indexado
						//Entonces se reindexa
						//aux = indiceDocs.find(linea)->second.getFechaModificacion();
						if(indiceDocs.find(linea)->second.getFechaModificacion().Posterior(fecha)){
							//REINDEXAR CON EL MISMO ID
							id=indiceDocs.find(linea)->second.getIdDoc();
							BorraDoc(linea);//Borrará el documento para volver a indexarlo luego
							//infdoc.~InfDoc();
							indexe=true;
						}
						else{
							indexe=false;
						}
					}

					if(indexe){//Si hace falta indexar algo
						infdoc.setIdDoc(id);
						infdoc.setFechaModificacion(fecha);
						infdoc.setTamBytes(attrib.st_size);

						//Una vez leida la linea del documento, tokenizo el documento
						tok.TokenizarDoc(linea, tokens);//Tokenizará y guardara el token en f
						///Indexar
						numPal=0;
						numPalParada=0;
						for(itS= tokens.begin();itS!= tokens.end();++itS){
							//lineaTokenizada=(*itS);//lineaTokenizada contendrá el termino tokenizado
							//Limpiamos las variables auxiliares para no meter mierda de indexaciones anteriores
							//////////////////////////////////////////////////////////////
							//Auxiliares para indice
							//infter=InformacionTermino();
							l_docs_local.clear();
							infopaldoc.~InfTermDoc();
							//infopaldoc.Limpia();
							lista.clear();
							//////////////////////////////////////////////////////////////

							if((*itS).length()!=0){//si la  linea no esta vacía
								//Tokenizacion de Terminos
								//////////////////////////////////////////////////////////////
								if(stopWords.find((*itS)) == stopWords.end()){//Si la palabra no es una stopWord
									if(Existe((*itS))){//Si el termino ya ha sido indexado
										//Devuelve(lineaTokenizada,infter);
									/*	l_docs_local=infter.getDocs();
										infter.setFtc(infter.getFtc()+1);
										l_docs_local[id].setFt(l_docs_local[id].getFt()+1);
										if(almacenarPosTerm){
											lista= l_docs_local[id].getPosTerm();
											lista.push_back(numPal+numPalParada);
											l_docs_local[id].setPosTerm(lista);//Posicion de la palabra en el documento
										}
										infter.setDocs(l_docs_local);
	*/

										//Peligro!! zona experimental
										//////////////////////////////////////////////

										indice.find((*itS))->second.setFtc(indice.find((*itS))->second.getFtc()+1);
										infopaldoc.setFt(infopaldoc.getFt()+1);
										if(almacenarPosTerm){
											lista.push_back(numPal+numPalParada);
											infopaldoc.setPosTerm(lista);
										}
										l_docs_local.insert({id,infopaldoc});
										//Para comprobar lo que hay en el l_docs_local
										/////////////////////////////////////////////////////
										/*cout << lineaTokenizada << endl;
										for(auto it= l_docs_local.begin();it!=l_docs_local.end();++it){
											cout << "idDoc: " << it->first << " infTermDoc: " << it ->second << endl;
										}*/
										/////////////////////////////////////////////////////

										indice.find((*itS))->second.AppendDocs(l_docs_local);
										//infter=indice.find(lineaTokenizada)->second;
										/*Borra(lineaTokenizada);
										Inserta(lineaTokenizada,indice.find(lineaTokenizada)->second);*/
										//////////////////////////////////////////////
										//Actualiza(lineaTokenizada,indice.find(lineaTokenizada)->second);//Actualiza la palabra tokenizada
									}
									else{
										infter.setFtc(1);
										infopaldoc.setFt(1);
										if(almacenarPosTerm){
											lista.push_back(numPal+numPalParada);
											infopaldoc.setPosTerm(lista);
										}
										l_docs_local.insert({id,infopaldoc});
										infter.setDocs(l_docs_local);
										Inserta((*itS),infter);
									}
									++numPal;
								}
								else{
									++numPalParada;
								}
								/////////////////////////////////////////////////////////////
							}
						}


						//Hacemos el set en indiceDocs
						////////////////////////////////////////////////////
						infdoc.setNumPalSinParada(numPal);
						infdoc.setNumPal(numPal+numPalParada);//Stopword y normales
						indiceDocs.insert({linea,infdoc});//inserta un nuevo documento en el indice
						infdoc.setNumPalDiferentes(PalDistintasDoc(linea));
						indiceDocs.erase(linea);//Posible problema
						indiceDocs.insert({linea,infdoc});
						////////////////////////////////////////////////////

						//Actualizamos inormacionColeccionDocs
						////////////////////////////////////////////////////
						informacionColeccionDocs.setNumDocs(informacionColeccionDocs.getNumDocs()+1);
						informacionColeccionDocs.setNumTotalPal(informacionColeccionDocs.getNumTotalPal()+numPal+numPalParada);
						informacionColeccionDocs.setNumTotalPalDiferentes(indice.size());//Palabras indexadas
						informacionColeccionDocs.setNumTotalPalSinParada(informacionColeccionDocs.getNumTotalPalSinParada()+numPal);
						informacionColeccionDocs.setTamBytes(informacionColeccionDocs.getTamBytes()+attrib.st_size);
						////////////////////////////////////////////////////
					}
				}
			}
			else{
				//cerr <<"El fichero es un directorio."<<endl;
			}
		}
		file.close();//Cerramos el archivo que contiene la lista de documentos
		return true;
	}
	else{
		//cerr << "No se ha podido abrir el fichero de documentos." << endl;
		return false;
	}
}

bool IndexadorHash::IndexarDirectorio(const string& dirAIndexar) {
	struct stat dir;
	// Compruebo la existencia del directorio
	int err=stat(dirAIndexar.c_str(), &dir);

	if(err==-1 || !S_ISDIR(dir.st_mode)){
		return false;
	}
	else {
		// Hago una lista en un fichero con find>fich
		string cmd="find "+dirAIndexar+" -follow |sort > .lista_fich"; //Extrae todas las rutas de los ficheros a un archivo .lista_fich
		system(cmd.c_str()); //Ejecuta el comando cmd en el tablero de comandos
		return Indexar(".lista_fich");
	}
}

bool IndexadorHash::GuardarIndexacion() const {
	/**Ficheros:
	 * 	listaTerminos.txt
	 * 	InformacionTermino.txt
	 * 	listaDocumentos.txt
	 * 	InfDoc.txt
	 * 	InfColeccionDoc.txt
	 * 	terminosPregunta.txt
	 * 	InformacionTerminoPregunta.txt
	 * 	InformacionPregunta.txt
	 * 	stopWords.txt //Inicializable a partir del nombre del fichero con el constructor anterior
	 * 	tokenizador.txt
	 * 	Variables.txt //Guardará las variables pregunta, ficheroStopWords, tipoStemmer, almacenarEnDisco y AlmacenarPosTerm en ese orden, una por cada linea
	 * 	//La variable directorioIndice no es necesario guardarla
	 */

	ofstream ofile;
	struct stat dir;
	// Compruebo la existencia del directorio
	int err=stat(directorioIndice.c_str(), &dir);
	Tokenizador tokaux(tok);

	if(err==-1 || !S_ISDIR(dir.st_mode)){//Si no existe el directorio lo creamos
		string cmd="mkdir "+directorioIndice; //Crea el directorio
		system(cmd.c_str()); //Ejecuta el comando cmd en el tablero de comandos
	}

	string fichero[7]= {{directorioIndice+"/indice.txt"},{directorioIndice+"/indiceDocs.txt"},{directorioIndice+"/InfColeccionDoc.txt"},{directorioIndice+"/indicePregunta.txt"},{directorioIndice+"/InformacionPregunta.txt"},{directorioIndice+"/tokenizador.txt"},{directorioIndice+"/Variables.txt"}};
	int MAX=7;
	if(almacenarEnDisco){
		MAX=1;
	}
	for(int i=0;i<MAX;++i){
		ofile.open(fichero[i].c_str(), ios::out);
		if(ofile.is_open()){
			switch(i){
			case 0://indice.txt
				for(auto i=indice.begin();i!=indice.end();++i){
					ofile << i->first << '\t'<< i->second << endl;
				}
				break;
			case 1://indiceDocs.txt
				for(auto i=indiceDocs.begin();i!=indiceDocs.end();++i){
					ofile << i->first << '\t'<< i->second << endl;
				}
				break;
			case 2://InfColeccionDocs.txt
				ofile << informacionColeccionDocs.getNumDocs() << endl;
				ofile << informacionColeccionDocs.getNumTotalPal() << endl;
				ofile << informacionColeccionDocs.getNumTotalPalSinParada() << endl;
				ofile << informacionColeccionDocs.getNumTotalPalDiferentes() << endl;
				ofile << informacionColeccionDocs.getTamBytes() << endl;
				break;
			case 3://indicePregunta.txt
				for(auto it=indicePregunta.begin();it!=indicePregunta.end();++it){
					ofile << it->first << '\t' << it->second << endl;
				}
				break;
			case 4://informacionPregunta
				ofile << infPregunta.getNumTotalPal() << endl;
				ofile << infPregunta.getNumTotalPalSinParada() << endl;
				ofile << infPregunta.getNumTotalPalDiferentes() << endl;
				break;
			case 5://tokenizador.txt
				ofile << tokaux.CasosEspeciales() << endl << tokaux.DelimitadoresPalabra() << endl << tokaux.PasarAminuscSinAcentos();
				break;
			case 6://Variables.txt
				ofile << pregunta << endl <<ficheroStopWords << endl << tipoStemmer << endl << almacenarEnDisco  << endl<< almacenarPosTerm;
				break;
			}
			ofile.close();
		}
	}
	return true;
}

bool IndexadorHash::RecuperarIndexacion(const string& directorioIndexacion) {
	//this->~IndexadorHash();
	(*this)=IndexadorHash(directorioIndexacion);
	return true;
}

bool IndexadorHash::IndexarPregunta(const string& preg) {
	InformacionTerminoPregunta infterpre;
	pregunta=preg;
	//Antes de indexar una nueva pregunta limpiamos lo que habia antes
	//////////////////////////////////////////////////
	indicePregunta.clear();
	infPregunta.~InformacionPregunta();
	//////////////////////////////////////////////////
	list<string> tokens;
	tok.Tokenizar(preg,tokens);
	string aIndexar;
	int numPal, numPalParada;
	numPal=0;
	numPalParada=0;
	list<int> lista;
	list<string>::const_iterator itCadena;
	for(itCadena=tokens.begin();itCadena!=tokens.end();++itCadena)
	{
			aIndexar= (*itCadena);
			//Limpiamos la variable auxiliar
			//////////////////////////////////////////////
			infterpre.~InformacionTerminoPregunta();
			//////////////////////////////////////////////

			if(stopWords.find(aIndexar)!= stopWords.end()){//Es una StopWord
				++numPalParada;
			}else{
				if(indicePregunta.find(aIndexar)!=indicePregunta.end()){//Si la palabra estaba indexada
					infterpre= indicePregunta.find(aIndexar)->second;
					infterpre.setFt(infterpre.getFt()+1);
					if(almacenarPosTerm){
						lista= infterpre.getPosTerm();
						lista.push_back(numPal+numPalParada);
						infterpre.setPosTerm(lista);
					}
					indicePregunta.erase(aIndexar);//Borras y  vuelves a insertar
					indicePregunta.insert({aIndexar,infterpre});//Actualiza
				}
				else{//Si la palabra no estaba indexada
					infterpre.setFt(infterpre.getFt()+1);
					if(almacenarPosTerm){
						lista.clear();
						lista= infterpre.getPosTerm();
						lista.push_back(numPal+numPalParada);
						infterpre.setPosTerm(lista);
					}
					indicePregunta.insert({aIndexar,infterpre});//Inserta
				}
				++numPal;
			}
	}

	infPregunta.setNumTotalPal(numPal+numPalParada);
	infPregunta.setNumTotalPalDiferentes(indicePregunta.size());//Palabras indexadas de la pregunta
	infPregunta.setNumTotalPalSinParada(numPal);
	return true;

}

bool IndexadorHash::DevuelvePregunta(string& preg) const {
	if(!indicePregunta.empty()){
		preg= pregunta;
		return true;
	}
	return false;
}

bool IndexadorHash::DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const {
	if(indicePregunta.find(word)!= indicePregunta.end()){//El término de la pregunta está indexado
		inf = indicePregunta.find(word)->second;
		return true;
	}
	else{
		return false;
	}
}

bool IndexadorHash::DevuelvePregunta(InformacionPregunta& inf) const {
	if(indicePregunta.size()!=0){
		inf=infPregunta;
		return true;
	}
	else{
		return false;
	}
}

bool IndexadorHash::Devuelve(const string& word, InformacionTermino& inf) const {
	if(Existe(word)){
		inf=indice.find(word)->second;
		return true;
	}
	else{

		return false;
	}
}

bool IndexadorHash::Devuelve(const string& word, const string& nomDoc,InfTermDoc& Infdoc) const {
	InfDoc id;
	unordered_map<string, InfDoc> aux=indiceDocs;
	if(Existe(word)){//La palabra está indexada
		//La palabra aparece en el documento
		id= aux[nomDoc];
		int i=aux[nomDoc].getIdDoc();
		if(indice.find(word)->second.getDocs().find(i)!= indice.find(word)->second.getDocs().end()){
			Infdoc= indice.find(word)->second.getDocs().find(i)->second;
			//Infdoc= l_docs_local.find(id.getIdDoc())->second;
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}

}

bool IndexadorHash::Existe(const string& word) const {
	return (indice.find(word) != indice.end());
}

bool IndexadorHash::Borra(const string& word) {
	if(indice.find(word)!=indice.end()){//Si la palabra esta indexada
		indice.erase(word);
		return true;
	}
	return false;
}

bool IndexadorHash::BorraDoc(const string& nomDoc) {
	//Borrar palabras indexadas del documento
	InformacionTermino infter;
	InfDoc id;
	unordered_map<long int, InfTermDoc> l_docs_local;
	unordered_map<string, InformacionTermino> aux;
	string nombre;
	if(indiceDocs.find(nomDoc)!= indiceDocs.end()){//Si el documento está indexado

		id =indiceDocs.find(nomDoc)->second;

		//Borra terminos del documento
		////////////////////////////////////////////
		for(auto it=indice.begin();it!=indice.end();++it){//Recorro el indice de terminos
			Devuelve(it->first,infter);
			nombre= it->first;

			if(infter.getFtc()>0){//Si existe informacion del termino
				l_docs_local.clear();
				l_docs_local= infter.getDocs();
				if(l_docs_local.find(id.getIdDoc())!= l_docs_local.end()){//Existe
					infter.setFtc(infter.getFtc()-l_docs_local[id.getIdDoc()].getFt());//Actualizamos el Ftc
					l_docs_local.erase(id.getIdDoc());
					//Borra(it->first);
					if(l_docs_local.size()>=1){//Si el termino pertenece a algun otro documento
						infter.setDocs(l_docs_local);
						aux.insert({nombre, infter});
					}
				}
				else{
					aux.insert({nombre, infter});
				}
			}

		}
		indice=aux;
		////////////////////////////////////////////

		//Borra info del documento de la coleccion
		////////////////////////////////////////////
		informacionColeccionDocs.setNumDocs(informacionColeccionDocs.getNumDocs()-1);//Disminuye un documento
		informacionColeccionDocs.setTamBytes(informacionColeccionDocs.getTamBytes()-id.getTamBytes());
		informacionColeccionDocs.setNumTotalPal(informacionColeccionDocs.getNumTotalPal()-id.getNumPal());
		informacionColeccionDocs.setNumTotalPalSinParada(informacionColeccionDocs.getNumTotalPalSinParada()-id.getNumPalSinParada());
		informacionColeccionDocs.setNumTotalPalDiferentes(indice.size());

		////////////////////////////////////////////

		return(indiceDocs.erase(nomDoc) != 0);//Borra el documento de indiceDocs
	}
	else{
		return false;
	}

}

void IndexadorHash::VaciarIndice() {
	indice.clear();
	indiceDocs.clear();
	indicePregunta.clear();
}

bool IndexadorHash::Actualiza(const string& word,const InformacionTermino& inf) {
	if(Existe(word)){//El termino esta indexado
		Borra(word);
		indice.insert({word,inf});
		return true;
	}
	else{
		return false;
	}
}

bool IndexadorHash::Inserta(const string& word, const InformacionTermino& inf) {
	if(Existe(word)){
		return false;
	}
	else
	{
		indice.insert({word, inf});
		//numeroTerminos++;
		return true;
	}
}

int IndexadorHash::NumPalIndexadas() const {
	return indice.size();
}

string IndexadorHash::DevolverFichPalParada() const {
	return ficheroStopWords;
}

void IndexadorHash::ListarPalParada() const {
	//unordered_set<string>::iterator it;
	for(auto it=stopWords.begin();it!=stopWords.end();++it){
		cout << *(it) <<endl;
	}
}

int IndexadorHash::NumPalParada() const {
	return stopWords.size();
}

string IndexadorHash::DevolverDelimitadores() const {
	return tok.DelimitadoresPalabra();
}

bool IndexadorHash::DevolverCasosEspeciales() const {
	Tokenizador token=tok;
	return token.CasosEspeciales();
}

bool IndexadorHash::DevolverPasarAminuscSinAcentos() const {
	Tokenizador token=tok;
	return token.PasarAminuscSinAcentos();
}

bool IndexadorHash::DevolverAlmacenarPosTerm() const {
	return almacenarPosTerm;
}

string IndexadorHash::DevolverDirIndice() const {
	return directorioIndice;
}

int IndexadorHash::DevolverTipoStemming() const {
	return tipoStemmer;
}

bool IndexadorHash::DevolverAlmEnDisco() const {
	return almacenarEnDisco;
}

void IndexadorHash::ListarInfColeccDocs() const {
	cout << informacionColeccionDocs << endl;
}

void IndexadorHash::ListarTerminos() const {
	//unordered_map<string, InfDoc>::iterator it;
	for(auto it=indice.begin();it!=indice.end();++it){
		cout << it->first << '\t' << it->second << endl;
	}
}

bool IndexadorHash::ListarTerminos(const string& nomDoc) const {
	bool existe=false;
	InformacionTermino it;
	InfDoc id;
	unordered_map<string, InfDoc> auxDoc= indiceDocs;
	unordered_map<long int, InfTermDoc> l_docs_local;
	//unordered_map<string, InformacionTermino>::iterator it2;
	if(indiceDocs.find(nomDoc) != indiceDocs.end()){//Si existe el documento
		//El documento existe en la colección
		id= auxDoc[nomDoc];
		existe=true;
		for(auto it2=indice.begin();it2!=indice.end();++it2){//Recorremos el índice de terminos
			it= it2->second;
			l_docs_local= it.getDocs();
			if(l_docs_local.find(id.getIdDoc()) != l_docs_local.end()){//Si existe el termino en el documento
				cout << it2->first << '\t' << it2->second << endl;
			}
		}
	}
	return existe;
}

void IndexadorHash::ListarDocs() const {
	//unordered_map<string, InfDoc>::iterator it;
	for(auto it=indiceDocs.begin();it!=indiceDocs.end();++it){
		cout << it->first << '\t' << it->second << endl;
	}
}

bool IndexadorHash::ListarDocs(const string& nomDoc) const {
	bool existe=false;
	if(indiceDocs.find(nomDoc) != indiceDocs.end()){//Si existe el documento
		cout << nomDoc << '\t' << indiceDocs.find(nomDoc)->second << endl;
		existe=true;
	}

	return existe;
}

int IndexadorHash::PalDistintasDoc(const string& nomDoc) const {
	int cuenta, id;
	cuenta=0;
	unordered_map<string, InfDoc> auxDoc= indiceDocs;
	InfDoc infdoc;
	infdoc=auxDoc[nomDoc];
	id= infdoc.getIdDoc();
	unordered_map<long int,InfTermDoc> l_docs_local;
	//unordered_map<string, InformacionTermino>::iterator it;
	for(auto it=indice.begin();it!=indice.end();++it){//Recorre todos los terminos indexados
		l_docs_local=it->second.getDocs();
		if(l_docs_local.find(id)!=l_docs_local.end()){
			++cuenta;
		}
	}
	return cuenta;
}

IndexadorHash::IndexadorHash() {
	pregunta="";
	ficheroStopWords="";
	directorioIndice="";
	tipoStemmer=0;
	almacenarEnDisco=false;
	almacenarPosTerm=false;
}

