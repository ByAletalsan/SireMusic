#!/bin/bash
# -*- ENCODING: UTF-8 -*-

var=$(ls ../music/channel_0)

echo $var > ../data/songs_data.txt

lol=$(soxi -D ../music/channel_0/*)

echo $lol > ../data/time_data.txt

exit
