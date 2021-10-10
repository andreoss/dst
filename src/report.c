#include <stdlib.h>
#include <string.h>

#include "report.h"
struct report_s {
    int             size;
    char           *current;
    char           *pending;
};
Report         *
report_new()
{
    Report         *rep = calloc(1, sizeof(Report));
    rep->size = 256;
    rep->current = calloc(rep->size, sizeof(char));
    rep->pending = calloc(rep->size, sizeof(char));
    return rep;
}
void
report_pend(Report * report, const char *msg)
{
    strcpy(report->pending, msg);
}

void
report_pop(Report * report)
{
    strcpy(report->current, report->pending);
}

char           *
report_current(Report * report)
{
    return report->current;
}
void
report_destroy(Report * rep)
{
    free(rep->current);
    free(rep->pending);
    free(rep);
}

bool
report_is_unsynced(Report * rep)
{
    return strcmp(rep->current, rep->pending) != 0;
}
