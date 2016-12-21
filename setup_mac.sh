#!/bin/bash

# DB
wget --no-check-certificate "https://download.oracle.com/berkeley-db/db-6.1.29.NC.tar.gz"
shasum -a 256 -c db_checksum
mv db-6.1.29.NC.tar.gz ./deps/
cd ./deps/
tar -xzf db-6.1.29.NC.tar.gz

