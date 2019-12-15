#include <math.h>
#include <stdlib.h>

#include "member.h"

/*
double fitness(
    struct Member *member,
    const int member_gene_size,
    const struct TrainingSet *data
)
{
    double quality = 0.0;
    double sigma = 15.0;
    double dx, dy, ds, min;

    int i, j;
    for(i = 0; i < (data->size * data->dimensions); i += 2)
    {
        for(j = 0; j < member_gene_size; j += 3)
        {
            dx = data->data[i] - member->gene[j];
            dy = data->data[i+1] - member->gene[j+1];
            ds = fabs(sqrt(dx*dx + dy*dy) - member->gene[j+2]);
            if (j == 0)
                min = ds;
            else
                min = fmin(min,ds);
        }
        // normal distribution
        quality += exp(-(min*min)/(2.0*sigma*sigma));
    }
    member->fitness = quality;
    return quality;
}
*/
void member_linear_gene_transform(struct Member *member, double factor, int down_or_up)
{
    int j;
    for(j = 0; j < member->gene_definition.size; j++)
    {
        if (down_or_up == 0)
            member->gene[j] /= factor;
        else
            member->gene[j] *= factor;
    }
    if (down_or_up == 0)
        member->gene_definition.max_value /= factor;
    else
        member->gene_definition.max_value *= factor;
}

double fitnessXY(struct Member *member, struct TrainingData *training_data)
{
    double quality = 0.0;
    double sigma = 3.5; // small sigma causes very sharp function, so even small change can create big change
    double dx, dy, ds, min;

    int i, j;
    for(i = 0; i < training_data->size; i++)
    {
        for(j = 0; j < member->gene_definition.size; j += 3)
        {
            dx = training_data->dataX[i] - member->gene[j];
            dy = training_data->dataY[i] - member->gene[j+1];
            ds = fabs(sqrt(dx*dx + dy*dy) - member->gene[j+2]);
            if (j == 0)
                min = ds;
            else
                min = fmin(min,ds);
        }
        // normal distribution
        quality += exp(-(min*min)/(2*sigma*sigma));
    }
    member->fitness = quality;
    return quality;
}

double fitnessXYZ(struct Member *member, struct TrainingData *training_data)
{
    double quality = 0.0;
    double sigma = 10;
    double dx, dy, ds, min, ds_centre;

    int i, j, k;
    int ds_centres[3];

    for(i = 0; i < training_data->size; i++)
    {
        for(j = 0; j < member->gene_definition.size; j += 3)
        {
            dx = training_data->dataX[i] - member->gene[j];
            dy = training_data->dataY[i] - member->gene[j+1];
            ds = fabs(sqrt(dx*dx + dy*dy) - member->gene[j+2]);
            //ds_centres[j/3] = ds;

            if (j == 0)
                min = ds;
            else
                min = fmin(min,ds);
        }
        // normal distribution
        quality += exp(-(min*min)/(2*sigma*sigma));
        //for(k = 0; k < 3; k++)
        //    quality += exp(-(ds_centres[k]*ds_centres[k])/(2*sigma*sigma));

    }
    member->fitness = quality;
    return quality;
}

void print_member(const struct Member *member)
{
    int j;
    printf("member fitness: %f\n", member->fitness);
    for(j = 0; j < member->gene_definition.size; j++)
    {
        printf("%f ", member->gene[j]);
    }
    printf("\n");
}

void graph_member(gnuplot_ctrl *plot, const struct Member *member)
{
    int i, j, k;
    for(i = 0; i < member->gene_definition.size; i += 3)
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

// create two new members by gene copy & exchange
void generate_members_gene_copy_exchange(const struct Member *in_1, const struct Member *in_2, struct Member *out_1, struct Member *out_2)
{
    int i;
    for(i = 0; i < in_1->gene_definition.size; i++)
    {
        if((double)rand()/RAND_MAX < 0.5)
        {
            // gene copy
            out_1->gene[i] = in_1->gene[i];
            out_2->gene[i] = in_2->gene[i];
        }
        else
        {
            // gene exchange
            out_1->gene[i] = in_2->gene[i];
            out_2->gene[i] = in_1->gene[i];
        }
    }
}

// create one member by gene copy & exchange and second with random value
void generate_members_gene_copy_exchange_random(const struct Member *in_1, const struct Member *in_2, struct Member *out_1, struct Member *out_2)
{
    int i;
    for(i = 0; i < in_1->gene_definition.size; i++)
    {
        if((double)rand()/RAND_MAX < 0.5)
        {
            // gene copy
            out_1->gene[i] = in_1->gene[i];
        }
        else
        {
            // gene exchange
            out_1->gene[i] = in_2->gene[i];
        }
        out_2->gene[i] = (double)in_1->gene_definition.max_value * rand()/RAND_MAX;
    }
}

// create members by gene copy
void generate_members_gene_copy(const struct Member *in_1, const struct Member *in_2, struct Member *out_1, struct Member *out_2)
{
    int i;
    for(i = 0; i < in_1->gene_definition.size; i++)
    {
        out_1->gene[i] = in_1->gene[i];
        out_2->gene[i] = in_2->gene[i];
    }
}
