#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "context.h"
#include "member.h"
#include "population.h"
#include "utils.h"
#include "record.h"
#include "transform_switch.h"

#define GENETIC_DIVERSITY_LIMIT 0.2

#define POPULATION_SIZE 200
#define POPULATION_REGENERATION_PERCENTAGE 0.4

#define MAX_FITNESS_VALUE 300

#define FITNESS_STAGNATION_LIMIT 0.001
#define STAGNATION_COUNTER_LIMIT_MAX 10
#define STAGNATION_COUNTER_LIMIT_MIN 1

#define MUTATE_GENE_LEN_SIZE_MIN 1
#define MUTATE_GENE_LEN_SIZE_MAX 3
#define MUTATE_GENES_MAX 3
#define MUTATE_GENES_MIN 1

#define GENERATIONS_NUM 10000
#define PLOT_EVERY_GENERATION 100

#define POPULATION_FILE_NAME "population"
#define SAVE_POPULATION 0
#define READ_POPULATION 1

#define CIRCLE_FILE_NAME "dataset"
#define READ_CIRCLES 1
#define SAVE_CIRCLES 0

#define GENERATIONS_FITNESS_FILE_NAME "fitness"
#define SAVE_FITNESS 1

#define BEST_MEMBER_GENE_FILE_NAME "best-memeber-gene"
#define SAVE_BEST_MEMBER 1

