#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VERTICES 50
#define INF 999999

// ==========================================
// 자료구조 정의
// ==========================================
typedef struct {
    int u, v, weight;
} Edge;

typedef struct AdjNode {
    int dest;
    int weight;
    struct AdjNode* next;
} AdjNode;

typedef struct {
    int n;
    AdjNode* head[MAX_VERTICES];
} AdjListGraph;

typedef struct {
    int n;
    int adj[MAX_VERTICES][MAX_VERTICES];
} AdjMatrixGraph;

// ==========================================
// 유니온-파인드 (크루스칼용)
// ==========================================
int parent[MAX_VERTICES];

void initUnionFind(int n) {
    for (int i = 0; i < n; i++) parent[i] = i;
}

int find_set(int i) {
    if (parent[i] == i) return i;
    return parent[i] = find_set(parent[i]);
}

bool union_set(int i, int j) {
    int root_i = find_set(i);
    int root_j = find_set(j);
    if (root_i != root_j) {
        parent[root_j] = root_i;
        return true;
    }
    return false;
}

int compareEdges(const void* a, const void* b) {
    Edge* e1 = (Edge*)a;
    Edge* e2 = (Edge*)b;
    return e1->weight - e2->weight;
}

// ==========================================
// 그래프 생성 및 초기화 함수
// ==========================================
void initGraphs(AdjListGraph* listGraph, AdjMatrixGraph* matrixGraph, int n) {
    listGraph->n = n;
    matrixGraph->n = n;
    for (int i = 0; i < n; i++) {
        listGraph->head[i] = NULL;
        for (int j = 0; j < n; j++) {
            matrixGraph->adj[i][j] = 0;
        }
    }
}

void addEdge(AdjListGraph* listGraph, AdjMatrixGraph* matrixGraph, int u, int v, int weight) {
    matrixGraph->adj[u][v] = weight;
    matrixGraph->adj[v][u] = weight;

    AdjNode* n1 = (AdjNode*)malloc(sizeof(AdjNode));
    n1->dest = v; n1->weight = weight; n1->next = listGraph->head[u];
    listGraph->head[u] = n1;

    AdjNode* n2 = (AdjNode*)malloc(sizeof(AdjNode));
    n2->dest = u; n2->weight = weight; n2->next = listGraph->head[v];
    listGraph->head[v] = n2;
}

bool isConnected(AdjMatrixGraph* graph) {
    bool visited[MAX_VERTICES] = { false };
    int queue[MAX_VERTICES];
    int front = 0, rear = 0;

    queue[rear++] = 0;
    visited[0] = true;
    int count = 1;

    while (front < rear) {
        int curr = queue[front++];
        for (int i = 0; i < graph->n; i++) {
            if (graph->adj[curr][i] > 0 && !visited[i]) {
                visited[i] = true;
                queue[rear++] = i;
                count++;
            }
        }
    }
    return (count == graph->n);
}

void generateGraph(AdjListGraph* listGraph, AdjMatrixGraph* matrixGraph, int targetEdges) {
    while (true) {
        initGraphs(listGraph, matrixGraph, MAX_VERTICES);

        for (int i = 1; i < MAX_VERTICES; i++) {
            int p = rand() % i;
            int w = (rand() % 50) + 1;
            addEdge(listGraph, matrixGraph, p, i, w);
        }

        int currentEdges = MAX_VERTICES - 1;
        while (currentEdges < targetEdges) {
            int u = rand() % MAX_VERTICES;
            int v = rand() % MAX_VERTICES;
            if (u != v && matrixGraph->adj[u][v] == 0) {
                int w = (rand() % 50) + 1;
                addEdge(listGraph, matrixGraph, u, v, w);
                currentEdges++;
            }
        }

        if (isConnected(matrixGraph)) break;
    }
}

// ==========================================
// Case 1 & Case 2: 인접 리스트 + Kruskal 알고리즘
// ==========================================
int runKruskal(AdjListGraph* graph, int e_count, int* outCount, size_t* outMemoryBytes, double* outTimeMs) {
    clock_t start, end;

    size_t memoryBytes = sizeof(AdjListGraph);
    for (int i = 0; i < graph->n; i++) {
        AdjNode* curr = graph->head[i];
        while (curr != NULL) {
            memoryBytes += sizeof(AdjNode);
            curr = curr->next;
        }
    }
    memoryBytes += sizeof(Edge) * e_count;
    memoryBytes += sizeof(int) * MAX_VERTICES;
    *outMemoryBytes = memoryBytes;

    start = clock();

    Edge* allEdges = (Edge*)malloc(sizeof(Edge) * e_count);
    int idx = 0;
    bool visited[MAX_VERTICES][MAX_VERTICES] = { false };

    for (int i = 0; i < graph->n; i++) {
        AdjNode* curr = graph->head[i];
        while (curr != NULL) {
            if (!visited[i][curr->dest]) {
                allEdges[idx++] = (Edge){ i, curr->dest, curr->weight };
                visited[i][curr->dest] = true;
                visited[curr->dest][i] = true;
            }
            curr = curr->next;
        }
    }

    qsort(allEdges, e_count, sizeof(Edge), compareEdges);
    initUnionFind(graph->n);

    int totalCost = 0;
    int edgeCount = 0;

    for (int i = 0; i < e_count; i++) {
        if (union_set(allEdges[i].u, allEdges[i].v)) {
            totalCost += allEdges[i].weight;
            edgeCount++;
            if (edgeCount == graph->n - 1) break;
        }
    }

    free(allEdges);

    end = clock();
    *outTimeMs = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    *outCount = edgeCount;

    return totalCost;
}

