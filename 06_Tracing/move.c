#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define EX_USAGE 1
#define EX_STAT 2
#define EX_SAME 3
#define EX_OPEN_IN 4
#define EX_OPEN_OUT 5
#define EX_OOM 6
#define EX_READ 7
#define EX_WRITE 8
#define EX_CLOSE 9
#define EX_UNLINK 10

int main(int argc, char **argv) {
    const char *progname = argv[0];
    if (argc != 3) {
        fprintf(stderr, "%s: Usage: %s infile outfile\n", progname, progname);
        return EX_USAGE;
    }

    const char *inpath = argv[1];
    const char *outpath = argv[2];

    struct stat st_in;
    if (stat(inpath, &st_in) != 0) {
        fprintf(stderr, "%s: Failed to stat infile\n", progname);
        return EX_STAT;
    }

    struct stat st_out;
    if (stat(outpath, &st_out) == 0 && st_out.st_ino == st_in.st_ino) {
        fprintf(stderr, "%s: Infile and outfile are the same file\n", progname);
        return EX_SAME;
    }

    int in_fd = open(inpath, O_RDONLY);
    if (in_fd < 0) {
        fprintf(stderr, "%s: Failed to open infile\n", progname);
        return EX_OPEN_IN;
    }

    int out_fd = open(outpath, O_WRONLY | O_CREAT | O_TRUNC, st_in.st_mode);
    if (out_fd < 0) {
        fprintf(stderr, "%s: Failed to open outfile\n", progname);
        close(in_fd);
        unlink(outpath);
        return EX_OPEN_OUT;
    }

    size_t file_size = st_in.st_size;
    char *buf = malloc(file_size);
    if (!buf) {
        fprintf(stderr, "%s: Failed to allocate %zd bytes for infile contents\n", progname, file_size);
        close(in_fd);
        unlink(outpath);
        close(out_fd);
        return EX_OOM;
    }

    ssize_t r = 0;
    size_t got = 0;
    while (got < file_size) {
        r = read(in_fd, buf + got, (size_t)(file_size - got));
        if (r < 0) {
            fprintf(stderr, "%s: Failed to read from infile\n", progname);
            free(buf);
            close(in_fd);
            unlink(outpath);
            close(out_fd);
            return EX_READ;
        }
        got += (size_t)r;
    }

    ssize_t w = 0;
    size_t left = file_size;
    while (left) {
        w = write(out_fd, buf + file_size - left, left);
        if (w < 0) {
            fprintf(stderr, "%s: Failed to write to outfile\n", progname);
            free(buf);
            close(in_fd);
            unlink(outpath);
            close(out_fd);
            return EX_WRITE;
        }
        left -= (size_t)w;
    }

    free(buf);

    if (close(out_fd) == -1) {
        fprintf(stderr, "%s: Failed to close outfile\n", progname);
        unlink(outpath);
        close(in_fd);
        return EX_CLOSE;
    }

    close(in_fd);
    if (unlink(inpath) != 0) {
        fprintf(stderr, "%s: Failed to unlink infile\n", progname);
        unlink(outpath);
        return EX_UNLINK;
    }

    return 0;
}
