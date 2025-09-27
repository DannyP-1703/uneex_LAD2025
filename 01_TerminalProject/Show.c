#include <curses.h>
#include <locale.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define PAD_X 2
#define PAD_Y 2

#define KEY_ESC 27

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments: expected 1, got %d", argc - 1);
        return 1;
    }

    char *filename = argv[1];
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Cannot open provided file");
        return 2;
    }

    // setup windows
    WINDOW *box_win, *text_win;
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    // setup box
    box_win = newwin(LINES - 2 * PAD_Y, COLS - 2 * PAD_X, PAD_Y, PAD_X);
    box(box_win, 0, 0);
    mvwaddstr(box_win, 0, 2, filename);
    wrefresh(box_win);

    // setup text frame
    int text_win_line_capacity = LINES - 2 * PAD_Y - 2;
    int text_win_col_capacity = COLS - 2 * PAD_X - 2;
    text_win = newwin(text_win_line_capacity, text_win_col_capacity, PAD_Y + 1, PAD_X + 1);
    keypad(text_win, TRUE);
    scrollok(text_win, TRUE);

    // fill in text frame
    int array_capacity = text_win_line_capacity;
    int loaded_lines = 0;
    char **file_contents_array = malloc(array_capacity * sizeof(char *));
    char buffer[1024];
    while (loaded_lines < text_win_line_capacity && fgets(buffer, sizeof(buffer), file)) {
        if (loaded_lines >= array_capacity) {
            array_capacity *= 2;
            char **tmp = realloc(file_contents_array, array_capacity * sizeof(char *));
            file_contents_array = tmp;
        }
        buffer[strcspn(buffer, "\n\r")] = '\0';
        file_contents_array[loaded_lines] = strndup(buffer, text_win_col_capacity - 1);
        if (loaded_lines == 0) {
            wprintw(text_win, "%s", file_contents_array[loaded_lines]);
        } else {
            wprintw(text_win, "\n%s", file_contents_array[loaded_lines]);
        }
        loaded_lines++;
    }

    // control text frame
    int first_line_indx = 0;
    int input_char;
    while ((input_char = wgetch(text_win)) != KEY_ESC) {
        switch (input_char) {
        case KEY_UP:
            if (first_line_indx > 0) {
                first_line_indx--;
                wscrl(text_win, -1);
                mvwaddstr(text_win, 0, 0, file_contents_array[first_line_indx]);
            }
            break;

        case KEY_DOWN:
        case ' ':
            if (first_line_indx + text_win_line_capacity >= loaded_lines) {
                if (fgets(buffer, sizeof(buffer), file)) {
                    if (loaded_lines >= array_capacity) {
                        array_capacity *= 2;
                        char **tmp = realloc(file_contents_array, array_capacity * sizeof(char *));
                        file_contents_array = tmp;
                    }
                    // buffer[text_win_col_capacity - 1] = '\0';
                    buffer[strcspn(buffer, "\n\r")] = '\0';
                    file_contents_array[loaded_lines] = strndup(buffer, text_win_col_capacity - 1);
                    loaded_lines++;
                }
            }
            if (first_line_indx + text_win_line_capacity < loaded_lines) {
                wscrl(text_win, 1);
                mvwaddstr(text_win, text_win_line_capacity - 1, 0, file_contents_array[first_line_indx + text_win_line_capacity]);
                first_line_indx++;
            }
            break;

        default:
            break;
        }
    }

    // cleanup
    for (int i = 0; i != loaded_lines; ++i) {
        free(file_contents_array[i]);
    }
    free(file_contents_array);
    fclose(file);
    delwin(text_win);
    delwin(box_win);
    endwin();
    return 0;
}