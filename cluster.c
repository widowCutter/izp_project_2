#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define err -1
#define success 0
struct Object {
  int id;
  int x;
  int y;
};
struct Matrix {
  struct Object *objects;
  int size;
};
void print_matrix_cli(struct Matrix *matrix) {
  for (int i = 0; i < matrix->size; i++) {
    printf("ID:[%d] x:[%d], y:[%d]\n", matrix->objects[i].id,
           matrix->objects[i].x, matrix->objects[i].y);
  }
}
FILE *init_file(char *path) {
  FILE *fp = fopen(path, "r");
  return fp;
}
int parse_table(struct Matrix *matrix, FILE *fp) {
  int count;
  fscanf(fp, "count=%d", &count);
  printf("%d\n", count);
  matrix->size = count;
  if ((matrix->objects = malloc(sizeof(struct Object) * count)) == NULL) {
    printf("Memory allocation\n");
    return err;
  };
  for (int i = 0; i < matrix->size; i++) {
    struct Object object;
    fscanf(fp, "%d %d %d", &object.id, &object.x, &object.y);
    matrix->objects[i] = object;
  }
  return success;
}
int cluster_matrix(int num_clusters, struct Matrix *matrix) {
  for (int i = 0; i < matrix->size; i++) {
    for (int o = 0; o < matrix->size; o++) {
    }
  }
  return err;
}

float calculate_lenght(struct Object *obj_a, struct Object *obj_b)

{
  float lenght = -1;
  int a = obj_a->x - obj_b->x;
  a = abs(a);

  return lenght;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Wrong number of arguments supplied\n");
    return err;
  }
  FILE *fp = init_file(argv[1]);
  if (!fp) {
    printf("Wrong file supplied\n");
    return err;
  }
  struct Matrix matrix;
  if (parse_table(&matrix, fp)) {
    printf("Input file not valid\n");
    return err;
  }
  print_matrix_cli(&matrix);
  return success;
}
