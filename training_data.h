#ifndef TRAINING_DATA_H
#define TRAINING_DATA_H

extern struct TrainingData
{
    int size; // size of training data
    double *dataX; // training dataX
    double *dataY; // training dataY
};

void init_training_data(struct TrainingData *training_data, double *dataX, double *dataY, int size);
void copy_training_data(const struct TrainingData *in, struct TrainingData *out);
void print_training_data(const struct TrainingData *training_data);
void training_data_linear_transform(struct TrainingData *training_data, double factor, int down_or_up);

#endif
