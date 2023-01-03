//
// Created by Laura Kucharska on 28/11/2022.
//
// WFDB macros copied from wfdb library.
// 
//

#ifndef ECG_ANALYZERRRR_WFDB_MACROS_H
#define ECG_ANALYZERRRR_WFDB_MACROS_H

/* wfdb_fopen mode values (WFDB_anninfo '.stat' values) */
#define WFDB_READ      0   /* standard input annotation file */
#define WFDB_WRITE     1   /* standard output annotation file */
#define WFDB_AHA_READ  2   /* AHA-format input annotation file */
#define WFDB_AHA_WRITE 3   /* AHA-format output annotation file */
#define WFDB_APPEND    4   /* for output info files */

#define strtotime strtoll

/* Values for WFDB_FILE 'type' field */
#define WFDB_LOCAL    0    /* a local file, read via C standard I/O */
#define WFDB_NET    1    /* a remote file, read via libwww */

#define INT_MAX __INT_MAX__
#define LONG_MAX __LONG_MAX__

/* Array sizes
   Many older applications use the values of WFDB_MAXANN, WFDB_MAXSIG, and
   WFDB_MAXSPF to determine array sizes, but (since WFDB library version 10.2)
   there are no longer any fixed limits imposed by the WFDB library.
*/
#define WFDB_MAXANN    2   /* maximum number of input or output annotators */
#define    WFDB_MAXSIG   32   /* maximum number of input or output signals */
#define WFDB_MAXSPF    4   /* maximum number of samples per signal per frame */
#define WFDB_MAXRNL   50   /* maximum length of record name */
#define WFDB_MAXUSL   50   /* maximum length of WFDB_siginfo `.units' string */
#define WFDB_MAXDSL  100   /* maximum length of WFDB_siginfo `.desc' string */

/* Default signal specifications */
#define WFDB_DEFFREQ    250.0  /* default sampling frequency (Hz) */
#define WFDB_DEFGAIN    200.0  /* default value for gain (adu/physical unit) */
#define WFDB_DEFRES    12     /* default value for ADC resolution (bits) */

/* getvec operating modes */
#define WFDB_LOWRES    0    /* return one sample per signal per frame */
#define WFDB_HIGHRES    1    /* return each sample of oversampled signals,
				   duplicating samples of other signals */
#define WFDB_GVPAD    2    /* replace invalid samples with previous valid
				   samples */


#define WFDB_TIME_PRINTF_MODIFIER  "l"
#define WFDB_Pd_TIME WFDB_TIME_PRINTF_MODIFIER "d"

#define WFDB_FMT_LIST {0, 8, 16, 61, 80, 160, 212, 310, 311, 24, 32, \
      508, 516, 524}
#define WFDB_NFMTS      14    /* number of items in WFDB_FMT_LIST */

#define FLAC__StreamEncoder struct dummy
#define FLAC__StreamDecoder struct dummy

#define WFDB_INVALID_SAMPLE (-32768)
#define INT_MIN (-INT_MAX - 1)
#define WFDB_SAMPLE_MIN             INT_MIN
#define WFDB_SAMPLE_MAX             INT_MAX

#define UINT_MAX 65535

#endif //ECG_ANALYZERRRR_WFDB_MACROS_H
