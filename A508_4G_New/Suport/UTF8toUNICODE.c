#include "ALLHead.h"
//#include <assert.h>
//#include <stddef.h>
char utf8Str1[3] = {0xe6, 0x89, 0x93}; 
char utf8Str2[3] = {0xe5, 0x8D, 0xA1}; 
char utf8Str3[3] = {0xe5, 0xA4, 0xB1}; 
char utf8Str4[3] = {0xe8, 0xB4, 0xA5}; 
char unicodeStr[2];
  
int Utf8ToUnicode(char* pInput, char* pOutput);

void UTF8toUNICODE_test(void)
{
  //utf8תunicode
  int num;
  memset(unicodeStr,0,2);
  num = Utf8ToUnicode(utf8Str1, unicodeStr);
  num = Utf8ToUnicode(utf8Str2, unicodeStr);
  num = Utf8ToUnicode(utf8Str3, unicodeStr);
  num = Utf8ToUnicode(utf8Str4, unicodeStr);
  if (num == -1)
  {
    num=num;
  }
  else
  {
    unsigned char* p = (unsigned char*)unicodeStr;
     num=num;
  }
}




/*************************************************************************************************
* ��UTF8����ת����Unicode��UCS-2LE������  �͵�ַ���λ�ֽ�
* ������
*    char* pInput     �����ַ���
*    char*pOutput   ����ַ���
* ����ֵ��ת�����Unicode�ַ������ֽ�������������򷵻�-1
**************************************************************************************************/
//utf8תunicode
int Utf8ToUnicode(char* pInput, char* pOutput)
{
  int outputSize = 0; //��¼ת�����Unicode�ַ������ֽ���
  while (*pInput)
  {
    if (*pInput > 0x00 && *pInput <= 0x7F) //�����ֽ�UTF8�ַ���Ӣ����ĸ�����֣�
    {
      *pOutput = *pInput;
      pOutput++;
      *pOutput = 0; //С�˷���ʾ���ڸߵ�ַ�0
    }
    else if (((*pInput) & 0xE0) == 0xC0) //����˫�ֽ�UTF8�ַ�
    {
      char high = *pInput;
      pInput++;
      char low = *pInput;
      if ((low & 0xC0) != 0x80)  //����Ƿ�Ϊ�Ϸ���UTF8�ַ���ʾ
                  {
                    return -1; //��������򱨴�
                  }
                  *pOutput = (high << 6) + (low & 0x3F);
                  pOutput++;
                  *pOutput = (high >> 2) & 0x07;
    }
    else if (((*pInput) & 0xF0) == 0xE0) //�������ֽ�UTF8�ַ�
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
                  *pOutput = (middle << 6) + (low & 0x3F);//ȡ��middle�ĵ���λ��low�ĵ�6λ����ϳ�unicode�ַ��ĵ�8λ
                  pOutput++;
                  *pOutput = (high << 4) + ((middle >> 2) & 0x0F); //ȡ��high�ĵ���λ��middle���м���λ����ϳ�unicode�ַ��ĸ�8λ
		}
		else //���������ֽ�����UTF8�ַ������д���
		{
                  return -1;
		}
                pInput ++;//������һ��utf8�ַ�
		pOutput ++;
		outputSize += 2;
  }
	//unicode�ַ������棬������\0
	*pOutput = 0;
	 pOutput++;
	*pOutput = 0;
	return outputSize;
}

