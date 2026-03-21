# Arduino CSMA/CD Protocol Implementation

An **Arduino C++ implementation of CSMA/CD** that supports both **1-persistent** and **non-persistent** transmission modes, along with **collision detection**, **Stop-and-Wait reliability**, and **exponential backoff**.

This project focuses on low-level communication behavior in a shared medium and demonstrates how medium access control logic can be implemented in an embedded environment.

## Tech Stack

- **Language:** C++
- **Platform:** Arduino
- **Concepts:** CSMA/CD, collision detection, exponential backoff, Stop-and-Wait, embedded communication, protocol implementation

## What the Project Does

The project simulates communication between devices sharing the same transmission medium.

It implements key MAC-layer behaviors, including:

- sensing whether the line is free before transmission
- selecting between **1-persistent** and **non-persistent** access strategies
- detecting collisions during transmission
- stopping and retrying transmission after a collision
- using **exponential backoff** to reduce repeated contention
- transmitting data and acknowledgment frames using a simple frame format

The implementation is built on top of the provided `Ethernet.h` interface and focuses on the protocol logic itself.

## Main Features

- CSMA/CD with **1-persistent** mode
- CSMA/CD with **non-persistent** mode
- collision handling during transmission
- exponential backoff after collisions
- Stop-and-Wait style acknowledgment flow
- frame-based communication with sequence numbers
- Arduino-based embedded implementation

## Provided Interface

The implementation uses the following functions from `Ethernet.h` :contentReference[oaicite:1]{index=1}:

- `setAddress(int number, int pair)` – configure device identity
- `setMode(int mod)` – choose persistent or non-persistent mode
- `checkLine()` – check whether the line is free
- `startPackage(void *payload, int payload_size)` – start sending a frame
- `endPackage(int option)` – complete or abort transmission after propagation delay
- `readPackage(char* payload, int payload_size)` – receive incoming frames

## Frame Structure

The transmitted frame includes:

1. **Type** – `0` for data, `1` for ACK
2. **SN** – sequence number
3. **Data Length** – length of payload in bytes
4. **Data** – frame payload

This structure supports reliable exchange using acknowledgments and sequence tracking. :contentReference[oaicite:2]{index=2}

## Protocol Behavior

### 1-Persistent Mode

In **1-persistent CSMA/CD**, the sender continuously checks the line until it becomes free, then transmits immediately.

If a collision is detected:
- transmission is stopped
- the sender waits using exponential backoff
- the frame is retransmitted later

### Non-Persistent Mode

In **non-persistent CSMA/CD**, the sender checks whether the line is free.

If the line is busy:
- the sender waits a random amount of time
- the line is checked again later
- this reduces the chance of repeated collisions under contention

If a collision still occurs:
- transmission is aborted
- exponential backoff is applied
- retransmission is attempted

## Exponential Backoff

After a collision, the sender waits for a random number of time slots before retrying.

The waiting window grows after each collision attempt. According to the implementation requirements, the backoff range expands from small values up to a capped maximum window. :contentReference[oaicite:3]{index=3}

This behavior helps reduce repeated collisions when multiple devices compete for the medium.

## What I Implemented

This project focused on implementing protocol behavior rather than using a high-level networking library.

Key implementation areas included:

- line sensing before transmission
- persistent and non-persistent access logic
- collision handling
- retry behavior using exponential backoff
- frame construction and parsing
- acknowledgment-based transmission flow
- embedded protocol behavior using Arduino APIs

## Why This Project Matters

This project demonstrates practical understanding of:

- medium access control
- contention on shared communication lines
- embedded communication logic
- protocol design and implementation
- collision avoidance and recovery strategies
- reliability mechanisms such as acknowledgments and sequence numbers

It is a strong example of low-level systems and communication work, especially because it focuses on protocol behavior rather than only application-level programming.

## How It Works

A typical transmission flow looks like this:

1. configure device address and transmission mode
2. check whether the line is free
3. send a frame if the medium is available
4. monitor for collision during transmission
5. stop and retry if a collision occurs
6. receive an ACK frame when transmission succeeds
7. continue with the next frame

## Core Concepts Practiced

- medium access control
- CSMA/CD
- 1-persistent vs. non-persistent transmission
- collision detection
- exponential backoff
- Stop-and-Wait reliability
- frame-based communication
- embedded systems programming

## Key Takeaways

Through this project, I strengthened my understanding of:

- how shared-medium communication protocols behave under contention
- how persistent and non-persistent strategies differ
- how collision handling affects throughput and reliability
- how low-level protocol logic can be implemented on embedded hardware
- how acknowledgment and retransmission mechanisms support reliable delivery

## Future Improvements

Possible next steps for the project:

- add clearer debugging and event logging
- measure collision and retry statistics
- compare persistent and non-persistent performance experimentally
- visualize transmission attempts and backoff timing
- extend the protocol to support more than two communicating devices
- document the hardware setup and testing workflow
