#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

typedef struct {
    int from, to, weight;
} edge;

typedef struct set {
    struct set *parent;
} set;

int cmp(const void *a, const void *b);

set *find_parent(set *s);

set *union_sets(set *a, set *b);

set *make_sets(int NV);

int main(void) {
    FILE *file;
    if ((file = fopen("in.txt", "r")) == NULL) {
        printf("Bad input 1\n");
        return EXIT_SUCCESS;
    }
    int n, m;
    if (fscanf(file, "%d", &n) != 1) {
        printf("bad number of lines");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if ((n < 0) || (n > 5000)) {
        fclose(file);
        printf("bad number of vertices");
        return EXIT_SUCCESS;
    }
    if (fscanf(file, "%d", &m) != 1) {
        printf("bad number of lines");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if ((m < 0) || (m > n * (n - 1) / 2)) {
        printf("bad number of edges");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if (m == 0 && n == 0) {
        printf("no spanning tree");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if (m == 0 && n == 1) {
        fclose(file);
        return EXIT_SUCCESS;
    }
    edge *edges = (edge *) malloc(sizeof(edge) * m);
    int from = 0, to = 0;
    long weight = 0;
    int i = 0;
    while (fscanf(file, "%d %d %ld", &from, &to, &weight) == 3) {
        if (from < 1 || from > n || to < 1 || to > n) {
            printf("bad vertex");
            free(edges);
            fclose(file);
            return EXIT_SUCCESS;
        }
        if (weight < 0 || weight > INT_MAX) {
            printf("bad length");
            free(edges);
            fclose(file);
            return EXIT_SUCCESS;
        }
        edges[i].from = from;
        edges[i].to = to;
        edges[i].weight = weight;
        ++i;
    }
    if (i != m) {
        printf("bad number of lines");
        free(edges);
        fclose(file);
        return EXIT_SUCCESS;
    }
    qsort(edges, m, sizeof(edge), cmp);
    set *sets = make_sets(n);
    FILE *out;
    if ((out = fopen("out.txt", "w")) == NULL) {
        free(sets);
        free(edges);
        fclose(file);
        printf("Bad input 2\n");
        return EXIT_SUCCESS;
    }
    for (int j = 0; (j < m) && (n > 1); ++j) {
        if (find_parent(&sets[edges[j].from - 1]) != find_parent(&sets[edges[j].to - 1])) {
            union_sets(&sets[edges[j].from - 1], &sets[edges[j].to - 1]);
            fprintf(out,"%d %d\n",edges[j].to, edges[j].from);
            --n;
        }
    }
    fclose(out);
    if (n != 1) {
        if ((out = fopen("out.txt", "w")) == NULL) {
            printf("Bad input 3\n");
            free(sets);
            free(edges);
            fclose(file);
            return EXIT_SUCCESS;
        }
        fprintf(out, "no spanning tree");
        fclose(out);
    }
    free(sets);
    free(edges);
    fclose(file);
    return EXIT_SUCCESS;
}

int cmp(const void *a1, const void *b1) {
    edge *a = (edge *) a1;
    edge *b = (edge *) b1;
    return a->weight - b->weight;
}

set *make_sets(int NV) {
    set *sets = (set *) malloc(NV * sizeof(set));
    for (int i = 0; i < NV; i++)
        sets[i].parent = &sets[i];
    return sets;
}

set *find_parent(set *s) {
    if (s == s->parent) {
        return s;
    } else {
        return s->parent = find_parent(s->parent);
    }
}

set *union_sets(set *a, set *b) {
    a->parent = b;
    return a;
}


