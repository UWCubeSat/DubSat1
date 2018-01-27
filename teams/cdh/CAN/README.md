# CAN Database Guide (CANdb++)
---
A CAN Database is a file that can be used to keep track of all the CAN messages and signals in a CAN Network. For our database, we are going to be using Vector's format (signified by the .dbc file extension). This guide will go through how to get started and work with the CAN database.

For anyone interested in how a dbc file is formated you can read about it [here](http://www.socialledge.com/sjsu/index.php?title=DBC_Format).

## Get database editor
The program we are going to be using for editing our database is called CANdb++ (by Vector). This editor is included in a lot of the products released by Vector which can be downloaded [here](https://vector.com/vi_canalyzer_en.html). Unfortunately, there is no Mac or Linux version of the software so you will have to be using a windows PC (or use bootcamp/virtual machine).

Once on the page, click the button labeled download and check the box for the demo version of the download with the Description "CANoe / CANalyzer 10.0 SP2 (64 bit.

![download page](link to download page image)

>Insert instructions on how to install and run CANdb++.

## Using the CANdb++ editor
When you open up the .dbc file in CANdb++, you will see a screen that looks like this.

![CANdb++ interface](link to CANdb++ interface image)

On the left hand side you can see 5 major sections: Networks, ECUs, Network Nodes, Messages, and Signals. For our purposes, we can ignore Networks (we only have one Network) and ECUs (Only really relevant for automobiles). Below is a description of the other three sections.

### - Signals
Signals are the individual pieces of data that will sent over CAN. Examples of signals would be something like "the temperature of battery 1" or "state of thrusters". To add a new signal, right click on the Signals section on the left column and select "new...". This will bring up a screen that looks something like this:

![New Signal Menu](link to new signal menu image)

Here is a break down of all the properties.
* __Name__ - the name of the signal (should be descriptive)
* __Length [bit]__ - the number of bits need for this piece of data. (If you don't understand what bits are, look up a tutorial on binary).
* __Byte Order__ - Whether the bits are read right-to-left or left-to-right. (keep this at Intel unless you know it should be otherwise).
* __Value Type__ - The type of the data. signed - Negative and Positive numbers, unsigned - only positive numbers, IEEE float - decimal data, IEEE double - larger decimal data.
* __Factor__ - How much the data is scaled by.
* __Minimum and Maximum__ - the min and max value that the data can be (in terms of the scaled, not the binary value of the bits).
* __Unit__ - The units for the data (Celsius, Voltage, etc.).
* __Init. Value__ - the default value for the signal.
* __Offset__ - The value that binary 0 represents.
* __Value Table__ - The table of named constants that can be use for this signal.

#### - Messages
Messages (also known as frames) are collections of signals over CAN communication. Same as with signals Messages can be created by right click on the message section on the left and selecting "new...". Here is a breakdown of the different message properties.
* __Name__ - the name of the message (should be descriptive)
* __Type__ - the type of the id. We will be using CAN Extended for all our messages.
* __ID__ - The CAN identifier for the message. This ID number is not arbitrary and can effect things such as the messages' priority. If you don't know what the ID of your message should be, speak with the cdh team.
* __DLC__ - the total length of the data (in bytes) contained in the message. This can be auto generated when attaching signals to the message.

To attach signals to your message, simply drag the individual signals onto the message's name in the left overall view panel. A message might pop up saying that the DLC or data length count is be auto generated. If this happens, select the option that says that you want the DLC to be auto generated.

#### - Network Nodes
This section contains all the nodes in the Network, with a node being any device communicating through CAN. For our purpose, these will basically be all the MSPs that will be communicating with other systems over CAN. Same as with the signals and messages, we can create a new node by right clicking and selecting "new...". When initially making a new network node, you only have to set the name of the node.

Once the node is created, you assign the messages that the node can sent. This is done by dragging and dropping the messages on to the name of the network node in the left overall view panel (same as assigning signals to a message).
