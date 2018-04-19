# MCP25625 Getting Started

The MCP25625 has more features than this, but this is a quick and dirty guide to
getting started. **This guide does not guarentee the most efficient
implementation,** but is a reliable way to start.

Full documetnation can be found
[here](http://ww1.microchip.com/downloads/en/DeviceDoc/20005282B.pdf)

## Intro

MCP25625 CAN interfaces via SPI. To read a bit, we send 0x00 over the bus
and read it back.

## Table of Commands

| Command        | Byte | Description                                 |
| -------------- | ----:|:-------------------------------------------:|
| RESET          | 0xC0 | Reset all registers, put into config mode   |
| READ           | 0x03 | Read a byte from a register                 |
| WRITE          | 0x02 | Set a register to a value                   |
| RTS            | 0x0? | Request to Send a message in the Tx Buffer* |
| READ_STATUS    | 0xA0 | TODO                                        |
| RX_STATUS      | 0xB0 | TODO                                        |
| BIT_MODIFY     | 0x05 | Use a bit mask to set bits in a register    |

* ?=0x4 to send Tx Buffer 2, ?=0x2 to send Tx Buf 1, ?=0x1 to send Tx Buf 0

## Table of Registers/Macros

| Register/Macro | Byte | Description                   |
| -------------- | ----:|:-----------------------------:|
| CNF1           | 0xC0 | Configuration 1 Regsiter      |
| CNF2           | 0x03 | Configuration 2 Regsiter      |
| CNF3           | 0x02 | Configuration 3 Regsiter      |
| CANINTE        | 0x2B | CAN Interrupt Enable Register |
 
## Sequences
Here is a sequence of bits to transmit and receive over SPI we use to
initialize, Tx and Rx packets with our MCP:

### CAN Initialization

Here are the steps to follow to initialize the CAN bus. A full example
showing the initialization is at the bottom.

1. Reset
2. Set Configuration Bits
3. Enable the Rx Interrupt

#### 1. RESET

Commands:

```RESET```

Bits:

```0x07```

Translation:

First, you need to send the RESET instruction, as you may have power-cycled,
but the MCP may not have, so you should do this to make sure you and the MCP
agree on your state.

#### 2. Set the configuration bits

Commands:

```SET CNF1 0x83 SET CNF2 0xBF SET CNF3 0x02```

Bits:

```0x02 0x2A 0x83 0x02 0x29 0xBF 0x02 0x28 0x02```

Translation:

This sets registers CNF1 CNF2 and CNF3 to their correct values. This tells
the MCP25625 the speed of the bus (125kbps), the SJW (3), and some other
things.

#### 3. Enable the Rx Interrupt

Commands:

```BIT_MODIFY CANINTE 0x03 0x03```

Bits:

```0x05 0x2B 0x03 0x03```

Translation:

This enables the interrupts for the RX0Buffer and RX1Buffer. This will
mean the INT pin will go low when a new message has been received in either
buffer. 

#### Full Example:

Commands:

```
RESET
SET CNF1 0x83
SET CNF2 0xBF
SET CNF3 0x02

```

Bits:

```
0x07
0x02 0x2A 0x83 0x02 0x29 0xBF 0x02 0x28 0x02
```

Translation:

This fully initializes the MCP25625 so that it is ready to send and receive
messages at 125kbps, SJW 3

### CAN Sending



### CAN Rx