/*------------------------------------------------------------------------------
* FtpUtil.cpp : GNSS data downloading module
*
* Copyright (C) 2020-2099 by SpAtial SurveyIng and Navigation (SASIN) group, all rights reserved.
*    This file is part of GAMP II - GOOD (Gnss Observations and prOducts Downloader) toolkit
*
* References:
*    Dr Cheng Wang @ BUAA provided the C++ code for observation downloading as reference
*
* history : 2021/04/16 1.0  new (by Feng Zhou @ SDUST)
*           2021/04/20      MGEX multi-GNSS precise products option added (by Feng Zhou @ SDUST)
*           2021/04/23 1.1  IGN and WHU FPT archives added (by Feng Zhou @ SDUST)
*           2021/04/30      the day before and after the current day for precise satellite orbit and clock products 
*                             downloading is added (by Feng Zhou @ SDUST and Zan Liu @ CUMT)
*           2021/04/30 1.2  CNES real-time orbit, clock, bias files (in offline mode) option added (by Feng Zhou @ SDUST)
*           2021/05/01      from DOY 345/2020, 'brdcDDD0.YYn' was converted from '*.Z' to '*.gz' (by Feng Zhou @ SDUST and 
*                             Zan Liu @ CUMT)
*           2021/05/01      added the full path of third-party softwares (by Feng Zhou @ SDUST and Yudan Yi)
*           2021/05/05 1.3  add 'GetRoti' for Rate of TEC index (ROTI) file downloading (by Feng Zhou @ SDUST and Lei Liu @ WHU)
*           2021/05/06 1.4  add the compatibility for changing directory (chdir) in Windows and Linux OS (by Feng Zhou @ SDUST)
*           2021/05/07      add the option 'printInfoWget' in configure file for (not) printing the information of running 
*                             'wget' (by Feng Zhou @ SDUST and Hong Hu @ AHU)
*           2021/05/08      add IGS and MGEX hourly observation (30s) downloading (by Feng Zhou @ SDUST and Menghao Li @ HEU)
*           2021/05/10 1.5  modify some codes to make GOOD adaptable to different compression form (i.e., '*.Z' or '*.gz') 
*                             (by Feng Zhou @ SDUST)
*           2021/05/12      add the option for IGR, IGU, GFU (from GFZ), and WUU (from WHU) products as well as the site-specific 
*                             broadcast ephemeris files downloading (by Feng Zhou @ SDUST and Menghao Li @ HEU)
*           2021/05/15      sub-directory (i.e., daily, hourly, and highrate) creation for observation downloading (by Feng Zhou @ SDUST and 
*                             Menghao Li @ HEU)
*           2021/05/18 1.6  modify some codes to make configuration file and program look more concise (by Feng Zhou @ SDUST and 
*                             Menghao Li @ HEU)
*           2021/05/21      add the extraction and conversion for the 'all' option in IGS and MGEX observation downloading (by Feng Zhou @ SDUST)
*           2021/06/01 1.7  add 'getObc', 'getObg', and 'getObh' options for Curtin University of Technology (CUT), Geoscience Australia (GA), 
*                             and Hong Kong CORS observation downloading (by Feng Zhou @ SDUST)
*           2021/07/16      modify "*_R_" to "*" for CRX file naming convention (by Feng Zhou @ SDUST)
*           2021/09/01 1.8  add various types of ultra-rapid (i.e., ESA, GFZ, IGS, WHU) and rapid (i.e., CODE, NRCan, ESA, GFZ, IGS) 
*                             orbit and clock products for downloading (by Feng Zhou @ SDUST and Sermet Ogutcu @ NEU)
*           2021/09/04      add the option for NGS/NOAA CORS (from Amazon Web Services (AWS)) and EUREF Permanent Network (EPN) 
*                             observation downloading (by Feng Zhou @ SDUST)
*           2021/09/05      fix the bug of broadcast ephemeris downloading via WHU FTP before and after 2020 (by Feng Zhou @ SDUST and 
*                             Zan Liu @ CUMT)
*           2021/09/06      modify the setting of directories of GNSS observations and products in configure file to make them 
*                             more elastic (by Feng Zhou @ SDUST and Caoming Fan @ SDU)
*           2021/12/01 1.9  delete the function of "GetDcbCode", "GetDcbMgex", "GetRtOrbClkCNT", and "GetRtBiasCNT" (by Feng Zhou @ SDUST)
*           2021/12/03      add the option for CNES real-time and MGEX final ORBEX (ORBit EXchange format) for satllite attitude information 
*                             from MGEX (by Feng Zhou @ SDUST and Zan Liu @ CUMT)
*           2021/12/05      add the option for CNES real-time and MGEX final observable-specific code/phase signal bias (OSB) 
*                             (by Feng Zhou @ SDUST)
*           2021/12/10      add the option for various mixed broadcast ephemeris files downloading (i.e., "igs", "dlr", "ign", "gop", or 
*                             "wrd") (by Feng Zhou @ SDUST and Jiahuan Hu @ YorkU)
*           2021/12/19      make many optimizations in function of "GetOrbClk", "GetEop", "GetObx", "GetDsb", and "GetOsb" (by Feng Zhou @ SDUST)
*           2021/12/20      support high-frequency GNSS observation data (i.e., 1s) downloading from WHU FTP (by Feng Zhou @ SDUST)
*           2021/12/20      fix the bug for hourly broadcast ephemeris file downloading from WHU FTP (by Feng Zhou @ SDUST and Menghao Li @ HEU)
*           2021/12/21      add the corresponding codes for the comment/header component in 'site.list' (by Feng Zhou @ SDUST and 
*                             Biquan Cheng @ TJU)
*           2021/12/23      detele the option of "rtnav" for "getNav" since the FTP address "ftp://ftp.lrz.de/transfer/steigenb/brdm" is 
*                             now invalid (by Feng Zhou @ SDUST)
*           2021/12/24      add the downloading option for various rapid and predicted global ionosphere map (by Feng Zhou @ SDUST)
*           2021/12/24      integrate all of the observation downloading options (i.e., "getObm", "getObc", ...) into "getObs" (by Feng Zhou @ SDUST)
*           2021/12/25      add the option "igm" of "getObs" for taking the union of IGS and MGEX (IGS + MGEX, while the priority of 
*                             MGEX sites is higher) observation downloading with respect to the site name (by Feng Zhou @ SDUST and Menghao Li @ HEU)
*           2021/12/25      add the function "GetDailyObsIgm", "GetHourlyObsIgm", and "GetHrObsIgm" for daily, hourly, and high-rate IGS and MGEX
*                             observation downloading (taking the union of them) (by Feng Zhou @ SDUST)
*           2021/12/29      add the function "GetDailyObsPbo2", "GetDailyObsPbo3", and "GetDailyObsPbo5" for daily Plate Boundary Observatory (PBO)
*                             observation downloading ("pbo2" is for RINEX version 2.xx, short name "d"; "pbo3" is for RINEX version 3.xx, 
*                             long name "crx"; "pbo5" is for pbo2 + pbo3, while the priority of pbo3 sites is higher) (by Feng Zhou @ SDUST, 
*                             Yuze Yang  @ SDUST, and Zan Liu @ CUMT)
*           2022/01/01 2.0  add the option "mixed3" and "mixed4" for RINEX 3.xx and 4.xx broadcast ephemeris file downloading (by Feng Zhou @ SDUST and Zan Liu @ CUMT)
*           2022/01/05      add the option for generating log file to give information about whether the downloading was successful (by Feng Zhou @ SDUST and Zan Liu @ CUMT)
*           2022/01/07      add the option for downloading prodcuts (i.e., "GetOrbClk" and "GetIono") from multiple analysis centers, i.e., 
*                             "cod+igs", "cod+igs+jpl" etc (by Feng Zhou @ SDUST)
*           2022/01/10      make a deeper optimization for "GetOrbClk" to make its codes more concise (by Feng Zhou @ SDUST)
*           2022/01/15      adopt 2D vector to adapt to a variety of ultra-rapid ephemeris downloading, i.e., "cod+igs+igs_r+igs_u+whu_u+cnt" (by Feng Zhou @ SDUST)
*           2022/01/16      add some codes to adapt to the orbit and clock products with "all" and/or "all_m" option(s), i.e., 
*                             "cod+igs+igs_r+igs_u+whu_u+cnt+all+all_m" (by Feng Zhou @ SDUST)
*           2022/01/17      perfect the output information of the log file to more intuitively understand the name of the downloading files (by Feng Zhou @ SDUST)
*           2022/02/19      change "BRDX*" to "BRD4" for "mixed4" (RINEX 4.xx) broadcast ephemeris file downloading (by Feng Zhou @ SDUST and Zan Liu @ CUMT)
*           2022/03/19      move the codes for log file writing from "FtpUtil.cpp" to "PreProcess.cpp" (by Feng Zhou @ SDUST)
*-----------------------------------------------------------------------------*/
#include "Good.h"
#include "TimeUtil.h"
#include "StringUtil.h"
#include "FtpUtil.h"


/* constants/macros ----------------------------------------------------------*/
#define IDX_OBSD   0    /* index for IGS daily observation (30s) file downloaded (short name "d" files) */
#define IDX_OBSH   1    /* index for IGS hourly observation (30s) file downloaded (short name "d" files) */
#define IDX_OBSHR  2    /* index for IGS high-rate observation (1s) file downloaded (short name "d" files) */
#define IDX_OBMD   3    /* index for MGEX daily observation (30s) file downloaded (long name "crx" files) */
#define IDX_OBMH   4    /* index for MGEX daily observation (30s) file downloaded (long name "crx" files) */
#define IDX_OBMHR  5    /* index for MGEX high-rate observation (1s) file downloaded (long name "crx" files) */
#define IDX_NAV    6    /* index for broadcast ephemeris file downloaded */
#define IDX_SP3    7    /* index for IGS SP3 file downloaded */
#define IDX_CLK    8    /* index for IGS CLK file downloaded */
#define IDX_EOP    9    /* index for IGS EOP file downloaded */
#define IDX_SNX    10   /* index for IGS weekly SINEX file downloaded */
#define IDX_SP3M   11   /* index for MGEX SP3 file downloaded */
#define IDX_CLKM   12   /* index for MGEX CLK file downloaded */
#define IDX_OBXM   13   /* index for MGEX ORBEX file downloaded */
#define IDX_DSBM   14   /* index for MGEX DSB file downloaded */
#define IDX_OSBM   15   /* index for MGEX OSB file downloaded */
#define IDX_ION    16   /* index for global ionosphere map (GIM) downloaded */
#define IDX_ROTI   17   /* index for Rate of TEC index (ROTI) downloaded */
#define IDX_ZTD    18   /* index for IGS final tropospheric product downloaded */

#define PROD_REALTIME    1   /* index for real-time orbit and clock products downloading */
#define PROD_ULTRA_RAPID 2   /* index for ultra-rapid orbit and clock products downloading */
#define PROD_RAPID       3   /* index for rapid orbit and clock products downloading */
#define PROD_FINAL_IGS   4   /* index for IGS final orbit and clock products downloading */
#define PROD_FINAL_MGEX  5   /* index for MGEX final orbit and clock products downloading */


/* function definition -------------------------------------------------------*/

/**
* @brief   : init - Get FTP archive for CDDIS, IGN, or WHU
* @param[I]: none
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::init()
{
    /* FTP archive for CDDIS */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/daily");                /* IGS daily observation (30s) files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/hourly");               /* IGS hourly observation (30s) files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/highrate");             /* IGS high-rate observation (1s) files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/daily");                /* MGEX daily observation (30s) files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/hourly");               /* MGEX hourly observation (30s) files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/highrate");             /* MGEX high-rate observation (1s) files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/daily");                /* broadcast ephemeris files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products");                  /* IGS SP3 files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products");                  /* IGS CLK files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products");                  /* IGS EOP files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products");                  /* IGS weekly SINEX files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/mgex");             /* MGEX SP3 files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/mgex");             /* MGEX CLK files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/mgex");             /* MGEX ORBEX files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/bias");             /* MGEX DSB files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/mgex");             /* MGEX OSB files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/ionex");            /* global ionosphere map (GIM) files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/ionex");            /* Rate of TEC index (ROTI) files */
    _ftpArchive.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/troposphere/zpd");  /* IGS final tropospheric product files */

    /* FTP archive for IGN */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/data");                                         /* IGS daily observation (30s) files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/data/hourly");                                  /* IGS hourly observation (30s) files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/data/highrate");                                /* IGS high-rate observation (1s) files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/data");                                         /* MGEX daily observation (30s) files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/data/hourly");                                  /* MGEX hourly observation (30s) files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/data/highrate");                                /* MGEX high-rate observation (1s) files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/data");                                         /* broadcast ephemeris files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/products");                                     /* IGS SP3 files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/products");                                     /* IGS CLK files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/products");                                     /* IGS EOP files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/products");                                     /* IGS weekly SINEX files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/mgex");                                /* MGEX SP3 files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/mgex");                                /* MGEX CLK files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/mgex");                                /* MGEX ORBEX files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/mgex/dcb");                            /* MGEX DSB files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/mgex");                                /* MGEX OSB files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/ionosphere");                          /* global ionosphere map (GIM) files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/ionosphere");                          /* Rate of TEC index (ROTI) files */
    _ftpArchive.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/troposphere");                         /* IGS final tropospheric product files */

    /* FTP archive for WHU */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/data/daily");                               /* IGS daily observation (30s) files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/data/hourly");                              /* IGS hourly observation (30s) files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/highrate");                                     /* IGS high-rate observation (1s) files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/data/daily");                               /* MGEX daily observation (30s) files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/data/hourly");                              /* MGEX hourly observation (30s) files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/highrate");                                     /* MGEX high-rate observation (1s) files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/data/daily");                               /* broadcast ephemeris files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products");                                 /* IGS SP3 files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products");                                 /* IGS CLK files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products");                                 /* IGS EOP files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products");                                 /* IGS weekly SINEX files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/mgex");                            /* MGEX SP3 files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/mgex");                            /* MGEX CLK files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/mgex");                            /* MGEX ORBEX files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/mgex/dcb");                        /* MGEX DSB files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/mgex");                            /* MGEX OSB files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/ionex");                           /* global ionosphere map (GIM) files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/ionex");                           /* Rate of TEC index (ROTI) files */
    _ftpArchive.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/troposphere/new");                 /* IGS final tropospheric product files */
} /* end of init */

