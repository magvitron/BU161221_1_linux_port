#define cmFrontPage			180

#define hcRunAllTnR			0x8001
#define hcTesting			0x8002
#define hcTested			0x8003
#define hcStartTesting		0x8004
#define hcStopTesting		0x8005
// because this can used by many files for disabling
#define cmResume				154

// HELP Ir Test
#define hcAutoRange		0x9000
#define hcAutoCal		0x9001
#define hcFindOffset	0x9002
#define hcIrSelfCheck	0x9003
#define hcRunInd		0x9004
#define hcRunCnS		0x9005
#define hcTest          0x9006

#define hcCalibrate		0xA001
#define hcDataDirectory 0xA002

const
  hcAsciiTable           = 6,
  hcCalculator           = 4,
  hcCalendar             = 5,
  hcCancelBtn            = 35,
  hcF7                   = 43,
  hcFCChDirDBox          = 37,
  hcFChangeDir           = 15,
  hcFDosShell            = 16,
  hcFExit                = 17,
  hcFOFileOpenDBox       = 31,
  hcFOFiles              = 33,
  hcFOName               = 32,
  hcFOOpenBtn            = 34,
  hcFOpen                = 14,
  hcFile                 = 13,
  hcHHvirCaution         = 42,
  hcHMnuNHotKeys         = 41,
  hcHUseHelp             = 40,
  hcNocontext            = 0,
  hcOCColorsDBox         = 39,
  hcOColors              = 28,
  hcOMMouseDBox          = 38,
  hcOMouse               = 27,
  hcORestoreDesktop      = 30,
  hcOSaveDesktop         = 29,
  hcOpenBtn              = 36,
  hcOptions              = 26,
  hcPuzzle               = 3,
  hcSAbout               = 8,
  hcSAsciiTable          = 11,
  hcSCalculator          = 12,
  hcSCalendar            = 10,
  hcSPuzzle              = 9,
  hcSystem               = 7,
  hcViewer               = 2,
  hcWCascade             = 22,
  hcWClose               = 25,
  hcWNext                = 23,
  hcWPrevious            = 24,
  hcWSizeMove            = 19,
  hcWTile                = 21,
  hcWZoom                = 20,
  hcWindows              = 18,
  hcc                    = 44;
