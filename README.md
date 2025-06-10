# IGNITOR-BOARD
Embedded C code for GTXR's ignitor board and backup ignitor board. This code is designed to be run on the STM32L431CBT6 microcontroller (MCU). The repository utilizes Lazo Attar's configuration manager toolchain.

## Hardware Description 
There are two configurations of the ingitor board: the primary ignitor board, and the backup ignitor board. Both utilize the same MCU and have the same continuity sensing and firing topologies. There are some minor differences, including MCU pinouts and the primary ignitor board utilizing an enable line for its firing circuitry. 

The pinout differences are managed through the use of Lazo's toolchain. We maintain two .ioc configration files: `/data/configs/main` main for the primary ignitor board; and `data/configs/backup` for the backup ignitor board. Running Lazo's tools will automatically configure the makefile to build for the proper configuration. IO pin functions shared between the primary and backup ignitor boards should have consistent user labels in the .ioc files to make the code applicable to both boards (i.e.: calling a pin `FIRE_1` in the .ioc file of both boards means we can just use a `FIRE_1` pin name in the software without having to change the underlying pin definitions depending on which board we are flashing). 

Any differences in code between the two boards that do arise should be resolved with preprocessor directives (i.e.: `#ifdef __BACKUP__` to enable specific sections of code for the backup board) that are defined/not defined prior to compiling and flashing the software.

## First-Time Setup
To run this toolchain, you will need Docker Desktop **[(download here)](https://docs.docker.com/get-started/get-docker/)**
You will also need the VSCode Extension "Dev Containers".

Once you have installed both, run Docker Desktop and do the following:
- Open the repository in VSCode
- Open the command window (Ctrl + P)
- Run the command "Dev Containers: Reopen in Container"
- VSCode will now run the container described in .devcontainer\Dockerfile and open the directory in this container.

To run this toolchain on Linux without Docker, you will need the following:
- Make for compiling the project
- OpenOCD for programming/debugging
- STM32 CubeMX for generating HAL libraries
Don't do it, it is not worth it.

## Building the Project
This section describes how to configure, build, and deploy the project. Lazo has created a very useful set of tools for configuring this repository that make this entire process significantly easier. The general steps are: 

1. Configure the Makefile
2. Build the project
3. Flash the program

These steps are elaborated in the sections below.

### Makefile Configuration
The makefile configuration tool sets up the makefile given the configuration/processor we want to use, and can be called via: 

```./tools/configure.sh -c data/configs/main -p . -m stm32l4```. 

The flags for this tool are shown below:

- ```-c``` path to the CubeMX config we want to use. In the example, we are using the config located in ```data/configs/main```. To compile for the backup board,this would be ```data/configs/backup``` 
- ```-p``` is the type of processor we want to compile to. In this case, these boards are based on the STM32L4.
- ```-e``` is a flag we use to add extra libraries to the makefile.

### Building the project
Once the makefile is configured, we can build the project. We can do this simply through make, with a call to: 

```make -j$(nproc)```

The ```-j$(nproc)``` flag uses all your computer's available cores so that the code is built faster by using parallel processing.

### Flashing code
Once the program is built, we can flash the board using OpenOCD. We do this with the command: 

```openocd -f interface/stlink.cfg -f target/stm32L4x.cfg -c "program ./build/main/main.bin 0x08000000 verify exit reset"```

Here, we first use -f to tell the program which programming method to use, then -f to tell it which processor we are targetting, followed by -c to tell OpenOCD which binary we are writing (change it from main to backup if flashing the backup board) and where to write it in memory. 

# Formatting standards
We wrote these originally but have realized we all just do snake_case all the time. So do that please.