#ifndef player_h
#define player_h
#include "songs.h"

namespace player {

class Music_Player{
	private:
		static const unsigned List_MAX = 100;
		typedef songs::Song tList[List_MAX];
		tList listPlayer;
		unsigned sizeList;
	public:
		
		Music_Player();	

		void read();

		void write();

		void insert(songs::Song _song);

		bool deleted(unsigned pos);

		void print() const;

		//Gets

		unsigned getSizeList() const;

		songs::Song getSongs(unsigned pos) const;

};
}
#endif 
