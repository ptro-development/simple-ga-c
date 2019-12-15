#ifndef LINE_H
#define LINE_H

// line equation y = ax + b

struct Line
{
    double a;
    double b;
    double min_x;
    double max_x;
    double min_y;
    double max_y;
};

void init_line(struct Line *line, double min_x, double max_x, double min_y, double max_y);
void copy_line(struct Line *in, struct Line *out);
double compute_line_y(const struct Line *line, const double *x);
void line_linear_transform(struct Line *line, double factor, int down_or_up);

#endif
