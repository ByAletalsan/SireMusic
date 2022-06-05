#include <iostream>
#include <string>
#include "songs.h"
#include <stdlib.h>
#include <fstream>

using namespace std;

namespace songs {

	Song::Song(){
		this->min = 0;
		this->hour = 0;
		this->day = 0;
		this->channel = 0;
		this->sizeList = 0;
	}

	void Song::create(unsigned _sizeList, std::string _listSongs[], unsigned _channel, unsigned _day, unsigned _hour, unsigned _min){
		this->sizeList = _sizeList;
		for(unsigned i = 0; i < this->sizeList; i++){
			this->listSongs[i] = _listSongs[i];
		}
		this->channel = _channel;
		this->day = _day;
		this->hour = _hour;
		this->min = _min;
		
	}

	unsigned Song::getSizeList() const{
		return this->sizeList;
	}
	
	std::string Song::getSong(unsigned pos) const{
		return this->listSongs[pos];
	}

	unsigned Song::getChannel() const{
		return this->channel;
	}

	double Song::getDay() const{
		return this->day;
	}

	double Song::getHour() const{
		return this->hour;
	}
		
	double Song::getMin() const{
		return this->min;
	}



}
