# Flight Model Magnetorquer Buck Converter Bridge

On the flight model magnetorquer board, the LTC3624 buck converter will not operate without adding a solder bridge to pin 3. This is the "RUN" pin and it must be wired HIGH for the converter to functin. This can be accomplished with a short wire between this pin and the VBAT pad of capacitor C4 immediately across from it. The pin is rated for up to 17 V.

Note: Pin 4 on the LTC3624 has also been left floating. This is the PGOOD pin and is not rated to handle VBAT voltages.