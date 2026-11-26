#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct { int id; char name[64]; char gender; int kor, eng, math, total; } Student;
static long long cmp_cnt = 0;
static int name_cmp(const char* a, const char* b) { cmp_cnt++; return strcmp(a, b); }

static int load(const char* path, Student** out) {
    FILE* f = fopen(path, "r"); if (!f) return -1;
    int cap = 1024, n = 0; Student* a = malloc(cap * sizeof * a);
    char line[512]; fgets(line, sizeof line, f);           /* header */
    while (fgets(line, sizeof line, f)) {
        Student s; char g[8];
        if (sscanf(line, "%d,%63[^,],%7[^,],%d,%d,%d", &s.id, s.name, g, &s.kor, &s.eng, &s.math) != 6) continue;
        s.gender = g[0]; s.total = s.kor + s.eng + s.math;
        if (n == cap) { cap *= 2; a = realloc(a, cap * sizeof * a); }
        a[n++] = s;
    }
    fclose(f); *out = a; return n;
}

/* ---- Quick sort (3-way partition, median-of-three pivot; every name compare counted) ---- */
static void swap(Student* a, Student* b) { Student t = *a; *a = *b; *b = t; }
static void quick_sort(Student* a, int lo, int hi) {
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        /* median of three */
        if (name_cmp(a[mid].name, a[lo].name) < 0) swap(&a[mid], &a[lo]);
        if (name_cmp(a[hi].name, a[lo].name) < 0) swap(&a[hi], &a[lo]);
        if (name_cmp(a[hi].name, a[mid].name) < 0) swap(&a[hi], &a[mid]);
        char pivot[64]; strcpy(pivot, a[mid].name);
        int lt = lo, i = lo, gt = hi;
        while (i <= gt) {
            int c = name_cmp(a[i].name, pivot);
            if (c < 0) swap(&a[lt++], &a[i++]);
            else if (c > 0) swap(&a[i], &a[gt--]);
            else i++;
        }
        /* recurse on smaller part, loop on larger */
        if (lt - lo < hi - gt) { quick_sort(a, lo, lt - 1); lo = gt + 1; }
        else { quick_sort(a, gt + 1, hi); hi = lt - 1; }
    }
}

/* ---- Sequential search: full scan (all homonyms), returns match count; idx[] gets indices ---- */
static int seq_search(Student* a, int n, const char* name, int* idx) {
    int m = 0;
    for (int i = 0; i < n; i++) if (name_cmp(name, a[i].name) == 0) idx[m++] = i;
    return m;
}
/* ---- Binary search + expand left/right for homonyms ---- */
static int bin_search(Student* a, int n, const char* name, int* idx) {
    int lo = 0, hi = n - 1, pos = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2, c = name_cmp(name, a[mid].name);
        if (c == 0) { pos = mid; break; }
        if (c < 0) hi = mid - 1; else lo = mid + 1;
    }
    if (pos < 0) return 0;
    int l = pos, r = pos;
    while (l > 0 && name_cmp(name, a[l - 1].name) == 0) l--;
    while (r < n - 1 && name_cmp(name, a[r + 1].name) == 0) r++;
    int m = 0; for (int i = l; i <= r; i++) idx[m++] = i;
    return m;
}
static void print_result(Student* a, int* idx, int m, const char* name) {
    if (!m) { printf("Search Result\nName    : %s\n(not found)\n\n", name); return; }
    for (int i = 0; i < m; i++) {
        Student* s = &a[idx[i]];
        printf("Search Result\nID      : %d\nName    : %s\nKorean  : %d\nEnglish : %d\nMath    : %d\nTotal   : %d\n\n",
            s->id, s->name, s->kor, s->eng, s->math, s->total);
    }
}
static int id_cmp(const void* x, const void* y) { return *(const int*)x - *(const int*)y; }

int main(int argc, char** argv) {
    const char* path = argc > 1 ? argv[1] : "dataset_id_ascending (1).csv";
    char query[64] = "Liam";
    if (argc > 2) { strncpy(query, argv[2], 63); query[63] = 0; }
    else { printf("Search name: "); if (scanf("%63s", query) != 1) strcpy(query, "Liam"); }
    Student* orig; int n = load(path, &orig);
    if (n <= 0) { fprintf(stderr, "cannot read %s\n", path); return 1; }
    Student* sorted = malloc(n * sizeof * sorted); memcpy(sorted, orig, n * sizeof * sorted);
    int* ix = malloc(n * sizeof(int)), * iy = malloc(n * sizeof(int));

    /* quick sort cost (once) */
    cmp_cnt = 0; quick_sort(sorted, 0, n - 1); long long qs_cost = cmp_cnt;
    printf("Students loaded: %d, Quick sort comparisons: %lld\n\n", n, qs_cost);

    /* demo search (both methods; results must agree) */
    int m1 = seq_search(orig, n, query, ix), m2 = bin_search(sorted, n, query, iy);
    print_result(orig, ix, m1, query);
    int ok = (m1 == m2);
    if (ok) { /* compare sets of IDs */
        int* a = malloc(m1 * sizeof(int)), * b = malloc(m1 * sizeof(int));
        for (int i = 0; i < m1; i++) { a[i] = orig[ix[i]].id; b[i] = sorted[iy[i]].id; }
        qsort(a, m1, sizeof(int), id_cmp); qsort(b, m1, sizeof(int), id_cmp);
        for (int i = 0; i < m1; i++) if (a[i] != b[i]) ok = 0;
        free(a); free(b);
    }
    printf("Sequential/Binary results identical: %s (%d match%s)\n\n", ok ? "YES" : "NO", m1, m1 == 1 ? "" : "es");

    /* query list: fixed seed, 80%% existing names, 20%% non-existent; K grows by prefix */
    int MAXK = 5000; srand(12345);
    char (*q)[64] = malloc(MAXK * 64);
    for (int i = 0; i < MAXK; i++) {
        if (rand() % 5 == 0) sprintf(q[i], "Nobody%d", rand() % 100000);
        else strcpy(q[i], orig[rand() % n].name);
    }
    int Ks[] = { 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000 }, nk = sizeof Ks / sizeof * Ks, ki = 0;
    long long seq_tot = 0, bin_tot = qs_cost; int mismatches = 0, breakeven = -1;
    printf("Comparison Results\n\nK      Sequential     Quick + Binary\n--------------------------------------\n");
    for (int k = 1; k <= MAXK; k++) {
        cmp_cnt = 0; int a = seq_search(orig, n, q[k - 1], ix); seq_tot += cmp_cnt;
        cmp_cnt = 0; int b = bin_search(sorted, n, q[k - 1], iy); bin_tot += cmp_cnt;
        if (a != b) mismatches++;
        if (breakeven < 0 && bin_tot <= seq_tot) breakeven = k;
        if (k == Ks[ki]) { printf("%-6d %-14lld %lld\n", k, seq_tot, bin_tot); ki++; }
    }
    printf("\nResult mismatches over %d searches: %d\n", MAXK, mismatches);
    printf("Break-even: Quick+Binary first becomes <= Sequential at K = %d\n", breakeven);
    return 0;
}
