#include <stdio.h>


typedef unsigned short ucs2; //Unicode2 16bits.
typedef unsigned char utf8;  // utf-8 8bits.

/*
 *
 * 一：e4 			b8 		80 (utf-8)
 *    1110 0000  1011 1000  1000 0000 
 *     
 *
 *	  e0 (ucs2)
 *    0000 0000  11 1000   00 0000  
 *
 *---------------------------------------------------
 * ucs2              : UTF8
 * 	u16					1 Bytes 0xxxxxxx 
 * 	u16					2 Bytes 110xxxxx 10xxxxxx 
 *	u16				  3 Bytes 1110xxxx 10xxxxxx 10xxxxxx 
 */


/*
 * Convert UTF8-coded characters into Unicode2-coded. 
 *
 * @Description:
 *		
 *
 * @Params
 * 		dst : A pointer to indicate the memmory address which was used to store Unicode2-coded characters. 
 * 	This functions never check args(dst, src), after Convertion dst ends with
 * 	zero.
 * 		src : A pointer to indicate the memmory address which will be converted.
 *	Before invoke this functions, you should assure that two pointer is valid
 *	and you have adequte memmory ,that is, the length of dst no less than src.
 *
 * @Return value:
 * 		It Always should be dst.
 *
 *   see also ucs2utf.
 */
ucs2 *utf2ucs (ucs2 *dst, utf8 *src)
{
 unsigned int i = 0;
 unsigned int j = 0;
 unsigned int next = 0;

 while (src[i] != 0)
 {
  ucs2 temp = 0;
  if (src[i] & 0x80 && src[i+1] & 0x80)
  {

   next = 3;
   temp |= ((src[i] & 0xF) << 12);
   temp |= ((src[i+1] & 0x3F) << 6);
   temp |= ((src[i+2] & 0x3F) << 0);

  }
  else if(src[i] & 0x80 && src[i+1] & 0xC0)
  {
   next = 2;

   temp |= (src[i] & 0x1F) << 6;
   temp |= (src[i+1] & 0x3F) << 0;

  }
  else
  {
   next = 1;
   temp = src[i];
  }
  dst[j++] = temp;
  i += next;
 }
 dst[j] = 0;
 return dst;
};


/*
 *    
 * Convert Unicode2-coded characters into UTF8-coded. 
 *
 * see also utf2ucs.
 *
 */
utf8 *ucs2utf (utf8 *dst, ucs2 *src)
{
 unsigned int i = 0;
 unsigned int j = 0;
 unsigned int next = 0;

 while (src[i] != 0)
 {
  if (src[i] < 0x80)
  { 
   next = 1;
   dst[j] = 0;
   dst[j] = src[i];
  }
  else if(src[i] < 0x800)
  {
   next = 2;
   dst[j] = 0;
   dst[j+1] = 0;

   dst[j+1] = (utf8)((src[i] & 0x3F) | 0x80);

   dst[j] = (utf8)(((src[i] & 0x3F) & 0x1F) | 0xC0);
  }
  else
  {
   next = 3;
   dst[j] = 0;
   dst[j+1] = 0;
   dst[j+2] = 0;

   dst[j] |= ((((utf8)(src[i] >> 12)) & 0xF) | 0xE0);


   dst[j+1] |= (((utf8)(src[i] >> 6) & 0x3F) | 0x80);


   dst[j+2] |= (((utf8)(src[i] >> 0) & 0x3F) | 0x80);

  }

  j += next;
  i++;
 }
 dst[j] = 0;
 return dst;
}

/****************************************************/
//test code.
int main(int argc,const char *argv[])
{
 utf8 str[1024];
 ucs2 dst[512];
 int i = 0;
 int j = 0;
 
 sprintf(str,"%s",argv[1]);


 utf2ucs(dst, str);
 ucs2utf(str, dst);


 printf("%s\n", str);

 return 0;
}

