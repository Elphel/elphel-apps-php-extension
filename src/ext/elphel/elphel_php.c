/** @file elphel_php.c
 *  @brief Implementation of elphel extension module for PHP to support camera functionality
 *  @copyright Copyright (C) 2008-2016 Elphel, Inc.
 *
 * @par <b>License</b>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/****************************************************************************
 *! -----------------------------------------------------------------------------**
 *!  $Log: elphel_php.c,v $
 *!  Revision 1.8  2012/04/20 00:25:26  elphel
 *!  fixed bug in reading Exif data from PHP that was introduced while expanding MakerNote to include temperatures
 *!
 *!  Revision 1.7  2012/04/08 04:11:28  elphel
 *!  8.2.2 changes related to temperatures measurement and embedding in the Exif MakerNote
 *!
 *!  Revision 1.6  2010/08/10 21:14:31  elphel
 *!  8.0.8.39 - added EEPROM support for multiplexor and sensor boards, so autocampars.php uses application-specific defaults. Exif Orientation tag support, camera Model reflects application and optional mode (i.e. camera number in Eyesis)
 *!
 *!  Revision 1.5  2010/07/20 20:13:34  elphel
 *!  8.0.8.33 - added MakerNote info for composite images made with multisensor cameras (with 10359 board)
 *!
 *!  Revision 1.4  2010/05/13 03:39:31  elphel
 *!  8.0.8.12 -  drivers modified for multi-sensor operation
 *!
 *!  Revision 1.3  2010/03/04 06:41:40  elphel
 *!  8.0.7.3 - more data to makerNote
 *!
 *!  Revision 1.2  2009/12/28 06:24:17  elphel
 *!  8.0.6.6 - added MakerNote to Exif, it icludes channels gains and gammas/black levels
 *!
 *!  Revision 1.1  2009/04/03 23:18:59  elphel
 *!  rev 8.0.4.1 - upgraded PHP to 5.2.9 - directories/files added/removed
 *!
 *!  Revision 1.5  2009/02/18 06:25:41  elphel
 *!  fixed unterminated string of 1 character (GPS mode - 2/3)
 *!
 *!  Revision 1.4  2008/12/08 08:17:50  elphel
 *!  bug fix in elphel_get_P_value() - it did not handle bit filed for non-global variables ( elphel_get_P_arr() was OK)
 *!
 *!  Revision 1.3  2008/11/30 05:04:46  elphel
 *!  added Doxygen file data
 *!
 *!  Revision 1.2  2008/11/28 08:17:09  elphel
 *!  keeping Doxygen a little happier
 *!
 *!  Revision 1.1.1.1  2008/11/27 20:04:02  elphel
 *!
 *!
 *!  Revision 1.28  2008/11/22 05:56:40  elphel
 *!  elphel_set_P_arr(), elphel_set_P_value() now return frame number parameters were written to
 *!
 *!  Revision 1.27  2008/11/20 07:06:04  elphel
 *!  just touched to overcome dependency (or lack of its support) to make it notice updated c313a.h
 *!
 *!  Revision 1.26  2008/11/15 07:04:27  elphel
 *!  new parameters to modify analog gains while white balancing
 *!
 *!  Revision 1.25  2008/11/13 05:40:45  elphel
 *!  8.0.alpha16 - modified histogram storage, profiling
 *!
 *!  Revision 1.24  2008/11/05 02:01:25  elphel
 *!  Added bit field manipulation in parameters
 *!
 *!  Revision 1.23  2008/11/04 17:41:43  elphel
 *!  added elphel_gamma(), elphel_reverse_gamma(), elphel_histogram(), elphel_reverse_histogram() functions
 *!
 *!  Revision 1.22  2008/11/02 07:24:20  elphel
 *!  added support for some legacy functions
 *!
 *!  Revision 1.21  2008/11/02 00:33:02  elphel
 *!  Added TODO
 *!
 *!  Revision 1.20  2008/11/01 06:25:46  elphel
 *!  elphel_get_circbuf_pointers() now returns framenumber too
 *!
 *!  Revision 1.19  2008/10/31 18:26:32  elphel
 *!  Adding support for constants like SENSOR_REGS32 (defined constant plus 32 to simplify referencing sensor registers from PHP
 *!
 *!  Revision 1.18  2008/10/29 04:18:28  elphel
 *!  v.8.0.alpha10 made a separate structure for global parameters (not related to particular frames in a frame queue)
 *!
 *!  Revision 1.17  2008/10/28 07:05:49  elphel
 *!  touched
 *!
 *!  Revision 1.16  2008/10/25 19:51:06  elphel
 *!  Changed word order in writes to gamma tables driver
 *!
 *!  Revision 1.15  2008/10/23 18:26:14  elphel
 *!  Fixed percentile calculations in histograms
 *!
 *!  Revision 1.14  2008/10/23 08:11:38  elphel
 *!  updated for histograms wait queues
 *!
 *!  Revision 1.13  2008/10/19 06:56:05  elphel
 *!  elphel_wait_frame() now works only for compressed frames, new elphel_skip_frames() and elphel_wait_frame_abs() wait sequencer frames (all sensor frames, even those that are not compressed)
 *!
 *!  Revision 1.12  2008/10/15 22:28:56  elphel
 *!  snapshot 8.0.alpha2
 *!
 *!  Revision 1.11  2008/10/13 16:56:20  elphel
 *!  just touched to force recompile
 *!
 *!  Revision 1.10  2008/10/12 06:13:10  elphel
 *!  snapshot
 *!
 *!  Revision 1.9  2008/10/08 21:26:25  elphel
 *!  snapsot 7.2.0.pre4 - first images (actually - second)
 *!
 *!  Revision 1.8  2008/10/05 05:13:33  elphel
 *!  snapshot003
 *!
 *!  Revision 1.7  2008/10/04 16:10:12  elphel
 *!  snapshot
 *!
 *!  Revision 1.6  2008/09/25 00:58:11  elphel
 *!  snapshot
 *!
 *!  Revision 1.5  2008/09/19 18:06:38  elphel
 *!  snapshot
 *!
 *!  Revision 1.4  2008/09/19 04:37:24  elphel
 *!  snapshot
 *!
 *!  Revision 1.3  2008/09/07 19:48:08  elphel
 *!  snapshot
 *!
 *!  Revision 1.2  2008/06/04 20:07:08  elphel
 *!  adding support for multiple frames
 *!
 *!  Revision 1.21  2008/05/02 15:12:09  elphel
 *!  minor comment edit
 *!
 *!  Revision 1.20  2008/05/01 01:32:04  elphel
 *!  support for the frame number - combining hardware frame counter used by i2c (3-bit) and software one
 *!
 *!  Revision 1.19  2008/04/25 21:31:35  elphel
 *!  Added Exif_Photo_ExposureTime to be returned by elphel_get_exif_elphel()
 *!
 *!  Revision 1.18  2008/04/24 18:16:40  elphel
 *!  New function to retrieve circbuf structure (frame pointers and Exif pointers)
 *!
 *!  Revision 1.17  2008/04/22 22:14:08  elphel
 *!  Added functions related to Exif data
 *!
 *!  Revision 1.16  2008/04/20 06:49:04  elphel
 *!  Added histogram related functions
 *!
 *!  Revision 1.15  2008/04/17 22:36:07  elphel
 *!  Bug fix, new function added - elphel_is_compressor_idle()
 *!
 *!  Revision 1.14  2008/03/20 22:25:09  elphel
 *!  elphel_trigger - programming synchronization parameters
 *!
 *!  Revision 1.13  2008/03/15 23:04:21  elphel
 *!  added FPGA registers R/W
 *!
 *!  Revision 1.12  2008/01/27 06:23:17  elphel
 *!  New function - elphel_wait_frame (wait fro the next frame to be compressed)
 *!
 *!  Revision 1.11  2008/01/12 06:53:23  elphel
 *!  7.1.7.2 - added elphel_autoexposure_get() function to elphel php extension
 *!
 *!  Revision 1.10  2008/01/11 07:47:44  elphel
 *!  added elphel_autoexposure_set() function
 *!
 *!  Revision 1.9  2008/01/10 02:43:37  elphel
 *!  Added balance of 2 greens in Bayer mosaic
 *!
 *!  Revision 1.8  2008/01/09 10:22:02  elphel
 *!  Implemented elphel_white_balance() function
 *!
 *!  Revision 1.7  2007/12/14 22:38:53  elphel
 *!  cleaned up after fixing cache bug
 *!
 *!  Revision 1.6  2007/12/06 19:05:33  elphel
 *!  2 new functions - arbitrary compressor command (elphel_compressor_cmd) and elphel_compressor_reset
 *!
 *!  Revision 1.5  2007/12/05 23:41:04  elphel
 *!  Fixing minor in bugs in the sensor reset code
 *!
 *!  Revision 1.4  2007/12/05 22:01:19  elphel
 *!  added handling etrax fs mmap cache problem when communicating between PHP extension and the driver
 *!
 *!  Revision 1.3  2007/12/04 06:41:11  elphel
 *!  Implementation of 2 additional functions:    elphel_get_P_arr, elphel_set_P_arr
 *!
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <sys/mman.h>		/* mmap */
#include <sys/ioctl.h>
#include <fcntl.h>     /* (O_RDWR) */
#include <asm/byteorder.h>
#include <errno.h>
#include "php.h"
#include "php_ini.h"  /* for php.ini processing */
#include "ext/standard/info.h" /* for php_info_print_table_* */
#include "elphel_php.h"


ZEND_DECLARE_MODULE_GLOBALS(elphel)
static zend_function_entry elphel_functions[] = {
        PHP_FE(elphel_get_frame, NULL)
        PHP_FE(elphel_get_compressed_frame, NULL)
        PHP_FE(elphel_skip_frames, NULL)
        PHP_FE(elphel_wait_frame_abs, NULL)
        PHP_FE(elphel_framepars_get_raw, NULL)
        PHP_FE(elphel_parse_P_name, NULL)
        PHP_FE(elphel_is_global_par, NULL)
        PHP_FE(elphel_is_frame_par, NULL)
        PHP_FE(elphel_get_P_value, NULL)
        PHP_FE(elphel_test, NULL)
        PHP_FE(elphel_set_P_value, NULL)
        PHP_FE(elphel_get_P_arr, NULL)
        PHP_FE(elphel_set_P_arr, NULL)
        PHP_FE(elphel_gamma_add, NULL)
        PHP_FE(elphel_gamma_add_custom, NULL)
        PHP_FE(elphel_gamma_get, NULL)
        PHP_FE(elphel_gamma_get_index, NULL)
        PHP_FE(elphel_gamma_get_raw, NULL)
        PHP_FE(elphel_histogram_get_raw, NULL)
        PHP_FE(elphel_histogram_get, NULL)
        PHP_FE(elphel_get_state, NULL)
        PHP_FE(elphel_num_sensors, NULL)

        PHP_FE(elphel_compressor_reset, NULL)
        PHP_FE(elphel_compressor_run, NULL)
        PHP_FE(elphel_compressor_stop, NULL)
        PHP_FE(elphel_compressor_frame, NULL)
        PHP_FE(elphel_reset_sensor, NULL)
        PHP_FE(elphel_set_fpga_time, NULL)
        PHP_FE(elphel_get_fpga_time, NULL)
        PHP_FE(elphel_wait_frame, NULL)
        PHP_FE(elphel_fpga_read, NULL)
        PHP_FE(elphel_fpga_write, NULL)
        PHP_FE(elphel_gamma, NULL)
        PHP_FE(elphel_reverse_gamma, NULL)
        PHP_FE(elphel_histogram, NULL)
        PHP_FE(elphel_reverse_histogram, NULL)
        PHP_FE(elphel_get_exif_field, NULL)
        PHP_FE(elphel_set_exif_field, NULL)
        PHP_FE(elphel_get_interframe_meta, NULL)
        PHP_FE(elphel_get_exif_elphel, NULL)
        PHP_FE(elphel_update_exif, NULL)
        PHP_FE(elphel_get_circbuf_pointers, NULL)
        {NULL, NULL, NULL}
};

zend_module_entry elphel_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
        STANDARD_MODULE_HEADER,
#endif
        PHP_ELPHEL_EXTNAME,
        elphel_functions,
        PHP_MINIT(elphel),
        PHP_MSHUTDOWN(elphel),
        PHP_RINIT(elphel),
        NULL,
        PHP_MINFO(elphel),
#if ZEND_MODULE_API_NO >= 20010901
        PHP_ELPHEL_VERSION,
#endif
        STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_ELPHEL
ZEND_GET_MODULE(elphel)
#endif


PHP_INI_BEGIN()
//! read ini entries here
PHP_INI_END()


//!call before any access to sensor parameters/functions
static void init_sens() {
}

/**
 * @brief See if the name ends with __A,__B,__c,__a,__b.__c
 * @param name constant name, possibly ending with a suffix __A,__B,__c,__a,__b or __c
 * @return <0 - no suffix, >=0 channel (0..2) | 4 for strict (lower case)
 */
//multiSensIndex 
int parseMultiSens(char * name) {
    int len= strlen(name);
    int nIndex;
    int d=0;
    int dp=1;
    int bfields;
    int rslt=-1;
    if ((len>3) && (name[len-2]=='_') && (name[len-3]=='_')) {
        switch (name[len-1]) {
        case 'A':rslt=0; break;
        case 'B':rslt=1; break;
        case 'C':rslt=2; break;
        case 'a':rslt=256; break;
        case 'b':rslt=257; break;
        case 'c':rslt=258; break;
        }
    }
    if (rslt >=0) name[len-3]='\0';
    return rslt;
}

/**
 * @brief apply
 * @param parNum parameter composite index
 * @param multiMod bit 8 - strict, lower bits - channel index
 * @return -1 - failure, otherwise - parameter index with allcontrol  bits preserved
 */
//multiSensIndex P_MAX_PAR MAX_SENSORS=3
int applyMultiSens(long parNum, long multiMod) {
    unsigned long * multiSensIndex = ((unsigned long *) (ELPHEL_G (multiSensIndex)));
    long parIndex= parNum &0xffff;
    if (parIndex >= P_MAX_PAR) return -1;
    if (multiSensIndex[parIndex]==0) {
        if (multiMod & 0x100) return -1;
        return parNum; /// No individual parameters available, return the original one as 'strict' bit is not set.
    }
    multiMod &= 0xff;
    if (multiMod >= MAX_SENSORS) return -1;
    parIndex= (multiSensIndex[parIndex]+multiMod) & 0xffff;
    if (parIndex >= P_MAX_PAR) return -1; /// multiSensIndex seems to be corrupted - add error message?

    return (parNum & 0xffff0000) | parIndex; /// preserve original modifiers
}

/**
 * @brief See if the name ends with number, truncate name to remove number and return number value
 * @param name constant name, possibly ending with a number (decimal)
 *        if constant includes "__" (double "_") and has 4 decimals after __WWBB, those decimals are treated as
 *        bit field specs (WW 1..31 - width, BB - 0..31 - start bit ), i.e. "__0816" is the 3-rd byte of 4 in the 32-bit parameter
 *        These bit-filed modifiers are stored in bits 16..20 (bit) and 21..25 (width) of parameter address (number)
 * @return number value (to be added to the constant value) or -1 if name does not end with a number
 */
//multiSensIndex 
int splitConstantName(char * name) {
    int len= strlen(name);
    int nIndex;
    int d=0;
    int dp=1;
    int bfields;
    int success=0;
    if ((len>6) && (name[len-6]=='_') && (name[len-5]=='_')) {

        d= FRAMEPAIR_FRAME_BITS(((name[len-3] - '0') + ((name[len-4] - '0') * 10)), ((name[len-1] - '0') + ((name[len-2] - '0') * 10)));
        name[len-6]='\0';
        len-=6;
        success=1;
    }
    nIndex= len-1;
    while ((nIndex>0) && (name[nIndex]>='0') && (name[nIndex] <='9')){
        d+=dp*(name[nIndex]-'0');
        dp*=10;
        nIndex--;
        success=1;
    }
    nIndex++;
    if (success) {
        name[nIndex]='\0'; /// terminate name with '\0'
        return d;
    } else return -1;
}

