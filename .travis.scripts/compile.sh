#!/bin/bash
trap abort_on_failure ERR

abort_on_failure () {
	echo "Last command failed, exising"
	exit 42;
}

build_vpack() {
	mkdir tmp-vpack
	pushd tmp-vpack
		git clone https://github.com/arangodb/velocypack.git velocypack
		cd velocypack
		mkdir -p build
		cd build
		cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-fPIC -std=c++11"
		sudo make install
	popd
}
build_vpack

build_fuerte() {
	mkdir tmp-fuerte
	pushd tmp-fuerte
        git clone https://github.com/arangodb/fuerte.git fuerte
		cd fuerte
		cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-fPIC -std=c++11"
		sudo make install
	popd
}
build_fuerte

phpize
./configure
make all -j4
sudo make install

echo "Use the most-up-to-date run-tests.. old ones like 5.3 don't report failure exit codes"
wget -O run-tests.php https://raw.githubusercontent.com/php/php-src/master/run-tests.php


rm -rf tmp-vpack tmp tmp-fuerte # coveralls may pick it up and lie about our coverage

echo "extension=arangodb.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`
