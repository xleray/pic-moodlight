// definitins used for the board test routine


//return values
#define		TEST_OK			0
#define		TEST_GND		1
#define		TEST_VCC		2
#define		TEST_ZERO_A	3
#define		TEST_ZERO_B	4
#define		TEST_ZERO_C	5
#define		TEST_ZERO_D	6
#define		TEST_ZERO_E	7
#define		TEST_ZERO_F	8
#define		TEST_ZERO_G	9
#define		TEST_ZERO_H 10
#define		TEST_ZERO_J	11
#define		TEST_RTC		12
#define		TEST_ANALOG	13

#define DELAY_TIME 			300
#define DT 					150
#define	DELAY_TIME_LONG		2000

#define	BIT0	0x01
#define	BIT1	0x02
#define	BIT2	0x04
#define	BIT3	0x08
#define	BIT4	0x10
#define	BIT5	0x20
#define	BIT6	0x40
#define	BIT7	0x80


#define DELAY_TIME 			300
#define DT 					150
#define	DELAY_TIME_LONG		2000
	
unsigned char TestExt(void);
void TestOptrons(void);
void InitRTC(void);
