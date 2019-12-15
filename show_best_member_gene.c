#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gnuplot_i.h"

#define SLEEP 15

void read_data(const char *file_name, double *data, int size);
void graph_member(gnuplot_ctrl *h, const double *gene, int gene_size, char *name);
int read_training_data_records(const char *file_name, double *dataX, double *dataY, int num);

int main ( int argc, char *argv[] )
{
    char *file_names, *training_data_file_name;
    int size = 9;
    int sleep_time = SLEEP;
    int num;

    if ( argc != 6 )
    {
        printf("usage: %s training_set_file  gene_file_name1,gene_file_name2,... gene_size sleeptime", argv[0]);
    }
    else 
    {
        training_data_file_name = argv[1];
        num = atoi(argv[2]);
        file_names = argv[3];
        size = atoi(argv[4]);
        sleep_time = atoi(argv[5]);
    }

   double data[size];
    gnuplot_ctrl *plot;
    plot = gnuplot_init();
    gnuplot_set_xlabel(plot, "X axis");
    gnuplot_set_ylabel(plot, "Y axis");
    gnuplot_setstyle(plot, "points");
 
    //draw training data set
    double dataX[num], dataY[num];
    read_training_data_records(training_data_file_name, dataX, dataY, num);
    gnuplot_plot_xy(plot, dataX, dataY, num, training_data_file_name);
 
    char *file_name = strtok(file_names, ",");
    while (file_name != NULL)
    {
        read_data(file_name, data, size);
        graph_member(plot, data, size, file_name);
        file_name = strtok(NULL, ",");
    }

    sleep(sleep_time);

    return 0;
}

void graph_member(gnuplot_ctrl *h, const double *gene, int gene_size, char *name)
{
    int i, j, k;
    k = 0;
    int div = 2;
    int size = 360/div;
    double x[3 * size], y[3 * size];
    for(i = 0; i < gene_size; i += 3)
    {
        for (j = 0; j < 360; j += div)
        {
            x[k] = gene[i] + gene[i+2] * cos(j);
            y[k] = gene[i+1] + gene[i+2] * sin(j);
            k++;
        }
    }
    gnuplot_plot_xy(h, x, y, size * 3, name);
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
        fclose(fd);
    }
}

int read_training_data_records(const char *file_name, double *dataX, double *dataY, int num)
{
    if(num == 0)
        return 0;

    FILE *fd = fopen (file_name, "r");
    if(fd != NULL)
    {
 
        fseek(fd, 0L, SEEK_SET);
        int i = 0;
        for(i = 0 ; i < num ; i++)
        {
            if(fscanf(fd, "%lf %lf", &dataX[i], &dataY[i]) != 2)
                break;
            /* error on fscanf! */
        }
        fclose(fd);
        return i;
    }
}
