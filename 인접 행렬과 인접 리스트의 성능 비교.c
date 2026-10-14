#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define V 100 

// 인접 리스트 노드 구조체
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// 간선 정보를 담기 위한 구조체
typedef struct {
    int u;
    int v;
} Edge;

// 인접 리스트에 무방향 간선 (u, v) 추가
void addEdgeToList(Node* adjList[], int u, int v) {
    Node* n1 = (Node*)malloc(sizeof(Node));
    n1->vertex = v;
    n1->next = adjList[u];
    adjList[u] = n1;

    Node* n2 = (Node*)malloc(sizeof(Node));
    n2->vertex = u;
    n2->next = adjList[v];
    adjList[v] = n2;
}

// 인접 리스트 메모리 해제
void freeAdjList(Node* adjList[]) {
    for (int i = 0; i < V; i++) {
        Node* curr = adjList[i];
        while (curr != NULL) {
            Node* temp = curr;
            curr = curr->next;
            free(temp);
        }
        adjList[i] = NULL;
    }
}

// 무방향 단순 그래프 조건 만족하는 간선 생성 (자기 루프 및 중복 방지)
int generateSimpleEdges(Edge edges[], int num_edges) {
    int count = 0;
    while (count < num_edges) {
        int u = rand() % V;
        int v = rand() % V;
        if (u == v) continue;
        if (u > v) { int t = u; u = v; v = t; }

        bool exists = false;
        for (int i = 0; i < count; i++) {
            if (edges[i].u == u && edges[i].v == v) { exists = true; break; }
        }
        if (!exists) {
            edges[count].u = u;
            edges[count].v = v;
            count++;
        }
    }
    return count;
}

// 그래프 구조체 정의
typedef struct {
    int matrix[V][V];
    Node* list[V];
} Graph;

void initGraph(Graph* g) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) g->matrix[i][j] = 0;
        g->list[i] = NULL;
    }
}

void populateGraph(Graph* g, Edge edges[], int edge_count) {
    for (int i = 0; i < edge_count; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        g->matrix[u][v] = 1;
        g->matrix[v][u] = 1;
        addEdgeToList(g->list, u, v);
    }
}


