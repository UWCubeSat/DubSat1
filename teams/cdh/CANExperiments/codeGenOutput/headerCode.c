typedef struct Message5ValueTableSigs {
    uint8_t ValueTableSignal1; //  (No Units)
} Message5ValueTableSigs;

typedef struct Message4OddSizes {
    uint64_t OddSize3; //  (No Units)
    int16_t OddSize2; //  (No Units)
    uint8_t OddSize1; //  (No Units)
} Message4OddSizes;

typedef struct Message3 {
    double NormalDouble; //  (No Units)
} Message3;

typedef struct Message2Smaller {
    float NormalFloat; // 2.752
} Message2Smaller;

typedef struct Message1 {
    int8_t NormalSignedInt; //  (No Units)
    int32_t IntFactorOffset; //  (No Units)
    float FloatFactor; // FFGFGF!!!
    uint8_t NormalUint; // m/s
} Message1;

void encodeMessage5ValueTableSigs(Message5ValueTableSigs *input, CANPacket* output);
void decodeMessage5ValueTableSigs(CANPacket *input, Message5ValueTableSigs *output);

void encodeMessage4OddSizes(Message4OddSizes *input, CANPacket* output);
void decodeMessage4OddSizes(CANPacket *input, Message4OddSizes *output);

void encodeMessage3(Message3 *input, CANPacket* output);
void decodeMessage3(CANPacket *input, Message3 *output);

void encodeMessage2Smaller(Message2Smaller *input, CANPacket* output);
void decodeMessage2Smaller(CANPacket *input, Message2Smaller *output);

void encodeMessage1(Message1 *input, CANPacket* output);
void decodeMessage1(CANPacket *input, Message1 *output);

