#include "record.h"

void save_member_data(const struct Member *member, const char *file_name, int flag)
{
    FILE *fd;
    if(flag == 1)
    {
        fd = fopen(file_name, "w");
        if (fd != NULL)
        {
            write_member_gene(fd, member);
            fclose(fd);
        }
    }
}

int write_member_gene(FILE *fp, const struct Member *member)
{
    int i = 0;
    fseek(fp, 0L, SEEK_SET);
    for(i = 0 ; i < member->gene_definition.size ; i++)
    {
        if(fprintf(fp, "%lf ", member->gene[i]) < 0)
            break;
        /* error on fprintf! */
    }
    fputs("\n", fp);
    return i;
}

void save_training_data(const struct TrainingData *training_data, const char *file_name, int flag)
{
    FILE *fd;
    const char *format = "%lf %lf\n";
    if(flag == 1)
    {
        fd = fopen(file_name, "w");
        if (fd != NULL)
        {
            write_training_data_records(fd, format, training_data->dataX, training_data->dataY, training_data->size);
            fclose(fd);
        }
    }
}

int write_training_data_records(FILE *fp, const char *format, double *dataX, double *dataY, int num)
{
    int i = 0;
    fseek(fp, 0L, SEEK_SET);
    for(i = 0 ; i < num ; i++)
    {
        if(fprintf(fp, format, dataX[i], dataY[i]) < 0)
            break;
        /* error on fprintf! */
    }
    return i;
}

void read_training_data(const struct TrainingData *training_data, const char *file_name, int flag)
{
    FILE *fd;
    const char *format = "%lf %lf";
    if(flag == 1)
    {
        fd = fopen(file_name, "r");
        if (fd != NULL)
        {
            read_training_data_records(fd, format, training_data->dataX, training_data->dataY, training_data->size);
            fclose(fd);
        }
    }
}

int read_training_data_records(FILE *fp, const char *format, double *dataX, double *dataY, int num)
{
    if(num == 0)
        return 0;

    fseek(fp, 0L, SEEK_SET);
    int i = 0;
    for(i = 0 ; i < num ; i++)
    {
        if(fscanf(fp, format, &dataX[i], &dataY[i]) != 2)
            break;
        /* error on fscanf! */
    }
    return i;
}

void save_population(const struct Population *population, const struct GeneDefinition *gene_definition, const char *file_name, int flag)
{
    FILE *fd;
    if(flag == 1)
    {
        fd = fopen(file_name, "w");
        if (fd != NULL)
        {
            write_population_records(fd, population, gene_definition);
            fclose(fd);
        }
    }
}

int write_population_records(FILE *fp, const struct Population *population, const struct GeneDefinition *gene_definition)
{
    int i, j;
    fseek(fp, 0L, SEEK_SET);
    for(i = 0 ; i < population->size; i++)
    {
        for (j = 0; j < gene_definition->size; j++)
        {
            if(fprintf(fp, "%lf ", population->members[i].gene[j]) < 0)
                break;
            /* error on fprintf! */
        }
        fputs("\n", fp);
    }
    return i;
}

void read_population(struct Population *population, const struct GeneDefinition *gene_definition, const char *file_name, int flag)
{
    FILE *fd;
    if(flag == 1)
    {
        fd = fopen(file_name, "r");
        if (fd != NULL)
        {
            read_population_records(fd, population, gene_definition);
            fclose(fd);
        }
    }
}
// this function expects population already initialized / alocated in memory
// it just overwites genes
int read_population_records(FILE *fp, struct Population *population, const struct GeneDefinition *gene_definition)
{
    int i, j;

    fseek(fp, 0L, SEEK_SET);
    for(i = 0 ; i < population->size; i++)
    {
        for (j = 0; j < gene_definition->size; j++)
        {
            if(fscanf(fp, "%lf ", &(population->members[i].gene[j])) != 1)
                break;
            /* error on fprintf! */
        }
    }
    return i;
}

void save_fitness_data(const double *fitness, const int size, const char *file_name, int flag)
{
    FILE *fd;
    if(flag == 1)
    {
        fd = fopen(file_name, "w");
        if (fd != NULL)
        {
            write_fitness_records(fd, fitness, size);
            fclose(fd);
        }
    }
}

int write_fitness_records(FILE *fp, const double *fitness, const int size)
{
    int i;
    fseek(fp, 0L, SEEK_SET);
    for(i = 0 ; i < size; i++)
    {
        if(fprintf(fp, "%lf\n", fitness[i]) < 0)
            break;
    }
    return i;
}
