#include "functions2.h"
#include "functions1.h"

double check_double() {
    char input[100];
    char *endptr;
    double number;

    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Ошибка ввода.\n");
            continue;
        }

        int len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        int valid = 1;
        int dotCount = 0;

        for (int i = 0; i < len; i++) {
            if (input[i] == '.') {
                dotCount++;
                if (dotCount > 1) {
                    valid = 0;
                    break;
                }
            } else if (!isdigit(input[i]) && input[i] != '.') {
                valid = 0;
                break;
            }
        }

        errno = 0;
        number = strtod(input, &endptr);

        if (endptr == input) {
            printf("Ошибка: введены нечисловые символы.\n");
        } else if (*endptr != '\0') {
            printf("Ошибка: введены лишние символы.\n");
        } else if (errno == ERANGE) {
            printf("Ошибка: число выходит за допустимый диапазон.\n");
        } else if (number <= 0) {
            printf("Ошибка: число должно быть положительным.\n");
        } else {
            break;
        }
    }

    return number;
}

void start_excursion_check(char *date) {
    while (1) {
        printf("Введите дату начала экскурсии (формат: дд.мм.гггг): ");
        if (fgets(date, 11, stdin) == NULL) {
            printf("Ошибка ввода.\n");
            continue;
        }

        int len = strlen(date);
        if (len > 0 && date[len - 1] == '\n') {
            date[len - 1] = '\0';
        }

        if (len != 10) {
            printf("Ошибка: неверный формат даты.\n");
            continue;
        }

        if (date[2] != '.' || date[5] != '.') {
            printf("Ошибка: неверный формат даты.\n");
            continue;
        }

        int day = (date[0] - '0') * 10 + (date[1] - '0');
        int month = (date[3] - '0') * 10 + (date[4] - '0');
        int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 2025 || year > 2100) {
            printf("Ошибка: неверная дата.\n");
            continue;
        }

        break;
    }
}

void input_excursion(Excursion *tour) {
    char buffer[100];

    printf("Введите название экскурсии: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Ошибка ввода.\n");
        return;
    }

    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    tour->name = (char *)malloc((len + 1) * sizeof(char));
    if (tour->name == NULL) {
        printf("Ошибка выделения памяти.\n");
        return;
    }
    strcpy(tour->name, buffer);

    printf("Введите стоимость экскурсии: ");
    tour->cost = check_double();

    start_excursion_check(tour->time.start_excursion);

    printf("Введите количество дней: ");
    do {
        rewind(stdin);
        tour->time.amount = check();
        if (tour->time.amount < 0)
            puts("Введите положительное число");
    } while (tour->time.amount < 0);
}

void print_excursion(const Excursion *tour) {
    printf("Название: %s\n", tour->name);
    printf("Стоимость: %.2f\n", tour->cost);
    printf("Дата начала: %s\n", tour->time.start_excursion);
    printf("Количество дней: %d\n", tour->time.amount);
}

void delete_excursion(Excursion **tours, int *count, const char *name) {
    for (int i = 0; i < *count; i++) {
        if (strcmp((*tours)[i].name, name) == 0) {
            free((*tours)[i].name);
            for (int j = i; j < *count - 1; j++) {
                (*tours)[j] = (*tours)[j + 1];
            }
            (*count)--;
            *tours = (Excursion *)realloc(*tours, (*count) * sizeof(Excursion));
            printf("Тур \"%s\" удален.\n", name);
            return;
        }
    }
    printf("Тур с названием \"%s\" не найден.\n", name);
}

void find_excursion(const Excursion *tours, int count, const char *name) {
    if (count == 0) {
        puts("Нет доступных туров.");
        return;
    }

    puts("\nСписок доступных туров:");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i+1, tours[i].name);
    }

    puts("\nВведите номер тура для просмотра подробной информации (или 0 для отмены): ");
    int choice;
    do {
        choice = check();
        if (choice < 0 || choice > count) {
            printf("Неверный номер. Введите число от 1 до %d (или 0 для отмены): ", count);
        }
    } while (choice < 0 || choice > count);

    if (choice == 0) {
        printf("Отмена поиска.\n");
        return;
    }

    puts("\nПодробная информация о туре:");
    print_excursion(&tours[choice-1]);
}

void structure_task(Excursion **tours, int *count) {
    int choice;

    do {
        puts("1 - добавить экскурсию.");
        puts("2 - вывести все туры.");
        puts("3 - удалить тур по названию");
        puts("4 - найти тур по названию");
        puts("0 - выход");

        choice = check();

        switch (choice) {
            case 1:
                *tours = (Excursion *)realloc(*tours, (*count + 1) * sizeof(Excursion));
                if (*tours == NULL) {
                    printf("Ошибка выделения памяти.\n");
                    return;
                }
                input_excursion(&(*tours)[*count]);
                (*count)++;
                break;
            case 2:
                if (*count == 0) {
                    printf("Нет доступных туров.\n");
                } else {
                    for (int i = 0; i < *count; i++) {
                        printf("Тур #%d:\n", i + 1);
                        print_excursion(&(*tours)[i]);
                        printf("\n");
                    }
                }
                break;
            case 3: {
                char name[100];
                printf("Введите название тура для удаления: ");
                scanf("%s", name);
                delete_excursion(tours, count, name);
                break;
            }
            case 4: {
                char name[100];
                printf("Введите название тура для поиска: ");
                scanf("%s", name);
                find_excursion(*tours, *count, name);
                break;
            }
            case 0:
                printf("Выход.\n");
                break;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
                break;
        }
    } while (choice != 0);
}
