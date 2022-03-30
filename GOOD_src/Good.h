/*------------------------------------------------------------------------------
* Good.h : constants, types and function prototypes
*
* Copyright (C) 2020-2099 by SpAtial SurveyIng and Navigation (SASIN) group, all rights reserved.
*    This file is part of GAMP II - GOOD (Gnss Observations and prOducts Downloader) toolkit
*
* history : 2020/08/16 1.0  new (by Feng Zhou)
*-----------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#ifdef _WIN32  /* for Windows */
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <direct.h>
#include "win\unistd.h"
#else          /* for Linux or Mac */
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#endif


/* load namespace */
using namespace std;

/* constant definitions ----------------------------------------------------------*/
#define MAXCHARS        1024      /* maximum characters in one line */
#define MAXSTRPATH      1024      /* max length of stream path */

#ifdef _WIN32  /* for Windows */
#define FILEPATHSEP '\\'
#define strcasecmp  _stricmp
#define access      _access
#else          /* for Linux or Mac */
#define FILEPATHSEP '/'
#endif

/* type definitions ----------------------------------------------------------*/
struct gtime_t
{                                 /* time struct */
    int mjd;                      /* modified Julian date (MJD) */
    double sod;                   /* seconds of the day */
};

struct ftpopt_t
{                                 /* the type of GNSS data downloading */
    bool ftpDownloading;          /* the master switch for data downloading, 0:off  1:on, only for data downloading */
    char ftpFrom[MAXCHARS];       /* FTP archive: CDDIS, IGN, or WHU */
    bool getObs;                  /* (0:off  1:on) GNSS observation data downloading option */
    char obsTyp[MAXCHARS];        /* "daily", "hourly", "highrate", "30s", "5s", or "1s" */
    char obsFrom[MAXCHARS];       /* where to download the observations (i.e., "igs", "mgex", "igm", "cut", "ga", "hk", "ngs", or "epn") */
    char obsLst[MAXCHARS];        /* 'all'; the full path of 'site.list' */
    std::vector<int> hhObs;       /* hours array (0: 00:00, 1: 1:00, 2: 2:00, 3: 3:00, ...) */
    bool getNav;                  /* (0:off  1:on) various broadcast ephemeris */
    char navTyp[MAXCHARS];        /* 'daily' or 'hourly' */
    char navSys[MAXCHARS];        /* 'gps', 'glo', 'bds', 'gal', 'qzs', 'irn', 'mixed' or 'all' */
    char navAc[MAXCHARS];         /* analysis center (i.e., "igs", "dlr", "ign", "gop", or "wrd") that carries out the combination of broadcast 
                                     ephemeris for mixed navigation data */
    char navLst[MAXCHARS];        /* the full path of 'site.list' */
    std::vector<int> hhNav;       /* hours array (0: 00:00, 1: 1:00, 2: 2:00, 3: 3:00, ...) */
    bool getOrbClk;               /* (0:off  1:on) precise orbit 'sp3' and precise clock 'clk' */
    char orbClkAc[MAXCHARS];      /* analysis center (i.e., final: "cod", "emr", "esa", "gfz", "grg", "igs", "jpl", "mit", "cod_m", "gfz_m", 
                                     "grg_m", "whu_m"; rapid: "cod_r", "emr_r", "esa_r", "gfz_r", "igs_r"; ultra-rapid: "esa_u", "gfz_u", 
                                     "igs_u", "whu_u"; real-time: "cnt"). NOTE: the option of "cnt" is for real-time precise orbit and 
                                     clock products from CNES offline files */
    std::vector<std::vector<int> > hhOrbClk;  /* hours array (0: 00:00, 6: 6:00, 12: 12:00, 18: 18:00 for esa_u and/or igs_u; 0: 00:00, 3: 3:00,
                                                 6: 6:00, ... for gfz_u; 0: 00:00, 1: 1:00, 2: 2:00, ... for whu_u) */
    bool getObx;                  /* (0:off  1:on) ORBEX (ORBit EXchange format) for satellite attitude information */
    char obxAc[MAXCHARS];         /* analysis center (i.e., final/rapid: "cod_m", "gfz_m", "grg_m", "whu_m"; real-time: cnt). NOTE: the
                                     option of "cnt" is for real-time ORBEX from CNES offline files */
    bool getEop;                  /* (0:off  1:on) earth rotation parameter */
    char eopAc[MAXCHARS];         /* analysis center (i.e., final: "cod", "emr", "esa", "gfz", "grg", "igs", "jpl", "mit"; 
                                     ultra: "esa_u", "gfz_u", "igs_u") */
    std::vector<int> hhEop;       /* hours array (0: 00:00, 6: 6:00, 12: 12:00, 18: 18:00 for esa_u and/or igs_u; 0: 00:00, 3: 3:00, 
                                     6: 6:00, ... for gfz_u) */
    bool getSnx;                  /* (0:off  1:on) IGS weekly SINEX */
    bool getDsb;                  /* (0:off  1:on) differential code/signal bias (DCB/DSB) */
    char dsbAc[MAXCHARS];         /* analysis center (i.e., "cod", "cas") */
    bool getOsb;                  /* (0:off  1:on) observable-specific signal bias (OSB) */
    char osbAc[MAXCHARS];         /* analysis center (i.e., final/rapid: "cod_m", "gfz_m", "grg_m", "whu_m"; real-time: cnt). NOTE: the 
                                     option of "cnt" is for real-time OSBs from CNES offline files */
    bool getIon;                  /* (0:off  1:on) global ionosphere map (GIM) */
    char ionAc[MAXCHARS];         /* analysis center (i.e., "igs", "cod", "cas", ...) */
    bool getRoti;                 /* (0:off  1:on) rate of TEC index (ROTI) */
    bool getTrp;                  /* (0:off  1:on) tropospheric product */
    char trpAc[MAXCHARS];         /* analysis center (i.e., "igs" or "cod") */
    char trpLst[MAXCHARS];        /* 'all'; the full path of 'site.list' */
    bool getAtx;                  /* (0:off  1:on) ANTEX format antenna phase center correction */
    char dir3party[MAXSTRPATH];   /* (optional) the directory where third-party softwares (i.e., 'wget', 'gzip', 'crx2rnx' etc) are stored. 
                                     This option is not needed if you have set the path or environment variable for them */
    bool isPath3party;            /* if true: the path need be set for third-party softwares */
    char wgetFull[MAXCHARS];      /* if isPath3party == true, set the full path where 'wget' is */
    char gzipFull[MAXCHARS];      /* if isPath3party == true, set the full path where 'gzip' is */
    char crx2rnxFull[MAXCHARS];   /* if isPath3party == true, set the full path where 'crx2rnx' is */
    bool minusAdd1day;            /* (0:off  1:on) the day before and after the current day for precise satellite orbit and 
                                     clock products downloading */
    bool printInfoWget;           /* (0:off  1 : on) print the information generated by 'wget' */
    char qr[MAXCHARS];            /* "-qr" (printInfoWget == false) or "r" (printInfoWget == true) in 'wget' command line */

