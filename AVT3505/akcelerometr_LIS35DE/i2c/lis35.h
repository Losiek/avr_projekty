//-----------------------------------------------------------------------------
//    KAmodMEMS2 software library
//-----------------------------------------------------------------------------
//
//    THE SOFTWARE INCLUDED IN THIS FILE IS FOR GUIDANCE ONLY.
//    BTC KORPORACJA SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
//    OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
//    FROM USE OF THIS SOFTWARE.
//
//-----------------------------------------------------------------------------


#define LIS35_ERROR 1
#define LIS35_OK 	0
#define LIS35_WRITE 0
#define LIS35_READ 	0x80
#define LIS35_ADDR_NO_INC 0
#define LIS35_ADDR_INC 0x40

#define LIS35_REG_OUTX 	0x29
#define LIS35_REG_OUTY	0x2B
#define LIS35_REG_OUTZ	0x2D

#define LIS35_REG_CR1 0x20
#define LIS35_REG_CR1_XEN 0x1
#define LIS35_REG_CR1_YEN 0x2
#define LIS35_REG_CR1_ZEN 0x4
#define LIS35_REG_CR1_DR_400HZ 0x80
#define LIS35_REG_CR1_ACTIVE 0x40
#define LIS35_REG_CR1_FULL_SCALE 0x20

#define LIS35_REG_CR2 0x21
#define LIS35_REG_CR2_BOOT 0x40

#define LIS35_CR3 0x22
#define LIS35_CR3_IHL 0x80
#define LIS35_CR3_CLICK_INT 0x7
#define LIS35_CR3_FF1_INT 0x1


#define LIS35_FF_WU_CFG_1 0x30
#define LIS35_FF_WU_SRC_1 0x31
#define LIS35_FF_WU_THS_1 0x32
#define LIS35_FF_WU_DURATION_1 0x33


#define LIS35_CLICK_CFG 0x38
#define LIS35_CLICK_THSY_X 0x3b
#define LIS35_CLICK_THSZ 0x3c
#define LIS35_CLICK_TIME_LIMIT 0x3D


#define LIS35_STATUS_REG 0x27

char InitializeLIS35();
void LIS35_ReadRegister(char addr,char * v);
void LIS35_WriteRegister(char addr,char v);
void LIS35_GetPosition(signed char * x, signed char * y, signed char * z);
