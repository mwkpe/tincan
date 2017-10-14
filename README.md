# tincan
Visualization of CAN bus traffic via UDP interface

<img src="https://github.com/jwkpeter/tincan/blob/master/tincan.png" alt="tincan banner" width="300">

Description
---
Tincan displays CAN frames received from an UDP socket and therefore requires a gateway device, e.g. a Raspberry Pi 3 with a PiCAN2 HAT running [cangw](https://github.com/jwkpeter/cantools).

Status
---
WIP

Features
---
* Live view of received frames
* Import of frame and signal definitions from DBC files

Acknowledgements
---
Tincan is using a rubenvb [mingw-w64](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds) build, [Qt](http://https://www.qt.io), [Spirit X3](http://boost-spirit.com), [Asio](https://think-async.com/), [GSL](https://github.com/Microsoft/GSL), [fmt](https://github.com/fmtlib/fmt), [JSON](https://github.com/nlohmann/json) and [doctest](https://github.com/onqtam/doctest).