    char logFil[MAXSTRPATH];      /* The log file with full path that gives the indications of whether the data downloading is
                                     successful or not */
    int logWriteMode;             /* 0: off  1: overwrite mode  2: append mode */
    FILE *fpLog;                  /* output file pointer */
};

struct prcopt_t
{                                 /* processing options type */
    /* processing directory */
    char mainDir[MAXSTRPATH];     /* the root/main directory of GNSS observations and products */
                                  /* the settings below are the sub-directories, and two parameters are needed
                                     1st: can be set to 0 or 1, 0: use the path of root/main directory; 1: NOT use the path of 
                                     root/main directory
                                     If '0' is set, the full path will NOT be needed, i.e., 'obxDir         = 0  obx', 
                                     indicating 'obxDir         = D:\data\obx'
                                     If '1' is set, the full path should be given, i.e., 'obxDir         = 1  D:\data\obx'
                                     2nd: the path of sub-directory, and absolute (1st: 1) or relative (1st: 0) path need to be set */
    char obsDir[MAXSTRPATH];      /* the sub-directory of RINEX format observation files */
    char navDir[MAXSTRPATH];      /* the sub-directory of RINEX format broadcast ephemeris files */
    char orbDir[MAXSTRPATH];      /* the sub-directory of SP3 format precise ephemeris files */
    char clkDir[MAXSTRPATH];      /* the sub-directory of RINEX format precise clock files */
    char eopDir[MAXSTRPATH];      /* the sub-directory of earth rotation/orientation parameter (EOP) files */
    char obxDir[MAXSTRPATH];      /* the sub-directory of MGEX final/rapid and/or CNES real-time ORBEX (ORBit EXchange format) files */
    char snxDir[MAXSTRPATH];      /* the sub-directory of SINEX format IGS weekly solution files */
    char biaDir[MAXSTRPATH];      /* the sub-directory of CODE/MGEX differential code/signal bias (DCB/DSB), MGEX observable-specific 
                                     signal bias (OSB), and/or CNES real-time OSB files */
    char ionDir[MAXSTRPATH];      /* the sub-directory of CODE/IGS global ionosphere map (GIM) files */
    char ztdDir[MAXSTRPATH];      /* the sub-directory of CODE/IGS tropospheric product files */
    char tblDir[MAXSTRPATH];      /* the sub-directory of table files (i.e., ANTEX, ocean tide loading files, etc.) for processing */

    /* time settings */
    gtime_t ts;                   /* start time for processing */
    int ndays;                    /* number of consecutive days */
};