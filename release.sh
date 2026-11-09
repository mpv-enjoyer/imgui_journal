rm AttendanceJournal.zip -f
cd build
make clean
make WIN32=True
yes | cp -f journal.exe Release/
cd Release
rm *.data
zip ../../AttendanceJournal.zip -r * 
echo "Now copy the zip file to the shared folder"