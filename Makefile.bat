@echo off
pushd bin

cl^
  -Zi^
  -EHsc^
  -W4^
  -I "../src/"^
  -I "../include/"^
  -c^
  ../src/*.cpp &&^
lib^
  *.obj

move *.lib ../

popd