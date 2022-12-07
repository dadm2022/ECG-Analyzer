//
// Created by Laura Kucharska on 28/11/2022.
//
// This header file contains necessary declarations copied from wfdb library,
// and functions used to work with wfdb records.
//

#ifndef ECG_ANALYZERRRR_WFDB_H
#define ECG_ANALYZERRRR_WFDB_H

#include "wfdb_macros.h"

/* Simple data types */
typedef int WFDB_Sample;             /* units are adus */
typedef long WFDB_Date;              /* units are days */
typedef double WFDB_Frequency;       /* units are Hz (samples/second/signal) */
typedef double WFDB_Gain;            /* units are adus per physical unit */
typedef unsigned int WFDB_Group;     /* signal group number */
typedef unsigned int WFDB_Signal;    /* signal number */
typedef unsigned int WFDB_Annotator; /* annotator number */
typedef long long WFDB_Time;

struct WFDB_seginfo {              /* segment record structure */
    char recname[WFDB_MAXRNL + 1]; /* segment name */
    WFDB_Time nsamp;               /* number of samples in segment */
    WFDB_Time samp0;               /* sample number of first sample */
};

/* Composite data types */
typedef struct WFDB_siginfo WFDB_Siginfo;
typedef struct WFDB_calinfo WFDB_Calinfo;
typedef struct WFDB_anninfo WFDB_Anninfo;
typedef struct WFDB_ann WFDB_Annotation;
typedef struct WFDB_seginfo WFDB_Seginfo;

typedef int FINT;
typedef WFDB_Date FDATE;

/* WFDB_siginfo structure definition */
struct WFDB_siginfo {    /* signal information structure */
    char *fname;         /* filename of signal file */
    char *desc;          /* signal description */
    char *units;         /* physical units (mV unless otherwise specified) */
    WFDB_Gain gain;      /* gain (ADC units/physical unit, 0: uncalibrated) */
    WFDB_Sample initval; /* initial value (that of sample number 0) */
    WFDB_Group group;    /* signal group number */
    int fmt;             /* format (8, 16, etc.) */
    int spf;             /* samples per frame (>1 for oversampled signals) */
    int bsize;           /* block size (for character special files only) */
    int adcres;          /* ADC resolution in bits */
    int adczero;         /* ADC output given 0 VDC input */
    int baseline;        /* ADC output given 0 physical units input */
    long nsamp;          /* number of samples (0: unspecified) */
    int cksum;           /* 16-bit checksum of all samples */
};

/**
 * @brief Function for opening selected record data.
 * 
 * @param[in] record   Name of the record. 
 * @param[in] filepath Path to record with record name, but not the extention. 
 * @param siarray      Pointer to the WFDB_Siginfo array. Can be nullptr when size is unknown.
 * @param nsig         Number of signals to be read.
 * @return FINT        nsig when nsig match the number of signals in record. Otherwise -1.
 */
FINT isigopen(const char *record, const char *filepath, WFDB_Siginfo *siarray, int nsig);

/**
 * @brief Function for getting one sample vector data from signal.
 * 
 * @param vector Pointer to the WFDB_Sample array
 * @return FINT  
 */
FINT getvec(WFDB_Sample *vector);

#endif //ECG_ANALYZERRRR_WFDB_H
