#ifndef EVENT_H
#define EVENT_H

typedef struct{
    int time;
    char team[5];
    char event[32];
} Event;

Event *load_events(const char *filename, int *count);
void free_events(Event *events);

#endif