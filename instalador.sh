#!/bin/bash

echo "\033[38;2;173;216;230mInstalando actualizaciones...\033[0m"
sudo apt-get -y update
echo "\033[38;2;144;238;144mActualizaciones instaladas!\033[0m"
echo "\033[38;2;173;216;230mInstalando reproductor...\033[0m"
sudo apt update -y && sudo apt install sox -y
sudo apt-get install libsox-fmt-all
echo "\033[38;2;144;238;144mReproductor instalado!\033[0m"
echo "\033[38;2;173;216;230mInstalando constructor del programa...\033[0m"
sudo apt-get install build-essential
echo "\033[38;2;144;238;144mInstalado constructor!\033[0m"
echo "\033[38;2;173;216;230mInstalando herramientas necesarias...\033[0m"
sudo apt-get install git
sudo apt-get install evince
echo "\033[38;2;144;238;144mHerramientas instaladas!\033[0m"
echo "\033[38;2;173;216;230mCreando programa...\033[0m"

if [ -d "SireMusic" ] && [ -d "SireMusic/bin" ] && [ -d "SireMusic/bin/data" ] && [ -d "SireMusic/bin/scripts" ] && [ -d "SireMusic/bin/music" ]; then
	echo "\033[0;31mSireMusic detectado! Quieres actualizarlo [se conservaran las canciones] (yes, no):\033[0m"
	read opcion
	if [ "$opcion" = "yes" ] || [ "$opcion" = "y" ]; then
		sudo mkdir -p .sire_copia
		sudo mv SireMusic/bin/music .sire_copia/music_copia
		sudo mv SireMusic/bin/data .sire_copia/data_copia
		if [ -d "SireMusic/discoteca" ]; then
			sudo mv SireMusic/discoteca .sire_copia/discoteca_copia
		fi
		sudo rm -rf SireMusic
		sudo git clone https://github.com/ByAletalsan/SireMusic.git
		sudo rm -rf SireMusic/instalador.sh
		sudo rm -rf SireMusic/bin/music
		sudo rm -rf SireMusic/bin/data
		sudo mkdir -p SireMusic/new_songs
		if [ ! -d ".sire_copia/discoteca_copia" ]; then
			sudo mkdir -p SireMusic/discoteca
		fi
		sudo chmod 777 SireMusic
		sudo chmod 777 SireMusic/*
		sudo chmod 777 SireMusic/bin/*
		sudo chmod 777 SireMusic/bin/data/*
		sudo chmod 777 SireMusic/bin/scripts/*
		sudo mv .sire_copia/music_copia SireMusic/bin/music
		sudo mv .sire_copia/data_copia SireMusic/bin/data
		if [ -d ".sire_copia/discoteca_copia" ]; then
			sudo mv .sire_copia/discoteca_copia SireMusic/discoteca
		fi
		sudo rm -rf .sire_copia
	fi
else
	if [ -d "SireMusic" ]; then
		echo "\033[0;31mYa existe una carpeta llamada SireMusic!\033[0m"
	else
		sudo git clone https://github.com/ByAletalsan/SireMusic.git
		sudo rm -rf SireMusic/instalador.sh
		sudo mkdir -p SireMusic/new_songs
		sudo mkdir -p SireMusic/discoteca
		sudo mkdir -p SireMusic/bin/music/channel_0
		sudo mkdir -p SireMusic/bin/music/channel_1
		sudo mkdir -p  SireMusic/bin/music/channel_2
		sudo chmod 777 SireMusic
		sudo chmod 777 SireMusic/*
		sudo chmod 777 SireMusic/bin/*
		sudo chmod 777 SireMusic/bin/data/*
		sudo chmod 777 SireMusic/bin/scripts/*
		sudo chmod 777 SireMusic/bin/music/*
	fi
fi
cd SireMusic
sudo make
echo "\033[38;2;144;238;144mYa puedes ejecutar el programa principal!!\033[0m"
exit
