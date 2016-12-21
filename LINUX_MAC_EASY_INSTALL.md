V(anilla)cash
===========

### Build status - MacOS, clang
[![Build Status](https://travis-ci.org/spiralus/vcash.png)](https://travis-ci.org/spiralus/vcash)

A decentralized currency for the internet.

**Modified branch for easier building on MacOS or Linux using CMake instead of bjam, etc **

For Mac or Linux, make sure dependencies are installed

Linux Ubuntu Xenial 
*  sudo apt-get install -qq cmake gcc g++ libssl-dev libboost-all-dev 
	
For Mac use homebrew to install boost and openssl. 

* brew install boost openssl 

Take note of the version of openssl downloaded make sure that the version matches in the CMakeLists.txt file in this directory if you have issues

For Mac, run the script `setup_mac.sh` or type the following lines from this directory

* wget --no-check-certificate "https://download.oracle.com/berkeley-db/db-6.1.29.NC.tar.gz"
* shasum -a 256 -c db_checksum
* mv db-6.1.29.NC.tar.gz ./deps/
* cd ./deps/
* tar -xzf db-6.1.29.NC.tar.gz

For Linux, run the script `setup_linux.sh`

Now create a build directory and run cmake as usually done

* mkdir build
* cd build
* cmake ..
* make
* make install

vcashd will be installed in /usr/local/bin

For faster parallel builds replace last step with

* make -j4

For Xcode project, replace `cmake ..` above with `cmake .. -GXcode`

This is an example rpc command you can use to communicate with vcashd

* curl -s --data-binary '{"jsonrpc": "2.0", "method": "getinfo", "params":"", "id":1}' -H 'content-type: application/json;' http://127.0.0.1:9195/




```
