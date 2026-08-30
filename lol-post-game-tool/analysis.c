#include <stdlib.h>
#include <string.h>

#include "analysis.h"

MatchAnalysis analyze_events(const Event *events, int count)
{
    MatchAnalysis result;

    result.blue_kill_count = 0;
    result.red_kill_count = 0;
    result.blue_dragon_count = 0;
    result.red_dragon_count = 0;
    result.critical_count = 0;
    result.critical_capacity = 4;

    result.moments = malloc(
        sizeof(CriticalMoment) * result.critical_capacity
    );

    if (result.moments == NULL) {
        result.critical_capacity = 0;
        return result;
    }

    // Calculate basic statistics
    for(int i = 0; i < count; i++){
        if(strcmp(events[i].event, "KILL") == 0){
            if(strcmp(events[i].team, "BLUE") == 0)
                result.blue_kill_count++;
            else if(strcmp(events[i].team, "RED") == 0)
                result.red_kill_count++;
        }

        else if(strcmp(events[i].event, "DRAGON") == 0){
            if(strcmp(events[i].team, "BLUE") == 0)
                result.blue_dragon_count++;
            else if(strcmp(events[i].team, "RED") == 0)
                result.red_dragon_count++;
        }
    }

    // Detect critical moments
    for (int i = 0; i < count - 2; i++) {
        if (strcmp(events[i].event, "DEATH") == 0 &&
            strcmp(events[i + 1].event, "DEATH") == 0 &&
            strcmp(events[i].team, events[i + 1].team) == 0 &&
            events[i + 1].time - events[i].time <= 30 &&
            (strcmp(events[i + 2].event, "BARON") == 0 ||
            strcmp(events[i + 2].event, "DRAGON") == 0 ||
            strcmp(events[i + 2].event, "TOWER") == 0) &&
            strcmp(events[i + 2].team, events[i].team) != 0) {
            if(result.critical_count == result.critical_capacity){
                int new_capacity = 2 * result.critical_capacity;

                CriticalMoment *new_moments = realloc(result.moments,
                sizeof(CriticalMoment) * new_capacity);

                if(new_moments == NULL){
                    return result;
                }

                result.critical_capacity = new_capacity;
                result.moments = new_moments;
            }

            result.moments[result.critical_count].start_time = events[i].time;
            result.moments[result.critical_count].end_time = events[i+2].time;
            strcpy(
                result.moments[result.critical_count].object,
                events[i + 2].event
            );
            result.critical_count++;
        }
    }

    return result;
}

void free_analysis(MatchAnalysis *result)
{
    free(result->moments);

    result->moments = NULL;
    result->critical_count = 0;
    result->critical_capacity = 0;
}