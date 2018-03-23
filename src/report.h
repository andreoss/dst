#pragma once
#include <stdbool.h>

typedef struct report_s Report;

Report         *report_new(void);
void            report_pop(Report * report);
void            report_pend(Report * report, const char *msg);
const char     *report_current(const Report * report);
void            report_destroy(Report * rep);
bool            report_is_unsynced(const Report * rep);
