#include "plot.h"

void init_fitness_plot(gnuplot_ctrl *fitness_plot)
{
    fitness_plot = gnuplot_init();
    gnuplot_set_xlabel(fitness_plot, "X axis");
    gnuplot_set_ylabel(fitness_plot, "Fitness");
    gnuplot_set_ylabel(fitness_plot, "Fitness");
    gnuplot_cmd(fitness_plot, "set log x");
    gnuplot_cmd(fitness_plot, "set yrange [ 250 : 305 ]");
}
