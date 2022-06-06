//compilar con: g++ -Wall -lpthread -o SireMusic/SireMusic SireMusic/bin/scripts/main.cpp SireMusic/bin/scripts/songs.cpp SireMusic/bin/scripts/player.cpp
//instalar: sudo apt-get install git
					//sudo apt-get install evince
//Cosas que hacer
/*
1. Probar si funcionan varias canciones seguidas (HECHO)
2. Probar si funciona con canciones combinadas (HECHO)
3. Hacer apartado de ayuda
4. Mejorar un poco la interfac gráfica (HECHO)
5. Diseñar el instalador desde github y un acceso directo
6. Diseñar un desinstalador
*/
#include <iostream>
#include <string>
#include "songs.h"
#include <time.h>
#include "player.h"
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <chrono>
#include <thread>


using namespace std;
using namespace songs;
using namespace player;

string dias[7] = {"LUNES", "MARTES", "MIERCOLES", "JUEVES", "VIERNES", "SABADO", "DOMINGO"};

string fileSongs[1000]; //Canciones en la carpeta CHANNEL_0
float timeSongs[1000]; //Tiempos
unsigned size_fileSongs;

void lectura();
void lectura_songs();
void creacion(string c);
void run_th(bool& fin, Music_Player &player, bool& menu, string& cancion_reproduciendo, bool& terminado);
void menu_th(bool& fin, Music_Player &player, bool& menu, bool& estoy_menu);
void reproduciendo_th(bool& fin, bool& estoy_menu, bool& menu, string& cancion_reproduciendo, bool& terminado);

int main(){

	Music_Player player;

		//INTRO
		system("clear");
		cout << "\033[1;35m"; //Color
		sleep(1);
		cout << " #####  ### ######  ####### #     # #     #  #####  ###  #####  " << endl;
		cout << "#     #  #  #     # #       ##   ## #     # #     #  #  #     # " << endl;
		sleep(1);
		cout << "#        #  #     # #       # # # # #     # #        #  #       " << endl;
		cout << " #####   #  ######  #####   #  #  # #     #  #####   #  #       " << endl;
		sleep(1);
		cout << "      #  #  #   #   #       #     # #     #       #  #  #       " << endl;
		cout << "#     #  #  #    #  #       #     # #     # #     #  #  #     # " << endl;
		sleep(1);
		cout << " #####  ### #     # ####### #     #  #####   #####  ###  #####  " << endl;
		sleep(2);
		cout << "\n\033[1;96mCreado por: ByAletalsan" << endl;
		sleep(3);

	//HILOS
	bool fin = false;
	bool menu = true;
	bool estoy_menu = true;
	bool terminado = false;
	string cancion_reproduciendo;
	thread th_menu(menu_th, ref(fin), ref(player), ref(menu), ref(estoy_menu));
  thread th_run(run_th, ref(fin), ref(player), ref(menu), ref(cancion_reproduciendo), ref(terminado));
	thread th_reproduciendo(reproduciendo_th, ref(fin), ref(estoy_menu), ref(menu), ref(cancion_reproduciendo), ref(terminado));

	if(th_run.joinable()) {
      th_run.join();
  }
	if(th_menu.joinable()) {
      th_menu.join();
  }
	if(th_reproduciendo.joinable()) {
      th_reproduciendo.join();
  }

	return 0;

} //main

