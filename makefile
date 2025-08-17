all:
	# build embeded text files
	xxd -i ./source/dragon/standard/print.dragon > ./build_temps/dragon-print.c
	xxd -i ./source/dragon/standard/cast.dragon > ./build_temps/dragon-cast.c
	xxd -i ./source/dragon/standard/buffer.dragon > ./build_temps/dragon-buffer.c
	xxd -i ./source/dragon/standard/current.dragon > ./build_temps/dragon-current.c
	xxd -i ./source/dragon/standard/list.dragon > ./build_temps/dragon-list.c
	xxd -i ./source/dragon/standard/context.dragon > ./build_temps/dragon-context.c
	xxd -i ./source/dragon/standard/check.dragon > ./build_temps/dragon-check.c
	xxd -i ./source/dragon/standard/error.dragon > ./build_temps/dragon-error.c
	xxd -i ./source/dragon/standard/json.dragon > ./build_temps/dragon-json.c
	xxd -i ./source/dragon/standard/time.dragon > ./build_temps/dragon-time.c
	xxd -i ./source/dragon/standard/anvil.dragon > ./build_temps/dragon-anvil.c
	xxd -i ./source/dragon/standard/compile.dragon > ./build_temps/dragon-compile.c
	xxd -i ./source/dragon/standard/just_run.dragon > ./build_temps/dragon-just_run.c

	# compile executables
	gcc ./source/main.c -Wall -Wextra -fsanitize=address -g -o ./../dragon-forge-debug.elf
	gcc ./source/main.c -o ./../dragon-forge.elf
