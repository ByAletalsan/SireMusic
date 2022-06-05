#!/bin/bash
# -*- ENCODING: UTF-8 -*-
echo "Instalando actualizaciones..."
sudo apt-get -y update
echo "Actualizaciones instaladas!"
echo "Instalando reproductor..."
sudo apt update && sudo apt install sox
sudo apt-get install libsox-fmt-all
echo "Reproductor instalado!"
echo "Instalando constructor del programa..."
sudo apt-get install build-essential
echo "Creando programa..."
sudo g++ -Wall -o SireMusic bin/main.cpp bin/itemCancion.cpp bin/reproductor.cpp
echo "Instalado constructor!"
echo "Ya puedes ejecutar el programa principal!!"
exit
