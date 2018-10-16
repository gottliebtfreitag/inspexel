# inspexel
The swiss army knife for dynamixel servo motors.

# Features
- Automatic Motor discovery
- All Baudrates
- Dynamixel Protocol V1 and V2
- AX-Series, MX-Series, Pro-Series, X-Series (excluded all discontinued motors)
- Reading Register with additional information
- Setting/Getting individual registers
- Integrating motors into filesystem for command line based access
- reboot single motors

# Usage
Detect all connected motors
```
$ inspexel
```
Detect all connected motors and gives verbose information about them
```
$ inspexel detect --read_all
```
![consol output of inspexel](https://github.com/gottliebtfreitag/miscellaneous/blob/master/inspexel/inspexel.png)


List all supported motor types
```
$ inspexel meta
```
Reboots motor with id 3
```
$ inspexel reboot --id 3
```
Set register 0x44 of motor 3 to value 1
```
$ inspexel set_register --register 0x44 --values 1 --id 0x03
```
Checking the manpage
```
man inspexel
```

# How to install
## Ubuntu 16.04
```
# install gcc-8
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update
sudo apt-get install g++-8 gcc-8
# build inspexel
git clone https://github.com/gottliebtfreitag/inspexel.git
cd inspexel
make && sudo make install
```
## Archlinux
```
# build inspexel
git clone https://github.com/gottliebtfreitag/inspexel.git
cd inspexel
make && sudo make install
```
