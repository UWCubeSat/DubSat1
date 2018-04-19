# MCP25625 Getting Started

The MCP25625 has more features than this, but this is a quick and dirty guide to
getting started. **This guide does not guarentee the most efficient
implementation,** but is a reliable way to start.

Full documetnation can be found
[here](http://ww1.microchip.com/downloads/en/DeviceDoc/20005282B.pdf)

## Intro

MCP25625 CAN interfaces via SPI.

Most of the interaction with the MCP will be in the form of a command, usually
followed a register address, possibly followed by 1 or 2 more arguments for a
value. If that command was to read something, sending an extra bit (we use 0x00)
will the MCP25625 to respond over the MISO/SDO SPI Line during that extra bit
transmission. 

The chip has 3 modes. When powering on or resetting the MCP, it goes into
configuration mode. It will only send and recieve packets after it is set
to normal mode after you've finished your configuration routine.

The chip has 3 send buffers. You can send any one of the 3 buffers. They
are mainly there for your convenience, as far as we know.

The chip has 2 receive buffers. Unlike the Tx buffers, these serve a more
important purpose. Along with each Rx buffer, there are filters associated
with it, which will reduce the number of interrupts you get for messages
you don't actually care about.

The filters work on extended IDs or standard IDs + the first 2 bits of the
message. We use extended ID filtering for HuskySat-1. Each buffer has its own
bit mask, Rx0 has 2 filters for its mask, and Rx1 has 4 filters for its mask.
The acceptance condition is as follows: For every bit in the mask that is a 1,
the corresponding filter bit must match the id bit for a message to be accepted.

The filters on Rx0 have a higher priority than the filters on Rx1.

If the filters are confusing, there is a more detailed description in the
full documentation, linked at the top.

## Table of Commands

| Command        | Byte | Description                                     |
| -------------- | ----:|:-----------------------------------------------:|
| RESET          | 0xC0 | Reset all registers, put into config mode       |
| READ           | 0x03 | Read a byte from a register                     |
| WRITE          | 0x02 | Set a register to a value                       |
| RTS            | 0x0? | Request to Send a message in the Tx Buffer (1)  |
| READ_STATUS    | 0xA0 | TODO                                            |
| RX_STATUS      | 0xB0 | TODO                                            |
| BIT_MODIFY     | 0x05 | Use a bit mask to set bits in a register        |
| LOAD_TX_BUFFER | 0x4? | Shorthand for sending several bits to Tx Buf (2)|

(1) ?=0x4 to send Tx Buffer 2, ?=0x2 to send TxB 1, ?=0x1 to send TxB 0

(2) ?=0x0 for Tx Buffer 0 ID, ?=0x01 for TxB 0 Data, ?=0x2 for TxB 1 ID, ?=0x03
for TxB 1 Data, ?=0x4 for TxB 2 ID, ?=0x05 for TxB 2 Data.

## Table of Registers/Macros

This does not include all Registers, just the ones that are
necessary to get started.

| Register/Macro | Byte | Description                   |
| -------------- | ----:|:-----------------------------:|
| CNF1           | 0xC0 | Configuration 1 Regsiter      |
| CNF2           | 0x03 | Configuration 2 Regsiter      |
| CNF3           | 0x02 | Configuration 3 Regsiter      |
| CANINTE        | 0x2B | CAN Interrupt Enable Register |
| RXB0CTRL       | 0x60 | Rx Buffer 0 Control Register  |
| RXB1CTRL       | 0x70 | Rx Buffer 1 Control Register  |
| CANCTRL        | 0x0F | CAN Control Register          |

 
## Sequences
Here is a sequence of bits to transmit and receive over SPI we use to
initialize, Tx and Rx packets with our MCP:

### CAN Initialization

Here are the steps to follow to initialize the CAN bus. A full example
showing the initialization is at the bottom.

1. Reset
2. Set Configuration Bits
3. Enable the Rx Interrupt
4. Acceptance Filter Settings
5. Set operating mode to normal

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
things. This is what we are using for HuskySat-1.

#### 3. Enable the Rx Interrupt

Commands:

```BIT_MODIFY CANINTE 0x03 0x03```

Bits:

```0x05 0x2B 0x03 0x03```

Translation:

This enables the interrupts for the RX0Buffer and RX1Buffer. This will
mean the INT pin will go low when a new message has been received in either
buffer. 

#### 4. Acceptance Filter Settings

Commands:

```BIT_MODIFY RXB0CTRL 0x60 0x00 BIT_MODIFY RXB1CTRL 0x60 0x00```

Bits:

```0x05 0x60 0x60 0x00 0x05 0x70 0x60 0x00```

Translation:

This sets the acceptance filters to filter only on extended IDs. This is
what we are using for Huskysat-1.


#### 5. Set Operating mode to Normal

Commands:

```BIT_MODIFY CANCTRL 0xE0 0x00```

Bits:

```0x05 0x0F 0xE0 0x00```

Translation:

This sets the acceptance filters to filter only on extended IDs. This is
what we are using for Huskysat-1.

#### Full Example:

Commands:

```
RESET

SET CNF1 0x83
SET CNF2 0xBF
SET CNF3 0x02

BIT_MODIFY CANINTE 0x03 0x03

BIT_MODIFY RXB0CTRL 0x60 0x00
BIT_MODIFY RXB1CTRL 0x60 0x00
BIT_MODIFY CANCTRL 0xE0 0x00
```

Bits:

```
0x07

0x02 0x2A 0x83
0x02 0x29 0xBF
0x02 0x28 0x02

0x05 0x2B 0x03 0x03

0x05 0x60 0x60 0x00
0x05 0x70 0x60 0x00

0x05 0x0F 0xE0 0x00
```

Translation:

This fully initializes the MCP25625 so that it is ready to send and receive
messages at 125kbps, SJW 3

### CAN Sending

Unfortunately, the ID register bits don't really align with any normal byte
alignment. Don't worry, though as we've already solved this problem and will
provide example code to translate an extedned ID encoded as a uint32_t
into the correct bytes to load to the MCP25625.

We will be sending a packet of length 8 with ID 0x1BADASSS and
Data 0x0123456789ABCDEF.

A full example showing the initialization is at the bottom.

1. Convert ID to align with Registers
2. Load ID into TX Buffer
3. Load Data into TX Buffer
4. RTS (Request to Send)

#### 1. Convert ID to align with Registers

Code:

```c
uint32_t id = 0x1BADASSS
uint8_t NEWID_1 = (uint8_t) (id >> 21),
uint8_t NEWID_1 = (uint8_t) (id >> 16) & 0x03 | (uint8_t) (packet->id >> 13) & 0xE0 | 0x08,
uint8_t NEWID_1 = (uint8_t) (id >> 8),
uint8_t NEWID_1 = (uint8_t) id
```

Translation:

We need to load the ID into the proper register, but it the registers are
not all byte aligned so we need to do some translation. the results are in
ID1 ID2 ID3 ID4

#### 1. Convert ID to align with Registers

Commands:

```RST```

Bits:

```0x07```

Translation:

First, you need to send the RESET instruction, as you may have power-cycled,
but the MCP may not have, so you should do this to make sure you and the MCP
agree on your state.

#### Full Example:

Commands:

```
RESET

SET CNF1 0x83
SET CNF2 0xBF
SET CNF3 0x02

BIT_MODIFY CANINTE 0x03 0x03

BIT_MODIFY RXB0CTRL 0x60 0x00
BIT_MODIFY RXB1CTRL 0x60 0x00
BIT_MODIFY CANCTRL 0xE0 0x00
```

Bits:

```
0x07

0x02 0x2A 0x83
0x02 0x29 0xBF
0x02 0x28 0x02

0x05 0x2B 0x03 0x03

0x05 0x60 0x60 0x00
0x05 0x70 0x60 0x00

0x05 0x0F 0xE0 0x00
```

Translation:

This fully initializes the MCP25625 so that it is ready to send and receive
messages at 125kbps, SJW 3

### CAN Receiving