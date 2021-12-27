#include <stdlib.h>
#include <stdio.h>
#include <toylib.h>

typedef array_type(int) int_array;

int main() {
    int_array positions;
    array_init(positions);

    FILE *fp = fopen("inputs/day07", "r");
    char c = fgetc(fp);
    while (c != 255) {
        int n = 0;
        while ((c >= 48) && (c <= 57)) {
            n = (n * 10) + (c - 48);
            c = fgetc(fp);
        }
        array_add(positions, int, n);
        n = 0;
        c = fgetc(fp);
    }
    fclose(fp);

    int max_pos = 0;
    for (int i = 0; i < array_size(positions); i++) {
        int p = array_ref(positions, i);
        if (p > max_pos) max_pos = p;
    }

    int min_fuel1 = -1, min_fuel2 = -1;
    for (int new_pos = 0; new_pos <= max_pos; new_pos++) {
        int sum_fuel1 = 0, sum_fuel2 = 0;
        for (int i = 0; i < array_size(positions); i++) {
            int gap = abs(array_ref(positions, i) - new_pos);
            sum_fuel1 += gap;
            sum_fuel2 += (gap * (gap + 1) / 2);
        }
        if ((min_fuel1 == -1) || (min_fuel1 > sum_fuel1))
            min_fuel1 = sum_fuel1;
        if ((min_fuel2 == -1) || (min_fuel2 > sum_fuel2))
            min_fuel2 = sum_fuel2;
    }

    printf("%d\n%d\n", min_fuel1, min_fuel2);

    exit(EXIT_SUCCESS);
}