void reproduciendo_th(bool& fin, bool& estoy_menu, bool& menu, string& cancion_reproduciendo, bool& terminado){
	bool funcionando = false;
	while(fin == false){
		if(!menu && estoy_menu && !funcionando)	{
			funcionando = true;
			system("clear");
			cout << endl;
			cout << "\033[1;93m________________" << endl;
		cout << "\033[1;93m|_____MENU_____| \033[1;32m(v7.0.0)Alpha\033[1;37m" << endl;
		cout << endl;
		cout << "\033[1;35m" << "# Reproduciendo: " << cancion_reproduciendo << " #\033[1;37m" << endl;
		cout << endl;
		cout << "1 -> CREAR ALARMA" << endl;
		cout << endl;
		cout << "2 -> BORRAR ALARMA" << endl;
		cout << endl;
		cout << "3 -> VER ALARMAS" << endl;
		cout << endl;
		cout << "4 -> VER CANCIONES DISPONIBLES" << endl; 
		cout << endl;
		cout << "5 -> BORRAR CANCIONES DISPONIBLES" << endl; 
		cout << endl;
		cout << "6 -> CREAR CANCION COMBINADA" << endl;
		cout << endl;
		cout << "7 -> AYUDA" << endl;
		cout << endl;
		cout << "\033[1;91m8 -> SALIR\033[1;0m\n\n";
		}
		if(menu || !estoy_menu) funcionando = false;
		if(terminado && estoy_menu){
			system("clear");
			cout << endl;
			cout << "\033[1;93m________________" << endl;
		cout << "\033[1;93m|_____MENU_____| \033[1;32m(v7.0.0)Alpha\033[1;37m" << endl;
		cout << endl;
		cout << endl;
		cout << "1 -> CREAR ALARMA" << endl;
		cout << endl;
		cout << "2 -> BORRAR ALARMA" << endl;
		cout << endl;
		cout << "3 -> VER ALARMAS" << endl;
		cout << endl;
		cout << "4 -> VER CANCIONES DISPONIBLES" << endl; 
		cout << endl;
		cout << "5 -> BORRAR CANCIONES DISPONIBLES" << endl; 
		cout << endl;
		cout << "6 -> CREAR CANCION COMBINADA" << endl;
		cout << endl;
		cout << "7 -> AYUDA" << endl;
		cout << endl;
		cout << "\033[1;91m8 -> SALIR\033[1;0m\n\n";
		terminado = false;
		} else if(terminado && !estoy_menu){
			terminado = false;
		}
	}
}

void run_th(bool& fin, Music_Player &player, bool& menu, string& cancion_reproduciendo, bool& terminado){
	time_t now = time(0);
	tm * tim = localtime(&now);
	double dia = tim->tm_wday, hora = tim->tm_hour, min = tim->tm_min;

	string c[3] = {"channel_0", "channel_1", "channel_2"};

	string cancion;

	while(fin == false){
		for(unsigned i = 0; i < player.getSizeList(); i++){
			if(player.getSongs(i).getDay() == (dia - 1)){
				if(player.getSongs(i).getHour() == hora){
					if(player.getSongs(i).getMin() == min){
							for(unsigned j = 0; j < player.getSongs(i).getSizeList(); j++){
								menu = false;
								cancion_reproduciendo = player.getSongs(i).getSong(j);
								cancion = "play \"bin/music/" + c[player.getSongs(i).getChannel()] + "/" + player.getSongs(i).getSong(j) + "\" -q";
								system(cancion.c_str());
								terminado = true;
								menu = true;
								sleep(1);
							}
						}
					}
				}
			}
			time_t now = time(0);
			tm * tim = localtime(&now);
			dia = tim->tm_wday;
			hora = tim->tm_hour;
			min = tim->tm_min;
			sleep(1);
	}

}

