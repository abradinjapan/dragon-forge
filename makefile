all:
	# build embeded text files
	xxd -i ./source/compiler/standard/print.dragon > ./build_temps/print.c
	xxd -i ./source/compiler/standard/cast.dragon > ./build_temps/cast.c
	xxd -i ./source/compiler/standard/buffer.dragon > ./build_temps/buffer.c
	xxd -i ./source/compiler/standard/current.dragon > ./build_temps/current.c
	xxd -i ./source/compiler/standard/list.dragon > ./build_temps/list.c
	xxd -i ./source/compiler/standard/context.dragon > ./build_temps/context.c
	xxd -i ./source/compiler/standard/check.dragon > ./build_temps/check.c
	xxd -i ./source/compiler/standard/error.dragon > ./build_temps/error.c
	xxd -i ./source/compiler/standard/json.dragon > ./build_temps/json.c
	xxd -i ./source/compiler/standard/time.dragon > ./build_temps/time.c
	xxd -i ./source/compiler/standard/compile.dragon > ./build_temps/compile.c

	# compile executables
	gcc ./source/main.c -Wall -Wextra -fsanitize=address -g -o ./../dragon-debug.elf
	gcc ./source/main.c -o ./../dragon.elf
