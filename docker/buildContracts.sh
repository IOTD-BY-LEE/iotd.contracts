#!/bin/bash
set -e # exit on failure of any "simple" command (excludes &&, ||, or | chains)
cd /vktio.contracts
./build.sh -c /usr/opt/eosio.cdt -e /opt/eosio -y
cd build
tar -pczf /artifacts/contracts.tar.gz *
