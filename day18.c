#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <toylib.h>

typedef enum { PAIR, NUMBER } NODE_TYPE;
typedef array_type(char*) array_strings;

struct node {
    NODE_TYPE type;
    struct node *left;
    struct node *right;
    struct node *up;
    int number; };

toystack *stack;

struct node *convert(char *str)
{
    for (char *c = str; *c != '\0'; c++) {
        if ((*c >= '0') && (*c <= '9')) {
            struct node *n = malloc(sizeof(struct node));
            n->type = NUMBER;
            n->number = *c - '0';
            toystack_push(stack, n);
        }
        if (*c == ']') {
            struct node *r = toystack_pop(stack);
            struct node *l = toystack_pop(stack);
            struct node *n = malloc(sizeof(struct node));
            n->type = PAIR;
            n->left  = l;
            n->right = r;
            n->up = NULL;
            l->up = n;
            r->up = n;
            toystack_push(stack, n);
        }
    }
    return toystack_pop(stack);
}

void print_node_rec(struct node *n)
{
    switch (n->type) {
        case NUMBER:
            printf("%d", n->number);
            break;
        case PAIR:
            putchar('[');
            print_node_rec(n->left);
            putchar(',');
            print_node_rec(n->right);
            putchar(']');
            break;
    }
}

void print_node(struct node *n)
{
    print_node_rec(n);
    putchar('\n');
}

int is_num_pair(struct node *n)
{
    return (n->type == PAIR) && (n->left->type == NUMBER) && (n->right->type == NUMBER);
}

void explode_left (struct node *n, int number)
{
    while (n->up && (n == n->up->left)) {
        n = n->up;
    }

    if (n == NULL || n->up == NULL)
        return;

    n = n->up->left;

    while (n->type != NUMBER)
        n = n->right;

    n->number += number;
}

void explode_right (struct node *n, int number)
{
    while (n->up && (n == n->up->right)) {
        n = n->up;
    }

    if (n == NULL || n->up == NULL)
        return;

    n = n->up->right;

    while (n->type != NUMBER)
        n = n->left;

    n->number += number;
}

int explode (struct node *n, int depth)
{
    if (n == NULL)
        return 0;

    if (n->type == NUMBER)
        return 0;

    if ((depth > 4) && is_num_pair(n)) {
        int l = n->left->number;
        int r = n->right->number;

        n->type = NUMBER;
        n->number = 0;

        free(n->left);
        free(n->right);

        explode_left(n, l);
        explode_right(n, r);

        return 1;
    }

    if (explode(n->left, depth + 1) == 1)
        return 1;

    if (explode(n->right, depth + 1) == 1)
        return 1;

    return 0;

}

int split (struct node *n)
{
    if (n == NULL)
        return 0;

    if (n->type == NUMBER) {
        if (n->number >= 10) {
            n->type = PAIR;
            div_t q = div(n->number, 2);

            n->left  = malloc(sizeof(struct node));
            n->left->type = NUMBER;
            n->left->up = n;
            n->left->number = q.quot;

            n->right = malloc(sizeof(struct node));
            n->right->type = NUMBER;
            n->right->up = n;
            n->right->number = q.quot + q.rem;

            return 1;
        }
        else
            return 0;
    }

    if (split(n->left) == 1)
        return 1;

    if (split(n->right) == 1)
        return 1;

    return 0;
}

void reduce (struct node *n)
{
    int has_reduced = 1;

    while (has_reduced) {
        has_reduced = 0;
        while (explode(n, 1))
            has_reduced = 1;
        if (split(n))
            has_reduced = 1;
    }
}

struct node *add (struct node *l, struct node *r)
{
    struct node *n = malloc(sizeof(struct node));

    n->type = PAIR;
    n->left = l;
    n->right = r;
    n->up = NULL;
    n->left->up = n;
    n->right->up = n;

    reduce(n);

    return n;
}

int magnitude (struct node *n)
{
    if (n->type == NUMBER)
        return n->number;
    else
        return (3 * magnitude(n->left)) + (2 * magnitude(n->right));
}

int main()
{
    stack = toystack_new(50);

    FILE *fp = fopen("inputs/day18", "r");
    char buffer[100];
    array_strings sfnums;
    array_init(sfnums);

    while ((fscanf(fp, "%s", buffer)) != EOF) {
        char *s = malloc(100);
        strcpy(s, buffer);
        array_add(sfnums, char*, s);
    }

    fclose(fp);

    struct node *sum = convert(array_ref(sfnums, 0));
    for (int i = 1; i < array_size(sfnums); i++)
        sum = add(sum, convert(array_ref(sfnums, i)));

    printf("%d\n", magnitude(sum));

    int m, max = 0;

    for (int i = 0; i < array_size(sfnums) - 1; i++)
        for (int j = 1; j < array_size(sfnums); j++) {
            m = magnitude(add(convert(array_ref(sfnums, i)), convert(array_ref(sfnums, j))));
            if (m > max) max = m;
            m = magnitude(add(convert(array_ref(sfnums, j)), convert(array_ref(sfnums, i))));
            if (m > max) max = m;
        }

    printf("%d\n", max);

    exit(EXIT_SUCCESS);
}

