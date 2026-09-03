#ifndef OUTPUT_H
#define OUTPUT_H

#include "analysis.h"

void print_summary(const MatchAnalysis *result);

void print_events_in_range(
    const struct event *events,
    int count,
    int start_time,
    int end_time
);

void print_critical_moments(const MatchAnalysis *result);

#endif