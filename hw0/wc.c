/*
 * Heavily inspired by
 * https://www.gnu.org/software/cflow/manual/html_node/Source-of-wc-command.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

typedef unsigned long count_t;

/* Counter for chars, words, linies */
count_t ccount, wcount, lcount;

void report(char *filename, count_t ccount, count_t wcount, count_t lcount) {
    if (filename) {
        printf("%lu %lu %lu %s\n", lcount, wcount, ccount, filename);
        return;
    }
    printf("%lu %lu %lu\n", lcount, wcount, ccount);
}

void count(char c) {
    ccount++;
    if (c == '\n')
        lcount++;
}

/* Get next word from input stream.
 * Returns 0 if reach end of line. Return 1 otherwise. */
int get_word(FILE *stream) {
    int c;

    if (feof(stream)) {
        return 0;
    }

    while ((c = getc(stream)) != EOF) {
        if (isalnum(c)) {
            wcount++;
            break;
        }
        count(c);
    }

    for (; c != EOF; c = getc(stream)) {
        count(c);
        if (!isalnum(c)) {
            break;
        }
    }

    return c != EOF;
}

void counter (FILE *stream, char *filename) {
    ccount = wcount = lcount = 0;

    if (!stream) {
        fprintf(stderr, "Cannot open file '%s'\n", filename);
        exit(1);
    }

    while(get_word(stream));

    fclose(stream);

    report(filename, ccount, wcount, lcount);
}

int main(int argc, char *argv[]) {
    FILE *stream;

    if (argc < 2) {
        stream = stdin;
        counter(stream, NULL);
    }
    else {
        char *filename = argv[1];
        stream = fopen(filename, "r");
        counter(stream, filename);
    }

    return 0;
}