/**
* @brief   : GetDailyObsIgs - download IGS RINEX daily observation (30s) files (short name "d") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsIgs(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "daily");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    /* FTP archive: CDDIS, IGN, or WHU */
    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the IGS observation files */
        string url, cutDirs = " --cut-dirs=7 ";
        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBSD] + "/" +
            sYyyy + "/" + sDoy + "/" + sYy + "d";
        else if (ftpName == "IGN")
        {
            url = _ftpArchive.IGN[IDX_OBSD] + "/" + sYyyy + "/" + sDoy;
            cutDirs = " --cut-dirs=5 ";
        }
        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBSD] + "/" +
            sYyyy + "/" + sDoy + "/" + sYy + "d";
        else url = _ftpArchive.CDDIS[IDX_OBSD] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d";

        /* it is OK for '*.Z' or '*.gz' format */
        string dFile = "*" + sDoy + "0." + sYy + "d";
        string dxFile = dFile + ".*";
        string cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        string suffix = "." + sYy + "d";
        vector<string> dFiles;
        str.GetFilesAll(subDir, suffix, dFiles);
        char sitName[MAXCHARS];
        for (int i = 0; i < dFiles.size(); i++)
        {
            if (access(dFiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipFull + " -d -f " + dFiles[i];
                std::system(cmd.c_str());

                str.StrMid(sitName, dFiles[i].c_str(), 0, 4);
                string site = sitName;
                str.ToLower(site);
                dFile = site + sDoy + "0." + sYy + "d";
                if (access(dFile.c_str(), 0) == -1) continue;
                string oFile = site + sDoy + "0." + sYy + "o";
                /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + dFile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + dFile;
#endif
                std::system(cmd.c_str());
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the IGS observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetDailyObsIgs): open site.list = " << fopt->obsLst << 
                    " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                str.ToLower(sitName);
                string oFile = sitName + sDoy + "0." + sYy + "o";
                string dFile = sitName + sDoy + "0." + sYy + "d";
                if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                {
                    string url, cutDirs = " --cut-dirs=7 ";
                    if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBSD] + "/" +
                        sYyyy + "/" + sDoy + "/" + sYy + "d";
                    else if (ftpName == "IGN")
                    {
                        url = _ftpArchive.IGN[IDX_OBSD] + "/" + sYyyy + "/" + sDoy;
                        cutDirs = " --cut-dirs=5 ";
                    }
                    else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBSD] + "/" +
                        sYyyy + "/" + sDoy + "/" + sYy + "d";
                    else url = _ftpArchive.CDDIS[IDX_OBSD] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d";

                    /* it is OK for '*.Z' or '*.gz' format */
                    string dxFile = dFile + ".*";
                    string cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
                    std::system(cmd.c_str());

                    string dgzFile = dFile + ".gz", dzFile = dFile + ".Z";
                    bool isgz = false;
                    if (access(dgzFile.c_str(), 0) == 0)
                    {
                        /* extract '*.gz' */
                        cmd = gzipFull + " -d -f " + dgzFile;
                        std::system(cmd.c_str());
                        isgz = true;

                        if (access(dFile.c_str(), 0) == 0 && access(dzFile.c_str(), 0) == 0)
                        {
                            /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + dzFile;
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + dzFile;
#endif
                            std::system(cmd.c_str());
                        }
                    }
                    if (access(dFile.c_str(), 0) == -1 && access(dzFile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipFull + " -d -f " + dzFile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, oFile.c_str());
                    string localFile = tmpFile;
                    if (access(dFile.c_str(), 0) == -1)
                    {
                        cout << "*** WARNING(FtpUtil::GetDailyObsIgs): failed to download IGS daily observation file " << oFile << endl;

                        string url0 = url + '/' + dxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsIgs): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                        continue;
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    if (access(oFile.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::GetDailyObsIgs): successfully download IGS daily observation file " << oFile << endl;

                        if (isgz) dxFile = dgzFile;
                        else dxFile = dzFile;
                        string url0 = url + '/' + dxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDailyObsIgs): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::GetDailyObsIgs): IGS daily observation file " << oFile << 
                    " or " << dFile << " has existed!" << endl;
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetDailyObsIgs */

/**
* @brief   : GetHourlyObsIgs - download IGS RINEX hourly observation (30s) files (short name "d") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHourlyObsIgs(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('hourly') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "hourly");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    /* FTP archive: CDDIS, IGN, or WHU */
    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the IGS observation files */
        for (int i = 0; i < fopt->hhObs.size(); i++)
        {
            string sHh = str.hh2str(fopt->hhObs[i]);
            char tmpDir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
            string sHhDir = tmpDir;
            if (access(sHhDir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                string cmd = "mkdir -p " + sHhDir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
            chdir(sHhDir.c_str());
#endif

            string url, cutDirs = " --cut-dirs=7 ";
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBSH] + "/" +
                sYyyy + "/" + sDoy + "/" + sHh;
            else if (ftpName == "IGN")
            {
                url = _ftpArchive.IGN[IDX_OBSH] + "/" + sYyyy + "/" + sDoy;
                cutDirs = " --cut-dirs=6 ";
            }
            else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBSH] + "/" +
                sYyyy + "/" + sDoy + "/" + sHh;
            else url = _ftpArchive.CDDIS[IDX_OBSH] + "/" + sYyyy + "/" + sDoy + "/" + sHh;

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhObs[i] + 97;
            char ch = ii;
            string sch;
            sch.push_back(ch);
            /* it is OK for '*.Z' or '*.gz' format */
            string dFile = "*" + sDoy + sch + "." + sYy + "d";
            string dxFile = dFile + ".*";
            string cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
            std::system(cmd.c_str());

            /* get the file list */
            string suffix = "." + sYy + "d";
            vector<string> dFiles;
            str.GetFilesAll(sHhDir, suffix, dFiles);
            char sitName[MAXCHARS];
            for (int i = 0; i < dFiles.size(); i++)
            {
                if (access(dFiles[i].c_str(), 0) == 0)
                {
                    /* extract it */
                    cmd = gzipFull + " -d -f " + dFiles[i];
                    std::system(cmd.c_str());

                    str.StrMid(sitName, dFiles[i].c_str(), 0, 4);
                    string site = sitName;
                    str.ToLower(site);
                    dFile = site + sDoy + sch + "." + sYy + "d";
                    if (access(dFile.c_str(), 0) == -1) continue;
                    string oFile = site + sDoy + sch + "." + sYy + "o";
                    /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + dFile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + dFile;
#endif
                    std::system(cmd.c_str());
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the IGS observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetHourlyObsIgs): open site.list = " << fopt->obsLst << 
                    " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                for (int i = 0; i < fopt->hhObs.size(); i++)
                {
                    string sHh = str.hh2str(fopt->hhObs[i]);
                    char tmpDir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
                    string sHhDir = tmpDir;
                    if (access(sHhDir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                        string cmd = "mkdir -p " + sHhDir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
                    chdir(sHhDir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhObs[i] + 97;
                    char ch = ii;
                    string sch;
                    sch.push_back(ch);
                    str.ToLower(sitName);
                    string oFile = sitName + sDoy + sch + "." + sYy + "o";
                    string dFile = sitName + sDoy + sch + "." + sYy + "d";
                    if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                    {
                        string url, cutDirs = " --cut-dirs=7 ";
                        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBSH] + "/" +
                            sYyyy + "/" + sDoy + "/" + sHh;
                        else if (ftpName == "IGN")
                        {
                            url = _ftpArchive.IGN[IDX_OBSH] + "/" + sYyyy + "/" + sDoy;
                            cutDirs = " --cut-dirs=6 ";
                        }
                        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBSH] + "/" +
                            sYyyy + "/" + sDoy + "/" + sHh;
                        else url = _ftpArchive.CDDIS[IDX_OBSH] + "/" + sYyyy + "/" + sDoy + "/" + sHh;

                        /* it is OK for '*.Z' or '*.gz' format */
                        string dxFile = dFile + ".*";
                        string cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
                        std::system(cmd.c_str());

                        string dgzFile = dFile + ".gz", dzFile = dFile + ".Z";
                        bool isgz = false;
                        if (access(dgzFile.c_str(), 0) == 0)
                        {
                            /* extract '*.gz' */
                            cmd = gzipFull + " -d -f " + dgzFile;
                            std::system(cmd.c_str());
                            isgz = true;

                            if (access(dFile.c_str(), 0) == 0 && access(dzFile.c_str(), 0) == 0)
                            {
                                /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                                cmd = "del " + dzFile;
#else           /* for Linux or Mac */
                                cmd = "rm -rf " + dzFile;
#endif
                                std::system(cmd.c_str());
                            }
                        }
                        if (access(dFile.c_str(), 0) == -1 && access(dzFile.c_str(), 0) == 0)
                        {
                            /* extract '*.Z' */
                            cmd = gzipFull + " -d -f " + dzFile;
                            std::system(cmd.c_str());
                            isgz = false;
                        }

                        char tmpFile[MAXSTRPATH] = { '\0' };
                        char sep = (char)FILEPATHSEP;
                        sprintf(tmpFile, "%s%c%s", sHhDir.c_str(), sep, oFile.c_str());
                        string localFile = tmpFile;
                        if (access(dFile.c_str(), 0) == -1)
                        {
                            cout << "*** WARNING(FtpUtil::GetHourlyObsIgs): failed to download IGS Hourly observation file " << oFile << endl;

                            string url0 = url + '/' + dxFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetHourlyObsIgs): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                            continue;
                        }

#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                        cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                        std::system(cmd.c_str());

                        if (access(oFile.c_str(), 0) == 0)
                        {
                            cout << "*** INFO(FtpUtil::GetHourlyObsIgs): successfully download IGS hourly observation file " << oFile << endl;

                            if (isgz) dxFile = dgzFile;
                            else dxFile = dzFile;
                            string url0 = url + '/' + dxFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetHourlyObsIgs): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                            /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + dFile;
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + dFile;
#endif
                            std::system(cmd.c_str());
                        }
                    }
                    else cout << "*** INFO(FtpUtil::GetHourlyObsIgs): IGS hourly observation file " <<
                        oFile << " or " << dFile << " has existed!" << endl;
                }
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetHourlyObsIgs */

/**
* @brief   : GetHrObsIgs - download IGS RINEX high-rate observation (1s) files (short name "d") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHrObsIgs(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('highrate') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "highrate");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    /* FTP archive: CDDIS, IGN, or WHU */
    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the IGS observation files */
        for (int i = 0; i < fopt->hhObs.size(); i++)
        {
            string sHh = str.hh2str(fopt->hhObs[i]);
            char tmpDir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
            string sHhDir = tmpDir;
            if (access(sHhDir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                string cmd = "mkdir -p " + sHhDir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
            chdir(sHhDir.c_str());
#endif

            string url, cutDirs = " --cut-dirs=8 ";
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBSHR] + "/" +
                sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
            else if (ftpName == "IGN")
            {
                url = _ftpArchive.IGN[IDX_OBSHR] + "/" + sYyyy + "/" + sDoy;
                cutDirs = " --cut-dirs=6 ";
            }
            else if (ftpName == "WHU")
            {
                url = _ftpArchive.WHU[IDX_OBSHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
                cutDirs = " --cut-dirs=6 ";
            }
            else url = _ftpArchive.CDDIS[IDX_OBSHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhObs[i] + 97;
            char ch = ii;
            string sch;
            sch.push_back(ch);
            /* it is OK for '*.Z' or '*.gz' format */
            string dFile = "*" + sDoy + sch + "*." + sYy + "d";
            string dxFile = dFile + ".*";
            string cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
            std::system(cmd.c_str());

            std::vector<string> minuStr = { "00", "15", "30", "45" };
            for (int i = 0; i < minuStr.size(); i++)
            {
                /* get the file list */
                string suffix = minuStr[i] + "." + sYy + "d";
                vector<string> dFiles;
                str.GetFilesAll(sHhDir, suffix, dFiles);
                char sitName[MAXCHARS];
                for (int j = 0; j < dFiles.size(); j++)
                {
                    if (access(dFiles[j].c_str(), 0) == 0)
                    {
                        /* extract it */
                        cmd = gzipFull + " -d -f " + dFiles[j];
                        std::system(cmd.c_str());

                        str.StrMid(sitName, dFiles[j].c_str(), 0, 4);
                        string site = sitName;
                        str.ToLower(site);
                        dFile = site + sDoy + sch + minuStr[i] + "." + sYy + "d";
                        if (access(dFile.c_str(), 0) == -1) continue;
                        string oFile = site + sDoy + sch + minuStr[i] + "." + sYy + "o";
                        /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                        cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                        std::system(cmd.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the IGS observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetHrObsIgs): open site.list = " << fopt->obsLst << 
                    " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                for (int i = 0; i < fopt->hhObs.size(); i++)
                {
                    string sHh = str.hh2str(fopt->hhObs[i]);
                    char tmpDir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
                    string sHhDir = tmpDir;
                    if (access(sHhDir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                        string cmd = "mkdir -p " + sHhDir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
                    chdir(sHhDir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhObs[i] + 97;
                    char ch = ii;
                    string sch;
                    sch.push_back(ch);

                    std::vector<string> minuStr = { "00", "15", "30", "45" };
                    for (int i = 0; i < minuStr.size(); i++)
                    {
                        str.ToLower(sitName);
                        string oFile = sitName + sDoy + sch + minuStr[i] + "." + sYy + "o";
                        string dFile = sitName + sDoy + sch + minuStr[i] + "." + sYy + "d";
                        if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                        {
                            string url, cutDirs = " --cut-dirs=8 ";
                            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBSHR] + "/" +
                                sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
                            else if (ftpName == "IGN")
                            {
                                url = _ftpArchive.IGN[IDX_OBSHR] + "/" + sYyyy + "/" + sDoy;
                                cutDirs = " --cut-dirs=6 ";
                            }
                            else if (ftpName == "WHU")
                            {
                                url = _ftpArchive.WHU[IDX_OBSHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
                                cutDirs = " --cut-dirs=6 ";
                            }
                            else url = _ftpArchive.CDDIS[IDX_OBSHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;

                            /* it is OK for '*.Z' or '*.gz' format */
                            string dxFile = dFile + ".*";
                            string cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
                            std::system(cmd.c_str());

                            string dgzFile = dFile + ".gz", dzFile = dFile + ".Z";
                            bool isgz = false;
                            if (access(dgzFile.c_str(), 0) == 0)
                            {
                                /* extract '*.gz' */
                                cmd = gzipFull + " -d -f " + dgzFile;
                                std::system(cmd.c_str());
                                isgz = true;

                                if (access(dFile.c_str(), 0) == 0 && access(dzFile.c_str(), 0) == 0)
                                {
                                    /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                                    cmd = "del " + dzFile;
#else           /* for Linux or Mac */
                                    cmd = "rm -rf " + dzFile;
#endif
                                    std::system(cmd.c_str());
                                }
                            }
                            if (access(dFile.c_str(), 0) == -1 && access(dzFile.c_str(), 0) == 0)
                            {
                                /* extract '*.Z' */
                                cmd = gzipFull + " -d -f " + dzFile;
                                std::system(cmd.c_str());
                                isgz = false;
                            }

                            char tmpFile[MAXSTRPATH] = { '\0' };
                            char sep = (char)FILEPATHSEP;
                            sprintf(tmpFile, "%s%c%s", sHhDir.c_str(), sep, oFile.c_str());
                            string localFile = tmpFile;
                            if (access(dFile.c_str(), 0) == -1)
                            {
                                cout << "*** WARNING(FtpUtil::GetHrObsIgs): failed to download IGS high-rate observation file " << oFile << endl;

                                string url0 = url + '/' + dxFile;
                                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetHrObsIgs): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                                continue;
                            }

#ifdef _WIN32  /* for Windows */
                            cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                            cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                            std::system(cmd.c_str());

                            if (access(oFile.c_str(), 0) == 0)
                            {
                                cout << "*** INFO(FtpUtil::GetHrObsIgs): successfully download IGS high-rate observation file " << oFile << endl;

                                if (isgz) dxFile = dgzFile;
                                else dxFile = dzFile;
                                string url0 = url + '/' + dxFile;
                                if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetHrObsIgs): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                                cmd = "del " + dFile;
#else           /* for Linux or Mac */
                                cmd = "rm -rf " + dFile;
#endif
                                std::system(cmd.c_str());
                            }
                        }
                        else cout << "*** INFO(FtpUtil::GetHrObsIgs): IGS high-rate observation file " <<
                            oFile << " or " << dFile << " has existed!" << endl;
                    }
                }
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetHrObsIgs */

/**
* @brief   : GetDailyObsMgex - download MGEX RINEX daily observation (30s) files (long name "crx") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsMgex(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "daily");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    /* FTP archive: CDDIS, IGN, or WHU */
    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the MGEX observation files */
        string url, cutDirs = " --cut-dirs=7 ";
        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMD] + "/" + 
            sYyyy + "/" + sDoy + "/" + sYy + "d";
        else if (ftpName == "IGN")
        {
            url = _ftpArchive.IGN[IDX_OBMD] + "/" + sYyyy + "/" + sDoy;
            cutDirs = " --cut-dirs=5 ";
        }
        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBMD] + "/" + 
            sYyyy + "/" + sDoy + "/" + sYy + "d";
        else url = _ftpArchive.CDDIS[IDX_OBMD] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d";

        /* it is OK for '*.Z' or '*.gz' format */
        string crxFile = "*" + sYyyy + sDoy + "0000_01D_30S_MO.crx";
        string crxxFile = crxFile + ".*";
        string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        string suffix = ".crx";
        vector<string> crxFiles;
        str.GetFilesAll(subDir, suffix, crxFiles);
        char sitName[MAXCHARS];
        for (int i = 0; i < crxFiles.size(); i++)
        {
            if (access(crxFiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipFull + " -d -f " + crxFiles[i];
                std::system(cmd.c_str());

                str.StrMid(sitName, crxFiles[i].c_str(), 0, 4);
                string site = sitName;
                str.ToLower(site);
                str.StrMid(sitName, crxFiles[i].c_str(), 0, (int)crxFiles[i].find_last_of('.'));
                crxFile = sitName;
                if (access(crxFile.c_str(), 0) == -1) continue;
                string oFile = site + sDoy + "0." + sYy + "o";
                /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + crxFile;
#endif
                std::system(cmd.c_str());
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the MGEX observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetDailyObsMgex): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                str.ToLower(sitName);
                string oFile = sitName + sDoy + "0." + sYy + "o";
                string dFile = sitName + sDoy + "0." + sYy + "d";
                if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                {
                    string url, cutDirs = " --cut-dirs=7 ";
                    if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMD] + "/" + 
                        sYyyy + "/" + sDoy + "/" + sYy + "d";
                    else if (ftpName == "IGN")
                    {
                        url = _ftpArchive.IGN[IDX_OBMD] + "/" + sYyyy + "/" + sDoy;
                        cutDirs = " --cut-dirs=5 ";
                    }
                    else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBMD] + "/" + 
                        sYyyy + "/" + sDoy + "/" + sYy + "d";
                    else url = _ftpArchive.CDDIS[IDX_OBMD] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d";

                    /* it is OK for '*.Z' or '*.gz' format */
                    str.ToUpper(sitName);
                    string crxFile = sitName + "*" + sYyyy + sDoy + "0000_01D_30S_MO.crx";
                    string crxxFile = crxFile + ".*";
                    string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    string crxgzFile = crxFile + ".gz";
                    cmd = gzipFull + " -d -f " + crxgzFile;
                    std::system(cmd.c_str());
                    string changeFileName;
#ifdef _WIN32  /* for Windows */
                    changeFileName = "rename";
#else          /* for Linux or Mac */
                    changeFileName = "mv";
#endif
                    cmd = changeFileName + " " + crxFile + " " + dFile;
                    std::system(cmd.c_str());
                    bool isgz = true;
                    string crxzFile;
                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, oFile.c_str());
                    string localFile = tmpFile;
                    if (access(dFile.c_str(), 0) == -1)
                    {
                        /* extract '*.Z' */
                        crxzFile = crxFile + ".Z";
                        cmd = gzipFull + " -d -f " + crxzFile;
                        std::system(cmd.c_str());

                        cmd = changeFileName + " " + crxFile + " " + dFile;
                        std::system(cmd.c_str());
                        isgz = false;
                        if (access(dFile.c_str(), 0) == -1)
                        {
                            cout << "*** WARNING(FtpUtil::GetDailyObsMgex): failed to download MGEX daily observation file " << oFile << endl;

                            string url0 = url + '/' + crxxFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsMgex): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                            continue;
                        }
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    if (access(oFile.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::GetDailyObsMgex): successfully download MGEX daily observation file " << oFile << endl;

                        if (isgz) crxxFile = crxgzFile;
                        else crxxFile = crxzFile;
                        string url0 = url + '/' + crxxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDailyObsMgex): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::GetDailyObsMgex): MGEX daily observation file " << oFile << 
                    " or " << dFile << " has existed!" << endl;
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetDailyObsMgex */

/**
* @brief   : GetHourlyObsMgex - download MGEX RINEX hourly observation (30s) files (long name "crx") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHourlyObsMgex(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('hourly') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "hourly");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    /* FTP archive: CDDIS, IGN, or WHU */
    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the MGEX observation files */
        for (int i = 0; i < fopt->hhObs.size(); i++)
        {
            string sHh = str.hh2str(fopt->hhObs[i]);
            char tmpDir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
            string sHhDir = tmpDir;
            if (access(sHhDir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                string cmd = "mkdir -p " + sHhDir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
            chdir(sHhDir.c_str());
#endif

            string url, cutDirs = " --cut-dirs=7 ";
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMH] + "/" +
                sYyyy + "/" + sDoy + "/" + sHh;
            else if (ftpName == "IGN")
            {
                url = _ftpArchive.IGN[IDX_OBMH] + "/" + sYyyy + "/" + sDoy;
                cutDirs = " --cut-dirs=6 ";
            }
            else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBMH] + "/" +
                sYyyy + "/" + sDoy + "/" + sHh;
            else url = _ftpArchive.CDDIS[IDX_OBMH] + "/" + sYyyy + "/" + sDoy + "/" + sHh;

            /* it is OK for '*.Z' or '*.gz' format */
            string crxFile = "*" + sYyyy + sDoy + sHh + "00_01H_30S_MO.crx";
            string crxxFile = crxFile + ".*";
            string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
            std::system(cmd.c_str());

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhObs[i] + 97;
            char ch = ii;
            string sch;
            sch.push_back(ch);

            /* get the file list */
            string suffix = ".crx";
            vector<string> crxFiles;
            str.GetFilesAll(sHhDir, suffix, crxFiles);
            char sitName[MAXCHARS];
            for (int i = 0; i < crxFiles.size(); i++)
            {
                if (access(crxFiles[i].c_str(), 0) == 0)
                {
                    /* extract it */
                    cmd = gzipFull + " -d -f " + crxFiles[i];
                    std::system(cmd.c_str());

                    str.StrMid(sitName, crxFiles[i].c_str(), 0, 4);
                    string site = sitName;
                    str.ToLower(site);
                    str.StrMid(sitName, crxFiles[i].c_str(), 0, (int)crxFiles[i].find_last_of('.'));
                    crxFile = sitName;
                    if (access(crxFile.c_str(), 0) == -1) continue;
                    string oFile = site + sDoy + sch + "." + sYy + "o";
                    /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + crxFile;
#endif
                    std::system(cmd.c_str());
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the MGEX observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetHourlyObsMgex): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                for (int i = 0; i < fopt->hhObs.size(); i++)
                {
                    string sHh = str.hh2str(fopt->hhObs[i]);
                    char tmpDir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
                    string sHhDir = tmpDir;
                    if (access(sHhDir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                        string cmd = "mkdir -p " + sHhDir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
                    chdir(sHhDir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhObs[i] + 97;
                    char ch = ii;
                    string sch;
                    sch.push_back(ch);
                    str.ToLower(sitName);
                    string oFile = sitName + sDoy + sch + "." + sYy + "o";
                    string dFile = sitName + sDoy + sch + "." + sYy + "d";
                    if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                    {
                        string url, cutDirs = " --cut-dirs=7 ";
                        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMH] + "/" +
                            sYyyy + "/" + sDoy + "/" + sHh;
                        else if (ftpName == "IGN")
                        {
                            url = _ftpArchive.IGN[IDX_OBMH] + "/" + sYyyy + "/" + sDoy;
                            cutDirs = " --cut-dirs=6 ";
                        }
                        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBMH] + "/" +
                            sYyyy + "/" + sDoy + "/" + sHh;
                        else url = _ftpArchive.CDDIS[IDX_OBMH] + "/" + sYyyy + "/" + sDoy + "/" + sHh;

                        /* it is OK for '*.Z' or '*.gz' format */
                        str.ToUpper(sitName);
                        string crxFile = sitName + "*" + sYyyy + sDoy + sHh + "00_01H_30S_MO.crx";
                        string crxxFile = crxFile + ".*";
                        string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
                        std::system(cmd.c_str());

                        /* extract '*.gz' */
                        string crxgzFile = crxFile + ".gz";
                        cmd = gzipFull + " -d -f " + crxgzFile;
                        std::system(cmd.c_str());
                        string changeFileName;
#ifdef _WIN32  /* for Windows */
                        changeFileName = "rename";
#else          /* for Linux or Mac */
                        changeFileName = "mv";
#endif
                        cmd = changeFileName + " " + crxFile + " " + dFile;
                        std::system(cmd.c_str());
                        bool isgz = true;
                        string crxzFile;
                        char tmpFile[MAXSTRPATH] = { '\0' };
                        char sep = (char)FILEPATHSEP;
                        sprintf(tmpFile, "%s%c%s", sHhDir.c_str(), sep, oFile.c_str());
                        string localFile = tmpFile;
                        if (access(dFile.c_str(), 0) == -1)
                        {
                            /* extract '*.Z' */
                            crxzFile = crxFile + ".Z";
                            cmd = gzipFull + " -d -f " + crxzFile;
                            std::system(cmd.c_str());

                            cmd = changeFileName + " " + crxFile + " " + dFile;
                            std::system(cmd.c_str());
                            if (access(dFile.c_str(), 0) == -1)
                            {
                                cout << "*** WARNING(FtpUtil::GetDailyObsMgex): failed to download MGEX daily observation file " << oFile << endl;

                                string url0 = url + '/' + crxxFile;
                                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsMgex): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                                continue;
                            }
                        }

#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                        cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                        std::system(cmd.c_str());

                        if (access(oFile.c_str(), 0) == 0)
                        {
                            cout << "*** INFO(FtpUtil::GetHourlyObsMgex): successfully download MGEX hourly observation file " << oFile << endl;

                            if (isgz) crxxFile = crxgzFile;
                            else crxxFile = crxzFile;
                            string url0 = url + '/' + crxxFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetHourlyObsMgex): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                            /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + dFile;
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + dFile;
#endif
                            std::system(cmd.c_str());
                        }
                    }
                    else cout << "*** INFO(FtpUtil::GetHourlyObsMgex): MGEX hourly observation file " <<
                        oFile << " or " << dFile << " has existed!" << endl;
                }
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetHourlyObsMgex */

/**
* @brief   : GetHrObsMgex - download MGEX RINEX high-rate observation (1s) files (long name "crx") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHrObsMgex(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('highrate') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "highrate");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    /* FTP archive: CDDIS, IGN, or WHU */
    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the MGEX observation files */
        for (int i = 0; i < fopt->hhObs.size(); i++)
        {
            string sHh = str.hh2str(fopt->hhObs[i]);
            char tmpDir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
            string sHhDir = tmpDir;
            if (access(sHhDir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                string cmd = "mkdir -p " + sHhDir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
            chdir(sHhDir.c_str());
#endif

            string url, cutDirs = " --cut-dirs=8 ";
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMHR] + "/" +
                sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
            else if (ftpName == "IGN")
            {
                url = _ftpArchive.IGN[IDX_OBMHR] + "/" + sYyyy + "/" + sDoy;
                cutDirs = " --cut-dirs=6 ";
            }
            else if (ftpName == "WHU")
            {
                url = _ftpArchive.WHU[IDX_OBMHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
                cutDirs = " --cut-dirs=6 ";
            }
            else url = _ftpArchive.CDDIS[IDX_OBMHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;

            /* it is OK for '*.Z' or '*.gz' format */
            string crxFile = "*_15M_01S_MO.crx";
            string crxxFile = crxFile + ".*";
            string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
            std::system(cmd.c_str());

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhObs[i] + 97;
            char ch = ii;
            string sch;
            sch.push_back(ch);

            std::vector<string> minuStr = { "00", "15", "30", "45" };
            for (int i = 0; i < minuStr.size(); i++)
            {
                /* get the file list */
                string suffix = minuStr[i] + "_15M_01S_MO.crx";
                vector<string> crxFiles;
                str.GetFilesAll(sHhDir, suffix, crxFiles);
                char sitName[MAXCHARS];
                for (int j = 0; j < crxFiles.size(); j++)
                {
                    if (access(crxFiles[j].c_str(), 0) == 0)
                    {
                        /* extract it */
                        cmd = gzipFull + " -d -f " + crxFiles[j];
                        std::system(cmd.c_str());

                        str.StrMid(sitName, crxFiles[j].c_str(), 0, 4);
                        string site = sitName;
                        str.ToLower(site);
                        str.StrMid(sitName, crxFiles[j].c_str(), 0, (int)crxFiles[j].find_last_of('.'));
                        crxFile = sitName;
                        if (access(crxFile.c_str(), 0) == -1) continue;
                        string oFile = site + sDoy + sch + minuStr[i] +  "." + sYy + "o";
                        /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                        cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                        std::system(cmd.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + crxFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the MGEX observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetHrObsMgex): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                for (int i = 0; i < fopt->hhObs.size(); i++)
                {
                    string sHh = str.hh2str(fopt->hhObs[i]);
                    char tmpDir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
                    string sHhDir = tmpDir;
                    if (access(sHhDir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                        string cmd = "mkdir -p " + sHhDir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
                    chdir(sHhDir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhObs[i] + 97;
                    char ch = ii;
                    string sch;
                    sch.push_back(ch);

                    std::vector<string> minuStr = { "00", "15", "30", "45" };
                    for (int i = 0; i < minuStr.size(); i++)
                    {
                        str.ToLower(sitName);
                        string oFile = sitName + sDoy + sch + minuStr[i] + "." + sYy + "o";
                        string dFile = sitName + sDoy + sch + minuStr[i] + "." + sYy + "d";
                        if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                        {
                            string url, cutDirs = " --cut-dirs=8 ";
                            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMHR] + "/" +
                                sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
                            else if (ftpName == "IGN")
                            {
                                url = _ftpArchive.IGN[IDX_OBMHR] + "/" + sYyyy + "/" + sDoy;
                                cutDirs = " --cut-dirs=6 ";
                            }
                            else if (ftpName == "WHU")
                            {
                                url = _ftpArchive.WHU[IDX_OBMHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
                                cutDirs = " --cut-dirs=6 ";
                            }
                            else url = _ftpArchive.CDDIS[IDX_OBMHR] + "/" + sYyyy + "/" + sDoy + "/" + 
                                sYy + "d" + "/" + sHh;

                            /* it is OK for '*.Z' or '*.gz' format */
                            str.ToUpper(sitName);
                            string crxFile = sitName + "*" + sYyyy + sDoy + sHh + minuStr[i] + "_15M_01S_MO.crx";
                            string crxxFile = crxFile + ".*";
                            string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
                            std::system(cmd.c_str());

                            /* extract '*.gz' */
                            string crxgzFile = crxFile + ".gz";
                            cmd = gzipFull + " -d -f " + crxgzFile;
                            std::system(cmd.c_str());
                            string changeFileName;
#ifdef _WIN32  /* for Windows */
                            changeFileName = "rename";
#else          /* for Linux or Mac */
                            changeFileName = "mv";
#endif
                            cmd = changeFileName + " " + crxFile + " " + dFile;
                            std::system(cmd.c_str());
                            bool isgz = true;
                            string crxzFile;
                            char tmpFile[MAXSTRPATH] = { '\0' };
                            char sep = (char)FILEPATHSEP;
                            sprintf(tmpFile, "%s%c%s", sHhDir.c_str(), sep, oFile.c_str());
                            string localFile = tmpFile;
                            if (access(dFile.c_str(), 0) == -1)
                            {
                                /* extract '*.Z' */
                                crxzFile = crxFile + ".Z";
                                cmd = gzipFull + " -d -f " + crxzFile;
                                std::system(cmd.c_str());

                                cmd = changeFileName + " " + crxFile + " " + dFile;
                                std::system(cmd.c_str());
                                isgz = false;
                                if (access(dFile.c_str(), 0) == -1)
                                {
                                    cout << "*** WARNING(FtpUtil::GetHrObsMgex): failed to download MGEX high-rate observation file " << oFile << endl;

                                    string url0 = url + '/' + crxxFile;
                                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetHrObsMgex): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                                    continue;
                                }
                            }

#ifdef _WIN32  /* for Windows */
                            cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                            cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                            std::system(cmd.c_str());

                            if (access(oFile.c_str(), 0) == 0)
                            {
                                cout << "*** INFO(FtpUtil::GetHrObsMgex): successfully download MGEX high-rate observation file " << oFile << endl;

                                if (isgz) crxxFile = crxgzFile;
                                else crxxFile = crxzFile;
                                string url0 = url + '/' + crxxFile;
                                if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetHrObsMgex): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                                cmd = "del " + dFile;
#else           /* for Linux or Mac */
                                cmd = "rm -rf " + dFile;
#endif
                                std::system(cmd.c_str());
                            }
                        }
                        else cout << "*** INFO(FtpUtil::GetHrObsMgex): MGEX high-rate observation file " << 
                            oFile << " or " << dFile << " has existed!" << endl;
                    }
                }
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetHrObsMgex */

/**
* @brief   : GetDailyObsIgm - to download IGS (short name "d") and MGEX RINEX daily observation (30s) files (long name "crx") 
*              simutaneously according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsIgm(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "daily");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    /* FTP archive: CDDIS, IGN, or WHU */
    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the MGEX observation files */
        string url, cutDirs = " --cut-dirs=7 ";
        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMD] + "/" +
            sYyyy + "/" + sDoy + "/" + sYy + "d";
        else if (ftpName == "IGN")
        {
            url = _ftpArchive.IGN[IDX_OBMD] + "/" + sYyyy + "/" + sDoy;
            cutDirs = " --cut-dirs=5 ";
        }
        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBMD] + "/" +
            sYyyy + "/" + sDoy + "/" + sYy + "d";
        else url = _ftpArchive.CDDIS[IDX_OBMD] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d";

        /* it is OK for '*.Z' or '*.gz' format */
        string crxFile = "*" + sYyyy + sDoy + "0000_01D_30S_MO.crx";
        string crxxFile = crxFile + ".*";
        string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        string suffix = ".crx";
        vector<string> crxFiles;
        str.GetFilesAll(subDir, suffix, crxFiles);
        char sitName[MAXCHARS];
        for (int i = 0; i < crxFiles.size(); i++)
        {
            if (access(crxFiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipFull + " -d -f " + crxFiles[i];
                std::system(cmd.c_str());

                str.StrMid(sitName, crxFiles[i].c_str(), 0, 4);
                string site = sitName;
                str.ToLower(site);
                str.StrMid(sitName, crxFiles[i].c_str(), 0, (int)crxFiles[i].find_last_of('.'));
                crxFile = sitName;
                if (access(crxFile.c_str(), 0) == -1) continue;
                string oFile = site + sDoy + "0." + sYy + "o";
                /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + crxFile;
#endif
                std::system(cmd.c_str());
            }
        }

        /* download all the IGS observation files */
        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBSD] + "/" +
            sYyyy + "/" + sDoy + "/" + sYy + "d";
        else if (ftpName == "IGN")
        {
            url = _ftpArchive.IGN[IDX_OBSD] + "/" + sYyyy + "/" + sDoy;
            cutDirs = " --cut-dirs=5 ";
        }
        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBSD] + "/" +
            sYyyy + "/" + sDoy + "/" + sYy + "d";
        else url = _ftpArchive.CDDIS[IDX_OBSD] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d";

        /* it is OK for '*.Z' or '*.gz' format */
        string dFile = "*" + sDoy + "0." + sYy + "d";
        string dxFile = dFile + ".*";
        cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        suffix = "." + sYy + "d";
        vector<string> dFiles;
        str.GetFilesAll(subDir, suffix, dFiles);
        for (int i = 0; i < dFiles.size(); i++)
        {
            if (access(dFiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipFull + " -d -f " + dFiles[i];
                std::system(cmd.c_str());

                str.StrMid(sitName, dFiles[i].c_str(), 0, 4);
                string site = sitName;
                str.ToLower(site);
                dFile = site + sDoy + "0." + sYy + "d";
                if (access(dFile.c_str(), 0) == -1) continue;
                string oFile = site + sDoy + "0." + sYy + "o";
                if (access(oFile.c_str(), 0) == 0)
                {
                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + dFile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + dFile;
#endif
                    std::system(cmd.c_str());

                    continue;
                }
                /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + dFile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + dFile;
#endif
                std::system(cmd.c_str());
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetDailyObsIgm): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                str.ToLower(sitName);
                string oFile = sitName + sDoy + "0." + sYy + "o";
                string dFile = sitName + sDoy + "0." + sYy + "d";
                if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                {
                    /* download the MGEX observation file site-by-site */
                    string url, cutDirs = " --cut-dirs=7 ";
                    if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMD] + "/" +
                        sYyyy + "/" + sDoy + "/" + sYy + "d";
                    else if (ftpName == "IGN")
                    {
                        url = _ftpArchive.IGN[IDX_OBMD] + "/" + sYyyy + "/" + sDoy;
                        cutDirs = " --cut-dirs=5 ";
                    }
                    else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBMD] + "/" +
                        sYyyy + "/" + sDoy + "/" + sYy + "d";
                    else url = _ftpArchive.CDDIS[IDX_OBMD] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d";

                    /* it is OK for '*.Z' or '*.gz' format */
                    str.ToUpper(sitName);
                    string crxFile = sitName + "*" + sYyyy + sDoy + "0000_01D_30S_MO.crx";
                    string crxxFile = crxFile + ".*";
                    string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    string crxgzFile = crxFile + ".gz";
                    cmd = gzipFull + " -d -f " + crxgzFile;
                    std::system(cmd.c_str());
                    string changeFileName;
#ifdef _WIN32  /* for Windows */
                    changeFileName = "rename";
#else          /* for Linux or Mac */
                    changeFileName = "mv";
#endif
                    cmd = changeFileName + " " + crxFile + " " + dFile;
                    std::system(cmd.c_str());
                    bool isgz = true;

                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, oFile.c_str());
                    string localFile = tmpFile;
                    if (access(dFile.c_str(), 0) == -1)
                    {
                        /* extract '*.Z' */
                        string crxzFile = crxFile + ".Z";
                        cmd = gzipFull + " -d -f " + crxzFile;
                        std::system(cmd.c_str());

                        cmd = changeFileName + " " + crxFile + " " + dFile;
                        std::system(cmd.c_str());
                        isgz = false;
                        if (access(dFile.c_str(), 0) == -1)
                        {
                            /* download the IGS observation file site-by-site */
                            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBSD] + "/" +
                                sYyyy + "/" + sDoy + "/" + sYy + "d";
                            else if (ftpName == "IGN")
                            {
                                url = _ftpArchive.IGN[IDX_OBSD] + "/" + sYyyy + "/" + sDoy;
                                cutDirs = " --cut-dirs=5 ";
                            }
                            else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBSD] + "/" +
                                sYyyy + "/" + sDoy + "/" + sYy + "d";
                            else url = _ftpArchive.CDDIS[IDX_OBSD] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d";

                            /* it is OK for '*.Z' or '*.gz' format */
                            string dxFile = dFile + ".*";
                            string cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
                            std::system(cmd.c_str());

                            string dgzFile = dFile + ".gz", dzFile = dFile + ".Z";
                            if (access(dgzFile.c_str(), 0) == 0)
                            {
                                /* extract '*.gz' */
                                cmd = gzipFull + " -d -f " + dgzFile;
                                std::system(cmd.c_str());
                                isgz = true;

                                if (access(dFile.c_str(), 0) == 0 && access(dzFile.c_str(), 0) == 0)
                                {
                                    /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                                    cmd = "del " + dzFile;
#else           /* for Linux or Mac */
                                    cmd = "rm -rf " + dzFile;
#endif
                                    std::system(cmd.c_str());
                                }
                            }
                            if (access(dFile.c_str(), 0) == -1 && access(dzFile.c_str(), 0) == 0)
                            {
                                /* extract '*.Z' */
                                cmd = gzipFull + " -d -f " + dzFile;
                                std::system(cmd.c_str());
                                isgz = false;
                            }
                            if (access(dFile.c_str(), 0) == -1)
                            {
                                cout << "*** WARNING(FtpUtil::GetDailyObsIgm): failed to download IGM daily observation file " << oFile << endl;

                                string url0 = url + '/' + crxxFile;
                                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsIgm): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                                url0 = url + '/' + dxFile;
                                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsIgm): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                                continue;
                            }
                        }
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    if (access(oFile.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::GetDailyObsIgm): successfully download IGM daily observation file " << oFile << endl;

                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDailyObsIgm): %s  ->  %s  OK\n", oFile.c_str(), localFile.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::GetDailyObsIgm): IGM daily observation file " << oFile <<
                    " or " << dFile << " has existed!" << endl;
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetDailyObsIgm */

/**
* @brief   : GetHourlyObsIgm  - to download IGS (short name "d") and MGEX RINEX hourly observation (30s) files (long name "crx") 
*              simutaneously according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHourlyObsIgm(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('hourly') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "hourly");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    /* FTP archive: CDDIS, IGN, or WHU */
    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        for (int i = 0; i < fopt->hhObs.size(); i++)
        {
            string sHh = str.hh2str(fopt->hhObs[i]);
            char tmpDir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
            string sHhDir = tmpDir;
            if (access(sHhDir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                string cmd = "mkdir -p " + sHhDir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
            chdir(sHhDir.c_str());
#endif

            /* download all the MGEX observation files */
            string url, cutDirs = " --cut-dirs=7 ";
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMH] + "/" +
                sYyyy + "/" + sDoy + "/" + sHh;
            else if (ftpName == "IGN")
            {
                url = _ftpArchive.IGN[IDX_OBMH] + "/" + sYyyy + "/" + sDoy;
                cutDirs = " --cut-dirs=6 ";
            }
            else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBMH] + "/" +
                sYyyy + "/" + sDoy + "/" + sHh;
            else url = _ftpArchive.CDDIS[IDX_OBMH] + "/" + sYyyy + "/" + sDoy + "/" + sHh;

            /* it is OK for '*.Z' or '*.gz' format */
            string crxFile = "*" + sYyyy + sDoy + sHh + "00_01H_30S_MO.crx";
            string crxxFile = crxFile + ".*";
            string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
            std::system(cmd.c_str());

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhObs[i] + 97;
            char ch = ii;
            string sch;
            sch.push_back(ch);

            /* get the file list */
            string suffix = ".crx";
            vector<string> crxFiles;
            str.GetFilesAll(sHhDir, suffix, crxFiles);
            char sitName[MAXCHARS];
            for (int i = 0; i < crxFiles.size(); i++)
            {
                if (access(crxFiles[i].c_str(), 0) == 0)
                {
                    /* extract it */
                    cmd = gzipFull + " -d -f " + crxFiles[i];
                    std::system(cmd.c_str());

                    str.StrMid(sitName, crxFiles[i].c_str(), 0, 4);
                    string site = sitName;
                    str.ToLower(site);
                    str.StrMid(sitName, crxFiles[i].c_str(), 0, (int)crxFiles[i].find_last_of('.'));
                    crxFile = sitName;
                    if (access(crxFile.c_str(), 0) == -1) continue;
                    string oFile = site + sDoy + sch + "." + sYy + "o";
                    /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + crxFile;
#endif
                    std::system(cmd.c_str());
                }
            }

            /* download all the IGS observation files */
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBSH] + "/" +
                sYyyy + "/" + sDoy + "/" + sHh;
            else if (ftpName == "IGN")
            {
                url = _ftpArchive.IGN[IDX_OBSH] + "/" + sYyyy + "/" + sDoy;
                cutDirs = " --cut-dirs=6 ";
            }
            else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBSH] + "/" +
                sYyyy + "/" + sDoy + "/" + sHh;
            else url = _ftpArchive.CDDIS[IDX_OBSH] + "/" + sYyyy + "/" + sDoy + "/" + sHh;

            /* it is OK for '*.Z' or '*.gz' format */
            string dFile = "*" + sDoy + sch + "." + sYy + "d";
            string dxFile = dFile + ".*";
            cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
            std::system(cmd.c_str());

            /* get the file list */
            suffix = "." + sYy + "d";
            vector<string> dFiles;
            str.GetFilesAll(sHhDir, suffix, dFiles);
            for (int i = 0; i < dFiles.size(); i++)
            {
                if (access(dFiles[i].c_str(), 0) == 0)
                {
                    /* extract it */
                    cmd = gzipFull + " -d -f " + dFiles[i];
                    std::system(cmd.c_str());

                    str.StrMid(sitName, dFiles[i].c_str(), 0, 4);
                    string site = sitName;
                    str.ToLower(site);
                    dFile = site + sDoy + sch + "." + sYy + "d";
                    if (access(dFile.c_str(), 0) == -1) continue;
                    string oFile = site + sDoy + sch + "." + sYy + "o";
                    if (access(oFile.c_str(), 0) == 0)
                    {
                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dFile;
#endif
                        std::system(cmd.c_str());

                        continue;
                    }
                    /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + dFile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + dFile;
#endif
                    std::system(cmd.c_str());
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetHourlyObsIgm): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                for (int i = 0; i < fopt->hhObs.size(); i++)
                {
                    string sHh = str.hh2str(fopt->hhObs[i]);
                    char tmpDir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
                    string sHhDir = tmpDir;
                    if (access(sHhDir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                        string cmd = "mkdir -p " + sHhDir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
                    chdir(sHhDir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhObs[i] + 97;
                    char ch = ii;
                    string sch;
                    sch.push_back(ch);
                    str.ToLower(sitName);
                    string oFile = sitName + sDoy + sch + "." + sYy + "o";
                    string dFile = sitName + sDoy + sch + "." + sYy + "d";
                    if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                    {
                        /* download the MGEX observation file site-by-site */
                        string url, cutDirs = " --cut-dirs=7 ";
                        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMH] + "/" +
                            sYyyy + "/" + sDoy + "/" + sHh;
                        else if (ftpName == "IGN")
                        {
                            url = _ftpArchive.IGN[IDX_OBMH] + "/" + sYyyy + "/" + sDoy;
                            cutDirs = " --cut-dirs=6 ";
                        }
                        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBMH] + "/" +
                            sYyyy + "/" + sDoy + "/" + sHh;
                        else url = _ftpArchive.CDDIS[IDX_OBMH] + "/" + sYyyy + "/" + sDoy + "/" + sHh;

                        /* it is OK for '*.Z' or '*.gz' format */
                        str.ToUpper(sitName);
                        string crxFile = sitName + "*" + sYyyy + sDoy + sHh + "00_01H_30S_MO.crx";
                        string crxxFile = crxFile + ".*";
                        string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
                        std::system(cmd.c_str());

                        /* extract '*.gz' */
                        string crxgzFile = crxFile + ".gz";
                        cmd = gzipFull + " -d -f " + crxgzFile;
                        std::system(cmd.c_str());
                        string changeFileName;
#ifdef _WIN32  /* for Windows */
                        changeFileName = "rename";
#else          /* for Linux or Mac */
                        changeFileName = "mv";
#endif
                        cmd = changeFileName + " " + crxFile + " " + dFile;
                        std::system(cmd.c_str());
                        bool isgz = true;

                        char tmpFile[MAXSTRPATH] = { '\0' };
                        char sep = (char)FILEPATHSEP;
                        sprintf(tmpFile, "%s%c%s", sHhDir.c_str(), sep, oFile.c_str());
                        string localFile = tmpFile;
                        if (access(dFile.c_str(), 0) == -1)
                        {
                            /* extract '*.Z' */
                            string crxzFile = crxFile + ".Z";
                            cmd = gzipFull + " -d -f " + crxzFile;
                            std::system(cmd.c_str());

                            cmd = changeFileName + " " + crxFile + " " + dFile;
                            std::system(cmd.c_str());
                            isgz = false;
                            if (access(dFile.c_str(), 0) == -1)
                            {
                                /* download the IGS observation file site-by-site */
                                if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBSH] + "/" +
                                    sYyyy + "/" + sDoy + "/" + sHh;
                                else if (ftpName == "IGN")
                                {
                                    url = _ftpArchive.IGN[IDX_OBSH] + "/" + sYyyy + "/" + sDoy;
                                    cutDirs = " --cut-dirs=6 ";
                                }
                                else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBSH] + "/" +
                                    sYyyy + "/" + sDoy + "/" + sHh;
                                else url = _ftpArchive.CDDIS[IDX_OBSH] + "/" + sYyyy + "/" + sDoy + "/" + sHh;

                                /* it is OK for '*.Z' or '*.gz' format */
                                string dxFile = dFile + ".*";
                                string cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
                                std::system(cmd.c_str());

                                string dgzFile = dFile + ".gz", dzFile = dFile + ".Z";
                                if (access(dgzFile.c_str(), 0) == 0)
                                {
                                    /* extract '*.gz' */
                                    cmd = gzipFull + " -d -f " + dgzFile;
                                    std::system(cmd.c_str());
                                    isgz = true;

                                    if (access(dFile.c_str(), 0) == 0 && access(dzFile.c_str(), 0) == 0)
                                    {
                                        /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                                        cmd = "del " + dzFile;
#else           /* for Linux or Mac */
                                        cmd = "rm -rf " + dzFile;
#endif
                                        std::system(cmd.c_str());
                                    }
                                }
                                if (access(dFile.c_str(), 0) == -1 && access(dzFile.c_str(), 0) == 0)
                                {
                                    /* extract '*.Z' */
                                    cmd = gzipFull + " -d -f " + dzFile;
                                    std::system(cmd.c_str());
                                    isgz = false;
                                }
                                if (access(dFile.c_str(), 0) == -1)
                                {
                                    cout << "*** WARNING(FtpUtil::GetHourlyObsIgm): failed to download IGM Hourly observation file " << oFile << endl;

                                    string url0 = url + '/' + crxxFile;
                                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetHourlyObsIgm): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                                    url0 = url + '/' + dxFile;
                                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetHourlyObsIgm): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                                    continue;
                                }
                            }
                        }

#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                        cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                        std::system(cmd.c_str());

                        if (access(oFile.c_str(), 0) == 0)
                        {
                            cout << "*** INFO(FtpUtil::GetHourlyObsIgm ): successfully download IGM hourly observation file " << oFile << endl;

                            if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetHourlyObsIgm): %s  ->  %s  OK\n", oFile.c_str(), localFile.c_str());

                            /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + dFile;
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + dFile;
#endif
                            std::system(cmd.c_str());
                        }
                    }
                    else cout << "*** INFO(FtpUtil::GetHourlyObsIgm ): IGM hourly observation file " <<
                        oFile << " or " << dFile << " has existed!" << endl;
                }
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetHourlyObsIgm */

/**
* @brief   : GetHrObsIgm - to download IGS (short name "d") and MGEX RINEX high-rate observation (1s) files (long name "crx") 
*              simutaneously according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHrObsIgm(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('highrate') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "highrate");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    /* FTP archive: CDDIS, IGN, or WHU */
    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        for (int i = 0; i < fopt->hhObs.size(); i++)
        {
            string sHh = str.hh2str(fopt->hhObs[i]);
            char tmpDir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
            string sHhDir = tmpDir;
            if (access(sHhDir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                string cmd = "mkdir -p " + sHhDir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
            chdir(sHhDir.c_str());
#endif

            /* download all the MGEX observation files */
            string url, cutDirs = " --cut-dirs=8 ";
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMHR] + "/" +
                sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
            else if (ftpName == "IGN")
            {
                url = _ftpArchive.IGN[IDX_OBMHR] + "/" + sYyyy + "/" + sDoy;
                cutDirs = " --cut-dirs=6 ";
            }
            else if (ftpName == "WHU")
            {
                url = _ftpArchive.WHU[IDX_OBMHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
                cutDirs = " --cut-dirs=6 ";
            }
            else url = _ftpArchive.CDDIS[IDX_OBMHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;

            /* it is OK for '*.Z' or '*.gz' format */
            string crxFile = "*_15M_01S_MO.crx";
            string crxxFile = crxFile + ".*";
            string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
            std::system(cmd.c_str());

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhObs[i] + 97;
            char ch = ii;
            string sch;
            sch.push_back(ch);

            std::vector<string> minuStr = { "00", "15", "30", "45" };
            for (int i = 0; i < minuStr.size(); i++)
            {
                /* get the file list */
                string suffix = minuStr[i] + "_15M_01S_MO.crx";
                vector<string> crxFiles;
                str.GetFilesAll(sHhDir, suffix, crxFiles);
                char sitName[MAXCHARS];
                for (int j = 0; j < crxFiles.size(); j++)
                {
                    if (access(crxFiles[j].c_str(), 0) == 0)
                    {
                        /* extract it */
                        cmd = gzipFull + " -d -f " + crxFiles[j];
                        std::system(cmd.c_str());

                        str.StrMid(sitName, crxFiles[j].c_str(), 0, 4);
                        string site = sitName;
                        str.ToLower(site);
                        str.StrMid(sitName, crxFiles[j].c_str(), 0, (int)crxFiles[j].find_last_of('.'));
                        crxFile = sitName;
                        if (access(crxFile.c_str(), 0) == -1) continue;
                        string oFile = site + sDoy + sch + minuStr[i] + "." + sYy + "o";
                        /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                        cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                        std::system(cmd.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + crxFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }

            /* download all the IGS observation files */
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBSHR] + "/" +
                sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
            else if (ftpName == "IGN")
            {
                url = _ftpArchive.IGN[IDX_OBSHR] + "/" + sYyyy + "/" + sDoy;
                cutDirs = " --cut-dirs=6 ";
            }
            else if (ftpName == "WHU")
            {
                url = _ftpArchive.WHU[IDX_OBSHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
                cutDirs = " --cut-dirs=6 ";
            }
            else url = _ftpArchive.CDDIS[IDX_OBSHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;

            /* it is OK for '*.Z' or '*.gz' format */
            string dFile = "*" + sDoy + sch + "*." + sYy + "d";
            string dxFile = dFile + ".*";
            cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
            std::system(cmd.c_str());

            for (int i = 0; i < minuStr.size(); i++)
            {
                /* get the file list */
                string suffix = minuStr[i] + "." + sYy + "d";
                vector<string> dFiles;
                str.GetFilesAll(sHhDir, suffix, dFiles);
                char sitName[MAXCHARS];
                for (int j = 0; j < dFiles.size(); j++)
                {
                    if (access(dFiles[j].c_str(), 0) == 0)
                    {
                        /* extract it */
                        cmd = gzipFull + " -d -f " + dFiles[j];
                        std::system(cmd.c_str());

                        str.StrMid(sitName, dFiles[j].c_str(), 0, 4);
                        string site = sitName;
                        str.ToLower(site);
                        dFile = site + sDoy + sch + minuStr[i] + "." + sYy + "d";
                        if (access(dFile.c_str(), 0) == -1) continue;
                        string oFile = site + sDoy + sch + minuStr[i] + "." + sYy + "o";
                        /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                        cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                        std::system(cmd.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the MGEX observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetHrObsIgm): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                for (int i = 0; i < fopt->hhObs.size(); i++)
                {
                    string sHh = str.hh2str(fopt->hhObs[i]);
                    char tmpDir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
                    string sHhDir = tmpDir;
                    if (access(sHhDir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                        string cmd = "mkdir -p " + sHhDir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
                    chdir(sHhDir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhObs[i] + 97;
                    char ch = ii;
                    string sch;
                    sch.push_back(ch);

                    std::vector<string> minuStr = { "00", "15", "30", "45" };
                    for (int i = 0; i < minuStr.size(); i++)
                    {
                        str.ToLower(sitName);
                        string oFile = sitName + sDoy + sch + minuStr[i] + "." + sYy + "o";
                        string dFile = sitName + sDoy + sch + minuStr[i] + "." + sYy + "d";
                        if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                        {
                            string url, cutDirs = " --cut-dirs=8 ";
                            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMHR] + "/" +
                                sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
                            else if (ftpName == "IGN")
                            {
                                url = _ftpArchive.IGN[IDX_OBMHR] + "/" + sYyyy + "/" + sDoy;
                                cutDirs = " --cut-dirs=6 ";
                            }
                            else if (ftpName == "WHU")
                            {
                                url = _ftpArchive.WHU[IDX_OBMHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
                                cutDirs = " --cut-dirs=6 ";
                            }
                            else url = _ftpArchive.CDDIS[IDX_OBMHR] + "/" + sYyyy + "/" + sDoy + "/" +
                                sYy + "d" + "/" + sHh;

                            /* it is OK for '*.Z' or '*.gz' format */
                            str.ToUpper(sitName);
                            string crxFile = sitName + "*" + sYyyy + sDoy + sHh + minuStr[i] + "_15M_01S_MO.crx";
                            string crxxFile = crxFile + ".*";
                            string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
                            std::system(cmd.c_str());

                            /* extract '*.gz' */
                            string crxgzFile = crxFile + ".gz";
                            cmd = gzipFull + " -d -f " + crxgzFile;
                            std::system(cmd.c_str());
                            string changeFileName;
#ifdef _WIN32  /* for Windows */
                            changeFileName = "rename";
#else          /* for Linux or Mac */
                            changeFileName = "mv";
#endif
                            cmd = changeFileName + " " + crxFile + " " + dFile;
                            std::system(cmd.c_str());
                            bool isgz = true;

                            char tmpFile[MAXSTRPATH] = { '\0' };
                            char sep = (char)FILEPATHSEP;
                            sprintf(tmpFile, "%s%c%s", sHhDir.c_str(), sep, oFile.c_str());
                            string localFile = tmpFile;
                            if (access(dFile.c_str(), 0) == -1)
                            {
                                /* extract '*.Z' */
                                string crxzFile = crxFile + ".Z";
                                cmd = gzipFull + " -d -f " + crxzFile;
                                std::system(cmd.c_str());

                                cmd = changeFileName + " " + crxFile + " " + dFile;
                                std::system(cmd.c_str());
                                isgz = false;
                                if (access(dFile.c_str(), 0) == -1)
                                {
                                    if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBSHR] + "/" +
                                        sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
                                    else if (ftpName == "IGN")
                                    {
                                        url = _ftpArchive.IGN[IDX_OBSHR] + "/" + sYyyy + "/" + sDoy;
                                        cutDirs = " --cut-dirs=6 ";
                                    }
                                    else if (ftpName == "WHU")
                                    {
                                        url = _ftpArchive.WHU[IDX_OBSHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;
                                        cutDirs = " --cut-dirs=6 ";
                                    }
                                    else url = _ftpArchive.CDDIS[IDX_OBSHR] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "d" + "/" + sHh;

                                    /* it is OK for '*.Z' or '*.gz' format */
                                    string dxFile = dFile + ".*";
                                    cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
                                    std::system(cmd.c_str());

                                    string dgzFile = dFile + ".gz", dzFile = dFile + ".Z";
                                    if (access(dgzFile.c_str(), 0) == 0)
                                    {
                                        /* extract '*.gz' */
                                        cmd = gzipFull + " -d -f " + dgzFile;
                                        std::system(cmd.c_str());
                                        isgz = true;

                                        if (access(dFile.c_str(), 0) == 0 && access(dzFile.c_str(), 0) == 0)
                                        {
                                            /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                                            cmd = "del " + dzFile;
#else           /* for Linux or Mac */
                                            cmd = "rm -rf " + dzFile;
#endif
                                            std::system(cmd.c_str());
                                        }
                                    }
                                    if (access(dFile.c_str(), 0) == -1 && access(dzFile.c_str(), 0) == 0)
                                    {
                                        /* extract '*.Z' */
                                        cmd = gzipFull + " -d -f " + dzFile;
                                        std::system(cmd.c_str());
                                        isgz = false;
                                    }
                                    if (access(dFile.c_str(), 0) == -1)
                                    {
                                        cout << "*** WARNING(FtpUtil::GetHrObsIgm): failed to download IGM high-rate observation file " << oFile << endl;

                                        string url0 = url + '/' + crxxFile;
                                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetHrObsIgm): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                                        url0 = url + '/' + dxFile;
                                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetHrObsIgm): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                                        continue;
                                    }
                                }
                            }

#ifdef _WIN32  /* for Windows */
                            cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                            cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                            std::system(cmd.c_str());

                            if (access(oFile.c_str(), 0) == 0)
                            {
                                cout << "*** INFO(FtpUtil::GetHrObsIgm): successfully download IGM high-rate observation file " << oFile << endl;

                                if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetHrObsIgm): %s  ->  %s  OK\n", oFile.c_str(), localFile.c_str());

                                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                                cmd = "del " + dFile;
#else           /* for Linux or Mac */
                                cmd = "rm -rf " + dFile;
#endif
                                std::system(cmd.c_str());
                            }
                        }
                        else cout << "*** INFO(FtpUtil::GetHrObsIgm): IGM high-rate observation file " <<
                            oFile << " or " << dFile << " has existed!" << endl;
                    }
                }
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetHrObsIgm */

/**
* @brief   : GetDailyObsCut - download Curtin University of Technology (CUT) RINEX daily observation (30s) files (long name "crx") 
*              according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsCut(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "daily");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    string url = "http://saegnss2.curtin.edu/ldc/rinex3/daily/" + sYyyy + "/" + sDoy;
    string cutDirs = " --cut-dirs=5 ";
    /* download the Curtin University of Technology (CUT) observation file site-by-site */
    if (access(fopt->obsLst, 0) == 0)
    {
        ifstream sitLst(fopt->obsLst);
        if (!sitLst.is_open())
        {
            cerr << "*** ERROR(FtpUtil::GetDailyObsCut): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

            return;
        }

        string sitName;
        while (getline(sitLst, sitName))
        {
            if (sitName[0] == '#') continue;
            str.TrimSpace4String(sitName);
            if (sitName.size() != 4) continue;
            str.ToLower(sitName);
            string oFile = sitName + sDoy + "0." + sYy + "o";
            if (access(oFile.c_str(), 0) == -1)
            {
                /* it is OK for '*.gz' format */
                str.ToUpper(sitName);
                string crxFile = sitName + "00AUS_R_" + sYyyy + sDoy + "0000_01D_30S_MO.crx";
                string crxgzFile = crxFile + ".gz";
                string cmd = wgetFull + " " + qr + " -nH " + cutDirs + url + "/" + crxgzFile;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                cmd = gzipFull + " -d -f " + crxgzFile;
                std::system(cmd.c_str());

                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, oFile.c_str());
                string localFile = tmpFile;
                if (access(crxFile.c_str(), 0) == -1)
                {
                    cout << "*** WARNING(FtpUtil::GetDailyObsCut): failed to download CUT daily observation file " << oFile << endl;

                    string url0 = url + '/' + crxgzFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsCut): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                    continue;
                }

#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                if (access(oFile.c_str(), 0) == 0)
                {
                    cout << "*** INFO(FtpUtil::GetDailyObsCut): successfully download CUT daily observation file " << oFile << endl;

                    string url0 = url + '/' + crxgzFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDailyObsCut): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                    /* delete 'crx' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + crxFile;
#endif
                    std::system(cmd.c_str());
                }
            }
            else cout << "*** INFO(FtpUtil::GetDailyObsCut): CUT daily observation file " << oFile <<
                " has existed!" << endl;
        }

        /* close 'site.list' */
        sitLst.close();
    }
} /* end of GetDailyObsCut */

/**
* @brief   : GetDailyObsGa - download Geoscience Australia (GA) RINEX daily observation (30s) files (long name "crx") 
*              according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsGa(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "daily");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    string url = "ftp://ftp.data.gnss.ga.gov.au/daily/" + sYyyy + "/" + sDoy;
    string cutDirs = " --cut-dirs=3 ";
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* it is OK for '*.gz' format */
        string crxFile = "*" + sYyyy + sDoy + "0000_01D_30S_MO.crx";
        string crxgzFile = crxFile + ".gz";
        string cmd = wgetFull + " " + qr + " -nH -A " + crxgzFile + cutDirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        string suffix = ".crx";
        vector<string> crxFiles;
        str.GetFilesAll(subDir, suffix, crxFiles);
        char sitName[MAXCHARS];
        for (int i = 0; i < crxFiles.size(); i++)
        {
            if (access(crxFiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipFull + " -d -f " + crxFiles[i];
                std::system(cmd.c_str());

                str.StrMid(sitName, crxFiles[i].c_str(), 0, 4);
                string site = sitName;
                str.ToLower(site);
                str.StrMid(sitName, crxFiles[i].c_str(), 0, (int)crxFiles[i].find_last_of('.'));
                crxFile = sitName;
                if (access(crxFile.c_str(), 0) == -1) continue;
                string oFile = site + sDoy + "0." + sYy + "o";
                /* convert from 'crx' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                /* delete crxFile */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + crxFile;
#endif
                std::system(cmd.c_str());
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the GA observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetDailyObsGa): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                str.ToLower(sitName);
                string oFile = sitName + sDoy + "0." + sYy + "o";
                string dFile = sitName + sDoy + "0." + sYy + "d";
                if (access(oFile.c_str(), 0) == -1)
                {
                    /* it is OK for '*.gz' format */
                    str.ToUpper(sitName);
                    string crxFile = sitName + "*" + sYyyy + sDoy + "0000_01D_30S_MO.crx";
                    string crxgzFile = crxFile + ".gz";
                    string cmd = wgetFull + " " + qr + " -nH -A " + crxgzFile + cutDirs + url;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    cmd = gzipFull + " -d -f " + crxgzFile;
                    std::system(cmd.c_str());
                    string changeFileName;
#ifdef _WIN32  /* for Windows */
                    changeFileName = "rename";
#else          /* for Linux or Mac */
                    changeFileName = "mv";
#endif
                    cmd = changeFileName + " " + crxFile + " " + dFile;
                    std::system(cmd.c_str());

                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, oFile.c_str());
                    string localFile = tmpFile;
                    if (access(dFile.c_str(), 0) == -1)
                    {
                        cout << "*** WARNING(FtpUtil::GetDailyObsGa): failed to download GA daily observation file " << oFile << endl;

                        string url0 = url + '/' + crxgzFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsGa): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                        continue;
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    if (access(oFile.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::GetDailyObsGa): successfully download GA daily observation file " << oFile << endl;

                        string url0 = url + '/' + crxgzFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDailyObsGa): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::GetDailyObsGa): GA daily observation file " << oFile <<
                    " has existed!" << endl;
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetDailyObsGa */

/**
* @brief   : GetHourlyObsGa - download Geoscience Australia (GA) RINEX hourly observation (30s) files (long name "crx") 
*              according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHourlyObsGa(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('hourly') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "hourly");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    string url0 = "ftp://ftp.data.gnss.ga.gov.au/hourly/" + sYyyy + "/" + sDoy;
    string cutDirs = " --cut-dirs=4 ";
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the GA observation files */
        for (int i = 0; i < fopt->hhObs.size(); i++)
        {
            string sHh = str.hh2str(fopt->hhObs[i]);
            char tmpDir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
            string sHhDir = tmpDir;
            if (access(sHhDir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                string cmd = "mkdir -p " + sHhDir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
            chdir(sHhDir.c_str());
#endif

            string url = url0 + "/" + sHh;
            /* it is OK for '*.gz' format */
            string crxFile = "*" + sYyyy + sDoy + sHh + "00_01H_30S_MO.crx";
            string crxgzFile = crxFile + ".gz";
            string cmd = wgetFull + " " + qr + " -nH -A " + crxgzFile + cutDirs + url;
            std::system(cmd.c_str());

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhObs[i] + 97;
            char ch = ii;
            string sch;
            sch.push_back(ch);

            /* get the file list */
            string suffix = ".crx";
            vector<string> crxFiles;
            str.GetFilesAll(sHhDir, suffix, crxFiles);
            char sitName[MAXCHARS];
            for (int i = 0; i < crxFiles.size(); i++)
            {
                if (access(crxFiles[i].c_str(), 0) == 0)
                {
                    /* extract it */
                    cmd = gzipFull + "  -d -f " + crxFiles[i];
                    std::system(cmd.c_str());

                    str.StrMid(sitName, crxFiles[i].c_str(), 0, 4);
                    string site = sitName;
                    str.ToLower(site);
                    str.StrMid(sitName, crxFiles[i].c_str(), 0, (int)crxFiles[i].find_last_of('.'));
                    crxFile = sitName;
                    if (access(crxFile.c_str(), 0) == -1) continue;
                    string oFile = site + sDoy + sch + "." + sYy + "o";
                    /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + crxFile;
#endif
                    std::system(cmd.c_str());
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the GA observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetHourlyObsGa): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                for (int i = 0; i < fopt->hhObs.size(); i++)
                {
                    string sHh = str.hh2str(fopt->hhObs[i]);
                    char tmpDir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
                    string sHhDir = tmpDir;
                    if (access(sHhDir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                        string cmd = "mkdir -p " + sHhDir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
                    chdir(sHhDir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhObs[i] + 97;
                    char ch = ii;
                    string sch;
                    sch.push_back(ch);
                    str.ToLower(sitName);
                    string oFile = sitName + sDoy + sch + "." + sYy + "o";
                    string dFile = sitName + sDoy + sch + "." + sYy + "d";
                    if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                    {
                        string url = url0 + "/" + sHh;
                        /* it is OK for '*.gz' format */
                        str.ToUpper(sitName);
                        string crxFile = sitName + "*" + sYyyy + sDoy + sHh + "00_01H_30S_MO.crx";
                        string crxgzFile = crxFile + ".gz";
                        string cmd = wgetFull + " " + qr + " -nH -A " + crxgzFile + cutDirs + url;
                        std::system(cmd.c_str());

                        /* extract '*.gz' */
                        cmd = gzipFull + " -d -f " + crxgzFile;
                        std::system(cmd.c_str());
                        string changeFileName;
#ifdef _WIN32  /* for Windows */
                        changeFileName = "rename";
#else          /* for Linux or Mac */
                        changeFileName = "mv";
#endif
                        cmd = changeFileName + " " + crxFile + " " + dFile;
                        std::system(cmd.c_str());

                        char tmpFile[MAXSTRPATH] = { '\0' };
                        char sep = (char)FILEPATHSEP;
                        sprintf(tmpFile, "%s%c%s", sHhDir.c_str(), sep, oFile.c_str());
                        string localFile = tmpFile;
                        if (access(dFile.c_str(), 0) == -1)
                        {
                            cout << "*** WARNING(FtpUtil::GetHourlyObsGa): failed to download GA hourly observation file " << oFile << endl;

                            string url0 = url + '/' + crxgzFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetHourlyObsGa): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                            continue;
                        }

#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                        cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                        std::system(cmd.c_str());

                        if (access(oFile.c_str(), 0) == 0)
                        {
                            cout << "*** INFO(FtpUtil::GetHourlyObsGa): successfully download GA hourly observation file " << oFile << endl;

                            string url0 = url + '/' + crxgzFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetHourlyObsGa): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                            /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + dFile;
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + dFile;
#endif
                            std::system(cmd.c_str());
                        }
                    }
                    else cout << "*** INFO(FtpUtil::GetHourlyObsGa): GA hourly observation file " <<
                        oFile << " or " << dFile << " has existed!" << endl;
                }
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetHourlyObsMgex */

/**
* @brief   : GetHrObsGa - download Geoscience Australia (GA) RINEX high-rate observation (1s) files (long name "crx") 
               according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHrObsGa(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('highrate') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "highrate");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    string url0 = "ftp://ftp.data.gnss.ga.gov.au/highrate/" + sYyyy + "/" + sDoy;
    string cutDirs = " --cut-dirs=4 ";
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the MGEX observation files */
        for (int i = 0; i < fopt->hhObs.size(); i++)
        {
            string sHh = str.hh2str(fopt->hhObs[i]);
            char tmpDir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
            string sHhDir = tmpDir;
            if (access(sHhDir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                string cmd = "mkdir -p " + sHhDir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
            chdir(sHhDir.c_str());
#endif

            string url = url0 + "/" + sHh;
            /* it is OK for '*.gz' format */
            string crxFile = "*_15M_01S_MO.crx";
            string crxgzFile = crxFile + ".gz";
            string cmd = wgetFull + " " + qr + " -nH -A " + crxgzFile + cutDirs + url;
            std::system(cmd.c_str());

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhObs[i] + 97;
            char ch = ii;
            string sch;
            sch.push_back(ch);

            std::vector<string> minuStr = { "00", "15", "30", "45" };
            for (int i = 0; i < minuStr.size(); i++)
            {
                /* get the file list */
                string suffix = minuStr[i] + "_15M_01S_MO.crx";
                vector<string> crxFiles;
                str.GetFilesAll(sHhDir, suffix, crxFiles);
                char sitName[MAXCHARS];
                for (int j = 0; j < crxFiles.size(); j++)
                {
                    if (access(crxFiles[j].c_str(), 0) == 0)
                    {
                        /* extract it */
                        cmd = gzipFull + " -d -f " + crxFiles[j];
                        std::system(cmd.c_str());

                        str.StrMid(sitName, crxFiles[j].c_str(), 0, 4);
                        string site = sitName;
                        str.ToLower(site);
                        str.StrMid(sitName, crxFiles[j].c_str(), 0, (int)crxFiles[j].find_last_of('.'));
                        crxFile = sitName;
                        if (access(crxFile.c_str(), 0) == -1) continue;
                        string oFile = site + sDoy + sch + minuStr[i] + "." + sYy + "o";
                        /* convert from 'crx' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                        cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                        std::system(cmd.c_str());

                        /* delete 'crx' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + crxFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the GA observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetHrObsGa): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                for (int i = 0; i < fopt->hhObs.size(); i++)
                {
                    string sHh = str.hh2str(fopt->hhObs[i]);
                    char tmpDir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
                    string sHhDir = tmpDir;
                    if (access(sHhDir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                        string cmd = "mkdir -p " + sHhDir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
                    chdir(sHhDir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhObs[i] + 97;
                    char ch = ii;
                    string sch;
                    sch.push_back(ch);

                    string url = url0 + "/" + sHh;
                    std::vector<string> minuStr = { "00", "15", "30", "45" };
                    for (int i = 0; i < minuStr.size(); i++)
                    {
                        str.ToLower(sitName);
                        string oFile = sitName + sDoy + sch + minuStr[i] + "." + sYy + "o";
                        string dFile = sitName + sDoy + sch + minuStr[i] + "." + sYy + "d";
                        if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                        {
                            /* it is OK for '*.gz' format */
                            str.ToUpper(sitName);
                            string crxFile = sitName + "*" + sYyyy + sDoy + sHh + minuStr[i] + "_15M_01S_MO.crx";
                            string crxgzFile = crxFile + ".gz";
                            string cmd = wgetFull + " " + qr + " -nH -A " + crxgzFile + cutDirs + url;
                            std::system(cmd.c_str());

                            /* extract '*.gz' */
                            cmd = gzipFull + " -d -f " + crxgzFile;
                            std::system(cmd.c_str());
                            string changeFileName;
#ifdef _WIN32  /* for Windows */
                            changeFileName = "rename";
#else          /* for Linux or Mac */
                            changeFileName = "mv";
#endif
                            cmd = changeFileName + " " + crxFile + " " + dFile;
                            std::system(cmd.c_str());

                            char tmpFile[MAXSTRPATH] = { '\0' };
                            char sep = (char)FILEPATHSEP;
                            sprintf(tmpFile, "%s%c%s", sHhDir.c_str(), sep, oFile.c_str());
                            string localFile = tmpFile;
                            if (access(dFile.c_str(), 0) == -1)
                            {
                                cout << "*** WARNING(FtpUtil::GetHrObsGa): failed to download GA high-rate observation file " << oFile << endl;

                                string url0 = url + '/' + crxgzFile;
                                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetHrObsGa): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                                continue;
                            }

#ifdef _WIN32  /* for Windows */
                            cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                            cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                            std::system(cmd.c_str());

                            if (access(oFile.c_str(), 0) == 0)
                            {
                                cout << "*** INFO(FtpUtil::GetHrObsGa): successfully download GA high-rate observation file " << oFile << endl;

                                string url0 = url + '/' + crxgzFile;
                                if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetHrObsGa): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                                cmd = "del " + dFile;
#else           /* for Linux or Mac */
                                cmd = "rm -rf " + dFile;
#endif
                                std::system(cmd.c_str());
                            }
                        }
                        else cout << "*** INFO(FtpUtil::GetHrObsGa): GA high-rate observation file " <<
                            oFile << " or " << dFile << " has existed!" << endl;
                    }
                }
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetHrObsGa */

/**
* @brief   : Get30sObsHk - download Hong Kong CORS RINEX daily observation (30s) files (long name "crx") 
               according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::Get30sObsHk(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "30s");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    string url0 = "ftp://ftp.geodetic.gov.hk/rinex3/" + sYyyy + "/" + sDoy;
    string cutDirs = " --cut-dirs=5 ";
    /* download the Hong Kong CORS observation file site-by-site */
    if (access(fopt->obsLst, 0) == 0)
    {
        ifstream sitLst(fopt->obsLst);
        if (!sitLst.is_open())
        {
            cerr << "*** ERROR(FtpUtil::Get30sObsHk): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

            return;
        }

        string sitName;
        while (getline(sitLst, sitName))
        {
            if (sitName[0] == '#') continue;
            str.TrimSpace4String(sitName);
            if (sitName.size() != 4) continue;
            str.ToLower(sitName);
            string oFile = sitName + sDoy + "0." + sYy + "o";
            string url = url0 + "/" + sitName + "/30s";
            if (access(oFile.c_str(), 0) == -1)
            {
                /* it is OK for '*.gz' format */
                str.ToUpper(sitName);
                string crxFile = sitName + "00HKG_R_" + sYyyy + sDoy + "0000_01D_30S_MO.crx";
                string crxgzFile = crxFile + ".gz";
                string cmd = wgetFull + " " + qr + " -nH " + cutDirs + url + "/" + crxgzFile;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                cmd = gzipFull + " -d -f " + crxgzFile;
                std::system(cmd.c_str());

                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, oFile.c_str());
                string localFile = tmpFile;
                if (access(crxFile.c_str(), 0) == -1)
                {
                    cout << "*** WARNING(FtpUtil::Get30sObsHk): failed to download HK CORS 30s observation file " << oFile << endl;

                    string url0 = url + '/' + crxgzFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(Get30sObsHk): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                    continue;
                }

#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                if (access(oFile.c_str(), 0) == 0)
                {
                    cout << "*** INFO(FtpUtil::Get30sObsHk): successfully download HK CORS 30s observation file " << oFile << endl;

                    string url0 = url + '/' + crxgzFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(Get30sObsHk): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                    /* delete 'crx' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + crxFile;
#endif
                    std::system(cmd.c_str());
                }
            }
            else cout << "*** INFO(FtpUtil::Get30sObsHk): HK CORS 30s observation file " << oFile <<
                " has existed!" << endl;
        }

        /* close 'site.list' */
        sitLst.close();
    }
} /* end of Get30sObsHk */

/**
* @brief   : Get5sObsHk - download Hong Kong CORS RINEX daily observation (5s) files (long name "crx") 
               according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::Get5sObsHk(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "5s");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    string url0 = "ftp://ftp.geodetic.gov.hk/rinex3/" + sYyyy + "/" + sDoy;
    string cutDirs = " --cut-dirs=5 ";
    /* download the Hong Kong CORS observation file site-by-site */
    if (access(fopt->obsLst, 0) == 0)
    {
        ifstream sitLst(fopt->obsLst);
        if (!sitLst.is_open())
        {
            cerr << "*** ERROR(FtpUtil::Get5sObsHk): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

            return;
        }

        string sitName;
        while (getline(sitLst, sitName))
        {
            if (sitName[0] == '#') continue;
            str.TrimSpace4String(sitName);
            if (sitName.size() != 4) continue;
            for (int i = 0; i < fopt->hhObs.size(); i++)
            {
                string sHh = str.hh2str(fopt->hhObs[i]);
                char tmpDir[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
                string sHhDir = tmpDir;
                if (access(sHhDir.c_str(), 0) == -1)
                {
                    /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                    string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                    string cmd = "mkdir -p " + sHhDir;
#endif
                    std::system(cmd.c_str());
                }

                /* change directory */
#ifdef _WIN32   /* for Windows */
                _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
                chdir(sHhDir.c_str());
#endif

                /* 'a' = 97, 'b' = 98, ... */
                int ii = fopt->hhObs[i] + 97;
                char ch = ii;
                string sch;
                sch.push_back(ch);
                str.ToLower(sitName);
                string oFile = sitName + sDoy + sch + "." + sYy + "o";
                string url = url0 + "/" + sitName + "/5s";
                if (access(oFile.c_str(), 0) == -1)
                {
                    /* it is OK for '*.gz' format */
                    str.ToUpper(sitName);
                    string crxFile = sitName + "00HKG_R_" + sYyyy + sDoy + sHh + "00_01H_05S_MO.crx";
                    string crxgzFile = crxFile + ".gz";
                    string cmd = wgetFull + " " + qr + " -nH " + cutDirs + url + "/" + crxgzFile;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    cmd = gzipFull + " -d -f " + crxgzFile;
                    std::system(cmd.c_str());

                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", sHhDir.c_str(), sep, oFile.c_str());
                    string localFile = tmpFile;
                    if (access(crxFile.c_str(), 0) == -1)
                    {
                        cout << "*** WARNING(FtpUtil::Get5sObsHk): failed to download HK CORS 5s observation file " << oFile << endl;

                        string url0 = url + '/' + crxgzFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(Get5sObsHk): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                        continue;
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    if (access(oFile.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::Get5sObsHk): successfully download HK CORS 5s observation file " << oFile << endl;

                        string url0 = url + '/' + crxgzFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(Get5sObsHk): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                        /* delete 'crx' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + crxFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::Get5sObsHk): HK CORS 5s observation file " << oFile <<
                    " has existed!" << endl;
            }
        }

        /* close 'site.list' */
        sitLst.close();
    }
} /* end of Get5sObsHk */

/**
* @brief   : Get1sObsHk - download Hong Kong CORS RINEX daily observation (1s) files (long name "crx") 
               according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::Get1sObsHk(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "1s");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    string url0 = "ftp://ftp.geodetic.gov.hk/rinex3/" + sYyyy + "/" + sDoy;
    string cutDirs = " --cut-dirs=5 ";
    /* download the Hong Kong CORS observation file site-by-site */
    if (access(fopt->obsLst, 0) == 0)
    {
        ifstream sitLst(fopt->obsLst);
        if (!sitLst.is_open())
        {
            cerr << "*** ERROR(FtpUtil::Get1sObsHk): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

            return;
        }

        string sitName;
        while (getline(sitLst, sitName))
        {
            if (sitName[0] == '#') continue;
            str.TrimSpace4String(sitName);
            if (sitName.size() != 4) continue;
            for (int i = 0; i < fopt->hhObs.size(); i++)
            {
                string sHh = str.hh2str(fopt->hhObs[i]);
                char tmpDir[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
                string sHhDir = tmpDir;
                if (access(sHhDir.c_str(), 0) == -1)
                {
                    /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                    string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                    string cmd = "mkdir -p " + sHhDir;
#endif
                    std::system(cmd.c_str());
                }

                /* change directory */
#ifdef _WIN32   /* for Windows */
                _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
                chdir(sHhDir.c_str());
#endif

                /* 'a' = 97, 'b' = 98, ... */
                int ii = fopt->hhObs[i] + 97;
                char ch = ii;
                string sch;
                sch.push_back(ch);
                str.ToLower(sitName);
                string oFile = sitName + sDoy + sch + "." + sYy + "o";
                string url = url0 + "/" + sitName + "/1s";
                if (access(oFile.c_str(), 0) == -1)
                {
                    /* it is OK for '*.gz' format */
                    str.ToUpper(sitName);
                    string crxFile = sitName + "00HKG_R_" + sYyyy + sDoy + sHh + "00_01H_01S_MO.crx";
                    string crxgzFile = crxFile + ".gz";
                    string cmd = wgetFull + " " + qr + " -nH " + cutDirs + url + "/" + crxgzFile;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    cmd = gzipFull + " -d -f " + crxgzFile;
                    std::system(cmd.c_str());

                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", sHhDir.c_str(), sep, oFile.c_str());
                    string localFile = tmpFile;
                    if (access(crxFile.c_str(), 0) == -1)
                    {
                        cout << "*** WARNING(FtpUtil::Get1sObsHk): failed to download HK CORS 1s observation file " << oFile << endl;

                        string url0 = url + '/' + crxgzFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(Get1sObsHk): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                        continue;
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    if (access(oFile.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::Get1sObsHk): successfully download HK CORS 1s observation file " << oFile << endl;

                        string url0 = url + '/' + crxgzFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(Get1sObsHk): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                        /* delete 'crx' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + crxFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::Get1sObsHk): HK CORS 1s observation file " << oFile <<
                    " has existed!" << endl;
            }
        }

        /* close 'site.list' */
        sitLst.close();
    }
} /* end of Get1sObsHk */

/**
* @brief   : GetDailyObsNgs - download NGS/NOAA CORS RINEX daily observation (30s) files (short name "d") 
               according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsNgs(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "daily");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    string url = "https://noaa-cors-pds.s3.amazonaws.com/rinex/" + sYyyy + "/" + sDoy;
    string cutDirs = " --cut-dirs=4 ";
    /* download the NGS/NOAA CORS observation file site-by-site */
    if (access(fopt->obsLst, 0) == 0)
    {
        ifstream sitLst(fopt->obsLst);
        if (!sitLst.is_open())
        {
            cerr << "*** ERROR(FtpUtil::GetDailyObsNgs): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

            return;
        }

        string sitName;
        while (getline(sitLst, sitName))
        {
            if (sitName[0] == '#') continue;
            str.TrimSpace4String(sitName);
            if (sitName.size() != 4) continue;
            str.ToLower(sitName);
            string oFile = sitName + sDoy + "0." + sYy + "o";
            if (access(oFile.c_str(), 0) == -1)
            {
                /* it is OK for '*.gz' format */
                string dFile = sitName + sDoy + "0." + sYy + "d";
                string dgzFile = dFile + ".gz";
                string cmd = wgetFull + " " + qr + " -nH " + cutDirs + url + "/" + sitName + "/" + dgzFile;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                cmd = gzipFull + " -d -f " + dgzFile;
                std::system(cmd.c_str());

                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, oFile.c_str());
                string localFile = tmpFile;
                if (access(dFile.c_str(), 0) == -1)
                {
                    cout << "*** WARNING(FtpUtil::GetDailyObsNgs): failed to download NGS/NOAA CORS daily observation file " << oFile << endl;

                    string url0 = url + '/' + dgzFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsNgs): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                    continue;
                }

#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                if (access(oFile.c_str(), 0) == 0)
                {
                    cout << "*** INFO(FtpUtil::GetDailyObsNgs): successfully download NGS/NOAA CORS daily observation file " << oFile << endl;

                    string url0 = url + '/' + dgzFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDailyObsNgs): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + dFile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + dFile;
#endif
                    std::system(cmd.c_str());
                }
            }
            else cout << "*** INFO(FtpUtil::GetDailyObsNgs): NGS/NOAA CORS daily observation file " << oFile <<
                " has existed!" << endl;
        }

        /* close 'site.list' */
        sitLst.close();
    }
} /* end of GetDailyObsNgs */

/**
* @brief   : GetDailyObsEpn - download EUREF Permanent Network (EPN) RINEX daily observation (30s) files 
               (long name 'crx' and short name "d") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsEpn(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "daily");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    string url = "ftp://ftp.epncb.oma.be/pub/obs/" + sYyyy + "/" + sDoy;
    string cutDirs = " --cut-dirs=4 ";
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* it is OK for '*.gz' format */
        string crxFile = "*" + sYyyy + sDoy + "0000_01D_30S_MO.crx";
        string crxgzFile = crxFile + ".gz";
        string cmd = wgetFull + " " + qr + " -nH -A " + crxgzFile + cutDirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        string suffix = ".crx";
        vector<string> crxFiles;
        str.GetFilesAll(subDir, suffix, crxFiles);
        char sitName[MAXCHARS];
        for (int i = 0; i < crxFiles.size(); i++)
        {
            if (access(crxFiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipFull + " -d -f " + crxFiles[i];
                std::system(cmd.c_str());

                str.StrMid(sitName, crxFiles[i].c_str(), 0, 4);
                string site = sitName;
                str.ToLower(site);
                str.StrMid(sitName, crxFiles[i].c_str(), 0, (int)crxFiles[i].find_last_of('.'));
                crxFile = sitName;
                if (access(crxFile.c_str(), 0) == -1) continue;
                string oFile = site + sDoy + "0." + sYy + "o";
                /* convert from 'crx' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                /* delete crxFile */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + crxFile;
#endif
                std::system(cmd.c_str());
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the EPN observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetDailyObsEpn): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                str.ToLower(sitName);
                string oFile = sitName + sDoy + "0." + sYy + "o";
                string dFile = sitName + sDoy + "0." + sYy + "d";
                if (access(oFile.c_str(), 0) == -1)
                {
                    /* it is OK for '*.gz' format */
                    str.ToUpper(sitName);
                    string crxFile = sitName + "*" + sYyyy + sDoy + "0000_01D_30S_MO.crx";
                    string crxgzFile = crxFile + ".gz";
                    string cmd = wgetFull + " " + qr + " -nH -A " + crxgzFile + cutDirs + url;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    cmd = gzipFull + " -d -f " + crxgzFile;
                    std::system(cmd.c_str());
                    string changeFileName;
#ifdef _WIN32  /* for Windows */
                    changeFileName = "rename";
#else          /* for Linux or Mac */
                    changeFileName = "mv";
#endif
                    cmd = changeFileName + " " + crxFile + " " + dFile;
                    std::system(cmd.c_str());

                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, oFile.c_str());
                    string localFile = tmpFile;
                    if (access(dFile.c_str(), 0) == -1)
                    {
                        cout << "*** WARNING(FtpUtil::GetDailyObsEpn): failed to download EPN daily observation file " << oFile << endl;

                        string url0 = url + '/' + crxgzFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsEpn): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                        continue;
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    if (access(oFile.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::GetDailyObsEpn): successfully download EPN daily observation file " << oFile << endl;

                        string url0 = url + '/' + crxgzFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDailyObsEpn): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::GetDailyObsEpn): EPN daily observation file " << oFile <<
                    " has existed!" << endl;
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetDailyObsEpn */

/**
* @brief   : GetDailyObsPbo2 - to download Plate Boundary Observatory (PBO) RINEX daily observation (15s) files
               (RINEX version 2.xx, short name "d") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsPbo2(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "daily");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    string url = "ftp://data-out.unavco.org/pub/rinex/obs/" + sYyyy + "/" + sDoy;
    string cutDirs = " --cut-dirs=5 ";
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* it is OK for '*.Z' or '*.gz' format */
        string dFile = "*" + sDoy + "0." + sYy + "d";
        string dxFile = dFile + ".*";
        string cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        string suffix = "." + sYy + "d";
        vector<string> dFiles;
        str.GetFilesAll(subDir, suffix, dFiles);
        char sitName[MAXCHARS];
        for (int i = 0; i < dFiles.size(); i++)
        {
            if (access(dFiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipFull + " -d -f " + dFiles[i];
                std::system(cmd.c_str());

                str.StrMid(sitName, dFiles[i].c_str(), 0, 4);
                string site = sitName;
                str.ToLower(site);
                dFile = site + sDoy + "0." + sYy + "d";
                if (access(dFile.c_str(), 0) == -1) continue;
                string oFile = site + sDoy + "0." + sYy + "o";
                /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + dFile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + dFile;
#endif
                std::system(cmd.c_str());
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the PBO observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetDailyObsPbo2): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                str.ToLower(sitName);
                string oFile = sitName + sDoy + "0." + sYy + "o";
                string dFile = sitName + sDoy + "0." + sYy + "d";
                if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    string dxFile = dFile + ".*";
                    string cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
                    std::system(cmd.c_str());

                    string dgzFile = dFile + ".gz", dzFile = dFile + ".Z";
                    bool isgz = false;
                    if (access(dgzFile.c_str(), 0) == 0)
                    {
                        /* extract '*.gz' */
                        cmd = gzipFull + " -d -f " + dgzFile;
                        std::system(cmd.c_str());
                        isgz = true;

                        if (access(dFile.c_str(), 0) == 0 && access(dzFile.c_str(), 0) == 0)
                        {
                            /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + dzFile;
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + dzFile;
#endif
                            std::system(cmd.c_str());
                        }
                    }

                    if (access(dFile.c_str(), 0) == -1 && access(dzFile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipFull + " -d -f " + dzFile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, oFile.c_str());
                    string localFile = tmpFile;
                    if (access(dFile.c_str(), 0) == -1)
                    {
                        cout << "*** WARNING(FtpUtil::GetDailyObsPbo2): failed to download PBO daily observation file " << oFile << endl;

                        string url0 = url + '/' + dxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsPbo2): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                        continue;
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    if (access(oFile.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::GetDailyObsPbo2): successfully download PBO daily observation file " << oFile << endl;

                        if (isgz) dxFile = dgzFile;
                        else dxFile = dzFile;
                        string url0 = url + '/' + dxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDailyObsPbo2): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::GetDailyObsPbo2): PBO daily observation file " << oFile <<
                    " has existed!" << endl;
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetDailyObsPbo2 */

/**
* @brief   : GetDailyObsPbo3 - to download Plate Boundary Observatory (PBO) RINEX daily observation (15s) files
               (RINEX version 3.xx, long name "crx") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsPbo3(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "daily");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    string url = "ftp://data-out.unavco.org/pub/rinex3/obs/" + sYyyy + "/" + sDoy;
    string cutDirs = " --cut-dirs=5 ";
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* it is OK for '*.Z' or '*.gz' format */
        string crxFile = "*" + sYyyy + sDoy + "0000_01D_15S_MO.crx";
        string crxxFile = crxFile + ".*";
        string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        string suffix = ".crx";
        vector<string> crxFiles;
        str.GetFilesAll(subDir, suffix, crxFiles);
        char sitName[MAXCHARS];
        for (int i = 0; i < crxFiles.size(); i++)
        {
            if (access(crxFiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipFull + " -d -f " + crxFiles[i];
                std::system(cmd.c_str());

                str.StrMid(sitName, crxFiles[i].c_str(), 0, 4);
                string site = sitName;
                str.ToLower(site);
                str.StrMid(sitName, crxFiles[i].c_str(), 0, (int)crxFiles[i].find_last_of('.'));
                crxFile = sitName;
                if (access(crxFile.c_str(), 0) == -1) continue;
                string oFile = site + sDoy + "0." + sYy + "o";
                /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + crxFile;
#endif
                std::system(cmd.c_str());
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the PBO observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetDailyObsPbo3): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                str.ToLower(sitName);
                string oFile = sitName + sDoy + "0." + sYy + "o";
                string dFile = sitName + sDoy + "0." + sYy + "d";
                if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    str.ToUpper(sitName);
                    string crxFile = sitName + "*" + sYyyy + sDoy + "0000_01D_15S_MO.crx";
                    string crxxFile = crxFile + ".*";
                    string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    string crxgzFile = crxFile + ".gz";
                    cmd = gzipFull + " -d -f " + crxgzFile;
                    std::system(cmd.c_str());
                    string changeFileName;
#ifdef _WIN32  /* for Windows */
                    changeFileName = "rename";
#else          /* for Linux or Mac */
                    changeFileName = "mv";
#endif
                    cmd = changeFileName + " " + crxFile + " " + dFile;
                    std::system(cmd.c_str());
                    bool isgz = true;
                    string crxzFile;

                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, oFile.c_str());
                    string localFile = tmpFile;
                    if (access(dFile.c_str(), 0) == -1)
                    {
                        /* extract '*.Z' */
                        crxzFile = crxFile + ".Z";
                        cmd = gzipFull + " -d -f " + crxzFile;
                        std::system(cmd.c_str());

                        cmd = changeFileName + " " + crxFile + " " + dFile;
                        std::system(cmd.c_str());
                        isgz = false;
                        if (access(dFile.c_str(), 0) == -1)
                        {
                            cout << "*** WARNING(FtpUtil::GetDailyObsPbo3): failed to download PBO daily observation file " << oFile << endl;

                            string url0 = url + '/' + crxxFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsPbo3): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                            continue;
                        }
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    if (access(oFile.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::GetDailyObsPbo3): successfully download PBO daily observation file " << oFile << endl;

                        if (isgz) crxxFile = crxgzFile;
                        else crxxFile = crxzFile;
                        string url0 = url + '/' + crxxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDailyObsPbo3): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::GetDailyObsPbo3): PBO daily observation file " << oFile <<
                    " has existed!" << endl;
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetDailyObsPbo3 */

/**
* @brief   : GetDailyObsPbo5 - to download Plate Boundary Observatory (PBO) RINEX daily observation (15s) files
*              (taking the union of RINEX version 3.xx, long name "crx" and RINEX version 2.xx, short name "d") 
*              according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsPbo5(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* creation of sub-directory ('daily') */
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    sprintf(tmpDir, "%s%c%s", dir, sep, "daily");
    string subDir = tmpDir;
    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, crx2rnxFull = fopt->crx2rnxFull, qr = fopt->qr;
    if (strlen(fopt->obsLst) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        string url = "ftp://data-out.unavco.org/pub/rinex3/obs/" + sYyyy + "/" + sDoy;
        string cutDirs = " --cut-dirs=5 ";
        /* it is OK for '*.Z' or '*.gz' format */
        string crxFile = "*" + sYyyy + sDoy + "0000_01D_15S_MO.crx";
        string crxxFile = crxFile + ".*";
        string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        string suffix = ".crx";
        vector<string> crxFiles;
        str.GetFilesAll(subDir, suffix, crxFiles);
        char sitName[MAXCHARS];
        for (int i = 0; i < crxFiles.size(); i++)
        {
            if (access(crxFiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipFull + " -d -f " + crxFiles[i];
                std::system(cmd.c_str());

                str.StrMid(sitName, crxFiles[i].c_str(), 0, 4);
                string site = sitName;
                str.ToLower(site);
                str.StrMid(sitName, crxFiles[i].c_str(), 0, (int)crxFiles[i].find_last_of('.'));
                crxFile = sitName;
                if (access(crxFile.c_str(), 0) == -1) continue;
                string oFile = site + sDoy + "0." + sYy + "o";
                /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + crxFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + crxFile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + crxFile;
#endif
                std::system(cmd.c_str());
            }
        }
        
        /* to download the observation files with short name "d" */
        url = "ftp://data-out.unavco.org/pub/rinex/obs/" + sYyyy + "/" + sDoy;
        /* it is OK for '*.Z' or '*.gz' format */
        string dFile = "*" + sDoy + "0." + sYy + "d";
        string dxFile = dFile + ".*";
        cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        suffix = "." + sYy + "d";
        vector<string> dFiles;
        str.GetFilesAll(subDir, suffix, dFiles);
        for (int i = 0; i < dFiles.size(); i++)
        {
            if (access(dFiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipFull + " -d -f " + dFiles[i];
                std::system(cmd.c_str());

                str.StrMid(sitName, dFiles[i].c_str(), 0, 4);
                string site = sitName;
                str.ToLower(site);
                dFile = site + sDoy + "0." + sYy + "d";
                if (access(dFile.c_str(), 0) == -1) continue;
                string oFile = site + sDoy + "0." + sYy + "o";
                if (access(oFile.c_str(), 0) == 0)
                {
                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + dFile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + dFile;
#endif
                    std::system(cmd.c_str());

                    continue;
                }
                /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                std::system(cmd.c_str());

                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + dFile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + dFile;
#endif
                std::system(cmd.c_str());
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the PBO observation file site-by-site */
        if (access(fopt->obsLst, 0) == 0)
        {
            ifstream sitLst(fopt->obsLst);
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetDailyObsPbo5): open site.list = " << fopt->obsLst << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                str.ToLower(sitName);
                string oFile = sitName + sDoy + "0." + sYy + "o";
                string dFile = sitName + sDoy + "0." + sYy + "d";
                if (access(oFile.c_str(), 0) == -1 && access(dFile.c_str(), 0) == -1)
                {
                    string url = "ftp://data-out.unavco.org/pub/rinex3/obs/" + sYyyy + "/" + sDoy;
                    string cutDirs = " --cut-dirs=5 ";
                    /* it is OK for '*.Z' or '*.gz' format */
                    str.ToUpper(sitName);
                    string crxFile = sitName + "*" + sYyyy + sDoy + "0000_01D_15S_MO.crx";
                    string crxxFile = crxFile + ".*";
                    string cmd = wgetFull + " " + qr + " -nH -A " + crxxFile + cutDirs + url;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    string crxgzFile = crxFile + ".gz";
                    cmd = gzipFull + " -d -f " + crxgzFile;
                    std::system(cmd.c_str());
                    string changeFileName;
#ifdef _WIN32  /* for Windows */
                    changeFileName = "rename";
#else          /* for Linux or Mac */
                    changeFileName = "mv";
#endif
                    cmd = changeFileName + " " + crxFile + " " + dFile;
                    std::system(cmd.c_str());

                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, oFile.c_str());
                    string localFile = tmpFile;
                    if (access(dFile.c_str(), 0) == -1)
                    {
                        /* extract '*.Z' */
                        string crxzFile = crxFile + ".Z";
                        cmd = gzipFull + " -d -f " + crxzFile;
                        std::system(cmd.c_str());

                        cmd = changeFileName + " " + crxFile + " " + dFile;
                        std::system(cmd.c_str());
                        if (access(dFile.c_str(), 0) == -1)
                        {
                            /* to download the observation files with short name "d" */
                            url = "ftp://data-out.unavco.org/pub/rinex/obs/" + sYyyy + "/" + sDoy;
                            /* it is OK for '*.Z' or '*.gz' format */
                            string dxFile = dFile + ".*";
                            string cmd = wgetFull + " " + qr + " -nH -A " + dxFile + cutDirs + url;
                            std::system(cmd.c_str());

                            string dgzFile = dFile + ".gz", dzFile = dFile + ".Z";
                            if (access(dgzFile.c_str(), 0) == 0)
                            {
                                /* extract '*.gz' */
                                cmd = gzipFull + " -d -f " + dgzFile;
                                std::system(cmd.c_str());

                                if (access(dFile.c_str(), 0) == 0 && access(dzFile.c_str(), 0) == 0)
                                {
                                    /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                                    cmd = "del " + dzFile;
#else           /* for Linux or Mac */
                                    cmd = "rm -rf " + dzFile;
#endif
                                    std::system(cmd.c_str());
                                }
                            }
                            if (access(dFile.c_str(), 0) == -1 && access(dzFile.c_str(), 0) == 0)
                            {
                                /* extract '*.Z' */
                                cmd = gzipFull + " -d -f " + dzFile;
                                std::system(cmd.c_str());
                            }
                            if (access(dFile.c_str(), 0) == -1)
                            {
                                cout << "*** WARNING(FtpUtil::GetDailyObsPbo5): failed to download PBO daily observation file " << oFile << endl;

                                string url0 = url + '/' + crxxFile;
                                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsPbo5): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                                url0 = url + '/' + dxFile;
                                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDailyObsPbo5): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());

                                continue;
                            }
                        }
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxFull + " " + dFile + " -f - > " + oFile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dFile + " | " + crx2rnxFull + " -f - > " + oFile;
#endif
                    std::system(cmd.c_str());

                    if (access(oFile.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::GetDailyObsPbo5): successfully download PBO daily observation file " << oFile << endl;

                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDailyObsPbo5): %s  ->  %s  OK\n", oFile.c_str(), localFile.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dFile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dFile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::GetDailyObsPbo5): PBO daily observation file " << oFile <<
                    " has existed!" << endl;
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetDailyObsPbo5 */

/**
* @brief   : GetNav - download daily GPS, GLONASS and mixed RINEX broadcast ephemeris files
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetNav(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string nTyp(fopt->navTyp);
    str.TrimSpace4String(nTyp);
    str.ToLower(nTyp);
    string navSys(fopt->navSys);
    str.TrimSpace4String(navSys);
    str.ToLower(navSys);
    string navAc = fopt->navAc;
    str.TrimSpace4String(navAc);
    str.ToUpper(navAc);
    if (nTyp == "daily")
    {
        /* creation of sub-directory ('daily') */
        char tmpDir[MAXSTRPATH] = { '\0' };
        char sep = (char)FILEPATHSEP;
        sprintf(tmpDir, "%s%c%s", dir, sep, "daily");
        string subDir = tmpDir;
        if (access(subDir.c_str(), 0) == -1)
        {
            /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + subDir;
#endif
            std::system(cmd.c_str());
        }

        /* change directory */
#ifdef _WIN32   /* for Windows */
        _chdir(subDir.c_str());
#else           /* for Linux or Mac */
        chdir(subDir.c_str());
#endif

        string navFile, nav0File;
        if (navSys == "gps")
        {
            /* GPS broadcast ephemeris file */
            navFile = "brdc" + sDoy + "0." + sYy + "n";
            nav0File = "brdc" + sDoy + "0." + sYy + "n";
        }
        else if (navSys == "glo")
        {
            /* GLONASS broadcast ephemeris file */
            navFile = "brdc" + sDoy + "0." + sYy + "g";
            nav0File = "brdc" + sDoy + "0." + sYy + "g";
        }
        else if (navSys == "mixed3")
        {
            /* multi-GNSS broadcast ephemeris file in RINEX 3.xx format */
            navFile = "BRDC00" + navAc + "_R_" + sYyyy + sDoy + "0000_01D_MN.rnx";
            nav0File = "brdm" + sDoy + "0." + sYy + "p";
        }
        else if (navSys == "mixed4")
        {
            /* multi-GNSS broadcast ephemeris file in RINEX 4.xx format */
            navFile = "BRD400DLR_S_" + sYyyy + sDoy + "0000_01D_MN.rnx";
            nav0File = "brd4" + sDoy + "0." + sYy + "p";
        }

        if (access(navFile.c_str(), 0) == -1 && access(nav0File.c_str(), 0) == -1)
        {
            string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
            string url, cutDirs = " --cut-dirs=6 ";
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_NAV] + "/" +
                sYyyy + "/brdc";
            else if (ftpName == "IGN")
            {
                url = _ftpArchive.IGN[IDX_NAV] + "/" + sYyyy + "/" + sDoy;
                cutDirs = " --cut-dirs=5 ";
            }
            else if (ftpName == "WHU")
            {
                if (yyyy >= 2020) url = _ftpArchive.WHU[IDX_NAV] + "/" + sYyyy + "/brdc";
                else
                {
                    if (navSys == "gps") url = _ftpArchive.WHU[IDX_NAV] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "n";
                    else if (navSys == "glo") url = _ftpArchive.WHU[IDX_NAV] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "g";
                    if (navSys == "mixed3") url = _ftpArchive.WHU[IDX_NAV] + "/" + sYyyy + "/" + sDoy + "/" + sYy + "p";
                }
                cutDirs = " --cut-dirs=7 ";
            }
            else url = _ftpArchive.CDDIS[IDX_NAV] + "/" + sYyyy + "/brdc";

            if (navSys == "mixed3")
            {
                if (navAc == "DLR")
                {
                    navFile = "BRDM00" + navAc + "_S_" + sYyyy + sDoy + "0000_01D_MN.rnx";
                    if (ftpName != "CDDIS" && ftpName != "WHU")
                    {
                        cerr << "*** ERROR(FtpUtil::GetNav): DLR mixed broadcast ephemeris file is ONLY downloaded from CDDIS or WHU FTP!" << endl;

                        return;
                    }
                }
                else if (navAc == "IGN")
                {
                    if (ftpName != "IGN")
                    {
                        cerr << "*** ERROR(FtpUtil::GetNav): IGN mixed broadcast ephemeris file is ONLY downloaded from IGN FTP!" << endl;

                        return;
                    }
                }
                else if (navAc == "GOP")
                {
                    url = "ftp://ftp.pecny.cz/LDC/orbits_brd/gop3/" + sYyyy;
                    cutDirs = " --cut-dirs=4 ";
                }
                else if (navAc == "WRD")
                {
                    url = "https://igs.bkg.bund.de/root_ftp/IGS/BRDC/" + sYyyy + "/" + sDoy;
                    cutDirs = " --cut-dirs=5 ";
                }
            }

            /* it is OK for '*.Z' or '*.gz' format */
            string cmd, url0, navxFile;
            if (navAc == "WRD")
            {
                string navgzFile = navFile + ".gz";
                url0 = url + "/" + navgzFile;
                cmd = wgetFull + " " + qr + " -nH " + cutDirs + url0;
            }
            else
            {
                navxFile = navFile + ".*";
                cmd = wgetFull + " " + qr + " -nH -A " + navxFile + cutDirs + url;
                url0 = url + "/" + navxFile;
            }
            std::system(cmd.c_str());

            string navgzFile = navFile + ".gz", navzFile = navFile + ".Z";
            bool isgz = false;
            if (access(navgzFile.c_str(), 0) == 0)
            {
                /* extract '*.gz' */
                cmd = gzipFull + " -d -f " + navgzFile;
                std::system(cmd.c_str());
                isgz = true;
            }
            else if (access(navzFile.c_str(), 0) == 0)
            {
                /* extract '*.Z' */
                cmd = gzipFull + " -d -f " + navzFile;
                std::system(cmd.c_str());
                isgz = false;
            }

            char tmpFile[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, nav0File.c_str());
            string localFile = tmpFile;
            if (access(navFile.c_str(), 0) == -1)
            {
                cout << "*** INFO(FtpUtil::GetNav): failed to download broadcast ephemeris file " << nav0File << endl;

                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetNav): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
            }

            if (navSys == "mixed3" || navSys == "mixed4")
            {
                string changeFileName;
#ifdef _WIN32  /* for Windows */
                changeFileName = "rename";
#else          /* for Linux or Mac */
                changeFileName = "mv";
#endif
                cmd = changeFileName + " " + navFile + " " + nav0File;
                std::system(cmd.c_str());
            }

            if (navAc == "GOP")
            {
                /* delete some temporary directories */
                string tmpDir = "log";
                if (access(tmpDir.c_str(), 0) == 0)
                {
#ifdef _WIN32  /* for Windows */
                    cmd = "rd /s /q " + tmpDir;
#else          /* for Linux or Mac */
                    cmd = "rm -rf " + tmpDir;
#endif
                    std::system(cmd.c_str());
                }
            }

            if (access(nav0File.c_str(), 0) == 0)
            {
                cout << "*** INFO(FtpUtil::GetNav): successfully download broadcast ephemeris file " << nav0File << endl;

                if (isgz) navxFile = navgzFile;
                else navxFile = navzFile;
                string url0 = url + '/' + navxFile;
                if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetNav): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
            }
        }
        else cout << "*** INFO(FtpUtil::GetNav): broadcast ephemeris file " << nav0File << " or " <<
            navFile << " has existed!" << endl;
    }
    else if (nTyp == "hourly")
    {
        /* creation of sub-directory ('hourly') */
        char tmpDir[MAXSTRPATH] = { '\0' };
        char sep = (char)FILEPATHSEP;
        sprintf(tmpDir, "%s%c%s", dir, sep, "hourly");
        string subDir = tmpDir;
        if (access(subDir.c_str(), 0) == -1)
        {
            /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + subDir;
#endif
            std::system(cmd.c_str());
        }

        string sitFile = fopt->navLst;
        /* download the broadcast ephemeris file site-by-site */
        if (access(sitFile.c_str(), 0) == 0)
        {
            ifstream sitLst(sitFile.c_str());
            if (!sitLst.is_open())
            {
                cerr << "*** ERROR(FtpUtil::GetHourlyObsMgex): open site.list = " << sitFile << " file failed, please check it" << endl;

                return;
            }

            string sitName;
            while (getline(sitLst, sitName))
            {
                if (sitName[0] == '#') continue;
                str.TrimSpace4String(sitName);
                if (sitName.size() != 4) continue;
                for (int i = 0; i < fopt->hhNav.size(); i++)
                {
                    string sHh = str.hh2str(fopt->hhNav[i]);
                    char tmpDir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpDir, "%s%c%s", subDir.c_str(), sep, sHh.c_str());
                    string sHhDir = tmpDir;
                    if (access(sHhDir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        string cmd = "mkdir " + sHhDir;
#else           /* for Linux or Mac */
                        string cmd = "mkdir -p " + sHhDir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(sHhDir.c_str());
#else           /* for Linux or Mac */
                    chdir(sHhDir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhNav[i] + 97;
                    char ch = ii;
                    string sch;
                    sch.push_back(ch);
                    str.ToLower(sitName);
                    string navnsFile = sitName + sDoy + sch + "." + sYy + "n";  /* short file name */
                    string navgsFile = sitName + sDoy + sch + "." + sYy + "g";
                    
                    str.ToUpper(sitName);
                    string navnlFile = sitName + "*_R_" + sYyyy + sDoy + sHh + "00_01H_GN.rnx";  /* long file name */
                    string navglFile = sitName + "*_R_" + sYyyy + sDoy + sHh + "00_01H_RN.rnx";
                    string navclFile = sitName + "*_R_" + sYyyy + sDoy + sHh + "00_01H_CN.rnx";
                    string navelFile = sitName + "*_R_" + sYyyy + sDoy + sHh + "00_01H_EN.rnx";
                    string navjlFile = sitName + "*_R_" + sYyyy + sDoy + sHh + "00_01H_JN.rnx";
                    string navilFile = sitName + "*_R_" + sYyyy + sDoy + sHh + "00_01H_IN.rnx";
                    string navmlFile = sitName + "*_R_" + sYyyy + sDoy + sHh + "00_01H_MN.rnx";
                    std::vector<string> navFiles;
                    if (navSys == "gps")
                    {
                        navFiles.push_back(navnsFile);
                        navFiles.push_back(navnlFile);
                    }
                    else if (navSys == "glo")
                    {
                        navFiles.push_back(navgsFile);
                        navFiles.push_back(navglFile);
                    }
                    else if (navSys == "bds") navFiles.push_back(navclFile);
                    else if (navSys == "gal") navFiles.push_back(navelFile);
                    else if (navSys == "qzs") navFiles.push_back(navjlFile);
                    else if (navSys == "irn") navFiles.push_back(navilFile);
                    else if (navSys == "mixed") navFiles.push_back(navmlFile);
                    else if (navSys == "all")
                    {
                        navFiles.push_back(navnsFile);
                        navFiles.push_back(navnlFile);
                        navFiles.push_back(navgsFile);
                        navFiles.push_back(navglFile);
                        navFiles.push_back(navclFile);
                        navFiles.push_back(navelFile);
                        navFiles.push_back(navjlFile);
                        navFiles.push_back(navilFile);
                        navFiles.push_back(navmlFile);
                    }

                    str.ToLower(sitName);
                    string navns0File = sitName + sDoy + sch + "." + sYy + "n";
                    string navgs0File = sitName + sDoy + sch + "." + sYy + "g";
                    string navnl0File = sitName + sDoy + sch + "." + sYy + "gn";
                    string navgl0File = sitName + sDoy + sch + "." + sYy + "rn";
                    string navcl0File = sitName + sDoy + sch + "." + sYy + "cn";
                    string navel0File = sitName + sDoy + sch + "." + sYy + "en";
                    string navjl0File = sitName + sDoy + sch + "." + sYy + "jn";
                    string navil0File = sitName + sDoy + sch + "." + sYy + "in";
                    string navml0File = sitName + sDoy + sch + "." + sYy + "mn";
                    std::vector<string> nav0Files;
                    if (navSys == "gps")
                    {
                        nav0Files.push_back(navns0File);
                        nav0Files.push_back(navnl0File);
                    }
                    else if (navSys == "glo")
                    {
                        nav0Files.push_back(navgs0File);
                        nav0Files.push_back(navgl0File);
                    }
                    else if (navSys == "bds") nav0Files.push_back(navcl0File);
                    else if (navSys == "gal") nav0Files.push_back(navel0File);
                    else if (navSys == "qzs") nav0Files.push_back(navjl0File);
                    else if (navSys == "irn") nav0Files.push_back(navil0File);
                    else if (navSys == "mixed") nav0Files.push_back(navml0File);
                    else if (navSys == "all")
                    {
                        nav0Files.push_back(navns0File);
                        nav0Files.push_back(navnl0File);
                        nav0Files.push_back(navgs0File);
                        nav0Files.push_back(navgl0File);
                        nav0Files.push_back(navcl0File);
                        nav0Files.push_back(navel0File);
                        nav0Files.push_back(navjl0File);
                        nav0Files.push_back(navil0File);
                        nav0Files.push_back(navml0File);
                    }
                    
                    for (int i = 0; i < navFiles.size(); i++)
                    {
                        string url, cutDirs = " --cut-dirs=7 ";
                        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBMH] + "/" +
                            sYyyy + "/" + sDoy + "/" + sHh;
                        else if (ftpName == "IGN")
                        {
                            url = _ftpArchive.IGN[IDX_OBMH] + "/" + sYyyy + "/" + sDoy;
                            cutDirs = " --cut-dirs=6 ";
                        }
                        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBMH] + "/" + 
                            sYyyy + "/" + sDoy + "/" + sHh;
                        else url = _ftpArchive.CDDIS[IDX_OBMH] + "/" + sYyyy + "/" + sDoy + "/" + sHh;

                        /* it is OK for '*.Z' or '*.gz' format */
                        string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
                        string navxFile = navFiles[i] + ".*";
                        string cmd = wgetFull + " " + qr + " -nH -A " + navxFile + cutDirs + url;
                        std::system(cmd.c_str());

                        /* extract '*.gz' */
                        string navgzFile = navFiles[i] + ".gz";
                        cmd = gzipFull + " -d -f " + navgzFile;
                        std::system(cmd.c_str());
                        string changeFileName;
#ifdef _WIN32  /* for Windows */
                        changeFileName = "rename";
#else          /* for Linux or Mac */
                        changeFileName = "mv";
#endif
                        cmd = changeFileName + " " + navFiles[i] + " " + nav0Files[i];
                        std::system(cmd.c_str());
                        bool isgz = true;
                        string navzFile;
                        char tmpFile[MAXSTRPATH] = { '\0' };
                        char sep = (char)FILEPATHSEP;
                        sprintf(tmpFile, "%s%c%s", sHhDir.c_str(), sep, nav0Files[i].c_str());
                        string localFile = tmpFile;
                        if (access(nav0Files[i].c_str(), 0) == -1)
                        {
                            /* extract '*.Z' */
                            navzFile = navFiles[i] + ".Z";
                            cmd = gzipFull + " -d -f " + navzFile;
                            std::system(cmd.c_str());

                            cmd = changeFileName + " " + navFiles[i] + " " + nav0Files[i];
                            std::system(cmd.c_str());
                            isgz = false;
                            if (access(nav0Files[i].c_str(), 0) == 0)
                            {
                                cout << "*** INFO(FtpUtil::GetNav): successfully download hourly broadcast ephemeris file " <<
                                    navFiles[i] << endl;

                                if (isgz) navxFile = navgzFile;
                                else navxFile = navzFile;
                                string url0 = url + '/' + navxFile;
                                if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetNav): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                                continue;
                            }
                            else
                            {
                                cout << "*** WARNING(FtpUtil::GetNav): failed to download hourly broadcast ephemeris file " <<
                                    navFiles[i] << endl;

                                string url0 = url + '/' + navxFile;
                                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetNav): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                            }
                        }
                        else
                        {
                            cout << "*** INFO(FtpUtil::GetNav): successfully download hourly broadcast ephemeris file " <<
                                navFiles[i] << endl;

                            if (isgz) navxFile = navgzFile;
                            else navxFile = navzFile;
                            string url0 = url + '/' + navxFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetNav): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());

                            continue;
                        }
                    }
                }
            }

            /* close 'site.list' */
            sitLst.close();
        }
    }
} /* end of GetNav */

/**
* @brief   : GetOrbClk - download IGS/MGEX precise orbit and clock files
* @param[I]: ts (start time)
* @param[I]: dirs (orbit and clock directories)
* @param[I]: prodType (1:real-time products; 2:ultra-rapid; 3:rapid; 4: IGS final; 5: MGEX final)
* @param[I]: ac (analysis center, i.e., 'igs', 'cod', et al.)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetOrbClk(gtime_t ts, std::vector<string> dirs, int prodType, string ac, const ftpopt_t *fopt)
{
    /* compute GPS week and day of week */
    TimeUtil tu;
    int wwww, dow;
    dow = tu.time2gpst(ts, &wwww, nullptr);
    StringUtil str;
    string sWwww = str.wwww2str(wwww);
    string sDow(to_string(dow));
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    string sYyyy = str.yyyy2str(yyyy);
    string sDoy = str.doy2str(doy);

    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
    str.TrimSpace4String(ac);
    str.ToLower(ac);
    if (prodType == PROD_REALTIME)          /* for CNES real-time orbit and clock products from CNES offline files  */
    {
        string sp3File = ac + sWwww + sDow + ".sp3", clkFile = ac + sWwww + sDow + ".clk";
        std::vector<string> sp3clkFiles = { sp3File, clkFile };
        string sp3gzFile = sp3File + ".gz", clkgzFile = clkFile + ".gz";
        std::vector<string> sp3clkgzFiles = { sp3gzFile, clkgzFile };
        for (int i = 0; i < sp3clkFiles.size(); i++)
        {
            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(dirs[i].c_str());
#else           /* for Linux or Mac */
            chdir(dirs[i].c_str());
#endif

            if (access(sp3clkFiles[i].c_str(), 0) == -1)
            {
                string url = "http://www.ppp-wizard.net/products/REAL_TIME/" + sp3clkgzFiles[i];
                string cmd = wgetFull + " " + qr + " -nH --cut-dirs=2 " + url;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                cmd = gzipFull + " -d -f " + sp3clkgzFiles[i];
                std::system(cmd.c_str());

                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", dirs[i].c_str(), sep, sp3clkFiles[i].c_str());
                string localFile = tmpFile;
                if (access(sp3clkFiles[i].c_str(), 0) == 0)
                {
                    if (i == 0) cout << "*** INFO(FtpUtil::GetOrbClk): successfully download CNES real-time precise orbit file " <<
                        sp3clkFiles[i] << endl;
                    else if (i == 1) cout << "*** INFO(FtpUtil::GetOrbClk): successfully download CNES real-time precise clock file " <<
                        sp3clkFiles[i] << endl;

                    
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetOrbClk): %s  ->  %s  OK\n", url.c_str(), localFile.c_str());
                }
                else
                {
                    if (i == 0) cout << "*** WARNING(FtpUtil::GetOrbClk): failed to download CNES real-time precise orbit file " <<
                        sp3clkFiles[i] << endl;
                    else if (i == 1) cout << "*** WARNING(FtpUtil::GetOrbClk): failed to download CNES real-time precise clock file " <<
                        sp3clkFiles[i] << endl;

                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetOrbClk): %s  ->  %s  failed\n", url.c_str(), localFile.c_str());
                }

                /* delete some temporary directories */
                std::vector<string> tmpDir = { "FORMAT_BIAIS_OFFI1", "FORMATBIAS_OFF_v1" };
                for (int i = 0; i < tmpDir.size(); i++)
                {
                    if (access(tmpDir[i].c_str(), 0) == 0)
                    {
#ifdef _WIN32  /* for Windows */
                        cmd = "rd /s /q " + tmpDir[i];
#else          /* for Linux or Mac */
                        cmd = "rm -rf " + tmpDir[i];
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
            else
            {
                if (i == 0) cout << "*** INFO(FtpUtil::GetOrbClk): CNES real-time precise orbit file " << sp3clkFiles[i] <<
                    " has existed!" << endl;
                else if (i == 1) cout << "*** INFO(FtpUtil::GetOrbClk): CNES real-time precise clock file " << sp3clkFiles[i] <<
                    " has existed!" << endl;
            }
        }
    }
    else if (prodType == PROD_ULTRA_RAPID)  /* for ultra-rapid orbit and clock products  */
    {
        /* change directory */
#ifdef _WIN32   /* for Windows */
        _chdir(dirs[0].c_str());
#else           /* for Linux or Mac */
        chdir(dirs[0].c_str());
#endif

        string url, cutDirs, acName, acFile;
        int idx = 0;
        if (ac == "esa_u")       /* ESA */
        {
            url = "http://navigation-office.esa.int/products/gnss-products/" + sWwww;
            cutDirs = " --cut-dirs=3 ";
            acFile = "esu";
            acName = "ESA";
            idx = 0;
        }
        else if (ac == "gfz_u")  /* GFZ */
        {
            url = "ftp://ftp.gfz-potsdam.de/pub/GNSS/products/ultra/w" + sWwww;
            cutDirs = " --cut-dirs=5 ";
            acFile = "gfu";
            acName = "GFZ";
            idx = 1;
        }
        else if (ac == "igs_u")  /* IGS */
        {
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_SP3] + "/" + sWwww;
            else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_SP3] + "/" + sWwww;
            else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_SP3] + "/" + sWwww;
            else url = _ftpArchive.CDDIS[IDX_SP3] + "/" + sWwww;
            cutDirs = " --cut-dirs=4 ";
            acFile = "igu";
            acName = "IGS";
            idx = 2;
        }
        else if (ac == "whu_u")  /* WHU */
        {
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_SP3M] + "/" + sWwww;
            else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_SP3M] + "/" + sWwww;
            else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_SP3M] + "/" + sWwww;
            else url = _ftpArchive.CDDIS[IDX_SP3M] + "/" + sWwww;
            cutDirs = " --cut-dirs=5 ";
            acName = "WHU";
            acFile = "wuu";
            idx = 3;
        }

        for (int i = 0; i < fopt->hhOrbClk[idx].size(); i++)
        {
            string sHh = str.hh2str(fopt->hhOrbClk[idx][i]);
            string sp3File = acFile + sWwww + sDow + "_" + sHh + ".sp3";
            if (access(sp3File.c_str(), 0) == -1)
            {
                string cmd, url0, sp3zFile, sp3gzFile, sp3xFile;
                bool isgz = false;
                if (ac == "esa_u")  /* ESA */
                {
                    sp3zFile = sp3File + ".Z";
                    url0 = url + "/" + sp3zFile;
                    cmd = wgetFull + " " + qr + " -nH " + cutDirs + url0;
                    std::system(cmd.c_str());

                    if (access(sp3zFile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipFull + " -d -f " + sp3zFile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }
                    else
                    {
                        sp3gzFile = sp3File + ".gz";
                        url0 = url + "/" + sp3gzFile;
                        cmd = wgetFull + " " + qr + " -nH " + cutDirs + url0;
                        std::system(cmd.c_str());

                        if (access(sp3gzFile.c_str(), 0) == 0)
                        {
                            /* extract '*.gz' */
                            cmd = gzipFull + " -d -f " + sp3gzFile;
                            std::system(cmd.c_str());
                            isgz = true;
                        }
                    }
                }
                else if (ac == "whu_u")  /* WHU */
                {
                    string sp30File = "WUM0MGXULA_" + sYyyy + sDoy + sHh + "00_01D_*_ORB.SP3";
                    /* it is OK for '*.Z' or '*.gz' format */
                    sp3xFile = sp30File + ".*";
                    cmd = wgetFull + " " + qr + " -nH -A " + sp3xFile + cutDirs + url;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    sp3gzFile = sp30File + ".gz";
                    cmd = gzipFull + " -d -f " + sp3gzFile;
                    std::system(cmd.c_str());
                    string changeFileName;
#ifdef _WIN32  /* for Windows */
                    changeFileName = "rename";
#else          /* for Linux or Mac */
                    changeFileName = "mv";
#endif
                    cmd = changeFileName + " " + sp30File + " " + sp3File;
                    std::system(cmd.c_str());
                    isgz = true;
                    if (access(sp3File.c_str(), 0) == -1)
                    {
                        /* extract '*.Z' */
                        sp3zFile = sp30File + ".Z";
                        cmd = gzipFull + " -d -f " + sp3zFile;
                        std::system(cmd.c_str());

                        cmd = changeFileName + " " + sp30File + " " + sp3File;
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    url0 = url + '/' + sp3xFile;
                }
                else
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    sp3xFile = sp3File + ".*";
                    cmd = wgetFull + " " + qr + " -nH -A " + sp3xFile + cutDirs + url;
                    std::system(cmd.c_str());

                    sp3gzFile = sp3File + ".gz";
                    sp3zFile = sp3File + ".Z";
                    if (access(sp3gzFile.c_str(), 0) == 0)
                    {
                        /* extract '*.gz' */
                        cmd = gzipFull + " -d -f " + sp3gzFile;
                        std::system(cmd.c_str());
                        isgz = true;
                    }
                    if (access(sp3File.c_str(), 0) == -1 && access(sp3zFile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipFull + " -d -f " + sp3zFile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    url0 = url + '/' + sp3xFile;
                }

                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", dirs[0].c_str(), sep, sp3File.c_str());
                string localFile = tmpFile;
                if (access(sp3File.c_str(), 0) == 0)
                {
                    cout << "*** INFO(FtpUtil::GetOrbClk): successfully download " << acName << " ultra-rapid precise orbit file " <<
                        sp3File << endl;

                    if (isgz) sp3xFile = sp3gzFile;
                    else sp3xFile = sp3zFile;
                    url0 = url + '/' + sp3xFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetOrbClk): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                }
                else
                {
                    cout << "*** WARNING(FtpUtil::GetOrbClk): failed to download " << acName << " ultra-rapid precise orbit file " <<
                        sp3File << endl;

                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetOrbClk): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                }

                if (ac == "igs_u")
                {
                    /* delete some temporary directories */
                    string tmpDir = "repro3";
                    if (access(tmpDir.c_str(), 0) == 0)
                    {
#ifdef _WIN32  /* for Windows */
                        cmd = "rd /s /q " + tmpDir;
#else          /* for Linux or Mac */
                        cmd = "rm -rf " + tmpDir;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
            else cout << "*** INFO(FtpUtil::GetOrbClk): " << acName << " ultra-rapid precise orbit file " << sp3File <<
                " has existed!" << endl;
        }
    }
    else if (prodType == PROD_RAPID)  /* for rapid orbit and clock products  */
    {
        string url, cutDirs, acName;
        string sp3File, clkFile;
        if (ac == "cod_r")  /* CODE */
        {
            sp3File = "COD" + sWwww + sDow + ".EPH_M";
            clkFile = "COD" + sWwww + sDow + ".CLK_M";
            url = "ftp://ftp.aiub.unibe.ch/CODE/" + sYyyy + "_M";
            cutDirs = " --cut-dirs=2 ";
            acName = "CODE";
        }
        else if (ac == "emr_r")  /* NRCan */
        {
            sp3File = "emr" + sWwww + sDow + ".sp3";
            clkFile = "emr" + sWwww + sDow + ".clk";
            url = "ftp://rtopsdata1.geod.nrcan.gc.ca/gps/products/rapid/" + sWwww;
            cutDirs = " --cut-dirs=4 ";
            acName = "NRCan";
        }
        else if (ac == "esa_r")  /* ESA */
        {
            sp3File = "esr" + sWwww + sDow + ".sp3";
            clkFile = "esr" + sWwww + sDow + ".clk";
            url = "http://navigation-office.esa.int/products/gnss-products/" + sWwww;
            cutDirs = " --cut-dirs=3 ";
            acName = "ESA";
        }
        else if (ac == "gfz_r")  /* GFZ */
        {
            sp3File = "gfz" + sWwww + sDow + ".sp3";
            clkFile = "gfz" + sWwww + sDow + ".clk";
            url = "ftp://ftp.gfz-potsdam.de/pub/GNSS/products/rapid/w" + sWwww;
            cutDirs = " --cut-dirs=5 ";
            acName = "GFZ";
        }
        else if (ac == "igs_r")  /* IGS */
        {
            sp3File = "igr" + sWwww + sDow + ".sp3";
            clkFile = "igr" + sWwww + sDow + ".clk";
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_SP3] + "/" + sWwww;
            else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_SP3] + "/" + sWwww;
            else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_SP3] + "/" + sWwww;
            else url = _ftpArchive.CDDIS[IDX_SP3] + "/" + sWwww;
            cutDirs = " --cut-dirs=4 ";
            acName = "IGS";
        }

        std::vector<string> sp3clkFiles = { sp3File, clkFile };
        string sp3gzFile = sp3File + ".gz", clkgzFile = clkFile + ".gz";
        std::vector<string> sp3clkgzFiles = { sp3gzFile, clkgzFile };
        string sp3zFile = sp3File + ".Z", clkzFile = clkFile + ".Z";
        std::vector<string> sp3clkzFiles = { sp3zFile, clkzFile };
        string sp3xFile = sp3File + ".*", clkxFile = clkFile + ".*";
        std::vector<string> sp3clkxFiles = { sp3xFile, clkxFile };
        for (int i = 0; i < sp3clkFiles.size(); i++)
        {
            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(dirs[i].c_str());
#else           /* for Linux or Mac */
            chdir(dirs[i].c_str());
#endif

            if (access(sp3clkFiles[i].c_str(), 0) == -1)
            {
                string cmd, url0;
                bool isgz = false;
                if (ac == "esa_r")  /* ESA */
                {
                    url0 = url + "/" + sp3clkzFiles[i];
                    cmd = wgetFull + " " + qr + " -nH " + cutDirs + url0;
                    std::system(cmd.c_str());

                    if (access(sp3zFile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipFull + " -d -f " + sp3clkzFiles[i];
                        std::system(cmd.c_str());
                        isgz = false;
                    }
                    else
                    {
                        url0 = url + "/" + sp3clkgzFiles[i];
                        cmd = wgetFull + " " + qr + " -nH " + cutDirs + url0;
                        std::system(cmd.c_str());

                        if (access(sp3clkgzFiles[i].c_str(), 0) == 0)
                        {
                            /* extract '*.gz' */
                            cmd = gzipFull + " -d -f " + sp3clkgzFiles[i];
                            std::system(cmd.c_str());
                            isgz = true;
                        }
                    }
                }
                else
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    cmd = wgetFull + " " + qr + " -nH -A " + sp3clkxFiles[i] + cutDirs + url;
                    std::system(cmd.c_str());

                    if (access(sp3clkgzFiles[i].c_str(), 0) == 0)
                    {
                        /* extract '*.gz' */
                        cmd = gzipFull + " -d -f " + sp3clkgzFiles[i];
                        std::system(cmd.c_str());
                        isgz = true;

                        if (access(sp3clkFiles[i].c_str(), 0) == 0 && access(sp3clkzFiles[i].c_str(), 0) == 0)
                        {
                            /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + sp3clkzFiles[i];
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + sp3clkzFiles[i];
#endif
                            std::system(cmd.c_str());
                        }
                    }
                    if (access(sp3clkFiles[i].c_str(), 0) == -1 && access(sp3clkzFiles[i].c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipFull + " -d -f " + sp3clkzFiles[i];
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    url0 = url + "/" + sp3clkxFiles[i];
                }

                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", dirs[i].c_str(), sep, sp3clkFiles[i].c_str());
                string localFile = tmpFile;
                if (access(sp3clkFiles[i].c_str(), 0) == 0)
                {
                    if (i == 0) cout << "*** INFO(FtpUtil::GetOrbClk): successfully download " << acName << " rapid precise orbit file " <<
                        sp3clkFiles[i] << endl;
                    else if (i == 1) cout << "*** INFO(FtpUtil::GetOrbClk): successfully download " << acName << " rapid precise clock file " <<
                        sp3clkFiles[i] << endl;

                    string sp3clkxFile;
                    if (isgz) sp3clkxFile = sp3clkgzFiles[i];
                    else sp3clkxFile = sp3clkzFiles[i];
                    string url0 = url + '/' + sp3clkxFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetOrbClk): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                }
                else
                {
                    if (i == 0) cout << "*** WARNING(FtpUtil::GetOrbClk): failed to download " << acName << " rapid precise orbit file " <<
                        sp3clkFiles[i] << endl;
                    else if (i == 1) cout << "*** WARNING(FtpUtil::GetOrbClk): failed to download " << acName << " rapid precise clock file " <<
                        sp3clkFiles[i] << endl;

                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetOrbClk): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                }

                if (ac == "emr_r" || ac == "igs_r")
                {
                    /* delete some temporary directories */
                    string tmpDir;
                    if (ac == "emr_r") tmpDir = "dcm";
                    else if (ac == "igs_r") tmpDir = "repro3";
                    if (access(tmpDir.c_str(), 0) == 0)
                    {
#ifdef _WIN32  /* for Windows */
                        cmd = "rd /s /q " + tmpDir;
#else          /* for Linux or Mac */
                        cmd = "rm -rf " + tmpDir;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
            else
            {
                if (i == 0) cout << "*** INFO(FtpUtil::GetOrbClk): " << acName << " rapid precise orbit file " << sp3clkFiles[i] <<
                        " has existed!" << endl;
                else if (i == 1) cout << "*** INFO(FtpUtil::GetOrbClk): " << acName << " rapid precise clock file " << sp3clkFiles[i] <<
                    " has existed!" << endl;
            }
        }
    }
    else if (prodType == PROD_FINAL_IGS)  /* for IGS final orbit and clock products  */
    {
        string sp3File = ac + sWwww + sDow + ".sp3";
        string clkFile = ac + sWwww + sDow + ".clk";
        string acName;
        if (ac == "cod")
        {
            sp3File = ac + sWwww + sDow + ".eph";
            clkFile = ac + sWwww + sDow + ".clk_05s";
            acName = "CODE";
        }
        else if (ac == "igs")
        {
            sp3File = ac + sWwww + sDow + ".sp3";
            clkFile = ac + sWwww + sDow + ".clk_30s";
            acName = "IGS";
        }
        else if (ac == "emr") acName = "NRCan";
        else if (ac == "esa") acName = "ESA";
        else if (ac == "gfz") acName = "GFZ";
        else if (ac == "grg") acName = "CNES";
        else if (ac == "jpl") acName = "JPL";
        else if (ac == "mit") acName = "MIT";

        std::vector<string> sp3clkFiles = { sp3File, clkFile };
        string sp3gzFile = sp3File + ".gz", clkgzFile = clkFile + ".gz";
        std::vector<string> sp3clkgzFiles = { sp3gzFile, clkgzFile };
        string sp3zFile = sp3File + ".Z", clkzFile = clkFile + ".Z";
        std::vector<string> sp3clkzFiles = { sp3zFile, clkzFile };
        string sp3xFile = sp3File + ".*", clkxFile = clkFile + ".*";
        std::vector<string> sp3clkxFiles = { sp3xFile, clkxFile };

        string url, cutDirs = " --cut-dirs=4 ";
        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_SP3] + "/" + sWwww;
        else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_SP3] + "/" + sWwww;
        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_SP3] + "/" + sWwww;
        else url = _ftpArchive.CDDIS[IDX_SP3] + "/" + sWwww;
        for (int i = 0; i < sp3clkFiles.size(); i++)
        {
            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(dirs[i].c_str());
#else           /* for Linux or Mac */
            chdir(dirs[i].c_str());
#endif

            if (access(sp3clkFiles[i].c_str(), 0) == -1)
            {
                /* it is OK for '*.Z' or '*.gz' format */
                string cmd = wgetFull + " " + qr + " -nH -A " + sp3clkxFiles[i] + cutDirs + url;
                std::system(cmd.c_str());

                bool isgz = false;
                if (access(sp3clkgzFiles[i].c_str(), 0) == 0)
                {
                    /* extract '*.gz' */
                    cmd = gzipFull + " -d -f " + sp3clkgzFiles[i];
                    std::system(cmd.c_str());
                    isgz = true;
                }
                if (access(sp3clkFiles[i].c_str(), 0) == -1 && access(sp3clkzFiles[i].c_str(), 0) == 0)
                {
                    /* extract '*.Z' */
                    cmd = gzipFull + " -d -f " + sp3clkzFiles[i];
                    std::system(cmd.c_str());
                    isgz = false;
                }

                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", dirs[i].c_str(), sep, sp3clkFiles[i].c_str());
                string localFile = tmpFile;
                if (access(sp3clkFiles[i].c_str(), 0) == 0)
                {
                    if (i == 0) cout << "*** INFO(FtpUtil::GetOrbClk): successfully download " << acName << " final precise orbit file " <<
                        sp3clkFiles[i] << endl;
                    else if (i == 1) cout << "*** INFO(FtpUtil::GetOrbClk): successfully download " << acName << " final precise clock file " <<
                        sp3clkFiles[i] << endl;

                    string sp3clkxFile;
                    if (isgz) sp3clkxFile = sp3clkgzFiles[i];
                    else sp3clkxFile = sp3clkzFiles[i];
                    string url0 = url + '/' + sp3clkxFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetOrbClk): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                }
                else
                {
                    if (i == 0) cout << "*** WANING(FtpUtil::GetOrbClk): failed to download " << acName << " final precise orbit file " <<
                        sp3clkFiles[i] << endl;
                    else if (i == 1) cout << "*** WARNING(FtpUtil::GetOrbClk): failed to download " << acName << " final precise clock file " <<
                        sp3clkFiles[i] << endl;

                    string url0 = url + '/' + sp3clkxFiles[i];
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetOrbClk): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                }

                /* delete some temporary directories */
                string tmpDir = "repro3";
                if (access(tmpDir.c_str(), 0) == 0)
                {
#ifdef _WIN32  /* for Windows */
                    cmd = "rd /s /q " + tmpDir;
#else          /* for Linux or Mac */
                    cmd = "rm -rf " + tmpDir;
#endif
                    std::system(cmd.c_str());
                }
            }
            else
            {
                if (i == 0) cout << "*** INFO(FtpUtil::GetOrbClk): " << acName << " final precise orbit file " << sp3clkFiles[i] <<
                    " has existed!" << endl;
                else if (i == 1) cout << "*** INFO(FtpUtil::GetOrbClk): " << acName << " final precise clock file " << sp3clkFiles[i] <<
                    " has existed!" << endl;
            }
        }
    }
    else if (prodType == PROD_FINAL_MGEX)  /* for MGEX final orbit and clock products */
    {
        string sp3File, clkFile, sp30File, clk0File, acName;
        if (ac == "cod_m")
        {
            sp3File = "COD0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_ORB.SP3";
            clkFile = "COD0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_CLK.CLK";
            sp30File = "com" + sWwww + sDow + ".sp3";
            clk0File = "com" + sWwww + sDow + ".clk";
            acName = "CODE";
        }
        else if (ac == "gfz_m")
        {
            sp3File = "GFZ0MGXRAP_" + sYyyy + sDoy + "0000_01D_" + "*_ORB.SP3";
            clkFile = "GFZ0MGXRAP_" + sYyyy + sDoy + "0000_01D_" + "*_CLK.CLK";
            sp30File = "gbm" + sWwww + sDow + ".sp3";
            clk0File = "gbm" + sWwww + sDow + ".clk";
            acName = "GFZ";
        }
        else if (ac == "grg_m")
        {
            sp3File = "GRG0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_ORB.SP3";
            clkFile = "GRG0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_CLK.CLK";
            sp30File = "grm" + sWwww + sDow + ".sp3";
            clk0File = "grm" + sWwww + sDow + ".clk";
            acName = "CNES";
        }
        else if (ac == "whu_m")
        {
            sp3File = "WUM0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_ORB.SP3";
            clkFile = "WUM0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_CLK.CLK";
            sp30File = "wum" + sWwww + sDow + ".sp3";
            clk0File = "wum" + sWwww + sDow + ".clk";
            acName = "WHU";
        }

        std::vector<string> sp3clkFiles = { sp3File, clkFile };
        std::vector<string> sp3clk0Files = { sp30File, clk0File };
        string sp3gzFile = sp3File + ".gz", clkgzFile = clkFile + ".gz";
        std::vector<string> sp3clkgzFiles = { sp3gzFile, clkgzFile };
        string sp3zFile = sp3File + ".Z", clkzFile = clkFile + ".Z";
        std::vector<string> sp3clkzFiles = { sp3zFile, clkzFile };
        string sp3xFile = sp3File + ".*", clkxFile = clkFile + ".*";
        std::vector<string> sp3clkxFiles = { sp3xFile, clkxFile };

        string url, cutDirs = " --cut-dirs=5 ";
        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_SP3M] + "/" + sWwww;
        else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_SP3M] + "/" + sWwww;
        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_SP3M] + "/" + sWwww;
        else url = _ftpArchive.CDDIS[IDX_SP3M] + "/" + sWwww;
        for (int i = 0; i < sp3clkFiles.size(); i++)
        {
            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(dirs[i].c_str());
#else           /* for Linux or Mac */
            chdir(dirs[i].c_str());
#endif

            if (access(sp3clk0Files[i].c_str(), 0) == -1)
            {
                /* it is OK for '*.Z' or '*.gz' format */
                string cmd = wgetFull + " " + qr + " -nH -A " + sp3clkxFiles[i] + cutDirs + url;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                cmd = gzipFull + " -d -f " + sp3clkgzFiles[i];
                std::system(cmd.c_str());
                string changeFileName;
#ifdef _WIN32  /* for Windows */
                changeFileName = "rename";
#else          /* for Linux or Mac */
                changeFileName = "mv";
#endif
                cmd = changeFileName + " " + sp3clkFiles[i] + " " + sp3clk0Files[i];
                std::system(cmd.c_str());
                bool isgz = true;

                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", dirs[i].c_str(), sep, sp3clk0Files[i].c_str());
                string localFile = tmpFile;
                if (access(sp3clk0Files[i].c_str(), 0) == -1)
                {
                    /* extract '*.Z' */
                    cmd = gzipFull + " -d -f " + sp3clkzFiles[i];
                    std::system(cmd.c_str());

                    cmd = changeFileName + " " + sp3clkFiles[i] + " " + sp3clk0Files[i];
                    std::system(cmd.c_str());

                    isgz = false;

                    if (access(sp3clk0Files[i].c_str(), 0) == -1)
                    {
                        if (i == 0) cout << "*** WARNING(FtpUtil::GetOrbClk): failed to download " << acName << " MGEX final precise orbit file " <<
                            sp3clk0Files[i] << endl;
                        else if (i == 1) cout << "*** WARNING(FtpUtil::GetOrbClk): failed to download " << acName << " MGEX final precise clock file " <<
                            sp3clk0Files[i] << endl;

                        string url0 = url + '/' + sp3clkxFiles[i];
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetOrbClk): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                    }
                }

                if (access(sp3clk0Files[i].c_str(), 0) == 0)
                {
                    if (i == 0) cout << "*** INFO(FtpUtil::GetOrbClk): successfully download " << acName << " MGEX final precise orbit file " <<
                        sp3clk0Files[i] << endl;
                    else if (i == 1) cout << "*** INFO(FtpUtil::GetOrbClk): successfully download " << acName << " MGEX final precise clock file " <<
                        sp3clk0Files[i] << endl;

                    string sp3clkxFile;
                    if (isgz) sp3clkxFile = sp3clkgzFiles[i];
                    else sp3clkxFile = sp3clkzFiles[i];
                    string url0 = url + '/' + sp3clkxFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetOrbClk): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                }
            }
            else
            {
                if (i == 0) cout << "*** INFO(FtpUtil::GetOrbClk): " << acName << " MGEX final precise orbit file " << sp3clk0Files[i] <<
                    " has existed!" << endl;
                else if (i == 1) cout << "*** INFO(FtpUtil::GetOrbClk): " << acName << " MGEX final precise clock file " << sp3clk0Files[i] <<
                    " has existed!" << endl;
            }
        }
    }
} /* end of GetOrbClk */

/**
* @brief   : GetEop - download earth rotation parameter files
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetEop(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* compute GPS week and day of week */
    TimeUtil tu;
    int wwww, dow;
    dow = tu.time2gpst(ts, &wwww, nullptr);
    StringUtil str;
    string sWwww = str.wwww2str(wwww);
    string sDow(to_string(dow));

    string ac(fopt->eopAc);
    str.TrimSpace4String(ac);
    str.ToLower(ac);

    /* esu: ESA ultra-rapid EOP products
       gfu: GFZ ultra-rapid EOP products
       igu: IGS ultra-rapid EOP products
    */
    std::vector<string> ultraAc = { "esa_u", "gfz_u", "igs_u" };
    bool isUltra = false;
    int i = 0;
    for (i = 0; i < ultraAc.size(); i++)
    {
        if (ac == ultraAc[i])
        {
            isUltra = true;
            break;
        }
    }

    /* cod: CODE final EOP products
       emr: NRCan final EOP products
       esa: ESA final EOP products
       gfz: GFZ final EOP products
       grg: CNES final EOP products
       igs: IGS final EOP products
       jpl: JPL final EOP products
    */
    std::vector<string> finalAcIGS = { "cod", "emr", "esa", "gfz", "grg", "igs", "jpl", "mit" };
    bool isIGS = false;
    for (int i = 0; i < finalAcIGS.size(); i++)
    {
        if (ac == finalAcIGS[i])
        {
            isIGS = true;
            break;
        }
    }

    if (!isUltra && !isIGS)
    {
        cerr << "*** ERROR(FtpUtil::GetEop): Please check the setting of the 2nd item of getEop in configure file!" << endl;

        return;
    }

    /* creation of sub-directory */
    string subDir = dir;
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    if (isUltra)
    {
        sprintf(tmpDir, "%s%c%s", dir, sep, "ultra_rapid");
        subDir = tmpDir;
    }
    else if (isIGS)
    {
        sprintf(tmpDir, "%s%c%s", dir, sep, "final");
        subDir = tmpDir;
    }

    /* If the directory does not exist, creat it */
    if (access(subDir.c_str(), 0) == -1)
    {
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
    if (isUltra)  /* for ultra-rapid EOP products  */
    {
        string url, cutDirs, acName, acFile;
        if (ac == "esa_u")       /* ESA */
        {
            url = "http://navigation-office.esa.int/products/gnss-products/" + sWwww;
            cutDirs = " --cut-dirs=3 ";
            acFile = "esu";
            acName = "ESA";
        }
        else if (ac == "gfz_u")  /* GFZ */
        {
            url = "ftp://ftp.gfz-potsdam.de/pub/GNSS/products/ultra/w" + sWwww;
            cutDirs = " --cut-dirs=5 ";
            acFile = "gfu";
            acName = "GFZ";
        }
        else if (ac == "igs_u")  /* IGS */
        {
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_EOP] + "/" + sWwww;
            else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_EOP] + "/" + sWwww;
            else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_EOP] + "/" + sWwww;
            else url = _ftpArchive.CDDIS[IDX_SP3] + "/" + sWwww;
            cutDirs = " --cut-dirs=4 ";
            acFile = "igu";
            acName = "IGS";
        }

        for (int i = 0; i < fopt->hhEop.size(); i++)
        {
            string sHh = str.hh2str(fopt->hhEop[i]);
            string eopFile = acFile + sWwww + sDow + "_" + sHh + ".erp";
            if (access(eopFile.c_str(), 0) == -1)
            {
                string cmd, url0, eopzFile, eopgzFile, eopxFile;
                bool isgz = false;
                if (ac == "esa_u")  /* ESA */
                {
                    eopzFile = eopFile + ".Z";
                    url0 = url + "/" + eopzFile;
                    cmd = wgetFull + " " + qr + " -nH " + cutDirs + url0;
                    std::system(cmd.c_str());

                    if (access(eopzFile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipFull + " -d -f " + eopzFile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }
                    else
                    {
                        eopgzFile = eopFile + ".gz";
                        url0 = url + "/" + eopgzFile;
                        cmd = wgetFull + " " + qr + " -nH " + cutDirs + url0;
                        std::system(cmd.c_str());

                        if (access(eopgzFile.c_str(), 0) == 0)
                        {
                            /* extract '*.gz' */
                            cmd = gzipFull + " -d -f " + eopgzFile;
                            std::system(cmd.c_str());
                            isgz = true;
                        }
                    }
                }
                else
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    eopxFile = eopFile + ".*";
                    cmd = wgetFull + " " + qr + " -nH -A " + eopxFile + cutDirs + url;
                    std::system(cmd.c_str());

                    eopgzFile = eopFile + ".gz";
                    eopzFile = eopFile + ".Z";
                    if (access(eopgzFile.c_str(), 0) == 0)
                    {
                        /* extract '*.gz' */
                        cmd = gzipFull + " -d -f " + eopgzFile;
                        std::system(cmd.c_str());
                        isgz = true;
                    }
                    if (access(eopFile.c_str(), 0) == -1 && access(eopzFile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipFull + " -d -f " + eopzFile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    url0 = url + '/' + eopxFile;
                }

                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, eopFile.c_str());
                string localFile = tmpFile;
                if (access(eopFile.c_str(), 0) == 0)
                {
                    cout << "*** INFO(FtpUtil::GetEop): successfully download " << acName << " ultra-rapid EOP file " << eopFile << endl;

                    if (isgz) eopxFile = eopgzFile;
                    else eopxFile = eopzFile;
                    string url0 = url + '/' + eopxFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetEop): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                }
                else
                {
                    cout << "*** INFO(FtpUtil::GetEop): failed to download " << acName << " ultra-rapid EOP file " << eopFile << endl;

                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetEop): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                }

                if (ac == "igs_u")
                {
                    /* delete some temporary directories */
                    string tmpDir = "repro3";
                    if (access(tmpDir.c_str(), 0) == 0)
                    {
#ifdef _WIN32  /* for Windows */
                        cmd = "rd /s /q " + tmpDir;
#else          /* for Linux or Mac */
                        cmd = "rm -rf " + tmpDir;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
            else cout << "*** INFO(FtpUtil::GetEop): " << acName << " ultra-rapid EOP file " << eopFile << " has existed!" << endl;
        }
    }
    else if (isIGS)  /* for IGS final EOP products  */
    {
        string acName;
        if (ac == "cod") acName = "CODE";
        else if (ac == "emr") acName = "NRCan";
        else if (ac == "esa") acName = "ESA";
        else if (ac == "gfz") acName = "GFZ";
        else if (ac == "grg") acName = "CNES";
        else if (ac == "igs") acName = "IGS";
        else if (ac == "jpl") acName = "JPL";
        else if (ac == "mit") acName = "MIT";

        string eopFile = ac + sWwww + "7.erp";
        if (access(eopFile.c_str(), 0) == -1)
        {
            /* download the EOP file */
            string url, cutDirs = " --cut-dirs=4 ";
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_EOP] + "/" + sWwww;
            else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_EOP] + "/" + sWwww;
            else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_EOP] + "/" + sWwww;
            else url = _ftpArchive.CDDIS[IDX_EOP] + "/" + sWwww;

            /* it is OK for '*.Z' or '*.gz' format */
            string eopxFile = eopFile + ".*";
            string cmd = wgetFull + " " + qr + " -nH -A " + eopxFile + cutDirs + url;
            std::system(cmd.c_str());

            string eopgzFile = eopFile + ".gz", eopzFile = eopFile + ".Z";
            bool isgz = false;
            if (access(eopgzFile.c_str(), 0) == 0)
            {
                /* extract '*.gz' */
                cmd = gzipFull + " -d -f " + eopgzFile;
                std::system(cmd.c_str());
                isgz = true;
            }
            if (access(eopFile.c_str(), 0) == -1 && access(eopzFile.c_str(), 0) == 0)
            {
                /* extract '*.Z' */
                cmd = gzipFull + " -d -f " + eopzFile;
                std::system(cmd.c_str());
                isgz = false;
            }

            char tmpFile[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, eopFile.c_str());
            string localFile = tmpFile;
            if (access(eopFile.c_str(), 0) == 0)
            {
                cout << "*** INFO(FtpUtil::GetEop): successfully download " << acName << " final EOP file " << eopFile << endl;

                if (isgz) eopxFile = eopgzFile;
                else eopxFile = eopzFile;
                string url0 = url + '/' + eopxFile;
                if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetEop): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
            }
            else
            {
                cout << "*** INFO(FtpUtil::GetEop): failed to download " << acName << " final EOP file " << eopFile << endl;

                string url0 = url + '/' + eopxFile;
                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetEop): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
            }

            /* delete some temporary directories */
            string tmpDir = "repro3";
            if (access(tmpDir.c_str(), 0) == 0)
            {
#ifdef _WIN32  /* for Windows */
                cmd = "rd /s /q " + tmpDir;
#else          /* for Linux or Mac */
                cmd = "rm -rf " + tmpDir;
#endif
                std::system(cmd.c_str());
            }
        }
        else cout << "*** INFO(FtpUtil::GetEop): " << acName << " final EOP file " << eopFile << " has existed!" << endl;
    }
} /* end of GetEop */

/**
* @brief   : GetObx - to download ORBEX (ORBit EXchange format) files
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetObx(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* compute GPS week and day of week */
    TimeUtil tu;
    int wwww, dow;
    dow = tu.time2gpst(ts, &wwww, nullptr);
    StringUtil str;
    string sWwww = str.wwww2str(wwww);
    string sDow(to_string(dow));
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    string sYyyy = str.yyyy2str(yyyy);
    string sDoy = str.doy2str(doy);

    string ac(fopt->obxAc);
    str.TrimSpace4String(ac);
    str.ToLower(ac);

    /* cnt: CNES real-time ORBEX from CNES offline files */
    bool isRt = false;
    if (ac == "cnt") isRt = true;

    /* cod_m: CODE multi-GNSS final ORBEX
       gfz_m: GFZ multi-GNSS final ORBEX
       grg_m: CNES multi-GNSS final ORBEX
       whu_m: WHU multi-GNSS final ORBEX
    */
    std::vector<string> acMGEX = { "cod_m", "gfz_m", "grg_m", "whu_m" };
    bool isMGEX = false;
    for (int i = 0; i < acMGEX.size(); i++)
    {
        if (ac == acMGEX[i])
        {
            isMGEX = true;
            break;
        }
    }
    if (ac == "all_m") isMGEX = true;

    if (!isRt && !isMGEX)
    {
        cerr << "*** ERROR(FtpUtil::GetObx): Please check the setting of the 2nd item of getObx in configure file!" << endl;

        return;
    }

    /* creation of sub-directory */
    string subDir = dir;
    char tmpDir[MAXSTRPATH] = { '\0' };
    char sep = (char)FILEPATHSEP;
    if (isRt)
    {
        sprintf(tmpDir, "%s%c%s", dir, sep, "real_time");
        subDir = tmpDir;
    }
    else if (isMGEX)
    {
        sprintf(tmpDir, "%s%c%s", dir, sep, "final");
        subDir = tmpDir;
    }

    if (access(subDir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
        string cmd = "mkdir -p " + subDir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subDir.c_str());
#else           /* for Linux or Mac */
    chdir(subDir.c_str());
#endif

    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
    if (isRt)  /* for CNES real-time ORBEX from CNES offline files  */
    {
        string obxFile = ac + sWwww + sDow + ".obx";
        if (access(obxFile.c_str(), 0) == -1)
        {
            string obxgzFile = obxFile + ".gz";
            string url = "http://www.ppp-wizard.net/products/REAL_TIME/" + obxgzFile;
            string cmd = wgetFull + " " + qr + " -nH --cut-dirs=2 " + url;
            std::system(cmd.c_str());

            /* extract '*.gz' */
            cmd = gzipFull + " -d -f " + obxgzFile;
            std::system(cmd.c_str());

            char tmpFile[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, obxFile.c_str());
            string localFile = tmpFile;
            if (access(obxFile.c_str(), 0) == 0)
            {
                cout << "*** INFO(FtpUtil::GetObx): successfully download CNES real-time ORBEX file " << obxFile << endl;

                if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetObx): %s  ->  %s  OK\n", url.c_str(), localFile.c_str());
            }
            else
            {
                cout << "*** INFO(FtpUtil::GetObx): failed to download CNES real-time ORBEX file " << obxFile << endl;

                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetObx): %s  ->  %s  failed\n", url.c_str(), localFile.c_str());
            }

            /* delete some temporary directories */
            std::vector<string> tmpDir = { "FORMAT_BIAIS_OFFI1", "FORMATBIAS_OFF_v1" };
            for (int i = 0; i < tmpDir.size(); i++)
            {
                if (access(tmpDir[i].c_str(), 0) == 0)
                {
#ifdef _WIN32  /* for Windows */
                    cmd = "rd /s /q " + tmpDir[i];
#else          /* for Linux or Mac */
                    cmd = "rm -rf " + tmpDir[i];
#endif
                    std::system(cmd.c_str());
                }
            }
        }
        else cout << "*** INFO(FtpUtil::GetObx): CNES real-time ORBEX file " << obxFile <<
            " has existed!" << endl;
    }
    else if (isMGEX)  /* for MGEX final ORBEX files  */
    {
        string url, cutDirs = " --cut-dirs=5 ";
        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OBXM] + "/" + sWwww;
        else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_OBXM] + "/" + sWwww;
        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OBXM] + "/" + sWwww;
        else url = _ftpArchive.CDDIS[IDX_OBXM] + "/" + sWwww;

        if (ac == "all_m")
        {
            for (int i = 0; i < acMGEX.size(); i++)
            {
                string ac_m = acMGEX[i];
                string obxFile, obx0File, acName;
                if (ac_m == "cod_m")
                {
                    obxFile = "COD0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_ATT.OBX";
                    obx0File = "com" + sWwww + sDow + ".obx";
                    acName = "CODE";
                }
                else if (ac_m == "gfz_m")
                {
                    obxFile = "GFZ0MGXRAP_" + sYyyy + sDoy + "0000_01D_" + "*_ATT.OBX";
                    obx0File = "gbm" + sWwww + sDow + ".obx";
                    acName = "GFZ";
                }
                else if (ac_m == "grg_m")
                {
                    obxFile = "GRG0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_ATT.OBX";
                    obx0File = "grm" + sWwww + sDow + ".obx";
                    acName = "CNES";
                }
                else if (ac_m == "whu_m")
                {
                    obxFile = "WUM0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_ATT.OBX";
                    obx0File = "wum" + sWwww + sDow + ".obx";
                    acName = "WHU";
                }

                if (access(obx0File.c_str(), 0) == -1)
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    string obxxFile = obxFile + ".*";
                    string cmd = wgetFull + " " + qr + " -nH -A " + obxxFile + cutDirs + url;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    string obxgzFile = obxFile + ".gz";
                    cmd = gzipFull + " -d -f " + obxgzFile;
                    std::system(cmd.c_str());
                    string changeFileName;
#ifdef _WIN32  /* for Windows */
                    changeFileName = "rename";
#else          /* for Linux or Mac */
                    changeFileName = "mv";
#endif
                    cmd = changeFileName + " " + obxFile + " " + obx0File;
                    std::system(cmd.c_str());
                    bool isgz = true;
                    string obxzFile;
                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, obx0File.c_str());
                    string localFile = tmpFile;
                    if (access(obx0File.c_str(), 0) == -1)
                    {
                        /* extract '*.Z' */
                        obxzFile = obxFile + ".Z";
                        cmd = gzipFull + " -d -f " + obxzFile;
                        std::system(cmd.c_str());
                        isgz = false;

                        cmd = changeFileName + " " + obxFile + " " + obx0File;
                        std::system(cmd.c_str());

                        if (access(obx0File.c_str(), 0) == -1)
                        {
                            cout << "*** INFO(FtpUtil::GetObx): failed to download " << acName << " MGEX ORBEX file " << obx0File << endl;

                            string url0 = url + '/' + obxxFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetObx): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                        }
                    }

                    if (access(obx0File.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::GetObx): successfully download " << acName << " MGEX ORBEX file " << obx0File << endl;

                        if (isgz) obxxFile = obxgzFile;
                        else obxxFile = obxzFile;
                        string url0 = url + '/' + obxxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetObx): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::GetObx): " << acName << " MGEX ORBEX file " << obx0File << " has existed!" << endl;
            }
        }
        else
        {
            string obxFile, obx0File, acName;
            if (ac == "cod_m")
            {
                obxFile = "COD0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_ATT.OBX";
                    obx0File = "com" + sWwww + sDow + ".obx";
                    acName = "CODE";
            }
            else if (ac == "gfz_m")
            {
                obxFile = "GFZ0MGXRAP_" + sYyyy + sDoy + "0000_01D_" + "*_ATT.OBX";
                obx0File = "gbm" + sWwww + sDow + ".obx";
                acName = "GFZ";
            }
            else if (ac == "grg_m")
            {
                obxFile = "GRG0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_ATT.OBX";
                obx0File = "grm" + sWwww + sDow + ".obx";
                acName = "CNES";
            }
            else if (ac == "whu_m")
            {
                obxFile = "WUM0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_ATT.OBX";
                obx0File = "wum" + sWwww + sDow + ".obx";
                acName = "WHU";
            }

            if (access(obx0File.c_str(), 0) == -1)
            {
                /* it is OK for '*.Z' or '*.gz' format */
                string obxxFile = obxFile + ".*";
                string cmd = wgetFull + " " + qr + " -nH -A " + obxxFile + cutDirs + url;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                string obxgzFile = obxFile + ".gz";
                cmd = gzipFull + " -d -f " + obxgzFile;
                std::system(cmd.c_str());
                string changeFileName;
#ifdef _WIN32  /* for Windows */
                changeFileName = "rename";
#else          /* for Linux or Mac */
                changeFileName = "mv";
#endif
                cmd = changeFileName + " " + obxFile + " " + obx0File;
                std::system(cmd.c_str());
                bool isgz = true;
                string obxzFile;
                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, obx0File.c_str());
                string localFile = tmpFile;
                if (access(obx0File.c_str(), 0) == -1)
                {
                    /* extract '*.Z' */
                    obxzFile = obxFile + ".Z";
                    cmd = gzipFull + " -d -f " + obxzFile;
                    std::system(cmd.c_str());
                    isgz = false;

                    cmd = changeFileName + " " + obxFile + " " + obx0File;
                    std::system(cmd.c_str());

                    if (access(obx0File.c_str(), 0) == -1)
                    {
                        cout << "*** INFO(FtpUtil::GetObx): failed to download " << acName << " MGEX ORBEX file " << obx0File << endl;

                        string url0 = url + '/' + obxxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetObx): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                    }
                }

                if (access(obx0File.c_str(), 0) == 0)
                {
                    cout << "*** INFO(FtpUtil::GetObx): successfully download " << acName << " MGEX ORBEX file " << obx0File << endl;

                    if (isgz) obxxFile = obxgzFile;
                    else obxxFile = obxzFile;
                    string url0 = url + '/' + obxxFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetObx): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                }
            }
            else cout << "*** INFO(FtpUtil::GetObx): " << acName << " MGEX ORBEX file " << obx0File << " has existed!" << endl;
        }
    }
} /* end of GetObx */

