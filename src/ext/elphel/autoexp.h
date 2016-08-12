#ifndef __H_AUTOEXP__
#define __H_AUTOEXP__

#define AUTOEXP_DEV_NAME	"/dev/autoexp"

#define TABLES_LEN	8192

#define OFFSET_HIST_C	(4096 + 256 * 6 * 2)
#define OFFSET_GAMMA_C	(OFFSET_HIST_C + 4)

#define IOC_AUTOEXP_SET			0x01
#define IOC_AUTOEXP_GET			0x02
#define IOC_AUTOEXP_GET_LOG		0x03
#define IOC_AUTOEXP_GAMMA_TABLE	0x10

#ifndef HIST_NOT_CHANGE
#define HIST_NOT_CHANGE	0xFFFF
#endif
//!Moved to c313a.h
#if 0
struct autoexp_t {
	unsigned long on;
	/*
	 * in percents: 1 == 1, 100 == 100
	 */
	unsigned long width;
	unsigned long height;
	unsigned long left;
	unsigned long top;
	/*
	 * start exposure time really not needed...
	 */
	unsigned long exp_max;		/* 100 usec == 1 etc... */
	unsigned long overexp_max;	/* percentages for overexposured pixels - 1% == 100, 5% == 500, 0.02% == 2 etc... */
	/*
	 * changed chema - balance exposition for set percent of pixels in needed index
	 */
	unsigned long s_percent;
	unsigned long s_index;
	/*
	 * return current state
	 */
	unsigned long exp;
	/*
	 * "sleep" settings
	 */
	unsigned long skip_pmin;	/* percent of delta for skip changes: 1% == 100 */
	unsigned long skip_pmax;	/* percent of changes for wait one frame before apply changes: 1% == 100 */
	unsigned long skip_t;		/* time for skip changes: 100 usec == 1 */
};
#endif
#define LOG_C	200
//#define LOG_C	11

struct autoexp_log_t {
	unsigned long tv_sec;		/* time of log (fill in driver) */
	unsigned long tv_usec;
	unsigned long s_index;		/* requested in settings index */
	unsigned long s_percent;	/* requested percent for s_index */
	unsigned long n_index;		/* searched new index with needed percent - if 0 - index not searched, and use linear scale */
	unsigned long n_percent;	/* percent in searched index */
	unsigned long t_scale;		/* scale of exposition - in FFFF.FFFF fixed point format */
	unsigned long t_value;		/* new time, requested for sensor - w/o real correction, in 0.1ms scale (1ms == 10 etc...) */
};	/* 8 * 4 == 32 bytes in record... */

#endif	// __H_AUTOEXP__
