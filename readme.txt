External Library Setup
* FreeType (text rendering)
   - you shouldn't have to do anything to get this working (CMake should setup everything)

* IrrKlang (audio)
   MAC
      - Copy ext/irrKlang-64bit-1.6.0/bin/macosx-gcc/libirrklang.dylib to /usr/local/lib
      - Copy the 3 .dylib files in ext/irrKlang-64bit-1.6.0/bin/macosx-gcc/ to the build directory
   Windows
      - Copy the 3 .dll files in ext/irrKlang-64bit-1.6.0/bin/winx64-visualStudio/ to the build directory
      - Copy ext/irrKlang-64bit-1.6.0/lib/Winx64-visualStudio/irrKlang.lib to the build directory
