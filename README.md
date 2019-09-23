# DFR_CVC

## Overview
The DFR CVC project provides the code repository for the Dartmouth Formula Racing Central Vehicle Controller. The code is written in C and runs on the CVC's STM32F7 microcontroller (ARM M7 processor). 

## Development
The current CVC development toolchain has only been validated on Windows OS. It is theoretically possible to use the same toolchain on a Mac or Linux OS.

### To set up your development environment:
1. Install the STM32 System Workbench software: https://www.openstm32.org/Installing%2BSystem%2BWorkbench%2Bfor%2BSTM32%2Bwith%2Binstaller
2. Clone this repository and save it to your local machine. Github provides instructions for how to do this.
3. Open the project as a C/C++ project from the System Workbench and build. 

### To flash the board
The CVC board is flashed using the ST-Link external debugger/programming cable. The Nucleo development boards have on-board debuggers and can be flashed using a micro-USB programming cable. 

## Architecture

