#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "population.h"

void init_population(struct Context *ctx, struct Population *population, const int size)
{
    struct Member *members;
    double *gene;
    int i, j;

    members = (struct Member*) malloc(sizeof(struct Member) * size);
    if (members != NULL)
    {
        population->members = members;
        population->size = size;

        for(i=0; i<size; i++)
        {
            population->members[i].fitness = 0;
            gene = (double*) malloc(sizeof(double) * ctx->gene_definition.size);
            if (gene != NULL)
            {
                for(j=0; j < ctx->gene_definition.size; j++)
                    gene[j] = (double)ctx->gene_definition.max_value * rand()/RAND_MAX;
                population->members[i].gene = gene;
                population->members[i].gene_definition = ctx->gene_definition;
            }
        }
        // compute population fitness
        population_fitnessXY(ctx, population, ctx->fitness_function_ptr);
    }
}

void population_linear_gene_transform(struct Context *ctx, struct Population *population, double factor, int down_or_up)
{
    int i, j;
    for (i = 0; i < population->size ; i++)
        member_linear_gene_transform(&population->members[i], factor, down_or_up);

    population_fitnessXY(ctx, population, ctx->fitness_function_ptr);
}

void population_fitnessXY(const struct Context *ctx,
                          struct Population *population,
                          double(*pt_fitnessXY)(struct Member *member, const struct TrainingData *training_data)
                         )
{
    int i;
    double mp = 0;

    // set default values
    population->minFitness = DBL_MAX;
    population->maxFitness = 0;

    for(i = 0; i < population->size; i++)
    {
        mp = pt_fitnessXY(&population->members[i], &ctx->training_data);

        //printf("mp:%f i:%d\n", mp, i);
        // find maxFitness index
        if(mp > population->maxFitness)
            population->maxFitnessIndex = i;

        //find minFitness index
        if(mp < population->minFitness)
            population->minFitnessIndex = i;

        population->minFitness = fmin(population->minFitness, mp);
        population->maxFitness = fmax(population->maxFitness, mp);
    }
    //printf("Min, Max fittness %f, %f\n", population->minFitness, population->maxFitness);
    //printf("Min, Max fittness index %d, %d\n", population->minFitnessIndex, population->maxFitnessIndex);
}

/*
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
*/
void copy_population(const struct Population *in, struct Population *out, const struct GeneDefinition *gene_definition)
{
    out->size = in->size;
    out->maxFitness = in->maxFitness;
    out->maxFitnessIndex = in->maxFitnessIndex;
    out->minFitness = in->minFitness;
    out->minFitnessIndex = in->minFitnessIndex;

    int i, j;
    for (i = 0; i < in->size ; i++)
    {
        for (j = 0; j < gene_definition->size ; j++)
        {
            out->members[i].gene[j] = in->members[i].gene[j];
        }
        out->members[i].fitness = in->members[i].fitness;
        out->members[i].gene_definition = in->members[i].gene_definition;
    }
}

void copy_best_member(const struct Context *ctx, const struct Population *in, struct Population *out)
{
    out->size = in->size;

    int i, j;
    for (i = 0; i < in->size ; i++)
    {
        for (j = 0; j < ctx->gene_definition.size ; j++)
        {
            out->members[i].gene[j] = in->members[in->maxFitnessIndex].gene[j];
        }
        out->members[i].fitness = in->members[in->maxFitnessIndex].fitness;
        out->members[i].gene_definition = in->members[in->maxFitnessIndex].gene_definition;
    }
    population_fitnessXY(ctx, out, ctx->fitness_function_ptr);
}

// elitism - making sure that the best member survives
void apply_elitism(struct Context *ctx, struct Population *in, struct Population *out)
{
    int i;
    for(i = 0; i < ctx->gene_definition.size; i++)
    {
        out->members[0].gene[i] = in->members[in->maxFitnessIndex].gene[i];
    }
    out->members[0].fitness = in->members[in->maxFitnessIndex].fitness;
    out->members[0].gene_definition = in->members[in->maxFitnessIndex].gene_definition;

    // compute new population fitness
    population_fitnessXY(ctx, out, ctx->fitness_function_ptr);
}

int pick_random_member(const struct Population *population, double *sum_fitness)
{
    double sum = 0;
    double fitness_point = (double)*sum_fitness * rand() / RAND_MAX;
    int i;
    for(i = 0; i < population->size; i++)
    {
        sum += population->members[i].fitness;
        if(sum > fitness_point)
            return i;
    }
    return 0;
}

