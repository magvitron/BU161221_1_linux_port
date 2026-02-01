cmake_minimum_required(VERSION 3.16)
project(BU161221 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  tvision
  GIT_REPOSITORY https://github.com/magiblot/tvision.git
  GIT_TAG master
)
FetchContent_MakeAvailable(tvision)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/../BU161221_1)

# ---- Common settings for all targets ----
function(setup_bu_target tgt)
  target_compile_definitions(${tgt} PRIVATE
    Uses_TStreamable
    Uses_TStreamableClass
    Uses_TObjStrm
    Uses_TVObjects
    _NEAR=
    _FAR=
  )

  target_compile_options(${tgt} PRIVATE -w
    -include ${CMAKE_CURRENT_LIST_DIR}/linux_borland_compat.h
  )

  target_include_directories(${tgt} PRIVATE
    ${SRC_DIR}
    ${tvision_SOURCE_DIR}/include
    ${tvision_SOURCE_DIR}/include/tvision
    ${tvision_SOURCE_DIR}/include/tvision/compat/borland
  )

  target_link_libraries(${tgt} PRIVATE tvision)
endfunction()

# ---- CTS main program (ONLY ONE main: CTS.CPP) ----
add_executable(cts
  ${SRC_DIR}/CRTEST.CPP
  ${SRC_DIR}/CMTEST.CPP
  ${SRC_DIR}/CUPGTEST.CPP
  ${SRC_DIR}/CUPPTEST.CPP
  ${SRC_DIR}/CUPITEST.CPP
  ${SRC_DIR}/ATNTEST.CPP
  ${SRC_DIR}/NEXTTEST.CPP
  ${SRC_DIR}/ZCALTEST.CPP
  ${SRC_DIR}/FEXTTEST.CPP
  ${SRC_DIR}/ARGV.CPP
  ${SRC_DIR}/CTS.CPP
  ${SRC_DIR}/CBLSEQ.CPP
  ${SRC_DIR}/CONFIG.CPP
  #${SRC_DIR}/DISPUTL.CPP
  ${SRC_DIR}/FILEVIEW.CPP
  ${SRC_DIR}/FRONTPG.CPP
  ${SRC_DIR}/GADGETS.CPP
  ${SRC_DIR}/KEY.CPP
  ${SRC_DIR}/LIMITS.CPP
  # ${SRC_DIR}/MSGBOX.CPP        # <-- REMOVE (conflicts with tvision)
  ${SRC_DIR}/PASSWORD.CPP
  # ${SRC_DIR}/PRNSETUP.CPP      # <-- REMOVE (duplicates with REP.CPP)
  ${SRC_DIR}/PRODINI.CPP
  ${SRC_DIR}/RDSCH.CPP
  ${SRC_DIR}/READINI.CPP
  ${SRC_DIR}/REP.CPP
  ${SRC_DIR}/REPSETUP.CPP
  ${SRC_DIR}/RSETUP.CPP
  ${SRC_DIR}/RUN.CPP
  ${SRC_DIR}/SECURITY.CPP
  ${SRC_DIR}/SHARE.CPP
  ${SRC_DIR}/SPECK.CPP
  ${SRC_DIR}/UTILS.CPP
  ${SRC_DIR}/UTILS_EXTRA.CPP
#  ${SRC_DIR}/UTILS1.CPP        # <-- REMOVE (duplicates with UTILS.CPP)
  # ${SRC_DIR}/STIMER.CPP        # <-- REMOVE (duplicates with UTILS.CPP timer funcs)
)
setup_bu_target(cts)


# ---- DISPREP tool (main in DISPREP.CPP) ----
add_executable(disprep
  ${SRC_DIR}/DISPREP.CPP
)
setup_bu_target(disprep)

# ---- PRNREP tool (main in PRNREP.CPP) ----
add_executable(prnrep
  ${SRC_DIR}/PRNREP.CPP
)
setup_bu_target(prnrep)
