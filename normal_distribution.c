#include "normal_distribution.h"

void init_normal_distribution(struct NormalDistribution *nd, const double *max_y, const double *width)
{
    nd->max_y = *max_y;
    nd->width = *width;
}

void compute_normal_distribution_y(const struct NormalDistribution *nd, const double *x, double *delta)
{
    *delta = nd->max_y * exp( - (*x * *x) / nd->width);
    //printf("max_y %f data x %f with %f y %f\n", nd->max_y, *x, nd->width, *delta);
}

