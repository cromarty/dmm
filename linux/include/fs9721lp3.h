/**
*
* This header file contains definitions which relate to the Fortune Semiconductor FS9721_LP3 DMM
* chip which is contained in the Unit-T Digital Multimeters, including my UT60A.
*
*/
#ifndef FS9721LP3_H
#define FS9721LP3_H

/* The BAUD rate for this chip is always 2400 */
#define BAUDRATE B2400            


#define NEGATIVE_FLAG 0x08
#define DECPOINT_FLAG 0x08


// flags in byte 0 (first of 14)
#define AC_FLAG 0x08
#define DC_FLAG 0x04
#define AUTO_FLAG 0x02
#define RS232_FLAG 0x01

// flags in byte 9 (tenth of 14)
#define MICRO_FLAG 0x08
#define NANO_FLAG 0x04
#define KILO_FLAG 0x02
#define DIODE_FLAG 0x01

// flags in byte 10 (eleventh of 14)
#define MILLI_FLAG 0x08
#define PERCENT_FLAG 0x04
#define MEGA_FLAG 0x02
#define BEEP_FLAG 0x01

// flags in byte 11 (twelfth of 14)
#define FARADS_FLAG 0x08
#define OHMS_FLAG 0x04
#define RELATIVE_FLAG 0x02
#define HOLD_FLAG 0x01

// flags in byte 12 (thirteenth of 14)
#define AMPS_FLAG 0x08
#define VOLTS_FLAG 0x04
#define HERTZ_FLAG 0x02
#define LOW_BATTERY_FLAG 0x01

// flags in byte 13 (fourteenth of 14)
#define USER_BIT3_FLAG 0x08
#define USER_BIT2_FLAG 0x04
#define USER_BIT1_FLAG 0x02
#define USER_BIT0_FLAG 0x01

/**
* Note: The flags in the user flag byte, which is the last byte, may have different meanings
* on different models of Unit-T DMMs.
*
*/



/**
*
* 14-byte frame used to receive data from DMM and passed back to the callback function from the read
* thread.
*
*/
	typedef union {
	unsigned char data[14];
	struct {
		unsigned char mode;
		unsigned char digit[8];
		unsigned char flag[4];
		unsigned char user;
	};
} FS9721_LP3_FRAME_T;



#endif



