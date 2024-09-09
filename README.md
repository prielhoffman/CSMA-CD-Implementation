# CSMA/CD Implementation with 1-Persistent and Non-Persistent Modes

## Overview

This repository contains an implementation of CSMA/CD (Carrier Sense Multiple Access with Collision Detection) protocols, including both 1-persistent and non-persistent modes with Stop and Wait. The project demonstrates these protocols' behavior in a simulated communication environment using Arduino.

## Functions

The following functions are defined in the provided `EthernetLab5.h` file and are used in the implementation:

- **`void setAddress(int number, int pair)`**
  - **Description**: Configures the initial settings.
  - **Parameters**:
    - `number`: 0 or 1 (0 for one Arduino, 1 for the other).
    - `pair`: Pair number from Moodle.

- **`void setMode(int mod)`**
  - **Description**: Sets the mode of operation.
  - **Parameters**:
    - `mod`: `PRES` for 1-persistent, `NON_PRES` for non-persistent.

- **`int checkLine(void)`**
  - **Description**: Checks if the line is free.
  - **Returns**: 1 if the line is free, 0 otherwise.

- **`void startPackage(void *payload, int payload_size)`**
  - **Description**: Starts sending a frame to the server.
  - **Parameters**:
    - `payload`: A `char` array of data to send.
    - `payload_size`: Size of the data.

- **`int endPackage(int option)`**
  - **Description**: Called after propagation time (300 milliseconds).
  - **Parameters**:
    - `option`: 0 to stop mid-transmission (collision), 1 for successful transmission.

- **`int readPackage(char* payload, int payload_size)`**
  - **Description**: Reads frames and indicates new data arrival.
  - **Parameters**:
    - `payload`: Buffer to store received data.
    - `payload_size`: Max size of the buffer.
  - **Returns**: 1 if new data is available, 0 otherwise.

## Frame Structure

The frame sent consists of:
1. **Type**: 0 for data frame, 1 for ACK frame. Other types should be discarded.
2. **SN**: Sequence Number.
3. **Data Length**: Length in bytes of the `Data` field.
4. **Data**: The actual data being sent.

## Implementation Details

### CSMA/CD 1-Persistent

1. **Check Line**: Use `checkLine()` to determine if the line is free. If not, wait until it becomes free before transmitting.
2. **Collision Detection**: During transmission, check for collisions. If detected, stop transmission, wait a random time (Exponential Backoff), and retry.

### CSMA/CD Non-Persistent

1. **Check Line**: Use `checkLine()` to check if the line is free. If not, wait a random time before checking again.
2. **Collision Detection**: During transmission, check for collisions. If detected, stop transmission, wait for Exponential Backoff, and retry.

### Exponential Backoff

Exponential Backoff involves waiting for a random number of time slots:
- For the first collision, wait `U` time slots, where `U` is a random number between 0 and 15.
- For subsequent collisions, wait for a random number of slots from `{0, 31}`, `{0, 63}`, up to `{0, 1023}` (with a cap at 7 rounds).

The time to wait is calculated as `TIME_SLOT * U`.

## Usage

1. Download the `EthernetLab5.h` file from Moodle and place it in the same directory as your `.ino` file.
2. Import the header file in your `.ino` file:
   ```cpp
   #import "EthernetLab5.h"
3. Implement the CSMA/CD protocols in your code using the provided functions.
