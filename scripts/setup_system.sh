#!/bin/bash

ZYNTHIAN_DIR="/home/pi/zynthian"

#------------------------------------------------
# System Adjustments
#------------------------------------------------

mkdir $ZYNTHIAN_DIR/bak
sudo chmod a+rw /dev/ttyAMA0
sudo cp $ZYNTHIAN_DIR/etc/modules /etc

# SystemV Init
sudo cp $ZYNTHIAN_DIR/etc/init.d/* /etc/init.d
sudo cp $ZYNTHIAN_DIR/etc/inittab /etc
#sudo cp $ZYNTHIAN_DIR/etc/rc.local /etc
ln -s /etc/init.d/asplashscreen /etc/rcS.d/S01asplashscreen
ln -s /etc/init.d/zynthian /etc/rc2.d/S01zynthian
ln -s /etc/init.d/zynthian /etc/rc3.d/S01zynthian
ln -s /etc/init.d/zynthian /etc/rc4.d/S01zynthian
ln -s /etc/init.d/zynthian /etc/rc5.d/S01zynthian
sudo update-rc.d asplashscreen enable
sudo update-rc.d zynthian enable
sudo update-rc.d -f avahi-daemon remove
sudo update-rc.d -f triggerhappy remove
sudo update-rc.d -f cron remove
sudo update-rc.d -f rsyslog remove
sudo update-rc.d -f dbus remove

# DISABLE wpa_supplicant ??? => No hace nada
sudo mv /usr/share/dbus-1/system-services/fi.epitest.hostap.WPASupplicant.service $ZYNTHIAN_DIR/bak
sudo mv /usr/share/dbus-1/system-services/fi.w1.wpa_supplicant1.service $ZYNTHIAN_DIR/bak

# Systemd
# ...

# X11 Config
sudo mv /usr/share/X11/xorg.conf.d /usr/share/X11/xorg.conf.d.nouse
sudo mkdir /etc/X11/xorg.conf.d
sudo cp $ZYNTHIAN_DIR/etc/X11/xorg.conf.d/99-calibration.conf /etc/X11/xorg.conf.d
sudo cp $ZYNTHIAN_DIR/etc/X11/xorg.conf.d/99-pitft.conf /etc/X11/xorg.conf.d

# User
cp $ZYNTHIAN_DIR/etc/zynaddsubfxXML.cfg ~/.zynaddsubfxXML.cfg
sudo cp $ZYNTHIAN_DIR/etc/zynaddsubfxXML.cfg /root/.zynaddsubfxXML.cfg

#------------------------------------------------
# Install Required Debian Packages
#------------------------------------------------

# Tools
sudo apt-get install tree
sudo apt-get install joe
sudo apt-get install fbi
sudo apt-get install scrot # Screenshot Capture

# Libraries
#sudo apt-get install python-dev
sudo apt-get install python3-dev
sudo apt-get install libasound2-dev

# Fluidsynth & SF2
sudo apt-get install fluidsynth fluid-soundfont-gm fluid-soundfont-gs
cd $ZYNTHIAN_DIR/sf2
ln -s /usr/share/sounds/sf2/* .

#------------------------------------------------
# Compile / Install Other Required Software
#------------------------------------------------

cd $ZYNTHIAN_DIR/software/alsaseq-0.4.1
#sudo python setup.py install
sudo python3 setup.py install

cd $ZYNTHIAN_DIR/software
git clone git://git.drogon.net/wiringPi
git pull origin
./build

# ZynAddSubFX (execution only)
sudo apt-get install libfltk1.3 libfltk-images1.3 liblo7 libmxml1


#------------------------------------------------
# Install PiTFT Software
#------------------------------------------------

curl -SLs https://apt.adafruit.com/add | sudo bash
sudo apt-get install -y raspberrypi-bootloader

#------------------------------------------------
# Módulos Python3
#------------------------------------------------
sudo apt-get install python3-pip
sudo pip-3.2 install python-osc


