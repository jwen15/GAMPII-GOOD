/*------------------------------------------------------------------------------
* PreProcess.cpp : string functions
*
* Copyright (C) 2020-2099 by SpAtial SurveyIng and Navigation (SASIN) group, all rights reserved.
*    This file is part of GAMP II - GOOD (Gnss Observations and prOducts Downloader) toolkit
*
* References:
*    
* history : 2020/09/25 1.0  new (by Feng Zhou)
*           2021/04/30      a bug for EOP downloading is fixed (by Feng Zhou @ SDUST)
*           2021/04/30      the option "minusAdd1day" is added (by Feng Zhou @ SDUST)
*-----------------------------------------------------------------------------*/
#include "Good.h"
#include "StringUtil.h"
#include "TimeUtil.h"
#include "FtpUtil.h"
#include "PreProcess.h"


/* constants/macros ----------------------------------------------------------*/
#define MIN(x,y)    ((x) <= (y) ? (x) : (y))


/* function definition -------------------------------------------------------*/

/**
* @brief   : init - some initializations before processing
* @param[O]: popt (processing options)
* @param[O]: fopt (FTP options, nullptr:NO output)
* @return  : none
* @note    :
**/
void PreProcess::init(prcopt_t *popt, ftpopt_t *fopt)
{
    /* initialization for processing options */
    StringUtil str;
    /* processing directory */
    str.SetStr(popt->mainDir, "", 1);       /* the root/main directory of GNSS observations and products */
                                            /* the settings below are the sub-directories, and two parameters are needed
                                               1st: can be set to 0 or 1, 0: use the path of root/main directory; 1: NOT use the path of 
                                               root/main directory
                                               If '0' is set, the full path will NOT be needed, i.e., 'obxDir         = 0  obx', 
                                               indicating 'obxDir         = D:\data\obx'
                                               If '1' is set, the full path should be given, i.e., 'obxDir         = 1  D:\data\obx'
                                               2nd: the path of sub-directory, and absolute (1st: 1) or relative (1st: 0) path need to be set */
    str.SetStr(popt->obsDir, "", 1);        /* the sub-directory of RINEX format observation files */
    str.SetStr(popt->navDir, "", 1);        /* the sub-directory of RINEX format broadcast ephemeris files */
    str.SetStr(popt->orbDir, "", 1);        /* the sub-directory of SP3 format precise ephemeris files */
    str.SetStr(popt->clkDir, "", 1);        /* the sub-directory of RINEX format precise clock files */
    str.SetStr(popt->eopDir, "", 1);        /* the sub-directory of earth rotation/orientation parameter (EOP) files */
    str.SetStr(popt->obxDir, "", 1);        /* the sub-directory of MGEX final/rapid and/or CNES real-time ORBEX (ORBit EXchange format) files */
    str.SetStr(popt->snxDir, "", 1);        /* the sub-directory of SINEX format IGS weekly solution files */
    str.SetStr(popt->biaDir, "", 1);        /* the sub-directory of CODE/MGEX differential code/signal bias (DCB/DSB), MGEX observable-specific 
                                               signal bias (OSB), and/or CNES real-time OSB files */
    str.SetStr(popt->ionDir, "", 1);        /* the sub-directory of CODE/IGS global ionosphere map (GIM) files */
    str.SetStr(popt->ztdDir, "", 1);        /* the sub-directory of CODE/IGS tropospheric product files */
    str.SetStr(popt->tblDir, "", 1);        /* the sub-directory of table files (i.e., ANTEX, ocean tide loading files, etc.) for processing */
    str.SetStr(fopt->logFil, "", 1);        /* The log file with full path that gives the indications of whether the data downloading is 
                                               successful or not */
    fopt->logWriteMode = 0;                 /* 0: off  1: overwrite mode  2: append mode */
    
    /* time settings */
    popt->ts = { 0 };                       /* start time for processing */
    popt->ndays = 1;                        /* number of consecutive days */
    
    /* FTP downloading settings */
    str.SetStr(fopt->dir3party, "", 1);     /* the absolute path where third-party softwares are stored */
    fopt->isPath3party = false;             /* if true: the path need be set for third-party softwares */
    str.SetStr(fopt->wgetFull, "", 1);      /* if isPath3party == true, set the full path where 'wget' is */
    str.SetStr(fopt->gzipFull, "", 1);      /* if isPath3party == true, set the full path where 'gzip' is */
    str.SetStr(fopt->crx2rnxFull, "", 1);   /* if isPath3party == true, set the full path where 'crx2rnx' is */
    fopt->minusAdd1day = true;              /* (0:off  1:on) the day before and after the current day for precise satellite orbit and clock 
                                               products downloading */
    fopt->printInfoWget = false;            /* (0:off  1:on) print the information generated by 'wget' */
    str.SetStr(fopt->qr, "", 1);            /* '-qr' (printInfoWget == false) or '-r' (printInfoWget == true) in 'wget' command line */

    /* initialization for FTP options */
    fopt->ftpDownloading = false;           /* the master switch for data downloading, 0:off  1:on, only for data downloading */
    str.SetStr(fopt->ftpFrom, "", 1);       /* FTP archive: CDDIS, IGN, or WHU */
    fopt->getObs = false;                   /* (0:off  1:on) IGS observation (RINEX version 2.xx, short name 'd') */
    str.SetStr(fopt->obsTyp, "", 1);        /* "daily", "hourly", "highrate", "30s", "5s", or "1s" */
    str.SetStr(fopt->obsFrom, "", 1);       /* "igs", "mgex", "igm", "cut", "ga", "hk", "ngs", or "epn" */
    str.SetStr(fopt->obsLst, "", 1);        /* all; the full path of 'site.list' */
    fopt->getNav = false;                   /* (0:off  1:on) broadcast ephemeris */
    str.SetStr(fopt->navTyp, "", 1);        /* 'daily' or 'hourly' */
    str.SetStr(fopt->navSys, "", 1);        /* 'gps', 'glo', 'bds', 'gal', 'qzs', 'irn', 'mixed', or 'all' */
    str.SetStr(fopt->navAc, "", 1);         /* "igs", "dlr", "ign", "gop", or "wrd" */
    str.SetStr(fopt->navLst, "", 1);        /* the full path of 'site.list' */
    fopt->getOrbClk = false;                /* (0:off  1:on) precise orbit 'sp3' and precise clock 'clk' */
    str.SetStr(fopt->orbClkAc, "", 1);      /* analysis center (i.e., final: "cod", "emr", "esa", "gfz", "grg", "igs", "jpl", "mit", "cod_m", 
                                               "gfz_m", "grg_m", "whu_m"; rapid: "cod_r", "emr_r", "esa_r", "gfz_r", "igs_r"; ultra-rapid: "esa_u", 
                                               "gfz_u", "igs_u", "whu_u"; real-time: "cnt") */
    fopt->getEop = false;                   /* (0:off  1:on) earth rotation parameter */
    str.SetStr(fopt->eopAc, "", 1);         /* analysis center (i.e., final: "cod", "emr", "esa", "gfz", "grg", "igs", "jpl", "mit"; 
                                               ultra: "esa_u", "gfz_u", "igs_u") */
    fopt->getObx = false;                   /* (0:off  1:on) ORBEX (ORBit EXchange format) for satellite attitude information */
    str.SetStr(fopt->obxAc, "", 1);         /* analysis center (i.e., final/rapid: "cod_m", "gfz_m", "grg_m", "whu_m"; real-time: "cnt") */
    fopt->getSnx = false;                   /* (0:off  1:on) IGS weekly SINEX */
    fopt->getDsb = false;                   /* (0:off  1:on) differential code/signal bias (DCB/DSB) */
    str.SetStr(fopt->dsbAc, "", 1);         /* analysis center (i.e., "cod", "cas") */
    fopt->getOsb = false;                   /* (0:off  1:on) observable-specific signal bias (OSB) */
    str.SetStr(fopt->osbAc, "", 1);         /* analysis center (i.e., final/rapid: "cod_m", "gfz_m", "grg_m", "whu_m"; real-time: "cnt") */
    fopt->getIon = false;                   /* (0:off  1:on) global ionosphere map (GIM) */
    str.SetStr(fopt->ionAc, "", 1);         /* analysis center (i.e., "igs", "cod", "cas", ...) */
    fopt->getRoti = false;                  /* (0:off  1:on) rate of TEC index (ROTI) */
    fopt->getTrp = false;                   /* (0:off  1:on) CODE and/or IGS tropospheric product */
    str.SetStr(fopt->trpAc, "", 1);         /* analysis center (i.e., "igs" or "cod") */
    str.SetStr(fopt->trpLst, "", 1);        /* all; the full path of 'site.list' */
    fopt->getAtx = false;                   /* (0:off  1:on) ANTEX format antenna phase center correction */
} /* end of init */

