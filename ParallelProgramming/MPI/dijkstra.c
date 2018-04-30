#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <mpi.h>

const int UNDEF = 9999;

struct tour_t {
    int cities[50];
    int num_cities;
    int cost;
};

struct stack_t {
    struct tour_t list[50];
    int size;
};

bool contains_vertex(int *vertices, int vertex, int num_vertices);
int get_index_of(int vertex, int *vertices);

int tour_city(struct tour_t tour, int i);
void push(struct stack_t stack, struct tour_t tour);
bool is_empty(struct stack_t stack);
struct tour_t pop(struct stack_t stack);

int main()
{
    int num_data;
    printf("Enter num_data: ");
    fflush(stdout);
    scanf("%d", &num_data);
    int i = 0;
    char *pch;
    char line[100];
    int *data = malloc(sizeof(int) * num_data * 9);
    FILE *file = fopen("cities", "r");
    while (fgets(line, sizeof(line), file) != NULL) {
        pch = strtok(line, ";\n");
        while (pch != NULL) {
            data[i++] = atoi(pch);
            pch = strtok(NULL, ";\n");
        }
    }
    fclose(file);
    printf("\n");

    int num_edges = 0;
    int num_vertices = 0;
    int edges[50][50];
    int *vertices = malloc(sizeof(int) * num_data);
    int *vertex = malloc(sizeof(int) * 2);
    
    for (int j = 0; j < i; j++) {
        vertex[0] = data[j++];
        vertex[1] = data[j++];
        int distance = data[j];
        
        for (int k = 0; k < 2; k++) {
            if (!contains_vertex(vertices, vertex[k], num_vertices)) {
                vertices[num_vertices++] = vertex[k];
                for (int l = 0; l < num_vertices; l++) {
                    edges[num_vertices][l] = 0;
                    edges[l][num_vertices] = 0;
                }
            }
        }
        int row = get_index_of(vertex[0], vertices);
        int col = get_index_of(vertex[1], vertices);
        edges[row][col] = distance;
        num_edges++;
    }

    // for (int j = 0; j < num_vertices; j++) {
    //     for (int k = 0; k < num_vertices; k++) {
    //         printf("%d\t", edges[j][k]);
    //     }
    //     printf("\n");
    // }

    struct stack_t stack;
    struct tour_t tour = { .cost = 0, .num_cities = 1, .cities[0] = 0 };
    struct tour_t curr_tour;
    struct tour_t best_tour = { .cost = UNDEF };
    push(stack, tour);
    while (!is_empty(stack)) {
        curr_tour = pop(stack);
        if (curr_tour.num_cities == num_data) {
            if (curr_tour .cost < best_tour .cost) {
                best_tour .cities = curr_tour .cities;
                best_tour .num_cities = curr_tour .num_cities;
                best_tour .cost = curr_tour .cost;
            }
        } else {
            for (int j = num_data - 1; j >= 1; j--) {
                if (curr_tour .cost < best_tour .cost) {
                    curr_tour .cities[curr_tour .size] = j;
                    curr_tour .cost += edges[curr_tour .cities[curr_tour .size-1]][j];
                    curr_tour .size++;
                    push(stack, curr_tour);
                    curr_tour .cities[curr_tour .size-1] = NULL;
                }
            }
            free(curr_tour .cities);
        }
    }

    return 0;
}

bool contains_vertex(int *vertices, int vertex, int num_vertices)
{
    for (int i = 0; i < num_vertices; i++) {
        if (vertex == vertices[i]) return true;
    }
    return false;
}

int get_index_of(int vertex, int *vertices)
{
    int index = 0;
    while (vertex != vertices[index]) {
        index++;
    }
    return index;
}

int tour_city(struct tour_t tour, int i)
{
    return tour .cities[i];
}

void push(struct stack_t stack, struct tour_t tour)
{
    int location = stack .size;
    tour_t tmp = malloc(sizeof(tour_t));
    tmp .cities = tour .cities;
    tmp .num_cities = tour .num_cities;
    tmp .cost = tour .cost;
    stack .list[location] = tmp;
    stack .size++;
}

struct tour_t pop(struct stack_t stack)
{
    tour_t tmp = malloc(sizeof(tour_t));
    tmp .cities = stack .list[stack .size - 1] .cities;
    tmp .num_cities = stack .list[stack .size - 1] .num_cities;
    tmp .cost = stack .list[stack .size - 1] .cost;
    stack .list[stack .size - 1] = NULL;
    stack .size--;
    return tmp;
}

bool is_empty(struct stack_t stack)
{
    return stack .size == 0;
}
