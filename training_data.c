#include "training_data.h"
#include <stdio.h>

void init_training_data(struct TrainingData *training_data, double *dataX, double *dataY, int size)
{
    dataX = (double*) malloc(sizeof(double) * size);
    dataY = (double*) malloc(sizeof(double) * size);
    training_data->dataX = dataX;
    training_data->dataY = dataY;
    training_data->size = size;
};

void print_training_data(const struct TrainingData *training_data)
{
    int i;
    for (i = 0; i < training_data->size ; i++)
    {
        printf("index:%d X:%f Y:%f\n", i, training_data->dataX[i], training_data->dataY[i]);
    }
}

void copy_training_data(const struct TrainingData *in, struct TrainingData *out)
{
    double *dataX = (double*) malloc(sizeof(double) * in->size);
    double *dataY = (double*) malloc(sizeof(double) * in->size);
    if(dataX != NULL & dataY != NULL)
    {
        out->dataX = dataX;
        out->dataY = dataY;
        out->size = in->size;
        int i;
        for (i = 0; i < in->size ; i++)
        {
            out->dataX[i] = in->dataX[i];
            out->dataY[i] = in->dataY[i];
        }
    }
}

void training_data_linear_transform(struct TrainingData *training_data, double factor, int down_or_up)
{
    int i;
    for (i = 0; i < training_data->size ; i++)
    {
        if (down_or_up == 0)
        {
            training_data->dataX[i] /= factor;
            training_data->dataY[i] /= factor;
        }
        else
        {
            training_data->dataX[i] *= factor;
            training_data->dataY[i] *= factor;
        }
    }
}
