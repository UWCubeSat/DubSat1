# MCP 25625 Getting Started

MCP25625 CAN interfaces via SPI.

It has more features than this, but this is a quick and dirty guide to
getting started. **It does not guarentee the most efficient implementation,** 
but is a reliable way to start.
Full documetnation can be found
[here](http://ww1.microchip.com/downloads/en/DeviceDoc/20005282B.pdf)

## Table of Commands

| Command        | Byte | Description                                 |
| -------------- | ----:|:-------------------------------------------:|
| RESET          | 0xC0 | Reset all registers, put into config mode   |
| READ           | 0x03 | Read a byte from a register                 |
| WRITE          | 0x02 | Set a register to a value                   |
| RTS            | 0x0? | Request to Send a message in the Tx Buffer* |
| READ STATUS    | 0xA0 | TODO                                        |
| RX STATUS      | 0xB0 | TODO                                        |
| BIT MODIFY     | 0x05 | Use a bit mask to set bits in a register    |

* ?=0x4 to send Tx Buffer 2, ?=0x2 to send Tx Buf 1, ?=0x1 to send Tx Buf 0
 
## Sequences
Here is a sequence of bits to transmit and receive over SPI we use to
initialize, Tx and Rx packets with our MCP:

### CAN Initialization

##### 1. Iinitializ

### CAN Sending

### CAN Rx