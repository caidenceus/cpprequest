@echo off
pushd bin

cl^
  -Zi^
  -EHsc^
  -I "../src/"^
  -c^
  ../src/*.cpp &&^
lib^
  *.obj

move *.lib ../

popd