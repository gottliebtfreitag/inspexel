#!/bin/bash

echo "
.\" Manpage for inspexel
.\" Contact info@gottliebtfreitag.de
"

./inspexel --groff | head -n 1

echo "
.SH NAME
inspexel - inspecting motors using the dynamixel protocol

.SH SYNOPSIS
inspexel <COMMAND> [OPTION]... 

.SH DESCRIPTION
inspexel is a program to inspect connected motors that uses the dynamixel protocol. It supports protocol v1 and v2.
"
./inspexel --groff | tail -n +2

echo ".SH EXAMPLE
.SH
	$ inspexel
.TP
	detects all connected motors

.SH
	$ inspexel detect --read_all
.TP
	detects all connected motors and gives verbose information about them

.SH
	$ inspexel meta
.TP
	list all supported motor types

.SH
	$ inspexel reboot --id 3
.TP
	reboots motor with id 3

.SH
	$ inspexel set_register --register 0x44 --values 1 --id 0x03
.TP
	set register 0x44 of motor 3 to value 1

"

echo "
.SH BUGS
No known bugs.

.SH AUTHOR
Developed by Lutz Freitag <lutz@gottliebtfreitag.de> and Simon Gene Gottlieb <simon@gottliebtfreitag.de>
"

echo ".SH LICENSE"
cat LICENSE
