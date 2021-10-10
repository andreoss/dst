#include "io.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct io_stats {
    /*
     * sectors read 
     */
    uint64_t        rd_sectors;
    /*
     * sectors written 
     */
    uint64_t        wr_sectors;
    /*
     * sectors discarded 
     */
    uint64_t        dc_sectors;
} io_stats_t;

struct io_device {
    char            name[32];
    io_stats_t     *stats;
    struct io_device *next;
};
/**
 * Read /proc/diskstat
 *
 * See
 * https://github.com/torvalds/linux/blob/master/Documentation/admin-guide/iostats.rst
 */
void
read_diskstats(const char *diskstats, io_stats_t * dev)
{
    assert(dev);
    FILE           *fp;
    char            line[256],
                    dev_name[32];
    uint64_t        rd_sec_or_wr_ios;
    uint64_t        wr_sec;
    uint32_t        dc_sec;
    uint64_t        rd_merges_or_rd_sec;
    uint32_t        rd_ticks_or_wr_sec;

    if ((fp = fopen(diskstats, "r")) == NULL) {
	return;
    }
    uint8_t         minor,
                    major;
    while (fgets(line, sizeof(line), fp) != NULL) {
	size_t          i;
	/*
	 * major minor name rio rmerge rsect ruse wio wmerge wsect wuse
	 * running use aveq dcio dcmerge dcsect dcuse flio fltm 
	 */
	i = sscanf(line,
		   "%hhu %hhu %s %*u %*u %*u %lu %*u %*u %*u %*u %*u %*u %*u %*u %*u "
		   "%lu %u %lu %u",
		   &major, &minor,
		   dev_name, &rd_merges_or_rd_sec, &rd_sec_or_wr_ios,
		   &rd_ticks_or_wr_sec, &wr_sec, &dc_sec);
	if (minor) {
	    continue;
	}
	if (i >= 14) {
	    dev->rd_sectors = rd_sec_or_wr_ios;
	    dev->wr_sectors = wr_sec;
	    if (i >= 18) {
		/*
		 * Discard I/O 
		 */
		dev->dc_sectors = dc_sec;
	    }
	} else if (i == 7) {
	    dev->rd_sectors = rd_merges_or_rd_sec;
	    dev->wr_sectors = rd_ticks_or_wr_sec;
	} else {
	    /*
	     * Unknown entry: Ignore it 
	     */
	    continue;
	}
    }
    fclose(fp);
}
struct io_s {
    bool            warn;
    char           *current;
    char           *previous;

    io_stats_t     *stat;
};
void
io_print(Io * io, char *out)
{
    assert(io);
    assert(out);
}
Io             *
io_new()
{
    Io             *io = calloc(sizeof(Io), 1);
    io->stat = calloc(sizeof(io_stats_t), 1);
    return io;
}

void
io_destroy(Io * io)
{
    free(io);
}
void
io_sample(Io * io)
{
    assert(io);
    read_diskstats("/proc/diskstats", io->stat);
}
