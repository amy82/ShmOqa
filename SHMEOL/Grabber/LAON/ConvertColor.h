/**************************************************************************************************
*	Project : Eagle Eye Project (Gigabit Ethernet Camera)
*
*
*	
*	File Name : ConvertColor.h 
*	author : hkkim
**************************************************************************************************/
#ifndef __CONVERTCOLOR_H__
#define __CONVERTCOLOR_H__

#include <cv.h>
#include <highgui.h>

/**************************************************************************************************
*   ConvertColor : Display �ϱ� ���� �Է� ������ RGB�� ��ȯ �Ѵ�  
*
*   @param      iPixelFormat, [in] Pixel Format
*   @param      pSrc, [in] ��ȯ�� Source Image (OpenCV�� IplImage*)
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image  
*   @param      pTemp, [in] 10 bit �̻��� ��� ����� destination 
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
void ConvertColor(unsigned int iPixelFormat, IplImage* pSrc, IplImage* pDest, IplImage* pTemp, unsigned int nWidth, unsigned int nHeight);

#endif	//__CONVERTCOLOR_H__