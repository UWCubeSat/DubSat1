/*
   This file goes with parsingInLoop to parse the different kinds of logs from
   the GPS receiver.
*/

typedef struct {
  ENUM solStat;
  ENUM posType;
  double lat;
  double lon;
  double hgt;
  float undulation;
  ENUM datumId;
  float latDev;
  float lonDev;
  float hgtDev;
  char stnId[4];
  float diffAge;
  float solAge;
  unsigned char svs;
  unsigned char solnSvs;
  unsigned char solnL1Svs;
  unsigned char solnMultiSvs;
  char reserved;
  char extSolStat;
  char galileoBeiDouSigMask;
  char gpsGlonassSigMask;
  char crc[4];
} BestPos;

typedef struct {
  ENUM pSolStat;
  ENUM posType;
  double pX;
  double pY;
  double pZ;
  float pXDev;
  float pYDev;
  float pZDev;
  ENUM vSolStat;
  ENUM velType;
  double vX;
  double vY;
  double vZ;
  float vXDev;
  float vYDev;
  float vZDev;
  char stnId[4];
  float vLatency;
  float diffAge;
  float solAge;
  unsigned char svs;
  unsigned char solnSvs;
  unsigned char ggL1;
  unsigned char solnMultiSvs;
  char reserved;
  char extSolStat;
  char galileoBeiDouSigMask;
  char gpsGlonassSigMask;
  char crc[4];
} BestXYZ;

typedef struct {
  ENUM clockStatus;
  double offset;
  double offsetDev;
  double utcOffset;
  unsigned long utcYear;
  unsigned char utcMonth;
  unsigned char utcDay;
  unsigned char utcHour;
  unsigned char utcMin;
  unsigned long utcMs;
  ENUM utcStatus;
  char crc[4];
} Time;

/**
   Parses BESTPOS data from binary starting after the header.
   Gives latitude and longitude in degrees, altitude in meters.
*/
void parseBestPos(Stream *s) {  
  BestPos *bestPos = malloc(sizeof(BestPos));
  readThing(s, bestPos, sizeof(BestPos));
  Serial.print("solution status: ");
  Serial.println(bestPos->solStat);
  Serial.println(bestPos->lat);
  Serial.println(bestPos->lon);
  Serial.println(bestPos->hgt);
  free(bestPos);
}

/**
   Parses BESTXYZ data from binary starting after the header.
   Gives position in cartesian, ECEF coordinates.
   ECEF is easier to convert to orbital parameters and is potentially easier
   for the GPS to compute.
*/
void parseBestXYZ(Stream *s) {
  BestXYZ *bestXYZ = malloc(sizeof(BestXYZ));
  readThing(s, bestXYZ, sizeof(BestXYZ));
  Serial.print("solution status: ");
  Serial.println(bestXYZ->pSolStat);
  Serial.println(bestXYZ->pX);
  Serial.println(bestXYZ->pY);
  Serial.println(bestXYZ->pZ);
  free(bestXYZ);
}

/**
   Parses TIME data from binary starting after the header.
   See page 715 of the firmware reference.
*/
void parseTime(Stream *s) {
  Time *time = malloc(sizeof(Time));
  readThing(s, time, sizeof(Time));
  Serial.println("date: ");
  Serial.println(time->utcYear);
  Serial.println(time->utcMonth);
  Serial.println(time->utcDay);
  Serial.println(time->utcHour);
  Serial.println(time->utcMin);
  Serial.println(time->utcMs);
  free(time);
}

