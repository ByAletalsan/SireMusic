/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   songs.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atalaver <atalaver@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 16:25:00 by atalaver          #+#    #+#             */
/*   Updated: 2023/11/11 19:41:01 by atalaver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef songs_h
#define songs_h
#include <iostream>
#include <string>
#include <vector>

namespace songs {

class Song{
	private:
		double day, hour, min; //day (0:monday, 1:tuesday, ..., 6:sunday)
		unsigned channel; //0 -> both, 1 -> right, 2 -> left
		std::vector<std::string> listSongs;
		unsigned sizeList;
	public:
		
		Song();
		Song( const Song &s );
		~Song();
		Song	&operator=( const Song &s );

		bool	operator<( const Song &s1 ) const;

		//Read and Write in main

    	void create(std::vector<std::string> _listSongs, unsigned _channel, unsigned _day, unsigned _hour, unsigned _min);

		//Get Values

    	std::string getSong(unsigned pos) const; //Return song in this pos

		unsigned 					getSizeList() const;
		double 						getMin() const;
		double 						getHour() const;
		double 						getDay() const;
		unsigned 					getChannel() const;

};
}
#endif 