/**
* @brief   : GetDsb - to download CODE DCB or MGEX daily DSB (i.e., from CAS) file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDsb(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir);
#else           /* for Linux or Mac */
    chdir(dir);
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sDoy = str.doy2str(doy);

    string ac(fopt->dsbAc);
    str.TrimSpace4String(ac);
    str.ToLower(ac);
    if (ac == "all")
    {
        std::vector<string> acDsb = { "cod", "cas" };
        for (int i = 0; i < acDsb.size(); i++)
        {
            string ac_m = acDsb[i];
            if (ac_m == "cas")  /* MGEX daily DSB (i.e., from CAS) */
            {
                string ftpName = fopt->ftpFrom;
                str.TrimSpace4String(ftpName);
                str.ToUpper(ftpName);
                string dsbFile = "CAS0MGXRAP_" + sYyyy + sDoy + "0000_01D_01D_DCB.BSX";
                if (access(dsbFile.c_str(), 0) == -1)
                {
                    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
                    string url, cutDirs = " --cut-dirs=5 ";
                    if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_DSBM] + "/" + sYyyy;
                    else if (ftpName == "IGN")
                    {
                        url = _ftpArchive.IGN[IDX_DSBM] + "/" + sYyyy;
                        cutDirs = " --cut-dirs=6 ";
                    }
                    else if (ftpName == "WHU")
                    {
                        url = _ftpArchive.WHU[IDX_DSBM] + "/" + sYyyy;
                        cutDirs = " --cut-dirs=6 ";
                    }
                    else url = _ftpArchive.CDDIS[IDX_DSBM] + "/" + sYyyy;

                    /* it is OK for '*.Z' or '*.gz' format */
                    string dsbxFile = dsbFile + ".*";
                    string cmd = wgetFull + " " + qr + " -nH -A " + dsbxFile + cutDirs + url;
                    std::system(cmd.c_str());

                    string dsbgzFile = dsbFile + ".gz", dsbzFile = dsbFile + ".Z";
                    bool isgz = false;
                    if (access(dsbgzFile.c_str(), 0) == 0)
                    {
                        /* extract '*.gz' */
                        cmd = gzipFull + " -d -f " + dsbgzFile;
                        std::system(cmd.c_str());
                        isgz = true;
                    }
                    else if (access(dsbzFile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipFull + " -d -f " + dsbzFile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", dir, sep, dsbFile.c_str());
                    string localFile = tmpFile;
                    if (access(dsbFile.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::GetDsb): successfully download multi-GNSS DSB file " << dsbFile << endl;

                        if (isgz) dsbxFile = dsbgzFile;
                        else dsbxFile = dsbzFile;
                        string url0 = url + '/' + dsbxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDsb): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                    }
                    else
                    {
                        cout << "*** INFO(FtpUtil::GetDsb): failed to download multi-GNSS DSB file " << dsbFile << endl;

                        string url0 = url + '/' + dsbxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDsb): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::GetDsb): multi-GNSS DSB file " << dsbFile << " has existed!" << endl;
            }
            else if (ac_m == "cod")  /* CODE DCB */
            {
                int yy = tu.yyyy2yy(yyyy);
                string sYy = str.yy2str(yy);
                double date[6];
                tu.time2ymdhms(ts, date);
                int mm = (int)date[1];
                string sMm = str.mm2str(mm);

                string dcbFile, dcb0File;
                std::vector<string> dcbType = { "P1P2", "P1C1", "P2C2" };
                for (int i = 0; i < dcbType.size(); i++)
                {
                    dcbFile = dcbType[i] + sYy + sMm + ".DCB";
                    dcb0File = dcbType[i] + sYy + sMm + ".DCB";
                    if (dcbType[i] == "P2C2") dcbFile = dcbType[i] + sYy + sMm + "_RINEX.DCB";

                    if (access(dcbFile.c_str(), 0) == -1 && access(dcb0File.c_str(), 0) == -1)
                    {
                        /* download CODE DCB file */
                        string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
                        string url = "ftp://ftp.aiub.unibe.ch/CODE/" + sYyyy;

                        /* it is OK for '*.Z' or '*.gz' format */
                        string dcbxFile = dcbFile + ".*";
                        string cmd = wgetFull + " " + qr + " -nH -A " + dcbxFile + " --cut-dirs=2 " + url;
                        std::system(cmd.c_str());

                        string dcbgzFile = dcbFile + ".gz", dcbzFile = dcbFile + ".Z";
                        bool isgz = false;
                        if (access(dcbgzFile.c_str(), 0) == 0)
                        {
                            /* extract '*.gz' */
                            cmd = gzipFull + " -d -f " + dcbgzFile;
                            std::system(cmd.c_str());
                            isgz = true;
                        }
                        else if (access(dcbzFile.c_str(), 0) == 0)
                        {
                            /* extract '*.Z' */
                            cmd = gzipFull + " -d -f " + dcbzFile;
                            std::system(cmd.c_str());
                            isgz = false;
                        }

                        char tmpFile[MAXSTRPATH] = { '\0' };
                        char sep = (char)FILEPATHSEP;
                        sprintf(tmpFile, "%s%c%s", dir, sep, dcbFile.c_str());
                        string localFile = tmpFile;
                        if (access(dcbFile.c_str(), 0) == -1)
                        {
                            cout << "*** INFO(FtpUtil::GetDsb): failed to download CODE DCB file " << dcbFile << endl;

                            string url0 = url + '/' + dcbxFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDsb): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                        }

                        if (dcbType[i] == "P2C2")
                        {
                            string changeFileName;
#ifdef _WIN32  /* for Windows */
                            changeFileName = "rename";
#else          /* for Linux or Mac */
                            changeFileName = "mv";
#endif
                            cmd = changeFileName + " " + dcbFile + " " + dcb0File;
                            std::system(cmd.c_str());
                        }

                        if (access(dcb0File.c_str(), 0) == 0)
                        {
                            cout << "*** INFO(FtpUtil::GetDsb): successfully download CODE DCB file " << dcb0File << endl;

                            if (isgz) dcbxFile = dcbgzFile;
                            else dcbxFile = dcbzFile;
                            string url0 = url + '/' + dcbxFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDsb): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                        }
                    }
                    else cout << "*** INFO(FtpUtil::GetDsb): CODE DCB file " << dcb0File << " or " << dcbFile << " has existed!" << endl;
                }
            }
        }
    }
    else
    {
        if (ac == "cas")  /* MGEX daily DSB (i.e., from CAS) */
        {
            string ftpName = fopt->ftpFrom;
            str.TrimSpace4String(ftpName);
            str.ToUpper(ftpName);
            string dsbFile = "CAS0MGXRAP_" + sYyyy + sDoy + "0000_01D_01D_DCB.BSX";
            if (access(dsbFile.c_str(), 0) == -1)
            {
                string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
                string url, cutDirs = " --cut-dirs=5 ";
                if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_DSBM] + "/" + sYyyy;
                else if (ftpName == "IGN")
                {
                    url = _ftpArchive.IGN[IDX_DSBM] + "/" + sYyyy;
                    cutDirs = " --cut-dirs=6 ";
                }
                else if (ftpName == "WHU")
                {
                    url = _ftpArchive.WHU[IDX_DSBM] + "/" + sYyyy;
                    cutDirs = " --cut-dirs=6 ";
                }
                else url = _ftpArchive.CDDIS[IDX_DSBM] + "/" + sYyyy;

                /* it is OK for '*.Z' or '*.gz' format */
                string dsbxFile = dsbFile + ".*";
                string cmd = wgetFull + " " + qr + " -nH -A " + dsbxFile + cutDirs + url;
                std::system(cmd.c_str());

                string dsbgzFile = dsbFile + ".gz", dsbzFile = dsbFile + ".Z";
                bool isgz = false;
                if (access(dsbgzFile.c_str(), 0) == 0)
                {
                    /* extract '*.gz' */
                    cmd = gzipFull + " -d -f " + dsbgzFile;
                    std::system(cmd.c_str());
                    isgz = true;
                }
                else if (access(dsbzFile.c_str(), 0) == 0)
                {
                    /* extract '*.Z' */
                    cmd = gzipFull + " -d -f " + dsbzFile;
                    std::system(cmd.c_str());
                    isgz = false;
                }

                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", dir, sep, dsbFile.c_str());
                string localFile = tmpFile;
                if (access(dsbFile.c_str(), 0) == 0)
                {
                    cout << "*** INFO(FtpUtil::GetDsb): successfully download multi-GNSS DSB file " << dsbFile << endl;

                    if (isgz) dsbxFile = dsbgzFile;
                    else dsbxFile = dsbzFile;
                    string url0 = url + '/' + dsbxFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDsb): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                }
                else
                {
                    cout << "*** INFO(FtpUtil::GetDsb): failed to download multi-GNSS DSB file " << dsbFile << endl;

                    string url0 = url + '/' + dsbxFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDsb): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                }
            }
            else cout << "*** INFO(FtpUtil::GetDsb): multi-GNSS DSB file " << dsbFile << " has existed!" << endl;
        }
        else if (ac == "cod")  /* CODE DCB */
        {
            int yy = tu.yyyy2yy(yyyy);
            string sYy = str.yy2str(yy);
            double date[6];
            tu.time2ymdhms(ts, date);
            int mm = (int)date[1];
            string sMm = str.mm2str(mm);

            string dcbFile, dcb0File;
            std::vector<string> dcbType = { "P1P2", "P1C1", "P2C2" };
            for (int i = 0; i < dcbType.size(); i++)
            {
                dcbFile = dcbType[i] + sYy + sMm + ".DCB";
                dcb0File = dcbType[i] + sYy + sMm + ".DCB";
                if (dcbType[i] == "P2C2") dcbFile = dcbType[i] + sYy + sMm + "_RINEX.DCB";

                if (access(dcbFile.c_str(), 0) == -1 && access(dcb0File.c_str(), 0) == -1)
                {
                    /* download CODE DCB file */
                    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
                    string url = "ftp://ftp.aiub.unibe.ch/CODE/" + sYyyy;

                    /* it is OK for '*.Z' or '*.gz' format */
                    string dcbxFile = dcbFile + ".*";
                    string cmd = wgetFull + " " + qr + " -nH -A " + dcbxFile + " --cut-dirs=2 " + url;
                    std::system(cmd.c_str());

                    string dcbgzFile = dcbFile + ".gz", dcbzFile = dcbFile + ".Z";
                    bool isgz = false;
                    if (access(dcbgzFile.c_str(), 0) == 0)
                    {
                        /* extract '*.gz' */
                        cmd = gzipFull + " -d -f " + dcbgzFile;
                        std::system(cmd.c_str());
                        isgz = true;
                    }
                    else if (access(dcbzFile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipFull + " -d -f " + dcbzFile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", dir, sep, dcbFile.c_str());
                    string localFile = tmpFile;
                    if (access(dcbFile.c_str(), 0) == -1)
                    {
                        cout << "*** INFO(FtpUtil::GetDsb): failed to download CODE DCB file " << dcbFile << endl;

                        string url0 = url + '/' + dcbxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetDsb): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                    }

                    if (dcbType[i] == "P2C2")
                    {
                        string changeFileName;
#ifdef _WIN32  /* for Windows */
                        changeFileName = "rename";
#else          /* for Linux or Mac */
                        changeFileName = "mv";
#endif
                        cmd = changeFileName + " " + dcbFile + " " + dcb0File;
                        std::system(cmd.c_str());
                    }

                    if (access(dcb0File.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::GetDsb): successfully download CODE DCB file " << dcb0File << endl;

                        if (isgz) dcbxFile = dcbgzFile;
                        else dcbxFile = dcbzFile;
                        string url0 = url + '/' + dcbxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetDsb): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::GetDsb): CODE DCB file " << dcb0File << " or " << dcbFile << " has existed!" << endl;
            }
        }
    }
} /* end of GetDsb */

