#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "utils.h"

double get_random_number(double interval)
{
    return ((double)interval * rand()/RAND_MAX);
}

void get_delta_line_normal_distribution(const struct Mutation *mutation, const struct Line *line, const struct GeneDefinition *gene_definition, double *delta)
{
    double line_y = compute_line_y(line, &mutation->actual);
    double x = get_random_number(gene_definition->max_value);

    double width = 50;
    struct NormalDistribution nd;
    init_normal_distribution(&nd, &line_y, &width);
    compute_normal_distribution_y(&nd, &x, delta);

    // up or down ?
    if ((double)rand()/RAND_MAX < 0.5)
        *delta = -1 * *delta;
    //printf("line_y %f x %f actual mutation %f delta %f\n", line_y, x, mutation->actual, *delta);
}

void generate_circles(double *x, double *y, int circle_number, int size)
{
    int i, j, index, part;
    double r, c, d, in;
    if (size % circle_number > 0)
    {
        fprintf(stderr, "size %% circle_number in bigger than 0\n");
        exit(-1);
    }
    part = size / circle_number;
    for(i = 0; i < circle_number; i++ )
    {
        r = (double)100 * rand()/RAND_MAX;
        c = (double)100 * rand()/RAND_MAX;
        d = (double)100 * rand()/RAND_MAX;

        for(j = 0; j < part; j++)
        {
            index = i * part + j;
            if (index < size)
            {
                in = (double)360 * rand()/RAND_MAX;
                x[index] = c + r * cos(in);
                y[index] = d + r * sin(in);
                //printf("index:%d i:%d j:%d x:%f y:%f\n",index,i,j,x[index],y[index]);
            }
        }
    }
}

void init_fitness_plot(gnuplot_ctrl *plot)
{
    gnuplot_setstyle(plot, "lines");
    gnuplot_set_xlabel(plot, "X axis");
    gnuplot_set_ylabel(plot, "Fitness");
    gnuplot_cmd(plot, "set log x");
    gnuplot_cmd(plot, "set yrange [ 250 : 305 ]");
}

void plot_fitness_data(gnuplot_ctrl *plot, double *data, int size)
{
    gnuplot_plot_x(plot, data, size, "");
}

void init_training_data_plot(gnuplot_ctrl *plot)
{
    gnuplot_setstyle(plot, "points");
    gnuplot_set_xlabel(plot, "X axis");
    gnuplot_set_ylabel(plot, "TrainingData");
}

void plot_training_data(gnuplot_ctrl *plot, struct TrainingData *training_data)
{
    gnuplot_plot_xy(plot, training_data->dataX, training_data->dataY, training_data->size, "");
}

void init_array_with_value(double *array, double value, int size)
{
    int i;
    for (i = 0; i < size; i++)
        array[i] = value;
}
