assembler: main.o first_run.o second_run.o list.o label.o pre_asm.o write_files.o func.o errors.o	
	gcc -ansi -Wall -pedantic -g main.o first_run.o second_run.o list.o func.o errors.o label.o write_files.o pre_asm.o -o assembler 

main.o: main.c first_run.h second_run.h errors.h list.h pre_asm.h func.h label.h
	gcc -c -ansi -Wall -pedantic -g main.c -o main.o

first_run.o: first_run.c first_run.h second_run.h func.h label.h list.h errors.h
	gcc -c -ansi -Wall -pedantic -g first_run.c -o first_run.o

second_run.o: second_run.c second_run.h func.h label.h list.h write_files.h errors.h
	gcc -c -ansi -Wall -pedantic -g second_run.c -o second_run.o

list.o: list.c list.h errors.h 
	gcc -c -ansi -Wall -pedantic -g list.c -o list.o


label.o: label.c label.h errors.h
	gcc -c -ansi -Wall -pedantic -g label.c -o label.o


pre_asm.o: pre_asm.c pre_asm.h list.h errors.h func.h
	gcc -c -ansi -Wall -pedantic -g pre_asm.c -o pre_asm.o

write_files.o: write_files.c write_files.h label.h errors.h list.h func.h
	gcc -c -ansi -Wall -pedantic -g write_files.c -o write_files.o


func.o: func.c func.h label.h list.h errors.h 
	gcc -c -ansi -Wall -pedantic -g func.c -o func.o


errors.o: errors.c errors.h 
	gcc -c -ansi -Wall -pedantic -g errors.c -o errors.o


