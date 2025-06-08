#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "graph.h"
#include "menu.h"

int add_vertex_action(Graph *graph){
    unsigned int x, y, cellType;

    printf("Enter [x y cellType]:\n(cellTypes: 0 - common, 1 - enter, 2 - exit)\n");
    if (read_three_ui_lab5(&x, &y, &cellType) == -1){
        return -1;
    }
    Point point;
    point.x = x;
    point.y = y;

    add_vertex(graph, point, cellType);
    return 0;
}

int add_edge_action(Graph *graph){
    unsigned int x1, y1, x2, y2;

    printf("Enter coordinates of two vertices [x1 y1 x2 y2]:\n");
    if (read_four_ui_lab5(&x1, &y1, &x2, &y2) == -1){
        return -1;
    }

    Point point1, point2;
    point1.x = x1;
    point1.y = y1;
    point2.x = x2;
    point2.y = y2;

    add_edge(graph, point1, point2);
    return 0;
}

int add_all_possible_edges_action(Graph *graph){
    add_all_possible_edges(graph);

    return 0;
}

int print_adj_matrix_action(Graph *graph){
    print_adj_matrix(graph);
    return 0;
}

int remove_vertex_action(Graph *graph){
    unsigned int x, y;

    printf("Enter [x y]:\n");
    if (read_two_ui_lab5(&x, &y) == -1){
        return -1;
    }

    Point point;
    point.x = x;
    point.y = y;


    remove_vertex(graph, point);
    return 0;
}

int remove_edge_action(Graph *graph){
    unsigned int x1, y1, x2, y2;

    printf("Enter coordinates of two vertices [x1 y1 x2 y2]:\n");
    if (read_four_ui_lab5(&x1, &y1, &x2, &y2) == -1){
        return -1;
    }

    Point point1, point2;
    point1.x = x1;
    point1.y = y1;
    point2.x = x2;
    point2.y = y2;

    remove_edge(graph, point1, point2);
    return 0;
}

int change_vertex_type_action(Graph *graph){
    unsigned int x, y, cellType;

    printf("Enter coordinates of an old vertex and new cellType [x y cellType]:\n(cellTypes: 0 - common, 1 - enter, 2 - exit)\n");
    if (read_three_ui_lab5(&x, &y, &cellType) == -1){
        return -1;
    }

    Point point;
    point.x = x;
    point.y = y;

    change_vertex_type(graph, point, cellType);
    return 0;
}

int bfs_action(Graph *graph){
    unsigned int x, y;

    printf("Enter [x y] of the chosen enter:\n");
    if (read_two_ui_lab5(&x, &y) == -1){
        return -1;
    }

    Point point;
    point.x = x;
    point.y = y;

    bfs(graph, point);
    return 0;
}

int bellman_ford_action(Graph *graph){
    unsigned int x1, y1, x2, y2;

    printf("Enter coordinates of enter and exit vertices [x1 y1 x2 y2]:\n");
    if (read_four_ui_lab5(&x1, &y1, &x2, &y2) == -1){
        return -1;
    }

    Point point1, point2;
    point1.x = x1;
    point1.y = y1;
    point2.x = x2;
    point2.y = y2;

    bellman_ford(graph, point1, point2);
    return 0;
}

int find_closest_exit_action(Graph *graph){
    unsigned int x, y;

    printf("Enter coordinates of enter [x y]:\n");
    if (read_two_ui_lab5(&x, &y) == -1){
        return -1;
    }

    Point point;
    point.x = x;
    point.y = y;

    find_closest_exit(graph, point);
    return 0;
}

int exit_action(Graph *graph){
    delete_graph(graph);
    return 1;
}


MenuAction menu_actions[] = {
        exit_action,
        add_vertex_action,
        add_edge_action,
        add_all_possible_edges_action,
        print_adj_matrix_action,
        remove_vertex_action,
        remove_edge_action,
        change_vertex_type_action,
        bfs_action,
        bellman_ford_action,
        find_closest_exit_action
};

void show_menu(){
    printf("\n1. Add vertex\n");
    printf("2. Add edge\n");
    printf("3. Add all possible edges\n");
    printf("4. Print adjacency matrix\n");
    printf("5. Remove vertex\n");
    printf("6. Remove edge\n");
    printf("7. Change type of the vertex\n");
    printf("8. BFS (find exit)\n");
    printf("9. Bellman-Ford (find shortest way between enter and exit)\n");
    printf("10. Floyd-Warshall (find nearest exit and its remoteness)\n");
    printf("0. Exit\n\n");
    printf("Enter your choice:\n");
}

int menu(){
    Graph *graph = malloc(sizeof(Graph));
    graph->verticesNum = 0;
    graph->adjList = NULL;

    int choice = -1, check = 0;

    do {
        show_menu();

        if (read_input(&choice) == -1){
            delete_graph(graph);
            return -1;
        }
        if (choice < 0 || choice > 10) {
            printf("Invalid choice! Please input a number once more\n");
            continue;
        }

        check = menu_actions[choice](graph);
        if (check != 0){
            return check;
        }

    } while (1);
}
