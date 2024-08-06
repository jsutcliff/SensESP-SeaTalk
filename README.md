# SeaTalk 1 Data Input for SensESP

This repository implements a SeaTalk 1 parser library for [SensESP](https://signalk.org/SignalK/SensESP/).
It is loosely based on the [NMEA183](https://github.com/SensESP/NMEA0183) library, the [APRemote](https://github.com/richardJG/APRemote) project, and the information provided by [Thomas Knauf](http://www.thomasknauf.de/rap/seatalk2.htm).

To use the library in your own projects, you have to include it in your `platformio.ini` `lib_deps` section:

    lib_deps =
        SignalK/SensESP@^2.1.1
        jsutcliff/SensESP-SeaTalk@^0.0.1

See also the [example main file](blob/main/examples/seatalk_receiver.cpp).

For more information on using SensESP and external add-on libraries, see the [SensESP documentation](https://signalk.org/SensESP/docs/).
