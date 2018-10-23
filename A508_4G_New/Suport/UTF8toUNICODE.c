#include "ALLHead.h"
//#include <assert.h>
//#include <stddef.h>


/*void main(void)
{
  //utf8תunicode
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
    }//���1162
    printf("\n");
  }
  delete unicodeStr;
}*/




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

