#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int time;
    char team[5];
    char event[32];
} Event;

int main(){
    char s[1024];
    FILE* fp;
    Event e;
    Event *events = NULL;
    int capacity = 4;
    int count = 0;

    fp = fopen("match.txt", "r");
    if(fp == NULL) return -1;

    events = malloc(sizeof(Event) * capacity);
    if(events == NULL){
        fclose(fp);
        return -1;
    }

    while(fgets(s, sizeof s, fp) != NULL){
        if(count == capacity){
            int new_capacity = capacity * 2;
            Event* new_events = realloc(events, sizeof(Event) 
            * new_capacity);

            if(new_events == NULL){
                fclose(fp);
                free(events);
                return -1;
            }

            events = new_events;
            capacity = new_capacity;
        }

        int i = 0;
        e.time = 0;
        while(s[i] != ' '){
            e.time = e.time * 10 + s[i] - '0';
            i++;
        }
        i++;

        int j = 0;
        while(s[i] != ' '){
            e.team[j] = s[i];
            i++;
            j++;
        }
        e.team[j] = '\0';
        i++;

        int k = 0;
        while(s[i] != '\n' && s[i] != '\0'){
            e.event[k] = s[i];
            i++;
            k++;
        }
        e.event[k] = '\0';

        events[count] = e;
        count++;
    }

    int blue_kill_count = 0;
    int red_kill_count = 0;
    int blue_dragon_count = 0;
    int red_dragon_count = 0;
    for(int i = 0; i < count; i++){
        printf("time=%d team=%s event=%s\n"
        ,events[i].time, events[i].team, events[i].event);

        if(strcmp(events[i].event, "KILL") == 0){
            if(strcmp(events[i].team, "BLUE") == 0)
                blue_kill_count++;
            else if(strcmp(events[i].team, "RED") == 0)
                red_kill_count++;
        }

        else if(strcmp(events[i].event, "DRAGON") == 0){
            if(strcmp(events[i].team, "BLUE") == 0)
                blue_dragon_count++;
            else if(strcmp(events[i].team, "RED") == 0)
                red_dragon_count++;
        }
    }

    printf("\n");
    printf("BLUE kills: %d\n", blue_kill_count);
    printf("RED kills: %d\n", red_kill_count);
    printf("BLUE dragons: %d\n", blue_dragon_count);
    printf("RED dragons: %d\n", red_dragon_count);

    printf("\n");
    printf("Events between 25:00 and 26:00:\n");
    for (int i = 0; i < count; i++) {
        if (events[i].time >= 1500 && events[i].time <= 1560) {
            printf("time=%d team=%s event=%s\n", 
                events[i].time, events[i].team, events[i].event);
        }
    }

    printf("\nCritical Moment\n");
    for (int i = 0; i < count - 2; i++) {
        if (strcmp(events[i].event, "DEATH") == 0 &&
            strcmp(events[i + 1].event, "DEATH") == 0 &&
            strcmp(events[i].team, events[i + 1].team) == 0 &&
            events[i + 1].time - events[i].time <= 30 &&
            strcmp(events[i + 2].event, "BARON") == 0 &&
            strcmp(events[i + 2].team, events[i].team) != 0) {
            // Critical Moment candidate
            printf("%d:%02d ~ %d:%02d\n", events[i].time / 60,
            events[i].time % 60, events[i + 2].time / 60,
            events[i + 2].time % 60);
            printf("2 Deaths -> Baron Lost\n");
        }
    }

    free(events);
    fclose(fp);

    return 0;
}