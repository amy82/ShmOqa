//-----------------------------------------------------------------------------
//
//	DIODefine.h
//
//-----------------------------------------------------------------------------
#ifndef __DIODEFINE_H__
#define __DIODEFINE_H__

//#include "GlobalDefine.h"

//ȭ�����

///////////////// DIO Input DEFINE /////////////////
#define		DIO_IN_DOOR_OPEN_1					0x00000001		//!  1)		//�۾��� ���� ����
#define		DIO_IN_DOOR_OPEN_2					0x00000002		//!  2) 	//�۾��� ���� ����
#define		DIO_IN_DOOR_OPEN_3					0x00000004		//!  3)  	//�۾��� ���� ������
#define		DIO_IN_LIGHT_CURTAIN				0x00000008		//!  4)  
#define		DIO_IN_SPARE5						0x00000010		//!  5)
#define		DIO_IN_SPARE6						0x00000020		//!  6)   
#define     DIO_IN_SPARE7						0x00000040		//!  7)   
#define     DIO_IN_SPARE8						0x00000080		//!  8)   
//
#define		DIO_IN_PCB_VACUUM					0x00000100		//!  9)   
#define		DIO_IN_PCB_SOCKET_CHECK				0x00000200		//! 10)   
#define		DIO_IN_START_PB_L					0x00000400		//! 11)   
#define		DIO_IN_START_PB_R					0x00000800		//! 12)   
#define		DIO_IN_STOP_PB						0x00001000		//! 13)   
#define     DIO_IN_SPARE14						0x00002000		//! 14) 
#define     DIO_IN_SPARE15						0x00004000		//! 15) 
#define     DIO_IN_SPARE16						0x00008000		//! 16) 

////-----------------------------------------------------------------------------
#define		DIO_IN_SPARE17						0x00010000		//! 17)   
#define		DIO_IN_SPARE18						0x00020000		//! 18)	  
#define     DIO_IN_SPARE19						0x00040000		//! 19)   
#define     DIO_IN_SPARE20						0x00080000		//! 20)   
#define     DIO_IN_SPARE21						0x00100000		//! 21)	  
#define		DIO_IN_SPARE22						0x00200000		//! 22)	  
#define		DIO_IN_SPARE23						0x00400000		//! 23)   
#define		DIO_IN_SPARE24						0x00800000		//! 24)   
#define     DIO_IN_SPARE25						0x01000000		//! 25)	  
#define		DIO_IN_SPARE26						0x02000000		//! 26)
#define		DIO_IN_SPARE27						0x04000000		//! 27)
#define		DIO_IN_SPARE28						0x08000000		//! 28)
#define		DIO_IN_SPARE29						0x10000000		//! 29)
#define		DIO_IN_SPARE30						0x20000000		//! 30)	  
#define     DIO_IN_SPARE31						0x40000000		//! 31)
#define		DIO_IN_SPARE32						0x80000000		//! 32)
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------

//
//
///////////////// DIO Output DEFINE /////////////////
//ch-1
#define		DIO_OUT_TOWER_LAMP_R				0x00000001		//!  1) 
#define		DIO_OUT_TOWER_LAMP_Y				0x00000002		//!  2) 
#define		DIO_OUT_TOWER_LAMP_G				0x00000004		//!  3) 
#define		DIO_OUT_BUZZER_1 					0x00000008		//!  4) 
#define     DIO_OUT_BUZZER_2					0x00000010		//!  5) 
#define     DIO_OUT_BUZZER_3					0x00000020		//!  6) 
#define     DIO_OUT_BUZZER_4					0x00000040		//!  7) 
#define     DIO_OUT_TOWER_BUZZER				0x00000080		//!  8) 
//ch-2
#define     DIO_OUT_PCB_VACUUM_ON				0x00000001		//!  9) 
#define     DIO_OUT_DOOR_UNLOCK_ALL				0x00000002		//! 10) 
#define		DIO_OUT_START_PB_LED_L				0x00000004		//! 11) 
#define		DIO_OUT_START_PB_LED_R				0x00000008		//! 12) 
#define		DIO_OUT_STOP_PB_LED					0x00000010		//! 13) 
#define		DIO_OUT_PCB_VACUUM_OFF				0x00000020		//! 14) 
#define		DIO_OUT_SPARE15						0x00000040		//! 15) 
#define		DIO_OUT_SPARE16						0x00000080		//! 16) 
//ch-3
#define		DIO_OUT_SPARE17						0x00000001		//! 17)  
#define		DIO_OUT_SPARE18						0x00000002		//! 18)  
#define		DIO_OUT_SPARE19						0x00000004		//! 19)  
#define		DIO_OUT_SPARE20						0x00000008		//! 20)  
#define		DIO_OUT_SPARE21						0x00000010		//! 21) 
#define		DIO_OUT_SPARE22						0x00000020		//! 22)   
#define		DIO_OUT_SPARE23						0x00000040		//! 23)  
#define		DIO_OUT_SPARE24						0x00000080		//! 24)  
//ch-4
#define		DIO_OUT_SPARE25						0x00000001		//! 25) 
#define		DIO_OUT_SPARE26						0x00000002		//! 26) 
#define		DIO_OUT_SPARE27						0x00000004		//! 27) 
#define		DIO_OUT_SPARE28						0x00000008		//! 28) 
#define		DIO_OUT_SPARE29						0x00000010		//! 29) 
#define		DIO_OUT_SPARE30						0x00000020		//! 30) 
#define		DIO_OUT_SPARE31						0x00000040		//! 31) 
#define		DIO_OUT_LG_DOOR						0x00000080		//! 32) 

