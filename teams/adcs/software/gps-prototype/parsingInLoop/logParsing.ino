/*
   This file goes with parsingInLoop to parse the different kinds of logs from
   the GPS receiver.
*/

/**
   Parses BESTPOS data from binary starting after the header.
   Gives latitude and longitude in degrees, altitude in meters.
*/
void parseBestpos(Stream *s) {  
  unsigned int solStatus = readEnum(s);
  double lat = readDouble(s);
  double lon = readDouble(s);
  double height = readDouble(s);

  Serial.print("solution status: ");
  Serial.println(solStatus);
  Serial.println(lat);
  Serial.println(lon);
  Serial.println(height);
}

/**
   Parses BESTXYZ data from binary starting after the header.
   Gives position in cartesian, ECEF coordinates.
   ECEF is easier to convert to orbital parameters and is potentially easier
   for the GPS to compute.
*/
void parseBestxyz(Stream *s) {
  unsigned int solStatus = readEnum(s);
  double x = readDouble(s);
  double y = readDouble(s);
  double z = readDouble(s);

  Serial.print("solution status: ");
  Serial.println(solStatus);
  Serial.println(x);
  Serial.println(y);
  Serial.println(z);
}

/**
   Parses TIME data from binary starting after the header.
   See page 715 of the firmware reference.
*/
void parseTime(Stream *s) {
  skip(s, 28);
  
  unsigned long year = readLong(s);
  unsigned char month = readByte(s);
  unsigned char day = readByte(s);
  unsigned char hour = readByte(s);
  unsigned char minute = readByte(s);
  unsigned long ms = readLong(s);

  Serial.println("date: ");
  Serial.println(year);
  Serial.println(month);
  Serial.println(day);
  Serial.println(hour);
  Serial.println(minute);
  Serial.println(ms);
}

