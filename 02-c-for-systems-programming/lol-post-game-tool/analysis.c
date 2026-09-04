#include <stdlib.h>
#include <string.h>

#include "analysis.h"

void kill_analyzer(const struct event *ev, MatchAnalysis *ma)
{
    if (ev->type == KILL && ev->team == BLUE)
        ma->blue_kill_count++;
    
    else if (ev->type == KILL && ev->team == RED)
        ma->red_kill_count++;
}

void dragon_analyzer(const struct event *ev, MatchAnalysis *ma)
{
    if (ev->type == DRAGON && ev->team == BLUE)
        ma->blue_dragon_count++;
    
    else if (ev->type == DRAGON && ev->team == RED)
        ma->red_dragon_count++;
}

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

    printf("analyze_events reached\n");

    if (result.moments == NULL) {
        printf("malloc failed\n");
        result.critical_capacity = 0;
        return result;
    }

    // Calculate basic statistics
    void (*analyzers[ANALYZER_CNT])(const struct event *, MatchAnalysis *) = {
            kill_analyzer,
            dragon_analyzer
        };

    for(int i = 0; i < count; i++){
        for(int j = 0; j < ANALYZER_CNT; j++){
            analyzers[j](&events[i], &result);
        }
    }

    printf("analyzers done\n");

    printf("critical analysis start\n");
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

    printf("critical analysis done\n");

    printf("moments = %p\n", (void *)result.moments);
    
    return result;
}

void free_analysis(MatchAnalysis *result)
{
    free(result->moments);

    result->moments = NULL;
    result->critical_count = 0;
    result->critical_capacity = 0;
}