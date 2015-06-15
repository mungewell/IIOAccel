# IIOAccel - a demo app using the LSM303DLHC accelerometer on a Raspberry Pi via IIO

IIO (Industrial I/O) is a standardized interface between sensor kernel drivers and userland. This demo shows how to use the ST LSM303DLHC accelerometer with the IIO driver.

### Setup

Raspbian by default does not have IIO enabled. See the post at http://wp.me/p4qcHg-j7 for details of how to build a new kernel with the appropriate settings.

In addition, it may be useful to set the I2C speed to 400kHz or else the sample rate will be limited to 400 samples per second or less. See the post at http://wp.me/p4qcHg-jh for details of how to do this.

The LSM303DLHC needs to be connected to GND, +3.3v, SDA and SCL as usual. In addition it is necessary to connect the Int1 output to GPIO 25 on the Raspberry Pi. The app will not work without this line connected. The address is assumed to be 0x19 (as on the Adafruit 10-dof IMU for example).

### Build

The app is built natively on the Pi and requires a few packages:

    sudo apt-get install build-essential git libqt4-dev device-tree-compiler
    
Clone and build this repo:

    git clone git://github.com/richards-tech/IIOAccel
    cd IIOAccel
    qmake
    make -j4
    sudo make install
    
The device tree overlay for the LSM303DLHC accelerometer needs to be installed:

    cd dts
    sudo dtc -I dts -O dtb -@ -o /boot/overlays/lsm303dlhcaccel-overlay.dtb lsm303dlhcaccel.dts
    
Then edit /boot/config.txt and add the line:

    dtoverlay=lsm303dlhcaccel
    
The Raspberry Pi must then be rebooted. Run the following command:

    sudo vcdbg log msg
    
to see if any errors are being reported. If not, just run:

    IIOAccel
    
By default, it will run at 25 samples per second and display the x, y and z readings along with the delta time between samples (in uS). To modify the sample rate, edit the file MainClass.cpp. There is a call that looks like:

    m_accel->setSampleRate("2");
    
 The string "2" is for 25Hz. Other values are:
 
    0 = 1Hz
    1 = 10Hz
    2 = 25Hz
    3 = 50Hz
    4 = 100Hz
    5 = 200Hz
    6 = 400Hz
    7 = 1600Hz
    
Note that, for 1600 samples per second, the I2C bus must run at 400kHz or else the chip will lock up and require a power cycle.

The display code only displays the most recent sample and may miss multiple samples if rates are too high. The base code is able to operate at 1600Hz however.
    
 
