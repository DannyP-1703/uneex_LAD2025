#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXGR 10

typedef struct String {
    char *data;
    size_t capacity;
    size_t size;
} String;

int init_string(String *string, size_t cap) {
    string->capacity = cap;
    string->size = 0;
    string->data = malloc(cap);
    if (!string->data) {
        return 1;
    }
    return 0;
}

int append(String *string, char c) {
    if (string->size >= string->capacity) {
        string->capacity *= 2;
        string->data = realloc(string->data, string->capacity);
        if (!string->data) {
            return 1;
        }
    }
    string->data[string->size++] = c;
    return 0;
}

int extend(String *string, char *other, size_t n) {
    if (n <= 0) {
        return 0;
    }
    if (string->size + n >= string->capacity) {
        string->capacity += n;
        string->data = realloc(string->data, string->capacity);
        if (!string->data) {
            return 1;
        }
    }
    memcpy(string->data + string->size, other, n);
    string->size += n;
    return 0;
}

void free_string(String *string) {
    free(string->data);
}

void out_of_memory(char *prog_name) {
    fprintf(stderr, "%s: Out of memory\n", prog_name);
    exit(3);
}

int main(int argc, char *argv[]) {
    char *prog_name = argv[0];
    if (argc != 4) {
        fprintf(stderr, "%s: Usage: %s <regexp> <substitution> <string>\n", prog_name, prog_name);
        return 1;
    }
    char *regexp_str = argv[1];
    char *sub = argv[2];
    char *str = argv[3];

    regex_t regex;
    int errcode;
    if (errcode = regcomp(&regex, regexp_str, REG_EXTENDED)) {
        size_t errbuf_size = regerror(errcode, &regex, NULL, 0);
        char *errbuf = (char *)malloc(errbuf_size);
        regerror(errcode, &regex, errbuf, errbuf_size);
        fprintf(stderr, "%s: %.*s\n", prog_name, (int)errbuf_size, errbuf);
        free(errbuf);
        return 2;
    }

    regmatch_t bags[MAXGR];
    size_t bags_cnt = regex.re_nsub + 1 > MAXGR ? MAXGR : regex.re_nsub + 1;

    if (regexec(&regex, str, bags_cnt, bags, 0)) {
        fprintf(stderr, "%s: Failed to match a string\n", prog_name);
        return 5;
    }

    String new_string;
    if (init_string(&new_string, bags[0].rm_eo))
        out_of_memory(prog_name);

    if (extend(&new_string, str, bags[0].rm_so))
        out_of_memory(prog_name);

    int is_special_seq = 0;
    size_t sub_len = strlen(sub);
    size_t str_len = strlen(str);
    for (size_t i = 0; i < sub_len; ++i) {
        if (!is_special_seq) {
            if (sub[i] != '\\') {
                if (append(&new_string, sub[i]))
                    out_of_memory(prog_name);
            } else {
                is_special_seq = 1;
            }
        } else {
            if (sub[i] == '\\') {
                if (append(&new_string, sub[i]))
                    out_of_memory(prog_name);
            } else if ('1' <= sub[i] && sub[i] <= '9') {
                unsigned char bag_num = sub[i] - '0';
                if (bag_num >= bags_cnt) {
                    fprintf(stderr, "%s: Invalid back reference: '\\%c'\n", prog_name, sub[i]);
                    return 4;
                }
                if (extend(&new_string, str + bags[bag_num].rm_so, bags[bag_num].rm_eo - bags[bag_num].rm_so))
                    out_of_memory(prog_name);
            } else {
                fprintf(stderr, "%s: Invalid escape sequence: '\\%c'\n", prog_name, sub[i]);
                return 6;
            }
            is_special_seq = 0;
        }
    }

    if (extend(&new_string, str + bags[0].rm_eo, str_len - bags[0].rm_eo + 1))
        out_of_memory(prog_name);

    puts(new_string.data);

    free_string(&new_string);
    regfree(&regex);
    return 0;
}