void printBenchmarkResults(const char* graph_name, Graph* g, Edge all_edges[], int edge_count) {
    
    //  간선 삽입 대상 (존재하지 않는 정점 쌍)
    int ins_u = -1, ins_v = -1;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (g->matrix[i][j] == 0) { ins_u = i; ins_v = j; break; }
        }
        if (ins_u != -1) break;
    }

    // 간선 삭제 대상 (존재하는 첫 번째 간선)
    int del_u = all_edges[0].u;
    int del_v = all_edges[0].v;

    //  연결 여부 확인 대상 (존재하는 첫 번째 간선 사용)
    int check_u = all_edges[0].u;
    int check_v = all_edges[0].v;

    //  특정 정점 인접 정점 출력 대상 (정점 42번 선택)
    int target_vertex = 42;

    printf("=====================================================\n");
    printf(" [%s] 측정 결과\n", graph_name);
    printf("=====================================================\n");

    // ==========================================
    // Case A: 인접 행렬 (Adjacency Matrix)
    // ==========================================
    // 메모리: 100x100 int 배열 공간 + 행렬 포인터 등 부가 공간
    size_t matrix_memory = sizeof(g->matrix);

    // 
    int matrix_ins_comp = 1;

    // 
    int matrix_del_comp = 1;

    // 
    int matrix_check_comp = 1;

    // 인접 정점 비교 횟수: matrix[u][0] ~ matrix[u][99] 전체 확인 (100회)
    int matrix_adj_comp = 0;
    for (int j = 0; j < V; j++) {
        matrix_adj_comp++;
    }

    printf("[ %s - 인접 행렬 (Adjacency Matrix) ]\n", graph_name);
    printf("사용 메모리 용량                 : %zu 바이트\n", matrix_memory);
    printf("간선 삽입 비교 횟수              : %d 회\n", matrix_ins_comp);
    printf("간선 삭제 비교 횟수              : %d 회\n", matrix_del_comp);
    printf("두 정점 연결 여부 확인 비교 횟수 : %d 회\n", matrix_check_comp);
    printf("특정 정점 인접 정점 출력 비교 횟수 : %d 회\n\n", matrix_adj_comp);

    
    // 인접 리스트 (Adjacency List)
    
    // 메모리: 헤더 포인터 배열(100개) + 간선 노드(간선수 * 2) 저장 공간
    size_t list_header_memory = sizeof(g->list);
    size_t list_nodes_memory = (size_t)(edge_count * 2) * sizeof(Node);
    size_t list_total_memory = list_header_memory + list_nodes_memory;

    // 삽입 비교 횟수: ins_u의 리스트를 끝까지 순회한 노드 수
    int list_ins_comp = 0;
    Node* curr = g->list[ins_u];
    while (curr != NULL) { list_ins_comp++; curr = curr->next; }

    // 삭제 비교 횟수: del_u 리스트 탐색 횟수 + del_v 리스트 탐색 횟수
    int list_del_comp = 0;
    curr = g->list[del_u];
    while (curr != NULL) {
        list_del_comp++;
        if (curr->vertex == del_v) break;
        curr = curr->next;
    }
    curr = g->list[del_v];
    while (curr != NULL) {
        list_del_comp++;
        if (curr->vertex == del_u) break;
        curr = curr->next;
    }

    // 연결 여부 비교 횟수: check_u 리스트에서 check_v를 찾을 때까지 확인한 노드 수
    int list_check_comp = 0;
    curr = g->list[check_u];
    while (curr != NULL) {
        list_check_comp++;
        if (curr->vertex == check_v) break;
        curr = curr->next;
    }

    // 인접 정점 비교 횟수: target_vertex 리스트에 저장된 노드 수(차수)만큼 확인
    int list_adj_comp = 0;
    curr = g->list[target_vertex];
    while (curr != NULL) {
        list_adj_comp++;
        curr = curr->next;
    }

    printf("[ %s - 인접 리스트 (Adjacency List) ]\n", graph_name);
    printf("사용 메모리 용량                 : %zu 바이트 (헤더 %zu + 노드 %zu)\n", list_total_memory, list_header_memory, list_nodes_memory);
    printf("간선 삽입 비교 횟수              : %d 회\n", list_ins_comp);
    printf("간선 삭제 비교 횟수              : %d 회\n", list_del_comp);
    printf("두 정점 연결 여부 확인 비교 횟수 : %d 회\n", list_check_comp);
    printf("특정 정점 인접 정점 출력 비교 횟수 : %d 회\n", list_adj_comp);
    printf("-----------------------------------------------------\n\n");
}

int main() {
    // 난수 시드 초기화
    srand((unsigned int)time(NULL));

    int sparse_edge_count = 100;
    int dense_edge_count = 4000;

    Edge* sparse_edges = (Edge*)malloc(sizeof(Edge) * sparse_edge_count);
    Edge* dense_edges = (Edge*)malloc(sizeof(Edge) * dense_edge_count);

    generateSimpleEdges(sparse_edges, sparse_edge_count);
    generateSimpleEdges(dense_edges, dense_edge_count);

    // 희소 그래프 생성
    Graph sparse_graph;
    initGraph(&sparse_graph);
    populateGraph(&sparse_graph, sparse_edges, sparse_edge_count);

    // 밀집 그래프 생성
    Graph dense_graph;
    initGraph(&dense_graph);
    populateGraph(&dense_graph, dense_edges, dense_edge_count);

    // 결과 출력 (Case 1 ~ Case 4 형태 반영)
    printBenchmarkResults("희소 그래프 (Sparse Graph)", &sparse_graph, sparse_edges, sparse_edge_count);
    printBenchmarkResults("밀집 그래프 (Dense Graph)", &dense_graph, dense_edges, dense_edge_count);

    // 메모리 해제
    free(sparse_edges);
    free(dense_edges);
    freeAdjList(sparse_graph.list);
    freeAdjList(dense_graph.list);

    return 0;
}