void menu_th(bool& fin, Music_Player &player, bool& menu, bool& estoy_menu){

	//Molde para agregar canciones
	Song m_song;
	string listSongs[100];
	unsigned sizeList;
	string channel, day, hour, min;
	unsigned a_hour, a_min, a_sec;

	//Cargar canciones
	player.read();
	lectura_songs();
	lectura();

	//Menu
	string opcion = "0", mas;		
	unsigned minn, sec;
	unsigned f_hour, f_min, f_sec;
	string comando;

while(opcion[0] != '8'){
		estoy_menu = true;
		system("clear");
	
		cout << endl;
		cout << "\033[1;93m________________" << endl;
		cout << "\033[1;93m|_____MENU_____| \033[1;32m(v7.0.0)Alpha\033[1;37m" << endl;
		cout << endl;
		cout << endl;
		cout << "1 -> CREAR ALARMA" << endl;
		cout << endl;
		cout << "2 -> BORRAR ALARMA" << endl;
		cout << endl;
		cout << "3 -> VER ALARMAS" << endl;
		cout << endl;
		cout << "4 -> VER CANCIONES DISPONIBLES" << endl; 
		cout << endl;
		cout << "5 -> BORRAR CANCIONES DISPONIBLES" << endl; 
		cout << endl;
		cout << "6 -> CREAR CANCION COMBINADA" << endl;
		cout << endl;
		cout << "7 -> AYUDA" << endl; //Apartado con las preguntas más frecuentes (VOY POR AQUI)
		cout << endl;
		cout << "\033[1;91m8 -> SALIR\033[1;0m" << endl;
		cout << endl;

		cout << "\033[1;36m\\> "; cin >> opcion; cout << "\033[1;0m";
		while(!isdigit(opcion[0]) || (opcion.length() != 1) || (unsigned(opcion[0] - '0') < 1) || (unsigned(opcion[0] - '0') > 8)){
			cout << "\033[1;36m\\> "; cin >> opcion; cout << "\033[1;0m";
		}
		while((opcion == "2" || opcion == "5") && !menu){
			cout << endl;
			cout << "\033[1;41mMIENTRAS SE REPRODUCE NO ESTÁ PERMITIDO BORRAR NADA\033[1;0m" << endl;
			cout << endl;
			cout << "\033[1;36m\\> "; cin >> opcion; cout << "\033[1;0m";
		}

		estoy_menu = false;

			switch(opcion[0]){

				case '1':
					system("clear");
					if(size_fileSongs < 1){
						cout << "NO HAY CANCIONES PARA ELEGIR..." << endl;
						sleep(2);
						break;
					}
					sizeList = 0;
					cout << "\033[1;41m____DIA __ HORA : MIN __ CANCION____CANAL__\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;37mDIA? \033[1;0m(1.LUNES, 2.MARTES, 3.MIERCOLES, 4.JUEVES, 5.VIERNES, 6.SABADO, 7.DOMINGO)\033[1;37m" << endl;
					cout << endl;
					cout << "\033[1;36m\\> "; cin >> day; cout << "\033[1;0m";
					while((unsigned(day[0] - '0') < 1 || unsigned(day[0] - '0') > 7) || !isdigit(day[0]) || (day.length() != 1)){
						cout << "\033[1;36m\\> "; cin >> day; cout << "\033[1;0m";
					}
					system("clear");
					cout << "\033[1;41m____" << dias[unsigned(day[0] - '0') - 1] << " __ HORA : MIN __ CANCION____CANAL__\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;37mHORA? \033[1;0m(00h a 23h) Ej:00, 01, 12...\033[1;0m\033[1;37m" << endl;
					cout << endl;
					cout << "\033[1;36m\\> "; cin >> hour; cout << "\033[1;0m";
					while((unsigned(hour[0] - '0') < 0 || unsigned(hour[0] - '0') > 2) || (unsigned(hour[0] - '0') == 2 && unsigned(hour[1] - '0') > 3) || !isdigit(hour[0]) || !isdigit(hour[1]) || (hour.length() != 2)){
						cout << "\033[1;36m\\> "; cin >> hour; cout << "\033[1;0m";
					}
					system("clear");
					cout << "\033[1;41m____" << dias[unsigned(day[0] - '0') - 1] << " __ " << hour << " : MIN __ CANCION____CANAL__\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;37mMINUTOS? \033[1;0m(00m a 59m) Ej:00, 01, 12...\033[1;0m\033[1;37m" << endl;
					cout << "\033[1;36m\\> "; cin >> min; cout << "\033[1;0m";
					while((unsigned(min[0] - '0') < 0 || unsigned(min[0] - '0') > 5) || !isdigit(min[0]) || !isdigit(min[1]) || (min.length() != 2)){
						cout << "\033[1;36m\\> "; cin >> min; cout << "\033[1;0m";
					}
					system("clear");
					cout << "\033[1;41m____" << dias[unsigned(day[0] - '0') - 1] << " __ " << hour << " : " << min << " __ CANCION____CANAL__\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;37mCANCION? Ej:000, 001, 012...\033[1;37m" << endl;
					cout << endl;
					for(unsigned i = 0; i < size_fileSongs; i++){
						sec = timeSongs[i] + 1;
						minn = 0;
						while(sec >= 60){
							sec -= 60;
							minn++;
						}
						f_hour = stoi(hour);
						f_min = stoi(min);
						f_sec = 0;
						f_sec += sec;
						f_min += minn;
						while(f_sec >= 60){
							f_sec -= 60;
							f_min++;
						}
						while(f_min >= 60){
							f_min -= 60;
							f_hour++;
						}
						cout << "\033[1;36m";
						if(i < 10) cout << "0";
						if(i < 100) cout << "0";
						cout << i << ". " << fileSongs[i] << " (" << minn << ":" << sec << ")";
						cout << "\033[1;0m :: \033[1;37m(" << hour << "h:" << min << "m:00s) -> (" << f_hour << "h:" << f_min << "m:" << f_sec << "s)" << endl;
					}
					cout << endl;
					cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
					while(!isdigit(mas[0]) || !isdigit(mas[1]) || !isdigit(mas[2]) || (mas.length() != 3) || (stoi(mas) < 0) || (unsigned(stoi(mas)) > size_fileSongs)){
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
					}
					listSongs[sizeList++] = fileSongs[stoi(mas)];
					sec = timeSongs[stoi(mas)] + 1;
					minn = 0;
					while(sec >= 60){
						sec -= 60;
						minn++;
					}
					f_sec = sec;
					f_min = minn;
					f_hour = stoi(hour);
					while(f_min >= 60){
						f_min -= 60;
						f_hour++;
					}
					cout << endl;
					system("clear");
					cout << "\033[1;41m____" << dias[unsigned(day[0] - '0') - 1] << " __ " << hour << " : " << min << " __ " << listSongs[sizeList - 1] << "____CANAL__\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;37mCANAL? \033[1;0m(0.Ambos, 1.Derecho, 2.Izquierdo)\033[1;0m\033[1;37m" << endl;
					cout << endl;
					cout << "\033[1;36m\\> "; cin >> channel; cout << "\033[1;0m";
					while((unsigned(channel[0] - '0') < 0 || unsigned(channel[0] - '0') > 2) || !isdigit(channel[0]) || (channel.length() != 1)){
						cout << "\033[1;36m\\> "; cin >> channel; cout << "\033[1;0m";
					}
					system("clear");
					cout << "\033[1;41m____" << dias[unsigned(day[0] - '0') - 1] << " __ " << hour << " : " << min << " __ " << listSongs[sizeList - 1] << "____" << channel << "__\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;41mGUARDADO!\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;37mDeseas reproducir una cancion seguida de esta? \033[1;0m(Si : 1 / No : 0)\033[1;0m\033[1;37m" << endl;
					cout << endl;
					cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
					while(!isdigit(mas[0]) || (mas.length() != 1) || (mas[0] != '1' && mas[0] != '0')){
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
					}
					while(mas[0] == '1'){
						system("clear");
						a_hour = f_hour, a_min = f_min, a_sec = f_sec;
						cout << "\033[1;32mCANCIONES YA SELECCIONADAS" << endl;
						cout << "\033[1;36m";
						for(unsigned i = 0; i < sizeList; i++){ //Canciones ya seleccionadas
							cout << listSongs[i] << endl;
						}
						cout << "\033[1;37m\nCANCION? Ej:00, 01, 12...\033[1;37m" << endl;
						for(unsigned i = 0; i < size_fileSongs; i++){
							sec = timeSongs[i] + 1;
							minn = 0;
							while(sec >= 60){
								sec -= 60;
								minn++;
							}
							f_hour = a_hour;
							f_min = a_min;
							f_sec = a_sec;
							f_sec += sec;
							f_min += minn;
							while(f_sec >= 60){
								f_sec -= 60;
								f_min++;
							}
							while(f_min >= 60){
								f_min -= 60;
								f_hour++;
							}
							cout << "\033[1;36m";
							if(i < 10) cout << "0";
							if(i < 100) cout << "0";
							while(a_sec >= 60){
								a_sec -= 60;
								a_min++;
							}
							while(a_min >= 60){
								a_min -= 60;
								a_hour++;
							}
							cout << i << ". " << fileSongs[i] << " (" << minn << ":" << sec << ")";
							cout << "\033[1;0m :: \033[1;37m(" << a_hour << "h:" << a_min << "m:" << a_sec << "s) -> (" << f_hour << "h:" << f_min << "m:" << f_sec << "s)" << endl;
					}
						cout << endl;
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						while(!isdigit(mas[0]) || !isdigit(mas[1]) || !isdigit(mas[2]) || (mas.length() != 3) || (stoi(mas) < 0) || (unsigned(stoi(mas)) > size_fileSongs)){
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						}
						listSongs[sizeList++] = fileSongs[stoi(mas)];
						sec = timeSongs[stoi(mas)] + 1;
						minn = 0;
						while(sec >= 60){
							sec -= 60;
							minn++;
						}
						f_sec = a_sec;
						f_min = a_min;
						f_hour = a_hour;
						f_sec += sec;
						f_min += minn;
						while(f_min >= 60){
							f_min -= 60;
							f_hour++;
						}
						cout << endl;
						cout << "\033[1;41mGUARDADO!\033[1;0m" << endl;
						cout << endl;
						cout << "\033[1;37mDeseas reproducir una cancion seguida de esta? \033[1;0m(Si : 1 / No : 0)\033[1;0m\033[1;37m" << endl;
						cout << endl;
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						while(!isdigit(mas[0]) || (mas.length() != 1) || (mas[0] != '1' && mas[0] != '0')){
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						}
					}
					m_song.create(sizeList, listSongs, (channel[0] - '0'), ((day[0] - '0') - 1), stoi(hour), stoi(min));
					player.insert(m_song);
					player.write();
					system("clear");
					break;
			
				case '2':
					do{
						system("clear");
						cout << "\033[1;33mSELECCIONA UNA ALARMA Ej:00, 01, 12... (PARA SALIR SELLECIONE UNA QUE NO ESTÉ)\033[1;0m" << endl;
						cout << endl;
						player.print();
						cout << endl;
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						while(!isdigit(mas[0]) || !isdigit(mas[1]) || (mas.length() != 2)){
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						}
						if(player.deleted(stoi(mas))){
							cout << endl;
							cout << "\033[1;41mBORRADO CON EXITO!\033[1;0m" << endl;
							sleep(1);
							system("clear");
							cout << "\033[1;33mDeseas borrar  otra alarma? (Si : 1 / No : 0)\033[1;0m" << endl;
							cout << endl;
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
							while(!isdigit(mas[0]) || (mas.length() != 1) || (mas[0] != '1' && mas[0] != '0')){
								cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
							}
						} else {
							cout << endl;
							cout << "\033[1;41mSALIENDO!\033[1;0m" << endl;
							sleep(1);
							mas = "0";
						}
					} while(mas[0] == '1');
					system("clear");
					break;

				case '3':
					system("clear");
					player.print();
					cout << endl;
					cout << "\033[1;43mEscribe (0) SALIR\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
					while(mas[0] != '0' || !isdigit(mas[0]) || (mas.length() != 1)){
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
					}
					system("clear");
					break;

				case '4':
					system("clear");
					cout << "\033[1;33mCANCIONES DISPONIBLES\033[1;37m" << endl;
					cout << endl;
					for(unsigned i = 0; i < size_fileSongs; i++){
							sec = timeSongs[i] + 1;
							minn = 0;
							while(sec >= 60){
								sec -= 60;
								minn++;
							}
							if (i < 10) cout << "0";
							if (i < 100) cout << "0";
							cout << i << ". " << fileSongs[i] << " (" << minn << ":" << sec << ")" <<  endl;
					}
					if(size_fileSongs == 0) cout << "\n\033[1;43mUPS... NO HAY NADA :/\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;43mEscribe (0) SALIR\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
					while(mas[0] != '0' || !isdigit(mas[0]) || (mas.length() != 1)){
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
					}
					system("clear");
					break;

				case '5':
					do{
						system("clear");
						cout << "\033[1;33mSELECCIONA UNA CANCION Ej:00, 01, 12... (PARA SALIR SELLECIONE UNA QUE NO ESTÉ)\033[1;0m" << endl;
						cout << endl;
						cout << "\033[1;37m";
						for(unsigned i = 0; i < size_fileSongs; i++){
							sec = timeSongs[i];
							minn = 0;
							while(sec >= 60){
								sec -= 60;
								minn++;
							}
							if (i < 10) cout << "0";
							if (i < 100) cout << "0";
							cout << i << ". " << fileSongs[i] << " (" << minn << ":" << sec << ")" <<  endl;
						}
						cout << endl;
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						while(!isdigit(mas[0]) || !isdigit(mas[1]) || !isdigit(mas[2]) || (mas.length() != 3) || (stoi(mas) < 0) || (unsigned(stoi(mas)) > size_fileSongs)){
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						}
						if((stoi(mas) >= 0) && (unsigned(stoi(mas)) < size_fileSongs)){
							comando = "rm \"bin/music/channel_0/" + fileSongs[stoi(mas)] + "\" > /dev/null 2>&1 &";
							system(comando.c_str());
							comando = "rm \"bin/music/channel_1/" + fileSongs[stoi(mas)] + "\" > /dev/null 2>&1 &";
							system(comando.c_str());
							comando = "rm \"bin/music/channel_2/" + fileSongs[stoi(mas)] + "\" > /dev/null 2>&1 &";
							system(comando.c_str());
							for(unsigned i = stoi(mas); i < size_fileSongs - 1; i++){
								fileSongs[i] = fileSongs[i + 1];
							}
							size_fileSongs -= 1;
							cout << endl;
							cout << "\033[1;41mBORRADO CON EXITO!\033[1;0m" << endl;
							sleep(1);
							system("clear");
							cout << "\033[1;33mDeseas borrar  otra alarma? (Si : 1 / No : 0)\033[1;0m" << endl;
							cout << endl;
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
							while(!isdigit(mas[0]) || (mas.length() != 1)  || (mas[0] != '1' && mas[0] != '0')){
								cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
							}
						} else {
							cout << endl;
							cout << "\033[1;41mSALIENDO!\033[1;0m" << endl;
							sleep(1);
							mas = '0';
						}
					} while(mas[0] == '1');
					lectura_songs();
					system("clear");
					break;
				
				case '6':{
					system("clear");
					string c1, c2;
					string s;
					cout << "\033[1;33mSELECCIONA DOS CANCIONES Ej:00, 01, 12... (PARA SALIR SELLECIONE UNA QUE NO ESTÉ)\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;37m";
					for(unsigned i = 0; i < size_fileSongs; i++){
						sec = timeSongs[i] + 1;
						minn = 0;
						while(sec >= 60){
							sec -= 60;
							minn++;
						}
						if(i < 10) cout << "0";
						if(i < 100) cout << "0";
						cout << i << ". " << fileSongs[i] << " (" << minn << ":" << sec << ")" <<  endl;
					}
					cout << endl;
					cout << "\033[1;36mIzquierda -> "; cin >> c1; cout << "\033[1;0m";
					while(!isdigit(c1[0]) || !isdigit(c1[1]) || !isdigit(c1[2]) || (c1.length() != 3) || (stoi(c1) < 0) || (unsigned(stoi(c1)) > size_fileSongs)){
							cout << "\033[1;36mIzquierda -> "; cin >> c1; cout << "\033[1;0m";
						}
					if(stoi(c1) >= 0 && unsigned(stoi(c1)) < size_fileSongs){
						cout << "\033[1;36mDerecha -> "; cin >> c2; cout << "\033[1;0m";
						while(!isdigit(c2[0]) || !isdigit(c2[1]) || !isdigit(c2[2]) || (c2.length() != 3) || (stoi(c2) < 0) || (unsigned(stoi(c2)) > size_fileSongs)){
							cout << "\033[1;36mDerecha -> "; cin >> c2; cout << "\033[1;0m";
						}
						cout << "\033[1;36m";
						cout << "Nombre a esta nueva cancion: ";
						cin >> s; cout << "\033[1;0m";
						comando = "sox -M \"bin/music/channel_0/" + fileSongs[stoi(c1)] + "\" bin/music/channel_0/" + fileSongs[stoi(c2)] + " \"bin/music/channel_0/" + s + ".mp3\" > /dev/null 2>&1 &";
						system(comando.c_str());
						comando = "cp \"bin/music/channel_0/" + s + ".mp3\"" + " \"bin/music/channel_1/" + s + ".mp3\" > /dev/null 2>&1 &";
						system(comando.c_str());
						comando = "cp \"bin/music/channel_0/" + s + ".mp3\"" + " \"bin/music/channel_2/" + s + ".mp3\" > /dev/null 2>&1 &";
						system(comando.c_str());
					} else {
						cout << endl;
						cout << "\033[1;41mSALIENDO!\033[1;0m" << endl;
						sleep(1);
					}	
					lectura_songs();
					system("clear");
					}
					break;

				case '7':
					system("evince bin/7.pdf > /dev/null 2>&1 &");
					break;

		} //switch
	} //while
	fin = true;
	system("exit");
}

