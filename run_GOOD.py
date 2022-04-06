#!/usr/bin/env python
# coding:utf-8


################################################################################
# PROGRAM: 
################################################################################
"""

 To run GOOD software for GNSS observations and products downloading

 UsagD: python run_GOOD.py -dir_main <dir_main_path> -time <yyyy> <doy> <ndays> -ftp <FTP_archieve> -obs <data_type> <source> <site_list> <start_hour> <nhours> 
           -nav <data_type> <nav_type> <source> <site_list> <start_hour> <nhours> -orbclk <source> <start_hour> <nhours> -eop <source> <start_hour> <nhours> 
           -eop <source> <start_hour> <nhours> -obx <source> -dsb <source> -snx -ion <source> -roti -trop <source> <site_list> -atx

 OPTIONS:
   <-dir_main>     - The root/main directory of GNSS observations and products storage, i.e., '-dir_main D:\data' under Windows or 
                      '-dir_main /home/zhouforme/data' under Linux/Mac
   <-time>         - time setting, 1st: 4-digit year, 2nd: day of year, 3rd: number of consecutive days, i.e., '-time 2022 32 3'
   <-ftp>          - FTP archieve, i.e., '-ftp cddis', '-ftp whu', or '-ftp ign'
   <-obs>          - [optional] GNSS observation data downloading option:
                      1st <data_type>: 'daily', 'hourly', 'highrate', '30s', '5s', or '1s';
                      2nd <source>: 'igs', 'mgex', 'igm', 'cut', 'ga', 'hk', 'ngs', 'epn', 'pbo2', 'pbo3', or 'pbo5';
                      3rd <site_list>: 'all' (observation files downloaded in the whole remote directory) or the full path of 
                         site list (observation files downloaded site-by-site according to the site list file);
                      4th <start_hour>: Start hour (00, 01, 02, ...);
                      5th <nhours>: the consecutive hours, i.e., '01  3' denotes 01, 02, and 03.
                      i.e., '-obs daily igs D:\data\site_igs.list 0 24' or '-obs daily igs all 0 24'
   <-nav>          - [optional] various broadcast ephemeris downloading option:
                      1st <data_type>: 'daily' or 'hourly';
                      2nd <nav_type>: 'gps', 'glo', 'bds', 'gal', 'qzs', 'irn', 'mixed3', 'mixed4', or 'all';
                      3rd <source>: 'igs', 'dlr', 'ign', 'gop', or 'wrd';
                      4th <start_hour>: Start hour (00, 01, 02, ...);
                      5th <nhours>: the consecutive hours, i.e., '01  3' denotes 01, 02, and 03.
                      i.e., '-nav daily mixed3 igs D:\data\site_igs.list 0 3' or '-nav daily gps dlr D:\data\site_igs.list 0 3'
   <-orbclk>       - [optional] satellite final/rapid/ultra-rapid precise orbit and clock downloading option:
                      1st <source>: IGS final: 'cod', 'emr', 'esa', 'gfz', 'grg', 'igs', 'jpl', 'mit', 'all', 'cod+igs', 'cod+gfz+igs', ...; 
                         MGEX final: 'cod_m', 'gfz_m', 'grg_m', 'whu_m', 'all_m', 'cod_m+gfz_m', 'grg_m+whu_m', ...; 
                         rapid: 'cod_r', 'emr_r', 'esa_r', 'gfz_r', 'igs_r'; 
                         ultra-rapid: 'esa_u', 'gfz_u', 'igs_u', 'whu_u'; 
                         real-timD: 'cnt';
                      2nd <start_hour>: Start hour (00, 06, 12, or 18 for esa_u and igs_u; 00, 03, 06, ... for gfz_u; 01, 02, 03, ... for whu_u);
                      3rd <nhours>: the consecutive sessions, i.e., '00  3' denotes 00, 06, and 12 for esa_u and/or igs_u, 00, 03, and 06 for gfz_u, 
                         while 00, 01, and 02 for whu_u.
                      i.e., '-orbclk igs 0 3' or '-orbclk igs+cod+cod_m 0 3'
   <-eop>          - [optional] Earth rotation/orientation parameter (ERP/EOP) downloading option:
                      1st <source>: final: 'cod', 'emr', 'esa', 'gfz', 'grg', 'igs', 'jpl', 'mit'; 
                         ultra-rapid: 'esa_u', 'gfz_u', 'igs_u';
                      2nd <start_hour>: Start hour (00, 06, 12, or 18 for esa_u and igs_u; 00, 03, 06, ... for gfz_u);
                      3rd <nhours>: the consecutive sessions, i.e., '00  3' denotes 00, 06, and 12 for esa_u and/or igs_u, 00, 03, and 06 for gfz_u.
                      i.e., '-eop igs 0 3' or '-eop igs_u 0 3'
   <-obx>          - [optional] ORBEX (ORBit EXchange format) for satellite attitude information downloading option:
                      1st <source>: final/rapid: 'cod_m', 'gfz_m', 'grg_m', 'whu_m', 'all_m'; 
                         real-time: 'cnt'
   <-dsb>          - [optional] Differential code/signal bias (DCB/DSB) downloading option:
                      1st <source>: 'cod', 'cas', 'all'
   <-snx>          - [optional] IGS weekly SINEX downloading option
   <-ion>          - [optional] Global ionosphere map (GIM) downloading option:
                      1st <source>: final: 'cas', 'cod', 'emr', 'esa', 'igs', 'jpl', 'upc', 'all', 'cas+cod', 'cas+cod+igs', ...; 
                         rapid: 'cas_r', cod_r', 'esa_r', 'igs_r', 'jpl_r', 'upc_r', 'all_r', 'cas_r+cod_r', 'cas_r+cod_r+igs_r', ...; 
                         hourly rapid: 'emr_hr', 'upc_hr'; 
                         15-min rapid: 'upc_0.25hr'; 
                         predicted: 'cod_1d', 'cod_2d'
   <-roti>         - [optional] Rate of TEC index (ROTI) downloading option
   <-trop>         - [optional] CODE/IGS tropospheric product downloading option:
                      1st <source>: 'igs' or 'cod';
                      2nd <site_list>: 'all' (observation files downloaded in the whole remote directory) or the full path of 
                         site list (observation files downloaded site-by-site according to the site list file).
   <-atx>          - [optional] ANTEX format antenna phase center correction downloading option

EXAMPLES: python run_GOOD.py -dir_main D:\data -time 2022 32 3 -ftp cddis -obs daily igs site_igs.list 0 24
          python run_GOOD.py -dir_main D:\data -time 2022 32 3 -ftp cddis -obs highrate igs site_igs.list 0 24
          python run_GOOD.py -dir_main D:\data -time 2022 32 3 -ftp whu -nav daily mixed3 igs 0 24
          python run_GOOD.py -dir_main D:\data -time 2022 32 3 -ftp whu -orbclk igs 0 24 -eop igs 0 24
          python run_GOOD.py -dir_main D:\data -time 2022 32 3 -ftp whu -snx -roti -atx
          python run_GOOD.py -dir_main D:\data -time 2022 32 3 -ftp whu -obs daily igs site_igs.list 0 24 -nav daily mixed3 igs 0 24 
             -orbclk igs 0 24 -eop igs 0 24 -obx cod_m -dsb cod -snx -ion all -roti -trop igs site_igs.list -atx

Changes: 06-Apr-2022   fzhou: create the prototype of the scripts for version 1.0

 to get help, typD:
           python run_GOOD.py
 or        python run_GOOD.py -h

"""
################################################################################
# Import Python modules
import os, sys, pydoc
import platform


