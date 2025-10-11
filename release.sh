get_dep_github()
{
 # https://gist.github.com/steinwaywhw/a4cd19cda655b8249d908261a62687f8
 if [ "$#" -lt 3 ]; then
  echo "Usage: $0 owner repo postfix_for_grep [major_tag]"
  exit 1
 fi

 TAG="latest"

 if [ "$#" -eq 4 ]; then
  TAG=tags/$(curl -s https://api.github.com/repos/libsdl-org/sdl/tags | grep "name\": \"release-2" | head -n 1 | cut -d'"' -f 4)
 fi

 curl -s https://api.github.com/repos/$1/$2/releases/$TAG \
 | grep "browser_download_url.*$3" \
 | cut -d : -f 2,3 \
 | tr -d \" \
 | wget -qi -

 echo "$0: Downloaded $1/$2 with tag $TAG"
}

get_dep_msys()
{
 NAME=$(wget -O - http://repo.msys2.org/mingw/mingw32 | grep -e "$1.*.zst\"" | cut -d'"' -f 2 | tail -n 1)
 wget http://repo.msys2.org/mingw/mingw32/$NAME
 tar --zstd -xvf $1*
}

set -xeo pipefail
rm AttendanceJournal.zip -f
cd build

# Update MinGW32 packages before build:
get_dep_msys mingw-w64-i686-SDL2-
get_dep_msys mingw-w64-i686-glfw-3
get_dep_msys mingw-w64-i686-boost-libs
WIN32LIB_FOLDER="mingw32"

make clean
make WIN32=True
yes | cp -f journal.exe Release/
cd Release
rm *.data -f
rm backup/* -rf

# Gather all WIN32 dependencies:
rm *.dll -f
WiN32LIB_FOLDER="/usr/i686-w64-mingw32/bin/"
cp $WiN32LIB_FOLDER/libgcc_s_dw2-1.dll  .
cp $WiN32LIB_FOLDER/libstdc++-6.dll     .
cp $WiN32LIB_FOLDER/libwinpthread-1.dll .

echo "Dummy" > backup/Dummy.txt
zip ../../AttendanceJournal.zip -r * 
