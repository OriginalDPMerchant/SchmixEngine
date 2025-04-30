cmake -B Intermediate/Debug -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build  Intermediate/Debug --config Debug
cmake --install Intermediate/Debug --prefix . --config Debug
pause