#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph.h"
#include "queue.h"

AdjList *find_vert(Graph *graph, Point point){
    AdjList *current_adj_list = graph->adjList;
    while (current_adj_list != NULL){
        if (current_adj_list->vertex->point.x == point.x && current_adj_list->vertex->point.y == point.y){
            return current_adj_list;
        }
        current_adj_list = current_adj_list->next;
    }
    return NULL;
}

void add_vertex(Graph *graph, Point point, enum CellType type){
    if (find_vert(graph, point) != NULL){
        printf("You're trying to add an existing vertex!\n");
        return;
    }
    Vertex *new_vertex = malloc(sizeof(Vertex));
    new_vertex->id = graph->verticesNum;
    new_vertex->point.x = point.x;
    new_vertex->point.y = point.y;
    new_vertex->type = type;

    AdjList *new_adj_list = malloc(sizeof(AdjList));
    new_adj_list->vertex = new_vertex;
    new_adj_list->adjVert = NULL;
    new_adj_list->adjVertNum = 0;
    new_adj_list->next = NULL;

    if (graph->adjList == NULL){
        graph->adjList = new_adj_list;
    } else {
        AdjList *current_adj_list = graph->adjList;
        while (current_adj_list->next != NULL){
            current_adj_list = current_adj_list->next;
        }
        current_adj_list->next = new_adj_list;
    }
    graph->verticesNum++;
}

int add_edge(Graph *graph, Point point1, Point point2){
	int x1 = point1.x, x2 = point2.x, y1 = point1.y, y2 = point2.y;
    int vertexesEqual = x1 == x2 && y1 == y2;
    int vertexesDistant = abs(x1 - x2) > 1 || abs(y1 - y2) > 1;
    int vertexesDiagonal = (abs(x1 - x2) == 1 && abs(y1 - y2) == 1);
    if (vertexesEqual || vertexesDistant || vertexesDiagonal){
        printf("These vertexes can't be connected!\n");
        return -1;
    }

    // Находим вершины, которые нужно соединить
    AdjList *adj_list1 = find_vert(graph, point1);
    AdjList *adj_list2 = find_vert(graph, point2);

    if (adj_list1 == NULL || adj_list2 == NULL){
        printf("There is no such vertex!\n");
        return -1;
    }

    // Добавляем взаимные ссылки на списки смежности для обеих вершин
    adj_list1->adjVert = realloc(adj_list1->adjVert, (adj_list1->adjVertNum + 1) * sizeof(Vertex *));
    adj_list2->adjVert = realloc(adj_list2->adjVert, (adj_list2->adjVertNum + 1) * sizeof(Vertex *));
    adj_list1->adjVert[adj_list1->adjVertNum] = adj_list2->vertex;
    adj_list1->adjVertNum++;
    adj_list2->adjVert[adj_list2->adjVertNum] = adj_list1->vertex;
    adj_list2->adjVertNum++;
    return 0;
}

void add_all_possible_edges(Graph *graph){
    int visited = 0;
    Point point1, point2;
    AdjList *current_adj_list = graph->adjList;
    while (current_adj_list != NULL){
        Vertex *current_vertex = current_adj_list->vertex;
        AdjList *adj_list = graph->adjList;
        for (int i = 0; i < visited; i++){
            adj_list = adj_list->next;
        }
        while (adj_list != NULL){
            Vertex *vertex = adj_list->vertex;
            if (current_vertex != vertex){
            	point1.x = current_vertex->point.x;
            	point1.y = current_vertex->point.y;
            	point2.x = vertex->point.x;
            	point2.y = vertex->point.y;
                add_edge(graph, point1, point2);
            }
            adj_list = adj_list->next;
        }
        current_adj_list = current_adj_list->next;
        visited++;
    }
}

