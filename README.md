# Tincan
<p align="center"><img src="https://github.com/jwkpeter/tincan/blob/master/tincan.png" alt="tincan banner" width="300"></p>
<p align="center">Visualization of CAN bus traffic via UDP interface</p>

Description
---
Tincan displays CAN frames received from an UDP socket and therefore requires a gateway device, e.g. a Raspberry Pi 3 with a PiCAN2 HAT running [cangw](https://github.com/jwkpeter/cantools).

Status
---
WIP

Features
---
* Live view of received frames
* Trace view of frame data and signal values
* Import of frame and signal definitions from DBC files

Build
---
Building Tincan requires a C++17 compliant GCC version and a recent Qt framework build from source using said compiler. The project file must me modified where absolute paths to boost and Asio were used. The other libs are part of the repo.

Acknowledgements
---
Tincan is using a rubenvb [mingw-w64](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds) build, [Qt](http://https://www.qt.io), [boost](http://www.boost.org/), [Spirit X3](http://boost-spirit.com), [Asio](https://think-async.com/), [GSL](https://github.com/Microsoft/GSL), [fmt](https://github.com/fmtlib/fmt), [JSON](https://github.com/nlohmann/json) and [doctest](https://github.com/onqtam/doctest).
