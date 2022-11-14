# stm32-roboclaw

This repository contains a port of the official [roboclaw_arduino_library](https://github.com/basicmicro/roboclaw_arduino_library) for STM32 platforms. 

## Files in this Repository:

__/example__  
This folder contains an example project targeting the Nucleo STM32F767ZI dev board.

__/roboclaw__   
This folder contains the roboclaw library.

## Usage:
1) Import the library found in /roboclaw.
2) Add the files to the G++ include path.

See /example/roboclaw-dev/Core/Src/main.cpp for an example. Usage should be identical to that of the official library.