void lectura(){
	//Lectura
	system("sh bin/scripts/readSongs.sh");
	ifstream lectura;
	lectura.open("bin/data/temp.txt");
	string l, d, cancion;
	bool esta;
	while(!lectura.eof()){
		cancion = "";
		do{
			cancion += " ";
			lectura >> l;
			cancion += l;
			if(l == ""){
				lectura.close();
				return;
			}
			if(l.length() < 4){
				l += "nomefastidies";
			}
		}while(l.substr(l.length() - 4, 4) != ".mp3");
		cancion = cancion.substr(1, cancion.length());
		esta = false;
		for(unsigned i = 0; i < size_fileSongs; i++){
			if(cancion == fileSongs[i]){
				esta = true;
			}
		}
		if(!esta){
			creacion(cancion);
			fileSongs[size_fileSongs++] = l;
		}
	}
	string comando = "rm musica/*  > /dev/null 2>&1 &";
	system(comando.c_str());
	lectura_songs();
	lectura.close();
}

void lectura_songs(){
	system("sh bin/scripts/readChannel.sh");
	ifstream lectura, time;
	lectura.open("bin/data/songs_data.txt");
	time.open("bin/data/time_data.txt");
	size_fileSongs = 0;
	string l, t, cancion;
	while(!lectura.eof()){
		cancion = "";
		do{
			cancion += " ";
			lectura >> l;
			cancion += l;
			if(l == ""){
				lectura.close();
				time.close();
				return;
			}
			if(l.length() < 4){
				l += "nomefastidies";
			}
		}while(l.substr(l.length() - 4, 4) != ".mp3");
		cancion = cancion.substr(1, cancion.length());
		fileSongs[size_fileSongs] = cancion;		
		time >> t;
		timeSongs[size_fileSongs++] = atof(t.c_str());
	}
	size_fileSongs--;
	lectura.close();
	time.close();
}


void creacion(string c){
			string comando1, comando2, comando3_1, comando3_2;

			comando1 = "sox -v 0.95 \"musica/" + c + "\" -r 48000 \"bin/music/" + c + "\" ";
			comando2 = "sox \"bin/music/" + c + "\" \"bin/music/channel_0/" + c + "\" remix 1,2 ";
			comando3_1 = "sox -M \"bin/music/channel_0/" + c + "\" bin/music/silencio.mp3 \"bin/music/channel_2/" + c + "\" ";
			comando3_2 = "sox -M bin/music/silencio.mp3 \"bin/music/channel_0/" + c + "\" \"bin/music/channel_1/" + c + "\" ";
			system(comando1.c_str());
			system(comando2.c_str());
			cout << c << " : " << "Creado el mono" << endl;
			system(comando3_1.c_str());
			cout << c << " : " << "Creado el izquierdo" << endl;
			system(comando3_2.c_str());
			cout << c << " : " << "Creado el derecho" << endl;
			cout << c << " : " << "Despejado" << endl;
			comando1 = "rm \"bin/music/" + c + "\" ";
			system(comando1.c_str());
			cout << c << " : " << "CREADO!!" << endl;
}


