cd build
make clean
make WIN32=True
yes | cp -f journal.exe Release/
cd Release
zip ../../AttendanceJournal.zip -r * 