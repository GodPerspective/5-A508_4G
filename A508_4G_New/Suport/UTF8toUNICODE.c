#include "ALLHead.h"
//#include <assert.h>
//#include <stddef.h>


/*void main(void)
{
  //utf8转unicode
  char utf8Str[4] = {0xe6, 0x88, 0x91, 0x00}; 
  char* unicodeStr = new char[8];
  memset(unicodeStr,0,8);
  int num = Utf8ToUnicode(utf8Str, unicodeStr);
  if (num == -1)
  {
    printf("Error!\n");
  }
  else
  {
    unsigned char* p = (unsigned char*)unicodeStr;
    for (int i = 0; i < num; i++)
    {
      printf("%0x", *p);
      p++;
    }//输出1162
    printf("\n");
  }
  delete unicodeStr;
}*/




/*************************************************************************************************
* 将UTF8编码转换成Unicode（UCS-2LE）编码  低地址存低位字节
* 参数：
*    char* pInput     输入字符串
*    char*pOutput   输出字符串
* 返回值：转换后的Unicode字符串的字节数，如果出错则返回-1
**************************************************************************************************/
//utf8转unicode
int Utf8ToUnicode(char* pInput, char* pOutput)
{
  int outputSize = 0; //记录转换后的Unicode字符串的字节数
  while (*pInput)
  {
    if (*pInput > 0x00 && *pInput <= 0x7F) //处理单字节UTF8字符（英文字母、数字）
    {
      *pOutput = *pInput;
      pOutput++;
      *pOutput = 0; //小端法表示，在高地址填补0
    }
    else if (((*pInput) & 0xE0) == 0xC0) //处理双字节UTF8字符
    {
      char high = *pInput;
      pInput++;
      char low = *pInput;
      if ((low & 0xC0) != 0x80)  //检查是否为合法的UTF8字符表示
                  {
                    return -1; //如果不是则报错
                  }
                  *pOutput = (high << 6) + (low & 0x3F);
                  pOutput++;
                  *pOutput = (high >> 2) & 0x07;
		}
		else if (((*pInput) & 0xF0) == 0xE0) //处理三字节UTF8字符
		{
                  char high = *pInput;
                  pInput++;
                  char middle = *pInput;
                  pInput++;
                  char low = *pInput;
                  if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))
                  {
                    return -1;
                  }
                  *pOutput = (middle << 6) + (low & 0x3F);//取出middle的低两位与low的低6位，组合成unicode字符的低8位
                  pOutput++;
                  *pOutput = (high << 4) + ((middle >> 2) & 0x0F); //取出high的低四位与middle的中间四位，组合成unicode字符的高8位
		}
		else //对于其他字节数的UTF8字符不进行处理
		{
                  return -1;
		}
                pInput ++;//处理下一个utf8字符
		pOutput ++;
		outputSize += 2;
	}
	//unicode字符串后面，有两个\0
	*pOutput = 0;
	 pOutput++;
	*pOutput = 0;
	return outputSize;
}

