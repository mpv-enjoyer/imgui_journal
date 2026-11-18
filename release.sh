set -uxeo pipefail

rm AttendanceJournal.zip -f
cd build
make clean
make WIN32=True -j$(nproc)

cd Release
yes | cp -f ../journal.exe .
yes | cp -rf ../images .
mkdir -p backup
touch backup/Dummy.txt
echo "Dummy" >> backup/Dummy.txt
rm *.data

zip ../../AttendanceJournal.zip -r * 
echo "Now copy the zip file to the shared folder"
