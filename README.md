# Tincan
<p align="center"><img src="https://github.com/mwkpe/tincan/blob/master/tincan.png" alt="tincan banner" width="300"></p>
<p align="center">Visualization of CAN bus traffic via UDP interface</p>

Description
---
Tincan decodes and displays CAN frames. The default interface is via UDP socket and requires a gateway device, e.g. a Raspberry Pi 3 with a PiCAN2 HAT running [cangw](https://github.com/mwkpe/cantools), or any CAN to UDP gateway that sends raw frames. Adding other interfaces can be done by adapting the [dummy receiver](src/network/dummyreceiver.h).

Status
---
WIP

Features
---
* Import of frame, signal and value definitions from DBC files
* Live view of received frames and signals in a [tree view](tincan_treeview.png).
* Simple trace view of frame and signal data

Build
---
Building requires the Qt framework (Qt 5.12+ MinGW 7.3). The project file must be modified where absolute paths to boost and Asio were used. The other libs are part of the repo.

Acknowledgements
---
Tincan is using [Qt](http://https://www.qt.io), [boost](http://www.boost.org/), [Spirit X3](http://boost-spirit.com), [Asio](https://think-async.com/), [GSL](https://github.com/Microsoft/GSL), [fmt](https://github.com/fmtlib/fmt), [JSON](https://github.com/nlohmann/json) and [doctest](https://github.com/onqtam/doctest).