/// Get current frame number
PHP_FUNCTION(elphel_get_frame)
{
    long port;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &port) == FAILURE)
        RETURN_NULL();
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    RETURN_LONG( ELPHEL_GLOBALPARS(port,G_THIS_FRAME));
} 
/// Get current compressed frame number
PHP_FUNCTION(elphel_get_compressed_frame)
{
    long port;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &port) == FAILURE)
        RETURN_NULL();
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    RETURN_LONG( ELPHEL_GLOBALPARS(port,G_COMPRESSOR_FRAME));
}
PHP_FUNCTION(elphel_skip_frames)
{
    long port;
    long skip=1;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &port, &skip) == FAILURE)
        RETURN_NULL();
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    long target_frame=lseek((int) ELPHEL_G( fd_fparmsall[port]), 0, SEEK_CUR )+skip;
    if ((target_frame<0) || (target_frame > 0x7ffffdff))
        RETURN_NULL(); /// Out of limit for skip frames
    RETURN_LONG(lseek((int) ELPHEL_G( fd_fparmsall[port]), target_frame + LSEEK_FRAME_WAIT_ABS, SEEK_END ));
} 

PHP_FUNCTION(elphel_wait_frame_abs)
{
    long port;
    long target_frame;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &port, &target_frame) == FAILURE)
        RETURN_NULL();
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    if ((target_frame<0) || (target_frame > 0x7ffffdff))
        RETURN_NULL(); /// Out of limit for skip frames
    RETURN_LONG(lseek((int) ELPHEL_G( fd_fparmsall[port]), target_frame + LSEEK_FRAME_WAIT_ABS, SEEK_END ));
} 

/**
 * @brief Parse P_* /G_* parameter name with modifiers
 * @param name - constant name (w/o leading "ELPHEL_")
 * @return  full address/number or NULL if it does not exist
 */
PHP_FUNCTION(elphel_parse_P_name)
{
    char full_constant_name[256];
    char *name;
    int name_len;
    long full_addr =-1;
    zval const_value;
    long constAddNumber;
    long multiMod;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE)
        RETURN_NULL();
    if (strlen(name)>(sizeof(full_constant_name)-8)) RETURN_NULL();
    sprintf (full_constant_name,"ELPHEL_%s",name);
    if (zend_get_constant(full_constant_name, strlen(full_constant_name), &const_value TSRMLS_CC)) { /// found the constant as is
        full_addr= Z_LVAL(const_value);
    } else {
        multiMod=parseMultiSens(full_constant_name); /// will truncate  full_constant_name if sensor number suffix found
        constAddNumber=splitConstantName(full_constant_name);
        if ((constAddNumber>=0) && (zend_get_constant(full_constant_name, strlen(full_constant_name), &const_value TSRMLS_CC))) {
            full_addr= (Z_LVAL(const_value) & ~FRAMEPAIR_MASK_BYTES)+constAddNumber; /// FRAMEPAIR_MASK_BYTES to prevent bit-field modifier addition to constants that already have them
            if ((multiMod>=0) && (full_addr!=-1)) full_addr=applyMultiSens(full_addr,multiMod);
        }
    }
    if (full_addr!=-1) {
        RETURN_LONG (full_addr);
        zval_dtor(&const_value);
    }
    RETURN_NULL();
}

/**
 * @brief Strips integer constant of optional modifiers and finds if it is a valid global parameter number
 * @param full parameter address
 * @return true if this address is a valid address of a global parameter
 */
PHP_FUNCTION(elphel_is_global_par)
{
    long addr;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &addr) == FAILURE)
        RETURN_NULL();
    addr &= 0xffff; /// remove any possible flags
    if ((addr >= FRAMEPAR_GLOBALS) && (addr < (FRAMEPAR_GLOBALS+P_MAX_GPAR))) RETURN_TRUE;
    RETURN_FALSE;
}

/**
 * @brief Strips integer constant of optional modifiers and finds if it is a valid frame parameter number
 * @param full parameter address
 * @return true if this address is a valid address of a frame parameter
 */
PHP_FUNCTION(elphel_is_frame_par)
{
    long addr;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &addr) == FAILURE)
        RETURN_NULL();
    addr &= 0xffff; /// remove any possible flags
    if (addr < (sizeof (struct framepars_t) >>2)) RETURN_TRUE;
    RETURN_FALSE;
}

//! Read value from the sensor/compressor parameters ("read" parameters, verified by the driver), see asm/elphel/c313a.h
///TODO: Make it read pastPars also?
PHP_FUNCTION(elphel_get_P_value)
{
    long port, addr, full_addr;
    long frame=-1;
    long frame_index=-1;
    int  frame_stored;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll|l", &port, &full_addr,&frame) == FAILURE)
        RETURN_NULL();
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    if (frame <0)
        frame=ELPHEL_GLOBALPARS(port, G_THIS_FRAME);
    addr = full_addr & 0xffff; /// remove any possible flags
    /// is it a global parameter?
    if (addr >= FRAMEPAR_GLOBALS) { /// these globals can be written just through mmap
        if (addr >= (FRAMEPAR_GLOBALS+P_MAX_GPAR))
            RETURN_NULL();
        if (full_addr & FRAMEPAIR_MASK_BYTES) {
            RETURN_LONG(FRAMEPAIR_FRAME_FIELD(full_addr,ELPHEL_GLOBALPARS(port,addr)));
        } else {
            RETURN_LONG(ELPHEL_GLOBALPARS(port, addr));
        }
    }
    /// processing dynamic frame parameters
    if ((addr<0) ||(addr>= (sizeof (struct framepars_t) >>2))) {
        RETURN_NULL();
    }
    if (frame <0) {
        //      frame=ELPHEL_GLOBALPARS(G_THIS_FRAME) + FRAME_DEAFAULT_AHEAD;
        frame=ELPHEL_GLOBALPARS(port, G_THIS_FRAME);  /// read current (most recent) frame - different from _set_
    }

    ///try framePars
    frame_index = frame & PARS_FRAMES_MASK;
    frame_stored= ((struct framepars_t *) ELPHEL_G(framePars[port]))[frame_index].pars[P_FRAME];
    if (frame_stored == frame) { /// This is what we are looking for
        if (full_addr & FRAMEPAIR_MASK_BYTES) {
            RETURN_LONG( FRAMEPAIR_FRAME_FIELD(full_addr,((struct framepars_t *) ELPHEL_G(framePars[port]))[frame_index].pars[addr]));
        } else {
            RETURN_LONG( ((struct framepars_t *) ELPHEL_G(framePars[port]))[frame_index].pars[addr]);
        }
    }
    if (frame_stored < frame) { /// too early for the frame number specified
        RETURN_NULL();
    }
    /// too late for framePars - let's try pastPars
    frame_index = frame & PASTPARS_SAVE_ENTRIES_MASK;
    frame_stored= ((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[frame_index].past_pars[P_FRAME-PARS_SAVE_FROM];
    if (frame_stored == frame) { /// This is what we are looking for
        addr-=PARS_SAVE_FROM;
        if ((addr<0) ||(addr>= (sizeof (struct framepars_past_t) >>2))) {
            RETURN_NULL();
        }
        if (full_addr & FRAMEPAIR_MASK_BYTES) {
            RETURN_LONG(FRAMEPAIR_FRAME_FIELD(full_addr,((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[frame_index].past_pars[addr]));
        } else {
            RETURN_LONG( ((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[frame_index].past_pars[addr]);
        }
    }
    /// too late, probably
    RETURN_NULL();
}

PHP_FUNCTION(elphel_test)
{
    long result= ELPHEL_GLOBALPARS(0, G_THIS_FRAME);
    RETURN_LONG(result);
} 

/** Return the number of sensor ports */
PHP_FUNCTION(elphel_num_sensors)
{
	long result = SENSOR_PORTS;
    RETURN_LONG(result);
}


///NOTE: Just for compatibility with older code
//!Get sensor state, usually 7- sensor is running,  compressor is stopped, 8 - compressor is runing
//!program sensor/compressor according to the parameters specified. Argument==0 - restart the sensor, 1 - do it "on the fly"
PHP_FUNCTION(elphel_get_state)
{
    long port, frame16, compressor_state, sensor_state, result;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &port) == FAILURE)
        RETURN_NULL();
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    frame16=ELPHEL_GLOBALPARS(port, G_THIS_FRAME) & PARS_FRAMES_MASK;
    compressor_state= ((struct framepars_t *) ELPHEL_G(framePars[port]))[frame16].pars[P_COMPRESSOR_RUN];
    sensor_state=     ((struct framepars_t *) ELPHEL_G(framePars[port]))[frame16].pars[P_SENSOR_RUN];
    result=(compressor_state==COMPRESSOR_RUN_CONT)?0x8:
            ((compressor_state==COMPRESSOR_RUN_SINGLE)?0xa:
                    ((sensor_state==SENSOR_RUN_CONT)?0x7:0));
    RETURN_LONG(result);
} 

/**
 * @brief return selected (by integer index) framepars structure  (struct framepars_t) as a binary string
 * @param port - sensor port (0..3)
 * @param index - frame index (0..7). -1 - return func2call page instead, -2 - globalPars
 * @return NULL - error, otherwise a string with  (struct framepars_t)
 *
 */

PHP_FUNCTION(elphel_framepars_get_raw)
{
    long port, index;
    char * packed_framepars_structure;
    index=0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l",&port,  &index) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong index");
        RETURN_NULL ();
    }
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    packed_framepars_structure= (char*) emalloc (sizeof (struct framepars_t));
    if (packed_framepars_structure) {
        /// use gamma_cache_index to retrieve table from cache
        if (index>=0) {
            memcpy(packed_framepars_structure, &(((struct framepars_t *) ELPHEL_G(framePars[port]))[index & PARS_FRAMES_MASK]),sizeof(struct framepars_t));
        } else if (index == -1) {
            memcpy(packed_framepars_structure,   ((struct framepars_t *) ELPHEL_G(funcs2call[port])),sizeof(struct framepars_t));
        } else if (index == -2) {
            memcpy(packed_framepars_structure,   ((struct framepars_t *) ELPHEL_G(globalPars[port])),sizeof(struct framepars_t));
        } else RETURN_NULL ();
        RETURN_STRINGL (packed_framepars_structure, sizeof(struct framepars_t), 0);
    }
    php_error_docref(NULL TSRMLS_CC, E_ERROR, "emalloc error");
    RETURN_NULL ();
}


//! This function reads associative array and uses the keys as a template for the result array.
//! If mey is one of the defined P_VALUE names (same as global constant but w/o "ELPHEL_" prefix)
//! then the result array will have element with the same key and the value equal to the value
//! of the camera parameter
/// TODO:Make it per frame (absolute), combine with past frames
/// frame absent - "current frame"
/// frame ==0 - frame zero (static?)
PHP_FUNCTION(elphel_get_P_arr)
{
    long port;
    long frame=-1;
    long frame_index=-1;
    int  future=1;
    int  frame_stored;
    long addr,full_addr,val;
    char full_constant_name[256];
    zval *arr, **data;
    HashTable *arr_hash;
    HashPosition pointer;
    char *key;
    int   key_len;
    long index;
    long constAddNumber; /// number to add to the ELPHEL_* constant value
    zval const_value;
    long multiMod;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "la|l",&port, &arr, &frame) == FAILURE) {
        RETURN_NULL();
    }
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    //    init_sens();
    /// first see if this frame is either in the future, past or is frame zero (parameters that are not related to frames)
    if (frame < 0) { /// frame number not provided - use latest
        frame=ELPHEL_GLOBALPARS(port,G_THIS_FRAME);
    }
    /// try future first
    frame_index = frame & PARS_FRAMES_MASK;
    frame_stored= ((struct framepars_t *) ELPHEL_G(framePars[port]))[frame_index].pars[P_FRAME];
    if (frame_stored <frame) { /// too early for the frame number specified
        future=-1 ; /// not available - only global parameters could be retrieved
    }
    if (frame_stored > frame) { /// Maybe it is in the past frames (only subset of parameters preserved)
        frame_index = frame & PASTPARS_SAVE_ENTRIES_MASK;
        frame_stored= ((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[frame_index].past_pars[P_FRAME-PARS_SAVE_FROM];
        if (frame_stored != frame) { /// Too late, probably - all the records are gone by now
            future=-1 ;// not available - only global parameters could be retrieved
        } else future=0; /// should be there, but in the past
    }

    array_init(return_value);
    arr_hash = Z_ARRVAL_P(arr);
    for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer);
            zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS;
            zend_hash_move_forward_ex(arr_hash, &pointer)) {
        if (zend_hash_get_current_key_ex(arr_hash, &key, &key_len, &index, 0, &pointer) == HASH_KEY_IS_STRING) {
            if (strlen(key)>(sizeof(full_constant_name)-8)) RETURN_NULL();
            sprintf (full_constant_name,"ELPHEL_%s",key);
            full_addr =-1;
            if (zend_get_constant(full_constant_name, strlen(full_constant_name), &const_value TSRMLS_CC)) { /// found the constant as is
                full_addr= Z_LVAL(const_value);
            } else {
                multiMod=parseMultiSens(full_constant_name); /// will truncate  full_constant_name if sensor number suffix found
                constAddNumber=splitConstantName(full_constant_name);
                //              php_printf ("constAddNumber=%d, full_constant_name=%s (length=%d)\n",constAddNumber, full_constant_name,strlen(full_constant_name));
                if ((constAddNumber>=0) && (zend_get_constant(full_constant_name, strlen(full_constant_name), &const_value TSRMLS_CC))) { /// Try to remove number from the end
                    full_addr= (Z_LVAL(const_value) & ~FRAMEPAIR_MASK_BYTES)+constAddNumber; /// FRAMEPAIR_MASK_BYTES to prevent bit-field modifier addition to constants that already have them
                    if ((multiMod>=0) && (full_addr != -1)) full_addr=applyMultiSens(full_addr,multiMod);
                }
            }
            if (full_addr!=-1) {
                addr=full_addr & 0xffff;
                /// is it a global parameter?
                if (addr >= FRAMEPAR_GLOBALS) {
                    if (addr < (FRAMEPAR_GLOBALS+P_MAX_GPAR)) {
                        if (full_addr & FRAMEPAIR_MASK_BYTES) {
                            add_assoc_long(return_value, key, FRAMEPAIR_FRAME_FIELD(full_addr,ELPHEL_GLOBALPARS(port, addr)));
                        } else {
                            add_assoc_long(return_value, key, ELPHEL_GLOBALPARS(port, addr));
                        }
                    }
                    /// is it in the future/latest?
                } else if (future>0) {
                    if ((addr >=0) && (addr < (sizeof (struct framepars_t) >>2))) {
                        if (full_addr & FRAMEPAIR_MASK_BYTES) {
                            add_assoc_long(return_value, key, FRAMEPAIR_FRAME_FIELD(full_addr,((struct framepars_t *) ELPHEL_G(framePars[port]))[frame_index].pars[addr]));
                        } else {
                            add_assoc_long(return_value, key, ((struct framepars_t *) ELPHEL_G(framePars[port]))[frame_index].pars[addr]);
                        }
                    }
                    /// is it saved in past parameters?
                } else if (future==0){
                    addr-=PARS_SAVE_FROM;
                    if ((addr >=0) && (addr <(sizeof (struct framepars_past_t) >>2))) {
                        if (full_addr & FRAMEPAIR_MASK_BYTES) {
                            add_assoc_long(return_value, key, FRAMEPAIR_FRAME_FIELD(full_addr,((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[frame_index].past_pars[addr]));
                        } else {
                            add_assoc_long(return_value, key, ((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[frame_index].past_pars[addr]);
                        }
                    }
                }
                zval_dtor(&const_value);
            }
        }
    }
}

/**
 * @brief common part of elphel_set_P_value() and elphel_compressor_*()
 * @param addr    register address (with possible flags)
 * @param data    data to write
 * @param frame   frame to write (<0) - use current + FRAME_DEAFAULT_AHEAD
 * @param flags   additional flags (0) - none
 * @return <0 - -errno ( error), otherwise frame used
 */

long elphel_set_P_value_common(long port, long addr, long data, long frame, long flags) {
    unsigned long write_data[4];
    long maddr;
    ///shortcut for global parameters - directly mmaped
    if ((port < 0) || (port > SENSOR_PORTS))
        return -1;
    maddr=addr & 0xffff;
    if (( (addr & 0xff00) != 0xff00 ) && (maddr >= FRAMEPAR_GLOBALS)) { /// these globals can be written just through mmap
        if (maddr >= (FRAMEPAR_GLOBALS+P_MAX_GPAR)) {
            return -1;
        }
        ELPHEL_GLOBALPARS(port, maddr)=data;
        return 0;
    }
    if (frame <0) {
        frame=ELPHEL_GLOBALPARS(port, G_THIS_FRAME) + FRAME_DEAFAULT_AHEAD;
    }
    flags |= (flags << 16); /// will accept flags both shifted and not shifted
    flags &=0xffff0000;
    if ((addr<0) ||((maddr >= (sizeof (struct framepars_t) >>2)) && ( (addr & 0xff00) != 0xff00 )  )) {
        return -1;
    }
    write_data[0]=FRAMEPARS_SETFRAME;
    write_data[1]=frame;
    write_data[2]= addr | flags;
    write_data[3]= data;
    long rslt=write(ELPHEL_G(fd_fparmsall[port]), write_data, sizeof(write_data));
    //    if (rslt<0) rslt =-errno;
    if (rslt<0) return -errno;
    if (rslt == sizeof( write_data )) return frame;
    return -1;
}


//! Set acquisition/compression parameters.
/// addr may include flags - addr|=(flags>>16)
/// UPDATE: return frame number to which parameters was set

PHP_FUNCTION(elphel_set_P_value)
{
    long port;
    long addr;
    long data;
    long frame=-1;
    unsigned long flags=0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll|ll", &port, &addr,&data,&frame,&flags) == FAILURE) {
        RETURN_NULL();
    }
    if (((frame=elphel_set_P_value_common (port, addr, data, frame, flags))) <0) {
        RETURN_NULL();
    }
    RETURN_LONG(frame);
}

//! Reset FPGA compressor - set to the same as elphel_compressor_stop()
PHP_FUNCTION(elphel_compressor_reset)
{
    long port;
    long frame=-1;
    unsigned long flags=-1;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|ll", &port, &frame,&flags) == FAILURE) {
        RETURN_NULL();
    }
    if (flags<0) flags=FRAMEPAIR_FORCE_NEWPROC;
    if (((frame=elphel_set_P_value_common (port, P_COMPRESSOR_RUN, COMPRESSOR_RUN_STOP, frame, flags)))<0) {
        RETURN_NULL();
    }
    RETURN_LONG(frame);
} 

//! Start FPGA compressor
PHP_FUNCTION(elphel_compressor_run)
{
    long port;
    long frame=-1;
    unsigned long flags=-1;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|ll", &port, &frame,&flags) == FAILURE) {
        RETURN_NULL();
    }
    if (flags<0) flags=FRAMEPAIR_FORCE_NEWPROC;
    if (((frame=elphel_set_P_value_common (port, P_COMPRESSOR_RUN, COMPRESSOR_RUN_CONT, frame, flags)))<0) {
        RETURN_NULL();
    }
    RETURN_LONG(frame);
} 

//! Stop FPGA compressor

PHP_FUNCTION(elphel_compressor_stop)
{
    long port;
    long frame=-1;
    unsigned long flags=-1;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|ll", &port, &frame,&flags) == FAILURE) {
        RETURN_NULL();
    }
    if (flags<0) flags=FRAMEPAIR_FORCE_NEWPROC;
    if (((frame=elphel_set_P_value_common (port, P_COMPRESSOR_RUN, COMPRESSOR_RUN_STOP, frame, flags)))<0) {
        RETURN_NULL();
    }
    RETURN_LONG(frame);
}

