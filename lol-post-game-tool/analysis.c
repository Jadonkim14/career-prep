#include <stdlib.h>
#include <string.h>

#include "analysis.h"

MatchAnalysis analyze_events(const struct event *events, int count)
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
        if(events[i].type == KILL){
            if(events[i].team == BLUE)
                result.blue_kill_count++;
            else if(events[i].team == RED)
                result.red_kill_count++;
        }

        else if(events[i].type == DRAGON){
            if(events[i].team == BLUE)
                result.blue_dragon_count++;
            else if(events[i].team == RED)
                result.red_dragon_count++;
        }
    }

    // Detect critical moments
    for (int i = 0; i < count - 2; i++) {
        if (events[i].type == DEATH &&
            events[i + 1].type == DEATH &&
            events[i].team == events[i + 1].team &&
            events[i + 1].time - events[i].time <= 30 &&
            (events[i + 2].type == BARON ||
            events[i + 2].type == DRAGON ||
            events[i + 2].type == TOWER) &&
            events[i + 2].team != events[i].team) {

            if (result.critical_count == result.critical_capacity) {
                int new_capacity = 2 * result.critical_capacity;

                CriticalMoment *new_moments = realloc(
                    result.moments,
                    sizeof(CriticalMoment) * new_capacity
                );

                if (new_moments == NULL) {
                    return result;
                }

                result.critical_capacity = new_capacity;
                result.moments = new_moments;
            }

            result.moments[result.critical_count].start_time =
                events[i].time;

            result.moments[result.critical_count].end_time =
                events[i + 2].time;

            strcpy(
                result.moments[result.critical_count].object,
                event_type_to_string(events[i + 2].type)
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