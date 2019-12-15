#ifndef CONTEXT_H
#define CONTEXT_H

#include "gnuplot_i.h"

#include "mutation.h"
#include "line.h"
#include "selection.h"
#include "gene_definition.h"
#include "training_data.h"
#include "fifo.h"

#define MUTATION_RATE_MIN 0.02
#define MUTATION_RATE_MAX 0.2
#define MUTATION_RATE_STEP 0.01

#define GENE_SIZE 9
#define MAX_GENE_VALUE 100.0
#define MIN_GENE_VALUE 0.0

#define CROSS_RATE 0.9

#define CIRCLE_NUMBER 3
#define TRAINING_DATA_SIZE 300

#define TABOO_FIFO_SIZE 30

struct Context
{
    struct Mutation mutation;
    struct Line line;
    struct Selection selection;
    struct GeneDefinition gene_definition;
    struct TrainingData training_data;
    struct Fifo taboo_fifo;
    double(*fitness_function_ptr)(struct Member *member, const struct TrainingData *training_data);
    gnuplot_ctrl *training_data_plot;
    gnuplot_ctrl *fitness_plot;
};

//struct Context ctx;
void init_context(struct Context *ctx);
void copy_context(struct Context *in, struct Context *out);
void context_linear_transform(struct Context *ctx, double factor, int down_or_up);

#endif
