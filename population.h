#ifndef POPULATION_H
#define POPULATION_H

#include "member.h"
#include "context.h"
#include "training_data.h"
#include "gene_definition.h"
#include "mutation.h"

extern struct Population
{
    struct Member *members;
    int size;
    double minFitness;
    double maxFitness;
    int maxFitnessIndex;
    int minFitnessIndex;
};

void init_population(struct Context *ctx, struct Population *population, const int size);
void population_linear_gene_transform(struct Context *ctx, struct Population *population, double factor, int down_or_up);
void apply_elitism(struct Context *ctx, struct Population *in, struct Population *out);
void copy_best_member(const struct Context *ctx, const struct Population *in, struct Population *out);
void copy_population(const struct Population *in, struct Population *out, const struct GeneDefinition *gene_definition);
int pick_random_member(const struct Population *population, double *sum_fitness);
void regenerate_population(struct Population *population, double percentage, const struct GeneDefinition *gene_definition);
void print_population(const struct Population *population, const struct GeneDefinition *gene_definition);
void lower_fitness_by_min_fitness(struct Population *population, double *sum_fitness);

void population_fitnessXY(const struct Context *ctx,
                          struct Population *population,
                          double(*pt_fitnessXY)(struct Member *member, const struct TrainingData *training_data)
                         );

void generate_population(struct Context *ctx,
                         struct Population *population,
                         struct Population *new_population,
                         double genetic_diversity_limit
                        );

void generate_population_2(struct Context *ctx,
                           struct Population *population,
                           struct Population *new_population,
                           int mutate_gene_size,
                           double genetic_diversity_limit
                          );

void generate_population_3(struct Context *ctx,
                          struct Population *population,
                          struct Population *new_population
                         );

void generate_population_4(struct Context *ctx,
                           struct Population *population,
                           struct Population *new_population,
                           int mutate_genes_number,
                           double genetic_diversity_limit
                          );

#endif
