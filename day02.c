#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <toylib.h>
#include <pcre.h>

struct command {
    enum { FORWARD, UP, DOWN } name;
    int x;
};

typedef array_type(struct command *) array_commands;

char *get_match_string(char *str, int a, int b)
{
    size_t len = b - a;
    char *match = malloc(len + 1);

    memcpy(match, str + a, len);
    match[len] = '\0';

    return match;
}

int main()
{
    FILE *fp = fopen("inputs/day02", "r");
    static char *pattern = "^(\\w+) (\\d+)$";
    const char *err_msg;
    int err;
    pcre *re = pcre_compile(pattern, 0, &err_msg, &err, NULL); 
    size_t n = 0;
    char *buffer = NULL;
    int offsets[15];
    array_commands commands;

    array_init(commands);

    while (getline(&buffer, &n, fp) != EOF) {
        pcre_exec(re, NULL, buffer, strlen(buffer), 0, 0, offsets, 15);
        struct command *c = malloc(sizeof(struct command));
        char *name = get_match_string(buffer, offsets[2], offsets[3]);
        if (!strcmp(name, "forward"))
            c->name = FORWARD;
        else if (!strcmp(name, "up"))
            c->name = UP;
        else if (!strcmp(name, "down"))
            c->name = DOWN;
        else 
            fprintf(stderr, "Unknown command name: %s\n", name);
        c->x = atoi(get_match_string(buffer, offsets[4], offsets[5]));
        array_add(commands, struct command *, c);
    }

    int hpos = 0, depth = 0;

    for (int i = 0; i < array_size(commands); i++) {
        struct command *c = array_ref(commands, i);
        switch (c->name) {
            case FORWARD:
                hpos += c->x;
                break;
            case DOWN:
                depth += c->x;
                break;
            case UP:
                depth -= c->x;
                break;
        }
    }

    printf("%d\n", hpos * depth);

    int aim = 0;
    hpos = 0;
    depth = 0;

    for (int i = 0; i < array_size(commands); i++) {
        struct command *c = array_ref(commands, i);
        switch (c->name) {
            case DOWN:
                aim += c->x;
                break;
            case UP:
                aim -= c->x;
                break;
            case FORWARD:
                hpos += c->x;
                depth += (aim * c->x);
                break;
        }
    }

    printf("%d\n", hpos * depth);


    exit(EXIT_SUCCESS);
}

