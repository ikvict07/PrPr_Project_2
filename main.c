#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char designation;
    int numbering;
    char type_id;
} ID;
typedef struct {
    double latitude;
    double longitude;
} Position;
typedef struct {
    ID id;
    Position position;
    char type[3];
    double value;
    char time[5];
    char date[9];
} Module;
typedef struct ModuleLinkedList {
    Module module;
    struct ModuleLinkedList *next;
} ModuleLinkedList;


void deleteList(ModuleLinkedList **list) {
    ModuleLinkedList *current = *list;
    ModuleLinkedList *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *list = NULL;
}
int compare(char *date1, char *time1, char *date2, char *time2) {
    int dateA, dateB;
    sscanf(date1, "%d", &dateA);
    sscanf(date2, "%d", &dateB);
    int diff = dateA - dateB;
    if (diff == 0) {
        int timeA, timeB;
        sscanf(time1, "%d", &timeA);
        sscanf(time2, "%d", &timeB);
        diff = timeA - timeB;
    }
    return diff; // IF date1 is bigger => returns positive number
}

void case_v(ModuleLinkedList *list){
    if (list == NULL) {
        return;
    }
    int counter = 1;
    do {
        printf("%d:\n", counter);
        printf("ID: %c%d%c\t%s\t%.2lf\n", list->module.id.designation, list->module.id.numbering, list->module.id.type_id, list->module.type, list->module.value);
        printf("Poz: %c%.4lf\t%c%.4lf\n",
               list->module.position.latitude > 0 ? '+' : '-', list->module.position.latitude,
               list->module.position.longitude > 0 ? '+' : '-', list->module.position.longitude);
        printf("DaC: %s\t%s\n", list->module.date, list->module.time);
        counter++;
    }
    while (list->next != NULL && (list = list->next)); // We want to take next only if list has a next element.
    // Logical AND is lazy in C, so it will not evaluate the second part if the first part is false.

}
void case_n(ModuleLinkedList **list) {
    if (*list != NULL) {
        deleteList(list);
    }

    FILE *main_file = fopen( "dataloger_V2.txt", "r");
    rewind(main_file);
    if (main_file == NULL) {
        printf("Zaznamy neboli nacitane!\n");
        return;
    }

    int counter = 0;
    char _[4];
    char designation;
    int numbering;
    char type_id;

    char type[3];
    double value;
    char time[5];
    char date[9];

    char latitudeStr[9], longitudeStr[9];
    while(fscanf(main_file, "%3s\n%c%3d%c\n%8s%8s\n%2s\n%lf\n%s\n%s",
           _,
           &designation,
           &numbering,
           &type_id,
           latitudeStr,
           longitudeStr,
           type,
           &value,
           time,
           date) == 10) {
        counter++;

        ModuleLinkedList *new_module = (ModuleLinkedList *) malloc(sizeof(ModuleLinkedList));
        new_module->module.id.designation = designation;
        new_module->module.id.numbering = numbering;
        new_module->module.id.type_id = type_id;
        new_module->module.position.latitude = atof(latitudeStr);
        new_module->module.position.longitude = atof(longitudeStr);
        strcpy(new_module->module.type, type);
        new_module->module.value = value;
        strcpy(new_module->module.time, time);
        strcpy(new_module->module.date, date);
        new_module->next = NULL;
        if (*list == NULL) {
            *list = new_module;
        } else {
            ModuleLinkedList *temp = *list;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = new_module;
        }
    }
    printf("Nacitalo sa %d zaznamov\n", counter);
}
void case_p(ModuleLinkedList **list) {

    int insert_position;
    scanf("%d", &insert_position);
    char id[6];
    char designation;
    int numbering;
    char type_id;
    char type[3];
    double value;
    char time[5];
    char date[9];
    char latitudeStr[9], longitudeStr[9];

    scanf("%6s", id);
    sscanf(id, "%c%3d%c", &designation, &numbering, &type_id);
    scanf("%8s%8s", latitudeStr, longitudeStr);
    scanf("%2s", type);
    scanf("%lf", &value);
    scanf("%s", time);
    scanf("%s", date);

    ModuleLinkedList *new_node = (ModuleLinkedList *) malloc(sizeof(ModuleLinkedList));
    new_node->module.id.designation = designation;
    new_node->module.id.numbering = numbering;
    new_node->module.id.type_id = type_id;
    new_node->module.position.latitude = atof(latitudeStr);
    new_node->module.position.longitude = atof(longitudeStr);
    strcpy(new_node->module.type, type);
    new_node->module.value = value;
    strcpy(new_node->module.time, time);
    strcpy(new_node->module.date, date);
    new_node->next = NULL;

    if (insert_position == 1) {
        new_node->next = *list;
        *list = new_node;
        return;
    }

    int counter = 1;
    ModuleLinkedList *current_node = *list;

    while (current_node != NULL && counter < insert_position - 1) {
        current_node = current_node->next;
        counter++;
    }
    if (current_node != NULL) {
        new_node->next = current_node->next;
        current_node->next = new_node;
    } else {
        ModuleLinkedList *last_node = *list;
        while (last_node->next != NULL) {
            last_node = last_node->next;
        }
        last_node->next = new_node;
        new_node->next = NULL;
    }
}
void case_z(ModuleLinkedList **list) {
    char id[6];
    char designation;
    int numbering;
    char type_id;
    scanf("%6s", id);
    sscanf(id, "%c%3d%c", &designation, &numbering, &type_id);
    while((*list)->next != NULL) {
        while ((*list)->module.id.designation == designation &&
            (*list)->module.id.numbering == numbering &&
            (*list)->module.id.type_id == type_id) {
            ModuleLinkedList *temp = *list;
            *list = (*list)->next;
            free(temp);
        }
        list = &(*list)->next;
    }
}
void swap(ModuleLinkedList *a, ModuleLinkedList *b)
{
    Module temp = a->module;
    a->module = b->module;
    b->module = temp;
}
void bubbleSort(ModuleLinkedList **list)
{
    int swapped;
    ModuleLinkedList *current;
    ModuleLinkedList *temp = NULL;

    if(list == NULL)
        return;
    do
    {
        swapped = 0;
        current = *list;

        while(current->next != temp) //Sorts until the right border
        {
            if(compare(current->module.date, current->module.time, current->next->module.date, current->next->module.time) > 0)
            {
                swap(current, current->next);
                swapped = 1;
            }
            current = current->next;
        }

        temp = current; //Right border
    } while(swapped);
}