//! Acquire one frame to the buffer
PHP_FUNCTION(elphel_compressor_frame)
{
    long port;
    long frame=-1;
    unsigned long flags=-1;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|ll", &port, &frame,&flags) == FAILURE) {
        RETURN_NULL();
    }
    if (flags<0) flags=FRAMEPAIR_JUST_THIS;
    if (((frame=elphel_set_P_value_common (port, P_COMPRESSOR_RUN, COMPRESSOR_RUN_SINGLE, frame, flags)))<0) {
        RETURN_NULL();
    }
    RETURN_LONG(frame);
} 
//!reset sensor, and re-initialize it
PHP_FUNCTION(elphel_reset_sensor) {
    long port;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &port) == FAILURE) {
        RETURN_NULL();
    }
    lseek((int) ELPHEL_G(fd_fparmsall[port]), LSEEK_FRAMEPARS_INIT, SEEK_END ); /// reset all framepars and globalPars
    elphel_set_P_value_common (port, P_SENSOR, 0, 0, -1);
    RETURN_NULL();
}

//! This function reads associative array and writes values to the camera registers, using "ELPHEL_* constants"
//! to determine register address from the provided key in each key/value pair
//! All non-numerical values are ignored
//! Returns number of values written
///UPDATE:retuns frame number to which parameters were written
PHP_FUNCTION(elphel_set_P_arr)
{
    long port;
    char full_constant_name[256];
    zval *arr, **data;
    HashTable *arr_hash;
    HashPosition pointer;
    char *key;
    int   key_len;
    long index;
    zval const_value;
    int array_count;
    unsigned long * write_data=NULL;
    long frame=-1;
    long flags=0;
    int num_written=0;
    int num_mmap_written=0;
    int reg_addr, reg_data, constAddNumber;
    long multiMod;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "la|ll", &port, &arr,&frame,&flags) == FAILURE) {
        RETURN_LONG(num_written);
    }
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    if (frame <0) {
        frame=ELPHEL_GLOBALPARS(port, G_THIS_FRAME) + FRAME_DEAFAULT_AHEAD;
    }
    flags |= (flags << 16); /// will accept flags both shifted and not shifted
    flags &=0xffff0000;
    init_sens();
    arr_hash = Z_ARRVAL_P(arr);
    array_count = zend_hash_num_elements(arr_hash);
    ///allocate array to be written (8 bytes per value + 8)
    write_data=(unsigned long *) emalloc ((array_count+1)<<3);
    if (!write_data) RETURN_NULL(); /// emalloc failed

    write_data[0]=FRAMEPARS_SETFRAME;
    write_data[1]=frame;
    for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer);
            zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS;
            zend_hash_move_forward_ex(arr_hash, &pointer)) {
        if ((zend_hash_get_current_key_ex(arr_hash, &key, &key_len, &index, 0, &pointer) == HASH_KEY_IS_STRING) &&
                (Z_TYPE_PP(data) == IS_LONG)) {
            reg_data=Z_LVAL_PP(data);
            if (strlen(key)>(sizeof(full_constant_name)-8)) RETURN_NULL();
            sprintf (full_constant_name,"ELPHEL_%s",key);

            reg_addr =-1;
            if (zend_get_constant(full_constant_name, strlen(full_constant_name), &const_value TSRMLS_CC)) { /// found the constant as is
                reg_addr= Z_LVAL(const_value);
            } else {
                multiMod=parseMultiSens(full_constant_name); /// will truncate  full_constant_name if sensor number suffix found
                constAddNumber=splitConstantName(full_constant_name);
                if ((constAddNumber>=0) && (zend_get_constant(full_constant_name, strlen(full_constant_name), &const_value TSRMLS_CC))) { /// Try to remove number from the end
                    reg_addr= (Z_LVAL(const_value) & ~FRAMEPAIR_MASK_BYTES)+constAddNumber; /// FRAMEPAIR_MASK_BYTES to prevent bit-field modifier addition to constants that already have them
                    if ((multiMod>=0) && (reg_addr != -1)) reg_addr=applyMultiSens(reg_addr,multiMod);
                }
            }

            if (reg_addr>=0) {
                /// is it a global parameter?
                zval_dtor(&const_value); /// free resources used for constant value
                if (((reg_addr & 0xff00) != 0xff00 ) && ((reg_addr & 0xffff) >= FRAMEPAR_GLOBALS)) {  /// these globals can be written just through mmap
                    if ((reg_addr & 0xffff) < (FRAMEPAR_GLOBALS+P_MAX_GPAR)) { /// Fits in the range of the global parameters
                        if ((reg_addr & FRAMEPAIR_MASK_BYTES) ==0) { /// Full 32-bit writes - use mmap
                            ELPHEL_GLOBALPARS(port, reg_addr & 0xffff)=reg_data;
                            num_mmap_written++;
                        } else { /// only some bitfield is modified -  use (slower) write to have it atomic, no need to do bit field combining here
                            write_data[(num_written<<1) + 2]= reg_addr | flags;
                            write_data[(num_written<<1) + 3]= reg_data;
                            num_written++;
                        }
                    }
                } else if ((reg_addr>=0) && (((reg_addr & 0xffff) < (sizeof (struct framepars_t) >>2)) || ( (reg_addr & 0xff00) == 0xff00 )  )) {
                    write_data[(num_written<<1) + 2]= reg_addr | flags;
                    write_data[(num_written<<1) + 3]= reg_data;
                    num_written++;
                }
            }
        }
    }
    if (num_written) {
        long rslt=write(ELPHEL_G(fd_fparmsall[port]), write_data, (num_written+1)<<3);
        efree(write_data);
        if (rslt<0) RETURN_LONG(-errno);
        num_written=(rslt>>3) -1 ; ///actually written to driver
    }
    RETURN_LONG(frame);
}

/**
 * @brief Calculate gamma table (as array of 257 unsigned short values)
 * @param gamma - gamma value (1.0 - linear)
 * @param black - black level, 1.0 corresponds to 256 for 8bit values
 * @param gtable - gamma array reference (allocated by the caller)
 * @return 0 - OK, <0 - error
 */

int gamma_calc (double gamma, double black, unsigned short * gtable) {
    int i;
    double x, black256,k;
    int ig;
    black256=black*256.0;
    if (k>=1.0) k= k/256.0 ; /// just in case k is provided as a fraction of 256, not 1.0
    if (k>0.99) k=0.99;
    k=1.0/(256.0-black256);
    if (!gtable) return -1;
    ///Same 0.13 <= gamma <= 10.0 limits for gamma as used earlier
    if (gamma < 0.13) gamma=0.13;
    if (gamma >10.0)  gamma=10.0;
    for (i=0; i<257; i++) {
        x=k*(i-black256);
        if (x < 0.0 ) x=0.0;
        ig= 0.5+65535.0*pow(x,gamma);
        if (ig > 0xffff) ig=0xffff;
        gtable[i]=ig;
    }
    return 0;
}

/**
 * @brief Calculate new gamma table (specified by gamma value and black level) and put it into gamma cache
 * Gamma cache will be used to program gamma tables to FPGA, calculate derivative tables
 * Gamma tables should be loaded before used (gamma/black level/scale) specified as frame parameters 
 * gamma - floating point, <=1.0,  will be rounded to 0.01. Larger gammas are reseved for custom tables
 * black - floating point, <=1.0 or integer>1 (1..255) - level to subtract from sensor value
 * @return hash16 - ((gamma *100) & 0xff) | (((black * 256) 0xff) << 8)
 */
PHP_FUNCTION(elphel_gamma_add)
{
    unsigned short data_to_write[260];
    double gamma,black;
    int igamma, iblack, hash16;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &gamma, &black ) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong arguments");
        RETURN_NULL ();
    }
    igamma=100*gamma+0.5;
    if (igamma <   0) igamma=  0;
    if (igamma > 255) igamma=255;
    gamma=0.01*igamma;
    iblack= (black>=1.0)?black:(256*black+0.5);
    if (iblack <   0) iblack=  0;
    if (iblack > 254) iblack=254; /// don't use 255 - reserve it for custom tables
    black= (1.0/256.0) * iblack;
    hash16= igamma | (iblack<<8);
    data_to_write[0]= GAMMA_SCLALE_1; /// 1.0
    data_to_write[1]= hash16;
    ///- next 1 byte   [4]   - mode (1 - not_nice, 2 - need reverse, 4 - hardware)
    ///- next 1 byte   [5]   - color ( only if hardware bit in mode is set)
    data_to_write[2]=0;
    gamma_calc (gamma, black, &data_to_write[3]);
    long rslt=write(ELPHEL_G(fd_gamma_cache), data_to_write, sizeof(data_to_write));
    if (rslt<0) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Write to fd_gamma_cache returned errno=%d",errno);
        RETURN_LONG(-errno);
    }
    RETURN_LONG (hash16);
}

/**
 * @brief Load custom gamma table, tag it with hash16 (unsigned short).
 * Using lower byte of hash16 larger than 0x64 (100) or high byte of 0xff will prevent hash16 used by gamma/blacklevel pairs
 * @param hash16 - hash 16 that will be used to identify the table when setting parameters
 * @param zarray - array of 257 elements indexed as 0..256, in the range of 0.. 1.0 to be used for a table
 * @return   -1.. -998 - errno when writing to driver
 *         -998        - wrong arguments
 *         -999        - array length is not 257
 *        -1000..-1256 - missing element 0..256
 *        -2000..-2256 - non-numeric element 0..256
 *        >=0 - hash16
 */

PHP_FUNCTION(elphel_gamma_add_custom)
{
    unsigned short data_to_write[260];
    long  hash16;
    int i,d;
    zval *arr,  **data;
    HashTable *arr_hash;
    long array_count;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "la", &hash16, &arr ) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong arguments");
        RETURN_LONG (-998);
    }
    arr_hash = Z_ARRVAL_P(arr);
    array_count = zend_hash_num_elements(arr_hash);
    if (array_count != 257) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong array length (should be 257) - %d",array_count);
        RETURN_LONG (-999);
    }
    hash16 &= 0xffff;
    data_to_write[0]= GAMMA_SCLALE_1; /// 1.0
    data_to_write[1]= hash16;
    ///- next 1 byte   [4]   - mode (1 - not_nice, 2 - need reverse, 4 - hardware)
    ///- next 1 byte   [5]   - color - only if hardware bit in mode is set
    data_to_write[2]=0;
    /// now iterate through indexed array
    for (i=0; i<257; i++) {
        if (zend_hash_index_find(arr_hash, i, (void**)&data) == FAILURE) {
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "Missing array element [%d]",i);
            RETURN_LONG(-i-1000);
        }
        switch (Z_TYPE_PP(data)) {
        case IS_DOUBLE:
            d=Z_DVAL_PP(data);
            break;
        case IS_LONG:
            d=(1.0/255)* Z_LVAL_PP(data);
            break; // Was missing before NC393
        default:
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "Non-numeric array element [%d]",i);
            RETURN_LONG(-i-2000);
        }
        if (d<0)   d=0.0;
        if (d>1.0) d=1.0;
        data_to_write[i+3]=d*65535.0;
    }
    long rslt=write(ELPHEL_G(fd_gamma_cache), data_to_write, sizeof(data_to_write));
    if (rslt<0) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Write to fd_gamma_cache returned errno=%d",errno);
        RETURN_LONG(-errno);
    }
    RETURN_LONG (hash16);
}