__author__ = 'Feng Zhou @ SDUST'
__date__ = '$DatD: 2022-04-06 07:05:20 (Mon, 06 Apr 2022) $'[7:-21]
__version__ = '$Version: run_GOOD.py V1.0 $'[10:-2]


################################################################################
# FUNCTION: batch processing in single point positioning (SPP) mode using GAMPII software
################################################################################
def main_run_GOOD():
    nArgv = len(sys.argv)
    print(nArgv)
    if '-h' in sys.argv or nArgv < 2:
        pydoc.help(os.path.basename(sys.argv[0]).split('.')[0])
        return 0
    elif nArgv >= 2:
        mainDir = ''
        for i in range(0, nArgv):
            if sys.argv[i] == '-dir_main':
                if sys.argv[i+1][0] == '-':
                    print('*** ERROR: The full path of main directory is NOT given! Please check it.\n')
                    return 0
                else:
                    mainDir = sys.argv[i+1]
        if mainDir == '':
            return 0

        sep = ''
        if 'Windows' in platform.system():
            sep = '\\'
        elif 'Linux' or 'Mac' in platform.system():
            sep = '/'

        cfgFile = mainDir + sep + 'gamp_GOOD.cfg'
        with open(cfgFile,"w") as f_w:
            line = ''
            line += '# GAMP II - GOOD (Gnss Observations and prOducts Downloader) options, vers. 2.0\n'
            line += '\n'

            line += '# The directories of GNSS observations and products  ---------------------------\n'
            # to write the setting of main directory
            line += 'mainDir           = ' + mainDir + '                      % The root/main directory of GNSS observations and products\n'

            # to write the setting of sub-directories
            line += '  obsDir          = 0  obs                       % The sub-directory of RINEX format observation files\n'
            line += '  navDir          = 0  nav                       % The sub-directory of RINEX format broadcast ephemeris files\n'
            line += '  orbDir          = 0  orb                       % The sub-directory of SP3 format precise ephemeris files\n'
            line += '  clkDir          = 0  clk                       % The sub-directory of RINEX format precise clock files\n'
            line += '  eopDir          = 0  eop                       % The sub-directory of earth rotation/orientation parameter (EOP) files\n'
            line += '  obxDir          = 0  obx                       % The sub-directory of MGEX final/rapid and/or CNES real-time ORBEX (ORBit EXchange format) files\n'
            line += '  biaDir          = 0  bia                       % The sub-directory of CODE/MGEX differential code/signal bias (DCB/DSB), MGEX observable-specific \n'
            line += '                                                 %   signal bias (OSB), and/or CNES real-time OSB files\n'
            line += '  snxDir          = 0  snx                       % The sub-directory of SINEX format IGS weekly solution files\n'
            line += '  ionDir          = 0  ion                       % The sub-directory of CODE/IGS global ionosphere map (GIM) files\n'
            line += '  ztdDir          = 0  ztd                       % The sub-directory of CODE/IGS tropospheric product files\n'
            line += '  tblDir          = 0  tables                    % The sub-directory of table files (i.e., ANTEX, ocean tide loading files, etc.) for processing\n'

            # to write the setting of log file
            line += '\n'
            line += '# The directory of log files ---------------------------------------------------\n'
            line += 'logFile           = 1  ' + mainDir + sep + 'log' + sep + 'log.txt       % The log file with full path that gives the indications of whether the data downloading is successful or not\n'

            line += '\n'
            line += '# The directory of third-party softwares ---------------------------------------\n'
            line += '3partyDir         = 1  ' + mainDir + sep + 'thirdParty' + '        % (optional) The directory where third-party softwares (i.e., \'wget\', \'gzip\', \'crx2rnx\' etc) are stored, \n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-time':
                    if sys.argv[i+1][0] == '-' or sys.argv[i+2][0] == '-' or sys.argv[i+3][0] == '-':
                        print('*** ERROR: Three items are needed for time setting! Please check it.\n')
                        return 0
                    else:
                        yyyy = sys.argv[i+1]
                        doy = sys.argv[i+2]
                        ndays = sys.argv[i+3]
                        ctime = yyyy + '  ' + doy + '  ' + ndays
                        
                        line += '\n'
                        line += '# Time settings ----------------------------------------------------------------\n'
                        line += 'procTime          = 2  ' + ctime + '               % The setting of start time for processing\n'

                        line += '\n'
                        line += '# Settings of FTP downloading --------------------------------------------------\n'
                        line += 'minusAdd1day      = 1                            % The setting of the day before and after the current day for precise satellite orbit and clock \n'
                        line += '                                                 %   products downloading\n'
                        line += 'printInfoWget     = 1                            % Printing the information generated by \'wget\'\n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-ftp':
                    if sys.argv[i+1][0] == '-':
                        print('*** ERROR: The FTP archieve is NOT given! Please check it.\n')
                        return 0
                    else:
                        line += '\n'
                        line += '# Handling of FTP downloading --------------------------------------------------\n'
                        line += 'ftpDownloading    = 1  ' + sys.argv[i+1] + '                     % The setting of the master switch for data downloading\n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-obs':
                    if sys.argv[i+1][0] == '-' or sys.argv[i+2][0] == '-' or sys.argv[i+3][0] == '-' or sys.argv[i+4][0] == '-' or sys.argv[i+5][0] == '-':
                        print('*** ERROR: Five items are needed for observation downloading! Please check it.\n')
                        return 0
                    else:
                        cobs = ''
                        if sys.argv[i+3] == 'all':
                            cobs = sys.argv[i+1] + '  ' + sys.argv[i+2] + '  ' + sys.argv[i+3] + '  ' + sys.argv[i+4] + '  ' + sys.argv[i+5]
                        else:
                            cobs = sys.argv[i+1] + '  ' + sys.argv[i+2] + '  ' + mainDir + sep +  sys.argv[i+3] + '  ' + sys.argv[i+4] + '  ' + sys.argv[i+5]
                        line += '  getObs          = 1  ' + cobs + '    % GNSS observation data downloading option\n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-nav':
                    if sys.argv[i+1][0] == '-' or sys.argv[i+2][0] == '-' or sys.argv[i+3][0] == '-' or sys.argv[i+4][0] == '-' or sys.argv[i+5][0] == '-':
                        print('*** ERROR: Five items are needed for broadcast ephemeris downloading! Please check it.\n')
                        return 0
                    else:
                        cnav = sys.argv[i+1] + '  ' + sys.argv[i+2] + '  ' + sys.argv[i+3] + '  ' + sys.argv[i+4] + '  ' + sys.argv[i+5]
                        line += '  getNav          = 1  ' + cnav + ' % Various broadcast ephemeris downloading option\n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-orbclk':
                    if sys.argv[i+1][0] == '-' or sys.argv[i+2][0] == '-' or sys.argv[i+3][0] == '-':
                        print('*** ERROR: Three items are needed for final/rapid/ultra-rapid precise and clock downloading! Please check it.\n')
                        return 0
                    else:
                        corb = sys.argv[i+1] + '  ' + sys.argv[i+2] + '  ' + sys.argv[i+3]
                        line += '  getOrbClk       = 1  ' + corb + '                % Satellite final/rapid/ultra-rapid precise orbit and clock downloading option\n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-eop':
                    if sys.argv[i+1][0] == '-' or sys.argv[i+2][0] == '-' or sys.argv[i+3][0] == '-':
                        print('*** ERROR: Three items are needed for final/ultra-rapid earth rotation/orientation parameter (ERP/EOP) downloading! Please check it.\n')
                        return 0
                    else:
                        ceop = sys.argv[i+1] + '  ' + sys.argv[i+2] + '  ' + sys.argv[i+3]
                        line += '  getEop          = 1  ' + ceop + '                % Earth rotation/orientation parameter (ERP/EOP) downloading option\n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-obx':
                    if sys.argv[i+1][0] == '-':
                        print('*** ERROR: One item is needed for final/rapid/real-time ORBEX (ORBit EXchange format) downloading! Please check it.\n')
                        return 0
                    else:
                        line += '  getObx          = 1  ' + sys.argv[i+1] + '                     % ORBEX (ORBit EXchange format) for satellite attitude information downloading option\n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-dsb':
                    if sys.argv[i+1][0] == '-':
                        print('*** ERROR: One item is needed for differential code/signal bias (DCB/DSB) downloading! Please check it.\n')
                        return 0
                    else:
                        line += '  getDsb          = 1  ' + sys.argv[i+1] + '                       % Differential code/signal bias (DCB/DSB) downloading option\n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-snx':
                    line += '  getSnx          = 1                                 % IGS weekly SINEX downloading option\n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-ion':
                    if sys.argv[i+1][0] == '-':
                        print('*** ERROR: One item is needed for global ionosphere map (GIM) downloading! Please check it.\n')
                        return 0
                    else:
                        line += '  getIon          = 1  ' + sys.argv[i+1] + '                       % Global ionosphere map (GIM) downloading option\n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-roti':
                    line += '  getRoti         = 1                            % Rate of TEC index (ROTI) downloading option\n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-trop':
                    if sys.argv[i+1][0] == '-' or sys.argv[i+2][0] == '-':
                        print('*** ERROR: Two items are needed for tropospheric products downloading! Please check it.\n')
                        return 0
                    else:
                        cobs = ''
                        if sys.argv[i+2] == 'all':
                            ctrop = sys.argv[i+1] + '  ' + sys.argv[i+2]
                        else:
                            ctrop = sys.argv[i+1] + '  ' + mainDir + sep +  sys.argv[i+2]
                        line += '  getTrp          = 1  ' + ctrop + '                  % CODE/IGS tropospheric product downloading option\n'

            for i in range(0, nArgv):
                if sys.argv[i] == '-atx':
                    line += '  getAtx          = 1                            % ANTEX format antenna phase center correction downloading option\n'

            f_w.write(line)

    else:
        return 0

    if 'Windows' in platform.system():
        bin_GOOD = mainDir + sep + 'run_GAMP_GOOD.exe'
    elif 'Linux' or 'Mac' in platform.system():
        bin_GOOD = mainDir + sep + 'run_GAMP_GOOD'
    cfgFile = mainDir + sep + 'gamp_GOOD.cfg'
    cmd = bin_GOOD + ' ' + cfgFile
    os.system(cmd)


################################################################################
# Main program
################################################################################
if __name__ == '__main__':
    main_run_GOOD()