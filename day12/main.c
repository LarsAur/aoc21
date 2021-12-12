#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 50

typedef struct node_t
{
    char *name;
    int *neighbours;    // Index of the node in the nodes array
    int n_neighbours;
    bool large;
} node_t;

typedef struct 
{
    int start;
    int end;

    node_t *nodes;
    int n_nodes;
} graph_t;

void parseInput(graph_t* graph);
int get_node_index(char* name, graph_t *graph);
int add_node(char* name, graph_t *graph);
void free_graph(graph_t *graph);
void count_paths(graph_t *graph, int *paths, int current_node, bool *visit_history, bool double_visit);

int main()
{
    graph_t graph = 
    {
        .start = -1,
        .end = -1,
        .nodes = NULL,
        .n_nodes = 0,
    };

    parseInput(&graph);

    bool visit_history[graph.n_nodes];

    // Part 1
    int paths = 0;
    memset(visit_history, 0, graph.n_nodes);
    count_paths(&graph, &paths, graph.start, visit_history, true);
    printf("Paths: %d", paths);
    
    // Part 2
    paths = 0;
    memset(visit_history, 0, graph.n_nodes);
    count_paths(&graph, &paths, graph.start, visit_history, false);
    printf("Paths: %d", paths);

    free_graph(&graph);

    return 0;
}

void count_paths(graph_t *graph, int *paths, int current_node, bool *visit_history, bool double_visit)
{
    if(current_node == graph->end)
    {
        (*paths)++;
        return;
    }

    if(visit_history[current_node] && !graph->nodes[current_node].large) { double_visit = true; }
    visit_history[current_node] = true;
    for(int i = 0; i < graph->nodes[current_node].n_neighbours; i++)
    {
        int neighbour = graph->nodes[current_node].neighbours[i];
        if(neighbour != graph->start && (graph->nodes[neighbour].large || !visit_history[neighbour] || !double_visit))
        {
            bool prev_visit = visit_history[neighbour];
            count_paths(graph, paths, neighbour, visit_history, double_visit);
            visit_history[neighbour] = prev_visit;
        }
    }
}

void parseInput(graph_t* graph)
{
    FILE *fp = fopen("./inputs.txt", "r");
    if(fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    char line[MAX_LINE_LENGTH + 1];
    while(fgets(line, MAX_LINE_LENGTH, fp))
    {
        int index1;
        int index2;

        int i = 0;
        while(line[i++] != '-' && i < MAX_LINE_LENGTH);
        char name[MAX_LINE_LENGTH];
        memcpy(name, line, i-1);
        name[i-1] = '\0';
        if((index1 = get_node_index(name, graph)) == -1) index1 = add_node(name, graph);

        int j = i;
        while(line[j++] != '\n' && j < MAX_LINE_LENGTH);
        memcpy(name, line + i, j - i);
        name[j-i-1] = '\0';
        if((index2 = get_node_index(name, graph)) == -1) index2 = add_node(name, graph);

        // Add the nodes as neighbours
        // Assuming the input is correct and does not add the same nodes multiple times
        graph->nodes[index1].neighbours = realloc(graph->nodes[index1].neighbours, sizeof(int) * (++(graph->nodes[index1].n_neighbours)));
        graph->nodes[index1].neighbours[graph->nodes[index1].n_neighbours - 1] = index2;

        graph->nodes[index2].neighbours = realloc(graph->nodes[index2].neighbours, sizeof(int) * (++(graph->nodes[index2].n_neighbours)));
        graph->nodes[index2].neighbours[graph->nodes[index2].n_neighbours - 1] = index1;
    }

    fclose(fp);
}

int get_node_index(char* name, graph_t *graph)
{
    for(int i = 0; i < graph->n_nodes; i++)
    {
        if(!strcmp(graph->nodes[i].name, name))
        {
            return i;   
        }
    }

    return -1;
}

int add_node(char* name, graph_t *graph)
{
    graph->nodes = realloc(graph->nodes, sizeof(node_t) * (++graph->n_nodes));
    graph->nodes[graph->n_nodes - 1].name = malloc(strlen(name) + 1);
    strcpy(graph->nodes[graph->n_nodes - 1].name, name);
    graph->nodes[graph->n_nodes - 1].large = (name[0] <= 'Z' && name[0] >= 'A');

    if(graph->start == -1 && !strcmp("start", name))
    {
        graph->start = graph->n_nodes - 1;
    }
    if(graph->end == -1 && !strcmp("end", name))
    {
        graph->end = graph->n_nodes - 1;
    }

    return graph->n_nodes - 1;
}

void free_graph(graph_t *graph)
{
    for(int i = 0; i < graph->n_nodes; i++)
    {
        free(graph->nodes[i].neighbours);
        free(graph->nodes[i].name);
    }
    free(graph->nodes);
}