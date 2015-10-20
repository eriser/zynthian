
all: lib test

lib:
	gcc -c -O3 -fPIC -Wall -Werror midi_rencoder.c -o midi_rencoder.o
	gcc -shared midi_rencoder.o -o midi_rencoder.so -L /usr/local/lib -lwiringPi -lasound -llo
	
test:
	#gcc -c -O3 -fPIC -Wall -Werror test_zyncoders.c -o test_zyncoders.o
	#ld -o test_zyncoders test_zyncoders.o midi_rencoder.o -lwiringPi -lasound -lc
	gcc -O3 -fPIC -Wall -Werror test_zyncoders.c -o test_zyncoders  -L /usr/local/lib -L . -l :midi_rencoder.so -lwiringPi -lasound -llo -lc