/**
 * @brief Read loaded gamma table, optionally scale it and provide "raw" (integer) array
 * @param hash16 - unique ID (unsigned short ) of the table provided whan it was created. for regular gamma tables it is
 *                 ((gamma *100) & 0xff) | (((black * 256) 0xff) << 8)
 * @param scale  - optional scale will be applied to the gamma table. If provided scale is integer, it should be the same format
 *                 as  P_GTAB_* lower word - unsigned short, where  GAMMA_SCLALE_1 (0x400) is 1.0,
 *                 if it is floating point or integer 1 - it is "true" scale
 * @param raw    - 0: return array of floating point numbers, in the range 0..1.0, !=0 - array of unsigned short numbers
 * @return if OK - array of 257 numbers floating 0..1.0 or intefer 0..0xffff,
 *                negative integer - error
 *
 */

PHP_FUNCTION(elphel_gamma_get)
{
    //  double scale=1.0;
    unsigned short gtable[257];
    unsigned short data_to_write[3];
    long hash16;
    long raw=0;
    zval  *zscale=NULL;
    int iscale=GAMMA_SCLALE_1;
    int gamma_cache_index;
    int i;
    double gk=1.0/65535.0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|zl", &hash16, &zscale,&raw) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong arguments");
        RETURN_LONG (-998);
    }
    if (zscale) {
        switch (Z_TYPE_P(zscale)) {
        case IS_DOUBLE:
            iscale=(GAMMA_SCLALE_1 * Z_DVAL_P(zscale) +0.5);
            break;
        case IS_LONG:
            iscale = Z_LVAL_P(zscale);
            break;
        default:
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong scale type");
        }
    }
    if (iscale < 0)      iscale=0;
    if (iscale > 0xffff) iscale=0xffff;
    /// look for a table in cache
    data_to_write[0]= iscale; /// 0..0xffff
    data_to_write[1]= hash16;
    ///- next 1 byte   [4]   - mode (1 - not_nice, 2 - need reverse, 4 - hardware)
    ///- next 1 byte   [5]   - color - only if hardware bit in mode is set
    data_to_write[2]=0;
    long rslt=write(ELPHEL_G(fd_gamma_cache), data_to_write, sizeof(data_to_write));
    if (rslt<0) {
        //      php_error_docref(NULL TSRMLS_CC, E_ERROR, "Write to fd_gamma_cache returned errno=%d",errno);
        RETURN_LONG(-errno); /// i/o error/ table does not exist - "silent" error?
    }
    gamma_cache_index= lseek(ELPHEL_G(fd_gamma_cache), 0, SEEK_CUR);
    if (!gamma_cache_index) {
        RETURN_LONG (-997); /// requested table does not exist in the cache - "silent" error?
    }
    /// use gamma_cache_index to retrieve table from cache
    memcpy(gtable, &(((struct gamma_stuct_t *) ELPHEL_G(gamma_cache))[gamma_cache_index].direct[0]),sizeof(gtable));
    if (lseek(ELPHEL_G(fd_gamma_cache), LSEEK_GAMMA_ISCURRENT, SEEK_END)<=0) {
        RETURN_LONG (-996); /// requested table was overwritten - "silent" error?
    }
    /// convert gtable to PHP array
    array_init(return_value);
    if (raw) for (i=0;i<257;i++)  add_next_index_long  (return_value,    gtable[i]);
    else     for (i=0;i<257;i++)  add_next_index_double(return_value, gk*gtable[i]);
    /// will return array 'return_value'
}

/**
 * @brief Find loaded gamma table, optionally scale it and provide cache index (0 - not in cache)
 * @param hash16 - unique ID (unsigned short ) of the table provided whan it was created. for regular gamma tables it is
 *                 ((gamma *100) & 0xff) | (((black * 256) 0xff) << 8)
 * @param scale  - optional scale will be applied to the gamma table. If provided scale is integer, it should be the same format
 *                 as  P_GTAB_* lower word - unsigned short, where  GAMMA_SCLALE_1 (0x400) is 1.0,
 *                 if it is floating point or integer 1 - it is "true" scale
 * @return <0 - error, 0 - not in cache, >0 - cache index
 *
 */

PHP_FUNCTION(elphel_gamma_get_index)
{
    unsigned short data_to_write[3];
    long hash16;
    zval  *zscale=NULL;
    int iscale=GAMMA_SCLALE_1;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|z", &hash16, &zscale) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong arguments");
        RETURN_LONG (-998);
    }
    if (zscale) {
        switch (Z_TYPE_P(zscale)) {
        case IS_DOUBLE:
            iscale=(GAMMA_SCLALE_1 * Z_DVAL_P(zscale) +0.5);
            break;
        case IS_LONG:
            iscale = Z_LVAL_P(zscale);
            break;
        default:
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong scale type");
        }
    }
    if (iscale < 0)      iscale=0;
    if (iscale > 0xffff) iscale=0xffff;
    /// look for a table in cache
    data_to_write[0]= iscale; /// 0..0xffff
    data_to_write[1]= hash16;
    ///- next 1 byte   [4]   - mode (1 - not_nice, 2 - need reverse, 4 - hardware)
    ///- next 1 byte   [5]   - color - only if hardware bit in mode is set
    data_to_write[2]=0;
    long rslt=write(ELPHEL_G(fd_gamma_cache), data_to_write, sizeof(data_to_write));
    if (rslt<0) {
        //      php_error_docref(NULL TSRMLS_CC, E_ERROR, "Write to fd_gamma_cache returned errno=%d",errno);
        RETURN_LONG(-errno); /// i/o error/ table does not exist - "silent" error?
    }
    RETURN_LONG (lseek(ELPHEL_G(fd_gamma_cache), 0, SEEK_CUR));
}




/**
 * @brief return selected (by integer index) cached gamma structure  (struct gamma_stuct_t) as a binary string
 * @param index - gamma cache index
 * @return NULL - error, otherwise a string with  (struct gamma_stuct_t)
 *
 */

PHP_FUNCTION(elphel_gamma_get_raw)
{
    char * packed_gamma_structure;
    long index;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong index");
        RETURN_NULL ();
    }
    if (index >= GAMMA_CACHE_NUMBER) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong index (%d >= %d)",index, (int) GAMMA_CACHE_NUMBER);
        RETURN_NULL ();
    }
    packed_gamma_structure= (char*) emalloc (sizeof(struct gamma_stuct_t));
    if (packed_gamma_structure) {
        /// use gamma_cache_index to retrieve table from cache
        memcpy(packed_gamma_structure, &(((struct gamma_stuct_t *) ELPHEL_G(gamma_cache))[index]),sizeof(struct gamma_stuct_t));
        if (index==0) { /// just debug
            packed_gamma_structure[sizeof(struct gamma_stuct_t)-1]=0xff;
        }
        RETURN_STRINGL (packed_gamma_structure, sizeof(struct gamma_stuct_t), 0);
    }
    php_error_docref(NULL TSRMLS_CC, E_ERROR, "emalloc error");
    RETURN_NULL ();
}
//     memcpy(packed_framepars_structure, &(((struct framepars_t *) ELPHEL_G(framePars))[index]),sizeof(struct framepars_t));
///TODO: elphel_gamma_get_reverse - just testing? or is raw enough? yes, only _raw
///TODO: elphel_gamma_get_fpga  - no, not needed, could use binary string if really wanted

/**
 * @brief return selected (by integer index) cached histogram structure  (struct histogram_stuct_t) as a binary string
 * @param port - sensor port (0..3)
 * @param sub_chn - sensor sub-channel (for mux-ed sensors), NC393 initially ignored !
 * @param index - histogram cache index (or frame number, 3 lsb will be used)
 * @return NULL - error, otherwise a string with  (struct histogram_stuct_t)
 *
 */

PHP_FUNCTION(elphel_histogram_get_raw)
{
    long port, sub_chn;
    char * packed_histogram_structure;
    long frame=-1;
    long needed=0xfff;
    long index;
    long total_hist_entries;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll|ll", &port, &sub_chn, &needed, &frame) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong index");
        RETURN_NULL ();
    }
    if ((port <0)    || (port >=   SENSOR_PORTS)) RETURN_NULL ();
    if ((sub_chn <0) || (sub_chn >= MAX_SENSORS)) RETURN_NULL ();
//    if (frame<0) frame=lseek((int) ELPHEL_G( fd_fparmsall[port]), 0, SEEK_CUR );
    if (frame <0) {
        frame=ELPHEL_GLOBALPARS(port, G_THIS_FRAME)-1;
    }

    needed &= 0xfff;
    total_hist_entries = lseek(ELPHEL_G(fd_histogram_cache), LSEEK_HIST_SET_CHN + (4 * port) + sub_chn, SEEK_END); /// specify port/sub-channel is needed
    lseek(ELPHEL_G(fd_histogram_cache), LSEEK_HIST_WAIT_C, SEEK_END); /// wait for all histograms, not just Y (G1)
    lseek(ELPHEL_G(fd_histogram_cache), LSEEK_HIST_NEEDED + (needed & 0xff0), SEEK_END); /// mask out needed raw (fpga) bits
    index=lseek(ELPHEL_G(fd_histogram_cache), frame, SEEK_SET);    /// request histograms for frame=frame, wait until available if needed
    if (index <0) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Requested histograms are not available (frame=%d, needed=0x%x)",frame,needed);
        RETURN_NULL ();
    }
    if (index >= total_hist_entries) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Internal error: frame=%d, index=%d >= %d (total_hist_entries)",frame, index, total_hist_entries);
        RETURN_NULL ();
    }
    packed_histogram_structure= (char*) emalloc (sizeof(struct histogram_stuct_t));
    if (!packed_histogram_structure) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "emalloc error");
        RETURN_NULL ();
    }
    /// use gamma_cache_index to retrieve table from cache
    memcpy(packed_histogram_structure, &(((struct histogram_stuct_t *) ELPHEL_G(histogram_cache))[index]),sizeof(struct histogram_stuct_t));
    RETURN_STRINGL (packed_histogram_structure, sizeof(struct histogram_stuct_t), 0);
}

/**
 * @brief return histograms for selected frame (absolute frame number) as a plain integer array
 * some derivative histograms may be calculated if they do not exist yet
 * (raw FPGA data should be read from IRQ/tasklet - no attempt to re-read FPGA here)
 * @param port - sensor port (0..3)
 * @param sub_chn - sensor sub-channel (for mux-ed sensors), NC393 initially ignored !
 * @param frame - absolute frame number
 * @param needed - bitmask specifying what tables to include in the output array, each group is in the order r(0),g(1),gb(2),b(3)
 * - bits 0..3 - raw histograms from the FPGA (only if they are already in the cache)
 * - bits 4..7 - cumulative histograms (sum of raw ones) - normally called from applications
 * - bits 8..11 - calculate percentiles (reverse cumulative histograms) - normally called from applications
 * @return NULL - error, otherwise a string with  (struct histogram_stuct_t)
 *
 */

PHP_FUNCTION(elphel_histogram_get)
{
    long port, sub_chn;
    struct histogram_stuct_t * frame_histogram_structure;
    long frame=-1;
    long needed=0xfff;
    long index;
    long total_hist_entries;
    int i;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll|ll", &port, &sub_chn, &needed, &frame) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong index");
        RETURN_NULL ();
    }
    if ((port <0)    || (port >=   SENSOR_PORTS)) RETURN_NULL ();
    if ((sub_chn <0) || (sub_chn >= MAX_SENSORS)) RETURN_NULL ();

    //    if (frame<0) frame=lseek((int) ELPHEL_G( fd_fparmsall[port]), 0, SEEK_CUR );
    if (frame <0) {
        frame=ELPHEL_GLOBALPARS(port, G_THIS_FRAME)-1;
    }
    needed &= 0xfff;
    total_hist_entries= lseek(ELPHEL_G(fd_histogram_cache), LSEEK_HIST_SET_CHN + (4 * port) + sub_chn, SEEK_END); /// specify port/sub-channel is needed

    lseek(ELPHEL_G(fd_histogram_cache), LSEEK_HIST_WAIT_C, SEEK_END); // / wait for all histograms, not just Y (G1)
    lseek(ELPHEL_G(fd_histogram_cache), LSEEK_HIST_NEEDED + (needed & 0xff0), SEEK_END); // / mask out needed raw (fpga) bits
    index=lseek(ELPHEL_G(fd_histogram_cache), frame, SEEK_SET);    /// request histograms for frame=frame, wait until available if needed
    if (index <0) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Requested histograms are not available (frame=%d, needed=0x%x)",frame,needed);
        RETURN_NULL ();
    }
    if (index >= total_hist_entries) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Internal error: frame=%d, index=%d >= %d (total_hist_entries)",frame, index, total_hist_entries);
        RETURN_NULL ();
    }

    frame_histogram_structure= (struct histogram_stuct_t *) emalloc (sizeof(struct histogram_stuct_t));
    if (!frame_histogram_structure) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "emalloc error");
        RETURN_NULL ();
    }
    /// use gamma_cache_index to retrieve table from cache
    memcpy((void *) frame_histogram_structure, &(((struct histogram_stuct_t *) ELPHEL_G(histogram_cache))[index]),sizeof(struct histogram_stuct_t));
    /// verify that histogram is still valid
    if (frame != ((struct histogram_stuct_t *) ELPHEL_G(histogram_cache))[index].frame ) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Frame changed while retrieving histograms (frame requested=%d, received=%d, index=%d)",
                frame, (int)((struct histogram_stuct_t *) ELPHEL_G(histogram_cache))[index].frame, index);
        RETURN_NULL ();
        efree (frame_histogram_structure);
    }
    /// verify that selected tables are valid
    if ((needed & frame_histogram_structure->valid ) != needed) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Not all the requested tables are available (frame=%d needed=0x%x, valid=0x%x)",
                frame, needed, frame_histogram_structure->valid);
        RETURN_NULL ();
        efree (frame_histogram_structure);
    }
    /// TODO: make array with subarrays for each individual table/color?

    array_init(return_value);
    if (needed & 0x001)  for (i=0;i<256;i++)  add_next_index_long  (return_value,   frame_histogram_structure->hist_r[i]);
    if (needed & 0x002)  for (i=0;i<256;i++)  add_next_index_long  (return_value,   frame_histogram_structure->hist_g[i]);
    if (needed & 0x004)  for (i=0;i<256;i++)  add_next_index_long  (return_value,   frame_histogram_structure->hist_gb[i]);
    if (needed & 0x008)  for (i=0;i<256;i++)  add_next_index_long  (return_value,   frame_histogram_structure->hist_b[i]);

    if (needed & 0x010)  for (i=0;i<256;i++)  add_next_index_long  (return_value,   frame_histogram_structure->cumul_hist_r[i]);
    if (needed & 0x020)  for (i=0;i<256;i++)  add_next_index_long  (return_value,   frame_histogram_structure->cumul_hist_g[i]);
    if (needed & 0x040)  for (i=0;i<256;i++)  add_next_index_long  (return_value,   frame_histogram_structure->cumul_hist_gb[i]);
    if (needed & 0x080)  for (i=0;i<256;i++)  add_next_index_long  (return_value,   frame_histogram_structure->cumul_hist_b[i]);

    if (needed & 0x100)  for (i=0;i<256;i++)  add_next_index_long  (return_value,   frame_histogram_structure->percentile_r[i]);
    if (needed & 0x200)  for (i=0;i<256;i++)  add_next_index_long  (return_value,   frame_histogram_structure->percentile_g[i]);
    if (needed & 0x400)  for (i=0;i<256;i++)  add_next_index_long  (return_value,   frame_histogram_structure->percentile_gb[i]);
    if (needed & 0x800)  for (i=0;i<256;i++)  add_next_index_long  (return_value,   frame_histogram_structure->percentile_b[i]);

    efree (frame_histogram_structure);
    /// will return array 'return_value'
}


