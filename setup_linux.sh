#!/bin/bash

# DB
wget --no-check-certificate "https://download.oracle.com/berkeley-db/db-6.1.29.NC.tar.gz"
cat db_checksum | sha256sum -c
mv db-6.1.29.NC.tar.gz ./deps/
cd ./deps/
tar -xzf db-6.1.29.NC.tar.gz

