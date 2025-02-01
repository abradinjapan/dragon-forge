all:
	# build embeded text files
	xxd -i ./source/compiler/standard/print.dragon > ./build_temps/print.c
	xxd -i ./source/compiler/standard/cast.dragon > ./build_temps/cast.c
	xxd -i ./source/compiler/standard/buffer.dragon > ./build_temps/buffer.c

	# compile executables
	gcc ./source/main.c -Wall -Wextra -fsanitize=address -g -o ./../dragon-debug.elf
	gcc ./source/main.c -o ./../dragon.elf
