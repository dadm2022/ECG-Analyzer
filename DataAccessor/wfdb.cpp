//
// Created by Laura Kucharska on 28/11/2022.
//
// WFDB source file with necessary functions copied from wfdb library. 
//
//

#include <cstring>
#include <iostream>
#include <time.h>
#include "wfdb.h"

/* Structures used by internal WFDB library functions only */
struct WFDB_FILE {
    FILE *fp;
    struct netfile *netfp;
    int type;
};

typedef struct WFDB_FILE WFDB_FILE;

/* Dynamic memory allocation macros. */
#define MEMERR(P, N, S)                                                          \
    do {                                                                         \
        std::cout << "WFDB: can't allocate (%lu*%lu) bytes for %s\n" <<          \
                   (unsigned long)(N) <<  (unsigned long)(S) << #P  <<std::endl; \
    } while (0)

#define SREALLOC(P, N, S) \
    do {                                                                \
        size_t WFDB_tmp1 = (N), WFDB_tmp2 = (S);                        \
        if (!(P = (__typeof(P))((WFDB_tmp1 == 0 || WFDB_tmp2 == 0)      \
                   ? realloc(P, 1)                                      \
                   : ((WFDB_tmp1 > (size_t) -1 / WFDB_tmp2)             \
                      ? 0 : realloc(P, WFDB_tmp1 * WFDB_tmp2)))))       \
            MEMERR(P, WFDB_tmp1, WFDB_tmp2);                            \
    } while (0)


/* Dynamic memory allocation macros. */

#define SFREE(P) do { if (P) { free ((void*)P); P = 0; } } while (0)

#define SUALLOC(P, N, S)                                                \
    do {                                                                \
        size_t WFDB_tmp1 = (N), WFDB_tmp2 = (S);                        \
        if (WFDB_tmp1 == 0 || WFDB_tmp2 == 0)                           \
            WFDB_tmp1 = WFDB_tmp2 = 1;                                  \
        if (!(P = (__typeof(P))calloc(WFDB_tmp1, WFDB_tmp2)))           \
            MEMERR(P, WFDB_tmp1, WFDB_tmp2);                            \
    } while (0)

#define SALLOC(P, N, S) do { SFREE(P); SUALLOC(P, (N), (S)); } while (0)

#define SSTRCPY(P, Q)                                                   \
    do {                                                                \
        const char *WFDB_tmp = (Q);                                     \
        if (WFDB_tmp) {                                                 \
            SALLOC(P, (size_t)strlen(WFDB_tmp)+1, 1);                   \
            strcpy(P, WFDB_tmp);                                        \
        }                                                               \
    } while (0)

WFDB_FILE *wfdb_fopen(char *fname, const char *mode) {
    WFDB_FILE *wp;

    wp = new WFDB_FILE;

    wp->fp = fopen(fname, mode);
    if (wp->fp) {
        wp->type = WFDB_LOCAL;
        return (wp);
    }

    delete (wp);
    return nullptr;
}

WFDB_FILE *wfdb_open(const char *s, const char *record, int mode) {
    // s chyba path do directory
    // record chwilowo cały path
    static std::string directoryPath = "";
    static std::string dataPath = "";
    if (record) {
        std::string header(record);
        directoryPath = header.substr(0, header.find_last_of('/'));
    } else {
        dataPath = directoryPath + "/" + std::string(s);
    }

    char *path = const_cast<char *>(record ? record : dataPath.c_str());

    WFDB_FILE *ifile;

    if (mode == WFDB_READ) {
        if ((ifile = wfdb_fopen(path, "r")) != nullptr) {
            /* Found it! Add its path info to the WFDB path. */
            return (ifile);
        }
    }

    return nullptr;
}

static unsigned maxhsig;   /* # of hsdata structures pointed to by hsd */
static WFDB_FILE *hheader; /* file pointer for header file */
static int spfmax;             /* max number of samples per frame */
static WFDB_Frequency sfreq; /* samples/second/signal read by getvec */
static WFDB_Frequency ffreq; /* frame rate (frames/second) */
static int isedf;                   /* if non-zero, record is stored as EDF/EDF+ */
static int in_msrec;         /* current input record is: 0: a single-segment
					record; 1: a multi-segment record */
static struct isdata **vsd;
static char *linebuf;       /* temporary buffer for reading header lines */
static size_t linebufsize; /* size of linebuf */
static int segments;         /* number of segments found by readheader() */
//static WFDB_Frequency ffreq; /* frame rate (frames/second) */
static WFDB_Frequency ifreq; /* samples/second/signal returned by getvec */
//static WFDB_Frequency sfreq; /* samples/second/signal read by getvec */
static WFDB_Frequency cfreq; /* counter frequency (ticks/second) */
//static int spfmax;			 /* max number of samples per frame */
static long btime;             /* base time (milliseconds since midnight) */
static WFDB_Date bdate;         /* base date (Julian date) */
static WFDB_Time nsamples;     /* duration of signals (in samples) */
static double bcount;         /* base count (counter value at sample 0) */
static long prolog_bytes;     /* length of prolog, as told to wfdbsetstart
					(used only by setheader, if output signal
					file(s) are not open) */
static int gvmode = 0; /* getvec mode */

static WFDB_Time istime;           /* time of next input sample */
static WFDB_Time msnsamples; /* duration of multi-segment record */
static unsigned ispfmax;   /* max number of samples of any open signal
				  per input frame */
static WFDB_Time ostime; /* time of next output sample */

static char date_string[37];
static char time_string[62];
static WFDB_Date pdays = -1;

static long msbtime;         /* base time for multi-segment record */
static WFDB_Date msbdate;     /* base date for multi-segment record */
static WFDB_Seginfo *segp, *segend;
static WFDB_Seginfo *segarray;

/* beginning, current segment, end pointers */
static struct WFDB_seginfo_L *segarray_L;

static struct hsdata {
    WFDB_Siginfo info; /* info about signal from header */
    long start;           /* signal file byte offset to sample 0 */
    int skew;           /* intersignal skew (in frames) */
} **hsd;

int wfdb_getc(WFDB_FILE *wp) {
    return (getc(wp->fp));
}

/* Read a line of text, allocating a buffer large enough to hold the
result.  Note that unlike the POSIX getline function, this function
returns zero at end of file. */
size_t wfdb_getline(char **buffer, size_t *buffer_size, WFDB_FILE *fp) {
    size_t n = (*buffer == nullptr ? 0 : *buffer_size), i = 0;
    void *s = *buffer;
    int c;

    do {
        if (i + 1 >= n) {
            n += 256;
            SREALLOC(s, n, 1);
            if (s == nullptr)
                break;
            *buffer = (char *) s;
            *buffer_size = n;
        }
        c = wfdb_getc(fp);
        if (c == EOF)
            break;
        ((char *) s)[i++] = c;
    } while (c != '\n');

    if (i > 0)
        (*buffer)[i] = 0;
    return (i);
}

FINT setsampfreq(WFDB_Frequency freq) {
    if (freq >= 0.) {
        sfreq = ffreq = freq;
        if (spfmax == 0)
            spfmax = 1;
        if ((gvmode & 1) == 1)
            sfreq *= spfmax;
        return (0);
    }
    std::cout << "setsampfreq: sampling frequency must not be negative\n";
    return (-1);
}

FDATE strdat(const char *string) {
    const char *mp, *yp;
    int d, m, y, gcorr, jm, jy;
    WFDB_Date date;

    if ((mp = strchr(string, '/')) == nullptr || (yp = strchr(mp + 1, '/')) == nullptr ||
        (d = strtol(string, nullptr, 10)) < 1 || d > 31 ||
        (m = strtol(mp + 1, nullptr, 10)) < 1 || m > 12 ||
        (y = strtol(yp + 1, nullptr, 10)) == 0)
        return (0L);
    if (m > 2) {
        jy = y;
        jm = m + 1;
    } else {
        jy = y - 1;
        jm = m + 13;
    }
    if (jy > 0)
        date = (WFDB_Date) (365.25 * jy);
    else
        date = -(long) (-365.25 * (jy + 0.25));
    date += (int) (30.6001 * jm) + d + 1720995L;
    if (d + 31L * (m + 12L * y) >= (15 + 31L * (10 + 12L * 1582))) {                              /* 15/10/1582 */
        gcorr = (int) (0.01 * jy); /* Gregorian calendar correction */
        date += 2 - gcorr + (int) (0.25 * gcorr);
    }
    return (date);
}