/**
* @brief   : GetOsb - to download CNES real-time or MGEX final OSB files
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetOsb(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir);
#else           /* for Linux or Mac */
    chdir(dir);
#endif

    /* compute GPS week and day of week */
    TimeUtil tu;
    int wwww, dow;
    dow = tu.time2gpst(ts, &wwww, nullptr);
    StringUtil str;
    string sWwww = str.wwww2str(wwww);
    string sDow(to_string(dow));
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    string sYyyy = str.yyyy2str(yyyy);
    string sDoy = str.doy2str(doy);

    string ac(fopt->osbAc);
    str.TrimSpace4String(ac);
    str.ToLower(ac);

    /* cnt: CNES real-time OSB from CNES offline files */
    bool isRt = false;
    if (ac == "cnt") isRt = true;

    /* cod_m: CODE multi-GNSS final OSB
       gfz_m: GFZ multi-GNSS final OSB
       grg_m: CNES multi-GNSS final OSB
       whu_m: WHU multi-GNSS final OSB
    */
    std::vector<string> acMGEX = { "cas_m", "cod_m", "gfz_m", "grg_m", "whu_m" };
    bool isMGEX = false;
    for (int i = 0; i < acMGEX.size(); i++)
    {
        if (ac == acMGEX[i])
        {
            isMGEX = true;
            break;
        }
    }
    if (ac == "all_m") isMGEX = true;

    if (!isRt && !isMGEX)
    {
        cerr << "*** ERROR(FtpUtil::Getsb): Please check the setting of the 2nd item of getOsb in configure file!" << endl;

        return;
    }

    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
    if (isRt)  /* for CNES real-time OSB from CNES offline files  */
    {
        string osbFile = ac + sWwww + sDow + ".bia";
        if (access(osbFile.c_str(), 0) == -1)
        {
            string osbgzFile = osbFile + ".gz";
            string url = "http://www.ppp-wizard.net/products/REAL_TIME/" + osbgzFile;
            string cmd = wgetFull + " " + qr + " -nH --cut-dirs=2 " + url;
            std::system(cmd.c_str());

            /* extract '*.gz' */
            cmd = gzipFull + " -d -f " + osbgzFile;
            std::system(cmd.c_str());

            char tmpFile[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpFile, "%s%c%s", dir, sep, osbFile.c_str());
            string localFile = tmpFile;
            if (access(osbFile.c_str(), 0) == 0)
            {
                cout << "*** INFO(FtpUtil::GetOsb): successfully download CNES real-time OSB file " << osbFile << endl;

                if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetOsb): %s  ->  %s  OK\n", url.c_str(), localFile.c_str());
            }
            else
            {
                cout << "*** INFO(FtpUtil::GetOsb): failed to download CNES real-time OSB file " << osbFile << endl;

                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetOsb): %s  ->  %s  failed\n", url.c_str(), localFile.c_str());
            }

            /* delete some temporary directories */
            std::vector<string> tmpDir = { "FORMAT_BIAIS_OFFI1", "FORMATBIAS_OFF_v1" };
            for (int i = 0; i < tmpDir.size(); i++)
            {
                if (access(tmpDir[i].c_str(), 0) == 0)
                {
#ifdef _WIN32  /* for Windows */
                    cmd = "rd /s /q " + tmpDir[i];
#else          /* for Linux or Mac */
                    cmd = "rm -rf " + tmpDir[i];
#endif
                    std::system(cmd.c_str());
                }
            }
        }
        else cout << "*** INFO(FtpUtil::GetOsb): CNES real-time OSB file " << osbFile <<
            " has existed!" << endl;
    }
    else if (isMGEX)  /* for MGEX final OSB files  */
    {
        string url, cutDirs = " --cut-dirs=5 ";
        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_OSBM] + "/" + sWwww;
        else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_OSBM] + "/" + sWwww;
        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_OSBM] + "/" + sWwww;
        else url = _ftpArchive.CDDIS[IDX_OSBM] + "/" + sWwww;

        if (ac == "all_m")
        {
            for (int i = 0; i < acMGEX.size(); i++)
            {
                string ac_m = acMGEX[i];
                string osbFile, osb0File, acName;
                if (ac_m == "cas_m")
                {
                    osbFile = "CAS0MGXRAP_" + sYyyy + sDoy + "0000_01D_" + "*_OSB.BIA";
                    osb0File = "cas" + sWwww + sDow + ".bia";
                    acName = "CAS";
                }
                else if (ac_m == "cod_m")
                {
                    osbFile = "COD0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_OSB.BIA";
                    osb0File = "com" + sWwww + sDow + ".bia";
                    acName = "CODE";
                }
                else if (ac_m == "gfz_m")
                {
                    osbFile = "GFZ0MGXRAP_" + sYyyy + sDoy + "0000_01D_" + "*_OSB.BIA";
                    osb0File = "gbm" + sWwww + sDow + ".bia";
                    acName = "GFZ";
                }
                else if (ac_m == "grg_m")
                {
                    osbFile = "GRG0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_OSB.BIA";
                    osb0File = "grm" + sWwww + sDow + ".bia";
                    acName = "CNES";
                }
                else if (ac_m == "whu_m")
                {
                    osbFile = "WUM0MGXRAP_" + sYyyy + sDoy + "0000_01D_" + "*_OSB.BIA";
                    osb0File = "wum" + sWwww + sDow + ".bia";
                    acName = "WHU";
                }

                if (access(osb0File.c_str(), 0) == -1)
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    string osbxFile = osbFile + ".*";
                    string cmd;
                    if (ac_m == "cas_m")
                    {
                        string url0 = "ftp://ftp.gipp.org.cn/product/dcb/mgex/" + sYyyy;
                        string cutDirs0 = " --cut-dirs=4 ";
                        cmd = wgetFull + " " + qr + " -nH -A " + osbxFile + cutDirs0 + url0;
                        std::system(cmd.c_str());
                    }
                    else
                    {
                        cmd = wgetFull + " " + qr + " -nH -A " + osbxFile + cutDirs + url;
                        std::system(cmd.c_str());
                    }

                    /* extract '*.gz' */
                    string osbgzFile = osbFile + ".gz";
                    cmd = gzipFull + " -d -f " + osbgzFile;
                    std::system(cmd.c_str());
                    string changeFileName;
#ifdef _WIN32  /* for Windows */
                    changeFileName = "rename";
#else          /* for Linux or Mac */
                    changeFileName = "mv";
#endif
                    cmd = changeFileName + " " + osbFile + " " + osb0File;
                    std::system(cmd.c_str());
                    bool isgz = true;
                    string osbzFile;
                    char tmpFile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpFile, "%s%c%s", dir, sep, osb0File.c_str());
                    string localFile = tmpFile;
                    if (access(osb0File.c_str(), 0) == -1)
                    {
                        /* extract '*.Z' */
                        osbzFile = osbFile + ".Z";
                        cmd = gzipFull + " -d -f " + osbzFile;
                        std::system(cmd.c_str());
                        isgz = false;

                        cmd = changeFileName + " " + osbFile + " " + osb0File;
                        std::system(cmd.c_str());

                        if (access(osb0File.c_str(), 0) == -1)
                        {
                            cout << "*** INFO(FtpUtil::GetOsb): failed to download " << acName << " MGEX OSB file " << osb0File << endl;

                            string url0 = url + '/' + osbxFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetOsb): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                        }
                    }

                    if (access(osb0File.c_str(), 0) == 0)
                    {
                        cout << "*** INFO(FtpUtil::GetOsb): successfully download " << acName << " MGEX OSB file " << osb0File << endl;

                        if (isgz) osbxFile = osbgzFile;
                        else osbxFile = osbzFile;
                        string url0 = url + '/' + osbxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetOsb): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                    }
                }
                else cout << "*** INFO(FtpUtil::GetOsb): " << acName << " MGEX OSB file " << osb0File << " has existed!" << endl;
            }
        }
        else
        {
            string osbFile, osb0File, acName;
            if (ac == "cas_m")
            {
                osbFile = "CAS0MGXRAP_" + sYyyy + sDoy + "0000_01D_" + "*_OSB.BIA";
                osb0File = "cas" + sWwww + sDow + ".bia";
                acName = "CAS";
            }
            else if (ac == "cod_m")
            {
                osbFile = "COD0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_OSB.BIA";
                osb0File = "com" + sWwww + sDow + ".bia";
                acName = "CODE";
            }
            else if (ac == "gfz_m")
            {
                osbFile = "GFZ0MGXRAP_" + sYyyy + sDoy + "0000_01D_" + "*_OSB.BIA";
                osb0File = "gbm" + sWwww + sDow + ".bia";
                acName = "GFZ";
            }
            else if (ac == "grg_m")
            {
                osbFile = "GRG0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_OSB.BIA";
                osb0File = "grm" + sWwww + sDow + ".bia";
                acName = "CNES";
            }
            else if (ac == "whu_m")
            {
                osbFile = "WUM0MGXFIN_" + sYyyy + sDoy + "0000_01D_" + "*_OSB.BIA";
                osb0File = "wum" + sWwww + sDow + ".bia";
                acName = "WHU";
            }

            if (access(osb0File.c_str(), 0) == -1)
            {
                /* it is OK for '*.Z' or '*.gz' format */
                string osbxFile = osbFile + ".*";
                string cmd;
                if (ac == "cas_m")
                {
                    string url0 = "ftp://ftp.gipp.org.cn/product/dcb/mgex/" + sYyyy;
                    string cutDirs0 = " --cut-dirs=4 ";
                    cmd = wgetFull + " " + qr + " -nH -A " + osbxFile + cutDirs0 + url0;
                    std::system(cmd.c_str());
                }
                else
                {
                    cmd = wgetFull + " " + qr + " -nH -A " + osbxFile + cutDirs + url;
                    std::system(cmd.c_str());
                }

                /* extract '*.gz' */
                string osbgzFile = osbFile + ".gz";
                cmd = gzipFull + " -d -f " + osbgzFile;
                std::system(cmd.c_str());
                string changeFileName;
#ifdef _WIN32  /* for Windows */
                changeFileName = "rename";
#else          /* for Linux or Mac */
                changeFileName = "mv";
#endif
                cmd = changeFileName + " " + osbFile + " " + osb0File;
                std::system(cmd.c_str());
                bool isgz = true;
                string osbzFile;
                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", dir, sep, osb0File.c_str());
                string localFile = tmpFile;
                if (access(osb0File.c_str(), 0) == -1)
                {
                    /* extract '*.Z' */
                    osbzFile = osbFile + ".Z";
                    cmd = gzipFull + " -d -f " + osbzFile;
                    std::system(cmd.c_str());
                    isgz = false;

                    cmd = changeFileName + " " + osbFile + " " + osb0File;
                    std::system(cmd.c_str());

                    if (access(osb0File.c_str(), 0) == -1)
                    {
                        cout << "*** INFO(FtpUtil::GetOsb): failed to download " << acName << " MGEX OSB file " << osb0File << endl;

                        string url0 = url + '/' + osbxFile;
                        if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetOsb): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                    }
                }

                if (access(osb0File.c_str(), 0) == 0)
                {
                    cout << "*** INFO(FtpUtil::GetOsb): successfully download " << acName << " MGEX OSB file " << osb0File << endl;

                    if (isgz) osbxFile = osbgzFile;
                    else osbxFile = osbzFile;
                    string url0 = url + '/' + osbxFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetOsb): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                }
            }
            else cout << "*** INFO(FtpUtil::GetOsb): " << acName << " MGEX OSB file " << osb0File << " has existed!" << endl;
        }
    }
} /* end of GetOsb */

