/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atalaver <atalaver@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 16:24:47 by atalaver          #+#    #+#             */
/*   Updated: 2023/11/11 21:48:28 by atalaver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef player_h
#define player_h
#include "songs.hpp"

namespace player {

class Music_Player{
	private:
		std::vector<songs::Song> listPlayer;
		unsigned sizeList;

	public:
		
		Music_Player();

		void read();

		void write();

		void insert(songs::Song &_song);

		bool deleted(unsigned pos);

		void clear_day( unsigned day );

		void clear_all();

		void print();

		//Gets

		unsigned getSizeList() const;

		songs::Song getSongs(unsigned pos) const;

};
}
#endif 
