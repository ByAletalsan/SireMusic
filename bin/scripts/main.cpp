/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atalaver <atalaver@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 16:24:33 by atalaver          #+#    #+#             */
/*   Updated: 2023/11/12 00:34:32 by atalaver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "songs.hpp"
#include <time.h>
#include "player.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <chrono>
#include <thread>
#include "utils.hpp"

using namespace std;
using namespace songs;
using namespace player;

std::string dias[7] = {"LUNES", "MARTES", "MIERCOLES", "JUEVES", "VIERNES", "SABADO", "DOMINGO"};

std::vector<std::string> fileSongs;
std::vector<float> timeSongs;

void lectura();
void lectura_songs();
void creacion(string c);
void run_th(bool& fin, Music_Player &player, bool& menu, string& cancion_reproduciendo, bool& terminado);
void menu_th(bool& fin, Music_Player &player, bool& menu, bool& estoy_menu);
void reproduciendo_th(bool& fin, bool& estoy_menu, bool& menu, string& cancion_reproduciendo, bool& terminado);

void intro()
{
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
}

int main()
{
	Music_Player player;
	//intro();
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

void reproduciendo_th(bool& fin, bool& estoy_menu, bool& menu, string& cancion_reproduciendo, bool& terminado)
{
	bool funcionando = false;
	while(fin == false){
		if(!menu && estoy_menu && !funcionando)	{
			funcionando = true;
			system("clear");
			cout << endl;
			cout << "\033[1;93m________________" << endl;
		cout << "\033[1;93m|_____MENU_____| \033[1;32m(v7.1.0)Alpha\033[1;37m" << endl;
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
		cout << "5 -> CAMBIAR NOMBRE A UNA CANCION" << endl; 
		cout << endl;
		cout << "6 -> BORRAR CANCIONES DISPONIBLES" << endl; 
		cout << endl;
		cout << "7 -> CREAR CANCION COMBINADA" << endl;
		cout << endl;
		cout << "8 -> AYUDA" << endl;
		cout << endl;
		cout << "\033[1;91m9 -> SALIR\033[1;0m\n\n";
		}
		if(menu || !estoy_menu) funcionando = false;
		if(terminado && estoy_menu){
			system("clear");
			cout << endl;
			cout << "\033[1;93m________________" << endl;
		cout << "\033[1;93m|_____MENU_____| \033[1;32m(v7.1.0)Alpha\033[1;37m" << endl;
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
		cout << "5 -> CAMBIAR NOMBRE A UNA CANCION" << endl; 
		cout << endl;
		cout << "6 -> BORRAR CANCIONES DISPONIBLES" << endl; 
		cout << endl;
		cout << "7 -> CREAR CANCION COMBINADA" << endl;
		cout << endl;
		cout << "8 -> AYUDA" << endl;
		cout << endl;
		cout << "\033[1;91m9 -> SALIR\033[1;0m\n\n";
		terminado = false;
		} else if(terminado && !estoy_menu){
			terminado = false;
		}
	}
}

void run_th(bool& fin, Music_Player &player, bool& menu, string& cancion_reproduciendo, bool& terminado)
{
	time_t now = time(0);
	tm * tim = localtime(&now);
	double dia = tim->tm_wday == 0? 6 : tim->tm_wday - 1, hora = tim->tm_hour, min = tim->tm_min;

	string c[3] = {"channel_0", "channel_1", "channel_2"};

	string cancion;

	while(fin == false){
		for(unsigned i = 0; i < player.getSizeList(); i++){
			if(player.getSongs(i).getDay() == dia){
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

void menu_th(bool& fin, Music_Player &player, bool& menu, bool& estoy_menu)
{
	//Molde para agregar canciones
	Song m_song;
	std::vector<std::string> listSongs;
	string channel, day, hour, min;
	unsigned a_hour, a_min, a_sec;

	//Cargar canciones
	player.read();
	lectura_songs();
	lectura();

	//Menu
	string opcion = "0", mas, buffer;
	unsigned		index;	
	unsigned song_min, song_sec;
	unsigned f_hour, f_min, f_sec;
	string comando;


	while(opcion[0] != '9'){
		listSongs.clear();
		estoy_menu = true;
		system("clear");
	
		cout << endl;
		cout << "\033[1;93m________________" << endl;
		cout << "\033[1;93m|_____MENU_____| \033[1;32m(v7.1.0)Alpha\033[1;37m" << endl;
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
		cout << "5 -> CAMBIAR NOMBRE A UNA CANCION" << endl; 
		cout << endl;
		cout << "6 -> BORRAR CANCIONES DISPONIBLES" << endl; 
		cout << endl;
		cout << "7 -> CREAR CANCION COMBINADA" << endl;
		cout << endl;
		cout << "8 -> AYUDA" << endl; //Apartado con las preguntas más frecuentes (VOY POR AQUI)
		cout << endl;
		cout << "\033[1;91m9 -> SALIR\033[1;0m" << endl;
		cout << endl;

		cout << "\033[1;36m\\> "; cin >> opcion; cout << "\033[1;0m";
		while(!isdigit(opcion[0]) || (opcion.length() != 1) || (unsigned(opcion[0] - '0') < 1) || (unsigned(opcion[0] - '0') > 9)){
			cout << "\033[1;36m\\> "; cin >> opcion; cout << "\033[1;0m";
		}
		while((opcion == "2" || opcion == "5" || opcion == "6") && !menu){
			cout << endl;
			cout << "\033[1;41mMIENTRAS SE REPRODUCE NO ESTÁ PERMITIDO BORRAR NADA\033[1;0m" << endl;
			cout << endl;
			cout << "\033[1;36m\\> "; cin >> opcion; cout << "\033[1;0m";
		}

		estoy_menu = false;

			switch(opcion[0]){

				case '1':
					system("clear");
					if(fileSongs.size() < 1){
						cout << "NO HAY CANCIONES PARA ELEGIR..." << endl;
						sleep(2);
						break;
					}
					cout << "\033[1;37mDIA? \033[1;0m(1.LUNES, 2.MARTES, 3.MIERCOLES, 4.JUEVES, 5.VIERNES, 6.SABADO, 7.DOMINGO), salir\033[1;37m" << endl << endl;
					cout << "\033[1;41m____DIA __ HORA : MIN __ CANCION____CANAL__\033[1;0m" << endl << endl;
					cout << "\033[1;36m\\> "; cin >> day; cout << "\033[1;0m";
					if (day == "salir")
						break ;
					while (day.length() != 1 || !isStringDigit(day) || std::stoi(day) < 1 || std::stoi(day) > 7)
					{
						cout << "\033[1;36m\\> "; cin >> day; cout << "\033[1;0m";
						if (day == "salir")
							break ;
					}
					if (day == "salir")
						break ;
					system("clear");
					cout << "\033[1;37mHORA? \033[1;0m(00h a 23h) Ej:0, 7, 12..., salir\033[1;0m\033[1;37m" << endl << endl;
					cout << "\033[1;41m____" << dias[unsigned(day[0] - '0') - 1] << " __ HORA : MIN __ CANCION____CANAL__\033[1;0m" << endl << endl;
					cout << "\033[1;36m\\> "; cin >> hour; cout << "\033[1;0m";
					if (hour == "salir")
						break ;
					hour = ajustarLongitudString(hour, 2);
					while (hour.length() > 2 || !isStringDigit(hour) || std::stoi(hour) < 0 || std::stoi(hour) > 23)
					{
						cout << "\033[1;36m\\> "; cin >> hour; cout << "\033[1;0m";
						if (hour == "salir")
							break ;
						hour = ajustarLongitudString(hour, 2);
					}
					if (hour == "salir")
						break ;
					system("clear");
					cout << "\033[1;37mMINUTOS? \033[1;0m(00m a 59m) Ej:0, 7, 59..., salir\033[1;0m\033[1;37m" << endl << endl;
					cout << "\033[1;41m____" << dias[unsigned(day[0] - '0') - 1] << " __ " << hour << " : MIN __ CANCION____CANAL__\033[1;0m" << endl << endl;
					cout << "\033[1;36m\\> "; cin >> min; cout << "\033[1;0m";
					if (min == "salir")
						break ;
					min = ajustarLongitudString(min, 2);
					while (min.length() > 2 || !isStringDigit(min) || std::stoi(min) < 0 || std::stoi(min) > 59)
					{
						cout << "\033[1;36m\\> "; cin >> min; cout << "\033[1;0m";
						if (min == "salir")
							break ;
						min = ajustarLongitudString(min, 2);
					}
					if (min == "salir")
						break ;
					system("clear");
					for(unsigned i = 0; i < fileSongs.size() - 1; i++){
						//Calculo de cada uno
						song_sec = timeSongs[i] + 1;
						song_min = 0;
						while(song_sec >= 60){
							song_sec -= 60;
							song_min++;
						}
						f_hour = stoi(hour);
						f_min = stoi(min);
						f_sec = song_sec;
						f_min += song_min;
						while(f_sec >= 60){
							f_sec -= 60;
							f_min++;
						}
						while(f_min >= 60){
							f_min -= 60;
							f_hour++;
						}
						f_hour = f_hour % 24;
						cout << "\033[1;36m";
						std::string s_song_min = to_string(song_min), s_song_sec = to_string(song_sec);
						cout << i << ". " << fileSongs[i] << " (" << ajustarLongitudString(s_song_min, 2) << ":" << ajustarLongitudString(s_song_sec, 2) << ")";
						std::string s_f_hour = to_string(f_hour), s_f_min = to_string(f_min), s_f_sec = to_string(f_sec);
						cout << "\033[1;0m :: \033[1;37m(" << hour << "h:" << min << "m:00s) -> (" << ajustarLongitudString(s_f_hour, 2) << "h:" << ajustarLongitudString(s_f_min, 2) << "m:" << ajustarLongitudString(s_f_sec, 2) << "s)" << endl;
					}
					cout << endl;
					cout << "\033[1;37mCANCION? Ej:0, 12, 122... salir\033[1;37m" << endl << endl;
					cout << "\033[1;41m____" << dias[unsigned(day[0] - '0') - 1] << " __ " << hour << " : " << min << " __ CANCION____CANAL__\033[1;0m" << endl << endl;
					cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
					if (mas == "salir")
						break ;
					while (!isStringDigit(mas) || std::stoi(mas) < 0 || std::stoi(mas) > (int)fileSongs.size() - 1)
					{
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						if (mas == "salir")
							break ;
					}
					if (mas == "salir")
						break ;
					//Calculamos el definitivo
					listSongs.push_back(fileSongs[stoi(mas)]);
					song_sec = timeSongs[stoi(mas)] + 1;
					song_min = 0;
					while(song_sec >= 60){
						song_sec -= 60;
						song_min++;
					}
					f_hour = stoi(hour);
					f_min = stoi(min);
					f_sec = song_sec;
					f_min += song_min;
					while(f_sec >= 60){
						f_sec -= 60;
						f_min++;
					}
					while(f_min >= 60){
						f_min -= 60;
						f_hour++;
					}
					f_hour = f_hour % 24;
					cout << endl;
					system("clear");
					cout << "\033[1;37mCANAL? \033[1;0m(0.Ambos, 1.Derecho, 2.Izquierdo), salir\033[1;0m\033[1;37m" << endl << endl;
					cout << "\033[1;41m____" << dias[unsigned(day[0] - '0') - 1] << " __ " << hour << " : " << min << " __ " << listSongs[listSongs.size() - 1] << "____CANAL__\033[1;0m" << endl << endl;
					cout << "\033[1;36m\\> "; cin >> channel; cout << "\033[1;0m";
					if (channel == "salir")
						break ;
					while (channel.length() != 1 || !isStringDigit(channel) || std::stoi(channel) < 0 || std::stoi(channel) > 2)
					{
						cout << "\033[1;36m\\> "; cin >> channel; cout << "\033[1;0m";
						if (channel == "salir")
							break ;
					}
					if (channel == "salir")
						break ;
					system("clear");
					cout << "\033[1;41m____" << dias[unsigned(day[0] - '0') - 1] << " __ " << hour << " : " << min << " __ " << listSongs[listSongs.size() - 1] << "____" << channel << "__\033[1;0m" << endl << endl;
					cout << "\033[1;41mGUARDADO!\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;37mDeseas reproducir una cancion seguida de esta? \033[1;0m(Si : 1 / No : 0)\033[1;0m\033[1;37m" << endl;
					cout << endl;
					cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
					while (mas.length() != 1 || !isStringDigit(mas) || std::stoi(mas) < 0 || std::stoi(mas) > 1)
					{
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
					}
					while(mas[0] == '1')
					{
						system("clear");
						a_hour = f_hour, a_min = f_min, a_sec = f_sec;
						for(unsigned i = 0; i < fileSongs.size() - 1; i++){
							song_sec = timeSongs[i] + 1;
							song_min = 0;
							while(song_sec >= 60){
								song_sec -= 60;
								song_min++;
							}
							f_hour = a_hour;
							f_min = a_min + song_min;
							f_sec = a_sec + song_sec;
							while(f_sec >= 60){
								f_sec -= 60;
								f_min++;
							}
							while(f_min >= 60){
								f_min -= 60;
								f_hour++;
							}
							f_hour = f_hour % 24;
							cout << "\033[1;36m";
							std::string s_song_min = to_string(song_min), s_song_sec = to_string(song_sec);
							std::string s_a_hour = to_string(a_hour), s_a_min = to_string(a_min), s_a_sec = to_string(a_sec);
							std::string s_f_hour = to_string(f_hour), s_f_min = to_string(f_min), s_f_sec = to_string(f_sec);
							cout << i << ". " << fileSongs[i] << " (" << ajustarLongitudString(s_song_min, 2) << ":" << ajustarLongitudString(s_song_sec, 2) << ")";
							cout << "\033[1;0m :: \033[1;37m(" << ajustarLongitudString(s_a_hour, 2) << "h:" << ajustarLongitudString(s_a_min, 2) << "m:" << ajustarLongitudString(s_a_sec, 2);
							cout << "s) -> (" << ajustarLongitudString(s_f_hour, 2) << "h:" << ajustarLongitudString(s_f_min, 2) << "m:" << ajustarLongitudString(s_f_sec, 2) << "s)" << endl;
						}
						cout << endl;
						cout << "\033[1;32mCANCIONES YA SELECCIONADAS" << endl;
						cout << "\033[1;36m";
						for(unsigned i = 0; i < listSongs.size(); i++){ //Canciones ya seleccionadas
							cout << listSongs[i] << endl;
						}
						cout << "\033[1;37m\nCANCION? Ej:0, 12, 123..., salir -> (no seleccionar ninguna más)\033[1;37m" << endl;
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						if (mas == "salir")
							break ;
						while (!isStringDigit(mas) || std::stoi(mas) < 0 || std::stoi(mas) > (int)fileSongs.size() - 1)
						{
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
							if (mas == "salir")
								break ;
						}
						if (mas == "salir")
							break ;
						listSongs.push_back(fileSongs[stoi(mas)]);
						song_sec = timeSongs[stoi(mas)] + 1;
						song_min = 0;
						while(song_sec >= 60){
							song_sec -= 60;
							song_min++;
						}
						f_sec = a_sec + song_sec;
						f_min = a_min + song_min;
						f_hour = a_hour;
						while(f_sec >= 60){
							f_sec -= 60;
							f_min++;
						}
						while(f_min >= 60){
							f_min -= 60;
							f_hour++;
						}
						f_hour = f_hour % 24;
						cout << endl;
						cout << "\033[1;41mGUARDADO!\033[1;0m" << endl;
						cout << endl;
						cout << "\033[1;37mDeseas reproducir una cancion seguida de esta? \033[1;0m(Si : 1 / No : 0)\033[1;0m\033[1;37m" << endl;
						cout << endl;
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						while (mas.length() != 1 || !isStringDigit(mas) || std::stoi(mas) < 0 || std::stoi(mas) > 1)
						{
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						}
					}
					m_song.create(listSongs, (channel[0] - '0'), ((day[0] - '0') - 1), stoi(hour), stoi(min));
					player.insert(m_song);
					player.write();
					system("clear");
					break;
			
				case '2':
					do{
						system("clear");
						player.print();
						cout << endl;
						cout << "\033[1;33mSELECCIONA UNA ALARMA Ej:(0, 12, ...), (lunes, martes, ...), todos, salir\033[1;0m" << endl;
						cout << endl;
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						mas = stringToLower(mas);
						while (!isStringDigit(mas) || std::stoi(mas) < 0 || std::stoi(mas) > (int)player.getSizeList())
						{
							if (mas == "salir" || mas == "todos")
								break ;
							bool c = false;
							for (unsigned i = 0; i < 7; i++)
							{
								if (mas == stringToLower(dias[i]))
								{
									c = true;
									break ;
								}
							}
							if (c)
								break ;
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						}
						if (mas == "salir")
							break ;
						if (mas == "todos")
						{
							player.clear_all();
							cout << endl;
							cout << "\033[1;41mBORRADOS TODOS CON EXITO!\033[1;0m" << endl;
							sleep(1);
							system("clear");
						}
						else if (mas == "lunes" || mas == "martes" || mas == "miercoles" || mas == "jueves"
									|| mas == "viernes" || mas == "sabado" || mas == "domingo")
						{
							for (unsigned i = 0; i < 7; i++)
							{
								if (mas == stringToLower(dias[i]))
								{
									player.clear_day(i);
									break ;
								}
							}
							cout << endl;
							cout << "\033[1;41mBORRADOS TODOS LOS " << stringToUpper(mas) << "!\033[1;0m" << endl;
							sleep(1);
							system("clear");
						}
						else if(player.deleted(stoi(mas))){
							cout << endl;
							cout << "\033[1;41mBORRADO CON EXITO!\033[1;0m" << endl;
							sleep(1);
							system("clear");
							cout << "\033[1;33mDeseas borrar  otra alarma? (Si : 1 / No : 0)\033[1;0m" << endl;
							cout << endl;
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
							while (mas.length() != 1 || !isStringDigit(mas) || std::stoi(mas) < 0 || std::stoi(mas) > 1)
							{
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
					if(fileSongs.size() == 0)
					{
						cout << "\n\033[1;43mUPS... NO HAY NADA :/\033[1;0m" << endl;
						sleep(3);
						break;
					}
					for(unsigned i = 0; i < fileSongs.size() - 1; i++)
					{
						song_sec = timeSongs[i] + 1;
						song_min = 0;
						while(song_sec >= 60){
							song_sec -= 60;
							song_min++;
						}
						std::string s_song_min = to_string(song_min), s_song_sec = to_string(song_sec);
						cout << i << ". " << fileSongs[i] << " (" << ajustarLongitudString(s_song_min, 2) << ":" << ajustarLongitudString(s_song_sec, 2) << ")" <<  endl;
					}
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
					system("clear");
					cout << "\033[1;33mCANCIONES DISPONIBLES\033[1;37m" << endl;
					cout << endl;
					if(fileSongs.size() == 0)
					{
						cout << "\n\033[1;43mUPS... NO HAY NADA :/\033[1;0m" << endl;
						sleep(3);
						break;
					}
					for(unsigned i = 0; i < fileSongs.size() - 1; i++)
					{
						song_sec = timeSongs[i] + 1;
						song_min = 0;
						while(song_sec >= 60){
							song_sec -= 60;
							song_min++;
						}
						std::string s_song_min = to_string(song_min), s_song_sec = to_string(song_sec);
						cout << i << ". " << fileSongs[i] << " (" << ajustarLongitudString(s_song_min, 2) << ":" << ajustarLongitudString(s_song_sec, 2) << ")" <<  endl;
					}
					if(fileSongs.size() == 0) cout << "\n\033[1;43mUPS... NO HAY NADA :/\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;43mElige la cancion para cambiar de nombre (0, 1, 12, ...) salir\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
					if (mas == "salir")
						break ;
					while (!isStringDigit(mas) || std::stoi(mas) < 0 || std::stoi(mas) > (int)fileSongs.size() - 2)
					{
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						if (mas == "salir")
							break ;
					}
					if (mas == "salir")
						break ;
					index = stoi(mas);
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					cout << "\033[1;36m [" << fileSongs[index] << "] NUEVO NOMBRE (SIN EL .MP3) \\> "; getline(cin, mas); cout << "\033[1;0m";
					while (mas.find(".mp3") != std::string::npos || inVectorString(fileSongs, mas + ".mp3"))
					{
						cout << "\033[1;36m [" << fileSongs[index] << "] NUEVO NOMBRE (SIN EL .MP3) \\> "; getline(cin, mas); cout << "\033[1;0m";
					}
					cout << endl;
					buffer = "mv \"bin/music/channel_0/" + fileSongs[index] + "\" \"bin/music/channel_0/" + mas + ".mp3\"";
					system(buffer.c_str());
					buffer = "mv \"bin/music/channel_1/" + fileSongs[index] + "\" \"bin/music/channel_1/" + mas + ".mp3\"";
					system(buffer.c_str());
					buffer = "mv \"bin/music/channel_2/" + fileSongs[index] + "\" \"bin/music/channel_2/" + mas + ".mp3\"";
					system(buffer.c_str());
					lectura_songs();
					cout << "\033[1;41mCAMBIADO CON EXITO!\033[1;0m" << endl;
					sleep(3);
					system("clear");
					break;

				case '6':
					system("clear");
					if(fileSongs.size() == 0)
					{
						cout << "\n\033[1;43mUPS... NO HAY NADA :/\033[1;0m" << endl;
						sleep(3);
						break;
					}
					do{
						system("clear");
						cout << "\033[1;37m";
						for(unsigned i = 0; i < fileSongs.size(); i++){
							song_sec = timeSongs[i];
							song_min = 0;
							while(song_sec >= 60){
								song_sec -= 60;
								song_min++;
							}
							std::string s_song_min = to_string(song_min), s_song_sec = to_string(song_sec);
							cout << i << ". " << fileSongs[i] << " (" << ajustarLongitudString(s_song_min, 2) << ":" << ajustarLongitudString(s_song_sec, 2) << ")" <<  endl;
						}
						cout << endl;
						cout << "\033[1;33mSELECCIONA UNA CANCION Ej:0, 12, 123... salir\033[1;0m" << endl;
						cout << endl;
						cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
						if (mas == "salir")
							break ;
						while (!isStringDigit(mas) || std::stoi(mas) < 0 || std::stoi(mas) > (int)fileSongs.size() - 1)
						{
							cout << "\033[1;36m\\> "; cin >> mas; cout << "\033[1;0m";
							if (mas == "salir")
								break ;
						}
						if (mas == "salir")
							break ;
						if((stoi(mas) >= 0) && (unsigned(stoi(mas)) < fileSongs.size())){
							comando = "rm \"bin/music/channel_0/" + fileSongs[stoi(mas)] + "\" > /dev/null 2>&1 &";
							system(comando.c_str());
							comando = "rm \"bin/music/channel_1/" + fileSongs[stoi(mas)] + "\" > /dev/null 2>&1 &";
							system(comando.c_str());
							comando = "rm \"bin/music/channel_2/" + fileSongs[stoi(mas)] + "\" > /dev/null 2>&1 &";
							system(comando.c_str());
							fileSongs.erase(fileSongs.begin() + stoi(mas));
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
				
				case '7':{
					system("clear");
					if(fileSongs.size() < 2)
					{
						cout << "\n\033[1;43mUPS... NO HAY MIN 2 CANCIONES :/\033[1;0m" << endl;
						sleep(3);
						break;
					}
					string c1, c2;
					string s;
					cout << "\033[1;37m";
					for(unsigned i = 0; i < fileSongs.size(); i++)
					{
						song_sec = timeSongs[i] + 1;
						song_min = 0;
						while(song_sec >= 60){
							song_sec -= 60;
							song_min++;
						}
						std::string s_song_min = to_string(song_min), s_song_sec = to_string(song_sec);
						cout << i << ". " << fileSongs[i] << " (" << ajustarLongitudString(s_song_min, 2) << ":" << ajustarLongitudString(s_song_sec, 2) << ")" <<  endl;
					}
					cout << endl;
					cout << "\033[1;33mSELECCIONA DOS CANCIONES Ej:0, 12, 123... salir\033[1;0m" << endl;
					cout << endl;
					cout << "\033[1;36mIzquierda -> "; cin >> c1; cout << "\033[1;0m";
					if (c1 == "salir")
						break ;
					while (!isStringDigit(c1) || std::stoi(c1) < 0 || std::stoi(c1) > (int)fileSongs.size() - 1)
					{
						cout << "\033[1;36mIzquierda -> "; cin >> c1; cout << "\033[1;0m";
						if (c1 == "salir")
							break ;
					}
					if (c1 == "salir")
						break ;
					if(stoi(c1) >= 0 && unsigned(stoi(c1)) < fileSongs.size()){
						cout << "\033[1;36mDerecha -> "; cin >> c2; cout << "\033[1;0m";
						if (c2 == "salir")
							break ;
						while (!isStringDigit(c2) || std::stoi(c2) < 0 || std::stoi(c2) > (int)fileSongs.size() - 1)
						{
							cout << "\033[1;36mDerecha -> "; cin >> c2; cout << "\033[1;0m";
							if (c2 == "salir")
								break ;
						}
						if (c2 == "salir")
							break ;
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

				case '8':
					system("evince bin/7.pdf > /dev/null 2>&1 &");
					break;

		} //switch
	} //while
	fin = true;
	system("clear");
	system("exit");
}

void lectura()
{
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
		for(unsigned i = 0; i < fileSongs.size(); i++){
			if(cancion == fileSongs[i]){
				esta = true;
			}
		}
		if(!esta){
			creacion(cancion);
			fileSongs.push_back(l);
		}
	}
	string comando = "mv new_songs/* discoteca/. > /dev/null 2>&1 &";
	system(comando.c_str());
	lectura_songs();
	lectura.close();
}

void lectura_songs()
{
	system("sh bin/scripts/readChannel.sh");
	ifstream lectura, time;
	lectura.open("bin/data/songs_data.txt");
	time.open("bin/data/time_data.txt");
	string l, t, cancion;
	fileSongs.clear();
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
		fileSongs.push_back(cancion);		
		time >> t;
		timeSongs.push_back(atof(t.c_str()));
	}
	lectura.close();
	time.close();
}

void creacion(string c)
{
	string comando1, comando2, comando3_1, comando3_2;

	comando1 = "sox -v 0.95 \"new_songs/" + c + "\" -r 48000 \"bin/music/" + c + "\" ";
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
