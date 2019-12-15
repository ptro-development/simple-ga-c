CXX = gcc -I/home/snumrik/workspace-c/gnuplot_i/src/ -L /home/snumrik/workspace-c/gnuplot_i/
CC = gcc -I/home/snumrik/workspace-c/gnuplot_i/src/ -L /home/snumrik/workspace-c/gnuplot_i/

all:            show_fitness

show_fitness:        show_fitness.c /home/snumrik/workspace-c/gnuplot_i/gnuplot_i.o

clean clear:            
	rm -f show_fitness.o show_fitness

