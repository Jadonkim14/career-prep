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

enum read_status {
    READ_SUCCESS,
    READ_EOF,
    READ_ERROR
};

struct event {
    uint16_t time;
    enum event_type type;
    enum team team;
    uint16_t flags;
};

const char *event_type_to_string(enum event_type type);
const char *team_to_string(enum team team);

struct event *load_events(const char *filename, int *count);

bool serialize_event(const struct event *event, FILE *fp);
enum read_status deserialize_event(struct event *event, FILE *fp);

void free_events(struct event *events);

#endif