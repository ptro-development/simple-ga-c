#ifndef NORMAL_DISTRIBUTION_H
#define NORMAL_DISTRIBUTION_H

// normal distribution with peak at x = 0
struct NormalDistribution
{
    //peak point
    double max_y;
    double width;
};

void init_normal_distribution(struct NormalDistribution *nd, const double *max_y, const double *width);
void compute_normal_distribution_y(const struct NormalDistribution *nd, const double *x, double *delta);

#endif
