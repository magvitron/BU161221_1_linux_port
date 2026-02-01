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

set(SRC ${CMAKE_CURRENT_LIST_DIR}/../BU161221_1)

add_executable(cts
  ${SRC}/CTS.CPP

  # core modules used by CTS
  ${SRC}/CBLSEQ.CPP
  ${SRC}/CONFIG.CPP
  ${SRC}/FILEVIEW.CPP
  ${SRC}/FRONTPG.CPP
  ${SRC}/GADGETS.CPP
  ${SRC}/KEY.CPP
  ${SRC}/LIMITS.CPP
  ${SRC}/PASSWORD.CPP
  ${SRC}/PRODINI.CPP
  ${SRC}/RDSCH.CPP
  ${SRC}/READINI.CPP
  ${SRC}/REP.CPP
  ${SRC}/REPSETUP.CPP
  ${SRC}/RSETUP.CPP
  ${SRC}/RUN.CPP
  ${SRC}/SECURITY.CPP
  ${SRC}/SHARE.CPP
  ${SRC}/SPECK.CPP
  ${SRC}/UTILS.CPP

  # tests (these were your missing symbols)
  ${SRC}/CRTEST.CPP
  ${SRC}/CMTEST.CPP
  ${SRC}/CUPGTEST.CPP
  ${SRC}/CUPPTEST.CPP
  ${SRC}/CUPITEST.CPP
  ${SRC}/ATNTEST.CPP
  ${SRC}/NEXTTEST.CPP
  ${SRC}/ZCALTEST.CPP
  ${SRC}/FEXTTEST.CPP

  # if EndReport() really lives here in your tree
  ${SRC}/SPECKS.C
)

# IMPORTANT:
# Do NOT compile these into cts (they have their own main() + duplicates):
#   DISPREP.CPP, DISPUTL.CPP, PRNREP.CPP, PRNSETUP.CPP, UTILS1.CPP, MSGBOX.CPP, STIMER.CPP

target_include_directories(cts PRIVATE
  ${SRC}
  ${tvision_SOURCE_DIR}/include
  ${tvision_SOURCE_DIR}/include/tvision
  ${tvision_SOURCE_DIR}/include/tvision/compat/borland
)

target_compile_definitions(cts PRIVATE
  Uses_TStreamable
  Uses_TStreamableClass
  Uses_TObjStrm
  Uses_TVObjects
  _NEAR=
  _FAR=
)

target_compile_options(cts PRIVATE
  -w
  -include ${CMAKE_CURRENT_LIST_DIR}/linux_borland_compat.h
)

target_link_libraries(cts PRIVATE tvision)

# Optional: avoid PIE textrel warning (old code + globals)
# target_link_options(cts PRIVATE -no-pie)