/**
* @brief   : GetSnx - to download IGS weekly SINEX file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetSnx(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir);
#else           /* for Linux or Mac */
    chdir(dir);
#endif

    /* compute GPS week and day of week */
    TimeUtil tu;
    int wwww;
    int dow = tu.time2gpst(ts, &wwww, nullptr);
    StringUtil str;
    string sWwww = str.wwww2str(wwww);
    string sDow(to_string(dow));

    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string snx0File = "igs" + sWwww + ".snx";
    if (access(snx0File.c_str(), 0) == -1)
    {
        string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;        
        string url, cutDirs = " --cut-dirs=4 ";
        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_SNX] + "/" + sWwww;
        else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_SNX] + "/" + sWwww;
        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_SNX] + "/" + sWwww;
        else url = _ftpArchive.CDDIS[IDX_SNX] + "/" + sWwww;
        
        /* it is OK for '*.Z' or '*.gz' format */
        string snxFile = "igs*P" + sWwww + ".snx";
        string snxxFile = snxFile + ".*";
        string cmd = wgetFull + " " + qr + " -nH -A " + snxxFile + cutDirs + url;
        std::system(cmd.c_str());

        /* extract '*.Z' */
        string snxzFile = snxFile + ".Z";
        cmd = gzipFull + " -d -f " + snxzFile;
        std::system(cmd.c_str());
        string changeFileName;