void regenerate_population(struct Population *population, double percentage, const struct GeneDefinition *gene_definition)
{
    int size = (int)population->size * percentage;
    int o, k, flag, indexi;
    int new_indexes[size];
    double index;

    // prepare array of indexes for regeneration
    for(o = 0; o < size; o++)
    {
        index = (double)population->size*rand()/RAND_MAX;
        indexi = (int)index;

        // do not replace 0 element
        if(indexi != 0)
        {
            flag = 0;
            // is index already in array
            for(k = 0; k < size; k++)
            {
                if(new_indexes[k] == indexi)
                    flag = 1;
            }
            if(flag == 0)
                new_indexes[o] = indexi;
            else
                o--;
        }
        else
        {
            o--;
        }
    }

    // regenerate
    //printf("size %d\n", size);
    for (o = 0; o < size; o++)
    {
        //printf("%d ", new_indexes[o]);
        for(k = 0; k < gene_definition->size; k++)
        {
            population->members[new_indexes[o]].gene[k] = (double)100*rand()/RAND_MAX;
            //printf("gene %d %d %f ", o, k, population->members[new_indexes[o]].gene[k]);
        }
        //printf("\n");
    }
}

// modify fitness values for better selection - fitness is going to be <0 to bestFitness-minFitness>
void lower_fitness_by_min_fitness(struct Population *population, double *sum_fitness)
{
    int i;
    for(i = 0; i < population->size; i++)
    {
        population->members[i].fitness -= population->minFitness;
        *sum_fitness += population->members[i].fitness;
    }
}

void generate_population(struct Context *ctx,
                         struct Population *population,
                         struct Population *new_population,
                         double genetic_diversity_limit
                        )
{
    init_population(ctx, new_population, population->size);
    //print_population(new_population, &ctx->gene_definition);
    /* tournament_strategy
    int tg_size = (int)population->size / 0.5;
    int o, index;
    int tournament_group [tg_size];
    for(o=0;o<tg_size;o++)
    {
        index = (int)tg_size * rand() / RAND_MAX;
        tournament_group[o] = index;
    }
    */
    double sum_fitness = 0;
    lower_fitness_by_min_fitness(population, &sum_fitness);

    int member_index_1, member_index_2, i;
    for(i = 0; i < population->size; i +=2)
    {
        /* tournament strategy of picking members
        member_index_1 = tournament_group[(int)tg_size*rand()/RAND_MAX];
        member_index_2 = tournament_group[(int)tg_size*rand()/RAND_MAX];
        */

        // pick two random members
        member_index_1 = pick_random_member(population, &sum_fitness);
        member_index_2 = pick_random_member(population, &sum_fitness);

        // do natural selection
        if((double)rand()/RAND_MAX < ctx->selection.cross_rate)
        {
            // test genetic diversity
            if(population->members[member_index_1].fitness - population->members[member_index_2].fitness > genetic_diversity_limit)
            {
                // create two new members
                generate_members_gene_copy_exchange(
                    &population->members[member_index_1], &population->members[member_index_2],
                    &new_population->members[i], &new_population->members[i+1]);

                // randomly mutate the two new members
                mutate(&ctx->mutation, &ctx->line, &new_population->members[i]);
                mutate(&ctx->mutation, &ctx->line, &new_population->members[i+1]);
                /*
                print_member(&new_population->members[member_index_1]);
                print_member(&new_population->members[member_index_2]);
                print_member(&population->members[member_index_1]);
                print_member(&population->members[member_index_2]);
                */
            }
            else
            {
                // there is not a good genetic diversity do something with it
                //printf("Random offspring generation method, generation\n");
                generate_members_gene_copy_exchange_random(
                    &population->members[member_index_1], &population->members[member_index_2],
                    &new_population->members[i], &new_population->members[i+1]);
                /*
                print_member(&population->members[member_index_1]);
                print_member(&population->members[member_index_2]);
                print_member(&new_population->members[member_index_1]);
                print_member(&new_population->members[member_index_2]);
                */
            }
            // use taboo search to test already used genes, mutate if match
            mutate_if_taboo(&ctx->mutation, &ctx->line, &new_population->members[i], &ctx->taboo_fifo, 10);
            mutate_if_taboo(&ctx->mutation, &ctx->line, &new_population->members[i+1], &ctx->taboo_fifo, 10);
        }

        // no natural selection
        else
        {
            generate_members_gene_copy(&population->members[member_index_1], &population->members[member_index_2],
                                       &new_population->members[i], &new_population->members[i+1]);
        }
    }
    // compute new populations fitness
    population_fitnessXY(ctx, population, ctx->fitness_function_ptr);
    population_fitnessXY(ctx, new_population, ctx->fitness_function_ptr);
}

