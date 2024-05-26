debug:
	gcc ./source/main.c -Wall -Wextra -fsanitize=address -g -o ./../forge.elf

release:
	gcc ./source/main.c -Wall -Wextra -o ./../forge.elf