#ifdef _WIN32  /* for Windows */
        changeFileName = "rename";
#else          /* for Linux or Mac */
        changeFileName = "mv";
#endif
        cmd = changeFileName + " " + snxFile + " " + snx0File;
        std::system(cmd.c_str());
        bool isgz = false;
        string snxgzFile;
        char tmpFile[MAXSTRPATH] = { '\0' };
        char sep = (char)FILEPATHSEP;
        sprintf(tmpFile, "%s%c%s", dir, sep, snx0File.c_str());
        string localFile = tmpFile;
        if (access(snx0File.c_str(), 0) == -1)
        {
            /* extract '*.Z' */
            snxgzFile = snxFile + ".gz";
            cmd = gzipFull + " -d -f " + snxgzFile;
            std::system(cmd.c_str());

            cmd = changeFileName + " " + snxFile + " " + snx0File;
            std::system(cmd.c_str());
            isgz = true;
            if (access(snx0File.c_str(), 0) == -1)
            {
                cout << "*** INFO(FtpUtil::GetSnx): failed to download IGS weekly SINEX file " << snx0File << endl;

                string url0 = url + '/' + snxxFile;
                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetSnx): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
            }
        }

        /* If the weekly SINEX file does not exist,  we use the daily SINEX file instead */
        if (access(snx0File.c_str(), 0) == -1)
        {
            string url, cutDirs = " --cut-dirs=4 ";
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_SNX] + "/" + sWwww;
            else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_SNX] + "/" + sWwww;
            else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_SNX] + "/" + sWwww;
            else url = _ftpArchive.CDDIS[IDX_SNX] + "/" + sWwww;
            
            /* it is OK for '*.Z' or '*.gz' format */
            string snxFile = "igs*P" + sWwww + sDow + ".snx";
            string snxxFile = snxFile + ".*";
            string cmd = wgetFull + " " + qr + " -nH -A " + snxxFile + cutDirs + url;
            std::system(cmd.c_str());

            /* extract '*.Z' */
            string snxzFile = snxFile + ".Z";
            cmd = gzipFull + " -d -f " + snxzFile;
            std::system(cmd.c_str());
            string changeFileName;
