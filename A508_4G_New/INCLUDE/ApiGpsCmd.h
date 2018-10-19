#ifndef __APIGPSCMD_H
#define __APIGPSCMD_H

extern void ApiBeidou_PowerOnInitial(void);

extern bool beidou_valid(void);
extern u32 beidou_longitude_degree(void);//度
extern u32 beidou_longitude_minute(void);//分
extern u32 beidou_longitude_second(void);//小数点后的数
extern u32 beidou_latitude_degree(void);//度
extern u32 beidou_latitude_minute(void);//分
extern u32 beidou_latitude_second(void);//小数点后的数

extern  u32  CHAR_TO_Digital(u8 * pBuf, u8 Len);
extern void  Digital_TO_CHAR(u8 * pBuf, u32 data, u8 Len);
extern void  CHAR_TO_DIV_CHAR(u8 * pPrimary, u8 * pDestination, u8 Len);

extern u8 BeidouRxData[75];
extern u8 BeidouRxDataLen;
extern u8 RxStartFlag[6];
extern void ApiBeidou_Get_location_Information(void);
#endif