#include "structure.h"
#include "byte_fields.h"

double check_double() {
    char input[20];
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
            puts("Ошибка: введены нечисловые символы.");
        } else if (*endptr != '\0') {
            puts("Ошибка: введены лишние символы.");
        } else if (errno == ERANGE) {
            puts("Ошибка: число выходит за допустимый диапазон.");
        } else if (number <= 0) {
            puts("Ошибка: число должно быть положительным.");
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
            puts("Ошибка ввода.");
            continue;
        }

        int len = strlen(date);
        if (len > 0 && date[len - 1] == '\n') {
            date[len - 1] = '\0';
        }

        if (len != 10) {
            puts("Ошибка: неверный формат даты.");
            continue;
        }

        if (date[2] != '.' || date[5] != '.') {
            puts("Ошибка: неверный формат даты.");
            continue;
        }

        int day = (date[0] - '0') * 10 + (date[1] - '0');
        int month = (date[3] - '0') * 10 + (date[4] - '0');
        int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 2025 || year > 2100) {
            puts("Ошибка: неверная дата.");
            continue;
        }

        break;
    }
}

void input_excursion(Excursion *tour) {
    char buffer[100];

    printf("Введите название экскурсии: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        puts("Ошибка ввода.");
        return;
    }

    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    tour->name = (char *)malloc((len + 1) * sizeof(char));
    if (tour->name == NULL) {
        puts("Ошибка выделения памяти.");
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
    int deleted = 0;
    int new_count = *count;

    for (int i = 0; i < new_count; i++) {
        if (strcmp((*tours)[i].name, name) == 0) {
            deleted++;
        }
    }

    if (deleted == 0) {
        printf("Туры с названием \"%s\" не найдены.\n", name);
        return;
    }

    Excursion *temp = (Excursion *)malloc((*count - deleted) * sizeof(Excursion));
    if (temp == NULL) {
        puts("Ошибка выделения памяти.");
        return;
    }

    int temp_index = 0;
    for (int i = 0; i < *count; i++) {
        if (strcmp((*tours)[i].name, name) != 0) {
            temp[temp_index++] = (*tours)[i];
        } else {
            free((*tours)[i].name);
        }
    }

    free(*tours);

    *tours = temp;
    *count = new_count - deleted;

    printf("Удалено %d туров с названием \"%s\".\n", deleted, name);
}

void find_excursion(const Excursion *tours, int count) {
    if (count == 0) {
        puts("Нет доступных туров.");
        return;
    }

    puts("Введите максимальную стоимость для поиска: ");
    double max_price = check_double();

    int found = 0;

    for (int i = 0; i < count; i++) {
        if (tours[i].cost < max_price) {
            printf("\nТур #%d:\n", i + 1);
            print_excursion(&tours[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("Туров дешевле %.2f не найдено.\n", max_price);
    }
}

void structure_task(Excursion **tours, int *count) {
    int choice;

    do {
        system("clear");

        puts("1 - добавить экскурсию.");
        puts("2 - вывести все туры.");
        puts("3 - удалить тур по названию");
        puts("4 - найти тур по цене");
        puts("0 - выход");

        choice = check();

        switch (choice) {
            case 1:
                system("clear");
                *tours = (Excursion *)realloc(*tours, (*count + 1) * sizeof(Excursion));
                if (*tours == NULL) {
                    printf("Ошибка выделения памяти.\n");
                    return;
                }
                input_excursion(&(*tours)[*count]);
                (*count)++;
                press_enter_to_continue();
                break;
            case 2:
                system("clear");
                if (*count == 0) {
                    printf("Нет доступных туров.\n");
                } else {
                    for (int i = 0; i < *count; i++) {
                        printf("Тур #%d:\n", i + 1);
                        print_excursion(&(*tours)[i]);
                        printf("\n");
                    }
                }
                press_enter_to_continue();
                break;
            case 3: {
                system("clear");
                char name[100];
                printf("Введите название тура для удаления: ");
                scanf("%s", name);
                delete_excursion(tours, count, name);
                press_enter_to_continue();
                break;
            }
            case 4: {
                system("clear");
                char name[100];
                find_excursion(*tours, *count);
                press_enter_to_continue();
                break;
            }
            case 0:
                system("clear");
                printf("Выход.\n");
                break;
            default:
                system("clear");
                printf("Неверный выбор. Попробуйте снова.\n");
                press_enter_to_continue();
                break;
        }
    } while (choice != 0);
}