#ifdef _WIN32  /* for Windows */
            changeFileName = "rename";
#else          /* for Linux or Mac */
            changeFileName = "mv";
#endif
            cmd = changeFileName + " " + snxFile + " " + snx0File;
            std::system(cmd.c_str());
            isgz = false;
            if (access(snx0File.c_str(), 0) == -1)
            {
                /* extract '*.Z' */
                string snxgzFile = snxFile + ".gz";
                cmd = gzipFull + " -d -f " + snxgzFile;
                std::system(cmd.c_str());

                cmd = changeFileName + " " + snxFile + " " + snx0File;
                std::system(cmd.c_str());
                isgz = true;
                if (access(snx0File.c_str(), 0) == -1)
                {
                    cout << "*** INFO(FtpUtil::GetSnx): failed to download IGS weekly SINEX file " << snx0File << endl;

                    string url0 = url + '/' + snxxFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetSnx): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                }
            }
        }

        if (access(snx0File.c_str(), 0) == 0)
        {
            cout << "*** INFO(FtpUtil::GetSnx): successfully download IGS weekly SINEX file " << snx0File << endl;

            if (isgz) snxxFile = snxgzFile;
            else snxxFile = snxzFile;
            string url0 = url + '/' + snxxFile;
            if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetSnx): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
        }

        /* delete some temporary directories */
        string tmpDir = "repro3";
        if (access(tmpDir.c_str(), 0) == 0)
        {
#ifdef _WIN32  /* for Windows */
            cmd = "rd /s /q " + tmpDir;
#else          /* for Linux or Mac */
            cmd = "rm -rf " + tmpDir;
#endif
            std::system(cmd.c_str());
        }
    }
    else cout << "*** INFO(FtpUtil::GetSnx): IGS weekly SINEX file " << snx0File << " has existed!" << endl;
} /* end of GetSnx */

/**
* @brief   : GetIono - to download daily GIM file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetIono(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir);
#else           /* for Linux or Mac */
    chdir(dir);
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);

    string ac(fopt->ionAc);
    str.TrimSpace4String(ac);
    str.ToLower(ac);
    vector<string> acs;
    int iPos = (int)ac.find_first_of('+');
    if (iPos > 0) str.GetSubStr(ac, "+", acs);
    else acs.push_back(ac);
    for (int i = 0; i < acs.size(); i++)
    {
        string ac_i = acs[i];
        /* cod_1d: CODE'S 1-DAY PREDICTED IONOSPHERE MAPS
           cod_2d: CODE'S 2-DAY PREDICTED IONOSPHERE MAPS
        */
        std::vector<string> acFiles;
        if (ac_i == "cod_1d") acFiles.push_back("c1p");
        else if (ac_i == "cod_2d") acFiles.push_back("c2p");;

        /* upc_0.25hr: UPC 15-minute rapid global ionosphere map (GIM) */
        if (ac_i == "upc_0.25hr") acFiles.push_back("uqr");

        /* emr_hr: EMR/NRCan hourly rapid GIM
           upc_hr: UPC hourly rapid GIM */
        if (ac_i == "emr_hr") acFiles.push_back("ehr");
        else if (ac_i == "upc_hr") acFiles.push_back("uhr");

        /* cas_r: CAS rapid GIM
           cod_r: CODE rapid GIM
           esa_r: ESA rapid GIM
           igs_r: IGS rapid GIM
           jpl_r: JPL rapid GIM
           upc_r: UPC rapid GIM
        */
        bool isRapid = false;
        if (ac_i == "cas_r")
        {
            acFiles.push_back("car");
            isRapid = true;
        }
        else if (ac_i == "cod_r")
        {
            acFiles.push_back("cor");
            isRapid = true;
        }
        else if (ac_i == "esa_r")
        {
            acFiles.push_back("esr");
            isRapid = true;
        }
        else if (ac_i == "igs_r")
        {
            acFiles.push_back("igr");
            isRapid = true;
        }
        else if (ac_i == "jpl_r")
        {
            acFiles.push_back("jpr");
            isRapid = true;
        }
        else if (ac_i == "upc_r")
        {
            acFiles.push_back("upr");
            isRapid = true;
        }
        else if (ac_i == "all_r")
        {
            acFiles = { "car", "cor", "esr", "igr", "jpr", "upr" };
            isRapid = true;
        }

        /* cas: GFZ final GIM
           cod: CODE final GIM
           emr: EMR/NRCan final GIM
           esa: ESA final GIM
           igs: IGS final GIM
           jpl: JPL final GIM
           upc: UPC final GIM
        */
        std::vector<string> finalAc = { "cas", "cod", "emr", "esa", "igs", "jpl", "upc" };
        bool isFinal = false;
        for (int i = 0; i < finalAc.size(); i++)
        {
            if (ac_i == finalAc[i])
            {
                acFiles.push_back(ac_i);
                isFinal = true;

                break;
            }
        }
        if (ac_i == "all")
        {
            acFiles = { "cas", "cod", "emr", "esa", "igs", "jpl", "upc" };
            isFinal = true;
        }

        string subDir = dir;
        if (isRapid || isFinal)
        {
            /* creation of sub-directory ('daily') */
            char tmpDir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            if (isRapid) sprintf(tmpDir, "%s%c%s", dir, sep, "rapid");
            else sprintf(tmpDir, "%s%c%s", dir, sep, "final");
            subDir = tmpDir;
            if (access(subDir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
                string cmd = "mkdir -p " + subDir;
#endif
                std::system(cmd.c_str());
            }  
        }
        /* change directory */
#ifdef _WIN32   /* for Windows */
        _chdir(subDir.c_str());
#else           /* for Linux or Mac */
        chdir(subDir.c_str());
#endif

        for (int i = 0; i < acFiles.size(); i++)
        {
            string acFile = acFiles[i];
            string ionFile = acFile + "g" + sDoy + "0." + sYy + "i";
            if (access(ionFile.c_str(), 0) == -1)
            {
                string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
                string url, cutDirs = " --cut-dirs=6 ";
                if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_ION] + "/" +
                    sYyyy + "/" + sDoy;
                else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_ION] + "/" +
                    sYyyy + "/" + sDoy;
                else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_ION] + "/" +
                    sYyyy + "/" + sDoy;
                else url = _ftpArchive.CDDIS[IDX_ION] + "/" + sYyyy + "/" + sDoy;

                /* it is OK for '*.Z' or '*.gz' format */
                string ionxFile = ionFile + ".*";
                string cmd = wgetFull + " " + qr + " -nH -A " + ionxFile + cutDirs + url;
                std::system(cmd.c_str());

                string iongzFile = ionFile + ".gz", ionzFile = ionFile + ".Z";
                bool isgz = false;
                if (access(iongzFile.c_str(), 0) == 0)
                {
                    /* extract '*.gz' */
                    cmd = gzipFull + " -d -f " + iongzFile;
                    std::system(cmd.c_str());
                    isgz = true;
                }
                else if (access(ionzFile.c_str(), 0) == 0)
                {
                    /* extract '*.Z' */
                    cmd = gzipFull + " -d -f " + ionzFile;
                    std::system(cmd.c_str());
                    isgz = false;
                }

                char tmpFile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, ionFile.c_str());
                string localFile = tmpFile;
                if (access(ionFile.c_str(), 0) == 0)
                {
                    cout << "*** INFO(FtpUtil::GetIono): successfully download GIM file " << ionFile << endl;

                    if (isgz) ionxFile = iongzFile;
                    else ionxFile = ionzFile;
                    string url0 = url + '/' + ionxFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetIono): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                }
                else
                {
                    cout << "*** INFO(FtpUtil::GetIono): failed to download GIM file " << ionFile << endl;

                    string url0 = url + '/' + ionxFile;
                    if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetIono): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                }

                /* delete some temporary directories */
                string tmpDir = "topex";
                if (access(tmpDir.c_str(), 0) == 0)
                {
#ifdef _WIN32  /* for Windows */
                    cmd = "rd /s /q " + tmpDir;
#else          /* for Linux or Mac */
                    cmd = "rm -rf " + tmpDir;
#endif
                    std::system(cmd.c_str());
                }
            }
            else cout << "*** INFO(FtpUtil::GetIono): GIM file " << ionFile << " has existed!" << endl;
        }
    }
} /* end of GetIono */

/**
* @brief   : GetRoti - to download daily Rate of TEC index (ROTI) file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetRoti(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir);
#else           /* for Linux or Mac */
    chdir(dir);
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);

    string ftpName = fopt->ftpFrom;
    str.TrimSpace4String(ftpName);
    str.ToUpper(ftpName);
    string rotFile = "roti" + sDoy + "0." + sYy + "f";
    if (access(rotFile.c_str(), 0) == -1)
    {
        string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
        string url, cutDirs = " --cut-dirs=6 ";
        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_ROTI] + "/" +
            sYyyy + "/" + sDoy;
        else if (ftpName == "IGN") url = _ftpArchive.IGN[IDX_ROTI] + "/" +
            sYyyy + "/" + sDoy;
        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_ROTI] + "/" +
            sYyyy + "/" + sDoy;
        else url = _ftpArchive.CDDIS[IDX_ROTI] + "/" + sYyyy + "/" + sDoy;

        /* it is OK for '*.Z' or '*.gz' format */
        string rotxFile = rotFile + ".*";
        string cmd = wgetFull + " " + qr + " -nH -A " + rotxFile + cutDirs + url;
        std::system(cmd.c_str());

        string rotgzFile = rotFile + ".gz", rotzFile = rotFile + ".Z";
        bool isgz = false;
        if (access(rotgzFile.c_str(), 0) == 0)
        {
            /* extract '*.gz' */
            cmd = gzipFull + " -d -f " + rotgzFile;
            std::system(cmd.c_str());
            isgz = true;
        }
        else if (access(rotzFile.c_str(), 0) == 0)
        {
            /* extract '*.Z' */
            cmd = gzipFull + " -d -f " + rotzFile;
            std::system(cmd.c_str());
            isgz = false;
        }

        char tmpFile[MAXSTRPATH] = { '\0' };
        char sep = (char)FILEPATHSEP;
        sprintf(tmpFile, "%s%c%s", dir, sep, rotFile.c_str());
        string localFile = tmpFile;
        if (access(rotFile.c_str(), 0) == 0)
        {
            cout << "*** INFO(FtpUtil::GetRoti): successfully download ROTI file " << rotFile << endl;

            if (isgz) rotxFile = rotgzFile;
            else rotxFile = rotzFile;
            string url0 = url + '/' + rotxFile;
            if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetRoti): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
        }
        else
        {
            cout << "*** INFO(FtpUtil::GetRoti): failed to download ROTI file " << rotFile << endl;

            string url0 = url + '/' + rotxFile;
            if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetRoti): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
        }

        /* delete some temporary directories */
        string tmpDir = "topex";
        if (access(tmpDir.c_str(), 0) == 0)
        {
#ifdef _WIN32  /* for Windows */
            cmd = "rd /s /q " + tmpDir;
#else          /* for Linux or Mac */
            cmd = "rm -rf " + tmpDir;
#endif
            std::system(cmd.c_str());
        }
    }
    else cout << "*** INFO(FtpUtil::GetRoti): ROTI file " << rotFile << " has existed!" << endl;
} /* end of GetRoti */

