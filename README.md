# ECEN-5813-Project6

Written: Morgan Tilong, Xinyuan Zhoa

# A description of the repo contents

This repo contains every file used to generate this programs. That includes the following:
1. CMSIS: source, header, interfaces to processor, peripherals RTOS, middle ware files. 
2. board: KL25Z board. 
3. drivers: clock, gpio, communication protocols, touch sensor, timer, flash. 
4. freertos: Task, Semphr, Queue, FreeRTOS c and h files. 
5. source: main().
6. startup: Source and header file for FRDM board boot up
7. utilities: source and header for debuggin. g
8. Debug: debug target files. 
9. doc: text files referencing progra. 

# Observations: 

The lookup table initially provided eroneus values. Alot of reseasrch had to be done to correct and scal ehte values properly. There was also the issue of implementinthe RTOS commands throughout the program properly. There reference we used to learn more about FreeRTOS was https://www.freertos.org/a00020.html#taskYIELD.



# Installation/Execution notes: 

This code should be run from main using make -r -j3 "target". There are two modes to run this code in:
Debug Mode:  Comments on program steps are provided along with any values needed to verify proper operations
Normal: This will output less logging and certain mathemtical outputs. 
When using the MCUexpresso IDE, always "clean" the program, "build" then "debug".