//---IN
//
#define		EZDIO_IN_LIGHT_CURTAIN				0x00000001		//!  1)		AREA SENSER
#define		EZDIO_IN_PCB_CHECK					0x00000002		//!  2)		��ǰ����
#define		EZDIO_IN_PCB_VACUUM					0x00000004		//!  3)		���� Ȯ��
#define		EZDIO_IN_PCB_BUTTON_VACUUM			0x00000008		//!  4)		��ư LED
#define		EZDIO_IN_SPARE5						0x00000010		//!  5)
#define		EZDIO_IN_SPARE6						0x00000020		//!  6)
#define     EZDIO_IN_SPARE7						0x00000040		//!  7)
#define     EZDIO_IN_SPARE8						0x00000080		//!  8)		
//
#define		EZDIO_IN_START_BUTTON_L				0x00000100		//!  9)		��ŸƮ ��ư led L
#define		EZDIO_IN_START_BUTTON_R				0x00000200		//! 10)		��ŸƮ ��ư led R
#define		EZDIO_IN_SOCKET_Y_FOR				0x00000400		//! 11)		���� ���� ����
#define		EZDIO_IN_SOCKET_Y_BACK				0x00000800		//! 12)		���� ���� ����
#define		EZDIO_IN_OC_Z_UP					0x00001000		//! 13)		�̹����� ��� ����
#define     EZDIO_IN_OC_Z_DOWN					0x00002000		//! 14)		�̹����� �ϰ� ����
#define     EZDIO_IN_SPARE15					0x00004000		//! 15)
#define     EZDIO_IN_SPARE16					0x00008000		//! 16)
//
////
////---OUT
//
#define		EZDIO_OUT_TOWER_LAMP_R				0x00000001		//!  1) TOWER LAMP R
#define		EZDIO_OUT_TOWER_LAMP_Y				0x00000002		//!  2) TOWER LAMP Y
#define		EZDIO_OUT_SPARE3					0x00000004		//!  3)	
#define		EZDIO_OUT_BUZZER					0x00000008		//!  4)	BUZZER
#define		EZDIO_OUT_PCB_BUTTON_VACUUM_LED		0x00000010		//!  5) ������ư LED
#define		EZDIO_OUT_VACUUM_ON					0x00000020		//!  6) ����
#define     EZDIO_OUT_VACUUM_OFF				0x00000040		//!  7) Ż��
#define     EZDIO_OUT_SPARE8					0x00000080		//!  8) 
//
#define		EZDIO_OUT_START_BUTTON_L			0x00000100		//!  9) L ��ŸƮ ��ư PRESS 
#define		EZDIO_OUT_START_BUTTON_R			0x00000200		//! 10) R ��ŸƮ ��ư PRESS 
#define		EZDIO_OUT_SOCKET_Y_FOR				0x00000400		//! 11) ���� ����
#define		EZDIO_OUT_SOCKET_Y_BACK				0x00000800		//! 12) ���� ����
#define		EZDIO_OUT_OC_Z_UP					0x00001000		//! 13)	�̹� ���� ���
#define     EZDIO_OUT_OC_Z_DOWN					0x00002000		//! 14)	�̹� ���� �ϰ�
#define     EZDIO_OUT_TOWER_LAMP_G				0x00004000		//! 15) TOWER LAMP G
#define     EZDIO_OUT_SPARE16					0x00008000		//! 16)
//EZDIO_OUT_SOCKET_FOR



#endif
	

