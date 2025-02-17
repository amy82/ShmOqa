#pragma once

#include "export.h"
#include <LibACMISCommon\ACMISCommon.h>
#include <LibACMISSoftISPBasic\Include\DemosaicBasic.h>


namespace ACMISSoftISP
{
	ACMISSOFTISP_API void xInterpolation_BayerY(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_BayerY(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_BayerY(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_BayerY(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_BayerY(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_BayerY(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_OpenCV(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_OpenCV_VNG(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_OpenCV_VNG(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_OpenCV_VNG(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_OpenCV_VNG(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_OpenCV_VNG(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_OpenCV_VNG(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_GBTF(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_GBTF(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_GBTF(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_GBTF(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_GBTF(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_GBTF(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_BLLC(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_BLLC(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_BLLC(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_BLLC(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_BLLC(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_BLLC(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_BLLC2(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, double alpha, double beta, double gamma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_BLLC2(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, double alpha, double beta, double gamma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_BLLC2(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, double alpha, double beta, double gamma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_BLLC2(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, double alpha, double beta, double gamma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_BLLC2(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, double alpha, double beta, double gamma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_BLLC2(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, double alpha, double beta, double gamma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_Residual(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_Residual(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_Residual(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_Residual(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_Residual(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_Residual(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_Residual2(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, double sigma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_Residual2(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, double sigma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_Residual2(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, double sigma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_Residual2(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, double sigma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_Residual2(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, double sigma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_Residual2(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, double sigma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_RI_RI(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_RI_RI(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_RI_RI(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_RI_RI(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_RI_RI(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_RI_RI(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_RI_RI2(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, double sigma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_RI_RI2(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, double sigma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_RI_RI2(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, double sigma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_RI_RI2(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, double sigma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_RI_RI2(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, double sigma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_RI_RI2(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, double sigma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_Gradient_RI(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_Gradient_RI(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_Gradient_RI(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_Gradient_RI(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_Gradient_RI(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_Gradient_RI(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_GBTF_RI(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_GBTF_RI(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_GBTF_RI(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_GBTF_RI(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_GBTF_RI(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_GBTF_RI(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_BL33_RI(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_BL33_RI(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_BL33_RI(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_BL33_RI(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_BL33_RI(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_BL33_RI(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_RI_G_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_RI_G_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_RI_G_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_RI_G_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_RI_G_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_RI_G_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_RI_G_Only2(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, double sigma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_RI_G_Only2(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, double sigma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_RI_G_Only2(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, double sigma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_RI_G_Only2(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, double sigma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_RI_G_Only2(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, double sigma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_RI_G_Only2(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, double sigma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_Gradient_G_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_Gradient_G_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_Gradient_G_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_Gradient_G_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_Gradient_G_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_Gradient_G_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_GBTF_G_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_GBTF_G_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_GBTF_G_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_GBTF_G_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_GBTF_G_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_GBTF_G_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_BL33_G_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_BL33_G_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_BL33_G_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_BL33_G_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_BL33_G_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_BL33_G_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_RI_IR_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_RI_IR_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_RI_IR_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_RI_IR_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_RI_IR_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_RI_IR_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_RI_IR_Only2(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, double sigma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_RI_IR_Only2(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, double sigma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_RI_IR_Only2(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, double sigma, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_RI_IR_Only2(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, double sigma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_RI_IR_Only2(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, double sigma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_RI_IR_Only2(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, double sigma, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_Gradient_IR_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_Gradient_IR_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_Gradient_IR_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_Gradient_IR_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_Gradient_IR_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_Gradient_IR_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_GBTF_IR_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_GBTF_IR_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_GBTF_IR_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_GBTF_IR_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_GBTF_IR_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_GBTF_IR_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation_RGBIR_P2_BL33_IR_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation2Byte_RGBIR_P2_BL33_IR_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolation4Byte_RGBIR_P2_BL33_IR_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI_RGBIR_P2_BL33_IR_Only(EOUTMODE eOutMode, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI2Byte_RGBIR_P2_BL33_IR_Only(EOUTMODE eOutMode, unsigned short *pRaw, unsigned short *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xInterpolationROI4Byte_RGBIR_P2_BL33_IR_Only(EOUTMODE eOutMode, int *pRaw, int *pBMP, int nWidth, int nHeight, RECT *pROI, int nROICount, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
};



