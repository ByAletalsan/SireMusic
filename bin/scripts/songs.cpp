/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   songs.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atalaver <atalaver@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 16:24:55 by atalaver          #+#    #+#             */
/*   Updated: 2023/11/11 20:18:27 by atalaver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "songs.hpp"
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

	Song::Song( const Song &s )
	{
		this->min = s.min;
		this->hour = s.hour;
		this->day = s.day;
		this->channel = s.channel;
		this->sizeList = s.sizeList;
		this->listSongs = s.listSongs;
	}

	Song::~Song()
	{
	}

	Song	&Song::operator=( const Song &s )
	{
		if (this != &s)
		{
			this->min = s.min;
			this->hour = s.hour;
			this->day = s.day;
			this->channel = s.channel;
			this->sizeList = s.sizeList;
			this->listSongs = s.listSongs;
		}
		return (*this);
	}

	void Song::create(std::vector<std::string> _listSongs, unsigned _channel, unsigned _day, unsigned _hour, unsigned _min){
		this->sizeList = _listSongs.size();
		this->listSongs = _listSongs;
		this->channel = _channel;
		this->day = _day;
		this->hour = _hour;
		this->min = _min;
		
	}

	bool	Song::operator<( const Song &s ) const
	{
		if (this->day != s.getDay())
			return (this->day < s.getDay());
		else if (this->hour != s.getHour())
			return (this->hour < s.getHour());
		else
			return (this->min < s.getMin());
	}

	unsigned Song::getSizeList() const{
		return this->listSongs.size();
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
