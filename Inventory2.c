#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_GROUPS 10

pthread_mutex_t inventory_mutex; // for protection
sem_t room_semaphore; // give limit for availability

int inventory = 30;  // Example initial inventory

void* group_activity(void* arg) {
    int group_id = *(int*)arg;

    printf("Group %d: waiting for a room...\n", group_id);
    sem_wait(&room_semaphore);  // Wait for a room

    printf("Group %d: got a room!\n", group_id);

    pthread_mutex_lock(&inventory_mutex); // lock an inventory for an random group

    if (inventory >= 15) {
        inventory -= 15;
        printf("Group %d: used 15 units. Inventory left: %d\n", group_id, inventory);
    } else {
        printf("Group %d: not enough inventory.\n", group_id);
    }
    pthread_mutex_unlock(&inventory_mutex); // unlock an inventory for create another inventory for thread

    sleep(1);  // Simulate time in room
    printf("Group %d: leaving room.\n", group_id);

    sem_post(&room_semaphore);  // Release the room
    return NULL;
}

int main() {
    int num_rooms, num_groups;

    printf("Enter the number of rooms: ");
    scanf("%d", &num_rooms); // e.g., 2

    printf("Enter the number of groups: ");
    scanf("%d", &num_groups);

    printf("Enter the starting inventory: ");
    scanf("%d", &inventory);

    pthread_t threads[MAX_GROUPS];
    int ids[MAX_GROUPS];

    pthread_mutex_init(&inventory_mutex, NULL); // Initializaiton
    sem_init(&room_semaphore, 0, num_rooms);

    for (int i = 0; i < num_groups; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, group_activity, &ids[i]); // create threads for groups
    }

    for (int i = 0; i < num_groups; i++) {
        pthread_join(threads[i], NULL); // joining
    }

    pthread_mutex_destroy(&inventory_mutex); // deletion or destroy
    sem_destroy(&room_semaphore);

    printf("All groups are done.\n");
    return 0;
}

