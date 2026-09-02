#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

enum event_type {
    KILL,
    DEATH,
    DRAGON,
    BARON,
    TOWER
};

enum team {
    BLUE,
    RED
};

enum event_flag {
    COMBAT = 1,
    OBJECTIVE = 2,
    MAJOR = 4
};

struct event {
    uint16_t time;
    enum event_type type;
    enum team team;
    uint16_t flags;
};

struct event *load_events(const char *filename, int *count);

bool serialize_event(const struct event *event, FILE *fp);

void free_events(struct event *events);

#endif