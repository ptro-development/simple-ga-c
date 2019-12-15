#ifndef RECORD_H
#define RECORD_H

#include <stdio.h>

#include "gene_definition.h"
#include "training_data.h"
#include "member.h"
#include "population.h"

void save_member_data(const struct Member *member, const char *file_name, int flag);
int write_member_gene(FILE *fp, const struct Member *member);

int write_training_data_records(FILE *fp, const char *format, double *dataX, double *dataY, int num);
int read_training_data_records(FILE *fp, const char *format, double *dataX, double *dataY, int num);

int write_population_records(FILE *fp, const struct Population *population, const struct GeneDefinition *gene_definition);
void save_population(const struct Population *population, const struct GeneDefinition *gene_definition, const char *file_name, int flag);

void save_training_data(const struct TrainingData *training_data, const char *file_name, int flag);
void read_training_data(const struct TrainingData *training_data, const char *file_name, int flag);

void read_population(struct Population *population, const struct GeneDefinition *gene_definition, const char *file_name, int flag);
// this function expects population already initialized / alocated in memory
// it just overwites genes
int read_population_records(FILE *fp, struct Population *population, const struct GeneDefinition *gene_definition);

void save_fitness_data(const double *fitness, const int size, const char *file_name, int flag);
int write_fitness_records(FILE *fp, const double *fitness, const int size);

#endif
