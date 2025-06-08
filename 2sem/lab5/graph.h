#ifndef GRAPH
#define GRAPH

enum CellType {
    COMMON = 0,
    ENTER = 1,
    EXIT = 2
};

typedef struct Point {
	unsigned int x;
	unsigned int y;
} Point;

typedef struct Vertex {
    unsigned int id;
    Point point;
    enum CellType type;
} Vertex;

typedef struct AdjList {
    Vertex *vertex;
    unsigned int adjVertNum;
    Vertex **adjVert;
    struct AdjList *next;
} AdjList;

typedef struct Graph {
    unsigned int verticesNum;
    AdjList *adjList;
} Graph;


void add_vertex(Graph *graph, Point point, enum CellType type);
int add_edge(Graph *graph, Point point1, Point point2);
void add_all_possible_edges(Graph *graph);
void print_adj_matrix(Graph *graph);
int remove_edge(Graph *graph, Point point1, Point point2);
int remove_vertex(Graph *graph, Point point);
void change_vertex_type(Graph *graph, Point point, enum CellType newType);
void bfs(Graph *graph, Point point);
int bellman_ford(Graph *graph, Point point1, Point point2);
AdjList *find_closest_exit(Graph *graph, Point point);
void delete_graph(Graph *graph);

#endif