void print_adj_matrix(Graph *graph){
    int num_vertices = graph->verticesNum;

    int **adj_matrix = calloc(num_vertices, sizeof(int *));
    for (int i = 0; i < num_vertices; i++){
        adj_matrix[i] = calloc(num_vertices, sizeof(int));
    }

    AdjList *curr_list = graph->adjList;
    for (int i = 0; i < num_vertices; i++){
        if (curr_list->adjVertNum != 0){
            for (int j = 0; j < curr_list->adjVertNum; j++){
                adj_matrix[i][curr_list->adjVert[j]->id] = 1;
            }
        }
        curr_list = curr_list->next;
    }

    curr_list = graph->adjList;
    for (int i = 0; i < num_vertices; i++){
        printf("%2u:%-2u(%d)|", curr_list->vertex->point.x, curr_list->vertex->point.y, curr_list->vertex->type);
        curr_list = curr_list->next;
    }
    printf("\n");

    for (int i = 0; i < num_vertices; i++){
        printf("  ");
        for (int j = 0; j < num_vertices; j++){
            printf("%-6d|  ", adj_matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < num_vertices; i++){
        free(adj_matrix[i]);
    }
    free(adj_matrix);
}

int shift_adj_vert(AdjList *adjList, int index){
    // for (int i = index; i < adjList->adjVertNum - 1; i++){ // TODO think about it
        // adjList->adjVert[i] = adjList->adjVert[i + 1];
    // }
    adjList->adjVert[index] = adjList->adjVert[adjList->adjVertNum - 1];
    adjList->adjVertNum--;
    adjList->adjVert = realloc(adjList->adjVert, adjList->adjVertNum * sizeof(Vertex*));
    return 0;
}

int remove_edge(Graph *graph, Point point1, Point point2){
	int x1 = point1.x, y1 = point1.y, x2 = point2.x, y2 = point2.y;
    AdjList *adj_list1 = find_vert(graph, point1);
    AdjList *adj_list2 = find_vert(graph, point2);

    if (adj_list1 == NULL || adj_list2 == NULL){
        printf("There is no such vertex!\n");
        return -1;
    }

// Удаляем ссылки на вершины из списков смежности для обеих вершин
    int found1 = 0, found2 = 0;
    for (int i = 0; i < adj_list1->adjVertNum; i++){
        if (adj_list1->adjVert[i]->point.x == x2 && adj_list1->adjVert[i]->point.y == y2){
            shift_adj_vert(adj_list1, i);
            found1 = 1;
            break;
        }
    }
    for (int i = 0; i < adj_list2->adjVertNum; i++){
        if (adj_list2->adjVert[i]->point.x == x1 && adj_list2->adjVert[i]->point.y == y1){
            shift_adj_vert(adj_list2, i);
            found2 = 1;
            break;
        }
    }

    if (!found1 || !found2){
        printf("These vertexes are not connected!\n");
        return -1;
    }

    return 0;
}

int remove_vertex(Graph *graph, Point point){
    // Находим вершину, которую нужно удалить (c prev)
    AdjList *prev_adj_list = NULL;
    AdjList *current_adj_list = graph->adjList;
    while (current_adj_list != NULL){
        if (current_adj_list->vertex->point.x == point.x && current_adj_list->vertex->point.y == point.y){
            break;
        }
        prev_adj_list = current_adj_list;
        current_adj_list = current_adj_list->next;
    }

    if (current_adj_list == NULL){
        printf("There is no such vertex!\n");
        return -1;
    }

    // Удаляем ссылки на вершину из списков смежности для всех вершин
    AdjList *adj_list_to_remove = current_adj_list;
    current_adj_list = graph->adjList;
    for (int i = 0; i < graph->verticesNum; i++){
        for (int j = 0; j < adj_list_to_remove->adjVertNum; j++){
            if (adj_list_to_remove->adjVert[j] == current_adj_list->vertex){
                for (int k = 0; k < current_adj_list->adjVertNum; k++){
                    if (current_adj_list->adjVert[k] == adj_list_to_remove->vertex){
                        shift_adj_vert(current_adj_list, k);
                    }
                }
            }
        }
        current_adj_list = current_adj_list->next;
    }

    // Удаляем вершину из списка смежности
    if (prev_adj_list == NULL){
        graph->adjList = adj_list_to_remove->next;
    } else {
        prev_adj_list->next = adj_list_to_remove->next;
    }

    AdjList *decrease_list = adj_list_to_remove->next;

    // Уменьшаем индексы, большие удаленного
    while (decrease_list != NULL){
        (decrease_list->vertex->id)--;
        decrease_list = decrease_list->next;
    }

    free(adj_list_to_remove->vertex);
    free(adj_list_to_remove->adjVert);
    free(adj_list_to_remove);

    graph->verticesNum--;
    return 0;
}

void change_vertex_type(Graph *graph, Point point, enum CellType newType){
    AdjList *current_adj_list = find_vert(graph, point);
    if (current_adj_list == NULL){
        printf("There is no such vertex found!\n");
    } else {
        current_adj_list->vertex->type = newType;
    }
}

void bfs(Graph *graph, Point point){
    Queue *queue = create_queue();
    AdjList *current_adj_list = find_vert(graph, point);

    if (current_adj_list == NULL){
        return;
    } else if (current_adj_list->vertex->type != ENTER){
        printf("This vertex has the wrong type. It should be \"ENTER\".\n");
        return;
    }

    Vertex *start_vertex = current_adj_list->vertex;
    int *visited = calloc(graph->verticesNum, sizeof(int));

    visited[start_vertex->id] = 1;
    enqueue(queue, start_vertex);

    // Обходим граф в ширину
    while (!is_queue_empty(queue)){
        Vertex *current_vertex = dequeue(queue);
        printf("(%d, %d) ", current_vertex->point.x, current_vertex->point.y);

        // Перебираем все смежные вершины
        current_adj_list = graph->adjList;
        while (current_adj_list != NULL){
            if (visited[current_adj_list->vertex->id] == 0){
                for (int i = 0; i < current_adj_list->adjVertNum; i++){
                    if (current_adj_list->adjVert[i] == current_vertex){
                        if (current_adj_list->vertex->type == EXIT){
                            printf("\nExit found at (%d, %d)!\n", current_adj_list->vertex->point.x, current_adj_list->vertex->point.y);
                            free(visited);
                            destroy_queue(queue);
                            return;
                        }
                        visited[current_adj_list->vertex->id] = 1;
                        enqueue(queue, current_adj_list->vertex);
                        break;
                    }
                }
            }
            current_adj_list = current_adj_list->next;
        }
    }
    printf("\nExit not found!\n");
    free(visited);
    destroy_queue(queue);
}

int bellman_ford(Graph *graph, Point point1, Point point2){
	int x1 = point1.x, x2 = point2.x, y1 = point1.y, y2 = point2.y;
    int vertNum = graph->verticesNum;

    AdjList *enterAdjList = find_vert(graph, point1);
    AdjList *exitAdjList = find_vert(graph, point2);

    if (enterAdjList == NULL || exitAdjList == NULL){
        printf("There is no such vertex!\n");
        return -1;
    } else if (enterAdjList->vertex->type != ENTER || exitAdjList->vertex->type != EXIT){
        printf("The vertex type is wrong!\n");
        return -1;
    }

    unsigned int *dist = malloc(vertNum * sizeof(unsigned int));
    unsigned int *prev = malloc(vertNum * sizeof(unsigned int));

    int enterId = enterAdjList->vertex->id;
    int exitId = exitAdjList->vertex->id;

    // Инициализация расстояний до всех вершин графа как "бесконечность"
    for (int i = 0; i < vertNum; i++){
        dist[i] = UINT_MAX;
        prev[i] = UINT_MAX;
    }
    dist[enterId] = 0;

    // Проходим по всем вершинам граф V-1 раз
    for (int i = 0; i < vertNum - 1; i++){
        AdjList *adjList = graph->adjList;
        while (adjList != NULL){
            Vertex *curr_vert = adjList->vertex;
            for (int j = 0; j < adjList->adjVertNum; j++){
                Vertex *adj_vert = adjList->adjVert[j];
                int weight = 1; // Вес ребра всегда равен 1 в данном случае
                if (dist[curr_vert->id] != UINT_MAX && dist[curr_vert->id] + weight < dist[adj_vert->id]){
                    dist[adj_vert->id] = dist[curr_vert->id] + weight;
                    prev[adj_vert->id] = curr_vert->id;
                }
            }
            adjList = adjList->next;
        }
    }

    // Проверяем наличие отрицательных циклов
    AdjList *adjList = graph->adjList;
    while (adjList != NULL){
        Vertex *curr_vert = adjList->vertex;
        for (int i = 0; i < adjList->adjVertNum; i++){
            Vertex *adj_vert = adjList->adjVert[i];
            int weight = 1; // Вес ребра всегда равен 1 в данном случае
            if (dist[curr_vert->id] != UINT_MAX && dist[curr_vert->id] + weight < dist[adj_vert->id]){
                printf("Negative cycle is found!\n");
                return -1;
            }
        }
        adjList = adjList->next;
    }

    // Выводим кратчайший путь
    if (dist[exitId] == UINT_MAX){
        printf("There is no way from from vertex (%u, %u) to (%u, %u)!\n", x1, y1, x2, y2);
    } else {
        unsigned int *path = malloc(vertNum * sizeof(unsigned int));
        unsigned int curr = exitId, pathLen = 0;
        while (curr != enterId){
            path[pathLen++] = curr;
            curr = prev[curr];
        }
        path[pathLen++] = enterId;
        printf("The shortest way (in vert id) from vertex (%u, %u) to (%u, %u): ", x1, y1, x2, y2);
        for (int i = pathLen - 1; i > 0; i--){
            printf("%d -> ", path[i]);
        }
        printf("%d\n", path[0]);
        free(path);
    }

    free(dist);
    free(prev);
    return 0;
}

void floyd_warshall(Graph *graph, unsigned int **dist) {
    int vertNum = graph->verticesNum;
    AdjList *adjList = graph->adjList;
    Vertex *vertex = NULL, *adjVertex = NULL;

    for (int i = 0; i < vertNum; i++){
        vertex = adjList->vertex;
        for (int j = 0; j < adjList->adjVertNum; ++j){
            adjVertex = adjList->adjVert[j];
            dist[vertex->id][adjVertex->id] = 1;
        }
        adjList = adjList->next;
    }

    for (int k = 0; k < vertNum; k++) {
        for (int i = 0; i < vertNum; i++) {
            for (int j = 0; j < vertNum; j++) {
                if (dist[i][k] != UINT_MAX && dist[k][j] != UINT_MAX && dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

void delete_array(unsigned int **array, unsigned int len){
    for (int i = 0; i < len; i++) {
        free(array[i]);
    }
    free(array);
}

AdjList *find_closest_exit(Graph *graph, Point point) {
    unsigned int minDist = UINT_MAX, vertNum = graph->verticesNum;
    unsigned int **dist = malloc(vertNum * sizeof(unsigned int *));

    for (int i = 0; i < vertNum; i++) {
        dist[i] = malloc(vertNum * sizeof(unsigned int));
        memset(dist[i], UINT_MAX, vertNum * sizeof(unsigned int));
    }

    floyd_warshall(graph, dist);

    // Поиск кратчайшего пути для заданной вершины
    AdjList *adjList = find_vert(graph, point);
    if (adjList == NULL) {
        printf("There is no such vertex!\n");
        delete_array(dist, vertNum);
        return NULL;
    } else if (adjList->vertex->type != ENTER){
        printf("The vertex type is wrong!\n");
        delete_array(dist, vertNum);
        return NULL;
    }

    Vertex *vertex = adjList->vertex;
    AdjList *exitAdjList = NULL;
    AdjList *curr_adj_list = graph->adjList;

    for (int i = 0; i < vertNum; i++){
        if (curr_adj_list->vertex->type == EXIT){
            if (dist[vertex->id][i] < minDist) {
                minDist = dist[vertex->id][i];
                exitAdjList = curr_adj_list;
            }
        }
        curr_adj_list = curr_adj_list->next;
    }

    if (exitAdjList == NULL) {
        printf("No exit found!\n");
    } else {
        printf("The nearest exit is found at (%u, %u) at a distance of %u\n", exitAdjList->vertex->point.x, exitAdjList->vertex->point.y, minDist);
    }

    delete_array(dist, vertNum);
    return exitAdjList;
}

void delete_graph(Graph *graph){
    // Освобождаем память, занятую вершинами
    AdjList *currAdjList = graph->adjList;
    for (int i = 0; i < graph->verticesNum; i++){
        free(currAdjList->vertex);
        currAdjList = currAdjList->next;
    }

    // Освобождаем память, занятую списками смежности
    AdjList *current_adj_list = graph->adjList;
    AdjList *temp_adj_list = NULL;
    for (int i = 0; i < graph->verticesNum; i++){
        temp_adj_list = current_adj_list;
        current_adj_list = current_adj_list->next;
        free(temp_adj_list->adjVert);
        free(temp_adj_list);
    }

    free(graph);
}
