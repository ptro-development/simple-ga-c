#include "context.h"

//int mutate_gene_size = MUTATE_GENE_LEN_SIZE_MIN;

void init_context(struct Context *ctx)
{
    //init gene
    struct GeneDefinition gene_definition;
    init_gene_definition(&gene_definition, GENE_SIZE, MIN_GENE_VALUE, MAX_GENE_VALUE);
    ctx->gene_definition = gene_definition;

    //init selection
    struct Selection selection;
    init_selection(&selection, CROSS_RATE);
    ctx->selection = selection;

    // init mutation
    struct Mutation mutation;
    init_mutation(&mutation, MUTATION_RATE_MIN, MUTATION_RATE_MAX, MUTATION_RATE_STEP);
    ctx->mutation = mutation;

    // init line
    struct Line line;
    init_line(&line, ctx->mutation.min_limit, ctx->mutation.max_limit, ctx->gene_definition.max_value * 0.25, ctx->gene_definition.max_value);
    ctx->line = line;

    // init taboo search fifo
    struct Fifo taboo_fifo;
    init_fifo(&taboo_fifo, ctx->gene_definition.size, TABOO_FIFO_SIZE);
    ctx->taboo_fifo = taboo_fifo;

    // gerate training data
    struct TrainingData training_data;
    double *dataX, *dataY;
    init_training_data(&training_data, dataX, dataY, TRAINING_DATA_SIZE);
    generate_circles(training_data.dataX, training_data.dataY, CIRCLE_NUMBER, training_data.size);
    ctx->training_data = training_data;

    // init fitness function
    ctx->fitness_function_ptr = &fitnessXY;

    // inicialize gnuplot for member fitness
    gnuplot_ctrl *fitness_plot;
    ctx->fitness_plot = fitness_plot;
    ctx->fitness_plot = gnuplot_init();
    init_fitness_plot(ctx->fitness_plot);

    // initialize gnuplot for training data
    gnuplot_ctrl *training_data_plot;
    ctx->training_data_plot = training_data_plot;
    ctx->training_data_plot = gnuplot_init();
    init_training_data_plot(ctx->training_data_plot);
}

void context_linear_transform(struct Context *ctx, double factor, int down_or_up)
{
    gene_definition_linear_transform(&ctx->gene_definition, factor, down_or_up);
    line_linear_transform(&ctx->line, factor, down_or_up);
    fifo_linear_transform(&ctx->taboo_fifo, factor, down_or_up);
    training_data_linear_transform(&ctx->training_data, factor, down_or_up);
}

void copy_context(struct Context *in, struct Context *out)
{
    copy_gene_definition(&in->gene_definition, &out->gene_definition);
    copy_selection(&in->selection, &out->selection);
    copy_mutation(&in->mutation, &out->mutation);
    copy_line(&in->line, &out->line);
    copy_fifo(&in->taboo_fifo, &out->taboo_fifo);
    copy_training_data(&in->training_data, &out->training_data);
    out->fitness_function_ptr = in->fitness_function_ptr;

    // hmm ... this is going to be dirty, not a real copy

    // inicialize gnuplot for member fitness
    gnuplot_ctrl *fitness_plot;
    out->fitness_plot = fitness_plot;
    out->fitness_plot = gnuplot_init();
    init_fitness_plot(out->fitness_plot);

    // initialize gnuplot for training data
    gnuplot_ctrl *training_data_plot;
    out->training_data_plot = training_data_plot;
    out->training_data_plot = gnuplot_init();
    init_training_data_plot(out->training_data_plot);
}
