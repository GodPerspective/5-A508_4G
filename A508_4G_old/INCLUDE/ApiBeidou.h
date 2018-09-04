#ifndef __APIBEIDOU_H
#define __APIBEIDOU_H
#define COML_FUNENABLE

#define BEIDOU//Ĭ������GPS��ע��ȥ��Ϊ���ñ�����������������
#define BEIDOU_FUNENABLE

#ifdef	BEIDOU_FUNENABLE
  #define BEIDOUAPI extern
#else
  #define BEIDOUAPI 
#endif


extern void ApiBeidou_PowerOnInitial(void);
extern bool beidou_valid(void);
extern u32 beidou_longitude_degree(void);//��
extern u32 beidou_longitude_minute(void);//��
extern u32 beidou_longitude_second(void);//С��������
extern u32 beidou_latitude_degree(void);//��
extern u32 beidou_latitude_minute(void);//��
extern u32 beidou_latitude_second(void);//С��������

extern  u32  CHAR_TO_Digital(u8 * pBuf, u8 Len);
extern void  Digital_TO_CHAR(u8 * pBuf, u32 data, u8 Len);
extern void  CHAR_TO_DIV_CHAR(u8 * pPrimary, u8 * pDestination, u8 Len);

BEIDOUAPI u8 BeidouRxData[75];
BEIDOUAPI u8 BeidouRxDataLen;
BEIDOUAPI u8 RxStartFlag[6];
BEIDOUAPI void ApiBeidou_Get_location_Information(void);
#endif