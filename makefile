all:
	# build embeded text files
	xxd -i ./source/compiler/standard/printing.dragon > ./build_temps/printing.c

	# compile executables
	gcc ./source/main.c -Wall -Wextra -fsanitize=address -g -o ./../dragon-debug.elf
	gcc ./source/main.c -o ./../dragon.elf
