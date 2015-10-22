#!/bin/bash

xauth_home=$HOME
sudo su
xauth merge $xauth_home/.Xauthority
echo $1 $2 $3 $4 $5 $6 $7 $8 $9