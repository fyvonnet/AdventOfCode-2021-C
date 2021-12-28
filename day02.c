#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <toylib.h>
#include <pcre.h>

struct command {
    enum { FORWARD, UP, DOWN } name;
    int x;
};

typedef struct { int aim, hpos, depth; } data;

typedef array_type(struct command *) array_commands;

char *get_match_string(char *str, int a, int b)
{
    size_t len = b - a;
    char *match = malloc(len + 1);

    memcpy(match, str + a, len);
    match[len] = '\0';

    return match;
}

void forw1(int x, data *d) { d->hpos += x; }
void down1(int x, data *d) { d->depth += x; }
void up1(int x, data *d) { d->depth -= x; }

void down2(int x, data *d) { d->aim += x; }
void up2(int x, data *d) { d->aim -= x; }
void forw2(int x, data *d)
{
    d->hpos += x;
    d->depth += (d->aim * x);
}

int run_part(array_commands commands,
        void (*forw) (int, data *),
        void (*up)   (int, data *),
        void (*down) (int, data *))
{
    data *d = malloc(sizeof(data));
    d->aim = d->hpos = d->depth = 0;

    for (int i = 0; i < array_size(commands); i++) {
        struct command *c = array_ref(commands, i);
        switch (c->name) {
            case FORWARD:
                forw(c->x, d);
                break;
            case DOWN:
                down(c->x, d);
                break;
            case UP:
                up(c->x, d);
                break;
        }
    }
    return (d->hpos * d->depth);
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

    printf("%d\n", run_part(commands, forw1, up1, down1));
    printf("%d\n", run_part(commands, forw2, up2, down2));

    exit(EXIT_SUCCESS);
}

