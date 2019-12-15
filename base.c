#include "base.h"
#include <stdlib.h>
#include <float.h>

void init_population(struct Population *population,
                     const int size,
                     const int member_gene_size)
{
    struct Member *members;
    double *gene;
    int i, j;

    members = (struct Member*) malloc(sizeof(struct Member) * size);
    if (members != NULL)
    {
        population->members = members;
        population->size = size;
        population->member_gene_size = member_gene_size;

        for(i=0; i<size; i++)
        {
            population->members[i].fitness = 0;
            gene = (double*) malloc(sizeof(double) * member_gene_size);
            if (gene != NULL)
            {
                for(j=0; j<member_gene_size; j++)
                    gene[j] = (double)100*rand()/RAND_MAX;
                population->members[i].gene = gene;
            }
        }
    }
}

void population_fitnessXY(struct Population *population,
                        const double *dataX,
                        const double *dataY,
                        const int data_size,
                        double(*pt_fitnessXY)(
                            const struct Member *,
                            const int,
                            const double *,
                            const double *,
                            const int
                            ),
                        double *minFitness,
                        double *maxFitness,
                        int *maxFitnessIndex,
                        int *minFitnessIndex
                       )
{
    int i;
    double mp = 0;

    // set default values
    *minFitness = DBL_MAX;
    *maxFitness = 0;

    for(i = 0; i < population->size; i++)
    {
        mp = pt_fitnessXY(&population->members[i],
                        population->member_gene_size,
                        dataX,
                        dataY,
                        data_size);

        // find maxFitness index
        if(mp > *maxFitness)
            *maxFitnessIndex = i;

        //find minFitness index
        if(mp > *minFitness)
            *minFitnessIndex = i;

        *minFitness = fmin(*minFitness, mp);
        *maxFitness = fmax(*maxFitness, mp);
    }

    //printf("Min fittness is %f\n", *minFitness);
    //printf("Max fittness is %f\n", *maxFitness);
    //printf("Max fittness index %d\n", *maxFitnessIndex);
}

void population_fitness(struct Population *population,
                        const struct TrainingSet *data,
                        double(*pt_fitness)(
                            const struct Member *,
                            const int,
                            const struct TrainingSet *),
                        double *minFitness,
                        double *maxFitness,
                        int *maxFitnessIndex
                       )
{
    int i;
    double mp;

    // set default values
    *minFitness = DBL_MAX;
    *maxFitness = 0;

    for(i = 0; i < population->size; i++)
    {
        mp = pt_fitness(&population->members[i],
                        population->member_gene_size,
                        data);

        // find maxFitness index
        if(mp > *maxFitness)
            *maxFitnessIndex = i;

        *minFitness = fmin(*minFitness, mp);
        *maxFitness = fmax(*maxFitness, mp);
    }

    //printf("Min fittness is %f\n", *minFitness);
    //printf("Max fittness is %f\n", *maxFitness);
    //printf("Max fittness index %d\n", *maxFitnessIndex);
}

void init_evolution(struct Evolution *evolution,
                    const double cross_rate,
                    const double mutation_rate)
{
    evolution->cross_rate = cross_rate;
    evolution->mutation_rate = mutation_rate;
}
