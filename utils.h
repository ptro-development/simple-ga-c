#ifndef UTILS_H
#define UTILS_H

#include "gnuplot_i.h"

#include "mutation.h"
#include "line.h"
#include "gene_definition.h"
#include "normal_distribution.h"
#include "training_data.h"

void get_delta_line_normal_distribution(const struct Mutation *mutation, const struct Line *line, const struct GeneDefinition *gene_definition, double *delta);
double get_random_number(double interval);
void generate_circles(double *x, double *y, int circle_number, int size);
void init_fitness_plot(gnuplot_ctrl *plot);
void plot_fitness_data(gnuplot_ctrl *plot, double *data, int size);
void init_training_data_plot(gnuplot_ctrl *plot);
void plot_training_data(gnuplot_ctrl *plot, struct TrainingData *training_data);
void init_array_with_value(double *array, double value, int size);

#endif