void case_u(ModuleLinkedList **list) {
    bubbleSort(list);
    printf("Spajany zoznam bol usporiadany\n");
}
void case_r(ModuleLinkedList **list) {
    ModuleLinkedList *first_node = *list;
    ModuleLinkedList *second_node = *list;
    int first_position, second_position;
    scanf("%d %d", &first_position, &second_position);
    int counter = 1;
    while (counter < first_position) {
        counter++;
        first_node = first_node->next;
    }
    while(counter < second_position) {
        counter++;
        second_node = second_node->next;
    }
    swap(first_node, second_node);
}

int main() {
    char what_to_do;
    ModuleLinkedList *list = NULL;
    while (1) {
        scanf(" %c", &what_to_do);
        switch (what_to_do) {
            case 'v':
                case_v(list);
                break;
            case 'n':
                case_n(&list);
                break;
            case 'p':
                printf("Inserting new module\n");
                case_p(&list);
                break;
            case 'z':
                case_z(&list);
                break;
            case 'u':
                case_u(&list);
                break;
            case 'r':
                case_r(&list);
                break;
            case 'k':
                while (list != NULL) {
                    ModuleLinkedList *temp = list;
                    list = list->next;
                    free(temp);
                }
                return 0;
            default:
                printf("Invalid input\n");
                return 0;
        }
    }
}