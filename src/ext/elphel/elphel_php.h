#ifndef PHP_ELPHEL_H
#define PHP_ELPHEL_H 1

#ifdef ZTS
#include "TSRM.h"
#warning Using TSRM.h
#else
#warning Not using TSRM.h
#endif

#include <elphel/c313a.h>
#include <elphel/exifa.h>
#include <elphel/x393_devices.h>
//#include <autoexp.h>
//#include <elphel/x393_devices.h>

ZEND_BEGIN_MODULE_GLOBALS(elphel)
int fd_exif[SENSOR_PORTS];
int fd_exifdir;
int fd_exifmeta[SENSOR_PORTS];
int exif_size; // to see if verify exif directory has changed
struct exif_dir_table_t exif_dir[ExifKmlNumber] ;  //! store locations of the fields needed for KML generations in the Exif block


/// (circbuf.c) access to /dev/circbuf (mmap, lseek)
int             fd_circ[SENSOR_PORTS];
unsigned long * ccam_dma_buf[SENSOR_PORTS];
unsigned long   ccam_dma_buf_len[SENSOR_PORTS]; // in bytes
//struct framepars_t *framePars;

/// (framepars.c) access to /dev/frameparsall (write, lseek, mmap)

int                       fd_fparmsall[SENSOR_PORTS];
struct framepars_all_t   *frameParsAll[SENSOR_PORTS];
struct framepars_t       *framePars[SENSOR_PORTS];
struct framepars_past_t  *pastPars[SENSOR_PORTS];
unsigned long            *funcs2call[SENSOR_PORTS];        ///< each parameter has a 32-bit mask of what pgm_function to call - other fields not used
unsigned long            *globalPars[SENSOR_PORTS];        ///< parameters that are not frame-related, their changes do not initiate any actions so they can be mmaped for both
unsigned long            *multiSensIndex[SENSOR_PORTS];    ///< indexes of individual sensor register shadows (first of 3)
unsigned long            *multiSensRvrsIndex[SENSOR_PORTS];///< reverse index (to parent) for the multiSensIndex
/*
struct framepars_all_t  *aframeparsall = NULL;              // - will be mmap-ed, in 393 points to an array of structures
struct framepars_t      *aframepars[SENSOR_PORTS];          ///< getting rid of static to be able to use extern
struct framepars_past_t *apastpars[SENSOR_PORTS];            ///< getting rid of static to be able to use extern
unsigned long           *afuncs2call[SENSOR_PORTS];          //  sFrameParsAll.func2call.pars; - each parameter has a 32-bit mask of what pgm_function to call - other fields not used
unsigned long           *aglobalPars[SENSOR_PORTS];          // parameters that are not frame-related, their changes do not initiate any actions so they can be mmaped for both
unsigned long           *amultiSensIndex[SENSOR_PORTS];      // index for per-sensor alternatives
unsigned long           *amultiSensRvrsIndex[SENSOR_PORTS];  // reverse index (to parent) for the multiSensIndex
wait_queue_head_t       aframepars_wait_queue[SENSOR_PORTS];// used to wait for the frame to be acquired

 */

int fd_gamma_cache;
struct gamma_stuct_t  * gamma_cache; /// array of gamma structures

int fd_histogram_cache;
struct histogram_stuct_t  * histogram_cache; /// array of histogram

ZEND_END_MODULE_GLOBALS(elphel)
//!currently ZTS is not defined
#ifdef ZTS
#define ELPHEL_G(v) TSRMG(elphel_globals_id, zend_elphel_globals *, v)
#else
#define ELPHEL_G(v) (elphel_globals.v)
#endif

#define PHP_ELPHEL_VERSION "2.0"
#define PHP_ELPHEL_EXTNAME "elphel"
#ifdef NC353
#define ELPHEL_GLOBALPARS(x) (((unsigned long *) ELPHEL_G(globalPars))[x-FRAMEPAR_GLOBALS])
#else
#define ELPHEL_GLOBALPARS(p, x) (((unsigned long *) ELPHEL_G(globalPars[p]))[x-FRAMEPAR_GLOBALS])
#endif
PHP_FUNCTION(elphel_get_frame);      /// current absolute frame number (includes those that are not compressed)
PHP_FUNCTION(elphel_get_compressed_frame);
PHP_FUNCTION(elphel_skip_frames);    /// skip some frames (includes those that are not compressed) - will work even if no frames are compressed
PHP_FUNCTION(elphel_wait_frame_abs); /// wait for absolute frame number (includes those that are not compressed)
PHP_FUNCTION(elphel_framepars_get_raw);
PHP_FUNCTION(elphel_parse_P_name);
PHP_FUNCTION(elphel_is_global_par);
PHP_FUNCTION(elphel_is_frame_par);
PHP_FUNCTION(elphel_get_P_value);
PHP_FUNCTION(elphel_set_P_value);
PHP_FUNCTION(elphel_test);
PHP_FUNCTION(elphel_get_P_arr);
PHP_FUNCTION(elphel_set_P_arr);
PHP_FUNCTION(elphel_gamma_add);
PHP_FUNCTION(elphel_gamma_add_custom);
PHP_FUNCTION(elphel_gamma_get);
PHP_FUNCTION(elphel_gamma_get_index);
PHP_FUNCTION(elphel_gamma_get_raw);
PHP_FUNCTION(elphel_histogram_get_raw);
PHP_FUNCTION(elphel_histogram_get);

PHP_FUNCTION(elphel_get_state); /// compatibility with old code
PHP_FUNCTION(elphel_num_sensors);

PHP_FUNCTION(elphel_compressor_reset); /// these elphel_compressor_*() functions are for compatibility. Reset will now just stop, not actually reset
PHP_FUNCTION(elphel_compressor_run);
PHP_FUNCTION(elphel_compressor_stop);
PHP_FUNCTION(elphel_compressor_frame);

PHP_FUNCTION(elphel_reset_sensor);
PHP_FUNCTION(elphel_set_fpga_time);
PHP_FUNCTION(elphel_get_fpga_time);
PHP_FUNCTION(elphel_wait_frame);          /// wait for compressed frame in a circular frame buffer - will wait forever if compressor is off
PHP_FUNCTION(elphel_fpga_read);
PHP_FUNCTION(elphel_fpga_write);
PHP_FUNCTION(elphel_gamma);
PHP_FUNCTION(elphel_reverse_gamma);
PHP_FUNCTION(elphel_histogram);
PHP_FUNCTION(elphel_reverse_histogram);
PHP_FUNCTION(elphel_get_exif_field);
PHP_FUNCTION(elphel_set_exif_field);
PHP_FUNCTION(elphel_get_interframe_meta);
PHP_FUNCTION(elphel_get_exif_elphel);
PHP_FUNCTION(elphel_get_circbuf_pointers);
PHP_FUNCTION(elphel_update_exif); // force to rebuild directory after Exif format was changed Usually done automatically
PHP_FUNCTION(elphel_frame2tf);
PHP_FUNCTION(elphel_tf2frame);

PHP_MINIT_FUNCTION(elphel);
PHP_MSHUTDOWN_FUNCTION(elphel);
PHP_RINIT_FUNCTION(elphel);
PHP_MINFO_FUNCTION(elphel);
extern zend_module_entry elphel_module_entry;
#define phpext_elphel_ptr &elphel_module_entry
//static void init_sens();
int splitConstantName             (char * name);
int get_histogram_index           (long port, long sub_chn, long color,long frame, long needreverse); /// histogram is availble for previous frame, not for the current one
unsigned long get_imageParamsThat (int port, int indx, unsigned long frame);

#endif
