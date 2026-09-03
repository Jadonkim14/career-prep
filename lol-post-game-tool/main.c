#include <stdio.h>
#include <stdlib.h>

#include "event.h"
#include "analysis.h"
#include "output.h"

int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("Usage: %s <match_file>\n", argv[0]);
        return -1;
    }

    int count = 0;
    struct event *events, *first_events;
    MatchAnalysis result;

    result.blue_kill_count = 0;
    result.red_kill_count = 0;
    result.blue_dragon_count = 0;
    result.red_dragon_count = 0;

    events = load_events(argv[1], &count);
    if(events == NULL) return -1;

    first_events = malloc(sizeof(struct event) * count);
    if (first_events == NULL) {
        free_events(events);
        return -1;
    }
    
    for(int i = 0; i < count; i++){
        first_events[i] = events[i];
    }

    FILE *fp = fopen("match.bin", "wb");
    if(fp == NULL) {
        free_events(events);
        return -1;
    }

    for(int i = 0; i < count; i++){
        if (!serialize_event(&events[i], fp)) {
            fclose(fp);
            free_events(events);
            return -1;
        }
    }
    fclose(fp);

    fp = fopen("match.bin", "rb");
    if(fp == NULL) {
        free_events(events);
        return -1;
    }

    for(int i = 0; i < count; i++){
        enum read_status rs = deserialize_event(&events[i], fp);
        if (rs == READ_EOF || rs == READ_ERROR){
            fclose(fp);
            free_events(events);
            return -1;
        }
    }
    fclose(fp);

    bool all_same = true;
    for(int i = 0; i < count; i++){
        if(first_events[i].time != events[i].time
            || first_events[i].type != events[i].type
            || first_events[i].team != events[i].team
            || first_events[i].flags != events[i].flags) {
                all_same = false;
                printf("events[%d] is not same!\n", i);
            }
    }
    if (all_same)
        printf("All events are same!\n");
    
    free(first_events);

    result = analyze_events(events, count);
    if (result.moments == NULL) {
        free_events(events);
        return -1;
    }

    print_summary(&result);

    print_events_in_range(events, count, 1500, 1560); // 25:00 ~ 26:00
    print_critical_moments(&result);

    free_analysis(&result);
    free_events(events);

    return 0;
}