///=================================
long createExifDirectory (int rebuild) { /// build directory of pointers in the Exif data for some of the (variable) fields used in the Elphel cameras
    int indx;
    long numfields=0;
    struct exif_dir_table_t dir_table_entry;
    ///  Read the size  of the Exif data
//    php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d: createExifDirectory(%d)\n",__LINE__,rebuild);

    int exif_this_size=lseek((int) ELPHEL_G(fd_exifdir),1,SEEK_END); // at the beginning of page 1 - position == page length

//    php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d: exif_this_size = 0x%x, ELPHEL_G(exif_size)= 0x%x\n",
//            __LINE__,exif_this_size,ELPHEL_G(exif_size));

    if ((ELPHEL_G(exif_size) == exif_this_size) && !rebuild) return 0; // no need to rebuild
    ELPHEL_G(exif_size) = exif_this_size;

    for (indx=0; indx<ExifKmlNumber; indx++) ELPHEL_G(exif_dir)[indx].ltag=0;
    while (read((int) ELPHEL_G(fd_exifdir), &dir_table_entry, sizeof(dir_table_entry))>0) {
        switch (dir_table_entry.ltag) {
        case Exif_Image_ImageDescription:      indx= Exif_Image_ImageDescription_Index; break;
        //         case Exif_Image_FrameNumber:           indx= Exif_Image_FrameNumber_Index; break;
        case Exif_Photo_DateTimeOriginal:      indx= Exif_Photo_DateTimeOriginal_Index; break;
        case Exif_Photo_SubSecTimeOriginal:    indx= Exif_Photo_SubSecTimeOriginal_Index; break;
        case Exif_Photo_ExposureTime:          indx= Exif_Photo_ExposureTime_Index; break;
        case Exif_Photo_MakerNote:             indx= Exif_Photo_MakerNote_Index; break;
        case Exif_Image_Orientation:           indx= Exif_Image_Orientation_Index; break;
        case Exif_GPSInfo_GPSLatitudeRef:      indx= Exif_GPSInfo_GPSLatitudeRef_Index; break;
        case Exif_GPSInfo_GPSLatitude:         indx= Exif_GPSInfo_GPSLatitude_Index ; break;
        case Exif_GPSInfo_GPSLongitudeRef:     indx= Exif_GPSInfo_GPSLongitudeRef_Index ; break;
        case Exif_GPSInfo_GPSLongitude:        indx= Exif_GPSInfo_GPSLongitude_Index; break;
        case Exif_GPSInfo_GPSAltitudeRef:      indx= Exif_GPSInfo_GPSAltitudeRef_Index; break;
        case Exif_GPSInfo_GPSAltitude:         indx= Exif_GPSInfo_GPSAltitude_Index; break;
        case Exif_GPSInfo_GPSTimeStamp:        indx= Exif_GPSInfo_GPSTimeStamp_Index; break;
        case Exif_GPSInfo_GPSDateStamp:        indx= Exif_GPSInfo_GPSDateStamp_Index; break;
        case Exif_GPSInfo_GPSMeasureMode:      indx= Exif_GPSInfo_GPSMeasureMode_Index; break;
        case Exif_GPSInfo_CompassDirectionRef: indx= Exif_GPSInfo_CompassDirectionRef_Index; break;
        case Exif_GPSInfo_CompassDirection:    indx= Exif_GPSInfo_CompassDirection_Index; break;
        case Exif_GPSInfo_CompassPitchRef:     indx= Exif_GPSInfo_CompassPitchRef_Index; break;
        case Exif_GPSInfo_CompassPitch:        indx= Exif_GPSInfo_CompassPitch_Index; break;
        case Exif_GPSInfo_CompassRollRef:      indx= Exif_GPSInfo_CompassRollRef_Index; break;
        case Exif_GPSInfo_CompassRoll:         indx= Exif_GPSInfo_CompassRoll_Index; break;
        case Exif_Image_ImageNumber:           indx= Exif_Image_ImageNumber_Index; break;
        case Exif_Image_PageNumber:            indx= Exif_Image_PageNumber_Index; break;
        //         case Exif_Photo_MakerNote:             indx= Exif_Photo_MakerNote_Index; break;

        default: indx=-1;
        }
        if (indx>=0) {
            memcpy(&(ELPHEL_G(exif_dir)[indx]),&dir_table_entry,sizeof(dir_table_entry));

            numfields++;
        }
//        php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d: indx= 0x%02x ltag= 0x%08lx len = 0x%08lx  src = 0x%08lx  dst = 0x%08lx\n",
//                __LINE__, indx, dir_table_entry.ltag, dir_table_entry.len, dir_table_entry.src, dir_table_entry.dst);


    }
    return numfields;
}


///TODO: make reverse order, specify how many frames wanted. So first will be most reliable
PHP_FUNCTION(elphel_get_circbuf_pointers) {
    long port;
    char * ccam_dma_buf_char;
    long second=0;
    long p,frameParamPointer;
    long buff_size; //size of circbuf
    long meta_index,displacementInPage,exifPageStart,frame_be;
    long debug_numfields;

    zval *image_pointers;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &port, &second) == FAILURE) {
        RETURN_NULL();
    }
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    ccam_dma_buf_char= (char *) ELPHEL_G(ccam_dma_buf[port]);


    buff_size=lseek(ELPHEL_G( fd_circ[port]),0,SEEK_END); //size of circbuf

    ///Frame number is in Exif structure
    debug_numfields=createExifDirectory(0); /// make sure directory is current
    if (ELPHEL_G(exif_dir)[Exif_Image_ImageNumber_Index].ltag==Exif_Image_ImageNumber) /// Exif_Image_FrameNumber_Index is present in template
        displacementInPage=ELPHEL_G(exif_dir)[Exif_Image_ImageNumber_Index].dst;
    else
        displacementInPage=-1; /// no frame number in Exif

//    php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d: displacementInPage = 0x%x, buff_size= 0x%x\n",
//            __LINE__,displacementInPage, buff_size);

    p=lseek((int) ELPHEL_G( fd_circ[port]), second? LSEEK_CIRC_SCND: LSEEK_CIRC_FIRST, SEEK_END );
    if (p<0) RETURN_NULL();
    array_init(return_value);
    while (p>=0) {
        frameParamPointer=p-32;
        if (frameParamPointer<0) frameParamPointer+=buff_size;
        ALLOC_INIT_ZVAL(image_pointers);
        array_init(image_pointers);
        add_assoc_long(image_pointers, "circbuf_pointer", p);
        //      add_assoc_long(image_pointers, "exif_pointer", ((struct interframe_params_t *) &ccam_dma_buf_char[frameParamPointer])->meta_index);
        meta_index=((struct interframe_params_t *) &ccam_dma_buf_char[frameParamPointer])->meta_index;
        add_assoc_long(image_pointers, "exif_pointer", meta_index);
        /// Find out frame number...
        if (displacementInPage>=0){
            exifPageStart=lseek ((int) ELPHEL_G(fd_exif[port]), meta_index, SEEK_END); /// select specified Exif page

            lseek (ELPHEL_G(fd_exif[port]), exifPageStart+displacementInPage, SEEK_SET);
            read(ELPHEL_G(fd_exif[port]), &frame_be, 4);
            ///... and add it to the output array
            add_assoc_long(image_pointers, "frame", (long) __cpu_to_be32(frame_be));
#define DEBUG_BYRSH
#ifdef DEBUG_BYRSH
            int frame=  (int)  __cpu_to_be32(frame_be);
            int past_index=   frame & PASTPARS_SAVE_ENTRIES_MASK;
            add_assoc_long(image_pointers, "dbg_comp_frame16",      ((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[past_index].past_pars[PARS_SAVE_COPY + 0]);
            add_assoc_long(image_pointers, "dbg_comp_aframe",      ((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[past_index].past_pars[PARS_SAVE_COPY + 1]);
            add_assoc_long(image_pointers, "dbg_cmprs_mode",      ((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[past_index].past_pars[PARS_SAVE_COPY + 2]);
            add_assoc_long(image_pointers, "dbg_past_pars",      ((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[past_index].past_pars[PARS_SAVE_COPY + 3]);
            add_assoc_long(image_pointers, "dbg_frame",          frame);
            add_assoc_long(image_pointers, "dbg_past_index",      past_index);
            add_assoc_long(image_pointers, "dbg_past_pars_0",     ((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[past_index].past_pars[0]);
#endif
//            add_assoc_long(image_pointers, "dbg_exifPageStart",      exifPageStart);
//            add_assoc_long(image_pointers, "dbg_displacementInPage", displacementInPage);
//            add_assoc_long(image_pointers, "dbg_frame_be",           frame_be);
//            add_assoc_long(image_pointers, "debug_numfields",        debug_numfields);

        }
        add_next_index_zval(return_value, image_pointers);
        /// next frame
        p=lseek((int) ELPHEL_G( fd_circ[port]), LSEEK_CIRC_NEXT, SEEK_END );
        p=lseek((int) ELPHEL_G( fd_circ[port]), LSEEK_CIRC_READY, SEEK_END );
    }
}


PHP_FUNCTION(elphel_get_interframe_meta)
{
    long port;
    char * ccam_dma_buf_char;
    struct interframe_params_t frame_params;
    long circbuf_pointer=-1;
    long frameParamPointer,jpeg_len,timestamp_start;
    //   long circbuf_size=ELPHEL_GLOBALPARS(G_CIRCBUFSIZE);
    long circbuf_size;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &port, &circbuf_pointer) == FAILURE) {
        RETURN_NULL();
    }
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    circbuf_size=ELPHEL_G(ccam_dma_buf_len[port]);
    ccam_dma_buf_char= (char *) ELPHEL_G( ccam_dma_buf[port]);
    frameParamPointer=circbuf_pointer-32;
    if (frameParamPointer < 0) frameParamPointer+=circbuf_size;
    memcpy (&frame_params, &ccam_dma_buf_char[frameParamPointer],32);
    jpeg_len=frame_params.frame_length;
    ///Copy timestamp (goes after the image data)
    timestamp_start=circbuf_pointer+((jpeg_len+CCAM_MMAP_META+3) & (~0x1f)) + 32 - CCAM_MMAP_META_SEC; //! magic shift - should index first byte of the time stamp
    if (timestamp_start >= circbuf_size) timestamp_start-=circbuf_size;
    memcpy (&(frame_params.timestamp_sec), &ccam_dma_buf_char[timestamp_start],8);
    if (frame_params.signffff !=0xffff) {
        RETURN_NULL();
    }
    array_init(return_value);
    /// This data will survive as long as the frame itself in the circular buffer. Some other fields (like exposure) are stored in Exif
    add_assoc_long(return_value, "hash32_r",      frame_params.hash32_r); /// red color black level (MSB,gamma- bits 16..24, scale - bits 0..15)
    add_assoc_long(return_value, "hash32_g",      frame_params.hash32_g);
    add_assoc_long(return_value, "hash32_gb",     frame_params.hash32_gb);
    add_assoc_long(return_value, "hash32_b",      frame_params.hash32_b);
    /// Quality is represented by 2-byte value. Each byte uses Y table if the value is Q<128,// 16-17
    /// and C table with (Q-128) if it is Q>=128.
    /// If the High byte is zero, it is treated as Q^0x80  (Q|=(Q^0x80)<<8) for compatibility
    /// with a standard single-byte Q value
    add_assoc_long(return_value, "quality2",      frame_params.quality2);
    add_assoc_long(return_value, "color",         frame_params.color);      /// color mode //18
    add_assoc_long(return_value, "byrshift",      frame_params.byrshift);   /// bayer shift in compressor //19
    add_assoc_long(return_value, "width",         frame_params.width);      /// frame width, pixels   20-21 - NOTE: should be 20-21
    add_assoc_long(return_value, "height",        frame_params.height);     /// frame height, pixels  22-23
    add_assoc_long(return_value, "meta_index",    frame_params.meta_index); /// Exif page number (>0)
    add_assoc_long(return_value, "timestamp_sec", frame_params.timestamp_sec);  /// number of seconds since 1970 to the start of the frame exposure
    add_assoc_long(return_value, "timestamp_usec",frame_params.timestamp_usec); /// number of microseconds to add to seconds
}


#define saferead255(f,d,l) read(f,d,((l)<256)?(l):255)
PHP_FUNCTION(elphel_get_exif_elphel)
{
    long port;
    long makerNote[16];
    long rational3[6];
    long exif_page_start;
    char *key;
    long indx;
    char val[256];
    long exif_page=0;
    int hours=0, minutes=0;
    double seconds=0.0;
    double  longitude=0.0, latitude=0.0,  altitude=0.0,  heading=0.0,  roll=0.0, pitch=0.0, exposure=0.0;
    val[255]='\0';
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &port, &exif_page) == FAILURE) {
        RETURN_NULL();
    }
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();

    createExifDirectory(0); /// make sure directory is current
    if (exif_page) exif_page_start=lseek ((int) ELPHEL_G(fd_exif[port]), exif_page, SEEK_END); /// select specified Exif page
    else           exif_page_start=lseek ((int) ELPHEL_G(fd_exif[port]), 0, SEEK_SET); /// Select 0 (currently being acquired) Exif page
    if (exif_page_start<0) RETURN_NULL(); //exif_page may be out of range
//    php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d: exif_page = 0x%x\n",  __LINE__,exif_page);
//    php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d: exif_page_start = 0x%x\n",  __LINE__,exif_page_start);
//    php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d: Exif_Image_ImageNumber_Index = 0x%x\n",  __LINE__,Exif_Image_ImageNumber_Index);
//    php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d:  ELPHEL_G(exif_dir)[Exif_Image_ImageNumber_Index].dst = 0x%x\n",  __LINE__, ELPHEL_G(exif_dir)[Exif_Image_ImageNumber_Index].dst);
//    php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d: exif_page_start+ELPHEL_G(exif_dir)[Exif_Image_ImageNumber_Index].dst = 0x%x\n",  __LINE__,exif_page_start+ELPHEL_G(exif_dir)[Exif_Image_ImageNumber_Index].dst);

    array_init(return_value);
//    /exif_page_start
    ///Image Description
//    add_assoc_string(return_value, "dbg_exif_page",       exif_page,  1);
//    add_assoc_string(return_value, "dbg_exif_page_start", exif_page_start,  1);
//    add_assoc_string(return_value, "dbg_Exif_Image_ImageNumber_Index", Exif_Image_ImageNumber_Index,  1);
//    add_assoc_string(return_value, "dbg_dst", ELPHEL_G(exif_dir)[Exif_Image_ImageNumber_Index].dst,  1);
//    add_assoc_string(return_value, "dbg_offs", exif_page_start+ELPHEL_G(exif_dir)[Exif_Image_ImageNumber_Index].dst,  1);

    if (ELPHEL_G(exif_dir)[Exif_Image_ImageDescription_Index].ltag==Exif_Image_ImageDescription) { // Exif_Image_ImageDescription is present in template
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_Image_ImageDescription_Index].dst,
                SEEK_SET);
        saferead255(ELPHEL_G(fd_exif[port]), val, ELPHEL_G(exif_dir)[Exif_Image_ImageDescription_Index].len);
        add_assoc_string(return_value, "ImageDescription", val,  1);
    }
    ///Exif_Image_FrameNumber_Index           0x13
    if (ELPHEL_G(exif_dir)[Exif_Image_ImageNumber_Index].ltag==Exif_Image_ImageNumber) { // Exif_Image_FrameNumber_Index is present in template
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_Image_ImageNumber_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), rational3, 4);
        sprintf (val,"%ld", (long) __cpu_to_be32( rational3[0]));
        add_assoc_string(return_value, "FrameNumber", val,  1);
    }

    ///Exif_Image_PageNumber_Index           0x15 - mostly for testing - should be == port
    if (ELPHEL_G(exif_dir)[Exif_Image_PageNumber_Index].ltag==Exif_Image_PageNumber) { // Exif_Image_PageNumber_Index is present in template
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_Image_PageNumber_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), rational3, 4);
        sprintf (val,"%ld", (long) __cpu_to_be32( rational3[0]));
        add_assoc_string(return_value, "FramePage", val,  1);
    }

    ///Exif_Image_Orientation_Index           0x15
    if (ELPHEL_G(exif_dir)[Exif_Image_Orientation_Index].ltag==Exif_Image_Orientation) { // Exif_Image_Orientation_Index is present in template
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_Image_Orientation_Index].dst,
                SEEK_SET);
        rational3[0]=0;
        read(ELPHEL_G(fd_exif[port]), rational3, 2);
        sprintf (val,"%ld", (long) ( rational3[0]>>8));
        add_assoc_string(return_value, "Orientation", val,  1);
    }

    ///DateTimeOriginal (with subseconds)
    if (ELPHEL_G(exif_dir)[Exif_Photo_DateTimeOriginal_Index].ltag==Exif_Photo_DateTimeOriginal) {
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_Photo_DateTimeOriginal_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), val, 19);
        val[19]='\0';
        if (ELPHEL_G(exif_dir)[Exif_Photo_SubSecTimeOriginal_Index].ltag==Exif_Photo_SubSecTimeOriginal) {
            val[19]='.';
            lseek (ELPHEL_G(fd_exif[port]),
                    exif_page_start+ELPHEL_G(exif_dir)[Exif_Photo_SubSecTimeOriginal_Index].dst,
                    SEEK_SET);
            read(ELPHEL_G(fd_exif[port]), &val[20], 7);
            val[27]='\0';
        }
        add_assoc_string(return_value, "DateTimeOriginal", val,  1);
    }

    ///Exif_Photo_ExposureTime
    if (ELPHEL_G(exif_dir)[Exif_Photo_ExposureTime_Index].ltag==Exif_Photo_ExposureTime) { // Exif_Photo_ExposureTime is present in template
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_Photo_ExposureTime_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), rational3, 8);
        exposure=(1.0*__cpu_to_be32( rational3[0]))/__cpu_to_be32( rational3[1]);
        sprintf (val,"%f",exposure);
        add_assoc_string(return_value, "ExposureTime", val,  1);
    }

    ///Exif_Photo_MakerNote
    if (ELPHEL_G(exif_dir)[Exif_Photo_MakerNote_Index].ltag==Exif_Photo_MakerNote) { // Exif_Photo_MakerNote is present in template
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_Photo_MakerNote_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), makerNote, 64);
        sprintf (val,"0x%08lx,0x%08lx,0x%08lx,0x%08lx,0x%08lx,0x%08lx,0x%08lx,0x%08lx,0x%08lx,0x%08lx,0x%08lx,0x%08lx,0x%08lx,0x%08lx,0x%08lx,0x%08lx",
                (long) __cpu_to_be32(makerNote[0]),
                (long) __cpu_to_be32(makerNote[1]),
                (long) __cpu_to_be32(makerNote[2]),
                (long) __cpu_to_be32(makerNote[3]),
                (long) __cpu_to_be32(makerNote[4]),
                (long) __cpu_to_be32(makerNote[5]),
                (long) __cpu_to_be32(makerNote[6]),
                (long) __cpu_to_be32(makerNote[7]),
                (long) __cpu_to_be32(makerNote[8]),
                (long) __cpu_to_be32(makerNote[9]),
                (long) __cpu_to_be32(makerNote[10]),
                (long) __cpu_to_be32(makerNote[11]),
                (long) __cpu_to_be32(makerNote[12]),
                (long) __cpu_to_be32(makerNote[13]),
                (long) __cpu_to_be32(makerNote[14]),
                (long) __cpu_to_be32(makerNote[15]));
        add_assoc_string(return_value, "MakerNote", val,  1);
    }

    /// GPS measure mode
    if (ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSMeasureMode_Index].ltag==Exif_GPSInfo_GPSMeasureMode) {
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSMeasureMode_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), val, 1);
        val[1]='\0';
        add_assoc_stringl(return_value, "GPSMeasureMode", val, 1,  1);
    }

    ///GPS date/time
    if (ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSDateStamp_Index].ltag==Exif_GPSInfo_GPSDateStamp) {
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSDateStamp_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), val, 10);
        val[10]='\0';
        if (ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSTimeStamp_Index].ltag==Exif_GPSInfo_GPSTimeStamp) {
            lseek (ELPHEL_G(fd_exif[port]),
                    exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSTimeStamp_Index].dst,
                    SEEK_SET);
            read(ELPHEL_G(fd_exif[port]), rational3, 24);
            hours=   __cpu_to_be32( rational3[0]);
            minutes= __cpu_to_be32( rational3[2]);
            seconds= (1.0*(__cpu_to_be32( rational3[4])+1))/__cpu_to_be32( rational3[5]); /// GPS likes ".999", let's inc by one - anyway will round that out
            sprintf (&val[10]," %02d:%02d:%05.2f",hours,minutes,seconds);
        }
        add_assoc_string(return_value, "GPSDateTime", val,  1);
    }

    /// knowing format provided from GPS - degrees and minutes only, no seconds:
    ///GPS Longitude
    if (ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSLongitude_Index].ltag==Exif_GPSInfo_GPSLongitude) { // Exif_GPSInfo_GPSLongitude is present in template
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSLongitude_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), rational3, 24);
        longitude=__cpu_to_be32( rational3[0])/(1.0*__cpu_to_be32( rational3[1])) + __cpu_to_be32( rational3[2])/(60.0*__cpu_to_be32( rational3[3]));
        if (ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSLongitudeRef_Index].ltag==Exif_GPSInfo_GPSLongitudeRef) {
            lseek (ELPHEL_G(fd_exif[port]),
                    exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSLongitudeRef_Index].dst,
                    SEEK_SET);
            read(ELPHEL_G(fd_exif[port]), val, 1);
            if (val[0]!= 'E') longitude=-longitude;
        }
        sprintf (val,"%f",longitude);
        add_assoc_string(return_value, "GPSLongitude", val,  1);
    }
    ///GPS Latitude
    if (ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSLatitude_Index].ltag==Exif_GPSInfo_GPSLatitude) { // Exif_GPSInfo_GPSLatitude is present in template
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSLatitude_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), rational3, 24);
        latitude=__cpu_to_be32( rational3[0])/(1.0*__cpu_to_be32( rational3[1])) + __cpu_to_be32( rational3[2])/(60.0*__cpu_to_be32( rational3[3]));
        if (ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSLatitudeRef_Index].ltag==Exif_GPSInfo_GPSLatitudeRef) {
            lseek (ELPHEL_G(fd_exif[port]),
                    exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSLatitudeRef_Index].dst,
                    SEEK_SET);
            read(ELPHEL_G(fd_exif[port]), val, 1);
            if (val[0] != 'N') latitude=-latitude;
        }
        sprintf (val,"%f",latitude);
        add_assoc_string(return_value, "GPSLatitude", val,  1);
    }
    ///GPS Altitude
    if (ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSAltitude_Index].ltag==Exif_GPSInfo_GPSAltitude) { // Exif_GPSInfo_GPSAltitude is present in template
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSAltitude_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), rational3, 8);
        altitude=(1.0*__cpu_to_be32( rational3[0]))/__cpu_to_be32( rational3[1]);

        if (ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSAltitudeRef_Index].ltag==Exif_GPSInfo_GPSAltitudeRef) {
            lseek (ELPHEL_G(fd_exif[port]),
                    exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_GPSAltitudeRef_Index].dst,
                    SEEK_SET);
            read(ELPHEL_G(fd_exif[port]), val, 1);
            if (val[0] != '\0') altitude=-altitude;
        }
        sprintf (val,"%f",altitude);
        add_assoc_string(return_value, "GPSAltitude", val,  1);
    }
    ///Compass Direction (magnetic)
    if (ELPHEL_G(exif_dir)[Exif_GPSInfo_CompassDirection_Index].ltag==Exif_GPSInfo_CompassDirection) { // Exif_GPSInfo_CompassDirection is present in template
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_CompassDirection_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), rational3, 8);
        heading=(1.0*__cpu_to_be32( rational3[0]))/__cpu_to_be32( rational3[1]);
        sprintf (val,"%f",heading);
        add_assoc_string(return_value, "CompassDirection", val,  1);
    }
    ///Processing 'hacked' pitch and roll (made of Exif destination latitude/longitude)
    ///Compass Roll
    if (ELPHEL_G(exif_dir)[Exif_GPSInfo_CompassRoll_Index].ltag==Exif_GPSInfo_CompassRoll) { // Exif_GPSInfo_CompassRoll is present in template
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_CompassRoll_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), rational3, 8);
        roll=(1.0*__cpu_to_be32( rational3[0]))/__cpu_to_be32( rational3[1]);

        if (ELPHEL_G(exif_dir)[Exif_GPSInfo_CompassRollRef_Index].ltag==Exif_GPSInfo_CompassRollRef) {
            lseek (ELPHEL_G(fd_exif[port]),
                    exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_CompassRollRef_Index].dst,
                    SEEK_SET);
            read(ELPHEL_G(fd_exif[port]), val, 1);
            if (val[0] != EXIF_COMPASS_ROLL_ASCII[0]) roll=-roll;
        }
        sprintf (val,"%f",roll);
        add_assoc_string(return_value, "CompassRoll", val,  1);
    }

    ///Compass Pitch
    if (ELPHEL_G(exif_dir)[Exif_GPSInfo_CompassPitch_Index].ltag==Exif_GPSInfo_CompassPitch) { // Exif_GPSInfo_CompassPitch is present in template
        lseek (ELPHEL_G(fd_exif[port]),
                exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_CompassPitch_Index].dst,
                SEEK_SET);
        read(ELPHEL_G(fd_exif[port]), rational3, 8);
        pitch=(1.0*__cpu_to_be32( rational3[0]))/__cpu_to_be32( rational3[1]);

        if (ELPHEL_G(exif_dir)[Exif_GPSInfo_CompassPitchRef_Index].ltag==Exif_GPSInfo_CompassPitchRef) {
            lseek (ELPHEL_G(fd_exif[port]),
                    exif_page_start+ELPHEL_G(exif_dir)[Exif_GPSInfo_CompassPitchRef_Index].dst,
                    SEEK_SET);
            read(ELPHEL_G(fd_exif[port]), val, 1);
            if (val[0] != EXIF_COMPASS_PITCH_ASCII[0]) pitch=-pitch;
        }
        sprintf (val,"%f",pitch);
        add_assoc_string(return_value, "CompassPitch", val,  1);
    }

}