/**
* @brief   : ReadCfgFile - read configure file to get processing options
* @param[I]: cfgFile (configure file)
* @param[O]: popt (processing options)
* @param[O]: fopt (FTP options, nullptr:NO output)
* @return  : true:ok, false:error
* @note    :
**/
bool PreProcess::ReadCfgFile(const char *cfgFile, prcopt_t *popt, ftpopt_t *fopt)
{
    /* open configure file */
    FILE *fp = nullptr;
    if (!(fp = fopen(cfgFile, "r")))
    {
        cerr << "*** ERROR(PreProcess::ReadCfgFile): open configure file " << cfgFile << " FAILED!" << endl;

        return false;
    }

    StringUtil str;
    TimeUtil tu;
    char *p, sline[MAXCHARS] = { '\0' }, tmpLine[MAXCHARS] = { '\0' };
    char sep = (char)FILEPATHSEP;
    bool debug = false;
    int j = 0;
    gtime_t tt = { 0 };
    if (debug) cout << "##################### Start of configure file #########################" << endl;
    while (fgets(sline, MAXCHARS, fp))
    {
        if ((sline != nullptr) && (sline[0] == '#')) continue;

        p = strrchr(sline, '=');

        /* processing directory */
        if (strstr(sline, "mainDir"))                 /* the root/main directory of GNSS observations and products */
        {
            sscanf(p + 1, "%[^%]", tmpLine);          /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            strcpy(popt->mainDir, tmpLine);
            if (debug) cout << "* mainDir = " << popt->mainDir << endl;
        }
        else if (strstr(sline, "obsDir"))             /* the sub-directory of RINEX format observation files */
        {
            sscanf(p + 1, "%[^%]", tmpLine);          /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            char tmpDir[MAXSTRPATH] = { '\0' };
            sprintf(tmpDir, "%s%c%s", popt->mainDir, sep, tmpLine);
            strcpy(popt->obsDir, tmpDir);
            if (debug) cout << "* obsDir = " << popt->obsDir << endl;
        }
        else if (strstr(sline, "navDir"))             /* the sub-directory of RINEX format broadcast ephemeris files */
        {
            sscanf(p + 1, "%[^%]", tmpLine);          /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            char tmpDir[MAXSTRPATH] = { '\0' };
            sprintf(tmpDir, "%s%c%s", popt->mainDir, sep, tmpLine);
            strcpy(popt->navDir, tmpDir);
            if (debug) cout << "* navDir = " << popt->navDir << endl;
        }
        else if (strstr(sline, "orbDir"))             /* the sub-directory of SP3 format precise ephemeris files */
        {
            sscanf(p + 1, "%[^%]", tmpLine);          /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            char tmpDir[MAXSTRPATH] = { '\0' };
            sprintf(tmpDir, "%s%c%s", popt->mainDir, sep, tmpLine);
            strcpy(popt->orbDir, tmpDir);
            if (debug) cout << "* orbDir = " << popt->orbDir << endl;
        }
        else if (strstr(sline, "clkDir"))             /* the sub-directory of RINEX format precise clock files */
        {
            sscanf(p + 1, "%[^%]", tmpLine);          /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            char tmpDir[MAXSTRPATH] = { '\0' };
            sprintf(tmpDir, "%s%c%s", popt->mainDir, sep, tmpLine);
            strcpy(popt->clkDir, tmpDir);
            if (debug) cout << "* clkDir = " << popt->clkDir << endl;
        }
        else if (strstr(sline, "eopDir"))             /* the sub-directory of earth rotation/orientation parameter (EOP) files */
        {
            sscanf(p + 1, "%[^%]", tmpLine);          /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            char tmpDir[MAXSTRPATH] = { '\0' };
            sprintf(tmpDir, "%s%c%s", popt->mainDir, sep, tmpLine);
            strcpy(popt->eopDir, tmpDir);
            if (debug) cout << "* eopDir = " << popt->eopDir << endl;
        }
        else if (strstr(sline, "obxDir"))             /* the sub-directory of MGEX final/rapid and/or CNES real-time ORBEX (ORBit EXchange format) files */
        {
            sscanf(p + 1, "%[^%]", tmpLine);          /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            char tmpDir[MAXSTRPATH] = { '\0' };
            sprintf(tmpDir, "%s%c%s", popt->mainDir, sep, tmpLine);
            strcpy(popt->obxDir, tmpDir);
            if (debug) cout << "* obxDir = " << popt->obxDir << endl;
        }
        else if (strstr(sline, "biaDir"))             /* the directory of CODE and/or MGEX differential code bias (DCB) files */
        {
            sscanf(p + 1, "%[^%]", tmpLine);          /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            char tmpDir[MAXSTRPATH] = { '\0' };
            sprintf(tmpDir, "%s%c%s", popt->mainDir, sep, tmpLine);
            strcpy(popt->biaDir, tmpDir);
            if (debug) cout << "* biaDir = " << popt->biaDir << endl;
        }
        else if (strstr(sline, "snxDir"))             /* the directory of SINEX format IGS weekly solution files */
        {
            sscanf(p + 1, "%[^%]", tmpLine);          /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            char tmpDir[MAXSTRPATH] = { '\0' };
            sprintf(tmpDir, "%s%c%s", popt->mainDir, sep, tmpLine);
            strcpy(popt->snxDir, tmpDir);
            if (debug) cout << "* snxDir = " << popt->snxDir << endl;
        }
        else if (strstr(sline, "ionDir"))             /* the directory of CODE and/or IGS global ionosphere map (GIM) files */
        {
            sscanf(p + 1, "%[^%]", tmpLine);          /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            char tmpDir[MAXSTRPATH] = { '\0' };
            sprintf(tmpDir, "%s%c%s", popt->mainDir, sep, tmpLine);
            strcpy(popt->ionDir, tmpDir);
            if (debug) cout << "* ionDir = " << popt->ionDir << endl;
        }
        else if (strstr(sline, "ztdDir"))             /* the directory of CODE and/or IGS tropospheric product files */
        {
            sscanf(p + 1, "%[^%]", tmpLine);          /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            char tmpDir[MAXSTRPATH] = { '\0' };
            sprintf(tmpDir, "%s%c%s", popt->mainDir, sep, tmpLine);
            strcpy(popt->ztdDir, tmpDir);
            if (debug) cout << "* ztdDir = " << popt->ztdDir << endl;
        }
        else if (strstr(sline, "tblDir"))             /* the directory of table files for processing */
        {
            sscanf(p + 1, "%[^%]", tmpLine);          /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            char tmpDir[MAXSTRPATH] = { '\0' };
            sprintf(tmpDir, "%s%c%s", popt->mainDir, sep, tmpLine);
            strcpy(popt->tblDir, tmpDir);
            if (debug) cout << "* tblDir = " << popt->tblDir << endl;
        }
        else if (strstr(sline, "3partyDir"))          /* (optional) the directory where third-party softwares (i.e., 'wget', 'gzip', 'crx2rnx' etc) are stored. This option is not needed if you have set the path or environment variable for them */
        {
            sscanf(p + 1, "%d %[^%]", &j, &tmpLine);  /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            strcpy(fopt->dir3party, tmpLine);
            fopt->isPath3party = j == 1 ? true : false;
            if (debug) cout << "* 3partyDir = " << fopt->dir3party << "  " << fopt->isPath3party << endl;

            string tmpDir = fopt->dir3party;
            if (access(tmpDir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
                string cmd = "mkdir -p " + tmpDir;
#endif
                std::system(cmd.c_str());
            }
        }
        else if (strstr(sline, "logFile"))            /* The log file with full path that gives the indications of whether the data downloading is successful or not */
        {
            sscanf(p + 1, "%d %[^%]", &fopt->logWriteMode, &tmpLine);  /* %[^%] denotes regular expression, which means that using % as the end sign of the input string */
            str.TrimSpace4Char(tmpLine);
            str.CutFilePathSep(tmpLine);
            strcpy(fopt->logFil, tmpLine);
            if (debug) cout << "* logFile = " << fopt->logFil << endl;
        }

        /* time settings */
        else if (strstr(sline, "procTime"))           /* start time for processing */
        {
            sscanf(p + 1, "%d", &j);
            if (j == 1)
            {
                double date[6] = { 0.0 };
                if (sscanf(p + 1, "%d %lf %lf %lf %d", &j, date + 0, date + 1, date + 2,
                    &popt->ndays) < 5)
                {
                    cerr << "*** ERROR(PreProcess::ReadCfgFile): the parameter of number of consecutive days is MISSING, please check it!" << endl;
                    
                    return false;
                }
                popt->ts = tu.ymdhms2time(date);
            }
            else if (j == 2)
            {
                double year = 0.0, doy = 0.0;
                if (sscanf(p + 1, "%d %lf %lf %d", &j, &year, &doy, &popt->ndays) < 4)
                {
                    cerr << "*** ERROR(PreProcess::ReadCfgFile): the parameter of number of consecutive days is MISSING, please check it!" << endl;

                    return false;
                };
                int iYear = (int)year, iDoy = (int)doy;
                popt->ts = tu.yrdoy2time(iYear, iDoy);
            }

            if (debug) cout << "* procTime = " << popt->ts.mjd << "  " << popt->ts.sod << endl;
        }

        /* FTP downloading settings */
        else if (strstr(sline, "minusAdd1day"))       /* (0:off  1:on) the day before and after the current day for precise satellite orbit and clock products downloading */
        {
            sscanf(p + 1, "%d", &j);
            fopt->minusAdd1day = j == 1 ? true : false;
            if (debug) cout << "* minusAdd1day = " << fopt->minusAdd1day << endl;
        }
        else if (strstr(sline, "printInfoWget"))      /* (0:off  1:on) print the information generated by 'wget' */
        {
            sscanf(p + 1, "%d", &j);
            fopt->printInfoWget = j == 1 ? true : false;
            if (debug) cout << "* printInfoWget = " << fopt->printInfoWget << endl;
        }

        /* handling of FTP downloading */
        else if (strstr(sline, "ftpDownloading"))     /* the master switch for data downloading (0:off  1:on, only for data downloading); the FTP archive, i.e., CDDIS, IGN, or WHU */
        {
            sscanf(p + 1, "%d %s", &j, &fopt->ftpFrom);
            fopt->ftpDownloading = j == 1 ? true : false;
            if (debug) cout << "* ftpDownloading = " << fopt->ftpDownloading << "  " << fopt->ftpFrom << endl;

            if (fopt->ftpDownloading)
            {
                int hh = 0, nh = 0, step = 1;
                while (fgets(sline, MAXCHARS, fp))
                {
                    if ((sline != nullptr) && (sline[0] == '#')) continue;
                    p = strrchr(sline, '=');

                    if (strstr(sline, "getObs"))      /* (0:off  1:on) GNSS observation data */
                    {
                        sscanf(p + 1, "%d %s %s %s %d %d", &j, &fopt->obsTyp, &fopt->obsFrom, &fopt->obsLst, &hh, &nh);
                        fopt->getObs = j == 1 ? true : false;
                        if (debug) cout << "* getObs = " << fopt->getObs << "  " << fopt->obsTyp << "  " << fopt->obsFrom << 
                            "  " << fopt->obsLst << endl;
                        
                        int imax = MIN(hh + nh, 24);
                        for (int i = hh; i < imax; i++) fopt->hhObs.push_back(i);
                    }
                    else if (strstr(sline, "getNav")) /* (0:off  1:on) broadcast ephemeris */
                    {
                        sscanf(p + 1, "%d %s %s %s %s %d %d", &j, &fopt->navTyp, &fopt->navSys, &fopt->navAc, &fopt->navLst, &hh, &nh);
                        fopt->getNav = j == 1 ? true : false;
                        if (debug) cout << "* getNav = " << fopt->getNav << "  " << fopt->navTyp << "  " << fopt->navSys << "  " << 
                            fopt->navAc << "  " << fopt->navLst << endl;

                        int imax = MIN(hh + nh, 24);
                        for (int i = hh; i < imax; i += step) fopt->hhNav.push_back(i);
                    }
                    else if (strstr(sline, "getOrbClk"))   /* (0:off  1:on) precise orbit and clock */
                    {
                        sscanf(p + 1, "%d %s %d %d", &j, &fopt->orbClkAc, &hh, &nh);
                        fopt->getOrbClk = j == 1 ? true : false;
                        if (debug) cout << "* getOrbClk = " << fopt->getOrbClk << "  " << fopt->orbClkAc << endl;

                        string ocOpt = fopt->orbClkAc;
                        std::vector<string> acs;
                        int iPos = (int)ocOpt.find_first_of('+');
                        if (iPos > 0) str.GetSubStr(ocOpt, "+", acs);
                        else acs.push_back(ocOpt);
                        fopt->hhOrbClk.resize(4);  /* for "esa_u", "gfz_u", "igs_u", and "whu_u" */
                        for (int i = 0; i < acs.size(); i++)
                        {
                            string ac_i = acs[i];
                            if (ac_i == "igs_u" || ac_i == "esa_u") step = 6;
                            else if (ac_i == "gfz_u") step = 3;
                            else if (ac_i == "whu_u") step = 1;
                            else step = 24;
                            int imax = 24;
                            for (int i = 0; i < imax; i += step)
                            {
                                if (hh > i) hh = i + step;
                                else break;
                            }
                            imax = MIN(hh + nh * step, 24);
                            for (int i = hh; i < imax; i += step)
                            {
                                if (ac_i == "esa_u") fopt->hhOrbClk[0].push_back(i);
                                else if (ac_i == "gfz_u") fopt->hhOrbClk[1].push_back(i);
                                else if (ac_i == "igs_u") fopt->hhOrbClk[2].push_back(i);
                                else if (ac_i == "whu_u") fopt->hhOrbClk[3].push_back(i);
                            }
                        }
                    }
                    else if (strstr(sline, "getEop")) /* (0:off  1:on) earth rotation parameter */
                    {
                        sscanf(p + 1, "%d %s %d %d", &j, &fopt->eopAc, &hh, &nh);
                        fopt->getEop = j == 1 ? true : false;
                        if (debug) cout << "* getEop = " << fopt->getEop << "  " << fopt->eopAc << endl;

                        string eOpt = fopt->eopAc;
                        if (eOpt == "igs_u" || eOpt == "esa_u") step = 6;
                        else if (eOpt == "gfz_u") step = 3;
                        else step = 24;
                        int imax = 24;
                        for (int i = 0; i < imax; i += step)
                        {
                            if (hh > i) hh = i + step;
                            else break;
                        }
                        imax = MIN(hh + nh * step, 24);
                        for (int i = hh; i < imax; i += step) fopt->hhEop.push_back(i);
                    }
                    else if (strstr(sline, "getObx")) /* (0:off  1:on) ORBEX (ORBit EXchange format) for satllite attitude information */
                    {
                        sscanf(p + 1, "%d %s", &j, &fopt->obxAc);
                        fopt->getObx = j == 1 ? true : false;
                        if (debug) cout << "* getObx = " << fopt->getObx << "  " << fopt->obxAc << endl;
                    }
                    else if (strstr(sline, "getDsb")) /* (0:off  1:on) differential code/signal bias (DCB/DSB) */
                    {
                        sscanf(p + 1, "%d %s", &j, &fopt->dsbAc);
                        fopt->getDsb = j == 1 ? true : false;
                        if (debug) cout << "* getDsb = " << fopt->getDsb << "  " << fopt->dsbAc << endl;
                    }
                    else if (strstr(sline, "getOsb")) /* (0:off  1:on) observable-specific signal bias (OSB) */
                    {
                        sscanf(p + 1, "%d %s", &j, &fopt->osbAc);
                        fopt->getOsb = j == 1 ? true : false;
                        if (debug) cout << "* getOsb = " << fopt->getOsb << "  " << fopt->osbAc << endl;
                    }
                    else if (strstr(sline, "getSnx")) /* (0:off  1:on) IGS weekly SINEX */
                    {
                        sscanf(p + 1, "%d", &j);
                        fopt->getSnx = j == 1 ? true : false;
                        if (debug) cout << "* getSnx = " << fopt->getSnx << endl;
                    }
                    else if (strstr(sline, "getIon")) /* (0:off  1:on) CODE and/or IGS global ionosphere map (GIM) */
                    {
                        sscanf(p + 1, "%d %s", &j, &fopt->ionAc);
                        fopt->getIon = j == 1 ? true : false;
                        if (debug) cout << "* getIon = " << fopt->getIon << "  " << fopt->ionAc << endl;
                    }
                    else if (strstr(sline, "getRoti"))/* (0:off  1:on) Rate of TEC index (ROTI) */
                    {
                        sscanf(p + 1, "%d", &j);
                        fopt->getRoti = j == 1 ? true : false;
                        if (debug) cout << "* getRoti = " << fopt->getRoti << endl;
                    }
                    else if (strstr(sline, "getTrp")) /* (0:off  1:on) CODE and/or IGS tropospheric product */
                    {
                        sscanf(p + 1, "%d %s %s", &j, &fopt->trpAc, &fopt->trpLst);
                        fopt->getTrp = j == 1 ? true : false;
                        if (debug) cout << "* getTrp = " << fopt->getTrp << "  " << fopt->trpAc << "  " << fopt->trpLst << endl;
                    }
                    else if (strstr(sline, "getAtx")) /* (0:off  1:on) ANTEX format antenna phase center correction */
                    {
                        sscanf(p + 1, "%d", &j);
                        fopt->getAtx = j == 1 ? true : false;
                        if (debug) cout << "* getAtx = " << fopt->getAtx << endl;
                        break;
                    }
                }
            }
        }
    }
    fclose(fp);

    if (debug) cout << "##################### End of configure file ###########################" << endl;

    return true;
} /* end of ReadCfgFile */

/**
* @brief   : run - start iPPP-RTK processing
* @param[I]: cfgFile (configure file with full path)
* @param[O]: none
* @return  : none
* @note    :
**/
void PreProcess::run(const char *cfgFile)
{
    prcopt_t popt;
    ftpopt_t fopt;
    /* initialization */
    init(&popt, &fopt);

    /* read configure file to get processing information */
    ReadCfgFile(cfgFile, &popt, &fopt);

    /* data downloading for GNSS further processing */
    if (fopt.ftpDownloading)
    {
        FtpUtil ftp;
        TimeUtil tu;
        StringUtil str;
        string obsDirMain = popt.obsDir;
        string navDirMain = popt.navDir;
        string ionDirMain = popt.ionDir;
        string ztdDirMain = popt.ztdDir;

        /* check if it need to write log file */
        fopt.fpLog = nullptr;
        if (fopt.logWriteMode > 0)
        {
            string logFilePath = fopt.logFil;
            int iPos = 0;
#ifdef _WIN32   /* for Windows */
            iPos = (int)logFilePath.find_last_of('\\');
#else           /* for Linux or Mac */
            iPos = (int)logFilePath.find_last_of('/');
#endif
            char logDirPath[MAXSTRPATH];
            str.StrMid(logDirPath, fopt.logFil, 0, iPos);
            /* If the directory does not exist, creat it */
            if (access(logDirPath, 0) == -1)
            {
                string tmpDir = logDirPath;
#ifdef _WIN32   /* for Windows */
                string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
                string cmd = "mkdir -p " + tmpDir;
#endif
                std::system(cmd.c_str());
            }

            if (fopt.logWriteMode == 1) fopt.fpLog = fopen(fopt.logFil, "w");  /* overwrite mode */
            else if (fopt.logWriteMode == 2) fopt.fpLog = fopen(fopt.logFil, "a");  /* append mode */
        }

        for (int i = 0; i < popt.ndays; i++)
        {
            int yyyy, doy;
            tu.time2yrdoy(popt.ts, &yyyy, &doy);
            string sYyyy = str.yyyy2str(yyyy);
            string sDoy = str.doy2str(doy);

            /* creat new observation sub-directory */
            if (fopt.getObs)
            {
                char dir[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(dir, "%s%c%s%c%s", obsDirMain.c_str(), sep, sYyyy.c_str(), sep, sDoy.c_str());
                str.TrimSpace4Char(dir);
                str.CutFilePathSep(dir);
                strcpy(popt.obsDir, dir);
                string tmpDir = dir;
                if (access(tmpDir.c_str(), 0) == -1)
                {
                    /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                    string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
                    string cmd = "mkdir -p " + tmpDir;
#endif
                    std::system(cmd.c_str());
                }
            }

            /* creat new NAV sub-directory */
            if (fopt.getNav)
            {
                char dir[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(dir, "%s%c%s%c%s", navDirMain.c_str(), sep, sYyyy.c_str(), sep, sDoy.c_str());
                str.TrimSpace4Char(dir);
                str.CutFilePathSep(dir);
                strcpy(popt.navDir, dir);
                string tmpDir = dir;
                if (access(tmpDir.c_str(), 0) == -1)
                {
                    /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                    string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
                    string cmd = "mkdir -p " + tmpDir;
#endif
                    std::system(cmd.c_str());
                }
            }

            /* creat new ION sub-directory */
            if (fopt.getIon)
            {
                char dir[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(dir, "%s%c%s%c%s", ionDirMain.c_str(), sep, sYyyy.c_str(), sep, sDoy.c_str());
                str.TrimSpace4Char(dir);
                str.CutFilePathSep(dir);
                strcpy(popt.ionDir, dir);
                string tmpDir = dir;
                if (access(tmpDir.c_str(), 0) == -1)
                {
                    /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                    string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
                    string cmd = "mkdir -p " + tmpDir;
#endif
                    std::system(cmd.c_str());
                }
            }

            /* creat new ZTD sub-directory */
            if (fopt.getTrp)
            {
                char dir[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(dir, "%s%c%s%c%s", ztdDirMain.c_str(), sep, sYyyy.c_str(), sep, sDoy.c_str());
                str.TrimSpace4Char(dir);
                str.CutFilePathSep(dir);
                strcpy(popt.ztdDir, dir);
                string tmpDir = dir;
                if (access(tmpDir.c_str(), 0) == -1)
                {
                    /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                    string cmd = "mkdir " + tmpDir;
#else           /* for Linux or Mac */
                    string cmd = "mkdir -p " + tmpDir;
#endif
                    std::system(cmd.c_str());
                }
            }

            /* the main entry of FTP downloader */
            ftp.FtpDownload(&popt, &fopt);

            popt.ts = tu.TimeAdd(popt.ts, 86400.0);
        }

        if (fopt.fpLog) fclose(fopt.fpLog);  /* close the log file */
    }
} /* end of run */