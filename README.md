# noolite-rx
Noolite RX2164 receiver C++ binding

Main purpose of this project is to add support of noolite RX2164 to linux(MacOS, Windows etc. should also work)

About NooLite: http://www.noo.com.by/

Author: Igor K.

E-mail: ikuruch@gmail.com

Instalation instructions for Ubuntu:

1. Install libusb:
  sudo apt-get install libusb-1.0-0 libusb-1.0-0-dev
2. Download this sources:
  wget https://github.com/nonametr/noolite-rx/archive/noolite-rx-dev.zip
3. Unpack:
  tar xvfz noolite-rx-dev.zip
4. Build:
  make clean && make &&
  sudo cp noolite-rx /usr/bin &&
  sudo chown root /usr/bin/noolite-rx
5. noolite-rx -h
  run help.

