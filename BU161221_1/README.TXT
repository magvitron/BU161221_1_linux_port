CTS50726.F00
	:sdt:26072005:1130
	1. This first executable version of CTS Project. Just Skeletone of
	   menu is completed (i.e. First screen).
CTS50731.F00
	:sdt:31072005:1645
	1. SKeleton of dialog boxes for handing the specification is completed.
CTS20731.F01
	:sdt:31072005:2300
	1. ReadParamDotHdr() & ReadParamDotCts() routines are added.
	   These routines are kept in Share.Cpp file. Also Share.h header file is
	   copied from FXS20811 Project.
	2. Variable for PRM is declared. This structure used to differentiate
	   LF & HF parameters, as well as to find out testing method for
	   parameters.

	:sdt:01082005:0000
	1. Static variables declared to find out current selected parameter, Frequency &
	   combination setting.
	2. PARAMINFO structure used to create Dialog box to select parameter.
	   It will take first 14 parameters to display them in Dialog Box.

CTS50802.F00
	:sdt:06082005
	1. Specification Creation, editing & remove part completed.
	2. Dialog boxes used for handling the specification are defined in
	   Speck.cpp file and classes are defined in Speck.h file.
	3. Functions used for limits handling are defined in Limit.CPP.
	4. ReadINI.CPP added to read values from INI files.
	5. Funcitons to read Param.CTS & Param.hdr are added in Share.CPP.

CTS50807.F00
	:sdt:09082005
	1. Problem storing limits for HF frequencies is solved.
	   "selfreq->select" variable increamented by Total number of LF Frequencies.

	:sdt:11082005
	1. Parameter selection for Run is almost completed. Fine tuning such as
	   0.08 Khz should not selected for Hf Frequency is remained.

CTS50807.F01
	:sdt:12082005:2300
	1. 0.08Khz selectio problem for HF parameters is solved.
	:sdt:23082005:2300
	2. Run Setup selection is completed. Radio Button for selection of
	   fixture is added. Check boxes for selection of Break, Short, Single
	   Step & Zch Measeured for ATN/XT are added. Also Inputline to take bypass
	   pairs is added.

CTS50824.F00
	:sdt:24082005:0040
	1. RunSetup structure is created & defined in "Setup.h". This
	   structure is used to store Run Setup settings. This is declared as
	   global variable. Code added for setting & reading values of
	   Fixture Selection, Fanout, Single Step, Zch Measurement & Bypass Pairs.

CTS50825.F00
	:sdt:25082005:0100
	1. Run Setup Dialog box handling is completed.
	:sdt:25082005:2330
	2. Global Variables declared are moved in "GBLVAR.h" file. As it required
	   this variabled declare in other CPP Files with modifier "extern".
	   "GBLVAR.H" file is included only in "CTS.CPP".
	3. Function prototypes moved to "FUNPROTO.H" header file. This header file
	   is included in other .CPP files.

CTS50826.F00
	:sdt:26082005:1600
	1. Code for handling Config Dialog box is added.
	2. New "Config.cpp" file added in project.
	3. TConfig class declared in "cconfig.h"
	4. GetPrivateProfileString(), GetPrivateProfileInt() & other utility functions
	   moved to "Utils1.cpp" file.
	5. Code handling No. of Pair selection & Pairs in a unit is not added.


CTS50901.F00
	:sdt:06092005:2330
	1. Config Dialog - Code handling for Total Pairs & Pairs in unit is added.
	2. Classed added for handling of above code in "ccconfig.h" & it
	   are TPairDialog & TUnitDialog.

CTS50907.F00
	:sdt:07092005:1920
	1. Config Dialog - Code added for Cable type & Cable sequence.
	2. Unwanted code removed from "config.cpp" & "cconfig.h".

	:sdt:09092005:2300
	1. Code added for reading frequency list from "cts.ini". Code added
	   as new function "ReadFreqList()" in "readini.cpp" file.
	2. Above function called from "cts.cpp" & through function "InitSystem()".

	:sdt:12092005:2325
	1. Code required for initialise the system is added & it is taken from
	   sandy cable software.

CTS50921.F00:
	sdt:21092005:1200
	1. Code modified between the duration 13092005 to 20092005 was deleted
	   by mistakely.
	2. Started the modifications from the version "CTS50907.F00".
	3. Cable sequence read from Specification while reading default config
	   values from "Default.ini".

CTS50927.F00
	sdt:27092005:1325
	1. Code added for setting pre conditions for testing.
	2. Most of code is in RUN.CPP & some in Share.CPP.

CTS50927.F01
	sdt:27092005:1930
	1. Code added for testing a parameter CM. Created a new file as CMTEST.CPP.
	2. Code required for accessing hardware while testing a parameter is added
	   in Share.CPP.

CTS50927.F02
	sdt:27092005:1150
	1. Code required to show the Test results is added.
	2. Code added in "Frontpg.CPP" & "Frontpg.h" Files

CTS50927.F03
	sdt:27092005:0000
	1. Problem of Unit size display at results is solved.

CTS50928.F00
	sdt:28092005:1200
	1. Instead of using messageBox to display messages, it are shown on
	   status display bar.
	2. Warnings for redefination of some of constants are removed by
	   commenting constants in one of header file.
	3. "Specifiacation" variable is removed.

CTS50928.F01
	sdt:28092005:1315
	1. Code added for paramter "GM" in "GMtest.cpp".
	2. In Frontpg.cpp, starting Reading no., Current reading no., ending
	   reading no. is added in status bar.
	sdt:28092005:1500
	1. Code added for parameters CUPG & CUPS in "Cupgtest.cpp" & "Cupstest.cpp".

CTS50928.F02
	sdt:28092005:1705
	1. Code added for Parameter CR in "CRtest.cpp".
	sdt:28092005:1750
	2. Code added for Parameter CUPP in "Cupptest.cpp".
	sdt:28092005:1835
	3. Code added for Parameter CUPPI in "CUPITEST.cpp".
	4. In Frontpg.cpp units are displayed with readings & modified for
	   Pair to Pair parameters.


	sdt:30092005:1930
	This version is freezed for general. Further development done 
	specifically for quad. Quad software is named as "QXX".

QXX50930.F00
	sdt:30092005:2105
	1. Unwanted paramters removed. "param.cts" & "param.hdr" are
	   modified using "addprms" utility.
	2. Some constants value chaged.
	   i.e. MAXPARAMTERS, SRNO_X modified for Quad software.
	3. Extra parameters removed from Runsetup & Specks Dialog Box.
	4. Code with  MAXPARAMETERS+/-x (where x is any numerical value) is
	   modified to MAXPARAMETERS. In previous case there where extra parameters
	   were present such as ZMS, V1V2 which are considered internally only.
	   these paramters removed in this Quad software.
	5. Specification created for Quad Software.

