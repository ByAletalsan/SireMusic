#include <iostream>
#include <string>
#include <fstream>
#include "songs.h"
#include "player.h"
#include <stdlib.h>

using namespace std;
using namespace songs;

namespace player {

	Music_Player::Music_Player(){
		this->sizeList = 0;
	}	

	void Music_Player::read(){
		songs::Song molde;
		ifstream leer;
		leer.open("bin/data/alarm_data.txt");
		string listSongs[10];
		unsigned size = 0, channel, day, hour, min;
		string l, cancion;
		leer >> this->sizeList;
		for(unsigned i = 0; i < this->sizeList; i++){
			leer >> size;
			for(unsigned j = 0; j < size; j++){
				cancion = "";
				do{
					leer >> l;
					cancion += l;
					cancion += " ";
					if(l == ""){
						leer.close();
						return;
					}
					if(l.length() < 4){
						l += "nomefastidies";
					}
				}while(l.substr(l.length() - 4, 4) != ".mp3");
				cancion = cancion.substr(0, cancion.length() - 1);
				listSongs[j] = cancion;
			}
			leer >> channel;
			leer >> day;
			leer >> hour;
			leer >> min;
			molde.create(size, listSongs, channel, day, hour, min);
			this->listPlayer[i] = molde;
		}
		leer.close();
	}

	void Music_Player::write(){
		ofstream escribir;
		escribir.open("bin/data/alarm_data.txt");
		escribir << this->sizeList << endl;
		for(unsigned i = 0; i < this->sizeList; i++){
			escribir << this->listPlayer[i].getSizeList() << " ";
			for(unsigned j = 0; j < this->listPlayer[i].getSizeList(); j++){
				escribir << this->listPlayer[i].getSong(j) << " ";
			}
			escribir << this->listPlayer[i].getChannel() << " ";
			escribir << this->listPlayer[i].getDay() << " ";
			escribir << this->listPlayer[i].getHour() << " ";
			escribir << this->listPlayer[i].getMin() << endl;
		}
		escribir.close();
	}

	void Music_Player::insert(songs::Song _song){
		this->listPlayer[this->sizeList++] = _song;
	}

	bool Music_Player::deleted(unsigned pos){
		if(pos < 0 || pos >= this->sizeList){
			return false;
		}
		for(unsigned i = pos; i < this->sizeList - 1; i++){
			this->listPlayer[i] = this->listPlayer[i + 1];
		}
		this->sizeList -= 1;
		write();
		return true;
	}

	void Music_Player::print() const{
		string dias[7] = {"LUNES", "MARTES", "MIERCOLES", "JUEVES", "VIERNES", "SABADO", "DOMINGO"};
		string chan[3] = {"AMBOS", "DERECHA", "IZQUIERDA"};
		for(unsigned i = 0; i < this->sizeList; i++){
			cout << "\033[1;41m";
			if(i < 10) cout << "0";
			cout << i << ". __" << dias[unsigned(this->listPlayer[i].getDay())] << " __ " << this->listPlayer[i].getHour() << " : " << this->listPlayer[i].getMin() << " __ "; 

			for(unsigned j = 0; j < this->listPlayer[i].getSizeList(); j++){
				cout << "| " << this->listPlayer[i].getSong(j) << " | ";
			}
			cout << "__ " << chan[this->listPlayer[i].getChannel()] << " __\033[1;0m" << endl;
		}
	}

	unsigned Music_Player::getSizeList() const{
		return this->sizeList;
	}

	songs::Song Music_Player::getSongs(unsigned pos) const{
		return this->listPlayer[pos];
	}
}
