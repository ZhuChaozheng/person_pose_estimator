# person_pose_estimator_MCU_code

the MCU part collects the data from LISD3SH accelerator and do the attitude angular estimation from gravity. It also uses [Allan method](https://github.com/rpng/kalibr_allan) to calibrate the constant error and random error of the accelerator to improve the accuracy and stability of angular estimation. Finally,with Zigbee chip(LRF215) as the interface for the Pi part,sending the angular data to Pi with Zigbee and Uart.   

## Get Started

Before soldering the Zigbee module,link the module to PC and config the module to low-power-end and set the baudrate to 115200,then solder it to the board.

After soldering, you can download the Hex file to the MCU(STM32L053) with STLink, aware that the MCU need a voltage higer than 3V to have the flash programmed. So you should try to connect the power pin of the MCU to 3.3V from the STLink. If you use the default voltage provided by the LDO,Programming probably failed in the erase period.

After the download,test the data with Zigbee coordinate connected to the PC by Uart. Then turn the baudrate to 115200(The Zigbee coordinate also need to be baudrate set to 115200 with Config tool).Close the config tool to release the Uart. read the data stream with a Uart-assistant software([Serial Port Utility](https://serialport.en.softonic.com) is recommended).There should be data starts with hex code 'AA'.  It means the configuration is right, you can try to test the Pi code part.

## Principles of the code

We use the calibrated accel value and assume the person is at a constant velocity. In this way, we can easily calculate the gravity vector direction from the measured accel vector by the trigonometric functions. Obviously the gravity vector can be easily measured when the person is at a constant velocity. Then we can calculate the direction of  the gravity. We depose the gravity to each plane to calculate  the angle deflection on each plane. Finally the rotation angle of object itself is reverse to the gravity vector rotation from the initial status. There is equation as below:

![](https://github.com/LiFengcheng01/fengcheng.github.io/raw/master/clip_image002.gif)

​																				![clip_image004](https://github.com/LiFengcheng01/fengcheng.github.io/raw/master/clip_image004.gif)

The low power rules is to close as much devices on board as possible. We latched Zigbee module to idle module with a absolutely external resistor. Only when we need a transmission, we pull it up with MCU and send the data. The internal RTC of MCU is also used to awake MCU itself after MCU sleep positively to cut the MCU power consumption.  



## Power consumption of the system

as the figure below,the average power consumption(The blue mark) is 3.43mW and the peak power consumption(The red mark) is 37.57mW(mainly cost by Zigbee module transmission).

![power_result_raw](https://github.com/LiFengcheng01/fengcheng.github.io/raw/master/power_result_raw.png)
