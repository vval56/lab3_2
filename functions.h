#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

int check(){
    char * input = (char*)calloc(11, sizeof(char));
    char *endptr;
    long number;

    while (1) {
        if (fgets(input, 11, stdin) == NULL) {
            printf("Ошибка ввода.\n");
            return 1;
        }

        errno = 0;
        number = strtol(input, &endptr, 10);

        if (endptr == input) {
            puts("Ошибка: введите целое число: ");
        } else if (*endptr != '\n' && *endptr != '\0') {
            puts("Ошибка: введены недопустимые символы: ");
        } else if (errno == ERANGE || number < INT_MIN || number > INT_MAX) {
            puts("Ошибка: число выходит за допустимый диапазон: ");
        } else {
            break;
        }
    }

    free(input);
    return (int)number;
}

typedef struct BytesOfInt{
    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;
    unsigned char byte4;
} BytesOfInt;

typedef struct Number{
    int number;
    struct BytesOfInt bytes;
} Number;

void task1(){
    int choice;
    do{
        puts("задание 1.\nВведите число");
        Number * input_number = (Number*)calloc(1, sizeof(Number));
        input_number->number = check();
        printf("%d\n", input_number->number);

        input_number->bytes.byte1 = (input_number->number >> 0) & 0xFF;
        input_number->bytes.byte2 = (input_number->number >> 8) & 0xFF;
        input_number->bytes.byte3 = (input_number->number >> 16) & 0xFF;
        input_number->bytes.byte4 = (input_number->number >> 24) & 0xFF;

        printf("1-й байт: %d\n", input_number->bytes.byte1);
        printf("2-й байт: %d\n", input_number->bytes.byte2);
        printf("3-й байт: %d\n", input_number->bytes.byte3);
        printf("4-й байт: %d\n", input_number->bytes.byte4);

        puts("Желаете продолжить?\nНажмите '1', чтобы продолжить.");
        choice = check();
        free(input_number);
    } while(choice == 1);
}