// ==========================================
// Case 3 & Case 4: 인접 행렬 + Prim 알고리즘
// ==========================================
int minKey(int key[], bool mstSet[], int n) {
    int min = INF, min_idx = -1;
    for (int v = 0; v < n; v++) {
        if (!mstSet[v] && key[v] < min) {
            min = key[v];
            min_idx = v;
        }
    }
    return min_idx;
}

int runPrim(AdjMatrixGraph* graph, int* outCount, size_t* outMemoryBytes, double* outTimeMs) {
    clock_t start, end;
    int n = graph->n;

    size_t memoryBytes = sizeof(AdjMatrixGraph);
    memoryBytes += sizeof(int) * MAX_VERTICES;
    memoryBytes += sizeof(bool) * MAX_VERTICES;
    memoryBytes += sizeof(int) * MAX_VERTICES;
    *outMemoryBytes = memoryBytes;

    start = clock();

    int parentArr[MAX_VERTICES];
    int key[MAX_VERTICES];
    bool mstSet[MAX_VERTICES];

    for (int i = 0; i < n; i++) {
        key[i] = INF;
        mstSet[i] = false;
    }

    key[0] = 0;
    parentArr[0] = -1;

    for (int count = 0; count < n - 1; count++) {
        int u = minKey(key, mstSet, n);
        mstSet[u] = true;

        for (int v = 0; v < n; v++) {
            if (graph->adj[u][v] && !mstSet[v] && graph->adj[u][v] < key[v]) {
                parentArr[v] = u;
                key[v] = graph->adj[u][v];
            }
        }
    }

    int totalCost = 0;
    int edgeCount = 0;
    for (int i = 1; i < n; i++) {
        totalCost += graph->adj[i][parentArr[i]];
        edgeCount++;
    }

    end = clock();
    *outTimeMs = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    *outCount = edgeCount;

    return totalCost;
}

// ==========================================
// 메인 함수
// ==========================================
int main() {
    // 한글 출력을 위한 로케일 설정 (환경에 따라 필요할 수 있음)
    // setlocale(LC_ALL, "Korean");

    srand((unsigned int)time(NULL));

    int mstEdges;
    int mstCost;
    size_t memoryBytes;
    double execTime;

    AdjListGraph sparseList, denseList;
    AdjMatrixGraph sparseMatrix, denseMatrix;

    generateGraph(&sparseList, &sparseMatrix, 100);
    generateGraph(&denseList, &denseMatrix, 600);

    // --- Case 1 ---
    mstCost = runKruskal(&sparseList, 100, &mstEdges, &memoryBytes, &execTime);
    printf("케이스 1 : 희소 그래프 - 인접 리스트 - 크루스칼\n");
    printf("정점 개수   : 50\n");
    printf("간선 개수   : 100\n");
    printf("MST 간선 수 : %d\n", mstEdges);
    printf("MST 총 비용 : %d\n", mstCost);
    printf("메모리 사용량 : %zu 바이트\n", memoryBytes);
    printf("실행 시간   : %.4f ms\n\n", execTime);

    // --- Case 2 ---
    mstCost = runKruskal(&denseList, 600, &mstEdges, &memoryBytes, &execTime);
    printf("케이스 2 : 밀집 그래프 - 인접 리스트 - 크루스칼\n");
    printf("정점 개수   : 50\n");
    printf("간선 개수   : 600\n");
    printf("MST 간선 수 : %d\n", mstEdges);
    printf("MST 총 비용 : %d\n", mstCost);
    printf("메모리 사용량 : %zu 바이트\n", memoryBytes);
    printf("실행 시간   : %.4f ms\n\n", execTime);

    // --- Case 3 ---
    mstCost = runPrim(&sparseMatrix, &mstEdges, &memoryBytes, &execTime);
    printf("케이스 3 : 희소 그래프 - 인접 행렬 - 프림\n");
    printf("정점 개수   : 50\n");
    printf("간선 개수   : 100\n");
    printf("MST 간선 수 : %d\n", mstEdges);
    printf("MST 총 비용 : %d\n", mstCost);
    printf("메모리 사용량 : %zu 바이트\n", memoryBytes);
    printf("실행 시간   : %.4f ms\n\n", execTime);

    // --- Case 4 ---
    mstCost = runPrim(&denseMatrix, &mstEdges, &memoryBytes, &execTime);
    printf("케이스 4 : 밀집 그래프 - 인접 행렬 - 프림\n");
    printf("정점 개수   : 50\n");
    printf("간선 개수   : 600\n");
    printf("MST 간선 수 : %d\n", mstEdges);
    printf("MST 총 비용 : %d\n", mstCost);
    printf("메모리 사용량 : %zu 바이트\n", memoryBytes);
    printf("실행 시간   : %.4f ms\n", execTime);

    return 0;
}
