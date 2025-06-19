#include "byte_fields.h"

void press_enter_to_continue() {
    printf("\nНажмите Enter для продолжения...");
    getchar(); getchar();
}

int check(){
  	char s[20];
  	int i = 0;

  	while (1) {
        s[i] = getchar();
        if(s[i] == 10) break;
        if(s[i] >= '0' && s[i] <= '9'){	
            i++;
		}
  	}

  	s[i] = '\0';

  	return atoi(s);
}

void byte_fields(){
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
