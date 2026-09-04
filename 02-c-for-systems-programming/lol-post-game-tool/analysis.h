#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "event.h"

#define ANALYZER_CNT 2

typedef struct {
    int start_time;
    int end_time;
    char object[32];
} CriticalMoment;

typedef struct {
    int blue_kill_count;
    int red_kill_count;
    int blue_dragon_count;
    int red_dragon_count;

    CriticalMoment *moments;
    int critical_count;
    int critical_capacity;
} MatchAnalysis;

void kill_analyzer(const struct event *ev, MatchAnalysis *ma);
void dragon_analyzer(const struct event *ev, MatchAnalysis *ma);

MatchAnalysis analyze_events(const struct event *events, int count);

void free_analysis(MatchAnalysis *result);

#endif