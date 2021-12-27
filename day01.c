#include <stdlib.h>
#include <stdio.h>
#include <toylib.h>

typedef array_type(int) array_int;

int count_increases(array_int array)
{
    int counter = 0;

    for (int i = 0; i < array_size(array) - 1; i++)
        if (array_ref(array, i) < array_ref(array, i + 1))
            counter++;

    return counter;
}

int main()
{
    array_int input;
    array_init(input);

    int buffer;

    FILE *fp=fopen("inputs/day01", "r");

    while (fscanf(fp, "%d", &buffer) != EOF)
        array_add(input, int, buffer);

    fclose(fp);

    printf("%d\n", count_increases(input));

    array_int sliding_window;
    array_init(sliding_window);

    for (int i = 0; i < array_size(input) - 2; i++) {
        int value = 0;
        for (int j = 0; j < 3; j++)
            value += array_ref(input, i + j);
        array_add(sliding_window, int, value);
    }

    printf("%d\n", count_increases(sliding_window));

    exit(EXIT_SUCCESS);
}

