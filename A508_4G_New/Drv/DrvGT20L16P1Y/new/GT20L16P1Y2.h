#ifndef _GT20L16P1Y2_H_
#define _GT20L16P1Y2_H_

//---------------------------------------------------------------------------------
// 函数： unsigned long unicode_muti_addr( unsigned short unicode)
// 说明：
// 包括 拉丁文、希腊文、基里尔文、希伯来文、泰文
// BaseAdd：说明本套字库在字库芯片中的起始字节地址。
// unicode：表示unicode内码（16bits）
// Address：表示汉字点阵在芯片中的字节地址。
//计算方法：
//其中函数unsigned char find_zf_tab(unsigned short unicode) 参考下列代码

//extern void r_dat_bat(unsigned long ADDRESS,unsigned int len,unsigned char *DZ_Data);


unsigned long unicode_muti_addr( unsigned short unicode,unsigned char *DZ_Data);

//---------------------------------------------------------------------------------
//函数：unsigned long unicode_alb_addr( unsigned short unicode_alb)
//说明：
//16点阵不等宽阿拉伯文系字符
//BaseAdd：说明本套字库在字库芯片中的起始字节地址。
//unicode_alb：表示unicode内码（16bits）
//Address：表示汉字点阵在芯片中的字节地址。
//计算方法：
unsigned long unicode_alb_addr( unsigned short unicode_alb,unsigned char *DZ_Data);

//---------------------------------------------------------------------------------
//函数：unsigned long GB2312_addr( unsigned char MSB,unsigned char LSB )
//说明：
//15X16 GB2312编码汉字字符
//ZFAdd、HZAdd：表示字符和汉字的起始地址。
//MSB、LSB：表示GB2312编码的高字节和低字节。
//GB2312tab：表示GB2312转码表起始地址。
//Address：表示汉字点阵在芯片中的字节地址。
unsigned long GB2312_addr( unsigned char MSB,unsigned char LSB,unsigned char *DZ_Data);

//---------------------------------------------------------------------------------
//函数：unsigned long unicode_addr( unsigned short Unicode )
//说明：
//15X16 Unicode编码汉字字符
//ZFAdd、HZAdd：表示字符和汉字的起始地址。
//Unicode：表示汉字的unicode编码。
//Unicodetab：表示unicode转码表起始地址。
//Address：表示汉字点阵在芯片中的字节地址。
unsigned long unicode_addr( unsigned short Unicode,unsigned char *DZ_Data);


#endif

