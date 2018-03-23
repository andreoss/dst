#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "report.h"

struct report_s {
    size_t          size;
    char           *current;
    char           *pending;
};

Report         *
report_new(void)
{
    Report         *rep = calloc(1, sizeof(Report));
    if (rep == NULL) {
	return NULL;
    }
    rep->size = 512;
    rep->current = calloc(rep->size, sizeof(char));
    rep->pending = calloc(rep->size, sizeof(char));
    if (rep->current == NULL || rep->pending == NULL) {
	free(rep->current);
	free(rep->pending);
	free(rep);
	return NULL;
    }
    return rep;
}

void
report_pend(Report * report, const char *msg)
{
    if (report == NULL || msg == NULL) {
	return;
    }
    snprintf(report->pending, report->size, "%s", msg);
}

void
report_pop(Report * report)
{
    if (report == NULL) {
	return;
    }
    snprintf(report->current, report->size, "%s", report->pending);
}

const char     *
report_current(const Report * report)
{
    return report->current;
}

void
report_destroy(Report * rep)
{
    if (rep == NULL) {
	return;
    }
    free(rep->current);
    free(rep->pending);
    free(rep);
}

bool
report_is_unsynced(const Report * rep)
{
    return strcmp(rep->current, rep->pending) != 0;
}
