# packager

C program for encoding sensor data into CU InSpace radio packet format.

## Usage

`packager` expects input data coming from a message queue called `/fetcher/sensors`. See [fetcher][fetcher] for more 
details on the format of these messages.

For more information about how to use packager from the command line, type `use ./packager` in your shell.

## Installation

- Requires QNX 7.1

## Notes

This software is distributed under the [GNUGPLv3 license](https://github.com/CarletonURocketry/packager/blob/main/LICENSE),
which requires all source code in this repository to be made publicly available if this software is distributed.
Additionally, this license requires prominent attribution of the original author(s). It's encouraged to read the entire
license for more information.

[fetcher]: https://github.com/CarletonURocketry/fetcher
