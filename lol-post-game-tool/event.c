#include <stdio.h>
#include <stdlib.h>

#include "event.h"

Event *load_events(const char *filename, int *count)
{
    char s[1024];
    FILE *fp;
    Event e;
    Event *events = NULL;
    int capacity = 4;

    *count = 0;

    fp = fopen(filename, "r");
    if(fp == NULL){
        printf("Failed to open file: %s\n", filename);
        return NULL;
    } 

    events = malloc(sizeof(Event) * capacity);
    if (events == NULL) {
        fclose(fp);
        return NULL;
    }

    while(fgets(s, sizeof s, fp) != NULL){
        if(*count == capacity){
            int new_capacity = capacity * 2;
            Event* new_events = realloc(events, sizeof(Event) 
            * new_capacity);

            if(new_events == NULL){
                fclose(fp);
                free(events);
                return NULL;
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

        events[*count] = e;
        (*count)++;
    }

    fclose(fp);
    return events;
}

void free_events(Event *events)
{
    free(events);
}