static WFDB_Time fstrtim(const char *string, WFDB_Frequency f) {
    const char *p, *q, *r;
    double x, y, z;
    WFDB_Date days;
    WFDB_Time t;

    while (*string == ' ' || *string == '\t' || *string == '\n' || *string == '\r')
        string++;
    switch (*string) {
        case 'c':
            return (cfreq > 0. ? (WFDB_Time) ((strtod(string + 1, nullptr) - bcount) * f / cfreq) : strtotime(
                    string + 1, nullptr, 10));
        case 'e':
            return ((in_msrec ? msnsamples : nsamples) *
                    (((gvmode & WFDB_HIGHRES) == WFDB_HIGHRES) ? spfmax : 1));
        case 'f':
            return (WFDB_Time) (strtotime(string + 1, nullptr, 10) * f / ffreq);
        case 'i':
            return (WFDB_Time) (istime *
                                (ifreq > 0.0 ? (ifreq / sfreq) : 1.0) *
                                (((gvmode & WFDB_HIGHRES) == WFDB_HIGHRES) ? ispfmax : 1));
        case 'o':
            return (ostime);
        case 's':
            return (strtotime(string + 1, nullptr, 10));
        case '[': /* time of day, possibly with date or days since start */
            if ((q = strchr(++string, ']')) == nullptr)
                return ((WFDB_Time) 0); /* '[...': malformed time string */
            if ((p = strchr(string, ' ')) == nullptr || p > q)
                days = (WFDB_Date) 0; /* '[hh:mm:ss.sss]': time since midnight only */
            else if ((r = strchr(p + 1, '/')) == nullptr || r > q)
                days = (WFDB_Date) strtol(p + 1, nullptr, 10); /* '[hh:mm:ss.sss d]' */
            else
                days = strdat(p + 1) - bdate; /* '[hh:mm:ss.sss dd/mm/yyyy]' */
            x = fstrtim(string, 1000.0) - btime;
            if (days > 0L)
                x += (days * (24 * 60 * 60 * 1000.0));
            t = (WFDB_Time) (x * f / 1000.0 + 0.5);
            return (-t);
        default:
            x = strtod(string, nullptr);
            if ((p = strchr(string, ':')) == nullptr)
                return ((long) (x * f + 0.5));
            y = strtod(++p, nullptr);
            if ((p = strchr(p, ':')) == nullptr)
                return ((long) ((60. * x + y) * f + 0.5));
            z = strtod(++p, nullptr);
            return ((WFDB_Time) ((3600. * x + 60. * y + z) * f + 0.5));
    }
}