PHP_FUNCTION(elphel_update_exif) {
    RETURN_LONG(createExifDirectory(1)); // force rebuild
}


PHP_FUNCTION(elphel_get_exif_field)
{
    long port;
    long exif_page=0;
    long ltag;
    struct exif_dir_table_t dir_table_entry;
    char * rslt;
    int found=0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll|l", &port, &ltag, &exif_page) == FAILURE) {
        RETURN_NULL();
    }
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();

    // find the ltag in the directory (may need to scan all the directory)
    lseek ((int) ELPHEL_G(fd_exifdir), 0, SEEK_SET);
    while (read((int) ELPHEL_G(fd_exifdir), &dir_table_entry, sizeof(dir_table_entry))>0) {
        if (dir_table_entry.ltag==ltag) {
            found=1;
            break;
        }
    }
    if (!found) RETURN_NULL();
    //    lseek (fd_exifdir, 0, SEEK_SET);
    if (exif_page) found= lseek ((int) ELPHEL_G(fd_exif[port]), exif_page, SEEK_END); /// select specified Exif page
    else           found= lseek ((int) ELPHEL_G(fd_exif[port]), 0, SEEK_SET); /// Select 0 (currently being acquired) Exif page
    if (found<0) RETURN_NULL(); //exif_page may be out of range

    lseek ((int) ELPHEL_G(fd_exif[port]), dir_table_entry.dst, SEEK_CUR);
    rslt=emalloc(dir_table_entry.len);
    read((int) ELPHEL_G(fd_exif[port]), rslt, dir_table_entry.len);
    RETURN_STRINGL(rslt,dir_table_entry.len,0);
}



PHP_FUNCTION(elphel_set_exif_field)
{
    long port;
    long ltag;
    struct exif_dir_table_t dir_table_entry;
    char * value;
    int value_length;
    int found=0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lls", &port, &ltag, &value, &value_length) == FAILURE) {
        RETURN_NULL();
    }
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    // find the ltag in the directory (may need to scan all the directory)
    lseek ((int) ELPHEL_G(fd_exifdir), 0, SEEK_SET);
    while (read((int) ELPHEL_G(fd_exifdir), &dir_table_entry, sizeof(dir_table_entry))>0) {
        if (dir_table_entry.ltag==ltag) {
            found=1;
            break;
        }
    }
    if (!found) RETURN_NULL();
    lseek ((int) ELPHEL_G(fd_exifmeta[port]), dir_table_entry.src, SEEK_SET);
    if (value_length>dir_table_entry.len) value_length = dir_table_entry.len;
    ///NOTE:DEBUG
    // php_printf ("value=%s, value_length=%d\n",value, value_length);
    long rslt=write(ELPHEL_G(fd_exifmeta[port]), value, value_length);
    if (rslt<0) rslt =-errno;
    RETURN_LONG(rslt);
}


//! wait for the next frame to be compressed (and related parameters updated
PHP_FUNCTION(elphel_wait_frame)
{
    long port;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &port) == FAILURE) {
        RETURN_NULL();
    }
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_NULL();
    lseek((int) ELPHEL_G( fd_circ[port]), LSEEK_CIRC_TOWP, SEEK_END );
    lseek((int) ELPHEL_G( fd_circ[port]), LSEEK_CIRC_WAIT, SEEK_END );
    RETURN_NULL();
}

/**
 * @brief Use current (for the specified frame) gamma table to convert input data (fraction <1.0) into output value (used by histograms)
 * @param port - sensor port (0..3)
 * @param sub_chn - sensor sub-channel (for mux-ed sensors), NC393 initially ignored !
 * @param color - needed color (0..3)
 * @param sensorLevel - level (0.0 <=level<1.0) proportional to the sensor output, normalized to 0.0..1.0 scale
 * @param frame (optional) absolute frame number for which gamma table is needed. NOTE: Will not wait if it is in the future - just return -1
 *               if frame is not specified - will use the previous to current frame - same as the one for which histogram is availble (or will be very soon)
 * @return -1 if too late (or other errors), otherwise a fraction of the full output level (0.0..1.0)
 */

PHP_FUNCTION(elphel_gamma)
{
    long port, sub_chn;
    long   color;
    long frame =-1;
    double sensorLevel;
    int indx,gamma_index;
    unsigned long hash32; /// combined black, gamma, scale
    unsigned long  write_data[2];
    int rslt;
    unsigned short * gamma_direct;
    long lsensorLevel;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llld|l", &port, &sub_chn, &color, &sensorLevel, &frame ) == FAILURE) {
        RETURN_LONG (-1);
    }
    if ((color <0) || (color > 3)) RETURN_LONG (-1); /// wrong color number
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_LONG (-1);
    if ((sub_chn <0) || (sub_chn >= MAX_SENSORS))
        RETURN_LONG (-1);

    if (frame <0) {
        frame=ELPHEL_GLOBALPARS(port, G_THIS_FRAME)-1;
    }
    //  php_printf ("frame=0x%lx, color=0x%lx\n",frame, color);
    hash32=get_imageParamsThat (port, P_GTAB_R+color, frame); // FIXME: No sub_chn yet !!!
    //  php_printf ("hash32=0x%lx\n",hash32);
    if (hash32 == 0xffffffff) RETURN_LONG (-1);
    /// now request gamma table for that hash32, including reverse
    write_data[0]=hash32;
    write_data[1]=0; /// GAMMA_MODE_NEED_REVERSE;
    rslt=write(ELPHEL_G(fd_gamma_cache), write_data, 6);
    //  php_printf ("frame=0x%lx, write_data[0]=0x%lx, write_data[1]=0x%lx,rslt=0x%x\n",frame, write_data[0], write_data[1],rslt);

    if (rslt<= 0) {
        RETURN_LONG (-1);
    }
    gamma_index=lseek(ELPHEL_G(fd_gamma_cache), 0, SEEK_CUR);
    //  php_printf ("gamma_index=0x%x\n",gamma_index);

    if (gamma_index <= 0) {
        RETURN_LONG (-2); /// gamma table may be lost in cache - need reload/recalculation through elphel_gamma_add()
    }
    gamma_direct= &(((struct gamma_stuct_t *) ELPHEL_G(gamma_cache))[gamma_index].direct[0]);/// [257]  "Gamma" table, 16-bit for both non-scaled prototypes
    ///  and scaled, 0..0xffff range (hardware will use less)
    lsensorLevel=0x10000*sensorLevel;
    //  php_printf ("lsensorLevel=0x%lx\n",lsensorLevel);

    if (lsensorLevel <0) RETURN_LONG (-1);
    if (lsensorLevel >0xffff) lsensorLevel=0xffff;
    //  php_printf ("(long) gamma_direct[%lx]=0x%lx\n",lsensorLevel>>8, (long) gamma_direct[lsensorLevel>>8]);
    //  php_printf ("(long) gamma_direct[%lx]=0x%lx\n",(lsensorLevel>>8)+1, (long) gamma_direct[(lsensorLevel>>8)+1]);

    RETURN_DOUBLE ((1.0/(1<<24))* ((((long) gamma_direct[lsensorLevel>>8])<<8) +
            (((long) gamma_direct[(lsensorLevel>>8)+1] - ((long) gamma_direct[lsensorLevel>>8]))*(lsensorLevel & 0xff))));
}

