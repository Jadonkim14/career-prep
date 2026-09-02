#include <stdio.h>
#include "event.h"
#include "analysis.h"
#include "output.h"

int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("Usage: %s <match_file>\n", argv[0]);
        return -1;
    }

    int count = 0;
    struct event *events;
    MatchAnalysis result;

    events = load_events(argv[1], &count);
    if(events == NULL) return -1;

    FILE *fp = fopen("match.bin", "wb");
    if(fp == NULL) return -1;

    for(int i = 0; i < count; i++){
        serialize_event(&events[i], fp);
    }
    
    fclose(fp);

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