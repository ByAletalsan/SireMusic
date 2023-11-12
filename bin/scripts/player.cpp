/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atalaver <atalaver@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 16:24:40 by atalaver          #+#    #+#             */
/*   Updated: 2023/11/12 13:41:04 by atalaver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <fstream>
#include "songs.hpp"
#include "player.hpp"
#include "utils.hpp"
#include <stdlib.h>
#include <algorithm>

using namespace std;
using namespace songs;

namespace player {

	Music_Player::Music_Player(){
		this->sizeList = 0;
	}	

	void Music_Player::read(){
		ifstream leer;
		leer.open("bin/data/alarm_data.txt");
		std::vector<std::string> listSongs;
		unsigned size = 0, channel, day, hour, min;
		string l, cancion;
		leer >> this->sizeList;
		for(unsigned i = 0; i < this->sizeList; i++){
			songs::Song molde;
			listSongs.clear();
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
				listSongs.push_back(cancion);
			}
			leer >> channel;
			leer >> day;
			leer >> hour;
			leer >> min;
			molde.create(listSongs, channel, day, hour, min);
			this->listPlayer.push_back(molde);
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

	void Music_Player::insert(songs::Song &_song){
		this->listPlayer.push_back(_song);
		std::sort(this->listPlayer.begin(), this->listPlayer.end());
		this->sizeList++;
	}

	bool Music_Player::deleted(unsigned pos){
		if (pos >= this->sizeList)
			return false;
		this->listPlayer.erase(this->listPlayer.begin() + pos);
		this->sizeList -= 1;
		std::sort(this->listPlayer.begin(), this->listPlayer.end());
		write();
		return true;
	}

	void Music_Player::clear_day( unsigned day )
	{
		for (unsigned i = 0; i < this->sizeList; i++)
		{
			if (this->listPlayer[i].getDay() == day)
			{
				this->deleted(i);
				i--;
			}
		}
	}

	void Music_Player::clear_all()
	{
		this->listPlayer.clear();
		this->sizeList = 0;
	}

	void Music_Player::print(){
		std::sort(this->listPlayer.begin(), this->listPlayer.end());
		string dias[7] = {"LUNES", "MARTES", "MIERCOLES", "JUEVES", "VIERNES", "SABADO", "DOMINGO"};
		string chan[3] = {"AMBOS", "DERECHA", "IZQUIERDA"};
		for(unsigned i = 0; i < this->sizeList; i++){
			cout << "\033[1;41m";
			string	s_hour = to_string((int)this->listPlayer[i].getHour()), s_min = to_string((int)this->listPlayer[i].getMin());
			cout << i << ".\033[1;0m __";
			string color = "\033[0;3" + to_string((int)(this->listPlayer[i].getDay() + 1)) + "m";
			cout << color;
			cout << dias[unsigned(this->listPlayer[i].getDay())] << "\033[1;0m__\033[0;96m" << ajustarLongitudString(s_hour, 2) << " : " << ajustarLongitudString(s_min, 2) << "\033[1;0m__ "; 

			for(unsigned j = 0; j < this->listPlayer[i].getSizeList(); j++){
				cout << "| \033[0;93m" << this->listPlayer[i].getSong(j) << "\033[1;0m | ";
			}
			cout << "__\033[0;92m" << chan[this->listPlayer[i].getChannel()] << "\033[1;0m__" << endl;
			if (i + 1 < this->sizeList && this->listPlayer[i].getDay() != this->listPlayer[i + 1].getDay())
				cout << endl;
		}
	}

	unsigned Music_Player::getSizeList() const{
		return this->listPlayer.size();
	}

	songs::Song Music_Player::getSongs(unsigned pos) const{
		return this->listPlayer[pos];
	}
}
