REM to link to this library include the following header and cl commands
REM #include "cpprequest.h"
REM cl -Zi -EHsc -I "path to cpprequest.h" -c *.cpp && link *.obj cpprequest.lib /LIBPATH:"path to cpprequest.lib"

@echo off
pushd build

cl^
  -Zi^
  -EHsc^
  -I "."^
  -c^
  *.cpp &&^
lib^
  *.obj

popd