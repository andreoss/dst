#pragma once
#include <stdbool.h>
typedef struct report_s  Report;
Report *report_new();
void report_pop(Report *report);
void report_pend(Report *report, const char* msg);
char* report_current(Report *report);
void report_destroy(Report *rep);
bool report_is_unsynced(Report *rep);