int main()
{
    //init random generator
    srand(time(NULL));

    // initialize context
    struct Context ctx;
    init_context(&ctx);

    // initialize population
    struct Population population;
    init_population(&ctx, &population, POPULATION_SIZE);
    //print_population(&population, &ctx.gene_definition);

    apply_elitism(&ctx, &population, &population);
    //print_population(&population, &ctx.gene_definition);

    // store the best fitness in population
    double genFitness[GENERATIONS_NUM];
    genFitness[0] = population.members[0].fitness;

    // save initial population if needed
    save_population(&population, &ctx.gene_definition, POPULATION_FILE_NAME, SAVE_POPULATION);
    // read initial population if needed
    read_population(&population, &ctx.gene_definition, POPULATION_FILE_NAME, READ_POPULATION);

    // save initial training data if needed
    save_training_data(&ctx.training_data, CIRCLE_FILE_NAME, SAVE_CIRCLES);
    // read initial training data if nedeed
    read_training_data(&ctx.training_data, CIRCLE_FILE_NAME, READ_CIRCLES);

    // plot training data
    plot_training_data(ctx.training_data_plot, &ctx.training_data);

    // init max fitness array with value
    double max_fitness_array[GENERATIONS_NUM];
    init_array_with_value(max_fitness_array, MAX_FITNESS_VALUE, GENERATIONS_NUM);

    // init new population
    struct Population new_population;
    new_population.size = POPULATION_SIZE;

    int stagnation_counter = 0;
    int generation_type = 0;
    int iGeneration;
    int stagnation_counter_limit = STAGNATION_COUNTER_LIMIT_MAX;
    int mutate_gene_size = MUTATE_GENE_LEN_SIZE_MIN;
    int mutate_genes = MUTATE_GENES_MIN;

    /*
    The approach for this generation is to use transformation to scale things
    down, so there is less space to search through.
    */

    double transform_factor = 1;

    // transform context
    struct Context tr_ctx;
    init_context(&tr_ctx);
    copy_context(&ctx, &tr_ctx);
    context_linear_transform(&tr_ctx, transform_factor, 0);

    // transform initial population
    struct Population tr_population, tr_new_population;
    init_population(&tr_ctx, &tr_population, POPULATION_SIZE);
    init_population(&tr_ctx, &tr_new_population, POPULATION_SIZE);
    copy_population(&population, &tr_population, &ctx.gene_definition);
    population_linear_gene_transform(&tr_ctx, &tr_population, transform_factor, 0);

    // init trasfrom switch
    struct TransformSwitch tr_switch;
    init_transform_switch(&tr_switch, &tr_ctx, &ctx, &tr_population, &population, &tr_new_population, &new_population);

    for(iGeneration = 1; iGeneration < GENERATIONS_NUM; iGeneration++)
    {
        if(generation_type == 0)
        {
            //printf("not focused generation %d mutation %f\n", iGeneration, ctx.mutation.actual);

            generate_population(tr_switch.actual_context, tr_switch.actual_population,
                                tr_switch.actual_new_population, GENETIC_DIVERSITY_LIMIT);
        }

        if (generation_type == 1)
        {
            //printf("focused generation %d stagnation_counter %d mutate_gene_size %d mutation %f\n", iGeneration, stagnation_counter, mutate_gene_size, ctx.mutation.actual);
            generate_population_2(tr_switch.actual_context, tr_switch.actual_population, tr_switch.actual_new_population,
                                  mutate_gene_size, GENETIC_DIVERSITY_LIMIT);

            if(iGeneration > 1 && genFitness[iGeneration-1] - genFitness[iGeneration-2] == 0)
            {
                if(mutate_gene_size != MUTATE_GENE_LEN_SIZE_MAX)
                    mutate_gene_size += 1;
                else
                    mutate_gene_size = MUTATE_GENE_LEN_SIZE_MIN;
            }
        }

        if (generation_type == 2)
        {
            //printf("focused generation %d stagnation_counter %d\n", iGeneration, stagnation_counter);
            //printf("deterministic focused generation %d stagnation_counter %d mutate_gene_size %d mutation %f\n", iGeneration, stagnation_counter, mutate_gene_size, ctx.mutation.actual);
            generate_population_3(tr_switch.actual_context, tr_switch.actual_population, tr_switch.actual_new_population);
        }

        if (generation_type == 3)
        {
            generate_population_4(tr_switch.actual_context, tr_switch.actual_population, tr_switch.actual_new_population, mutate_genes, GENETIC_DIVERSITY_LIMIT);
            if(iGeneration > 1 && genFitness[iGeneration-1] - genFitness[iGeneration-2] == 0)
            {
                if(mutate_genes != MUTATE_GENES_MAX)
                    mutate_genes += 1;
                else
                    mutate_genes = MUTATE_GENES_MIN;
            }
        }

        // elitism - making sure that the best member survives
        if(genFitness[iGeneration-1] < tr_switch.actual_new_population->members[tr_switch.actual_new_population->maxFitnessIndex].fitness)
            apply_elitism(tr_switch.actual_context, tr_switch.actual_new_population, tr_switch.actual_new_population);
        else
            apply_elitism(tr_switch.actual_context, tr_switch.actual_population, tr_switch.actual_new_population);

        // put into taboo_fifo if not there
        if(is_in_fifo(&tr_switch.actual_context->taboo_fifo, tr_switch.actual_new_population->members[0].gene) != NULL)
            put_in_fifo(&tr_switch.actual_context->taboo_fifo, tr_switch.actual_new_population->members[0].gene);

        // write generation fitness
        genFitness[iGeneration] = tr_switch.actual_new_population->members[0].fitness;

        // replace population by new population
        copy_population(tr_switch.actual_new_population, tr_switch.actual_population, &tr_switch.actual_context->gene_definition);

        // graph results
        if(iGeneration % PLOT_EVERY_GENERATION == 0)
        {
            //printf("Generation: %d best_fitness %3.10f mutation_rate %f\n", iGeneration, population.members[0].fitness, ctx.mutation.actual);
            printf("Generation: %d best_fitness %3.10f\n", iGeneration, tr_switch.actual_population->members[0].fitness);

            gnuplot_resetplot(tr_switch.actual_context->training_data_plot);
            plot_training_data(tr_switch.actual_context->training_data_plot, &tr_switch.actual_context->training_data);
            graph_member(tr_switch.actual_context->training_data_plot, &tr_switch.actual_population->members[0]);
            sleep(1);

            gnuplot_resetplot(tr_switch.actual_context->fitness_plot);
            plot_fitness_data(tr_switch.actual_context->fitness_plot, genFitness, iGeneration);
            plot_fitness_data(tr_switch.actual_context->fitness_plot, max_fitness_array, GENERATIONS_NUM);
            sleep(1);
        }

        // adjust mutation rate if there is not any progres
        if(genFitness[iGeneration] - genFitness[iGeneration-1] == 0)
        {
            //printf("adjust mutation %f\n", ctx.mutation.actual);
            adjust_mutation(&tr_switch.actual_context->mutation);
        }
        else
        {
            //printf("generation %d fitness delta %f generation_type %d\n", iGeneration, genFitness[iGeneration] - genFitness[iGeneration-1], generation_type);
        }

        // adjust stagnation counter
        //generation_type = 0; continue;
        int circle_router = 2;
        if(genFitness[iGeneration] - genFitness[iGeneration-1] < FITNESS_STAGNATION_LIMIT)
        {
            stagnation_counter += 1;
            // activate generation type
            if(stagnation_counter > stagnation_counter_limit)
            {
                if(genFitness[iGeneration] - genFitness[iGeneration - stagnation_counter_limit] != 0)
                    printf("generation_type %d generation_delta %f\n", generation_type, genFitness[iGeneration] - genFitness[iGeneration - stagnation_counter_limit]);
                stagnation_counter = 0;

                if (generation_type != circle_router)
                    generation_type += 1;
                else
                    generation_type = 0;
            }
        }
    }

// save generations' fitness data if needed
    save_fitness_data(genFitness, GENERATIONS_NUM, GENERATIONS_FITNESS_FILE_NAME, SAVE_FITNESS);

    // save best member
    save_member_data(&tr_switch.actual_population->members[0], BEST_MEMBER_GENE_FILE_NAME, SAVE_BEST_MEMBER);

    return 0;
}
