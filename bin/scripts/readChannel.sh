#!/bin/bash
# -*- ENCODING: UTF-8 -*-

var=$(ls bin/music/channel_0)

echo $var > bin/data/songs_data.txt


c=$(ls bin/music/channel_0| wc -l)

if [ "$c" = "0" ]

then
	lol=""
else

	lol=$(soxi -D bin/music/channel_0/*)

fi

	echo $lol > bin/data/time_data.txt

exit
