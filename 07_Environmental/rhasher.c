#include "rhash.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifdef USE_READLINE
#include <readline/history.h>
#include <readline/readline.h>
#endif

#define ERR_OOM 1

int main(int argc, char *argv[]) {
    unsigned char digest[64];
    char output[130];

    rhash_library_init();

    while (1) {
        char *input_buf = NULL;

#ifdef USE_READLINE
        input_buf = readline("> ");
        if (input_buf == NULL) {
            puts("");
            break;
        }
        if (input_buf[0] != '\0') {
            add_history(input_buf);
        }
#else
        size_t input_len = 0;
        printf("> ");
        if (getline(&input_buf, &input_len, stdin) == -1) {
            puts("");
            break;
        }
#endif

        // parse algo string
        char *algo = strtok(input_buf, " \t\r\n");
        if (!algo) {
            fprintf(stderr, "Empty input line\n");
            continue;
        }
        size_t algo_len = strlen(algo);
        if (algo_len == 0) {
            fprintf(stderr, "Empty algo name\n");
            continue;
        }
        int is_hex_digest = 0;
        if ('A' <= algo[0] && algo[0] <= 'Z') {
            is_hex_digest = 1;
        }

        // parse input message
        char *msg = strtok(NULL, " \t\r\n");
        if (!msg) {
            fprintf(stderr, "No message provided\n");
            continue;
        }
        int is_file = 1;
        if (msg[0] == '"') {
            is_file = 0;
            msg++;
        }
        size_t msg_len = strlen(msg);
        if (msg_len == 0) {
            fprintf(stderr, "Empty message or file name\n");
            continue;
        }

        // determine algo
        for (int i = 0; algo[i]; i++) {
            algo[i] = tolower(algo[i]);
        }
        unsigned hash_id = 0;
        if (strcmp(algo, "sha1") == 0) {
            hash_id = RHASH_SHA1;
        } else if (strcmp(algo, "md5") == 0) {
            hash_id = RHASH_MD5;
        } else if (strcmp(algo, "tth") == 0) {
            hash_id = RHASH_TTH;
        } else {
            fprintf(stderr, "Unsupported algorithm: %s\n", algo);
            continue;
        }

        // calculate digest
        if (!is_file) {
            int res = rhash_msg(hash_id, msg, msg_len, digest);
            if (res < 0) {
                fprintf(stderr, "Message digest calculation error\n");
                continue;
            }
        } else {
            int res = rhash_file(hash_id, msg, digest);
            if (res < 0) {
                fprintf(stderr, "File digest calculation error: %s\n", strerror(errno));
                continue;
            }
        }

        // print hash
        rhash_print_bytes(
            output,
            digest,
            rhash_get_digest_size(hash_id),
            (is_hex_digest ? RHPR_HEX : RHPR_BASE64));
        puts(output);

        free(input_buf);
    }
    return 0;
}