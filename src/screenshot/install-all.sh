#!/bin/sh
check(){
	if [[ $3 != "-disable-error-check" ]]; then
		if [[ $? != 0 ]]; then
			echo -e "\e[31mFAILED\e[0m: $0 of $1 exit with status $? not 0"
			exit 1
		fi
	fi
}

echo "Building zlib"
cd ./zlib
sudo ./configure --prefix=/usr
check "Configure" "zlib" $1
sudo make
check "Make" "zlib" $1
sudo make install
check "Installing package to system" "zlib" $1
echo "Building libpng..."
cd ..

cd libpng
sudo ./configure --prefix=/usr
check "Configure" "libpng" $1
sudo make
check "Make" "libpng" $1
sudo make install
check "Installing package to system" "libpng" $1
cd ..
cd fbgrab
sudo make
check "Make" "fgrab" $1
sudo make install
check "Installing package to system" "fbgrab" $1

