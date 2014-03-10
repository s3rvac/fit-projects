#!/bin/sh
#
# Encoding: UTF-8
# File:     test.sh
# Project:  Client/server file transfer
# Date:     1.3.2007
# Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Testing script for client/server file transfer

#
# Compile the program
#
make && {

#
# Run some simple tests
#
echo "----------------------------------------"
echo " Client/server file transfer test script"
echo "----------------------------------------"

server_pid=-1
port=0
i=40043
echo ">> Starting server"
cd ./_server
while [ $i -lt 60000 ] && [ $port -eq 0 ]
do
	./server $i &
	server_pid=`ps aux | grep "./server $i" | grep -v "grep" | awk '{ print $2 }' | xargs`
	if [ "$server_pid" != "" ]; then
		port=$i
	fi
	i=$(($i + 100))
done
if [ $port -eq 0 ]; then
	echo ">> Error: Cannot start server, no free ports"
	exit 1
fi

echo ">> Server is listening on port $port"

cd ../_client
echo ">> Get file list from the server"
./client -h localhost -p $port -l . && echo "OK"

echo ">> Upload file logo.gif to the server"
./client -h localhost -p $port -u logo.gif && echo "OK"

echo ">> Download file words.cc from the server"
./client -h localhost -p $port -d words.cc && echo "OK"

echo ">> Download file mmedia.pdf from the server"
./client -h localhost -p $port -d mmedia.pdf && echo "OK"

echo ">> Delete file todelete.txt from the server"
./client -h localhost -p $port -e todelete.txt && echo "OK"
cd ..

echo ">> Shutting down the server"
if [ "$server_pid" != "" ]; then
	kill $server_pid
fi

# Restore original state for next run
echo "----------------------------------------"
echo "Completed... Restoring original state"
echo "----------------------------------------"
echo "Delete me ;)" > ./_server/todelete.txt
rm -f ./_server/logo.gif
rm -f ./_client/mmedia.pdf
rm -f ./_client/words.cc
}
