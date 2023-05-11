#include <stdio.h>
#include "reverse_array.h"

void reverse_array(uint8 data[], uint16 len)
{
    for (int i = 0; i < len / 2; i++)
    {
        char temp = data[i];
        data[i] = data[len - i - 1];
        data[len - i - 1] = temp;
    }
}

void print_data(uint8 data[], uint16 len)
{
    printf("data:");
    for (int i = 0; i < len; i++)
        printf("%02x", data[i]);
    printf("\n");
}