void skip(Stream *s, int n) {
  char buf[n];
  s->readBytes(buf, n);
}

double readDouble(Stream *s) {
  double d;
  readThing(s, &d, sizeof(double));
  return d;
}

ENUM readEnum(Stream *s) {
  return readLong(s);
}

unsigned char readChar(Stream *s) {
  unsigned char b;
  readThing(s, &b, sizeof(unsigned char));
  return b;
}

unsigned short readShort(Stream *s) {
  unsigned short sh;
  readThing(s, &sh, sizeof(unsigned short));
  return sh;
}

unsigned long readLong(Stream *s) {
  unsigned long lo;
  readThing(s, &lo, sizeof(unsigned long));
  return lo;
}

void readThing(Stream *s, void *ptr, int size) {
  s->readBytes((char*) ptr, size);
}

