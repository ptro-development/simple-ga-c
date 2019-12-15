#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gnuplot_i.h"

#define FILE_NAME "fitness"
#define SIZE 200
#define SLEEP 15

void read_data(const char *file_name, double *data, int size);

int main ( int argc, char *argv[] )
{
    char *file_names = FILE_NAME;
    int size = SIZE;
    int sleep_time = SLEEP;

    if ( argc != 4 )
    {
        printf( "usage: %s filename1,filename2,... datasize sleeptime", argv[0] );
    }
    else 
    {
        file_names = argv[1];
        size = atoi(argv[2]);
        sleep_time = atoi(argv[3]);
    }

   double data[size];
    gnuplot_ctrl *fitness_plot;
    fitness_plot = gnuplot_init();
    gnuplot_set_xlabel(fitness_plot, "X axis");
    gnuplot_set_ylabel(fitness_plot, "Fitness");
    gnuplot_cmd(fitness_plot, "set log x");
    gnuplot_cmd(fitness_plot, "set yrange [ 250 : 305 ]");
    gnuplot_setstyle(fitness_plot, "lines");

  
    char *file_name = strtok(file_names, ",");
    while (file_name != NULL)
    {
        read_data(file_name, data, size);
        gnuplot_plot_x(fitness_plot, data, size, file_name);
        file_name = strtok(NULL, ",");
    }

    sleep(sleep_time);

    return 0;
}

void read_data(const char *file_name, double *data, int size)
{
    FILE *fd = fopen (file_name, "r");
    if(fd != NULL)
    {
        int i;
        for(i = 0; i < size; i++)
        {
            fscanf(fd, "%lf", &data[i]);
        }
        close(fd);
    }
}
