rm AttendanceJournal.zip
cd build
make clean
make WIN32=True
yes | cp -f journal.exe Release/
cd Release
rm *.data
rm backup/* -rf
echo "Dummy" > backup/Dummy.txt
zip ../../AttendanceJournal.zip -r * 