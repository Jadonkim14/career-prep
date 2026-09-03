#include <stdio.h>
#include <stdlib.h>

#include "event.h"

const char *event_type_to_string(enum event_type type)
{
    switch (type)
    {
    case KILL:
        return "KILL";
    
    case DEATH:
        return "DEATH";
    
    case DRAGON:
        return "DRAGON";
    
    case BARON:
        return "BARON";
    
    case TOWER:
        return "TOWER";
    
    default:
        return "UNKNOWN";
    }
} 

const char *team_to_string(enum team team) 
{
    switch (team)
    {
    case BLUE:
        return "BLUE";
    
    case RED:
        return "RED";

    default:
        return "UNKNOWN";
    }
}

struct event *load_events(const char *filename, int *count)
{
    char s[1024];
    FILE *fp;
    struct event e;
    struct event *events = NULL;
    int capacity = 4;

    *count = 0;

    fp = fopen(filename, "r");
    if(fp == NULL){
        printf("Failed to open file: %s\n", filename);
        return NULL;
    } 

    events = malloc(sizeof(struct event) * capacity);
    if (events == NULL) {
        fclose(fp);
        return NULL;
    }

    while(fgets(s, sizeof s, fp) != NULL){
        if(*count == capacity){
            int new_capacity = capacity * 2;
            struct event* new_events = realloc(events, sizeof(struct event) 
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

        if(s[i] == 'B') e.team = BLUE;
        else e.team = RED;
        while(s[i] != ' ') i++;
        i++;

        if(s[i] == 'K') e.type = KILL;
        else if(s[i] == 'D' && s[i+1] == 'E') e.type = DEATH;
        else if(s[i] == 'D' && s[i+1] == 'R') e.type = DRAGON;
        else if(s[i] == 'B') e.type = BARON;
        else e.type = TOWER;

        if(e.type == KILL) e.flags = COMBAT;
        else if(e.type == DEATH) e.flags = COMBAT;
        else if(e.type == DRAGON) e.flags = (OBJECTIVE | MAJOR);
        else if(e.type == BARON) e.flags = (OBJECTIVE | MAJOR);
        else e.flags = OBJECTIVE;

        events[*count] = e;
        (*count)++;
    }

    fclose(fp);
    return events;
}

bool serialize_event(const struct event *event, FILE *fp)
{
    // This system is Little-Endian
    // Write time
    uint8_t low, high;
    low = (event->time & 0x00FF);
    high = (event->time & 0xFF00) >> 8;
    if(!fwrite(&low, 1, 1, fp) 
        || !fwrite(&high, 1, 1, fp)) 
        return false;

    // Write type
    uint8_t type = (uint8_t)event->type;
    if(!fwrite(&type, 1, 1, fp))
        return false;

    // Write team
    uint8_t team = (uint8_t)event->team;
    if(!fwrite(&team, 1, 1, fp))
        return false;

    // Write flags
    low = (event->flags & 0x00FF);
    high = (event->flags & 0xFF00) >> 8;
    if(!fwrite(&low, 1, 1, fp) 
        || !fwrite(&high, 1, 1, fp)) 
        return false;

    return true;
}

enum read_status deserialize_event(struct event *event, FILE *fp)
{
    // This system is Little-Endian
    // Read time
    uint8_t low, high;
    size_t low_cnt, high_cnt;

    low_cnt = fread(&low, 1, 1, fp);
    if(!low_cnt){
        if(feof(fp)) return READ_EOF;
        return READ_ERROR;
    }

    high_cnt = fread(&high, 1, 1, fp);
    if(!high_cnt){
        return READ_ERROR;
    }

    event->time = (uint16_t)low + ((uint16_t)high << 8);

    // Read type
    uint8_t type;
    size_t type_cnt = fread(&type, 1, 1, fp);
    if(!type_cnt){
        return READ_ERROR;
    }
    event->type = (enum event_type)type;

    // Read team
    uint8_t team;
    size_t team_cnt = fread(&team, 1, 1, fp);
    if(!team_cnt){
        return READ_ERROR;
    }
    event->team = (enum team)team;

    // Read flags
    low_cnt = fread(&low, 1, 1, fp);
    if(!low_cnt) return READ_ERROR;
    
    high_cnt = fread(&high, 1, 1, fp);
    if(!high_cnt) return READ_ERROR;

    event->flags = (uint16_t)low + ((uint16_t)high << 8);

    return READ_SUCCESS;
}

void free_events(struct event *events)
{
    free(events);
}