FINT setbasetime(char *string) {
    char *p;

    pdays = -1;
    if (string == nullptr || *string == '\0') {
#ifndef NOTIME
        struct tm *now;
        time_t t;

        t = time((time_t *) nullptr);    /* get current time from system clock */
        now = localtime(&t);
        (void) sprintf(date_string, "%02d/%02d/%d",
                       now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
        bdate = strdat(date_string);
        (void) sprintf(time_string, "%d:%d:%d",
                       now->tm_hour, now->tm_min, now->tm_sec);
        btime = fstrtim(time_string, 1000.0);
#endif
        return (0);
    }
    while (*string == ' ') string++;
    p = strchr(string, ' ');
    if (p)
        *p++ = '\0';    /* split time and date components */
    btime = fstrtim(string, 1000.0);
    bdate = p ? strdat(p) : (WFDB_Date) 0;
    if (btime == 0L && bdate == (WFDB_Date) 0 && *string != '[') {
        if (p) *(--p) = ' ';
        std::cout << "setbasetime: incorrect time format, '%s' " << string << std::endl;
        return (-1);
    }
    return (0);
}

static int isfmt(int f) {
    int i;
    static int fmt_list[WFDB_NFMTS] = WFDB_FMT_LIST;

    for (i = 0; i < WFDB_NFMTS; i++)
        if (f == fmt_list[i])
            return (1);
    return (0);
}

static int readheader(const char *record, const char *filepath) {
    char *p, *q;
    WFDB_Frequency f;
    WFDB_Signal s;
    WFDB_Time ns;
    unsigned int i, nsig;
    static char sep[] = " \t\n\r";

//    /* If another input header file was opened, close it. */
//    if (hheader)
//    {
//        (void)wfdb_fclose(hheader);
//        hheader = nullptr;
//    }

    spfmax = 1;
    sfreq = ffreq;
    isedf = 0;
//    if (strcmp(record, "~") == 0)
//    {
//        if (in_msrec && vsd)
//        {
//            char *p;
//
//            SALLOC(hsd, 1, sizeof(struct hsdata *));
//            SALLOC(hsd[0], 1, sizeof(struct hsdata));
//            SSTRCPY(hsd[0]->info.desc, "~");
//            hsd[0]->info.spf = 1;
//            hsd[0]->info.fmt = 0;
//            hsd[0]->info.nsamp = nsamples = segp->nsamp;
//            return (maxhsig = 1);
//        }
//        return (0);
//    }

    /* If the final component of the record name includes a '.', assume it is a
       file name. */
//    q = (char *)record + strlen(record) - 1;
//    while (q > record && *q != '.' && *q != '/' && *q != ':' && *q != '\\')
//        q--;
//    if (*q == '.')
//    {
//        if ((hheader = wfdb_open(nullptr, record, WFDB_READ)) == nullptr)
//        {
////            wfdb_error("init: can't open %s\n", record);
//            return (-1);
//        }
////        else if (strcmp(q + 1, "hea")) /* assume EDF if suffix is not '.hea' */
////            return (edfparse(hheader));
//    }

    /* Otherwise, assume the file name is record.hea. */
    if ((hheader = wfdb_open("hea", filepath, WFDB_READ)) == nullptr) {
        std::cout << "init: can't open header for record " << record << std::endl;
        return (-1);
    }

    /* Read the first line and check for a magic string. */
    if (wfdb_getline(&linebuf, &linebufsize, hheader) == 0) {
        std::cout << "init: record %s header is empty " << record << std::endl;
        return (-2);
    }

    /* Get the first token (the record name) from the first non-empty,
       non-comment line. */
    while ((p = strtok(linebuf, sep)) == nullptr || *p == '#') {
        if (wfdb_getline(&linebuf, &linebufsize, hheader) == 0) {
            std::cout << "init: can't find record name in record %s header" << record << std::endl;
            return (-2);
        }
    }

    for (q = p + 1; *q && *q != '/'; q++);
    if (*q == '/') {
        if (in_msrec) {
            std::cout << "init: record %s cannot be nested in another multi-segment record " << record << std::endl;
            return (-2);
        }
        segments = (int) strtol(q + 1, nullptr, 10);
        *q = '\0';
    }

    /* For local files, be sure that the name (p) within the header file
       matches the name (record) provided as an argument to this function --
       if not, the header file may have been renamed in error or its contents
       may be corrupted.  The requirement for a match is waived for remote
       files since the user may not be able to make any corrections to them. */
    if (hheader->type == WFDB_LOCAL &&
        hheader->fp != stdin && strncmp(p, record, strlen(p)) != 0) {
        /* If there is a mismatch, check to see if the record argument includes
           a directory separator (whether valid or not for this OS);  if so,
           compare only the final portion of the argument against the name in
           the header file. */
        const char *q, *r, *s;

        for (r = record, q = s = r + strlen(r) - 1; r != s; s--)
            if (*s == '/' || *s == '\\' || *s == ':')
                break;

        if (q > s && (r > s || strcmp(p, s + 1) != 0)) {
            std::cout << "init: record name in record %s header is incorrect" << record << std::endl;
            return (-2);
        }
    }

    /* Identify which type of header file is being read by trying to get
       another token from the line which contains the record name.  (Old-style
       headers have only one token on the first line, but new-style headers
       have two or more.) */
    if ((p = strtok((char *) nullptr, sep)) == nullptr) {
        /* The file appears to be an old-style header file. */
        std::cout << "init: obsolete format in record %s header" << record << std::endl;
        return (-2);
    }

    /* The file appears to be a new-style header file.  The second token
       specifies the number of signals. */
    nsig = (unsigned) strtol(p, nullptr, 10);

    /* Determine the frame rate, if present and not set already. */
    p = strtok((char *) nullptr, sep);
    if (p) {
        if ((f = (WFDB_Frequency) strtod(p, nullptr)) <= (WFDB_Frequency) 0.) {
            std::cout << "init: sampling frequency in record %s header is incorrect" << record << std::endl;
            return (-2);
        }
        if (ffreq > (WFDB_Frequency) 0. && f != ffreq) {
            std::cout << "warning (init):\n";
            std::cout << " record %s sampling frequency differs" << record << std::endl;
            std::cout << " from that of previously opened record\n";
        } else
            ffreq = f;
    } else if (ffreq == (WFDB_Frequency) 0.)
        ffreq = WFDB_DEFFREQ;

    /* Set the sampling rate to the frame rate for now.  This may be
       changed later by isigopen or by setgvmode, if this is a multi-
       frequency record and WFDB_HIGHRES mode is in effect. */
    sfreq = ffreq;

    /* Determine the counter frequency and the base counter value. */
    cfreq = bcount = 0.0;
    if (p) {
        for (; *p && *p != '/'; p++);
        if (*p == '/') {
            cfreq = strtod(++p, nullptr);
            for (; *p && *p != '('; p++);
            if (*p == '(')
                bcount = strtod(++p, nullptr);
        }
    }
    if (cfreq <= 0.0)
        cfreq = ffreq;

    /* Determine the number of samples per signal, if present and not
       set already. */
    p = strtok((char *) nullptr, sep);
    if (p) {
        if ((ns = strtotime(p, nullptr, 10)) < 0L) {
            std::cout << "init: number of samples in record %s header is incorrect\n" << record << std::endl;
            return (-2);
        }
        if (nsamples == (WFDB_Time) 0L)
            nsamples = ns;
        else if (ns > (WFDB_Time) 0L && ns != nsamples && !in_msrec) {
            std::cout << "warning (init):\n";
            std::cout << " record %s duration differs" << record;
            std::cout << " from that of previously opened record\n";
            /* nsamples must match the shortest record duration. */
            if (nsamples > ns)
                nsamples = ns;
        }
    } else
        ns = (WFDB_Time) 0L;

    /* Determine the base time and date, if present and not set already. */
    if ((p = strtok((char *) nullptr, "\n\r")) != nullptr &&
        btime == 0L && setbasetime(p) < 0)
        return (-2); /* error message will come from setbasetime */

    /* Special processing for master header of a multi-segment record. */
    if (segments && !in_msrec) {
        msbtime = btime;
        msbdate = bdate;
        msnsamples = nsamples;
        /* Read the names and lengths of the segment records. */
        SALLOC(segarray, segments, sizeof(WFDB_Seginfo));
        SFREE(segarray_L);
        segp = segarray;
        for (i = 0, ns = (WFDB_Time) 0L; i < segments; i++, segp++) {
            /* Get next segment spec, skip empty lines and comments. */
            do {
                if (wfdb_getline(&linebuf, &linebufsize, hheader) == 0) {
                    std::cout << "init: unexpected EOF in header file for record %s\n" << record << std::endl;
                    SFREE(segarray);
                    segments = 0;
                    return (-2);
                }
            } while ((p = strtok(linebuf, sep)) == nullptr || *p == '#');
            if (*p == '+') {
                std::cout << "init: `%s' is not a valid segment name in record %s\n" << p << record << std::endl;
                SFREE(segarray);
                segments = 0;
                return (-2);
            }
            if (strlen(p) > WFDB_MAXRNL) {
                std::cout << "init: `%s' is too long for a segment name in record %s\n" << p << record << std::endl;
                SFREE(segarray);
                segments = 0;
                return (-2);
            }
            (void) strcpy(segp->recname, p);
            if ((p = strtok((char *) nullptr, sep)) == nullptr ||
                (segp->nsamp = strtotime(p, nullptr, 10)) < 0L) {
                std::cout << "init: length must be specified for segment %s in record %s\n" << segp->recname << record
                          << std::endl;
                SFREE(segarray);
                segments = 0;
                return (-2);
            }
            segp->samp0 = ns;
            ns += segp->nsamp;
        }
        segend = --segp;
        segp = segarray;
        if (msnsamples == 0L)
            msnsamples = ns;
        else if (ns != msnsamples) {
            std::cout << "warning (init): in record %s, " <<
                      "stated record length (%" WFDB_Pd_TIME ")\n" << record << msnsamples << std::endl;
            std::cout << " does not match sum of segment lengths " <<
                      "(%" WFDB_Pd_TIME ")\n" << ns << std::endl;
        }
        return (0);
    }

    /* Allocate workspace. */
    if (maxhsig < nsig) {
        unsigned m = maxhsig;

        SREALLOC(hsd, nsig, sizeof(struct hsdata *));
        while (m < nsig) {
            SUALLOC(hsd[m], 1, sizeof(struct hsdata));
            m++;
        }
        maxhsig = nsig;
    }

    /* Now get information for each signal. */
    for (s = 0; s < nsig; s++) {
        struct hsdata *hp, *hs;
        int nobaseline;

        hs = hsd[s];
        hp = (s ? hsd[s - 1] : nullptr);
        /* Get the first token (the signal file name) from the next
           non-empty, non-comment line. */
        do {
            if (wfdb_getline(&linebuf, &linebufsize, hheader) == 0) {
                std::cout << "init: unexpected EOF in header file for record %s\n" << record << std::endl;
                return (-2);
            }
        } while ((p = strtok(linebuf, sep)) == nullptr || *p == '#');

        /* Determine the signal group number.  The group number for signal
           0 is zero.  For subsequent signals, if the file name does not
           match that of the previous signal, the group number is one
           greater than that of the previous signal. */
        if (s == 0 || strcmp(p, hp->info.fname)) {
            hs->info.group = (s == 0) ? 0 : hp->info.group + 1;
            SSTRCPY(hs->info.fname, p);
        }
            /* If the file names of the current and previous signals match,
               they are assigned the same group number and share a copy of the
               file name.  All signals associated with a given file must be
               listed together in the header in order to be identified as
               belonging to the same group;  readheader does not check that
               this has been done. */
        else {
            hs->info.group = hp->info.group;
            SSTRCPY(hs->info.fname, hp->info.fname);
        }

        /* Determine the signal format. */
        if ((p = strtok((char *) nullptr, sep)) == nullptr ||
            !isfmt(hs->info.fmt = strtol(p, nullptr, 10))) {
            std::cout << "init: illegal format for signal %d, record %s\n" << s << record << std::endl;
            return (-2);
        }
        hs->info.spf = 1;
        hs->skew = 0;
        hs->start = 0L;
        while (*(++p)) {
            if (*p == 'x' && *(++p))
                if ((hs->info.spf = strtol(p, nullptr, 10)) < 1)
                    hs->info.spf = 1;
            if (*p == ':' && *(++p))
                if ((hs->skew = strtol(p, nullptr, 10)) < 0)
                    hs->skew = 0;
            if (*p == '+' && *(++p))
                if ((hs->start = strtol(p, nullptr, 10)) < 0L)
                    hs->start = 0L;
        }
        if (hs->info.spf > spfmax)
            spfmax = hs->info.spf;
        /* The resolution for deskewing is one frame.  The skew in samples
           (given in the header) is converted to skew in frames here. */
        hs->skew = (int) (((double) hs->skew) / hs->info.spf + 0.5);

        /* Determine the gain in ADC units per physical unit.  This number
           may be zero or missing;  if so, the signal is uncalibrated. */
        p = strtok((char *) nullptr, sep);
        if (p)
            hs->info.gain = (WFDB_Gain) strtod(p, nullptr);
        else
            hs->info.gain = (WFDB_Gain) 0.;

        /* Determine the baseline if specified, and the physical units
           (assumed to be millivolts unless otherwise specified). */
        nobaseline = 1;
        if (p) {
            for (; *p && *p != '(' && *p != '/'; p++);
            if (*p == '(') {
                hs->info.baseline = (int) strtol(++p, nullptr, 10);
                nobaseline = 0;
            }
            while (*p)
                if (*p++ == '/' && *p)
                    break;
        }
        if (p && *p) {
            SALLOC(hs->info.units, WFDB_MAXUSL + 1, 1);
            (void) strncpy(hs->info.units, p, WFDB_MAXUSL);
        } else
            hs->info.units = nullptr;

        /* Determine the ADC resolution in bits.  If this number is
           missing and cannot be inferred from the format, the default
           value (from wfdb.h) is filled in. */
        p = strtok((char *) nullptr, sep);
        if (p)
            i = (unsigned) strtol(p, nullptr, 10);
        else
            switch (hs->info.fmt) {
                case 80:
                    i = 8;
                    break;
                case 160:
                    i = 16;
                    break;
                case 212:
                    i = 12;
                    break;
                case 310:
                case 311:
                    i = 10;
                    break;
                default:
                    i = WFDB_DEFRES;
                    break;
            }
        hs->info.adcres = i;

        /* Determine the ADC zero (assumed to be zero if missing). */
        hs->info.adczero = (p = strtok((char *) nullptr, sep)) ? strtol(p, nullptr, 10) : 0;

        /* Set the baseline to adczero if no baseline field was found. */
        if (nobaseline)
            hs->info.baseline = hs->info.adczero;

        /* Determine the initial value (assumed to be equal to the ADC
           zero if missing). */
        hs->info.initval = (p = strtok((char *) nullptr, sep)) ? strtol(p, nullptr, 10) : hs->info.adczero;

        /* Determine the checksum (assumed to be zero if missing). */
        p = strtok((char *) nullptr, sep);
        if (p) {
            hs->info.cksum = (int) strtol(p, nullptr, 10);
            hs->info.nsamp = (ns > LONG_MAX ? 0 : ns);
        } else {
            hs->info.cksum = 0;
            hs->info.nsamp = (WFDB_Time) 0L;
        }

        /* Determine the block size (assumed to be zero if missing). */
        hs->info.bsize = (p = strtok((char *) nullptr, sep)) ? strtol(p, nullptr, 10) : 0;

        /* Check that formats and block sizes match for signals belonging
           to the same group. */
        if (s && (hp == nullptr || (hs->info.group == hp->info.group &&
                                    (hs->info.fmt != hp->info.fmt ||
                                     hs->info.bsize != hp->info.bsize)))) {
            std::cout << "init: error in specification of signal %d or %d\n" << s - 1 << s << std::endl;
            return (-2);
        }

        /* Get the signal description.  If missing, a description of
           the form "record xx, signal n" is filled in. */
        SALLOC(hs->info.desc, 1, WFDB_MAXDSL + 1);
        p = strtok((char *) nullptr, "\n\r");
        if (p)
            (void) strncpy(hs->info.desc, p, WFDB_MAXDSL);
        else
            (void) sprintf(hs->info.desc,
                           "record %s, signal %d", record, s);
    }
//    setgvmode(gvmode); /* Reset sfreq if appropriate. */
    return (s);           /* return number of available signals */
}

/* These variables relate to open input signals. */
static unsigned maxisig;   /* max number of input signals */
static unsigned maxigroup; /* max number of input signal groups */
static unsigned nisig;       /* number of open input signals */
static unsigned nigroup;   /* number of open input signal groups */
//static unsigned ispfmax;   /* max number of samples of any open signal

static struct isdata {                       /* unique for each input signal */
    WFDB_Siginfo info; /* input signal information */
    WFDB_Sample samp;  /* most recent sample read */
    int skew;           /* intersignal skew (in frames) */
    int gvindex;       /* current high-resolution sample number */
    int gvcount;       /* counter for updating gvindex */
} **isd;

/* Allocate workspace for up to n input signals. */
static int allocisig(unsigned int n) {
    if (maxisig < n) {
        unsigned m = maxisig;

        SREALLOC(isd, n, sizeof(struct isdata *));
        while (m < n) {
            SUALLOC(isd[m], 1, sizeof(struct isdata));
            m++;
        }
        maxisig = n;
    }
    return (maxisig);
}

static struct igdata {                                  /* shared by all signals in a group (file) */
    int data;                      /* raw data read by r*() */
    int datb;                      /* more raw data used for bit-packed formats */
    WFDB_FILE *fp;                  /* file pointer for an input signal group */
    long start;                      /* signal file byte offset to sample 0 */
    int bsize;                      /* if non-zero, all reads from the input file
							 are in multiples of bsize bytes */
    char *buf;                      /* pointer to input buffer */
    char *bp;                      /* pointer to next location in buf[] */
    char *be;                      /* pointer to input buffer endpoint */
    FLAC__StreamDecoder *flacdec; /* internal state for FLAC decoder */
    char *packptr;                  /* pointer to next partially-decoded frame */
    unsigned packspf;              /* number of samples per signal per frame */
    unsigned packcount;              /* number of samples decoded in this frame */
    char count;                      /* input counter for bit-packed signal */
    char seek;                      /* 0: do not seek on file, 1: seeks permitted */
    char initial_skip;              /* 1 if isgsetframe is needed before reading */
    int stat;                      /* signal file status flag */
} **igd;

static unsigned framelen;           /* total number of samples per frame */


/* Allocate workspace for up to n input signal groups. */
static int allocigroup(unsigned int n) {
    if (maxigroup < n) {
        unsigned m = maxigroup;

        SREALLOC(igd, n, sizeof(struct igdata *));
        while (m < n) {
            SUALLOC(igd[m], 1, sizeof(struct igdata));
            m++;
        }
        maxigroup = n;
    }
    return (maxigroup);
}

static int ibsize;                   /* default input buffer size */
static unsigned skewmax;           /* max skew (frames) between any 2 signals */

static int copysi(WFDB_Siginfo *to, const WFDB_Siginfo *from) {
    if (to == nullptr || from == nullptr)
        return (0);
    *to = *from;
    to->fname = to->desc = to->units = nullptr;
    SSTRCPY(to->fname, from->fname);
    SSTRCPY(to->desc, from->desc);
    SSTRCPY(to->units, from->units);
    return (1);
}

static int need_sigmap, maxvsig, nvsig, tspf, vspfmax;
//static struct isdata **vsd;
static WFDB_Sample *ovec;

static struct sigmapinfo {
    char *desc;
    double gain, scale, offset;
    WFDB_Sample sample_offset;
    WFDB_Sample baseline;
    int index;
    int spf;
} *smi;


static int make_vsd(void) {
    int i;

    if (nvsig != nisig) {
        std::cout << "make_vsd: oops! nvsig = %d, nisig = %d\n" << nvsig << nisig << std::endl;
        return (-1);
    }
    for (i = 0; i < maxvsig; i++) {
        SFREE(vsd[i]->info.fname);
        SFREE(vsd[i]->info.desc);
        SFREE(vsd[i]->info.units);
    }
    if (maxvsig < nvsig) {
        unsigned m = maxvsig;

        SREALLOC(vsd, nvsig, sizeof(struct isdata *));
        while (m < nvsig) {
            SUALLOC(vsd[m], 1, sizeof(struct isdata));
            m++;
        }
        maxvsig = nvsig;
    }

    for (i = 0; i < nvsig; i++) {
        copysi(&vsd[i]->info, &isd[i]->info);
        vsd[i]->skew = isd[i]->skew;
    }

    return (nvsig);
}

static int sigmap_init(int first_segment) {
    int i, j, k, kmax, s, ivmin, ivmax;
    double ovmin, ovmax;
    struct sigmapinfo *ps;

    /* is this the layout segment?  if so, set up output side of map */
    if (in_msrec && first_segment && segarray && segarray[0].nsamp == 0) {
        need_sigmap = 1;

        /* The number of virtual signals is the number of signals defined
           in the layout segment. */
        nvsig = nisig;
        vspfmax = ispfmax;
        for (s = tspf = 0; s < nisig; s++)
            tspf += isd[s]->info.spf;
        SALLOC(smi, tspf, sizeof(struct sigmapinfo));
        for (i = s = 0; i < nisig; i++) {
            SSTRCPY(smi[s].desc, isd[i]->info.desc);
            smi[s].gain = isd[i]->info.gain;
            smi[s].baseline = isd[i]->info.baseline;
            k = smi[s].spf = isd[i]->info.spf;
            for (j = 1; j < k; j++)
                smi[s + j] = smi[s];
            s += k;
        }
        SALLOC(ovec, tspf, sizeof(WFDB_Sample));
        return (make_vsd());
    } else if (need_sigmap) { /* set up the input side of the map */
        for (s = 0; s < tspf; s++) {
            smi[s].index = 0;
            smi[s].scale = 0.;
            smi[s].offset = 0.;
            smi[s].sample_offset = WFDB_INVALID_SAMPLE;
        }
        ispfmax = vspfmax;

        if (isd[0]->info.fmt == 0 && nisig == 1)
            return (0); /* the current segment is a null record */

        for (i = j = 0; i < nisig; j += isd[i++]->info.spf)
            for (s = 0; s < tspf; s += smi[s].spf)
                if (strcmp(smi[s].desc, isd[i]->info.desc) == 0) {
                    if ((kmax = smi[s].spf) != isd[i]->info.spf) {
                        std::cout << "sigmap_init: unexpected spf for signal %d in segment %s\n" << i << segp->recname
                                  << std::endl;

                        if (kmax > isd[i]->info.spf)
                            kmax = isd[i]->info.spf;
                    }
                    for (k = 0; k < kmax; k++) {
                        ps = &smi[s + k];
                        ps->index = j + k;
                        ps->scale = ps->gain / isd[i]->info.gain;
                        if (ps->scale < 1.0)
                            std::cout << "sigmap_init: loss of precision in signal %d in segment %s\n" << i
                                      << segp->recname << std::endl;
                        ps->offset = ps->baseline -
                                     ps->scale * isd[i]->info.baseline + 0.5;

                        /* If it is possible to add an additional
                           offset such that all possible output values
                           will fit into a positive signed integer, we
                           can use the "fast" case in sigmap, below. */
                        switch (isd[i]->info.fmt) {
                            case 508:
                            case 80:
                                ivmin = -0x80;
                                ivmax = 0x7f;
                                break;
                            case 310:
                            case 311:
                                ivmin = -0x200;
                                ivmax = 0x1ff;
                                break;
                            case 212:
                                ivmin = -0x800;
                                ivmax = 0x7ff;
                                break;
                            case 16:
                            case 61:
                            case 516:
                            case 160:
                                ivmin = -0x8000;
                                ivmax = 0x7fff;
                                break;
                            case 524:
                            case 24:
                                ivmin = -0x800000;
                                ivmax = 0x7fffff;
                                break;
                            default:
                                ivmin = WFDB_SAMPLE_MIN;
                                ivmax = WFDB_SAMPLE_MAX;
                                break;
                        }
                        ovmin = ivmin * ps->scale + ps->offset;
                        ovmax = ivmax * ps->scale + ps->offset;
                        if (ovmin < ovmax &&
                            ovmin >= WFDB_SAMPLE_MIN + 1 &&
                            ovmax <= WFDB_SAMPLE_MAX &&
                            ovmax - ovmin + 1 < WFDB_SAMPLE_MAX) {
                            ps->sample_offset = ovmin - 1;
                            ps->offset -= ps->sample_offset;
                        } else {
                            ps->sample_offset = 0;
                        }
                    }
                    break;
                }
        if (j > tspf) {
            std::cout << "sigmap_init: frame size too large in segment %s\n" << segp->recname << std::endl;
            return (-1);
        }
    } else if (in_msrec && !first_segment && framelen == 0) {
        /* opening a new segment of a fixed-layout multisegment
           record */
        ispfmax = vspfmax;
        if (nisig > nvsig) {
            std::cout << "sigmap_init: wrong number of signals in segment %s\n" << segp->recname << std::endl;
            return (-1);
        }
        for (i = 0; i < nisig; i++) {
            if (isd[i]->info.spf != vsd[i]->info.spf) {
                std::cout << "sigmap_init: wrong spf for signal %d in segment %s\n" << i << segp->recname << std::endl;
                return (-1);
            }
        }
    } else { /* normal record, or multisegment record without a dummy
	 header */
        nvsig = nisig;
        vspfmax = ispfmax;
        for (s = tspf = 0; s < nisig; s++)
            tspf += isd[s]->info.spf;
        return (make_vsd());
    }

    return (0);
}

static WFDB_Sample *sbuf = nullptr;   /* buffer used by sample() */
static int sample_vflag;           /* if non-zero, last value returned by sample()
						  was valid */

int wfdb_fclose(WFDB_FILE *wp) {
    int status;

    status = fclose(wp->fp);
    if (wp->fp != stdin)
        SFREE(wp);
    return (status);
}

static int gvc;                       /* getvec sample-within-frame counter */
#define DEFWFDBGVMODE WFDB_LOWRES
typedef void FVOID;

FVOID setgvmode(int mode) {
    if (mode < 0) { /* (re)set to default mode */
        char *p;

        if (p = getenv("WFDBGVMODE"))
            mode = strtol(p, nullptr, 10);
        else
            mode = DEFWFDBGVMODE;
    }

    gvmode = mode & (WFDB_HIGHRES | WFDB_GVPAD);

    if ((mode & WFDB_HIGHRES) == WFDB_HIGHRES) {
        if (spfmax == 0)
            spfmax = 1;
        sfreq = ffreq * spfmax;
    } else {
        sfreq = ffreq;
    }
}

static unsigned nosig;       /* number of open output signals */
static void hsdfree(void) {
    struct hsdata *hs;

    if (hsd) {
        while (maxhsig)
            if (hs = hsd[--maxhsig]) {
                SFREE(hs->info.fname);
                SFREE(hs->info.units);
                SFREE(hs->info.desc);
                SFREE(hs);
            }
        SFREE(hsd);
    }
    maxhsig = 0;
}


static void isigclose(void) {
    struct isdata *is;
    struct igdata *ig;

    if (sbuf && !in_msrec) {
        SFREE(sbuf);
        sample_vflag = 0;
    }
    if (isd) {
        while (maxisig)
            if (is = isd[--maxisig]) {
                SFREE(is->info.fname);
                SFREE(is->info.units);
                SFREE(is->info.desc);
                SFREE(is);
            }
        SFREE(isd);
    }
    maxisig = nisig = 0;
    framelen = 0;

    if (igd) {
        while (maxigroup)
            if (ig = igd[--maxigroup]) {
//                if (ig->flacdec)
//                    flac_isclose(ig);
                if (ig->fp)
                    (void) wfdb_fclose(ig->fp);
                SFREE(ig->buf);
                SFREE(ig);
            }
        SFREE(igd);
    }
    maxigroup = nigroup = 0;

    istime = 0L;
    gvc = ispfmax = 1;
    if (hheader) {
        (void) wfdb_fclose(hheader);
        hheader = nullptr;
    }
    SFREE(linebuf);
    if (nosig == 0 && maxhsig != 0)
        hsdfree();
}

static int tuvlen;                   /* lengths of tvector and uvector in samples */
static WFDB_Sample *tvector;       /* getvec workspace */
static WFDB_Sample *uvector;       /* isgsettime workspace */
static WFDB_Sample *vvector;       /* tnextvec workspace */
static WFDB_Sample *dsbuf;           /* deskewing buffer */

static int dsbi;                   /* index to oldest sample in dsbuf (if < 0,
							  dsbuf does not contain valid data) */
static unsigned dsblen;               /* capacity of dsbuf, in samples */

FINT isigopen(const char *record, const char *filepath, WFDB_Siginfo *siarray, int nsig) {
    int navail, nn, spflimit;
    int first_segment = 0;
    struct hsdata *hs;
    struct isdata *is;
    struct igdata *ig;
    WFDB_Signal s, si, sj;
    WFDB_Group g;

    /* Close previously opened input signals unless otherwise requested. */
    if (*record == '+') record++;
    else isigclose();

    /* Remove trailing .hea, if any, from record name. */
//    wfdb_striphea(record);

    /* Save the current record name. */
//    if (!in_msrec) wfdb_setirec(record);

    /* Read the header and determine how many signals are available. */
    if ((navail = readheader(record, filepath)) <= 0) {
        if (navail == 0 && segments) {    /* this is a multi-segment record */
            in_msrec = 1;
            first_segment = 1;
            /* Open the first segment to get signal information. */
            if (segp && (navail = readheader(segp->recname, filepath)) >= 0) {
                if (msbtime == 0L) msbtime = btime;
                if (msbdate == (WFDB_Date) 0) msbdate = bdate;
            }
            if (nsig <= 0)
                in_msrec = 0;
        }
        if (navail == 0 && nsig)
            std::cout << "isigopen: record %s has no signals\n" << record << std::endl;
        if (navail <= 0)
            return (navail);
    }

    /* If nsig <= 0, isigopen fills in up to (-nsig) members of siarray based
       on the contents of the header, but no signals are actually opened.  The
       value returned is the number of signals named in the header. */
    if (nsig <= 0) {
        nsig = -nsig;
        if (navail < nsig) nsig = navail;
        if (siarray != nullptr)
            for (s = 0; s < nsig; s++)
                siarray[s] = hsd[s]->info;
        in_msrec = 0;    /* necessary to avoid errors when reopening */
        return (navail);
    }

    /* Determine how many new signals we should attempt to open.  The caller's
       upper limit on this number is nsig, and the upper limit defined by the
       header is navail. */
    if (nsig > navail) nsig = navail;

    /* Allocate input signals and signal group workspace. */
    nn = nisig + nsig;
    if (allocisig(nn) != nn)
        return (-1);    /* failed, nisig is unchanged, allocisig emits error */
    nn = nigroup + hsd[navail - 1]->info.group + 1;
    if (nn > nigroup + nsig)
        nn = nigroup + nsig;
    if (allocigroup(nn) != nn)
        return (-1);    /* failed, allocigroup emits error */

    /* Set default buffer size (if not set already by setibsize). */
    if (ibsize <= 0) ibsize = BUFSIZ;

    /* Open the signal files.  One signal group is handled per iteration.  In
       this loop, si counts through the entries that have been read from hsd,
       and s counts the entries that have been added to isd. */
    for (g = si = s = 0; si < navail && s < nsig; si = sj) {
        hs = hsd[si];
        is = isd[nisig + s];
        ig = igd[nigroup + g];

        /* Find out how many signals are in this group. */
        for (sj = si + 1; sj < navail; sj++)
            if (hsd[sj]->info.group != hs->info.group) break;

        /* Skip this group if there are too few slots in the caller's array. */
        if (sj - si > nsig - s) continue;

        /* Set the buffer size and the seek capability flag. */
        if (hs->info.bsize < 0) {
            ig->bsize = hs->info.bsize = -hs->info.bsize;
            ig->seek = 0;
        } else {
            if ((ig->bsize = hs->info.bsize) == 0) ig->bsize = ibsize;
            ig->seek = 1;
        }
        SALLOC(ig->buf, 1, ig->bsize);

        /* Check that the signal file is readable. */
        if (hs->info.fmt == 0)
            ig->fp = nullptr;    /* Don't open a file for a null signal. */
        else {
            ig->fp = wfdb_open(hs->info.fname, (char *) nullptr, WFDB_READ);
            /* Skip this group if the signal file can't be opened. */
            if (ig->fp == nullptr) {
                SFREE(ig->buf);
                continue;
            }
        }

        /* All tests passed -- fill in remaining data for this group. */
        ig->be = ig->bp = ig->buf + ig->bsize;
        ig->start = hs->start;
        ig->stat = 1;
        while (si < sj && s < nsig) {
            copysi(&is->info, &hs->info);
            is->info.group = nigroup + g;
            is->skew = hs->skew;
            ++s;
            if (++si < sj) {
                hs = hsd[si];
                is = isd[nisig + s];
            }
        }
        g++;
    }

    /* Produce a warning message if none of the requested signals could be
       opened. */
    if (s == 0 && nsig)
        std::cout << "isigopen: none of the signals for record %s is readable\n" << record << std::endl;

    /* Check that the total number of samples per frame is less than
       or equal to INT_MAX. */
    spflimit = INT_MAX - framelen;
    for (si = 0; si < s; si++) {
        spflimit -= isd[nisig + si]->info.spf;
        if (spflimit < 0) {
            std::cout << "isigopen: total frame size too large in record %s\n" << record << std::endl;
//            isigclose(); // TODO chyba trzeba nprwic
            return (-3);
        }
    }

    /* Copy the WFDB_Siginfo structures to the caller's array.  Use these
       data to construct the initial sample vector, and to determine the
       maximum number of samples per signal per frame and the maximum skew. */
    for (si = 0; si < s; si++) {
        is = isd[nisig + si];
        if (siarray)
            copysi(&siarray[si], &is->info);
        is->samp = is->info.initval;
        if (ispfmax < is->info.spf) ispfmax = is->info.spf;
        if (skewmax < is->skew) skewmax = is->skew;
    }
    nisig += s;        /* Update the count of open input signals. */
    nigroup += g;    /* Update the count of open input signal groups. */
    if (sigmap_init(first_segment) < 0) {
        isigclose();
        return (-3);
    }
    setgvmode(gvmode);    /* Reset sfreq if appropriate. */
    gvc = ispfmax;    /* Initialize getvec's sample-within-frame counter. */

    /* Determine the total number of samples per frame. */
    for (si = framelen = 0; si < nisig; si++)
        framelen += isd[si]->info.spf;

    /* Allocate workspace for getvec, isgsettime, and tnextvec. */
    if (tspf > tuvlen) {
        SALLOC(tvector, tspf, sizeof(WFDB_Sample));
        SALLOC(uvector, tspf, sizeof(WFDB_Sample));
        SALLOC(vvector, tspf, sizeof(WFDB_Sample));
        tuvlen = tspf;
    }

    /* If deskewing is required, allocate the deskewing buffer (unless this is
       a multi-segment record and dsbuf has been allocated already). */
    if (skewmax != 0 && (!in_msrec || dsbuf == nullptr)) {
        if (tspf > INT_MAX / (skewmax + 1)) {
            std::cout << "isigopen: maximum skew too large in record %s\n" << record << std::endl;
            isigclose();
            return (-3);
        }
        dsbi = -1;    /* mark buffer contents as invalid */
        dsblen = tspf * (skewmax + 1);
        SALLOC(dsbuf, dsblen, sizeof(WFDB_Sample));
    }
    return (s);
}


static int flac_isseek(struct igdata *ig, WFDB_Time t) {
    return (-1);
}


size_t wfdb_fread(void *ptr, size_t size, size_t nmemb, WFDB_FILE *wp) {
    return (fread(ptr, size, nmemb, wp->fp));
}

static int _l;    /* macro temporary storage for low byte of word */
static int _lw; /* macro temporary storage for low 16 bits of int */
static int _n;    /* macro temporary storage for byte count */

/* VFILL provides the value returned by getskewedframe() for a missing or
   invalid sample */
#define VFILL ((gvmode & WFDB_GVPAD) ? is->samp : WFDB_INVALID_SAMPLE)
#define r8(G) ((G->bp < G->be) ? *(G->bp++) : ((_n = (G->bsize > 0) ? G->bsize : ibsize), (G->stat = _n = wfdb_fread(G->buf, 1, _n, G->fp)), (G->be = (G->bp = G->buf) + _n), *(G->bp++)))

static int r16(struct igdata *g) {
    int l, h;

    l = r8(g);
    h = r8(g);
    return ((int) ((short) ((h << 8) | (l & 0xff))));
}

/* r212: read and return the next sample from a format 212 signal file
   (2 12-bit samples bit-packed in 3 bytes) */
static int r212(struct igdata *g) {
    int v;

    /* Obtain the next 12-bit value right-justified in v. */
    switch (g->count++) {
        case 0:
            v = g->data = r16(g);
            break;
        case 1:
        default:
            g->count = 0;
            v = ((g->data >> 4) & 0xf00) | (r8(g) & 0xff);
            break;
    }
    /* Sign-extend from the twelfth bit. */
    if (v & 0x800)
        v |= ~(0xfff);
    else
        v &= 0xfff;
    return (v);
}


/* r310: read and return the next sample from a format 310 signal file
   (3 10-bit samples bit-packed in 4 bytes) */
static int r310(struct igdata *g) {
    int v;

    /* Obtain the next 10-bit value right-justified in v. */
    switch (g->count++) {
        case 0:
            v = (g->data = r16(g)) >> 1;
            break;
        case 1:
            v = (g->datb = r16(g)) >> 1;
            break;
        case 2:
        default:
            g->count = 0;
            v = ((g->data & 0xf800) >> 11) | ((g->datb & 0xf800) >> 6);
            break;
    }
    /* Sign-extend from the tenth bit. */
    if (v & 0x200)
        v |= ~(0x3ff);
    else
        v &= 0x3ff;
    return (v);
}


/* r311: read and return the next sample from a format 311 signal file
   (3 10-bit samples bit-packed in 4 bytes; note that formats 310 and 311
   differ in the layout of the bit-packed data) */
static int r311(struct igdata *g) {
    int v;

    /* Obtain the next 10-bit value right-justified in v. */
    switch (g->count++) {
        case 0:
            v = (g->data = r16(g));
            break;
        case 1:
            g->datb = (r8(g) & 0xff);
            v = ((g->data & 0xfc00) >> 10) | ((g->datb & 0xf) << 6);
            break;
        case 2:
        default:
            g->count = 0;
            g->datb |= r8(g) << 8;
            v = g->datb >> 4;
            break;
    }
    /* Sign-extend from the tenth bit. */
    if (v & 0x200)
        v |= ~(0x3ff);
    else
        v &= 0x3ff;
    return (v);
}

int wfdb_fseek(WFDB_FILE *wp, long int offset, int whence) {
    return (fseek(wp->fp, offset, whence));
}

int wfdb_feof(WFDB_FILE *wp) {
    return (feof(wp->fp));
}


static int isgsetframe(WFDB_Group g, WFDB_Time t) {
    int i, trem = 0;
    long nb, tt;
    struct igdata *ig;
    WFDB_Signal s;
    unsigned int b, d = 1, n, nn, j;

    /* Do nothing if there is no more than one input signal group and
       the input pointer is correct already. */
    if (nigroup < 2 && istime == t && gvc == ispfmax &&
        igd[g]->start == 0)
        return (0);

    /* Find the first signal that belongs to group g. */
    for (s = 0; s < nisig && g != isd[s]->info.group; s++);
    if (s == nisig) {
        std::cout << "isgsettime: incorrect signal group number %d\n" << g << std::endl;
        return (-2);
    }

    /* If the current record contains multiple segments, locate the segment
       containing the desired sample. */
//    if (in_msrec)
//    {
//        WFDB_Seginfo *tseg = segp;
//        WFDB_Group h;
//
//        if (t >= msnsamples)
//        {
//            wfdb_error("isigsettime: improper seek on signal group %d\n", g);
//            return (-1);
//        }
//        while (t < tseg->samp0)
//            tseg--;
//        while (t >= tseg->samp0 + tseg->nsamp && tseg < segend)
//            tseg++;
//        if (segp != tseg)
//        {
//            segp = tseg;
//            if (isigopen(segp->recname, nullptr, (int)nvsig) <= 0)
//            {
//                wfdb_error("isigsettime: can't open segment %s\n",
//                           segp->recname);
//                return (-1);
//            }
//            /* Following isigopen(), nigroup may have changed and
//               group numbers may not make any sense anymore.  However,
//               isigsettime() will still call isgsettime() once for
//               each non-zero group (if the previous segment had
//               multiple groups) and then once for group zero.
//
//               Calling isgsetframe() multiple times for a non-zero
//               group is mostly harmless, but seeking on group 0 should
//               only be done once.  Thus, when we jump to a new
//               segment, implicitly seek on all non-zero groups
//               (regardless of g), but only seek on group 0 if g is 0.
//
//               (Note that isgsettime() is not and has never been fully
//               functional for multi-segment records, because it cannot
//               read signals from two different segments at once.) */
//            for (h = 1; h < nigroup; h++)
//                if (i = isgsetframe(h, t))
//                    return (i);
//            if (g == 0)
//                return (isgsetframe(0, t));
//            else
//                return (0);
//        }
//        t -= segp->samp0;
//    }

    ig = igd[g];
    ig->initial_skip = 0;
    /* Determine the number of samples per frame for signals in the group. */
    for (n = nn = 0; s + n < nisig && isd[s + n]->info.group == g; n++)
        nn += isd[s + n]->info.spf;
    /* Determine the number of bytes per sample interval in the file. */
    switch (isd[s]->info.fmt) {
        case 0:
            if (t < nsamples) {
                gvc = ispfmax;
                if (s == 0)
                    istime = (in_msrec) ? t + segp->samp0 : t;
                isd[s]->info.nsamp = nsamples - t;
                ig->stat = 1;
                return (0);
            } else {
                if (s == 0)
                    istime = (in_msrec) ? msnsamples : nsamples;
                isd[s]->info.nsamp = 0L;
                return (-1);
            }

        case 508:
        case 516:
        case 524:
            if (flac_isseek(ig, t) < 0) {
                std::cout << "isigsettime: improper seek on signal group %d\n" << g << std::endl;
                return (-1);
            }

            /* Reset the getvec sample-within-frame counter. */
            gvc = ispfmax;

            /* Reset the time (if signal 0 belongs to the group) and
               disable checksum testing (by setting the number of samples
               remaining to 0). */
            if (s == 0)
                istime = in_msrec ? t + segp->samp0 : t;
            while (n-- != 0)
                isd[s + n]->info.nsamp = (WFDB_Time) 0L;
            return (0);

        case 8:
        case 80:
        default:
            b = nn;
            break;
        case 16:
        case 61:
        case 160:
            if (nn > UINT_MAX / 2) {
                std::cout << "isigsettime: overflow in signal group %d\n" << g << std::endl;
                return (-1);
            }
            b = 2 * nn;
            break;
        case 212:
            if (nn > UINT_MAX / 3) {
                std::cout << "isigsettime: overflow in signal group %d\n" << g << std::endl;
                return (-1);
            }
            /* Reset the input counter. */
            ig->count = 0;
            /* If the desired sample does not lie on a byte boundary, seek to
               the previous sample and then read ahead. */
            if ((nn & 1) && (t & 1)) {
                if (in_msrec)
                    t += segp->samp0; /* restore absolute time */
                if (i = isgsetframe(g, t - 1))
                    return (i);
                for (j = 0; j < nn; j++)
                    (void) r212(ig);
                istime++;
                for (n = 0; s + n < nisig && isd[s + n]->info.group == g; n++)
                    isd[s + n]->info.nsamp = (WFDB_Time) 0L;
                return (0);
            }
            b = 3 * nn;
            d = 2;
            break;
        case 310:
            if (nn > UINT_MAX / 4) {
                std::cout << "isigsettime: overflow in signal group %d\n" << g << std::endl;
                return (-1);
            }
            /* Reset the input counter. */
            ig->count = 0;
            /* If the desired sample does not lie on a byte boundary, seek to
               the closest previous sample that does, then read ahead. */
            if ((nn % 3) && (trem = (t % 3))) {
                if (in_msrec)
                    t += segp->samp0; /* restore absolute time */
                if (i = isgsetframe(g, t - trem))
                    return (i);
                for (j = nn * trem; j > 0; j--)
                    (void) r310(ig);
                istime += trem;
                for (n = 0; s + n < nisig && isd[s + n]->info.group == g; n++)
                    isd[s + n]->info.nsamp = (WFDB_Time) 0L;
                return (0);
            }
            b = 4 * nn;
            d = 3;
            break;
        case 311:
            if (nn > UINT_MAX / 4) {
                std::cout << "isigsettime: overflow in signal group %d\n" << g << std::endl;
                return (-1);
            }
            /* Reset the input counter. */
            ig->count = 0;
            /* If the desired sample does not lie on a byte boundary, seek to
               the closest previous sample that does, then read ahead. */
            if ((nn % 3) && (trem = (t % 3))) {
                if (in_msrec)
                    t += segp->samp0; /* restore absolute time */
                if (i = isgsetframe(g, t - trem))
                    return (i);
                for (j = nn * trem; j > 0; j--)
                    (void) r311(ig);
                istime += trem;
                for (n = 0; s + n < nisig && isd[s + n]->info.group == g; n++)
                    isd[s + n]->info.nsamp = (WFDB_Time) 0L;
                return (0);
            }
            b = 4 * nn;
            d = 3;
            break;
        case 24:
            if (nn > UINT_MAX / 3) {
                std::cout << "isigsettime: overflow in signal group %d\n" << g << std::endl;
                return (-1);
            }
            b = 3 * nn;
            break;
        case 32:
            if (nn > UINT_MAX / 4) {
                std::cout << "isigsettime: overflow in signal group %d\n" << g << std::endl;
                return (-1);
            }
            b = 4 * nn;
            break;
    }

    if (t > (LONG_MAX / b) || ((long) (t * b)) / d > (LONG_MAX - ig->start)) {
        std::cout << "isigsettime: improper seek on signal group %d\n" << g << std::endl;
        return (-1);
    }

    /* Seek to the beginning of the block which contains the desired sample.
       For normal files, use fseek() to do so. */
    if (ig->seek) {
        tt = t * b;
        nb = tt / d + ig->start;
        if ((i = ig->bsize) == 0)
            i = ibsize;
        /* Seek to a position such that the next block read will contain the
           desired sample. */
        tt = nb / i;
        if (wfdb_fseek(ig->fp, tt * i, 0)) {
            std::cout << "isigsettime: improper seek on signal group %d\n" << g << std::endl;
            return (-1);
        }
        nb %= i;
    }
        /* For special files, rewind if necessary and then read ahead. */
    else {
        WFDB_Time t0, t1;

        /* Get the time of the earliest buffered sample ... */
        t0 = istime - (ig->bp - ig->buf) / b;
        /* ... and that of the earliest unread sample. */
        t1 = t0 + (ig->be - ig->buf) / b;
        /* There are three possibilities:  either the desired sample has been
           read and has passed out of the buffer, requiring a rewind ... */
        if (t < t0) {
            if (wfdb_fseek(ig->fp, 0L, 0)) {
                std::cout << "isigsettime: improper seek on signal group %d\n" << g << std::endl;
                return (-1);
            }
            tt = t * b;
            nb = tt / d + ig->start;
        }
            /* ... or it is in the buffer already ... */
        else if (t < t1) {
            tt = (t - t0) * b;
            ig->bp = ig->buf + tt / d;
            return (0);
        }
            /* ... or it has not yet been read. */
        else {
            tt = (t - t1) * b;
            nb = tt / d;
        }
        while (nb > ig->bsize && !wfdb_feof(ig->fp))
            nb -= wfdb_fread(ig->buf, 1, ig->bsize, ig->fp);
    }

    /* Reset the block pointer to indicate nothing has been read in the
       current block. */
    ig->bp = ig->be;
    ig->stat = 1;
    /* Read any bytes in the current block that precede the desired sample. */
    while (nb-- > 0 && ig->stat > 0)
        i = r8(ig);
    if (ig->stat <= 0)
        return (-1);

    /* Reset the getvec sample-within-frame counter. */
    gvc = ispfmax;

    /* Reset the time (if signal 0 belongs to the group) and disable checksum
       testing (by setting the number of samples remaining to 0). */
    if (s == 0)
        istime = in_msrec ? t + segp->samp0 : t;
    while (n-- != 0)
        isd[s + n]->info.nsamp = (WFDB_Time) 0L;
    return (0);
}

static int r61(struct igdata *g) {
    int l, h;

    h = r8(g);
    l = r8(g);
    return ((int) ((short) ((h << 8) | (l & 0xff))));
}

/* r24: read and return the next sample from a format 24 signal file */
static int r24(struct igdata *g) {
    int l, h;

    l = r16(g);
    h = r8(g);
    return ((int) ((h << 16) | (l & 0xffff)));
}

/* r32: read and return the next sample from a format 32 signal file */
static int r32(struct igdata *g) {
    int l, h;

    l = r16(g);
    h = r16(g);
    return ((int) ((h << 16) | (l & 0xffff)));
}

#define r80(G) ((r8(G) & 0xff) - (1 << 7))
#define r160(G) ((r16(G) & 0xffff) - (1 << 15))

static int flac_getsamp(struct igdata *ig) {
    ig->stat = -1;
    return (0);
}

static int getskewedframe(WFDB_Sample *vector) {
    int c, stat;
    struct isdata *is;
    struct igdata *ig;
    WFDB_Group g;
    WFDB_Sample v, *vecstart = vector;
    WFDB_Signal s;

    if ((stat = (int) nisig) == 0)
        return (nvsig > 0 ? -1 : 0);
    if (istime == 0L) {
        for (s = 0; s < nisig; s++)
            isd[s]->samp = isd[s]->info.initval;
    }
    for (g = nigroup; g;) {
        /* Go through groups in reverse order since seeking on group 0
           should always be done last. */
        if (igd[--g]->initial_skip)
            isgsetframe(g, (in_msrec ? segp->samp0 : 0));
    }

    /* If the vector needs to be rearranged (variable-layout record),
       then read samples into a temporary buffer. */
    if (need_sigmap)
        vector = ovec;

    for (s = 0; s < nisig; s++) {
        is = isd[s];
        ig = igd[is->info.group];
        for (c = 0; c < is->info.spf; c++, vector++) {
            switch (is->info.fmt) {
                case 0: /* null signal: return sample tagged as invalid */
                    *vector = v = VFILL;
                    if (is->info.nsamp == 0)
                        ig->stat = -1;
                    break;
                case 8: /* 8-bit first differences */
                default:
                    *vector = v = is->samp += r8(ig);
                    break;
                case 16: /* 16-bit amplitudes */
                    *vector = v = r16(ig);
                    if (v == -1 << 15)
                        *vector = VFILL;
                    else
                        is->samp = *vector;
                    break;
                case 61: /* 16-bit amplitudes, bytes swapped */
                    *vector = v = r61(ig);
                    if (v == -1 << 15)
                        *vector = VFILL;
                    else
                        is->samp = *vector;
                    break;
                case 80: /* 8-bit offset binary amplitudes */
                    *vector = v = r80(ig);
                    if (v == -1 << 7)
                        *vector = VFILL;
                    else
                        is->samp = *vector;
                    break;
                case 160: /* 16-bit offset binary amplitudes */
                    *vector = v = r160(ig);
                    if (v == -1 << 15)
                        *vector = VFILL;
                    else
                        is->samp = *vector;
                    break;
                case 212: /* 2 12-bit amplitudes bit-packed in 3 bytes */
                    *vector = v = r212(ig);
                    if (v == -1 << 11)
                        *vector = VFILL;
                    else
                        is->samp = *vector;
                    break;
                case 310: /* 3 10-bit amplitudes bit-packed in 4 bytes */
                    *vector = v = r310(ig);
                    if (v == -1 << 9)
                        *vector = VFILL;
                    else
                        is->samp = *vector;
                    break;
                case 311: /* 3 10-bit amplitudes bit-packed in 4 bytes */
                    *vector = v = r311(ig);
                    if (v == -1 << 9)
                        *vector = VFILL;
                    else
                        is->samp = *vector;
                    break;
                case 24: /* 24-bit amplitudes */
                    *vector = v = r24(ig);
                    if (v == -1 << 23)
                        *vector = VFILL;
                    else
                        is->samp = *vector;
                    break;
                case 32: /* 32-bit amplitudes */
                    *vector = v = r32(ig);
                    if (v == -1 << 31)
                        *vector = VFILL;
                    else
                        is->samp = *vector;
                    break;
                case 508: /* 8-bit compressed FLAC */
                    *vector = v = flac_getsamp(ig);
                    if (v == -1 << 7)
                        *vector = VFILL;
                    else
                        is->samp = *vector;
                    break;
                case 516: /* 16-bit compressed FLAC */
                    *vector = v = flac_getsamp(ig);
                    if (v == -1 << 15)
                        *vector = VFILL;
                    else
                        is->samp = *vector;
                    break;
                case 524: /* 24-bit compressed FLAC */
                    *vector = v = flac_getsamp(ig);
                    if (v == -1 << 23)
                        *vector = VFILL;
                    else
                        is->samp = *vector;
                    break;
            }
            if (ig->stat <= 0) {
                /* End of file -- reset input counter. */
                ig->count = 0;
                if (ig->stat == -2) {
                    /* error in decoding compressed data */
                    stat = -3;
                } else if (is->info.nsamp > (WFDB_Time) 0L) {
                    std::cout << "getvec: unexpected EOF in signal %d\n" << s << std::endl;
                    stat = -3;
                } else if (in_msrec && segp && segp < segend) {
                    std::cout << "isigopen nedd to be checked here \n" << std::endl;
//                    segp++;
//                    if (isigopen(segp->recname, nullptr, (int)nvsig) <= 0)
//                    {
//                        wfdb_error("getvec: error opening segment %s\n",
//                                   segp->recname);
//                        stat = -3;
//                        return (stat); /* avoid looping if segment is bad */
//                    }
//                    else
//                    {
//                        istime = segp->samp0;
//                        return (getskewedframe(vecstart));
//                    }
                } else
                    stat = -1;
            }
            is->info.cksum -= v;
        }
        if (is->info.nsamp >= 0 && --is->info.nsamp == 0 &&
            (is->info.cksum & 0xffff) &&
            !in_msrec && !isedf &&
            is->info.fmt != 0) {
            std::cout << "getvec: checksum error in signal %d\n" << s << std::endl;
            stat = -4;
        }
    }

    if (need_sigmap) {
//        sigmap(vecstart, ovec);
        std::cout << "sigmap(vecstart, ovec) needs to be checked" << std::endl;
    } else if (framelen != tspf)
        for (s = framelen; s < tspf; s++)
            vecstart[s] = WFDB_INVALID_SAMPLE;

    return (stat);
}

FINT getframe(WFDB_Sample *vector) {
    int stat = -1;

    if (dsbuf) { /* signals must be deskewed */
        int c, i, j, s;

        /* First, obtain the samples needed. */
        if (dsbi < 0) { /* dsbuf contents are invalid -- refill dsbuf */
            for (dsbi = i = 0; i < dsblen; dsbi = i += tspf) {
                stat = getskewedframe(dsbuf + dsbi);
                if (stat < 0)
                    break;
            }
            dsbi = 0;
        } else { /* replace oldest frame in dsbuf only */
            stat = getskewedframe(dsbuf + dsbi);
            if ((dsbi += tspf) >= dsblen)
                dsbi = 0;
        }

        /* Assemble the deskewed frame from the data in dsbuf. */
        for (j = s = 0; s < nvsig; s++) {
            if ((i = j + dsbi + vsd[s]->skew * tspf) >= dsblen)
                i %= dsblen;
            for (c = 0; c < vsd[s]->info.spf; c++)
                vector[j++] = dsbuf[i++];
        }
    } else /* no deskewing necessary */
        stat = getskewedframe(vector);
    istime++;
    return (stat);
}

//
static int rgetvec(WFDB_Sample *vector) {
    WFDB_Sample *tp;
    WFDB_Signal s;
    static int stat;

    if (ispfmax < 2) /* all signals at the same frequency */
        return (getframe(vector));

    if ((gvmode & WFDB_HIGHRES) != WFDB_HIGHRES) {
        /* return one sample per frame, decimating by averaging if necessary */
        stat = getframe(tvector);
        for (s = 0, tp = tvector; s < nvsig; s++) {
            int sf = vsd[s]->info.spf;
            std::cout << "*vector++ = meansamp(tp, sf); needs to be checke" << std::endl;
//            *vector++ = meansamp(tp, sf);
            tp += sf;
        }
    } else { /* return ispfmax samples per frame, using
	 zero-order interpolation if necessary */
        if (gvc >= ispfmax) {
            stat = getframe(tvector);
            gvc = 0;
        }
        for (s = 0, tp = tvector; s < nvsig; s++) {
            int sf = vsd[s]->info.spf;
            if (gvc == 0) {
                vsd[s]->gvindex = 0;
                vsd[s]->gvcount = -ispfmax;
            } else {
                vsd[s]->gvcount += sf;
                if (vsd[s]->gvcount >= 0) {
                    vsd[s]->gvindex++;
                    vsd[s]->gvcount -= ispfmax;
                }
            }
            *vector++ = tp[vsd[s]->gvindex];
            tp += sf;
        }
        gvc++;
    }
    return (stat);
}

FINT getvec(WFDB_Sample *vector) {
    int i, nsig;

    if (ifreq == 0.0 || ifreq == sfreq) /* no resampling necessary */
        return (rgetvec(vector));

    return 0;
    /* Resample the input. */
//    if (rgvtime > mnticks)
//    {
//        rgvtime -= mnticks;
//        gvtime -= mnticks;
//    }
//    nsig = (nvsig > nisig) ? nvsig : nisig;
//    while (gvtime > rgvtime)
//    {
//        for (i = 0; i < nsig; i++)
//            gv0[i] = gv1[i];
//        rgvstat = rgetvec(gv1);
//        rgvtime += nticks;
//    }
//    for (i = 0; i < nsig; i++)
//    {
//        vector[i] = gv0[i] + (gvtime % nticks) * (gv1[i] - gv0[i]) / nticks;
//        gv0[i] = gv1[i];
//    }
//    gvtime += mticks;
//    return (rgvstat);
}
