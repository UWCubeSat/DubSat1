#ifndef test_msp430_H
#define test_msp430_H

#define P5IE char _p5ie = 0; _p5ie
#define BIT7 0
#define P5IES char _p5ies = 0; _p5ies
#define P5IFG char _p5ifg = 0; _p5ifg
#define P5DIR char _p5dir = 0; _p5dir
#define GIE (0x0008)

#define __bis_SR_register
#define __interrupt

#endif
