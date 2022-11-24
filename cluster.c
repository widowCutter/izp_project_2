/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <assert.h>
#include <limits.h> // INT_MAX
#include <math.h>   // sqrtf
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#define succ 0
#define err -1
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...)                                                           \
  printf(" - "__FILE__                                                         \
         ":%u: "s                                                              \
         "\n",                                                                 \
         __LINE__, __VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
  int id;
  float x;
  float y;
};

struct cluster_t {
  int size;
  int capacity;
  struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap) {
  assert(c != NULL);
  assert(cap >= 0);

  c->obj = malloc(sizeof(struct obj_t) * cap);
  if (c->obj == NULL) {
    c->capacity = 0;
  } else {

    c->capacity = cap;
  }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c) {
  free(c->obj);
  init_cluster(c, 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap) {
  // TUTO FUNKCI NEMENTE
  assert(c);
  assert(c->capacity >= 0);
  assert(new_cap >= 0);

  if (c->capacity >= new_cap)
    return c;

  size_t size = sizeof(struct obj_t) * new_cap;

  void *arr = realloc(c->obj, size);
  if (arr == NULL)
    return NULL;

  c->obj = (struct obj_t *)arr;
  c->capacity = new_cap;
  return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
int append_cluster(struct cluster_t *c, struct obj_t obj) {
  if ((c->size + 1) > c->capacity) {
    c = resize_cluster(c, c->capacity + CLUSTER_CHUNK);
    }
  c->obj[c->size] = obj;
  c->size++;
  return succ;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
int merge_clusters(struct cluster_t *c1, struct cluster_t *c2) {
  assert(c1 != NULL);
  assert(c2 != NULL);

  for (int i = 0; i < c2->size; i++) {
    if (append_cluster(c1, c2->obj[i])) {
      return err;
    }
  }
  sort_cluster(c1);
  return succ;
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx) {
  assert(idx < narr);
  assert(narr > 0);

  clear_cluster(&carr[idx]);

  for (int i = idx; i < narr; i++) {
    carr[i] = carr[i + 1];
  }
  narr--;
  return narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2) {
  assert(o1 != NULL);
  assert(o2 != NULL);

  float x_dis = o1->x - o2->x;
  float y_dis = o1->y - o2->y;

  x_dis = powf(x_dis, 2);
  y_dis = powf(y_dis, 2);
  return sqrtf(x_dis + y_dis);
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2) {
  assert(c1 != NULL);
  assert(c1->size > 0);
  assert(c2 != NULL);
  assert(c2->size > 0);

  float low_dist = obj_distance(&c1->obj[0], &c2->obj[0]);

  for (int i = 0; i < c1->size; i++) {
    for (int o = 0; o < c2->size; o++) {
      float dist = obj_distance(&c1->obj[i], &c2->obj[o]);
      if (dist < low_dist) {
        low_dist = dist;
      }
    }
  }
  return low_dist;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void print_cluster(struct cluster_t *c); // remove
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2) {
  assert(narr > 0);

  float low_distance = 10000;
  for (int i = 0; i < narr; i++) {
    for (int o = 0; o < narr; o++) {
      if (i == o) {
        continue;
      }
      float distance = cluster_distance(&carr[i], &carr[o]);
      if (distance < low_distance) {
        *c1 = i;
        *c2 = o;
        low_distance = distance;
      }
    }
  }
  // TODO
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b) {
  // TUTO FUNKCI NEMENTE
  const struct obj_t *o1 = (const struct obj_t *)a;
  const struct obj_t *o2 = (const struct obj_t *)b;
  if (o1->id < o2->id)
    return -1;
  if (o1->id > o2->id)
    return 1;
  return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c) {
  // TUTO FUNKCI NEMENTE
  qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c) {
  // TUTO FUNKCI NEMENTE
  for (int i = 0; i < c->size; i++) {
    if (i)
      putchar(' ');
    printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
  }
  putchar('\n');
}

FILE *load_file(char *path) {
  FILE *fp = fopen(path, "r+");
  if (fp) {
    return fp;
  } else {
    perror("Invalid supplied file");
    exit(-1);
  }
}
/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr) {
  assert(arr != NULL);

  FILE *fp = load_file(filename);
  int count = 0;
  fscanf(fp, "count=%d\n", &count);
  struct cluster_t *array = malloc(sizeof(struct cluster_t) * count);
  *arr = array;
  for (int i = 0; i < count; i++) {
    int id = 0, x = 0, y = 0;
    if ((fscanf(fp, "%d %d %d\n", &id, &x, &y)) < 3) {
      printf("Wrong line %d, aborting...\n", i + 2);
      exit(10);
    }
    init_cluster(&array[i], CLUSTER_CHUNK);
    array[i].size = 1;
    array[i].obj[0].id = id;
    array[i].obj[0].x = x;
    array[i].obj[0].y = y;
  }
  return count;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr) {
  printf("Clusters:\n");
  for (int i = 0; i < narr; i++) {
    printf("cluster %d: ", i);
    print_cluster(&carr[i]);
  }
}
int try_n_clusters(int argc, char *argv[]) {
  int n_clusters = 1;

  if (argc > 2) {
    sscanf(argv[2], "%d", &n_clusters);
  }
  dint(n_clusters);

  return n_clusters;
}
struct clust_distance {
  struct cluster_t *a;
  int a_index;
  struct cluster_t *b;
  int b_index;
  float distance;
};
int cluster_table(struct cluster_t *arr, int n_clusters, int target) {


  print_clusters(arr, n_clusters);

  while (n_clusters > target) {
    int a = 0, b = 0;
    int *a_p = &a, *b_p = &b;
    print_clusters(arr, n_clusters);
    find_neighbours(arr, n_clusters, a_p, b_p);
    merge_clusters(&arr[a], &arr[b]);
    remove_cluster(arr, n_clusters, b);
    n_clusters--;
  }
  print_clusters(arr, n_clusters);
  return succ;
}

int main(int argc, char *argv[]) {

  int n_clusters = try_n_clusters(argc, argv);

  struct cluster_t *clusters;

  int n_obj = load_clusters(argv[1], &clusters);

  int result = cluster_table(clusters, n_obj, n_clusters);
  return result;
  // TODO
}
