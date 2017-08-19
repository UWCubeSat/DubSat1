/*
   This file goes with parsingInLoop to parse the different kinds of logs from
   the GPS receiver.
*/

/**
   Parses BESTPOS data from binary starting after the header.
   Gives latitude and longitude in degrees, altitude in meters.
*/
void parseBestpos() {
  unsigned int solStatus = *((unsigned int*) buf);
  double lat = *((double*) (buf + 8));
  double lon = *((double*) (buf + 16));
  double height = *((double*) (buf + 24));

  Serial.println("solution status: " + solStatus);
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
void parseBestxyz() {
  unsigned int solStatus = *((unsigned int*) buf);
  double x = *((double*) (buf + 8));
  double y = *((double*) (buf + 16));
  double z = *((double*) (buf + 24));

  Serial.println("solution status: " + solStatus);
  Serial.println(x);
  Serial.println(y);
  Serial.println(z);
}

/**
   Parses TIME data from binary starting after the header.
   See page 715 of the firmware reference.
*/
void parseTime() {
  unsigned long year = *((unsigned long*) (buf + 28));
  unsigned char month = *(buf + 32);
  unsigned char day = *(buf + 33);
  unsigned char hour = *(buf + 34);
  unsigned char minute = *(buf + 35);
  unsigned long ms = *((unsigned long*) (buf + 36));
}

