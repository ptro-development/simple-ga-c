extern struct Gene
{
  int min_value;
  int max_value;
};

void init_gene(struct Gene *gene, const int min_value, const int max_value);