/**
 * @brief Use current (for the specified frame) gamma table to back-translate gamma converter output data (fraction <1.0) into the  input (sensor) data
 * @param port - sensor port (0..3)
 * @param sub_chn - sensor sub-channel (for mux-ed sensors), NC393 initially ignored !
 * @param color - needed color (0..3)
 * @param gammaLevel - level (0.0 <=level<1.0) proportional to the senosor output, normalized to 0.0..1.0 scale
 * @param frame (optional) absolute frame number for which gamma table is needed. NOTE: Will not wait if it is in the future - just return -1
 *               if frame is not specified - will use the previous to current frame - same as the one for which histogram is availble (or will be very soon)
 * @return -1 if too late (or other errors), otherwise a fraction of the full output level (0.0..1.0)
 */

PHP_FUNCTION(elphel_reverse_gamma)
{
    long port, sub_chn;
    long   color;
    long   frame=-1;
    double gammaLevel;
    int indx,gamma_index;
    unsigned long hash32; /// combined black, gamma, scale
    unsigned long  write_data[2];
    int rslt;
    unsigned short * gamma_direct;
    unsigned char * gamma_reverse;
    long lgammaLevel, sensor_high8,delta, sensor_full;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llld|l",&port, &sub_chn,  &color, &gammaLevel, &frame ) == FAILURE) {
        RETURN_LONG (-1);
    }
    if ((color <0) || (color > 3)) RETURN_LONG (-1); /// wrong color number
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_LONG (-1);
    if ((sub_chn <0) || (sub_chn >= MAX_SENSORS))
        RETURN_LONG (-1);
    if (frame <0) {
        frame=ELPHEL_GLOBALPARS(port, G_THIS_FRAME)-1;
    }
    hash32=get_imageParamsThat (port, P_GTAB_R+color, frame);
    if (hash32 == 0xffffffff) RETURN_LONG (-1);
    /// now request gamma table for that hash32, including reverse
    write_data[0]=hash32;
    write_data[1]= GAMMA_MODE_NEED_REVERSE;
    rslt=write(ELPHEL_G(fd_gamma_cache), write_data, 6);
    if (rslt<= 0) {
        RETURN_LONG (-1);
    }
    gamma_index=lseek(ELPHEL_G(fd_gamma_cache), 0, SEEK_CUR);
    if (gamma_index <= 0) {
        RETURN_LONG (-2); /// gamma table may be lost in cache - need reload/recalculation through elphel_gamma_add()
    }
    gamma_direct= &(((struct gamma_stuct_t *) ELPHEL_G(gamma_cache))[gamma_index].direct[0]); /// [257]  "Gamma" table, 16-bit for both non-scaled prototypes
    ///  and scaled, 0..0xffff range (hardware will use less)
    gamma_reverse=&(((struct gamma_stuct_t *) ELPHEL_G(gamma_cache))[gamma_index].reverse[0]);/// [256] reverse table to speed-up reversing (still need
    /// interpolation).Index - most significant 8 bits, data - largest direct
    lgammaLevel=0x10000*gammaLevel;
    if (lgammaLevel <0) RETURN_LONG (-1);
    if (lgammaLevel >0xffff) lgammaLevel=0xffff;
    sensor_high8=gamma_reverse[lgammaLevel >> 8]; /// 8 MSBs used as index
    if (sensor_high8>0) sensor_high8--;                                                     /// seems gamma_reverse[] rounds up, not down
    while ((sensor_high8>0) &&  (gamma_direct[sensor_high8] > lgammaLevel)) sensor_high8--;    /// adjust down (is that needed at all?)
    sensor_high8++;
    while ((sensor_high8<255) &&  (gamma_direct[sensor_high8] <= lgammaLevel)) sensor_high8++; /// adjust up (is that needed at all?)
    sensor_high8--;
    delta=gamma_direct[sensor_high8+1] - gamma_direct[sensor_high8];
    if (delta) {
        sensor_full=((lgammaLevel-gamma_direct[sensor_high8]) << 8)/delta;
    } else sensor_full=0;
    sensor_full += sensor_high8 << 8;
    /// limit just in case?
    if      (sensor_full <      0) sensor_full=0;
    else if (sensor_full > 0xffff) sensor_full=0xffff;
    RETURN_DOUBLE ((1.0/(1<<16))* sensor_full);
}

/**
 * @brief common function to get index of the histogram cache for the specified color. May wait for the frame to become available
 * @param port - sensor port (0..3)
 * @param sub_chn - sensor sub-channel (for mux-ed sensors), NC393 initially ignored !
 * @param color 0..3 - requested color (0 -R, 1 - G (used as Y ), 2 - GB (second green), 3 - blue
 * @param frame absolute frame number (histogrames are available for the previous (to current) frame
 * @param needreverse 0 if only cumulative histogram is needed, >0 if the reverse is also needed
 * @return <0 if histogram can not be found fo the specified frame (i.e. too late), otherwise it is an index in histogram cache.
 */
int get_histogram_index (long port, long sub_chn, long color,long frame, long needreverse) { /// histogram is available for previous frame, not for the current one
    long hist_index;
    if ((color<0)    || (color >=   4))           return -1; /// wrong color
    if ((port <0)    || (port >=   SENSOR_PORTS)) return -1;
    if ((sub_chn <0) || (sub_chn >= MAX_SENSORS)) return -1;
    lseek(ELPHEL_G(fd_histogram_cache), LSEEK_HIST_SET_CHN + (4 * port) + sub_chn, SEEK_END); /// specify port/sub-channel is needed
    if (color == COLOR_Y_NUMBER) lseek(ELPHEL_G(fd_histogram_cache), LSEEK_HIST_WAIT_Y, SEEK_END); /// wait for just Y (G1)
    else                         lseek(ELPHEL_G(fd_histogram_cache), LSEEK_HIST_WAIT_C, SEEK_END); /// wait for all histograms, not just Y (G1)
    lseek(ELPHEL_G(fd_histogram_cache), LSEEK_HIST_NEEDED + ((1 << color) << (needreverse? 8:4)), SEEK_END); /// specify what color is needed and if reverse is needed
    return lseek(ELPHEL_G(fd_histogram_cache), frame, SEEK_SET);                                             /// request histogram for the specified frame
}

/**
 * @brief return value of parameter 'index' from frame 'frame' - use pastPars if too late for framePars
 * @param port sensor port (0..3)
 * @param indx parameter indx
 * @param frame absolute frame number
 * @return parameter value (error will be 0xffffffff, but that could be a legitimate value too)
 */
unsigned long get_imageParamsThat     (int port, int indx, unsigned long frame) {
    int frame_index=  frame & PARS_FRAMES_MASK;
    int past_index=   frame & PASTPARS_SAVE_ENTRIES_MASK;
    unsigned long value;
    port &= 3; // enforce 0..3
    /// Locate frame info in framePars
    if (((struct framepars_t *) ELPHEL_G(framePars[port]))[frame_index].pars[P_FRAME] != frame) {
        ///   too late, try pastPars
        if ((indx < PARS_SAVE_FROM) || (indx >= (PARS_SAVE_FROM+PARS_SAVE_NUM))) return 0xffffffff ; /// not saved
        value=((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[past_index].past_pars[indx-PARS_SAVE_FROM]; /// should be retrieved before checking frame (interrupts)
        if (((struct framepars_past_t *) ELPHEL_G(pastPars[port]))[frame_index].past_pars[P_FRAME-PARS_SAVE_FROM] != frame) { /// too late even for pastPars? Or a bug?
            return 0xffffffff;
        }
    } else {
        value=((struct framepars_t *) ELPHEL_G(framePars[port]))[frame_index].pars[indx];
    }
    return value;
}

/**
 * @brief Get cumulative histogram (fraction of all pixels below specified level)
 * @param port - sensor port (0..3)
 * @param sub_chn - sensor sub-channel (for mux-ed sensors), NC393 initially ignored !
 * @param color - needed color (0..3)
 * @param level - level (0.0 <=level<1.0) to compare pixel values to (-1 will return -1, error)
 * @param frame (optional) absolute frame number for which histogram is needed. NOTE: If specified in the future - will wait
 *               if frame is not specified - will use lates histogram (previous to current frame)
 * @return -1 if too late (or other errors), otherwise a fraction of pixels (0..1.0) that are below the specified level
 */
PHP_FUNCTION(elphel_histogram)
{
    long port, sub_chn;
    long frame=-1;
    long hist_index;
    double dlevel;
    long   llevel,total_pixels;
    unsigned long * hist_cumul;      /// 256 of cumulated histogram values (in pixels)
    long  hist,color;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llld|l", &port, &sub_chn, &color, &dlevel, &frame ) == FAILURE) {
        RETURN_LONG (-1);
    }
    if ((color <0) || (color > 3)) RETURN_LONG (-1); /// wrong color number
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_LONG (-1);
    if ((sub_chn <0) || (sub_chn >= MAX_SENSORS))
        RETURN_LONG (-1);
    if (frame <0) {
        frame=ELPHEL_GLOBALPARS(port, G_THIS_FRAME)-1;
    }
    if (((hist_index=get_histogram_index (port, sub_chn, color, frame, 0)))<0) RETURN_LONG (-1);
    llevel=0x10000*dlevel;
    if      (llevel< -0.5) RETURN_LONG(-1) ; /// if input level was ==-1 - error, don't try
    if      (llevel<0) llevel=0;
    else if (llevel>0xffff) llevel=0xffff;
    ///interpolate
    hist_cumul= &(((struct histogram_stuct_t *) ELPHEL_G(histogram_cache))[hist_index].cumul_hist[color<<8]);
    total_pixels=  hist_cumul[255];
    hist=  (llevel>>8)?hist_cumul[(llevel>>8)-1]:0;
    hist +=((hist_cumul[llevel>>8]-hist)*(llevel & 0xff))>>8;
    RETURN_DOUBLE(((double) hist)/total_pixels);
}

/**
 * @brief Get percentile (reverse cumulative histogram) - level (as a fraction 0..1.0) so that specified fraction of all pixels
 *        are below it
 * @param port - sensor port (0..3)
 * @param sub_chn - sensor sub-channel (for mux-ed sensors), NC393 initially ignored !
 * @param color - needed color (0..3)
 * @param fraction - fraction  (0.0 <=fraction<1.0) of all pixels to have value under the output (-1 will return -1, error)
 * @param frame (optional) absolute frame number for which histogram is needed. NOTE: If specified in the future - will wait
 *               if frame is not specified - will use latest histogram (previous to current frame)
 * @return -1 if too late (or other errors), otherwise a level (in the 0.0<1.0 range) so that a specified fraction of all pixels are below it
 */
PHP_FUNCTION(elphel_reverse_histogram)
{
    long port, sub_chn;
    long frame=-1;
    long hist_index;
    double fraction;
    long   frac_pixels,total_pixels, frac_256, delta;
    unsigned long * hist_cumul;      /// 256 of cumulated histogram values (in pixels)
    unsigned char * hist_percentile; /// 256 of rounded percentiles (1 byte) - used as a starting point for linear interpolation

    long  perc,perc_frac,color;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llld|l", &port, &sub_chn, &color, &fraction, &frame ) == FAILURE) {
        RETURN_LONG (-1);
    }
    if ((color <0) || (color > 3)) RETURN_LONG (-1); /// wrong color number
    if ((port <0) || (port >= SENSOR_PORTS))
        RETURN_LONG (-1);
    if ((sub_chn <0) || (sub_chn >= MAX_SENSORS))
        RETURN_LONG (-1);
    if (frame <0) {
        frame=ELPHEL_GLOBALPARS(port, G_THIS_FRAME)-1;
    }
    if (((hist_index=get_histogram_index (port, sub_chn, color, frame, 1)))<0) RETURN_LONG (-1);
    ///interpolate
    hist_cumul= &(((struct histogram_stuct_t *) ELPHEL_G(histogram_cache))[hist_index].cumul_hist[color<<8]);
    hist_percentile=&(((struct histogram_stuct_t *) ELPHEL_G(histogram_cache))[hist_index].percentile[color<<8]);
    total_pixels=  hist_cumul[255];
    frac_pixels=total_pixels*fraction;
    if (fraction < -0.5) RETURN_LONG(-1) ; /// if input level was ==-1 - error, don't try
    if      (frac_pixels<0) frac_pixels=0;
    else if (frac_pixels>=total_pixels) frac_pixels=total_pixels-1;
    frac_256=(1.0/256)*fraction; ///floor()
    if (frac_256 > 255) frac_256=255;
    perc=hist_percentile[frac_256];
    if (perc>0) perc--;                                              /// seems hist_percentile[perc] rounds up, not down
    while ((perc>0) &&  (hist_cumul[perc] > frac_pixels)) perc--;    /// adjust down  (is that needed at all?)
    perc++;
    while ((perc<255) &&  (hist_cumul[perc] <= frac_pixels)) perc++; /// adjust up  (is that needed at all?)
    perc--;
    delta=hist_cumul[perc+1] - hist_cumul[perc];
    if (delta) {
        perc_frac=((frac_pixels-hist_cumul[perc]) << 8)/delta;
    } else perc_frac=0;
    perc_frac += perc << 8;
    RETURN_DOUBLE((1.0/(1<<16)) * ((double) perc_frac));
}



//! Low-level, direct FPGA read/write
PHP_FUNCTION(elphel_fpga_read)
{
    long addr,data,res;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &addr) == FAILURE) {
        RETURN_LONG(-1);
    }
#ifdef NC353
    int fd=open("/dev/fpgaio", O_RDONLY);
    if (fd<0) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Can not open file %s","/dev/fpgaio");
        return ;
    }
    lseek (fd, addr,SEEK_SET) ; //! 32-bit registers, not bytes
    res=read(fd,&data,4);
    close (fd);
    if (res<4) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Can not read file %s","/dev/fpgaio");
        return ;
    }
    RETURN_LONG(data);
#else
    php_error_docref(NULL TSRMLS_CC, E_ERROR, "elphel_fpga_read() not implemented");
    return ;
//    RETURN_LONG(-1);
#endif
}

PHP_FUNCTION(elphel_fpga_write)
{
    long addr,data,res;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &addr,&data) == FAILURE) {
        RETURN_NULL();
    }
#ifdef NC353 // Not implemented in NC393 - Add?
    int fd=open("/dev/fpgaio", O_RDWR);
    if (fd<0) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Can not open file %s","/dev/fpgaio");
        return ;
    }
    lseek (fd, addr,SEEK_SET) ; //! 32-bit registers, not bytes
    res=write(fd,&data,4);
    close (fd);
    if (res<4) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Can not write to file %s","/dev/fpgaio");
        return ;
    }
