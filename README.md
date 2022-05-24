# [inspexel](https://gottliebtfreitag.de/code/inspexel.html)

The swiss army knife for dynamixel servo motors.

This software's purpose is to have a command line tool to configure dynamixel motors. 
Dynamixel motors are being used in many robotic projects but it seems like some tooling around the motors is missing.
There exists the robotis tool RoboManager which only runs under Windows.
There are also other projects like [Mixcell](https://github.com/clebercoutof/mixcell) which bring some of the functionality to Linux.
One big issue with these tools is that they have a GUI which makes it hard to use them over ssh on remote computers.
Also it is not possible to use them inside of scripts.
Inspexel is a command-line only utility to fill this gap.
It also enables the generation of scripted (via bash) motions.

# Features

- Automatic motor discovery
- All baudrates (even nonstandard baudrates)
- Dynamixel Protocol V1 and V2
- Support for all currently produced dynamixel motors
- Reading registers with additional information and pretty output
- Reading/Writing individual registers or register groups
- Represent motors and their registers as a fuse mounted filesystem for easy scriptability
- Reboot motors

## supported motors

| Family | Subtypes |
|--------|----------|
| MX28 | MX-28T, MX-28R, MX-28AT, MX-28AR |
| MX64 | MX-64T, MX-64R, MX-64AT, MX-64AR |
| MX106 | MX-106T, MX-106R |
| MX12 | MX-12W |
| MX28-V2 | MX-28T-V2, MX-28R-V2, MX-28AT-V2, MX-28AR-V2 |
| MX64-V2 | MX-64T-V2, MX-64R-V2, MX-64AT-V2, MX-64AR-V2 |
| MX106-V2 | MX-106T-V2, MX-106R-V2 |
| XH430-W350 | XH430-W350-T, XH430-W350-R |
| XH430-W210 | XH430-W210-T, XH430-W210-R |
| XM430-W350 | XM430-W350-T, XM430-W350-R |
| XM430-W210 | XM430-W210-T, XM430-W210-R |
| XH430-V350 | XH430-V350 |
| XH430-V210 | XH430-V210 |
| XL430-W250 | XL430-W250 |
| XM540-W150 | XM540-W150-T, XM540-W150-R |
| XM540-W270 | XM540-W270-T, XM540-W270-R |
| M42-10-S260-R | M42-10-S260-R |
| M54-40-S250-R | M54-40-S250-R |
| M54-60-S250-R | M54-60-S250-R |
| H42-20-S300-R | H42-20-S300-R |
| H54-100-S500-R | H54-100-S500-R |
| H54-200-S500-R | H54-200-S500-R |
| XL-320 | XL-320 |
| AX-12A | AX-12A |
| AX-18A | AX-18A |
| AX-12W | AX-12W |

# Usage

Inspexel comes with several subcommands.
Each subommand represents a different aspect or way to configure a dynamixel motor or inquire its configuration.
All commands accept the arguments
- `--device [path-to-serial-device]` select the serial device
- `--baudrate [baudrate-in-baud]` select the baudrate to communicate to the motors (some commands also support multiple baudrates)
- `--protocol_verion [1/2]` use either protocol version 1 or 2

## detect all connected motors
```
$ inspexel
```
or:
```
$ inspexel detect
```

Add the flag `--read_all` flag to get the content of all registers nicely printed.

```
$ inspexel detect --read_all
```

<figure>
    {% picture default assets/images/inspexel.png --alt console output of inspexel %}
    <figcaption>console output of inspexel</figcaption>
</figure>

## Reboot a motor
Reboots motor with id 3

```
$ inspexel reboot --id 3
```

## Manual setting of register(s)
Set register 0x44 (68) of motor 3 to value 1

```
$ inspexel set_register --register 0x44 --values 1 --id 0x03
```

## Fuse integration
Inspexel can expose all registers of all connected as a fuse filesystem.

```
$ inspexel fuse
```

Inspexel will create a directory `dynamixelFS`.
Then a detect cycle is run automatically and every detected motor will be represented in a subdirectory of `dynamixelFS` (e.g., `dynamixelFS/11/` for motor with id 11).
Within that directory are two subdirectories containing files representing the registers either by name `dynamixelFS/11/by-register-name` or by address `dynamixelFS/11/by-register-id`.
A read on any of the containing files will make inspexel perform a read of the corresponding register and return the content as string.
You can use that to live monitor the value of a register:

```
$ inspexel fuse & && watch cat "dynamixelFS/11/by-register-name/Present\ Position"
```

Likewise you can set register values as if they were files:

```
$ inspexel fuse & && echo 1 > dynamixelFS/11/by-register-name/LED
```

Further you can manually trigger detection of a motor by writing the motorID to look for to `dynamixelFS/detect_motor`:

```
$ echo 11 > dynamixelFS/detect_motor
```


## Miscellaneous

### getting help:

```
$ inspexel --help
```

### Manpage:

```
$ man inspexel
```

# How to install
## Ubuntu 20.04

```
# install Fuse deps
$ sudo apt-get install libfuse-dev
# build inspexel
$ git clone https://github.com/gottliebtfreitag/inspexel.git
$ cd inspexel
$ make && sudo make install
```

## Archlinux
```
# build inspexel
$ git clone https://github.com/gottliebtfreitag/inspexel.git
$ cd inspexel
$ make && sudo make install
```
