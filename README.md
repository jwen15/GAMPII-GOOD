# GAMP II - GOOD, version 2.0
GAMP II - GOOD (Gnss Observations and prOducts Downloader) is a powerful and easy-to-use lightweight GNSS observations and product downloading toolkit, which is developed by the SASIN (SpAtial SurveyIng and Navigation) group at Shandong University of Science and Technology (SDUST). The current version is 2.0, which is more stable and powerful. Hence, we decide to make it available for global GNSS users, and we are convinced that more and more GNSS users will benefit from GAMP II - GOOD. The source codes of GAMP II - GOOD is now available at GitHub (https://github.com/zhouforme0318/GAMPII-GOOD). We dedicated to create a more powerful GNSS data downloading tool, which can help each GNSS user to get rid of the trouble as much as possible in GNSS data and product downloading.
# Features
Main feature of GAMP II - GOOD are various GNSS observations and products downloading, i.e., 
- IGS daily, hourly, and high-rate observations with short file name (RINEX version 2.xx)
- MGEX daily, hourly, and high-rate observations with long file name (RINEX version 3.xx)
- IGS + MGEX (taking the union of IGS and MGEX, while MGEX is preferred) daily, hourly, and high-rate observations
- Curtin University of Technology (CUT) daily observations with long file name (RINEX v3.xx)
- Geoscience Australia (GA) daily, hourly, and high-rate observations with long file name (RINEX v3.xx)
- Hong Kong CORS 30-s, 5-s, and 1-s observations with long file name (RINEX version v3.xx)
- NGS/NOAA CORS daily observations with short file name (RINEX v2.xx)
- EUREF Permanent Network (EPN) observations (long file name in RINEX v3.xx and short file name in RINEX v2.xx)
- Plate Boundary Observatory (PBO) observations (long file name in RINEX v3.xx and short file name in RINEX v2.xx)
- Various types of broadcast ephemeris, i.e., GPS- and GLONASS-only in RINEX v2.xx, mixed types in RINEX v3.xx and v4.xx for multiple-GNSS
- Various types of IGS, MGEX, and other analysis center (AC) final, rapid, and ultra-rapid precise orbit and clock products, CNES real-time orbit and clock products in offline file format
- Various types of IGS final and ultra-rapid earth rotation/orientation parameter (ERP/EOP)
- ORBEX (ORBit EXchange format) products from five institutions, i.e., CODE, GFZ, GRG, WHU, and CNES
- GPS and GLONASS differential code bias (DCB) products from CODE and multi-GNSS differential signal bias (DSB) products from CAS
- Code observable-specific signal bias (OSB) products from six institutions, i.e., CAS, CODE, GFZ, GRG, WHU, and CNES
- IGS weekly SINEX solutions
- Various types of final, rapid, hourly rapid, 15-min rapid, 1-day and 2-day predicted global ionosphere map (GIM) products
- Rate of TEC index (ROTI) products
- CODE and IGS final tropospheric products
- IGS ANTEX format antenna phase center corrections  
But it is not limited to these, users can use the source code to expand its functionalities according to their needs.
# Version
- Vers. 1.0  
  2021/04/16 new  
  2021/04/20 MGEX multi-GNSS precise products option added  
- Vers. 1.1  
  2021/04/23 IGN and WHU FPT archives added  
  2021/04/30 the day before and after the current day for precise satellite orbit and clock products downloading is added
- Vers. 1.2  
  2021/04/30 CNES real-time orbit, clock, bias files (in offline mode) option added  
  2021/05/01 from DOY 345/2020, 'brdcDDD0.YYn' was converted from '*.Z' to '*.gz'  
  2021/05/01 added the full path of third-party softwares
- Vers. 1.3  
  2021/05/05 add 'GetRoti' for rate of TEC index (ROTI) file downloading
- Vers. 1.4  
  2021/05/06 add the compatibility for changing directory (chdir) in Windows and Linux OS  
  2021/05/07 add the option 'printInfoWget' in configure file for (not) printing the information of running 'wget'  
  2021/05/08 add IGS and MGEX hourly observation (30s) downloading
- Vers. 1.5  
  2021/05/10 modify some codes to make GOOD adaptable to different compression form (i.e., '*.Z' or '*.gz')  
  2021/05/12 add the option for IGR, IGU, GFU (from GFZ), and WUU (from WHU) products as well as the site-specific broadcast ephemeris files downloading  
  2021/05/15 sub-directory (i.e., daily, hourly, and highrate) creation for observation downloading
- Vers. 1.6  
  2021/05/18 modify some codes to make configuration file and program look more concise  
  2021/05/21 add the extraction and conversion for the 'all' option in IGS and MGEX observation downloading
- Vers. 1.7  
  2021/06/01 add 'getObc', 'getObg', and 'getObh' options for Curtin University of Technology (CUT), Geoscience Australia (GA), and Hong Kong CORS observation downloading
- Vers. 1.8  
  2021/09/01 add various types of (i.e., ESA, GFZ, IGS, WHU) and rapid (i.e., CODE, NRCan, ESA, GFZ, IGS) orbit and clock products for downloading (by Feng Zhou @ SDUST and Sermet Ogutcu @ NEU)  
  2021/09/04 add the option for NGS/NOAA CORS (from Amazon Web Services (AWS)) and EUREF Permanent Network (EPN) observation downloading (by Feng Zhou @ SDUST)  
  2021/09/05 fix the bug of broadcast ephemeris downloading via WHU FTP before and after 2020 (by Feng Zhou @ SDUST and Zan Liu @ CUMT)  
  2021/09/06 modify the setting of directories of GNSS observations and products in configure file to make them more elastic (by Feng Zhou @ SDUST and Caoming Fan @ SDU)
- Vers. 1.9  
  Please refer to the header component of FtpUtil.cpp
- Vers. 2.0  
  Please refer to the header component of FtpUtil.cpp
# Install
The source codes are written in ANSI C/C++, which are cross-platform and can be compiled on Windows, Linux/Unix and Mac OS. NOTE: Double-clicking the executable program does not work for data downloading. The correct way is to type ‘run_GAMP_GOOD  gamp_good.cfg’ on the terminal.
# How to use it
Please download GAMP II - GOOD Users Guide FROM https://github.com/zhouforme0318/GAMPII-GOOD/tree/master/Doc. The detailed description of how to use GAMP II - GOOD can be found in the document.
# Attention!
If some GNSS data cannot be successfully downloaded, check your network first. Then, check if the file exists on the remote server. Finally, rerun ‘run_GAMP_GOOD  gamp_good.cfg’ to try again. Don’t worry, once the data has been downloaded, it will not be downloaded repeatedly.
# Acknowledgments
- Dr Cheng Wang @ BUAA provided the C++ code for observation downloading as reference
- the suggestion of adding the option of the day before and after the current day for precise satellite orbit and clock products downloading is provide by Zan Liu @ CUMT
- the suggestion of adding the option of the full path of third-party softwares is provide by Dr Yudan Yi
- the suggestion of adding the option of rate of TEC index (ROTI) is provide by Lei Liu @ WHU
- the suggestion of adding the option of 'printInfoWget' in configure file for (not) printing the information of running 'wget' by Dr Hong Hu @ AHU
- the suggestion of adding the option of IGR, IGU, GFU (from GFZ), and WUU (from WHU) products as well as the site-specific broadcast ephemeris is provide by Menghao Li @ HEU
- the suggestion of adding the option of various ultra-rapid (i.e., from ESA, GFZ, IGS, WHU) and rapid (i.e., from CODE, NRCan, ESA, GFZ, IGS) orbit and clock products downloading is provided by Sermet Ogutcu @ Necmettin Erbakan University
- the suggestion of modifying the setting of directories of GNSS observations and products in configure file is provided by Caoming Fan @ SDU
# License
Copyright (C) 2022 by SpAtial SurveyIng and Navigation (SASIN) Group at Shandong University of Science and Technology (SDUST), all rights reserved.