QXX50930.F01
	sdt:30092005:2335
	1. In Run Setup parameters are divided as LF & HF Parameters.
	   i.e. they are shown in two different groups of checkboxes.
	2. Fanout & ZCH Measured options removed from Run Setup.
	3. HF parameters can be selected only if "both" fixtures are selected.
	   Otherwise only LF parameters will be selected.

QXX51001.F00
	sdt:01102005:1300
	1. Total frequencies reduced to 8 from 10.
	2. Corrosponding changes done for Specification & Run Setup.
	3. RU display removed from "Frontpage.cpp" as it is not required in
	   Quad.
	4. Condition for HF parameters can not select lowest LF frequency
	   removed from code.(see "Specks.Cts" & "rsetup.cpp")
	5. Specks files recreated, as total frequencies are changed.
	6. ZCH (ZCAL) is made as LF parameter.

QXX51001.F01
	sdt:01102005:1425
	1. Combination selection setting removed from Speck & Run setup dialog
	   box. (See changes in "Specks.cpp" & "rsetup.Cpp".
	2. "ALL_COMBINATIONS" Tag added in Sequence files. These combination
	    used for testing Xtalk paramters.
	3. Code added in "Cblseq.Cpp" file to read the "All_Combinations" from
	   ".Seq" files.
	4. Code added for counting readings of "All_Combinations".
	5. Pair switch option changed in "Default.ini" for Pair to Pair parameters.
	6. Calculation of Total number of reading for particular parameter is
	   modified in "Fillparaminfo()" function in "share.cpp" for Xtalk
	   Parameters. (Added only for "Fext" & "Next"
	7. Limit index for all paramters is taken as 0. As there are only
	   single pair combinatios are present for all parameters in Quad
	   combination.
	   Changes made in following functions "GiveAlarms()" & "GetLimitIndex()"
	   in "Share.cpp".

QXX51001.F02
	sdt:01102005:2350
	1. ZCH (ZCAL) parameter is added.
	2. MAXFREQUENCIES constant changed in ADDPRM. "Param.cts" & "param.hdr"
	   recreated and copied in QXX project.

QXX51002.F00
	sdt:02102005:1400
	1. ATN parameter is added.
	2. Changes made for sandy cable software removed from ATN routines.

QXX51003.F00
	sdt:03102005:2305
	1. Conditions added for ZCH & ATN to select CR & CM for Low frequencies.

QXX51004.F00
	sdt:04102005:2355
	1. NEXT Parameter added.
	2. SelectFixtureXTalkInQuad() function added to select fixture for
	   Xtalk Parameters.
	3. Prevously Adjacent Pairs, NonAdjancet Pairs & Both combinations were
	   considered in summary calculation. These are removed. Only single i.e.
	   ALL Combinations cosidered for Xtalk paramters. According to this
	   summary calculations are stored.
	4. FEXT Parameter added.
	5. Changes mentioned in 3 are also implemented for FEXT.

QXX51004.F01
	sdt:05102005:0035
	1. Changes mentioned in sdt:04102005:2355 is impleemted for CUPP & CUPPI.
	2. For CUPP only Adajcent Pair combination is considered.
	3. For CUPPI only Adajcent Unit combination is considered.

QXX51006.F00
	1. Code added to create & save testing result in a file.
	2. As "system" function is not working which hangs the program.
	   Therefore instead of using "system" function for coping ".use"
	   file from Ramdrive to Data Directory, "PrepareFile()" function
	   modified. Now this function will take a file path as an agrument and
	   create a file at a given path.

QXX51008.F00
	sdt:08102005:1825
	1. Code added for initialise the reports. Code added in "run.cpp".
	2. "speck.Type" set to '0' in "Specks.cpp", as "Type" field used to set
	   PCM cable option which is not required.
	3. "FileView.Cpp" & "fileview.h" files added in code to display the
		report file.

QXX51008.F01
	sdt:08102005:2345
	1. Changes started in "Disprep.CPP" file for Quad Software.

QXX51009.F00
	sdt:09102005:2320
	Following changes made in "Disprep.cpp" for Quad software.
	1. MAXPARAMETERS, MAXFREQUENCIES, MAXLIMITTYPES etc. constants changed
	   as per quad requirement.
	2. CONFIG, STRUCT_SPECK structures changed as per Quad software.
	3. Bugs in writting data in ".USE" File & reading data from ".USE"
	   file removed.
	4. USER ENTRY TITLES stored in INFOREMATION structure from Config
	   structure in "run.cpp".
	5. Line displaying Normalising Temperature in case of CR & Zch in case
	   of Xtalks removed.

QXX51009.F01
	sdt:10102005:0035
	1. "Units#" where displayed in detailed report is removed.
	2. In summary report condition of pair adjacency modified for Xtalk
	   parameters as ALL_COMBINATIONS are considered for Xtalks.

QXX51011.F00
	sdt:11102005:2230
	1. Complier options changed. Made as per HVIR project.

QXX51012.F00
	sdt:12102005:1425
	1. Disprep modified. In detailed report frequencies of a parameter
	   are displayed differently. Previously it are shown in group.

QXX51012.F01
	sdt:12102005:1900
	1. Disprep Modified. In detailed report Normalised & Unnormalise readings
	  are displayed separately.
	2. "nNormUnnormMatrix[][]" variable added. Which is used to store the
	  type of reading.

QXX51013.F00
	sdt:13102005:2310
	1. ".MMT" files modified for all paramters. "nReadings" value put as
	   it required for Quad.
	2. Disprep Modified for following points.
	3. Readings names added for each parameter.
	4. Instead of "Pair", "Quad" is displayed in Parameter Header.
	5. Instead of pair numbers, Unit number is displayed as Quad for all
	   parameters except Xtalks.

QXX51014.F00
	sdt:14102005:1055
	1. "Disprep.exe" is called from CTS project.
	2. "swanlp()" function used to call disprep.exe.

QXX51014.F01
	sdt:15102005:0205
	1. "Disprep" utility added in QXX project.
	2. It will show the readings in normalised & unnormalised format.
	3. "createReport()" will create text file for report in ramdrive
		which is then displayed.

QXX51015.F00
	sdt:15102005:0815
	1. SINGLESTEP is already implemented.
	sdt:15102005:2230
	1. Changes started for Runsetup.ini.

QXX51016.F00
	sdt:16102005:1100
	1. "RUNSETUP.INI" file added to set parameters & its frequencies
	   for test.
	2. Encrypted name of file is "ERNSETUP.INI".
	3. File contains following options e.g.
	   CR=1,0,0,0,0,0,0,0,0
	   it should be interpreted as
	   PARAMNAME=PARAM SELECTION,FREQ1 SELECTION,........,FREQ8 SELECTION.
	4. Condition used for forceful deselection of lowest LF Frequency
	   for HF parameter is removed.
	5. While selecting LF frequencies for HF parameter, First HF frequency
	   is also getting selection. Masking added for only LF frequencies
	   should be selected.

QXX516016.F01
	sdt:16102005:1500
	1. Report Setup implemented. Report Setup created in "REPSETUP.CPP".
	2. Default values for report setup are read from CTS.INI.
	3. Find these values under header "[REPORT_SETUP]" in CTS.INI.
	4. Report setup values used to create a report in "RUN.CPP" in a
	   function "PrepareFileForPrint()".

QXX51016.F02
	sdt:16102005:1815
	1. Problem :- When all desired frequencies for ATN selected, detailed
	   report was not displayed for ATN.
	   Solved :- Area required calculation modified. Now it will not
	   consider how many frequencies are selected.

QXX51016.F03
	sdt:16102005:2235
	1. Changes made in Report Setup. Only Normalised or Unnomalised
	   report will be displayed. Due Low heap memory problem full report
	   (with Normalised & Unnormalised) can not be displayed.

QXX51016.F04
	sdt:17102005:0120
	1. PRNREP Utility added. Chnages made in PRNREP as per in DISPREP.
	2. Code added to call PRNREP from main project.
	3. PRNREP called through "printReport()" function declared in
	   "RUN.CPP".
	4. Testing is not done for "PRNREP" utility.

QXX51017.F00
	sdt:17102005:2030
	1. PRNREP Utility tested. Changes made in to get proper report.
	2. Size of "SpeckList" variable is modified.
	3. Impedence displayed in summary of HF parameters is removed.
	4. Condtions modified for MAXPARAMETERS from MAXPARAMETERS-1.

QXX51018.F00
	sdt:18102005:2345
	1. File menu implemented.
	2. All functions for File menu handling are in CTS.CPP file.
	3. "Runall" menu implemented. Thosed parameters which are defined
	   in "RUNSETUP.INI" are tested using "Runall" option.
	4. PRNREP & DISPREP modified for following points.
	   a. In summary header "PAIR" is replaced with "QUAD" for all
		  parameters except Xtalk.
	   b. In Summary "No of Pair(s) tested" is replaced with
		  "No of Quads tested".

QXX51018.F01
	sdt:19102005:0045
	1. For LR "(Mx-Av)%" reading added in summary.
	2. To implement this changes made in DISPREP & PRNREP.
	3. "RemoveFilesFromRamDrive()" used to remove .INI & other files
	   from Ramdrive & not the "RemoveINIFiles()". Hence removal of
	   "sat.txt" (i.e. Output of disprep) is moved in "RemoveFilesFromRamDrive()"
	   function.

QXX51101.F00
	sdt:01112005:1715
	1. Function for reading ".USE" file modified. Previously for reading
	   param.cts file a pointer variable was used in sizeof(), which is
	   replaced with structure name.
	2. Draw function for frontpage modified. It will draw a whole window
	   for everyreading.
	3. Project name modified as per dates standard.

QXX51104.F00
	sdt:04112005:1530
	1. StopPrint() function added in Share.cpp file. This function used
	   to remove data present in buffers of "Pcache" & stop the printing
	   instantly.
	2. Problem of 5Khz frequency of CM get activated after first test file.
	   (Default frequencies of LF & HF restored in nConfig.nLFrequency &
	   nConfig.nHFrequency variables.)

QXX51105.F00
	1. Comparision condition for both fixtures changed. Only case of string is
	   changed in strcmp() function.
	2. Switching pattern is changed for 'C' parameters. Source firing done
	   for both fixtures when "Both" fixtures are selected.

QXX51108.F00
	1. Calibration report coding is started.

QXX51109.F00
	1. Calibration report as per FXS software.

QXX51109.F01
	1. Calibration report Created. Fine tuning of it is remaining

QXX51110.F00
	1. Unwanted variables in calibration report removed.
	   e.g. HF_Offset[][],fReadStd[][];
	2. Parameter V1V2 replaced with ATN.

QXX51110.F01
	1. Code added for Print calibration report.

QXX51111.F00
	1. Security code added. Installation & Uninstallion code added.
	   Both code added in new file "Security.cpp".
	2. Argument for EXE is changed from "siva" to "ePt" in Ects.ini
	3. Changes made for New serial in following utilites -
	   CTSD, CTSE, FIXTS, SYSTS, ADDPRMH.
	4. Also FIXTS & SYSTS modified for Quad software i.e. MAXPARAMETERS &
	   MAXFREQUENCY variables modified as per Quad software.

QXX51113.F00
	sdt:13112005:2230
	1. Bug of reading CALLOW & CALHIGH reading from CTS.ini is removed.
	   These values used in calculation of Cal Factor & Offset.

QXX51115.F00
	sdt:15112005:1530 (13112005)
	1. Unwanted variables & functions are commented to reduce size of Exe.
	2. Specklist[][] variables size reduced to 10 from 30.
	3. Above variable changes are also carried out in PRNREP & DISPREP.
	3. String in Startup.bin changed from "Sivananda" to "ePoint".
	4. Common Select Fixture function added for Single pair parameters
	   (CR, CM, CUPG, ATN) and individual select fixture functions removed.
	   Added in Share.cpp by name SelectFuxtureSinglePair().
	5. All changes done on 15112005 are mistakely written as 13112005 in
	   comments.
	6. Classes & Routines used for handling combination (in case of pair
	   to pair parameters) removed from RSETUP.cpp & Speck.cpp to reduce
	   size of EXE file.

QXX51119.F00
	sdt:19112005:2215
	1. Arguments of fuction which is used for calling "PRNREP.EXE" modified.
	2. PRNREP UTILITY tested, found working fine.
	3. "install" option modified. system() command used to copy all files to
	   harddisk is modified. Now only Main EXE file will be copied to
	   destination, supporting files has to copy seperately.

QXX51119.F01
	sdt:19112005:2250
	1. "uninstall" option modified. system() command used to copy all files to
	   harddisk is modified. Now only Main EXE file will be copied to
	   destination, supporting files has to copy seperately.

QXX51119.F02
	sdt:19112005:2300
	1. "install" & "uninstall" codes modified. Now program will copy all
	   files at source to destination instead of only Main.exe.
	sdt:20112005:0105
	2. A bug in Runsetup & Speck menu is removed. While every time opening
	   "Param.cts" file it will initiate "ParamCtsHandle" variable.

QXX51120.F00
	sdt:20112005:1200
	1. "install" & "uninstall" routines tested. Now both options are working
	   fine.

QXX51122.F00
	sdt:22112005:1900
	1. CR & CM test routines which were temporary commented, are uncommented
	   now. Again "install" routines tested again found working fine.

QXX51122.F01
	sdt:22112005:2215
	1. Bug - When "exit" option selected in File Menu it is giving error
	   "Heap Corrupt" & computer will be hanged.
	   Solution - "exit" call added in "exitSystem()" routine. Also code
	   added to save a data file if it is not saved before exiting software.
	   Thus now "exit" option working fine.

QXX51123.F00
	sdt:23112005:2235
	1. FEXT & NEXT parameters where not tested as "SEQ" file was not properly
	   read, when testing done after modification in config menu.
	   Bug removed - After reading config Sequence file name also stored in
	   "Config" structure.
	2. "OUT of LIMIT" alarm for CR test. Removed - This was given for RU.
	   As RU is not a parameter in Quad, that part of code is commented.
	3. Sequence file "2.Seq" modified for 4-Quad. Sequence name is changed from
	   "6-QUAD" to only "QUAD" for general QUAD.

QXX51123.F01
	sdt:24112005:0100
	1. "Super Unit no." entry removed from Config Menu. It was Title entry
	   therefore just removed from "default.ini" file.
	2. RMS speck Limit printing problem. Hardcoding done for FEXT parameter,
	   so that higher range will not be displayed or printed in a report.

QXX51124.F00
	sdt:24112005:0700
	1. Changes made storing sequence file name in "config" structure.

QXX51124.F01
	sdt:24112005:2255
	1. Instead of storing sequence file name in "config" structure,
	   speck file read before testing & copied sequence file name
	   from "speckData" structure to "config" structure.

QXX51125.F00
	sdt:25112005:2330
	1. FEXT testing pattern (i.e. method of reading V1 & V2) is made
	   same as NEXT. This changes done to avoid toggling between last & first
	   range.
	2. F7 => ResetSystem() - modified. Now it will not consumes memory
	   if F7 is pressed.
	3. Run count calculation modified in security routines. Previoulsy
	   it was a cable length count in LCKM. Now it is count for number of
	   files tested.
	4. Now file must be saved before printing. This is done to avoid
	   printing more than runcount.

QXX51126.F00
	sdt:26112005:1950
	1. Report header checked found almost same as Old Quad software.
	2. Inter / Intra unit pair combinations printed in summery of
	   pair to pair parameters is removed.
	3. File must be tested before report will be displayed / Printed.
	   Condition added for same.

QXX51126.F00
	sdt:26112005:2350
	1. Systs & Fixts utility modified & tested for this quad software.

QXX51127.F00
	sdt:27112005:0930
	1. ATN Look up table added for 0.9 guage. Chages made in code to
	   read it properly.

QXX51127.F01
	sdt:27112005:1500
	1. ZCH calculation modified. Frequency value was not properly taken.
	   Frequency value read from frequency list in CTS.INI. Now onwards
	   frequency unit must be written in Caps. (i.e. KHZ / HZ not as khz/hz).

QXX51204.F00
	sdt:04122005:2120
	1. Routine used to read FIX.OFF is modified. Hard coding used for unit
	   size removed and it will take directly from Config.
	2. Room temp not properly stored in CONFIG structure, modified. Its
	   required in normalised calculation of CR & ATN.
	3. ATN at calculated frequencies 1 Khz, 1.6 Khz & 3.4 Khz is giving
	   wrong results. Modified. Frequency values not perfectly interpreted.

QXX51204.F01
	sdt:04122005:2345
	1. No. of pairs option in Config menu modified as No. of Quads.
	   Internally it considered as no. of pairs only. Only at point of
	   displaying such as Config menu, Display & print report it is shown
	   in term of Quad.
	sdt:05122005:0055
	2. Cal Report SHOW_REMARK option implemented by View Calibration report.
	   It will read SHOW_REMARK option from CTS.ini and according to that
	   it will either display or not display the remark.

QXX51205.F00
	sdt:05122005:2320
	1. Average remark shown NOTOK. Modified. Bug at comparing upper limit
	   with normalised value.
	2. View & Print calibration report option in Utility menu will be
	   activated only agruments given with main exe file.
	3. Text "ePoint Technologies" added at end of screen.

QX512070
	sdt:07122005:2100
	1. Version infornation format is changed.
	   Previous Format
	   Q - Software Type
	   X - Client's Initials.
	   X - Type of System.
	   Next five digits for Date of software release.
	   .FXX - For Number of release on that day.

	   New Format
	   Q - Software Type
	   X - Client's Initials
	   Next five digit for date of software release
	   x - Last digit for number release on that day.

	2. New offset type for Parameter ONEXT for Adajacent pairs combination
	   is added. It is added in Ects.ini under header [ONEXT_OFFSET] and
	   at line [ADJ_PAIR_ONEXT_OFFSET_VOLTAGE]. Now while testing ONEXT
	   it will check for adjacent pairs & delete the offset from V2 instead of
	   taking it from Fix.off. For other combinations it will take from
	   Fix.off.

QX512080
	sdt:08122005:1100
	1. Bug - Overwriting of saved files. Rwmoved. Now it will prompt to
	   select New File when tried to test saved file again.

	2. Different user level of operators implemented.
	   Now OPERATOR level does not have Speck management power. Only Superuser
	   & Epoint Engineer can do the speck management.

QX513081
	sdt:08122005:1700
	1. Reset problem  when config menu opened 3 times. Solved. fstream files
	   remained open while reading seq files. That cause reset. Close function
	   added.

QX601040
	sdt:04012006:2300
	1. Background is changed. Now "ePT" word is displayed in background.

QX601050
	sdt:05012006:2000
	1. New offset type for Parameter FEXT for Adajacent pairs combination
	   is added. It is added in Ects.ini under header [FEXT_OFFSET] and
	   at line [ADJ_PAIR_FEXT_OFFSET_VOLTAGE]. Now while testing FEXT
	   it will check for adjacent pairs & delete the offset from V2 instead of
	   taking it from Fix.off. For other combinations it will take from
	   Fix.off.
	2. Speck managements memory handling bug removed.

QX601051
        sdt:05012006:2030
        1. Security routines changed. After expiry of runcount, instead of
           exiting from program, it will not display or print the reports.

QX602230
		sdt:23022006:2250
		1. As the latest version QX601280 is mot tested, previous tested
		   version is taken.

		MOD2302200601
		SUMMARY Options seperated for LF & HF frequencies. Changes made in
		DISPREP & PRNREP Utilities and DEFAULT.INI file. MAIN EXE is not
		changed. Following changes are carried out in these files.
		1. Size of SumOpt[] variable modified to accomodate options for
		   both frequencies for perticular parameter.
		2. SUMMARY_OPTIONS modified in DEFAULT.INI. Now instead of one entry
		   for a parameter there are two entries, one for LF Frquencies and
		   another for HF Frequencied.
		3. Changes made in DISPREP & PRNREP Utilities to read modified options.
		4. HFFREQUECYINFO structure added & used to store starting and ending
		   number for LF & HF frequencies.
		5. Depending upon frequency under consideration Summary is displayed
		   and printed.
		Find above changes in DEFUALT.INI, DISPREP.CPP, DISPREP.h, PRNREP.CPP,
		PRNREP.H.

QX602280
		sdt:28022006:2150
		DISPREP, PRENREP utilities and DEFAULT.INI added in QX601280 version.

QX602281
		sdt:28022006:2230
		MOD2802200601
		1. Constant declared for Adajacent Pairs in Cable in Share.h file.
		2. Following changes made in Share.CPP
		   a. Code for counting total number of readings for Adajacent Pairs in
			  cable option selected for Xtalk Parameters.
		   b. Assignment of starting pointer of first adajacent pair in cable
			  for Xtalk parameters in SelectFixture().
		3. Following Changes made in Cblseq.cpp
		   a. Code for reading Adajacent Pairs in cable is added.
		   b. Code to set free allocated memory is added for adajacent pairs
			  in cable combination.
		   c. Code added for counting total number pair combinations in case
			  of Adajacent Pairs in cable.

QX603080
		sdt:08032006:2355
		1. Bugs present in above modifications are removed.


QX603110:sdt:11032006:2255
		MOD1103200601
		1. Changes made in DISPREP.CPP, DISPREP.H, PRNREP.CPP & PRNREP.H files.
		   Changes made for Xtalk parameter's combination - Adajacent Pairs in
		   Cable.
QX603120:sdt:12032006:2320
		1. Changes made for Ajacent pairs in unit combination for XTalk
		   Parameters.

QX604230:sdt:23042006:1645
		Problem : In Summary report, MIN, MAX & other readings were displayed
		in pair format instead of Quad Format.
		Solution : Above problem solved. Changes made only in DISPREP.CPP
		& PRNREP.CPP files. That also in DispSummaryRep() function ONLY.

QX604240:sdt:24042006:2310
		Following Changes are made -
		Option in INI to set to test and report only Adjacent pair values  only of XT in two modes.
		e.g.
		NEXT=0  ,normal (All Combinations)
		NEXT=1  ,like in CUPP within Quad combination
						  i.e, 1-2, 3-4, 5-6, 7-8, 9-10, 11-12
		NEXT=2  ,1-2, 2-3, 3-4, 4-5, 5-6, ........ 10-11, 11-12

		FEXT=0  ,Normal
		FEXT=1  ,like in CUPP within Quad combination
						  i.e, 1-2, 3-4, 5-6, 7-8, 9-10, 11-12
		FEXT=2  ,1-2, 2-3, 3-4, 4-5, 5-6, ........ 10-11, 11-12

QX606120:sdt:12062006:2115
		Security portion was commented in previous version. Now comments are
		removed. Security is applied.

13062006-19062006:QX606190

QX606190:sdt:19062006:2215
		1. Over the above period a skeleton for calculation for X'talk
		   at Low Frequency is prepared.
		2. In this internally CUPP is measured for pairs under consideration.
		   Then this value of CUPP can be used to calculate the X'talk with
		   help of other paramters such as ATN & ZCH.

QX606191:sdt:19062006:2250
		1. Calculation for Xtalk at Low frequency is implemented in
		   generlised form. Previously it was implemented only for
		   FEXT.

QX606270:sdt:27062006:2230
		1. Code added for displaying formulas.
		2. Code added in Cts.cpp File under function "void TCts::formulaeDlgBox( void )".
		3. This function can be selected through "Utils" menu.
		4. A window will be displayed containing list of formulas for all
		   parameters.

QX606290:sdt:29062006:2320
		1. Code added for printing formulae.
		2. code added under menu "Print Formulae".
		3. Function for printing formulae was already present. It is made
		   active here.
		4. Testing of Print Formulae is prnding.

QX607270:sdt:27072006:2245
		1. Formula for calculation of XTalk at low frequency is implemented.
		2. Code added for Next calculation at LF.
		3. Unit for Xtalk at LF is hard coded while its calculation. This
		   hard coding only used in case of online reading display.

QX607280:sdt:28072006:2320
		1. Separate formulae implemented for NEXT & FEXT at LF.
		2. y = mx+c format implemented in calculations of Xtalk at LF.
		3. Multiplication Factor & Offset in CTS.ini under header
		   [LFXT_SETUP].

QX607290:sdt:29072006:2250
		1. View.Com is used to display the report.
		   Now it can also display the long reports also.

QX607300:sdt:30072006:2140
		1. Zch parameter added which was deleted earlier for debugging purpose.
		2. All ".Dat" files which are created in Ramdrive during testing are
		   removed while exiting from the program.

QX608010:sdt:01082006:2255
		1. The Changes made in ".MMT" files for displaying report in PAIR
		   format instead of QUAD.
		2. "Quad" word is replaced with "Pair" in Detailed Report's and
			Summary Report's header.
		3.  Unit number is displayed before starting readings for that Unit.
		4.  In the Config Dialog Box, "No of Quads" replaced with "No of Pairs"and
			"Pairs/Quad" with "Pairs/Unit".

QX608020:sdt:02082006:2320
		1. The formula used for the calculation LFXT is corrected.
		2. MAXPAIRS variable's value modified to 104.
		3. Modifications done for changing the Quad format to Pair format.
		   (i.e. In Quad, No. of Quad = No. of Pairs/2. In pair type of
		   representation this division by 2 is not required. This division
		   by 2 is removed for above change)

QX608040:sdt:04082006:2220
		1. Changed made in "PRNREP" utility. It is modified to represent a
		   report in Pair format instead Quad format.

QX608050:sdt:05082006:2155
		1. Unit and Decimal value for LFXT added in CTS.ini as it cann't
		directly taken from respective MMT.

QX608060:sdt:06082006:1300
		1. "Pairs in an Unit" entry added in the report header.
		   For above changed made in PRNREP & DISPREP Utility.

BX608110:sdt:11082006:2135
		1. Uncommented the Security code.
		2. New series of software is started. It will start with "B"
		   and used for Pair software.

BX610010:sdt:01102006:1630
	1. BUG : Log10 Domain error while testing Xtalk at LF.
	   Solution : As CUPP can be negative, Abosolute value of CUPP used in
	   calculation of Xtalk at LF.

BX610050:sdt:05102006:2030
	 1. Fomula for calulating Xtalk at low freq. is modified.
		Cupp value converted into Farads, which was in Pico Farads (pf)
		previously. (i.e. multiplication factor 1.0e-12 is added.

BX610051:sdt:05102006:2330
	 1. BUG : sqrt: DOMAIN ERROR & log10: OVERFLOW ERROR
	   Solution : As CM value can be 0 or less than 0, which is causing
	   above errors. Modification is made as, if CM's value is 0 or less
	   than 0 it is taken as 0.01 nF.


BX610052:sdt:05102006:2350
	1. For Fext at Low frequency, formula is implemented as below:
	   Fext at LF = MF*(Commom calculated value)+(8.686*factor)+Offset.
	   To implement this formulae are rearranged. See FEXTTEST.cpp.

BX610060:sdt:06102006:2145
	1. BUG: While calculating Xtalk at LF, Cupp was measured at
	   different low frequencies instead of at 1Khz.
	   Solution: Bug fixed. Cupp measured Default low frequency i.e. 1 Khz.

BX610061:sdt:06102006:2315
	1. BUG: In un Normalised report wrong readings displayed for FEXT & NEXT
	   at low frequency.
	   Solution: Raw/Un normalised reading is also stored in fRawReading
	   variable.

BX610100:sdt:10102006:2150:Important Modification
	1. BUG: Readings of  NEXT at 150 KHZ are getting incorrect.
	   Solution:Before measuring NEXT at 150 KHz, we calculates Next for Low Frequencies.
	   While calculating this, CUPP is measured in background & its value used
	   in calculation. While measurement of this CUPP, calibration is done.
	   Due calibration it overwrites the offset measured while measuring ATN.
	   Thus CUPP offsets were used in measurement of Next and giving wrong results.
	   To remove this bug, calibration of ATN done before measurement of NEXT at
	   150 Khz. This will again measures the offset and use it in measurement
	   NEXT at 150 KHz. See the changes in NEXTTEST.CPP.

BX610110:sdt:11102006:2205:
		1. Code used for nomalisation of NEXT below cable length 300 m is
		   commented. This done as per Telecon with Mr. Sudhakaran.
		2. "View Formulae" & "Print Formulae" options hence further will
		   be enable in "SuperUser" mode OR "EpointEngg" mode of S/W. It are
		   disabled for "Operator" mode.

BX610120:sdt:12102006:1930
		BUG: Giving incorrect error for Xtalk combination 12-1 at LF.
		Solution : In calculation Xtalk at LF, CR & CM values taken from
			arrays fCrPairVal & fCmPairVal. Index used to read this was
			CtPair.Pair1, which was actually gives CR & CM values for
			next reading. That causes problem while reading CR & CM for
			last pair (i.e for reading of combination 12-1). Hence index
			reduced by 1, which now giving the correct results.


BX610160:sdt:16102006:1210
		1. BUG : sqrt: DOMAIN ERROR & log10: OVERFLOW ERROR
		   Solution : If CR value is 0, which is causing
		   above error. Modification is made as, if CR's value  (LR1 + LR2)
		   is 0 it is taken as 0.2 Ohms.

BX611010:sdt:01112006:2132
		1. Chnages made in QUAD software between 01102006 to 16102006
		   implemented in this software. These changes are mentioned above
		   according to date wise.

BX611030:sdt:03112006:2125
		1. Parameter RU is added. Code used to calculate, to check, to add
		   a parameter is uncommented. These Changes carried out in Frontpg.cpp,
		   Share.cpp, Crtest.cpp & Run.cpp on 02112006 & 03112006.

BX611031:sdt:03112006:2255
		1. ATN, NEXT & FEXT will work for low frequencies even if inner or
		   outer fixture is selected.
		   See the changes in Rsetup.cpp & Run.cpp.

BX611050:sdt:05112006:2130
		1. Now Pair information also displayed in Error dialog box.

BX611060:sdt:06112006:2230
		1. Code added for Print Calibration report. This will take STD &
		   DEVIATION values from Cts.ini. But this function is not used yet.
		2. Now Print Calibration will be available for SuperUser & ePointEngg
		   without giving agrument to EXE file. This will submit without
		   deviation Min & Max values. If EXE called with argument it will
		   print report with Deviation Min & Max values.

BX611070:sdt:07112006:2220
		1. End Remark is implemented. Neccessary changes made in DISPREP.CPP
		   & PRNREP.CPP files.
		2. Option to Enable/Disable End remark is given in CTS.INI under
		   header [REPORT].

BX611080:sdt:08112006:2150
		1. Calibration report printing modified. Calibration report for
		   party is implemented. PrintAutocalRep() function used to print
		   calibration for party.
		2. Previous Code which was used to print calibration report in above
		   is commented.
		3. Now Print calibration report utility will also work in Supervisor/
		   ePointEngg mode even if argument is not given. In this case it
		   will print calibration report required for party.
		4. When Calibration report utility used with ePt argument it
		   will print calibration report for us. Which can be used to
		   diagnose the system.

BX611080:sdt:09112006:2330
		1. Break & short (Fanout) test is added. Fanout tests can be selected
		   from RunSetup dialog box.
		2. This Fanout option is enable only is "bsn.exe" file is present
		   in current directory.
		3. RU parameters reading are displayed two times. Code modified to
		   remove this bug.

BX611110:sdt:11112006:1745
		1. Normalisation temperature will be displayed in the detailed report
		   for parameters CR & ATN. Changes made in PRNREP.CPP & DISPREP.CPP.
		   Its selection option present in Disprep.ini.

BX611111:sdt:11112006:2110
		1. Initialisation Code added for Break & Short. If "Bns.exe" is
		   present in current directory, it automatically select the Break
		   & Short test else test will be deactivated.
		2. Config information get resetted every time when config Dialog Box
		   is opened. This bug is removed.

BX701310:sdt:31012007:2330
		1. Modification done for Normalisation of raw readings at different
		   length for different parameters.
		   a. STRUCT_SPECK Structure modified for normalisation length.
		   b. Changes made in specification dialog box for normalisation
			  length.
		   c. Changes made wherever length normalisation variable is used.
		   d. Dialog box showing raw reading & Normalised reading modified.
			  It will show normalised reading as "\Nm" instead "\km".
		   e. DISPREP & PRNREP utilities also modified for above changes.

BX702030:sdt:03022007:2225
	   1. Code added for WPN for all Parameters. Testing is remaining.
BX702031:sdt:03022007:2240
	   1. Normalised & Unnormalised selection in Report Setup Dialogbox is
		  modified. Checkboxes used instead of radiobutton for choosing report
		  option. Now both Normalised & Unnormalised reports are displayed.
		  But Normalised & Unnormalised readings displayed seperately, i.e.
		  Normalised & Unnormalised reading not displayed in one line.

BX702040:sdt:04022007:1230
	   1. Changes made in DISPREP & PRNREP Utilities for displaying normalised
		  & unnormalised readings of a parameter together.

BX702041:sdt:04022007:1645
	   1. WPN parameters implemented for all parameters & testing is done.

BX702042:sdt:04022007:2345
	   1. Before starting "Short Test" OR "CR Test" when only one fixture is
		  connected, a message displayed for shorting the other end of conductor.
	   2. Length less than X meters should be considered as X meters, this
		  modification done for all meters. Minimum lengths are defined
		  in CTS.INI under header "[MINIMUM_CABLE_LENGTH]",

BX702060:sdt:06022007:2300
	   1. Parameters which are not selected from RunParam.ini will be Disabled.
		  If user try to select this parameters Error message will appear as
		  "X parameter is currenlty not implemented."

BX702130:sdt:1302007:0830
	   1. Modification which are done are reimplemented.Modification done for WPN calculation
		for all parameters. This modification if 1% of total reading is equal to one. Then
		there is chance of wrong reading stored in array. To avoid this condition used to
		store the readings in array are modified.

BX702160:sdt:16022007:1400
	   1. In testing of parameters CUPP, CUPG & FEXT at Low frequencies
		  with cable length more than 3000m, testing is shown at 150KHz.
		  Problem solved. Frequency number variable set to last Low freq.
		  for terminating loop used to test loop. That statement is commented.

BX702230:sdt:23022007:0730
	   1. Condition added to avoid unneccessary calibration for NEXT at Low
		  frequency.

BX904100:sdt:10042009:1545
	   1. Memory allocation problem is also solved. Changes can be seen in
		 SPECK.CPP. "delete lbs;" line added in findSpeckfiles().

BX9071300:sdt:13072009:2020
	   1. Problem: "Assertion Failed: heap check Error" & "Floating Point error"
		 while testing CUPP, NEXT and FEXT parameter.
		 Solution:- These three parameters consist of Worst Pair next calculations.
		 For finding Worst Pair Next we are maintaining a Dynamic array in each
		 parameter. While putting values in this array a "Search()" function is used
		 to find place to insert value in array. This function is written in
		 "Utils1.CPP" file. This function was not properly handling memory locations
		 i.e. it was going beyond limit if is to be added at 2nd and second last
		 position in array. One "if()" condition added to restrict it access
		 memory less than 0th location. To restrict from accessing location greater than
		 array length "floor()" function is used and one condition is added so
		 that arraysize will never become zero. This solves our array sorting &
		 value insertion problem.

BX907140:sdt:14072009:1215
    1.	Problem: When a cable tested and any summary reading of parameter is
		    gone out of range, user want to retest particularly that
		    parameter only. After testing this parameter user wanted to save
		    result of all tested parameters including retested parameter.
		    But this was not possible in our s/w as it was showing reports &
		    save the reports only for currently tested or retested parameter,
		    not for all.
		Solution: This problem is solved. As ".Dat" file of tested paramter will
		    remain in Ramdrive till any file is saved. To solve above problem
		    I have taken backup of "runSetup.ulParamRunSetup" variable after
		    testing. If user retest any parameter, before showing report and saving
		    file, I will restore value into "runSetup.ulParamRunSetup" variable from backup.
		    This will help me to save all parameters and show reports including
		    all parameters. Whenever file is saved Retest flag and variable
		    used for backup is reseted.
		    To see the changes refer RUN.CPP.
	2.	Field width to accomodate PIJF_0.63 in speck mgmt is increased.
	3.   Problem : System Offset was not applicable for ATN at Low Frequency.
		Solution: Code added for reading System Offset from Sys.Off for ATN
		at Low Frequency.
	4.   Modifications made for ATN Normalisation for ATN at LF. Now it is
		calculated from Raw Value of ATN instead of calculating from
		Normalised value of CR & CM.
BX907141:sdt:14072009
	1. Problem: Normalisation for ATN not working.
	   Solution : In User_ceil() function, temp variable was used instead of
	   CtPair.Norval. Modified.

BX907142:sdt:14072009:
	1. Lookuptable calculation added for ATN at Low frequency.

BX141001:sdt:01102014
	1. Requirement:- If Old saved file is opened, its Config should copy
	   and display into config window and same config is to be used for next
	   New File testing.
	   Solution: A new funxtion "copynConfigtoConfig(void)" is added in Run.Cpp.
	   This function is called from File Open Menu Option. It will Copy details
	   of nConfig structure to config structure.


BX141130:sdt:30112014:1615
	    1. Now MAXREPLACELEN is defined as 4 and pinReplace structure size
		  reduced to MAXREPLACELEN. i.e. Now Pin replacement details stored
		  serialy for four position instead of actual pinno position of pinReplace
		  structure. This help to redue size of pinReplace structure and easy
		  implementation of pin replacement logic.

BX150315:sdt:15032015:1615
	  1. Problem: In Calibration report, ATN calibration was done only for
		one freq i.e. default freq. It is required that Calibration to be
		done for all selected frequencies in Run setup.
		Solution: Condtion added on 08112005 is commented and original
		condition is restored.

BX150426:sdt:26042015:1700
	   1. Implemention for adding PIN Replacement Option in Utils
		 Solution: PIN Replacement option added in "Utils" Menu. Pin
		 Replacement has given option for three type of Fixture selection
		 i.e. Inner, Outer & Both. For each option user can replace 4 pins.
		 Code for Pin Replacement dialog is added in CTS.CPP & class defined
		 in Cts.h. Pin Replacement structure modified for three options.
		 Pin Replacement logic modified in share.cpp. In this which type of
		 Fixture is selected, that type of Pin Replacement will be done.
		 Changes made in CTS.CPP, Share.CPP, ReadIni.Cpp, cts.h, share.h,
		 gblvar.h, funproto.h.

BX150428:sdt:28042015:2100
		1. Alarms OFF Option added in RunSetup Menu. By selecting this option
		   alarms shown while testing will be bypassed.
		2. Alarms OFF option implemented for Alarms generated for Summary options.
		3. Chnages done in all parameters test.cpp files, rsetup.cpp, share.cpp,
		   Rsetup.h, Setup.H, Disprep.h, Prnrep.h

BX151208:sdt:28112015-08122015:0600
		Changes for making report same as older software for UM Cable Silvassa.

28112015 -Changes made in disprep for making report header as required for UM cables.
		The position of TITLE HEADERS adjusted as per old report. The LOW FREQUENCY
		and HIGH FREQUENCY added in report header.(see DISPREP.CPP)

03122015- DISPREP.CPP - REQUIREMENT - In summary report where specification values are mentioned
		for parameter (MIN, MAX, AVG, RMS, STD DEV), value must be printed and
		spec values and remark should not be printed.
		SOLUTION - In current report summary option is enabled fron DEFAULT.INI
		it will print its specks and remarks. So for all Parameters condition
		added to check specks value is BLANKVALUE or NOT. If it is BLANKVALUE
		then remarks should not be printed for it.
0712015 - Condition added for options which where left out such as MIN & MAX.

04-05122015 -SPECK.CPP - But in current software, when creating speck if speck
		value mentioned, it was taking as 0.0. It was causing confusion as
		limit is 0.0 or its BLANKVALUE. There for now a constant BLANKVALUE (99999.99)
		assigned to speck values which are not mentioned while creating Speck.

		The same BLANKVALUE used in DISPREP.CPP for summary remark cheking.

01&07082015 - PRNREP.CPP - Changes made is DISPREP.cpp are implemented in
		PRNREP.CPP.

BU160425:25042016:2130
	    1. This SW is branch of BX SW for UMCABLES hence name changed to BU
	    from BX.
	    2. Parameter testing sequence of CUPG & CUPP interchanged. As it is
		  requirement party to match it with its OLD SW. To implement below
		  changes are done.
		  a. Paramter sequence changed in ADDPRM\header.txt. Param.Cts &
			Param.Hdr are regenerated. It are copied in main SW.
		  b. SRNO_CUPG & SRNO_CUPP interchanged in CT.H, DISPREP.H, PRNREP.H.
			Main SW, DISPREP , PRNREP & SYSTS Utiltiy is recompiled again.
			Please regerated SYS.OFF once again.

BU160928:28092016
	   1. If any previously save ".USE" file opened, then in config
		 "Pairs/Unit" button is getting disabled.
	   Solution:- Problem was due to value of constants for "cmSaveDatFile"
	   and "cmPairsPerUnit" was same. When saved file is opened, then
	   "cmSaveDatFile" was disabled. As both constants having same value,
	   the "cmPairsPerUnit" command and button "Pairs/Unit" was getting
	   disabled. Now value to constants used for Config menu changed and
	   now issue is resolved.

BU161210:141000
	  1. Relaced Pin information saved in file "pinrplc.bin". Before opening
		Pin Replace Menu it will read Replaced pin information for this file.
		After updatin Replaced Pins in Pin Replace Menu, it will store info
		in "pinrplc.bin" file. Changes done in CTS.INI.

	  2. Saving of Config information. Config informaion stored in config.bin
		file. While initiating software it will read this file. if file is
		not there, then it will read config info from default.ini. If config
		info is updated through CONFIG Menu it will get stored in Config.bin
		file and used for next time while initialising softare.

		BOTH FILES are kept in same folder as of Main SW.

BU161212:2115
	  1. for cable length more than 32000 s/w was not working.
		Solution: Cable length in the sw is treated as integer value. Integer
		data type has value from -32000 to +32000. Hence Cable length data type
		taken as unsigned integer which having value from 0 to 64000.
		Changes made in Config.CPP, Run.cpp and CPP files of all parameters.

	  2. Cm Ramp detailed reading where not printed/ Displayed.
		Solution: While saving detailed reading, Frequency number was not
		stored for CM Ramp. Due to it can not read proper values from .USE file.
		Now frequency number stored while saving reading in .dat file for
		CM Ramp.

BU161214:12122016 onward
	 1. Checking of remaining memory before starting New File.
	 Solution: An option "MINIMUM_HEAP_MEMORY" added in CTS.INI under
	 Heading "BASIC_INFO". This minimum heap memory indicates minimum
	 memory required to run the s/w, below that limit s/w will not work
	 properly. While opening New File current heap memory will be compared
	 to this Minimum_heap_memory, if current memory less than it S/W will
	 stop and get exited.
	    Changes done in CTS.INI, CTS.CPP under procedure handling command
	 "cmNewDatFile".

	 2. Delay for CM (Ramp/80Hz) is to be seperated from CUPG and other
	 parameters.
	    Seperate 80Hz delays defined for CM under header "CM_80HZ_CTDELAY_INFO"
	 in CTS.INI. This delays are common for CM Ramp & CM@80Hz. Changes done in
	 CTS.INI and CMTEST.CPP.

	 3. Selection Option for CMRamp & CM@80Hz.
	 Solution: An option "RAMP_TEST" added under header "BASIC_INFO" in CTS.INI.
	 While testing higher length cable it will be checked and if this value is 1
	 then CM Ramp will be tested, if value is 0 then CM@80Hz will be tested.
	 Changes done in CMTEST.CPP.

BU161215:2200
	1. Saving of RunSetup Information. Run Setup information stored in
	"rnsetup.bin". This information is read while initialising software. If
	this file is not present then Run Setup information read from
	"RunSetup.INI". When Run Setup Information is updated in Run Setup Menu
	this information is stored in "rnsetup.bin" file.Changes done in CTS.CPP
	and RSETUP.CPP.

	2. Saving of Report Setup Information. Report Setup information stored in
	"rpsetup.bin". This information is read while initialising software. If
	this file is not present then Run Setup information read from
	"CTS.INI" through section "REPORT_SETUP". When Report Setup Information is
	updated in Report Setup Menu this information is stored in "rpsetup.bin"
	file.Changes done in ReadINI.CPP and REPSETUP.CPP.

BU161217:2130
	1. Saving the Status of how the software is exiting i.e. by pressing AltX
		or due to lack of memory.
	   While existing from software exit status store in file "exit.bin". If
	   Exit status is 1 means exiting due to lack or memory. If Exit status is
	   0 means exiting by pressing AltX.
		When sw is getting exited due to lack of memory a message will be
		displayed as "Refreshing the system...".

BU161218:12:30
	1. Cm Ramp reading/switching was not happening.
	   Solution:- Function ExchangeRampInfo() was commented. This function
	   is uncommented now.

BU161219:18162016:1620
	1. CMRamp was not taking average of from CM_80HZ_CTDELAY_INFO from CTS.INI
	   for first time testing.
	   Solution :- ExchangeRampInfo(TRUE) function used to exchange CM switching line
	   and no of avg reading infromation with CM Ramp. But it was not getting
	   restored after testing CMRamp. Hence this information was getting
	   overwritten. Hence ExchangeRampInfo(FALSE) function added to restore
	   original switching & no of Avg information.
	   As already BU161218 version was under test, hence next version for day
	   was named as BU161219. (As per discussion with Sudhax)

		  ExchangeRampInfo() function called two times. First time from
	   OnRunFullsequence() & then while testing CMRamp. While calling from
	   OnRunFullsequence() it will passing Avg value as 0. When second time
	   this 0 value only exchanged with proper value of Avg reading. Now call
	   for ExchangeRampInfo() from OnRunFullsequence() is commented. Now
	   ExchangeRampInfo() only called while testing CMRamp.

BU161220:18162016:1930
    1. CM 80Hz should be tested in 3rd and 4th range. CurrentRange increased
	  by 2 for 80Hz and decresase by 2 after testing is over.

BU161221:20122016:2220
    1. Length wise multiplication factor to be added common for CM Ramp and
	  CM@80Hz.
	  Changes made in CTS.INI & CMTEST.CPP, details given below.
	  CTS.INI - Number of spans for CM@80Hz are defined under header
	  [BASIC_INFO] as "NOOFSPAN_CM80HZMF". Maxmimum value for this number of
	  Spans in 20. Spanwise length for CM80Hz are defined under header
	  "[CM80HZMF_SPAN]". Mutliplication factor for these Length wise spans
	  are defined under header "[CM80HZ_LENMF]".
	  Now these multiplication factor are common for CMRamp & CM80Hz. The
	  multiplication factor defined under header "[RAMP_RANGE2_MF]" will not
	  be used further.

	  CMTEST.CPP - The function "ReadCM80HzLenMFs()" defined in CMTEST.CPP.
	  It will take input as cable length. It will read Spans & its respective
	  MF from CTS.INI and returns MF respective to Cable length given as input.
	  If given cable length in not present in SPAN then it will return MF as
	  1.0.Multiplication factor provided by above function will be applied
	  to CMRamp or CM80Hz as per selection from CTS.INI.
