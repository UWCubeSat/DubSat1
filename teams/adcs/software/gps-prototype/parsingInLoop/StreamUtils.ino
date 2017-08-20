void skip(Stream *s, int n) {
  char buf[n];
  s->readBytes(buf, n);
}

double readDouble(Stream *s) {
  double d;
  readThing(s, &d, sizeof(double));
  return d;
}

unsigned int readEnum(Stream *s) {
  unsigned int e;
  readThing(s, &e, sizeof(unsigned int));
  return e;
}

unsigned char readByte(Stream *s) {
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
  char buf[size];
  s->readBytes(buf, size);
  memcpy(ptr, buf, size);
}

