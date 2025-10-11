set -xe
rm AttendanceJournal.zip -f
cd build
make clean
make WIN32=True
yes | cp -f journal.exe Release/
cd Release
rm *.data -f
rm backup/* -rf
rm libgcc_s_dw2-1.dll libstdc++-6.dll libwinpthread-1.dll -f
WiN32LIB_FOLDER="/usr/i686-w64-mingw32/bin/"
cp $WiN32LIB_FOLDER/libgcc_s_dw2-1.dll  .
cp $WiN32LIB_FOLDER/libstdc++-6.dll     .
cp $WiN32LIB_FOLDER/libwinpthread-1.dll .
echo "Dummy" > backup/Dummy.txt
zip ../../AttendanceJournal.zip -r * 