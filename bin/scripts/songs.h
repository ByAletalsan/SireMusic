#ifndef songs_h
#define songs_h
#include <string>

namespace songs {

class Song{
	private:
		double day, hour, min; //day (0:monday, 1:tuesday, ..., 6:sunday)
		unsigned channel; //0 -> both, 1 -> right, 2 -> left
		static const unsigned MAX_SONGS = 10; //Canciones maximas seguidas
		std::string listSongs[MAX_SONGS];
		unsigned sizeList;
	public:
		
		Song();	

		//Read and Write in main

    void create(unsigned _sizeList, std::string _listSongs[], unsigned _channel, unsigned _day, unsigned _hour, unsigned _min);

		//Get Values

		unsigned getSizeList() const;

    std::string getSong(unsigned pos) const; //Return song in this pos

		double getMin() const;
		
		double getHour() const;

		double getDay() const;

		unsigned getChannel() const;

};
}
#endif 