void generate_population_2(struct Context *ctx,
                           struct Population *population,
                           struct Population *new_population,
                           int mutate_gene_size,
                           double genetic_diversity_limit
                          )
{
    // init new generation
    copy_population(population, new_population, &ctx->gene_definition);

    // modify fitness values for better selection
    double sum_fitness = 0;
    lower_fitness_by_min_fitness(new_population, &sum_fitness);

    // get random index and shift gene_index if necessary
    int gene_index = (int)((double)(ctx->gene_definition.size - 1) * rand() / RAND_MAX);
    if(ctx->gene_definition.size - gene_index < mutate_gene_size)
        gene_index = ctx->gene_definition.size - mutate_gene_size;

    int i;
    for (i = 0; i < new_population->size ; i++)
    {
        if((double)rand()/RAND_MAX < 0.5)
            mutate_gene_group(&ctx->mutation, &ctx->line, &new_population->members[i], mutate_gene_size, gene_index);
        else
            mutate_gene_group_random_index(&ctx->mutation, &ctx->line, &new_population->members[i], mutate_gene_size);
    }
    //print_member(&new_population->members[*maxFitnessIndex], population->gene_definition->size);
    population_fitnessXY(ctx, new_population, ctx->fitness_function_ptr);
    int member_index_1, member_index_2;
    for(i = 0; i < population->size; i +=2 )
    {
        // pick two random members
        member_index_1 = pick_random_member(population, &sum_fitness);
        member_index_2 = pick_random_member(population, &sum_fitness);

        // test genetic diversity
        if(population->members[member_index_1].fitness - population->members[member_index_2].fitness > genetic_diversity_limit)
        {
            // create two new members
            //printf("Offspring generation method\n");
            generate_members_gene_copy_exchange(
                &population->members[member_index_1], &population->members[member_index_2],
                &new_population->members[i], &new_population->members[i+1]);
        }
        else
        {
            // there is not a good genetic diversity do something with it
            //printf("Random offspring generation method\n");
            generate_members_gene_copy_exchange_random(
                &population->members[member_index_1], &population->members[member_index_2],
                &new_population->members[i], &new_population->members[i+1]);
        }
    }
    // compute new populations fitness
    population_fitnessXY(ctx, population, ctx->fitness_function_ptr);
    population_fitnessXY(ctx, new_population, ctx->fitness_function_ptr);
}

void generate_population_3(
    struct Context *ctx,
    struct Population *population,
    struct Population *new_population
)
{
    copy_best_member(ctx, population, new_population);
    int gene_index = (int)((double)(ctx->gene_definition.size - 1) * rand() / RAND_MAX);
    int value = (int)new_population->members[0].gene[gene_index];

    int i;
    double deltas[(int)ctx->gene_definition.max_value];
    for(i = 0; i < ctx->gene_definition.max_value; i++)
    {
        if(i < value)
            deltas[i] = -1 * i;
        else
            deltas[i] = i - value;
    }

    for(i = 0; i < ctx->gene_definition.max_value; i++)
    {
        new_population->members[i].gene[gene_index] += deltas[i];
        limit_gene_mutation(&ctx->gene_definition, &new_population->members[i].gene[gene_index]);
        //printf("i %d gene_index %d delta %f value %f \n", i, gene_index, deltas[i], new_population->members[i].gene[gene_index]);
    }
    population_fitnessXY(ctx, new_population, ctx->fitness_function_ptr);
    //print_population(new_population, &ctx->gene_definition);
    //sleep(50);
}

void generate_population_4(struct Context *ctx,
                           struct Population *population,
                           struct Population *new_population,
                           int mutate_genes_number,
                           double genetic_diversity_limit
                          )
{
    //printf("mutate_genes_number %d\n", mutate_genes_number);
    copy_best_member(ctx, population, new_population);
    int i, j, found;
    int *gene_indexes = (int*) malloc(sizeof(int) * mutate_genes_number);
    if (gene_indexes != NULL)
    {
        j = 0;
        while(j != mutate_genes_number)
        {
            int gene_index = (int)((double)(ctx->gene_definition.size - 1) * rand() / RAND_MAX);
            found = 0;
            for(i = 0; i < mutate_genes_number; i++)
            {
                if(gene_indexes[i] == gene_index)
                    found = 1;
            }
            if(found == 0)
            {
                gene_indexes[j] = gene_index;
                j++;
            }
        }
    }
    else
        return;

    for (i = 0; i < new_population->size ; i++)
    {
        for(j = 0; j < mutate_genes_number; j++)
            mutate_gene_group(&ctx->mutation, &ctx->line, &new_population->members[i], 1, gene_indexes[j]);
    }
    population_fitnessXY(ctx, new_population, ctx->fitness_function_ptr);
}

void print_population(const struct Population *population, const struct GeneDefinition *gene_definition)
{
    printf("pupulation maxFitness:%f minFitness:%f maxFitnessIndex:%d minFitnessIndex:%d\n", population->maxFitness, population->minFitness, population->maxFitnessIndex, population->minFitnessIndex);
    int j, i;
    for(i = 0; i < population->size; i++)
    {
        printf("member num: %d fitness: %f\n", i, population->members[i].fitness);
        for(j = 0; j < gene_definition->size; j++)
        {
            printf("%f ", population->members[i].gene[j]);
        }
        printf("\n");
    }
}