#endif
    php_error_docref(NULL TSRMLS_CC, E_ERROR, "elphel_fpga_write() not implemented");
    return ;
//
//    RETURN_NULL();
}



//! set FPGA clock. Input parameter - (double) seconds (i.e. from 01/01/1970)
//! This clock is used to timestamp each image
//! returns same time rounded to microseconds (as written to FPGA)
//! Is it supported in NC393?
PHP_FUNCTION(elphel_set_fpga_time) {
    unsigned long write_data[8]= {FRAMEPARS_SETFRAME,     0,
            G_SECONDS,              0,
            G_MICROSECONDS,         0,
            FRAMEPARS_SETFPGATIME,  0};
    double dtime;
    long ltime_sec,ltime_usec;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &dtime) == FAILURE) {
        RETURN_NULL();
    }
    ltime_sec=dtime;
    ltime_usec=(dtime-ltime_sec)*1000000;
    write_data[3]=ltime_sec;
    write_data[5]=ltime_usec;

    long rslt=write(ELPHEL_G(fd_fparmsall[0]), write_data, sizeof(write_data)); // does not matter - which port
    if (rslt<0) RETURN_LONG(-errno);
    dtime=ltime_usec;
    dtime=ltime_sec+0.000001*dtime;
    RETURN_DOUBLE(dtime);
}

//! Get current FPGA time as (double) seconds
PHP_FUNCTION(elphel_get_fpga_time) {
    double dtime;
    long ltime_sec,ltime_usec;
    lseek((int) ELPHEL_G( fd_fparmsall[0]), LSEEK_GET_FPGA_TIME, SEEK_END );
    dtime= ELPHEL_GLOBALPARS(0, G_MICROSECONDS);
    dtime= ELPHEL_GLOBALPARS(0, G_SECONDS) + 0.000001*dtime;
    RETURN_DOUBLE(dtime);
}
#if 0
static struct framepars_all_t test_structure;
static void php_elphel_init_globals(zend_elphel_globals *elphel_globals)
{
    int port;
    test_structure.globalPars[G_THIS_FRAME] = 999;
    test_structure.framePars[0].pars[P_EXPOS] = 111;
    test_structure.framePars[1].pars[P_EXPOS] = 222;
    test_structure.framePars[2].pars[P_EXPOS] = 333;
    test_structure.framePars[3].pars[P_EXPOS] = 444;
    test_structure.framePars[4].pars[P_EXPOS] = 555;
    test_structure.framePars[5].pars[P_EXPOS] = 666;
    test_structure.framePars[6].pars[P_EXPOS] = 777;
    test_structure.framePars[7].pars[P_EXPOS] = 888;

    //ELPHEL_GLOBALPARS(G_THIS_FRAME)
    for (port = 0; port < SENSOR_PORTS; port ++) {
        elphel_globals->frameParsAll[port] = NULL;
        elphel_globals->framePars[port] =    NULL;
        elphel_globals->pastPars[port] =     NULL;
        elphel_globals->funcs2call[port]=    NULL;
    }

    elphel_globals->frameParsAll[0] = &test_structure;

    for (port = 0; port < SENSOR_PORTS; port ++) {
        elphel_globals->framePars[port] =          elphel_globals->frameParsAll[port]->framePars;
        elphel_globals->pastPars[port] =           elphel_globals->frameParsAll[port]->pastPars;
        elphel_globals->funcs2call[port]=          elphel_globals->frameParsAll[port]->func2call.pars;
        elphel_globals->globalPars[port] =         elphel_globals->frameParsAll[port]->globalPars;
        elphel_globals->multiSensIndex[port] =     elphel_globals->frameParsAll[port]->multiSensIndex;
        elphel_globals->multiSensRvrsIndex[port] = elphel_globals->frameParsAll[port]->multiSensRvrsIndex; /// not yet used
    }
}
#else

static void php_elphel_init_globals(zend_elphel_globals *elphel_globals)
{
    const char *frameparsPaths[] = { DEV393_PATH(DEV393_FRAMEPARS0), DEV393_PATH(DEV393_FRAMEPARS1),
                                     DEV393_PATH(DEV393_FRAMEPARS2), DEV393_PATH(DEV393_FRAMEPARS3)};
    const char *circbufPaths[] =   { DEV393_PATH(DEV393_CIRCBUF0), DEV393_PATH(DEV393_CIRCBUF1),
                                     DEV393_PATH(DEV393_CIRCBUF2), DEV393_PATH(DEV393_CIRCBUF3)};
    const char *exifPaths[] =      { DEV393_PATH(DEV393_EXIF0), DEV393_PATH(DEV393_EXIF1),
                                     DEV393_PATH(DEV393_EXIF2), DEV393_PATH(DEV393_EXIF3)};
    const char *exifMetaPaths[] =  { DEV393_PATH(DEV393_EXIF_META0), DEV393_PATH(DEV393_EXIF_META1),
                                     DEV393_PATH(DEV393_EXIF_META2), DEV393_PATH(DEV393_EXIF_META3)};
//DEV393_PATH(DEV393_CIRCBUF0
    int total_hist_entries;

    int port;
    //! open "/dev/sensorpars" and mmap array - only once
    for (port = 0; port < SENSOR_PORTS; port++){
        elphel_globals->frameParsAll[port] = NULL;
        elphel_globals->framePars[port] =    NULL;
        elphel_globals->pastPars[port] =     NULL;
        elphel_globals->funcs2call[port] =   NULL;
        //DEV393_PATH(DEV393_FRAMEPARS0)
        elphel_globals->fd_fparmsall[port] = open(frameparsPaths[port], O_RDWR); // "/dev/frameparsall", O_RDWR);
        if (elphel_globals->fd_fparmsall[port] <0) {
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "Can not open file %s",frameparsPaths[port]);
            return ;
        }
        //! now try to mmap (PROT_WRITE only for writing dependencies - func2call?
        elphel_globals->frameParsAll[port] = (struct framepars_all_t *) mmap(0, sizeof (struct framepars_all_t) , PROT_READ | PROT_WRITE, MAP_SHARED, elphel_globals->fd_fparmsall[port], 0);
        if((int)elphel_globals->frameParsAll[port] == -1) {
            elphel_globals->frameParsAll[port]=NULL;
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error in mmap frameparsall");
            close (elphel_globals->fd_fparmsall[port]);
            elphel_globals->fd_fparmsall[port] = -1;
            return ;
        }
        /// Shortcuts to the two sub-structures
        elphel_globals->framePars[port] =          elphel_globals->frameParsAll[port]->framePars;
        elphel_globals->pastPars[port] =           elphel_globals->frameParsAll[port]->pastPars;
        elphel_globals->funcs2call[port]=          elphel_globals->frameParsAll[port]->func2call.pars;
        elphel_globals->globalPars[port] =         elphel_globals->frameParsAll[port]->globalPars;
        elphel_globals->multiSensIndex[port] =     elphel_globals->frameParsAll[port]->multiSensIndex;
        elphel_globals->multiSensRvrsIndex[port] = elphel_globals->frameParsAll[port]->multiSensRvrsIndex; /// not yet used
    }

    /// (gamma_tables.c) access to gammas

    elphel_globals->gamma_cache = NULL;
    elphel_globals->fd_gamma_cache= open(DEV393_PATH(DEV393_GAMMA), O_RDWR); // "/dev/gamma_cache", O_RDWR);
    if (elphel_globals->fd_gamma_cache <0) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Can not open file %s",DEV393_PATH(DEV393_GAMMA));
        return ;
    }
    //! now try to mmap
    elphel_globals->gamma_cache = (struct gamma_stuct_t *) mmap(0, sizeof (struct gamma_stuct_t) * GAMMA_CACHE_NUMBER , PROT_READ, MAP_SHARED, elphel_globals->fd_gamma_cache, 0);
    if((int)elphel_globals->gamma_cache == -1) {
        elphel_globals->gamma_cache=NULL;
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error in mmap for gamma_cache");
        close (elphel_globals->fd_gamma_cache);
        elphel_globals->fd_gamma_cache = -1;
        return ;
    }
    ///debug
    /// (histogram.c) access to gammas
    elphel_globals->histogram_cache = NULL;
    elphel_globals->fd_histogram_cache= open(DEV393_PATH(DEV393_HISTOGRAM), O_RDWR); // "/dev/histogram_cache", O_RDWR);
    if (elphel_globals->fd_histogram_cache <0) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Can not open file %s",DEV393_PATH(DEV393_HISTOGRAM));
        return ;
    }
    total_hist_entries = lseek(elphel_globals->fd_histogram_cache, LSEEK_HIST_SET_CHN + 0, SEEK_END); /// specify port/sub-channel is needed
    //! now try to mmap
    elphel_globals->histogram_cache = (struct histogram_stuct_t *) mmap(0, sizeof (struct histogram_stuct_t) * total_hist_entries, PROT_READ, MAP_SHARED, elphel_globals->fd_histogram_cache, 0);
    if((int)elphel_globals->histogram_cache == -1) {
        elphel_globals->histogram_cache=NULL;
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error in mmap histogram_cache");
        close (elphel_globals->fd_histogram_cache);
        elphel_globals->fd_histogram_cache = -1;
        return ;
    }

    //! Now - same for /dev/circbuf
//    int dbg_i;
    for (port = 0; port < SENSOR_PORTS; port++){
        elphel_globals->ccam_dma_buf[port] = NULL;
        elphel_globals->fd_circ[port]= open(circbufPaths[port], O_RDWR); // "/dev/circbuf", O_RDWR);
        if (elphel_globals->fd_circ[0] <0) {
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "Can not open file %s",circbufPaths[port]);
            return ;
        }
        elphel_globals->ccam_dma_buf_len[port]=lseek(elphel_globals->fd_circ[port],0,SEEK_END); //size of circbuf

        //! now try to mmap
        elphel_globals->ccam_dma_buf[port] = (unsigned long *) mmap(0,
                                                                    elphel_globals->ccam_dma_buf_len[port] ,
                                                                    PROT_READ | PROT_WRITE, MAP_SHARED,
                                                                    elphel_globals->fd_circ[port],
                                                                    0);
        if((int)elphel_globals->ccam_dma_buf[port] == -1) {
            elphel_globals->ccam_dma_buf[port]=NULL;
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error in mmap circbuf");
            close (elphel_globals->fd_circ[port]);
            elphel_globals->fd_circ[port] = -1;
            return ;
        }
        /*
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d: mmap=-ed %ld (0x%08lx) bytes to ccam_dma_buf[%d]",
                __LINE__,elphel_globals->ccam_dma_buf_len[port],elphel_globals->ccam_dma_buf_len[port],port);

        for (dbg_i=0;dbg_i<sizeof(struct interframe_params_t )/4; dbg_i++){
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "elphel_globals->ccam_dma_buf[%d][%d]=0x%08lx\n",
                    port,dbg_i,elphel_globals->ccam_dma_buf[port][dbg_i]);
        }
*/



    }

    for (port = 0; port < SENSOR_PORTS; port++){
        elphel_globals->fd_exif[port] = open(exifPaths[port],O_RDONLY); // EXIF_DEV_NAME, O_RDONLY);
        if (elphel_globals->fd_exif[port] <0) {
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "Can not open file %s",exifPaths[port]);
            return ;
        }
        elphel_globals->fd_exifmeta[port] = open(exifMetaPaths[port], O_RDWR);
        if (elphel_globals->fd_exifmeta[port] <0) {
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "Can not open file %s",exifMetaPaths[port]);
            return ;
        }
    }
    elphel_globals->fd_exifdir = open(DEV393_PATH(DEV393_EXIF_METADIR), O_RDONLY);
    if (elphel_globals->fd_exifdir <0) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Can not open file %s",DEV393_PATH(DEV393_EXIF_METADIR));
        return ;
    }
    elphel_globals->exif_size=0;
//    php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d: elphel_globals->exif_size= 0x%x\n",  __LINE__,elphel_globals->exif_size);
}
#endif

PHP_RINIT_FUNCTION(elphel)
{
    // session initialization may be here

    return SUCCESS;
}
/// use gcc -save-temps to debug those macros for constants
PHP_MINIT_FUNCTION(elphel)
{
    ZEND_INIT_MODULE_GLOBALS(elphel, php_elphel_init_globals, NULL);

    DEFINE_P_NAMES(pname_arr);

    DEFINE_ONCHANGE_NAMES(onchange_arr);

    DEFINE_LSEEK_NAMES(lseek_arr);

    DEFINE_CONST_NAMES(const_arr);

    int i,j;
    char full_constant_name[256];

    //! here initialize "ELPHEL_*" constants
    REGISTER_INI_ENTRIES(); // not used yet
    for (i=0;i< (sizeof(pname_arr)/sizeof(pname_arr[0])); i++) {
        if (strlen(pname_arr[i].name)>(sizeof(full_constant_name)-8)) return FAILURE;
        sprintf (full_constant_name,"ELPHEL_%s",pname_arr[i].name);
        zend_register_long_constant(full_constant_name, strlen(full_constant_name)+1, pname_arr[i].value, (CONST_CS | CONST_PERSISTENT), module_number TSRMLS_CC);
    }

    for (i=0;i< (sizeof(onchange_arr)/sizeof(onchange_arr[0])); i++) {
        if (strlen(onchange_arr[i].name)>(sizeof(full_constant_name)-17)) return FAILURE;
        sprintf (full_constant_name,"ELPHEL_ONCHANGE_%s",onchange_arr[i].name);
        for (j=0; j<strlen(full_constant_name);j++) full_constant_name[j]=toupper(full_constant_name[j]);
        zend_register_long_constant(full_constant_name, strlen(full_constant_name)+1, onchange_arr[i].value, (CONST_CS | CONST_PERSISTENT), module_number TSRMLS_CC);
    }

    for (i=0;i< (sizeof(lseek_arr)/sizeof(lseek_arr[0])); i++) {
        if (strlen(lseek_arr[i].name)>(sizeof(full_constant_name)-14)) return FAILURE;
        sprintf (full_constant_name,"ELPHEL_LSEEK_%s",lseek_arr[i].name);
        zend_register_long_constant(full_constant_name, strlen(full_constant_name)+1, lseek_arr[i].value, (CONST_CS | CONST_PERSISTENT), module_number TSRMLS_CC);
    }
    for (i=0;i< (sizeof(const_arr)/sizeof(const_arr[0])); i++) {
        if (strlen(const_arr[i].name)>(sizeof(full_constant_name)-14)) return FAILURE;
        sprintf (full_constant_name,"ELPHEL_CONST_%s",const_arr[i].name);
        zend_register_long_constant(full_constant_name, strlen(full_constant_name)+1, const_arr[i].value, (CONST_CS | CONST_PERSISTENT), module_number TSRMLS_CC);
    }
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(elphel)
{
    int port;
    UNREGISTER_INI_ENTRIES();
    for (port = 0; port < SENSOR_PORTS; port++){
        if (ELPHEL_G(fd_fparmsall[port])>=0)       close (ELPHEL_G(fd_fparmsall[port]));
        if (ELPHEL_G(fd_circ[port])>=0)            close (ELPHEL_G(fd_circ[port]));
        if (ELPHEL_G(fd_exif[port])>=0)            close (ELPHEL_G(fd_exif[port]));
        if (ELPHEL_G(fd_exifmeta[port])>=0)        close (ELPHEL_G(fd_exifmeta[port]));
    }
    if (ELPHEL_G(fd_exifdir)>=0)         close (ELPHEL_G(fd_exifdir));
    if (ELPHEL_G(fd_gamma_cache)>=0)     close (ELPHEL_G(fd_gamma_cache));
    if (ELPHEL_G(fd_histogram_cache)>=0) close (ELPHEL_G(fd_histogram_cache));
    return SUCCESS;
}

PHP_MINFO_FUNCTION(elphel)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "Elphel support", "Enabled");
    php_info_print_table_row(2, "Elphel API Version", PHP_ELPHEL_VERSION);
    php_info_print_table_end();
}

