#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct E {
    char name[16];
    float damage;
    float magic;
    struct E* next;
} Enemy;

typedef struct {
    int num;
    Enemy* next;
} EnemyLinkedList;

void AddNewEnemy(EnemyLinkedList *head, Enemy* newEnemy) {
    newEnemy->next = head->next;
    head->next = newEnemy;
    head->num++;
}
void RemoveEnemy(EnemyLinkedList *head, const char* name) {
    Enemy *p = head->next;
    if (p != NULL && strcmp(p->name, name) == 0) {
        head->next = p->next;
        free(p);
        head->num--;
        printf("Enemy '%s' removed successfully.\n", name);
        return;
    }

    while (p != NULL && p->next != NULL) {
        if (strcmp(p->next->name, name) == 0) {
            Enemy *tmp = p->next->next;
            free(p->next);
            p->next = tmp;
            head->num--;
            printf("Enemy '%s' removed successfully.\n", name);
            return;
        }
        p = p->next;
    }

    printf("Enemy '%s' not found.\n", name);
}

Enemy* FindEnemyByName(EnemyLinkedList *head, const char* name) {
    Enemy *p = head->next;
    while (p != NULL && strcmp(p->name, name) != 0)
        p = p->next;
    return p;
}

Enemy* SortByDamage(Enemy *first) {
    if (first == NULL || first->next == NULL)
        return first;

    Enemy *i, *j;
    for (i = first; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (i->damage < j->damage) {
                // 交换敌人的属性
                float temp_damage = i->damage;
                float temp_magic = i->magic;
                char temp_name[16];
                strcpy(temp_name, i->name);

                i->damage = j->damage;
                i->magic = j->magic;
                strcpy(i->name, j->name);

                j->damage = temp_damage;
                j->magic = temp_magic;
                strcpy(j->name, temp_name);
            }
        }
    }
    return first;
}

// 输出链表中所有敌人的信息
void PrintEnemies(EnemyLinkedList *head) {
    Enemy* p = head->next;
    while (p != NULL) {
        printf("Name: %s, Damage: %.2f, Magic: %.2f\n", p->name, p->damage, p->magic);
        p = p->next;
    }
    printf("\n");
}

void DestroyEnemyList(EnemyLinkedList *head) {
    Enemy* p = head->next;
    Enemy* q;
    while (p != NULL) {
        q = p->next;
        free(p);
        p = q;
    }
    free(head);
}

void AddCustomEnemy(EnemyLinkedList *head) {
    Enemy *newEnemy = (Enemy*)malloc(sizeof(Enemy));
    printf("Enter enemy name: ");
    scanf("%s", newEnemy->name);
    printf("Enter enemy damage: ");
    scanf("%f", &newEnemy->damage);
    printf("Enter enemy magic: ");
    scanf("%f", &newEnemy->magic);

    AddNewEnemy(head, newEnemy);
    printf("Enemy '%s' added successfully.\n", newEnemy->name);
}

void RemoveSpecificEnemy(EnemyLinkedList *head) {
    char name[16];
    printf("Enter the name of the enemy to remove: ");
    scanf("%s", name);

    RemoveEnemy(head, name);
}

int main(void) {
    srand(time(NULL));

    // 创建敌人链表头
    EnemyLinkedList *head = (EnemyLinkedList*)malloc(sizeof(EnemyLinkedList));
    head->next = NULL;
    head->num = 0;

    // 创建初始的敌人
    for (int i = 0; i < 5; i++) {
        Enemy *enemy = (Enemy*)malloc(sizeof(Enemy));
        sprintf(enemy->name, "Enemy%d", i + 1);
        enemy->damage = (rand() % 101);  // 伤害值为0到100之间
        enemy->magic = (rand() % 101);   // 魔法值为0到100之间
        AddNewEnemy(head, enemy);
    }

    int choice;
    do {
        printf("\n----- Enemy Management System -----\n");
        printf("1. Print all enemies\n");
        printf("2. Add a custom enemy\n");
        printf("3. Remove a specific enemy\n");
        printf("4. Sort enemies by damage\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                PrintEnemies(head);
                break;
            case 2:
                AddCustomEnemy(head);
                break;
            case 3:
                RemoveSpecificEnemy(head);
                break;
            case 4:
                head->next = SortByDamage(head->next);
                printf("Enemies sorted by damage:\n");
                PrintEnemies(head);
                break;
            case 5:
                printf("Exiting the system...\n");
                break;
            default:
                printf("Invalid choice, please try again.\n");
                break;
        }
    } while (choice != 5);
    DestroyEnemyList(head);

    return 0;
}