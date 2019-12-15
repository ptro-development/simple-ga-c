#include "gnuplot_i.h"
#include "member.h"

#include <stdlib.h>

void graph_member(gnuplot_ctrl *plot, const struct Member *member, int member_gene_size)
{
    int i, j, k;
    for(i = 0; i < member_gene_size; i += 3)
    {
        int div = 2;
        int size = 360/div;
        double x[size], y[size];
        k = 0;
        for (j = 0; j < 360; j += div)
        {
            x[k] = member->gene[i] + member->gene[i+2] * cos(j);
            y[k] = member->gene[i+1] + member->gene[i+2] * sin(j);
            k++;
        }
        gnuplot_setstyle(plot, "points");
        gnuplot_plot_xy(plot, x, y, size, "");
    }
}

void generate_circles(double *x, double *y, int circle_number, int size)
{
    int i, j, index, part;
    double r, c, d, in;
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
            }
        }
    }
}

void init_fitness_plot(gnuplot_ctrl *fitness_plot)
{
    fitness_plot = gnuplot_init();
    gnuplot_set_xlabel(fitness_plot, "X axis");
    gnuplot_set_ylabel(fitness_plot, "Fitness");
    gnuplot_set_ylabel(fitness_plot, "Fitness");
    gnuplot_cmd(fitness_plot, "set log x");
    gnuplot_cmd(fitness_plot, "set yrange [ 250 : 305 ]");
}
