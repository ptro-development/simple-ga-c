extern struct Member
{
    double *gene;
    double fitness;
};

extern struct Population
{
    struct Member *members;
    int size;
    int member_gene_size;
};

extern struct Evolution
{
    double cross_rate;
    double mutation_rate;
};

void init_member(struct Member *, const int);
void init_population(struct Population *, const int, const int);
void init_evolution(struct Evolution *, const double, const double);

double fitness(
    struct Member *,
    const int,
    const struct TrainingSet *
);

double fitnessXY(
    struct Member *,
    const int,
    const double *,
    const double *,
    const int
);

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
                       );
