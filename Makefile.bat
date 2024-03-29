@echo off
pushd bin

cl -Zi -EHsc -W4 -std:c++17^
  -I "../src/" -I "../include/"^
  -c ../src/*.cpp &&^
lib -Out:cpprequest.lib *.obj

move *.lib ../

popd