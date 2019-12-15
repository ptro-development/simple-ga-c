#include "line.h"

// line equation y = ax + b

double compute_line_y(const struct Line *line, const double *x)
{
    return (line->a * *x + line->b);
}

void copy_line(struct Line *in, struct Line *out)
{
    *out = *in;
}

void init_line(struct Line *line, double min_x, double max_x, double min_y, double max_y)
{
    line->min_x = min_x;
    line->max_x = max_x;
    line->min_y = min_y;
    line->max_y = max_y;
    line->a = (line->max_y - line->min_y) / (line->max_x - line->min_x);
    line->b = (line->min_y * line->max_x - line->max_y * line->min_x) / (line->max_x - line->min_x);
}

void line_linear_transform(struct Line *line, double factor, int down_or_up)
{
    if (down_or_up == 0)
    {
        line->min_y /= factor;
        line->max_y /= factor;
    }
    else
    {
        line->min_y *= factor;
        line->max_y *= factor;
    }
    line->a = (line->max_y - line->min_y) / (line->max_x - line->min_x);
    line->b = (line->min_y * line->max_x - line->max_y * line->min_x) / (line->max_x - line->min_x);
}


