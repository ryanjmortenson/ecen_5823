#/usr/bin/env bash

# @file cscope.sh 
#
# @brief Builds cscope database 
# @author Ryan Mortenson
# @tools GCC 5.4.0, vim 7.4, make 4.1, Ubuntu 16.04
#

find . -name "*.[chmkpy]" -exec echo \"{}\" \; > files.txt
cscope -i files.txt -bqU
rm -rf files.txt