/**
* @brief   : GetTrop - to download daily tropospheric product file from CDDIS and/or CODE
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetTrop(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir);
#else           /* for Linux or Mac */
    chdir(dir);
#endif

    /* compute day of year */
    TimeUtil tu;
    int yyyy, doy;
    tu.time2yrdoy(ts, &yyyy, &doy);
    int yy = tu.yyyy2yy(yyyy);
    StringUtil str;
    string sYyyy = str.yyyy2str(yyyy);
    string sYy = str.yy2str(yy);
    string sDoy = str.doy2str(doy);
    int wwww;
    int dow = tu.time2gpst(ts, &wwww, nullptr);
    string sWwww = str.wwww2str(wwww);
    string sDow(to_string(dow));

    string ac(fopt->trpAc);
    str.TrimSpace4String(ac);
    str.ToLower(ac);
    string wgetFull = fopt->wgetFull, gzipFull = fopt->gzipFull, qr = fopt->qr;
    if (ac == "igs")
    {
        /* creation of sub-directory ('daily') */
        char tmpDir[MAXSTRPATH] = { '\0' };
        char sep = (char)FILEPATHSEP;
        sprintf(tmpDir, "%s%c%s", dir, sep, "IGS");
        string subDir = tmpDir;
        if (access(subDir.c_str(), 0) == -1)
        {
            /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + subDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + subDir;
#endif
            std::system(cmd.c_str());
        }

        /* change directory */
#ifdef _WIN32   /* for Windows */
        _chdir(subDir.c_str());
#else           /* for Linux or Mac */
        chdir(subDir.c_str());
#endif

        string ftpName = fopt->ftpFrom;
        str.TrimSpace4String(ftpName);
        str.ToUpper(ftpName);
        string sitFile = fopt->trpLst;
        if (strlen(sitFile.c_str()) < 9)  /* the option of 'all' is selected; the length of "site.list" is nine */
        {
            /* download all the IGS ZPD files */
            string url, cutDirs = " --cut-dirs=7 ";
            if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_ZTD] + "/" +
                sYyyy + "/" + sDoy;
            else if (ftpName == "IGN")
            {
                url = _ftpArchive.IGN[IDX_ZTD] + "/" + sYyyy + "/" + sDoy;
                cutDirs = " --cut-dirs=6 ";
            }
            else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_ZTD] + "/" +
                sYyyy + "/" + sDoy;
            else url = _ftpArchive.CDDIS[IDX_ZTD] + "/" + sYyyy + "/" + sDoy;

            /* it is OK for '*.Z' or '*.gz' format */
            string zpdFile = "*" + sDoy + "0." + sYy + "zpd";
            string zpdxFile = zpdFile + ".*";
            string cmd = wgetFull + " " + qr + " -nH -A " + zpdxFile + cutDirs + url;
            std::system(cmd.c_str());

            /* get the file list */
            string suffix = "." + sYy + "zpd";
            vector<string> zpdFiles;
            str.GetFilesAll(dir, suffix, zpdFiles);
            for (int i = 0; i < zpdFiles.size(); i++)
            {
                if (access(zpdFiles[i].c_str(), 0) == 0)
                {
                    /* extract it */
                    cmd = gzipFull + " -d -f " + zpdFiles[i];
                    std::system(cmd.c_str());
                }
            }
        }
        else  /* the option of full path of site list file is selected */
        {
            /* download the IGS ZPD file site-by-site */
            if (access(sitFile.c_str(), 0) == 0)
            {
                ifstream sitLst(sitFile.c_str());
                if (!sitLst.is_open())
                {
                    cerr << "*** ERROR(FtpUtil::GetTrop): open site.list = " << sitFile << " file failed, please check it" << endl;

                    return;
                }

                string sitName;
                while (getline(sitLst, sitName))
                {
                    if (sitName[0] == '#') continue;
                    str.TrimSpace4String(sitName);
                    if (sitName.size() != 4) continue;
                    str.ToLower(sitName);
                    string zpdFile = sitName + sDoy + "0." + sYy + "zpd";
                    if (access(zpdFile.c_str(), 0) == -1)
                    {
                        string url, cutDirs = " --cut-dirs=7 ";
                        if (ftpName == "CDDIS") url = _ftpArchive.CDDIS[IDX_ZTD] + "/" +
                            sYyyy + "/" + sDoy;
                        else if (ftpName == "IGN")
                        {
                            url = _ftpArchive.IGN[IDX_ZTD] + "/" + sYyyy + "/" + sDoy;
                            cutDirs = " --cut-dirs=6 ";
                        }
                        else if (ftpName == "WHU") url = _ftpArchive.WHU[IDX_ZTD] + "/" +
                            sYyyy + "/" + sDoy;
                        
                        /* it is OK for '*.Z' or '*.gz' format */
                        string zpdxFile = zpdFile + ".*";
                        string cmd = wgetFull + " " + qr + " -nH -A " + zpdxFile + cutDirs + url;
                        std::system(cmd.c_str());

                        string zpdgzFile = zpdFile + ".gz", zpdzFile = zpdFile + ".Z";
                        bool isgz = false;
                        if (access(zpdgzFile.c_str(), 0) == 0)
                        {
                            /* extract '*.gz' */
                            cmd = gzipFull + " -d -f " + zpdgzFile;
                            std::system(cmd.c_str());
                            isgz = true;
                        }
                        else if (access(zpdzFile.c_str(), 0) == 0)
                        {
                            /* extract '*.Z' */
                            cmd = gzipFull + " -d -f " + zpdzFile;
                            std::system(cmd.c_str());
                            isgz = false;
                        }

                        char tmpFile[MAXSTRPATH] = { '\0' };
                        char sep = (char)FILEPATHSEP;
                        sprintf(tmpFile, "%s%c%s", subDir.c_str(), sep, zpdFile.c_str());
                        string localFile = tmpFile;
                        if (access(zpdFile.c_str(), 0) == 0)
                        {
                            cout << "*** INFO(FtpUtil::GetTrop): successfully download IGS tropospheric product file " << zpdFile << endl;

                            if (isgz) zpdxFile = zpdgzFile;
                            else zpdxFile = zpdzFile;
                            string url0 = url + '/' + zpdxFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetTrop): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
                        }
                        else
                        {
                            cout << "*** WARNING(FtpUtil::GetTrop): failed to download IGS tropospheric product file " << zpdFile << endl;

                            string url0 = url + '/' + zpdxFile;
                            if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetTrop): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
                        }
                    }
                    else cout << "*** INFO(FtpUtil::GetTrop): IGS tropospheric product file " << zpdFile << 
                        " has existed!" << endl;
                }

                /* close 'site.list' */
                sitLst.close();
            }
        }
    }
    else if (ac == "cod")
    {
        /* creation of sub-directory ('daily') */
        char tmpDir[MAXSTRPATH] = { '\0' };
        char sep = (char)FILEPATHSEP;
        sprintf(tmpDir, "%s%c%s", dir, sep, "CODE");
        string subDir = tmpDir;
        if (access(subDir.c_str(), 0) == -1)
        {
            /* If the directory does not exist, creat it */
    #ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + subDir;
    #else           /* for Linux or Mac */
            string cmd = "mkdir -p " + subDir;
    #endif
            std::system(cmd.c_str());
        }

        /* change directory */
    #ifdef _WIN32   /* for Windows */
        _chdir(subDir.c_str());
    #else           /* for Linux or Mac */
        chdir(subDir.c_str());
    #endif

        string trpFile = "COD" + sWwww + sDow + ".TRO";
        if (access(trpFile.c_str(), 0) == -1)
        {
            string url = "ftp://ftp.aiub.unibe.ch/CODE/" + sYyyy;
            string cutDirs = " --cut-dirs=2 ";

            /* it is OK for '*.Z' or '*.gz' format */
            string trpxFile = trpFile + ".*";
            string cmd = wgetFull + " " + qr + " -nH -A " + trpxFile + cutDirs + url;
            std::system(cmd.c_str());

            string trpgzFile = trpFile + ".gz", trpzFile = trpFile + ".Z";
            bool isgz = false;
            if (access(trpgzFile.c_str(), 0) == 0)
            {
                /* extract '*.gz' */
                cmd = gzipFull + " -d -f " + trpgzFile;
                std::system(cmd.c_str());
                isgz = true;
            }
            else if (access(trpzFile.c_str(), 0) == 0)
            {
                /* extract '*.Z' */
                cmd = gzipFull + " -d -f " + trpzFile;
                std::system(cmd.c_str());
                isgz = false;
            }

            char tmpFile[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpFile, "%s%c%s", dir, sep, trpFile.c_str());
            string localFile = tmpFile;
            if (access(trpFile.c_str(), 0) == 0)
            {
                cout << "*** INFO(FtpUtil::GetTrop): successfully download CODE tropospheric product file " << trpFile << endl;

                if (isgz) trpxFile = trpgzFile;
                else trpxFile = trpzFile;
                string url0 = url + '/' + trpxFile;
                if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetTrop): %s  ->  %s  OK\n", url0.c_str(), localFile.c_str());
            }
            else
            {
                cout << "*** WARNING(FtpUtil::GetTrop): failed to download CODE tropospheric product file " << trpFile << endl;

                string url0 = url + '/' + trpxFile;
                if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetTrop): %s  ->  %s  failed\n", url0.c_str(), localFile.c_str());
            }
        }
        else cout << "*** INFO(FtpUtil::GetTrop): CODE tropospheric product file " << trpFile <<
            " has existed!" << endl;
    }
} /* end of GetTrop */

/**
* @brief   : GetAntexIGS - to download ANTEX file from IGS
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetAntexIGS(gtime_t ts, const char dir[], const ftpopt_t *fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir);
#else           /* for Linux or Mac */
    chdir(dir);
#endif

    string atxFile("igs14.atx");
    if (access(atxFile.c_str(), 0) == -1)
    {
        string wgetFull = fopt->wgetFull, qr = fopt->qr;
        string url = "https://files.igs.org/pub/station/general/" + atxFile;
        string cmd = wgetFull + " " + qr + " -nH --cut-dirs=3 " + url;
        std::system(cmd.c_str());

        char tmpFile[MAXSTRPATH] = { '\0' };
        char sep = (char)FILEPATHSEP;
        sprintf(tmpFile, "%s%c%s", dir, sep, atxFile.c_str());
        string localFile = tmpFile;
        if (access(atxFile.c_str(), 0) == 0)
        {
            cout << "*** INFO(FtpUtil::GetAntexIGS): successfully download IGS ANTEX file " << atxFile << endl;

            if (fopt->fpLog) fprintf(fopt->fpLog, "* INFO(GetAntexIGS): %s  ->  %s  OK\n", url.c_str(), localFile.c_str());
        }
        else
        {
            cout << "*** WARNING(FtpUtil::GetAntexIGS): failed to download IGS ANTEX file " << atxFile << endl;

            if (fopt->fpLog) fprintf(fopt->fpLog, "* WARNING(GetAntexIGS): %s  ->  %s  OK\n", url.c_str(), localFile.c_str());
        }
    }
    else cout << "*** INFO(FtpUtil::GetAntexIGS): IGS ANTEX file " << atxFile << " has existed!" << endl;
} /* end of GetAntexIGS */

/**
* @brief     : FtpDownload - GNSS data downloading via FTP
* @param[I]  : popt (processing options)
* @param[I/O]: fopt (FTP options)
* @return    : none
* @note      :
**/
void FtpUtil::FtpDownload(const prcopt_t *popt, ftpopt_t *fopt)
{
    /* Get FTP archive for CDDIS, IGN, or WHU */
    init();

    /* setting of the third-party softwares (.i.e, wget, gzip, crx2rnx etc.) */
    StringUtil str;
    if (fopt->isPath3party)  /* the path of third-party softwares is NOT set in the environmental variable */
    {
        char cmdTmp[MAXCHARS] = { '\0' };
        char sep = (char)FILEPATHSEP;

#ifdef _WIN32   /* for Windows */
        /* for wget */
        sprintf(cmdTmp, "%s%c%s", fopt->dir3party, sep, "wget.exe");
        str.TrimSpace4Char(cmdTmp);
        str.CutFilePathSep(cmdTmp);
        if (access(cmdTmp, 0) == 0) strcpy(fopt->wgetFull, cmdTmp);
        else
        {
            cout << "*** WARNING(FtpUtil::FtpDownload): wget CANNOT be found, please check the setting of 3partyDir!" << endl;

            return;
        }

        /* for gzip */
        sprintf(cmdTmp, "%s%c%s", fopt->dir3party, sep, "gzip.exe");
        str.TrimSpace4Char(cmdTmp);
        str.CutFilePathSep(cmdTmp);
        if (access(cmdTmp, 0) == 0) strcpy(fopt->gzipFull, cmdTmp);
        else
        {
            cout << "*** WARNING(FtpUtil::FtpDownload): gzip CANNOT be found, please check the setting of 3partyDir!" << endl;

            return;
        }

        /* for crx2rnx */
        sprintf(cmdTmp, "%s%c%s", fopt->dir3party, sep, "crx2rnx.exe");
        str.TrimSpace4Char(cmdTmp);
        str.CutFilePathSep(cmdTmp);
        if (access(cmdTmp, 0) == 0) strcpy(fopt->crx2rnxFull, cmdTmp);
        else
        {
            cout << "*** WARNING(FtpUtil::FtpDownload): crx2rnx CANNOT be found, please check the setting of 3partyDir!" << endl;

            return;
        }
#else           /* for Linux or Mac */
        str.SetStr(fopt->wgetFull, "wget", 5);
        str.SetStr(fopt->gzipFull, "gzip", 5);

        /* for crx2rnx */
        sprintf(cmdTmp, "%s%c%s", fopt->dir3party, sep, "crx2rnx");
        str.TrimSpace4Char(cmdTmp);
        str.CutFilePathSep(cmdTmp);
        if (access(cmdTmp, 0) == 0) strcpy(fopt->crx2rnxFull, cmdTmp);
        else
        {
            cout << "*** WARNING(FtpUtil::FtpDownload): crx2rnx CANNOT be found, please check the setting of 3partyDir!" << endl;

            return;
        }
#endif
    }
    else  /* the path of third-party softwares is set in the environmental variable */
    {
        str.SetStr(fopt->wgetFull, "wget", 5);
        str.SetStr(fopt->gzipFull, "gzip", 5);
        str.SetStr(fopt->crx2rnxFull, "crx2rnx", 8);
    }

    /* "-qr" (printInfoWget == false, not print information of 'wget') or "r" (printInfoWget == true, print information of 'wget') in 'wget' command line */
    if (fopt->printInfoWget) str.SetStr(fopt->qr, "-r", 3);
    else str.SetStr(fopt->qr, "-qr", 4);

    /* IGS observation (short name 'd') downloaded */
    if (fopt->getObs)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->obsDir, 0) == -1)
        {
            string tmpDir = popt->obsDir;
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + tmpDir;
#endif
            std::system(cmd.c_str());
        }

        string obsTyp = fopt->obsTyp;
        str.TrimSpace4String(obsTyp);
        str.ToLower(obsTyp);
        std::vector<string> obsFroms = { "igs", "mgex", "igm", "cut", "ga", "hk", "ngs", "epn", "pbo2", "pbo3", "pbo5" };
        string obsFrom = fopt->obsFrom;
        str.TrimSpace4String(obsFrom);
        str.ToLower(obsFrom);
        for (int i = 0; i < obsFroms.size(); i++)
        {
            if (obsFrom == obsFroms[i])
            {
                string obsuFrom = obsFrom;
                str.ToUpper(obsuFrom);
                /* creation of sub-directory */
                string subObsDir = popt->obsDir;
                char tmpDir[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpDir, "%s%c%s", popt->obsDir, sep, obsuFrom.c_str());
                subObsDir = tmpDir;
                if (access(subObsDir.c_str(), 0) == -1)
                {
                    /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                    string cmd = "mkdir " + subObsDir;
#else           /* for Linux or Mac */
                    string cmd = "mkdir -p " + subObsDir;
#endif
                    std::system(cmd.c_str());
                }

                if (obsFrom == "igs")       /* IGS observation (RINEX version 2.xx, short name "d") */
                {
                    if (obsTyp == "daily") GetDailyObsIgs(popt->ts, subObsDir.c_str(), fopt);
                    else if (obsTyp == "hourly") GetHourlyObsIgs(popt->ts, subObsDir.c_str(), fopt);
                    else if (obsTyp == "highrate") GetHrObsIgs(popt->ts, subObsDir.c_str(), fopt);
                }
                else if (obsFrom == "mgex") /* MGEX observation (RINEX version 3.xx, long name "crx") */
                {
                    if (obsTyp == "daily") GetDailyObsMgex(popt->ts, subObsDir.c_str(), fopt);
                    else if (obsTyp == "hourly") GetHourlyObsMgex(popt->ts, subObsDir.c_str(), fopt);
                    else if (obsTyp == "highrate") GetHrObsMgex(popt->ts, subObsDir.c_str(), fopt);
                }
                else if (obsFrom == "igm")  /* the union of IGS and MGEX observation with respect to the site name */
                {
                    if (obsTyp == "daily") GetDailyObsIgm(popt->ts, subObsDir.c_str(), fopt);
                    else if (obsTyp == "hourly") GetHourlyObsIgm(popt->ts, subObsDir.c_str(), fopt);
                    else if (obsTyp == "highrate") GetHrObsIgm(popt->ts, subObsDir.c_str(), fopt);
                }
                else if (obsFrom == "cut")  /* Curtin University of Technology (CUT) observation (RINEX version 3.xx, long name "crx") */
                {
                    if (obsTyp == "daily") GetDailyObsCut(popt->ts, subObsDir.c_str(), fopt);
                }
                else if (obsFrom == "ga")   /* Geoscience Australia (GA) observation (RINEX version 3.xx, long name "crx") */
                {
                    if (obsTyp == "daily") GetDailyObsGa(popt->ts, subObsDir.c_str(), fopt);
                    else if (obsTyp == "hourly") GetHourlyObsGa(popt->ts, subObsDir.c_str(), fopt);
                    else if (obsTyp == "highrate") GetHrObsGa(popt->ts, subObsDir.c_str(), fopt);
                }
                else if (obsFrom == "hk")   /* Hong Kong CORS observation (RINEX version 3.xx, long name "crx") */
                {
                    if (obsTyp == "30s") Get30sObsHk(popt->ts, subObsDir.c_str(), fopt);
                    else if (obsTyp == "5s" || obsTyp == "05s") Get5sObsHk(popt->ts, subObsDir.c_str(), fopt);
                    else if (obsTyp == "1s" || obsTyp == "01s") Get1sObsHk(popt->ts, subObsDir.c_str(), fopt);
                }
                else if (obsFrom == "ngs")  /* NGS/NOAA CORS observation (RINEX version 2.xx, short name "d") */
                {
                    if (obsTyp == "daily") GetDailyObsNgs(popt->ts, subObsDir.c_str(), fopt);
                }
                else if (obsFrom == "epn")  /* EUREF Permanent Network (EPN) observation (RINEX version 3.xx, long name "crx" and RINEX version 2.xx, short name "d") */
                {
                    if (obsTyp == "daily") GetDailyObsEpn(popt->ts, subObsDir.c_str(), fopt);
                }
                else if (obsFrom == "pbo2") /* Plate Boundary Observatory (PBO) observation (RINEX version 2.xx, short name "d") */
                {
                    if (obsTyp == "daily") GetDailyObsPbo2(popt->ts, subObsDir.c_str(), fopt);
                }
                else if (obsFrom == "pbo3") /* Plate Boundary Observatory (PBO) observation (RINEX version 3.xx, long name "crx") */
                {
                    if (obsTyp == "daily") GetDailyObsPbo3(popt->ts, subObsDir.c_str(), fopt);
                }
                else if (obsFrom == "pbo5") /* Plate Boundary Observatory (PBO) observation (taking the union of pbo2 and pbo3 (pbo2 + pbo3), while the priority of pbo3 sites is higher) */
                {
                    if (obsTyp == "daily") GetDailyObsPbo5(popt->ts, subObsDir.c_str(), fopt);
                }
            }
        }
    }

    /* broadcast ephemeris downloaded */
    if (fopt->getNav)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->navDir, 0) == -1)
        {
            string tmpDir = popt->navDir;
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + tmpDir;
#endif
            std::system(cmd.c_str());
        }

        GetNav(popt->ts, popt->navDir, fopt);
    }

    /* precise orbit and clock product downloaded */
    if (fopt->getOrbClk)
    {
        string ac(fopt->orbClkAc);
        str.TrimSpace4String(ac);
        str.ToLower(ac);
        std::vector<string> acs;
        int iPos = (int)ac.find_first_of('+');
        if (iPos > 0)
        {
            str.GetSubStr(ac, "+", acs);
            std::vector<string> acs_tmp;
            for (int i = 0; i < acs.size(); i++)
            {
                string ac_i = acs[i];
                if (ac_i == "all" || ac_i == "all_m")
                {
                    if (ac_i == "all")
                    {
                        std::vector<string> acs_igs = { "cod", "emr", "esa", "gfz", "grg", "igs", "jpl", "mit" };
                        for (int j = 0; j < acs_igs.size(); j++)
                        {
                            string ac_j = acs_igs[j];
                            acs_tmp.push_back(ac_j);
                        }
                    }
                    else
                    {
                        std::vector<string> acs_mgex = { "cod_m", "gfz_m", "grg_m", "whu_m" };
                        for (int j = 0; j < acs_mgex.size(); j++)
                        {
                            string ac_j = acs_mgex[j];
                            acs_tmp.push_back(ac_j);
                        }
                    }
                }
                else acs_tmp.push_back(ac_i);
            }
            sort(acs_tmp.begin(), acs_tmp.end());
            std::vector<string>::iterator tmp = unique(acs_tmp.begin(), acs_tmp.end());
            acs_tmp.erase(tmp, acs_tmp.end());
            acs = acs_tmp;
        }
        else
        {
            if (ac == "all" || ac == "all_m")
            {
                if (ac == "all") acs = { "cod", "emr", "esa", "gfz", "grg", "igs", "jpl", "mit" };
                else if (ac == "all_m") acs = { "cod_m", "gfz_m", "grg_m", "whu_m" };
            }
            else acs.push_back(ac);
        }
        for (int i = 0; i < acs.size(); i++)
        {
            string ac_i = acs[i];
            int prodType = 0;

            /* cnt: CNES real-time orbit and clock products from CNES offline files */
            if (ac_i == "cnt") prodType = PROD_REALTIME;

            /* esa_u: ESA ultra-rapid orbit and clock products
               gfz_u: GFZ ultra-rapid orbit and clock products
               igs_u: IGS ultra-rapid orbit and clock products
               whu_u: WHU ultra-rapid orbit and clock products
            */
            std::vector<string> ultraAc = { "esa_u", "gfz_u", "igs_u", "whu_u" };
            for (int i = 0; i < ultraAc.size(); i++)
            {
                if (ac_i == ultraAc[i])
                {
                    prodType = PROD_ULTRA_RAPID;

                    break;
                }
            }

            /* cod_r: CODE rapid orbit and clock products
               emr_r: NRCan rapid orbit and clock products
               esa_r: ESA rapid orbit and clock products
               gfz_r: GFZ rapid orbit and clock products
               igs_r: IGS rapid orbit and clock products
            */
            std::vector<string> rapidAc = { "cod_r", "emr_r", "esa_r", "gfz_r", "igs_r" };
            for (int i = 0; i < rapidAc.size(); i++)
            {
                if (ac_i == rapidAc[i])
                {
                    prodType = PROD_RAPID;

                    break;
                }
            }

            /* cod: CODE final orbit and clock products
               emr: NRCan final orbit and clock products
               esa: ESA final orbit and clock products
               gfz: GFZ final orbit and clock products
               grg: CNES final orbit and clock products
               igs: IGS final orbit and clock products
               jpl: JPL final orbit and clock products
               mit: MIT final orbit and clock products
            */
            std::vector<string> finalAcIGS = { "cod", "emr", "esa", "gfz", "grg", "igs", "jpl", "mit" };
            for (int i = 0; i < finalAcIGS.size(); i++)
            {
                if (ac_i == finalAcIGS[i])
                {
                    prodType = PROD_FINAL_IGS;

                    break;
                }
            }

            /* cod_m: CODE multi-GNSS final orbit and clock products
               gfz_m: GFZ multi-GNSS final orbit and clock products
               grg_m: CNES multi-GNSS final orbit and clock products
               whu_m: WHU multi-GNSS final orbit and clock products
            */
            std::vector<string> acMGEX = { "cod_m", "gfz_m", "grg_m", "whu_m" };
            for (int i = 0; i < acMGEX.size(); i++)
            {
                if (ac_i == acMGEX[i])
                {
                    prodType = PROD_FINAL_MGEX;

                    break;
                }
            }

            /* creation of sub-directory */
            string subOrbDir = popt->orbDir;
            string subClkDir = popt->clkDir;
            char tmpDir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            if (prodType == PROD_REALTIME)
            {
                sprintf(tmpDir, "%s%c%s", popt->orbDir, sep, "real_time");
                subOrbDir = tmpDir;
                sprintf(tmpDir, "%s%c%s", popt->clkDir, sep, "real_time");
                subClkDir = tmpDir;
            }
            else if (prodType == PROD_ULTRA_RAPID)
            {
                sprintf(tmpDir, "%s%c%s", popt->orbDir, sep, "ultra_rapid");
                subOrbDir = tmpDir;
            }
            else if (prodType == PROD_RAPID)
            {
                sprintf(tmpDir, "%s%c%s", popt->orbDir, sep, "rapid");
                subOrbDir = tmpDir;
                sprintf(tmpDir, "%s%c%s", popt->clkDir, sep, "rapid");
                subClkDir = tmpDir;
            }
            else if (prodType == PROD_FINAL_IGS || prodType == PROD_FINAL_MGEX)
            {
                sprintf(tmpDir, "%s%c%s", popt->orbDir, sep, "final");
                subOrbDir = tmpDir;
                sprintf(tmpDir, "%s%c%s", popt->clkDir, sep, "final");
                subClkDir = tmpDir;
            }
            /* for orb at a specified day */
            if (access(subOrbDir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                string cmd = "mkdir " + subOrbDir;
#else           /* for Linux or Mac */
                string cmd = "mkdir -p " + subOrbDir;
#endif
                std::system(cmd.c_str());
            }
            /* for clk at a specified day */
            if (prodType > 0 && prodType != PROD_ULTRA_RAPID)
            {
                if (access(subClkDir.c_str(), 0) == -1)
                {
                    /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                    string cmd = "mkdir " + subClkDir;
#else           /* for Linux or Mac */
                    string cmd = "mkdir -p " + subClkDir;
#endif
                    std::system(cmd.c_str());
                }
            }
            std::vector<string> subDirs = { subOrbDir, subClkDir };
            GetOrbClk(popt->ts, subDirs, prodType, ac_i, fopt);

            if (fopt->minusAdd1day && (prodType == PROD_FINAL_IGS || prodType == PROD_FINAL_MGEX))
            {
                TimeUtil tu;
                gtime_t tt;
                /* precise orbit and clock product downloaded for the day before the specified day */
                tt = tu.TimeAdd(popt->ts, -86400.0);
                GetOrbClk(tt, subDirs, prodType, ac_i, fopt);

                /* precise orbit and clock product downloaded for the day after the specified day */
                tt = tu.TimeAdd(popt->ts, 86400.0);
                GetOrbClk(tt, subDirs, prodType, ac_i, fopt);
            }
        }
    }
    
    /* EOP file downloaded */
    if (fopt->getEop)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->eopDir, 0) == -1)
        {
            string tmpDir = popt->eopDir;
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + tmpDir;
#endif
            std::system(cmd.c_str());
        }

        GetEop(popt->ts, popt->eopDir, fopt);
    }

    /* ORBEX (ORBit EXchange format) downloaded */
    if (fopt->getObx)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->obxDir, 0) == -1)
        {
            string tmpDir = popt->obxDir;
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + tmpDir;
#endif
            std::system(cmd.c_str());
        }

        GetObx(popt->ts, popt->obxDir, fopt);
    }


    /* differential code/signal bias (DCB/DSB) files downloaded */
    if (fopt->getDsb)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->biaDir, 0) == -1)
        {
            string tmpDir = popt->biaDir;
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + tmpDir;
#endif
            std::system(cmd.c_str());
        }

        GetDsb(popt->ts, popt->biaDir, fopt);
    }


    /* obsevable-specific code/phase signal bias (OSB) files downloaded */
    if (fopt->getOsb)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->biaDir, 0) == -1)
        {
            string tmpDir = popt->biaDir;
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + tmpDir;
#endif
            std::system(cmd.c_str());
        }

        GetOsb(popt->ts, popt->biaDir, fopt);
    }

    /* IGS SINEX file downloaded */
    if (fopt->getSnx)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->snxDir, 0) == -1)
        {
            string tmpDir = popt->snxDir;
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + tmpDir;
#endif
            std::system(cmd.c_str());
        }

        GetSnx(popt->ts, popt->snxDir, fopt);
    }

    /* global ionosphere map (GIM) downloaded */
    if (fopt->getIon)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->ionDir, 0) == -1)
        {
            string tmpDir = popt->ionDir;
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + tmpDir;
#endif
            std::system(cmd.c_str());
        }

        GetIono(popt->ts, popt->ionDir, fopt);
    }

    /* Rate of TEC index (ROTI) file downloaded */
    if (fopt->getRoti)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->ionDir, 0) == -1)
        {
            string tmpDir = popt->ionDir;
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + tmpDir;
#endif
            std::system(cmd.c_str());
        }

        GetRoti(popt->ts, popt->ionDir, fopt);
    }

    /* final tropospheric product downloaded */
    if (fopt->getTrp)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->ztdDir, 0) == -1)
        {
            string tmpDir = popt->ztdDir;
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + tmpDir;
#endif
            std::system(cmd.c_str());
        }

        GetTrop(popt->ts, popt->ztdDir, fopt);
    }

    /* IGS ANTEX file downloaded */
    if (fopt->getAtx)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->tblDir, 0) == -1)
        {
            string tmpDir = popt->tblDir;
#ifdef _WIN32   /* for Windows */
            string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
            string cmd = "mkdir -p " + tmpDir;
#endif
            std::system(cmd.c_str());
        }

        GetAntexIGS(popt->ts, popt->tblDir, fopt);
    }
} /* end of FtpDownload */