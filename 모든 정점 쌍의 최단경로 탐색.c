#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define V 10
#define E 20
#define INF 1e9

int parent_node[V];

int find_root(int i) {
    if (parent_node[i] == i)
        return i;
    return parent_node[i] = find_root(parent_node[i]);
}

bool union_vertices(int i, int j) {
    int root_i = find_root(i);
    int root_j = find_root(j);
    if (root_i != root_j) {
        parent_node[root_i] = root_j;
        return true;
    }
    return false;
}

void print_path(int u, int v, int next[V][V]) {
    if (next[u][v] == -1) {
        printf("경로 없음");
        return;
    }
    printf("%d", u);
    while (u != v) {
        u = next[u][v];
        printf(" -> %d", u);
    }
}

int main() {
    srand((unsigned int)time(NULL));

    int dist[V][V];
    int next[V][V];

    struct {
        int u;
        int v;
        int weight;
    } edge_list[E];

    for (int i = 0; i < V; i++) {
        parent_node[i] = i;
        for (int j = 0; j < V; j++) {
            if (i == j) {
                dist[i][j] = 0;
                next[i][j] = i;
            }
            else {
                dist[i][j] = INF;
                next[i][j] = -1;
            }
        }
    }

    int edge_count = 0;

    int connected_edges = 0;
    while (connected_edges < V - 1) {
        int u = rand() % V;
        int v = rand() % V;
        if (u != v && union_vertices(u, v)) {
            if (dist[u][v] == INF) {
                int weight = (rand() % 100) + 1;
                dist[u][v] = weight;
                dist[v][u] = weight;
                next[u][v] = v;
                next[v][u] = u;

                edge_list[edge_count].u = u;
                edge_list[edge_count].v = v;
                edge_list[edge_count].weight = weight;

                edge_count++;
                connected_edges++;
            }
        }
    }

    while (edge_count < E) {
        int u = rand() % V;
        int v = rand() % V;

        if (u == v || dist[u][v] != INF) {
            continue;
        }

        int weight = (rand() % 100) + 1;
        dist[u][v] = weight;
        dist[v][u] = weight;
        next[u][v] = v;
        next[v][u] = u;

        edge_list[edge_count].u = u;
        edge_list[edge_count].v = v;
        edge_list[edge_count].weight = weight;

        edge_count++;
    }

    printf("그래프\n");
    printf("정점 : %d\n", V);
    printf("간선 : %d\n\n", E);

    printf("간선 목록\n");
    for (int i = 0; i < E; i++) {
        printf("(%d, %d) 가중치: %d\n", edge_list[i].u, edge_list[i].v, edge_list[i].weight);
    }
    printf("\n");

    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }

    printf("최단 경로\n\n");

    int pair_count = 0;
    for (int u = 0; u < V; u++) {
        for (int v = u + 1; v < V; v++) {
            pair_count++;
            printf("%d -> %d\n", u, v);
            printf("경로     : ");
            print_path(u, v, next);
            printf("\n");
            printf("최단거리 : %d\n\n", dist[u][v]);
        }
    }

    printf("총 정점 쌍 : %d\n", pair_count);

    return 0;
}
