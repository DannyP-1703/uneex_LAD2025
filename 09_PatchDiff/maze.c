#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ERR_OOM 1

// up, down, left, right
int r_off[4] = {1, -1, 0, 0};
int c_off[4] = {0, 0, -1, 1};

char WALL = '#';
char PASS = '.';

char **init_maze(size_t n, size_t size) {
    char **maze = malloc(size * sizeof(*maze));
    if (!maze)
        return NULL;
    maze[0] = malloc(size * (size + 1) * sizeof(**maze));
    if (!maze[0])
        return NULL;
    for (int i = 1; i < size; i++)
        maze[i] = maze[0] + i * (size + 1);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
            maze[i][j] = WALL;
        maze[i][size] = '\0';
    }
    for (int r = 0; r < n; ++r)
        for (int c = 0; c < n; ++c)
            maze[2 * r + 1][2 * c + 1] = PASS;
    return maze;
}

char **init_visited(size_t n) {
    char **visited = malloc(n * sizeof(*visited));
    if (!visited)
        return NULL;
    visited[0] = calloc(n * n, sizeof(**visited));
    if (!visited[0])
        return NULL;
    for (int i = 1; i < n; i++)
        visited[i] = visited[0] + i * n;
    return visited;
}

void free_grid(char **grid) {
    free(grid[0]);
    free(grid);
}

void shuffle(int *a, int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
}

void dfs(int r, int c, char **maze, char **visited, size_t n) {
    visited[r][c] = 1;

    int directions[4] = {0, 1, 2, 3};
    shuffle(directions, 4);

    for (int i = 0; i < 4; ++i) {
        int k = directions[i];
        int r_next = r + r_off[k];
        int c_next = c + c_off[k];
        if (r_next < 0 || r_next >= n || c_next < 0 || c_next >= n)
            continue;
        if (!visited[r_next][c_next]) {
            int r_wall = 2 * r + 1 + r_off[k];
            int c_wall = 2 * c + 1 + c_off[k];
            maze[r_wall][c_wall] = PASS;
            dfs(r_next, c_next, maze, visited, n);
        }
    }
}

int main(void) {
    srand((unsigned)time(NULL));

    size_t N = 6;
    size_t SIZE = 2 * N + 1;

    char **maze = init_maze(N, SIZE);
    if (!maze) {
        fprintf(stderr, "Out of memory\n");
        return ERR_OOM;
    }

    char **visited = init_visited(N);
    if (!visited) {
        fprintf(stderr, "Out of memory\n");
        return ERR_OOM;
    }

    int r_start = rand() % N;
    int c_start = rand() % N;
    dfs(r_start, c_start, maze, visited, N);

    for (int i = 0; i < SIZE; ++i) {
        puts(maze[i]);
    }

    free_grid(maze);
    free_grid(visited);

    return 0;
}
