#include <stdio.h>

#include "output.h"

void print_summary(const MatchAnalysis *result)
{
    printf("\n=== Match Summary ===\n");
    printf("Kills: %d - %d\n", result->blue_kill_count, result->red_kill_count);
    printf("Dragons: %d - %d\n", result->blue_dragon_count, result->red_dragon_count);
}

void print_events_in_range(const Event *events, int count,
    int start_time, int end_time
){
    printf("\n=== Events %d:%02d ~ %d:%02d ===\n", 
            start_time / 60, start_time % 60,
            end_time / 60, end_time % 60);

    for(int i = 0; i < count; i++){
        if (events[i].time >= start_time && events[i].time <= end_time) {
            printf("%d:%02d %s %s\n", 
                events[i].time / 60, events[i].time % 60, 
                events[i].team, events[i].event);
        }

    }
}

void print_critical_moments(const MatchAnalysis *result)
{
    printf("\n=== Critical Moment ===");
    for(int i = 0; i < result->critical_count; i++){
        printf("\nCritical Moment #%d\n", i + 1);

        printf("%d:%02d ~ %d:%02d\n", 
            result->moments[i].start_time / 60,
            result->moments[i].start_time % 60, 
            result->moments[i].end_time / 60,
            result->moments[i].end_time % 60);

        printf("2 Deaths -> %s Lost\n", result->moments[i].object);
    }

    if (result->critical_count == 0)
        printf("\nNo Critical Moments\n");
}