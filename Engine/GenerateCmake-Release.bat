cmake -B Intermediate/Release -S . -DCMAKE_BUILD_TYPE=Release
cmake --build  Intermediate/Release
cmake --install Intermediate/Release --prefix .
pause