#include "LDPY_DMA.h"

  extern CCD_Info CCD1_info;
  extern CCD_Info CCD2_info;
  extern CCD_Info CCD3_info;
  
  extern Road_Condition Road_condition;
  extern Road_Condition Road_condition_Item;
  
  extern Gyro_Info Gyro_info;

  DMA_Required_Variable DMA_Variable_1 = {0};
  DMA_InitTypeDef dma_init_struct = {0};
  
/*************************************************************************
*				��	��	ƫ	��   
*  ��������: LDPY_DMA_StructInit
*  ����˵��: �ṹ���ʼ��
*  ����˵��:	 ����DMA�ĳ�ʼ������
*            
*  ��������:
*  �޸�ʱ��:2014-05-19
*  ��	ע:
*************************************************************************/

void LDPY_DMA_StructInit(DMA_InitTypeDef  *dma_init_struct,CCD_Info *CCD_info,DMA_Required_Variable *DMA_Required_Variable)
{
	dma_init_struct->DMA_Req=UART5_TRAN_DMAREQ;//ѡ��DMAͨ����Ӧ������Դ������DMA�����
	dma_init_struct->DMA_DestAddr=(uint32_t)&UART5->D;//����Ŀ�����ݵ�ַ
	dma_init_struct->DMA_DestAddrOffset=0;
	dma_init_struct->DMA_DestDataSize=DMA_DST_8BIT;
	dma_init_struct->DMA_Isr=0;//DMA�ж�
	dma_init_struct->DMA_LastDestAddrAdj=0;
	dma_init_struct->DMA_MajorCompleteIntEnable=0;
	dma_init_struct->DMA_MajorHalfCompleteIntEnable=0;
	dma_init_struct->DMA_SourceDataSize=DMA_SRC_8BIT;//����Դ���ݴ�С
	dma_init_struct->DMA_SourceAddrOffset=1;//����Դ���ݵ�ַƫ�ƣ����������ã�Ĭ��Ϊ��
	dma_init_struct->DMA_PeriodicTriggerEnable=FALSE; 
	dma_init_struct->DMA_SourceModulo=0x01;
	/*------------------��ʱ��--------------------*/
	dma_init_struct->DMA_Complete_Flag = FALSE;//��ѭ��δ������ݴ���
}


/*************************************************************************
*				��	��	ƫ	��   
*  ��������: LDPY_DMA_Init
*  ����˵��: ��ʼ��eDMAģ��
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-19
*  ��	ע:
*************************************************************************/
uint8_t LDPY_DMA_init(DMA_InitTypeDef *dma_init_struct)
{
  /*�Խṹ���Ա���ض���*/
  uint8_t chx = dma_init_struct->DMA_CHx;
  uint8_t req = dma_init_struct->DMA_Req;
  uint8_t periodic_trigg=dma_init_struct->DMA_PeriodicTriggerEnable;
  uint16_t major_cnt = dma_init_struct->DMA_MajorLoopCnt;
  uint32_t minor_cnt = dma_init_struct->DMA_MinorByteCnt;
  uint32_t src_addr = dma_init_struct->DMA_SourceAddr;
  uint8_t src_dsize = dma_init_struct->DMA_SourceDataSize;
  int16_t src_addroffset = dma_init_struct->DMA_SourceAddrOffset;
  int32_t src_lastadj = dma_init_struct->DMA_LastSourceAddrAdj;
  uint32_t dst_addr = dma_init_struct->DMA_DestAddr;
  uint8_t dst_dsize = dma_init_struct->DMA_DestDataSize;
  int16_t dst_addroffset = dma_init_struct->DMA_DestAddrOffset;
  int32_t dst_lastadj = dma_init_struct->DMA_LastDestAddrAdj;
  uint8_t auto_disable=dma_init_struct->DMA_AutoDisableReq;
	
	SIM->SCGC6|=SIM_SCGC6_DMAMUX_MASK;//��DMAͨ����·��������ʱ��
	SIM->SCGC7|=SIM_SCGC7_DMA_MASK;//��DMAģ��ʱ��
	
  /*�ȹر�ͨ��X������DMA����*/	
	DMA0->ERQ &= ~(1<<chx);

	/*ѡ��ͨ��X����������DMAԴ������*/
	if(req>15)
	{
	  return 0;
	}
	else 
	{
    DMAMUX->CHCFG[chx] |= DMAMUX_CHCFG_SOURCE(req);
	}
	
	/*�Ƿ�ʹ�����ڴ�������*/
	if(periodic_trigg == TRUE)
  {
    DMAMUX->CHCFG[chx] |= DMAMUX_CHCFG_TRIG_MASK;//���ڴ���ģʽ
  }
  else
  {
    DMAMUX->CHCFG[chx] &= ~(DMAMUX_CHCFG_TRIG_MASK);//����ģʽ
  }
	
	/*����DMAͨ���жϽ�ֹ*/
	DMA0->EEI = 0; 
	
  // ����Դ��ַ 
  DMA0->TCD[chx].SADDR = DMA_SADDR_SADDR(src_addr);
  //Դ��ַ�Ƿ�����/���ٵ�ַƫ����
  DMA0->TCD[chx].SOFF = DMA_SOFF_SOFF(src_addroffset);
  //Դ��ַģʽʹ��
  DMA0->TCD[chx].ATTR = 0|DMA_ATTR_SSIZE(src_dsize);
  //���ļ�������(major iteration count)�����,�Ƿ����µ���Դ��ַ
  DMA0->TCD[chx].SLAST = DMA_SLAST_SLAST(src_lastadj);
  
	  //����Ŀ�ĵ�ַ
	  DMA0->TCD[chx].DADDR = DMA_DADDR_DADDR(dst_addr);
	  //Ŀ�ĵ�ַ�Ƿ�����/���ٵ�ַƫ����
	  DMA0->TCD[chx].DOFF = DMA_DOFF_DOFF(dst_addroffset);
	  //����Ŀ�ĵ�ַ�Ĵ�����
	  DMA0->TCD[chx].ATTR = 0|DMA_ATTR_DSIZE(dst_dsize);
	  //���ļ�������(major iteration count)�����,���µ���Դ��ַ
	  DMA0->TCD[chx].DLAST_SGA = DMA_DLAST_SGA_DLASTSGA(dst_lastadj);
	  

	  if( 1 == 1)
	  {
		//������ѭ�������� current major loop count��ѭ������
		DMA0->TCD[chx].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(major_cnt);
		//��ʼѭ��������������ѭ����������СΪ���ʱ�����¼�����ʼѭ����������ֵ
		DMA0->TCD[chx].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(major_cnt);
	  }
	  
	  if( 1 == 1)
	  {
		//����ÿһ�δ����ֽڵĸ����������ﵽ���޺�DMA�㽫���ݴ����ڴ�
		DMA0->TCD[chx].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(minor_cnt);
	  }
	 
    DMA0->TCD[chx].CSR = 0;  //���CSR������ 
	
	DMA0->TCD[chx].CSR &= ~DMA_CSR_INTMAJOR_MASK;//����DMA��ѭ������������0ʱ�����ж�

    /*--------------------��ʱ��-----------------------*/
    DMA0->TCD[chx].CSR |= DMA_CSR_DREQ_MASK;//ͨ����ERQλ��ÿ�δ�����ɺ�����
	DMA0->TCD[chx].ATTR=0;//8λ���ͣ��ر�ģ����
    
  if(auto_disable == TRUE)
  {
     DMA0->TCD[chx].CSR |= DMA_CSR_DREQ_MASK; //��ѭ��������������󣬹ر�DMA
  }
  else
  {
     DMA0->TCD[chx].CSR &= ~(DMA_CSR_DREQ_MASK); //��ѭ��������������󣬲��ر�DMA
  } 
	
  //DMAͨ��ʹ��
  DMAMUX->CHCFG[chx] |= DMAMUX_CHCFG_ENBL_MASK;
  return 1;		
}

/*************************************************************************
*				��	��	ƫ	��   
*  ��������: LDPY_DMA_EnableIrq
*  ����˵��: ��ʼ��������
*  ����˵��:	 ��DMA����֮ǰ�������м��롮֡ͷ��
*            
*  ��������:
*  �޸�ʱ��:2014-05-19
*  ��	ע:
*************************************************************************/

uint8_t LDPY_DMA_EnableIrq(DMA_InitTypeDef dma_init_struct)
{
  ENABLE_IRQ((IRQn_Type)(dma_init_struct.DMA_CHx + DMA0_IRQn));  
  return 1;
}

/**********************************************************
**LDPY_DMA_DisableIrq
**����eDMA�ж�
**********************************************************/
uint8_t LDPY_DMA_DisableIrq(DMA_InitTypeDef dma_init_struct)
{
  Un_ENABLE_IRQ((IRQn_Type)(dma_init_struct.DMA_CHx + DMA0_IRQn));
  return 1;
}


/*DMA�������������ʼ*/
__INLINE void LDPY_DMA_SoftwareStartService(DMA_InitTypeDef dma_init_struct)
{
  DMA0->TCD[dma_init_struct.DMA_CHx].CSR |= DMA_CSR_START_MASK; 
}

/*************************************************************************
*				��	��	ƫ	��   
*  ��������: FrameHeader_Config
*  ����˵��: DMA֡ͷ����
*  ����˵��:	 ��DMA����֮ǰ�������м��롮֡ͷ��
*            
*  ��������: ��
*  �޸�ʱ��: 2014-05-19
*  ��	ע:
*************************************************************************/
uint8_t FrameHeader_Config(DMA_Required_Variable *DMA_Variable_1)
{
    DMA_Variable_1->CCD_Image[0] = 'C';//CCD1��ͼ��,1~128
	DMA_Variable_1->CCD_Image[129] = 'c';//CCD2��ͼ��130~257		
	DMA_Variable_1->CCD_Image[258] = 'R';//259~263,�����ұ߽�㼰��·ƫ��
	DMA_Variable_1->CCD_Image[264] = 'r';//265~269,�����ұ߽�㼰��·ƫ��
	
	DMA_Variable_1->CCD_Image[270] = 'S';//271~280,�����ز���
	DMA_Variable_1->CCD_Image[283] = 'F';//�����ǲɼ��ĵ�ADֵ
	DMA_Variable_1->CCD_Image[288] = 'M';//289~296,�����ز���
	DMA_Variable_1->CCD_Image[295] = 'w';//289~289,·����Ϣ
	DMA_Variable_1->CCD_Image[297] = 'O';//290~290
	DMA_Variable_1->CCD_Image[298] = 'o';//291~291,O,o������������	
		
	#ifdef DMA_CH0_Enable//����3��CCD��ԭʼͼ���ƫ��
	
		DMA_Variable_1->ThreeCCDs_Image[0] = 'C';//CCD1ͼ��
		DMA_Variable_1->ThreeCCDs_Image[129] = 'c';//CCD2ͼ��
		DMA_Variable_1->ThreeCCDs_Image[258] = 'W';//CCD3ͼ��
		
		DMA_Variable_1->ThreeCCDs_Image[387] = 'R';//CCD1�߽���ƫ��
		DMA_Variable_1->ThreeCCDs_Image[396] = 'r';//CCD1�߽���ƫ��
		DMA_Variable_1->ThreeCCDs_Image[405] = 'I';//CCD1�߽���ƫ��
		
		//Ԥ���ռ�
		DMA_Variable_1->ThreeCCDs_Image[392] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[393] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[394] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[395] = 'x';
		
		DMA_Variable_1->ThreeCCDs_Image[401] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[402] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[403] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[404] = 'x';
		
		DMA_Variable_1->ThreeCCDs_Image[410] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[411] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[412] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[413] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[414] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[415] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[416] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[417] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[418] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[419] = 'x';

	#endif
	
	//����CCD��ѹ��ͼ��֡ͷ
	#ifdef DMA_CH2_SendImage_Enable
		#ifndef DMA_CH2_SendLabVIEW_Info_Enable
		
			DMA_Variable_1->CCD_Image[302] = 'G';//293~308,CCD1ͼ���ѹ������
			DMA_Variable_1->CCD_Image[319] = 'g';//310~325,CCD2ͼ���ѹ������
			DMA_Variable_1->CCD_Image[336] = 'H';//327~342,CCD3ͼ���ѹ������
	
			//����ΪԤ���ռ�
			DMA_Variable_1->CCD_Image[353] = 'x';
			DMA_Variable_1->CCD_Image[354] = 'x';
		
		#endif
	#endif
	
	//һЩ�ط���Ҫ���͵���ʱ����
	#ifdef DMA_CH2_SendLabVIEW_Info_Enable
		#ifndef DMA_CH2_SendImage_Enable
		
		//����ΪԤ���ռ�
		DMA_Variable_1->CCD_Image[301] = 'M';
		DMA_Variable_1->CCD_Image[308] = 'h';
		DMA_Variable_1->CCD_Image[319] = 'd';//ת������ʱ����֡ͷ
		
		//����ΪԤ���ռ�
		DMA_Variable_1->CCD_Image[331] = 'x';
		DMA_Variable_1->CCD_Image[332] = 'x';
		DMA_Variable_1->CCD_Image[333] = 'x';
		DMA_Variable_1->CCD_Image[334] = 'x';
		DMA_Variable_1->CCD_Image[335] = 'x';
		DMA_Variable_1->CCD_Image[336] = 'x';
		DMA_Variable_1->CCD_Image[337] = 'x';
		
		DMA_Variable_1->CCD_Image[338] = 'm';//�����ʱ����֡ͷ
		
		DMA_Variable_1->CCD_Image[350] = 'w';//·����Ϣ
		
		//����ΪԤ���ռ�
		DMA_Variable_1->CCD_Image[351] = 'x';
		DMA_Variable_1->CCD_Image[352] = 'x';
		DMA_Variable_1->CCD_Image[353] = 'x';
		DMA_Variable_1->CCD_Image[354] = 'x';
		
		#endif
	#endif
	
	DMA_Variable_1->CCD_Image[286] = 'x';
	DMA_Variable_1->CCD_Image[287] = 'x';
	
	DMA_Variable_1->CCD_Image[355] = 'O';
	DMA_Variable_1->CCD_Image[356] = 'o';

	DMA_Variable_1->CCD_Image[357] = 'x';
	
	return 1;
}

/*************************************************************************
*				��	��	ƫ	��  
*
*  �������ƣ�LDPY_DMA_SendThreeCCDsImage
*  ����˵��������3��CCD��ԭʼͼ���ƫ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-07-25
*  ��    ע��
*************************************************************************/
#ifdef DMA_CH0_Enable

void LDPY_DMA_SendThreeCCDsImage(DMA_Required_Variable *DMA_Variable,CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info)
{
	uint8_t ii = 0;
	
	//---------------------------------CCDͼ��------------------------------//	
	for(ii = 0;ii < 128; ii++)
	{
		DMA_Variable->ThreeCCDs_Image[ii + 1] = (uint8)(CCD1_info->Pixel[ii] >> 4);
		DMA_Variable->ThreeCCDs_Image[ii + 130] = (uint8)(CCD2_info->Pixel[ii] >> 4);
		DMA_Variable->ThreeCCDs_Image[ii + 259] = (uint8)(CCD3_info->Pixel[ii] >> 4);
	}

	//-------------------------------֡ͷR------------------------------------------//
	DMA_Variable->ThreeCCDs_Image[388] = (uint8_t)(CCD1_info->LeftLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[389] = (uint8_t)(CCD1_info->RightLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[390] = (uint8_t)(CCD1_info->CentralLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[391] = (uint8_t)(CCD1_info->LineError[0] + 64);
	
	//--------------------------------֡ͷr------------------------------------------------//
	DMA_Variable->ThreeCCDs_Image[397] = (uint8_t)(CCD2_info->LeftLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[398] = (uint8_t)(CCD2_info->RightLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[399] = (uint8_t)(CCD2_info->CentralLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[400] = (uint8_t)(CCD2_info->LineError[0] + 64);
	
	//--------------------------------֡ͷI------------------------------------------------//
	DMA_Variable->ThreeCCDs_Image[406] = (uint8_t)(CCD3_info->LeftLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[407] = (uint8_t)(CCD3_info->RightLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[408] = (uint8_t)(CCD3_info->CentralLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[409] = (uint8_t)(CCD3_info->LineError[0] + 64);

}	

#endif

/*************************************************************************
*				��	��	ƫ	��  
*
*  �������ƣ�LDPY_DMA_SendTempVar
*  ����˵��������һЩ��ʱ��������λ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-07-24
*  ��    ע��
*************************************************************************/
#ifdef DMA_CH2_SendLabVIEW_Info_Enable

void LDPY_DMA_SendTempVar(	LabVIEW_Info *LabVIEW_info,
							DMA_Required_Variable *DMA_Variable,
							CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							CCD_Info *CCD3_info,
							HeadSteer_Info *HeadSteer_info,
							Steer_Info *Steer_info,
							Motor_Info *Motor_info,
							Speed_Info *Speed_info
						 )
{
	uint16_t LineError = 0;
	uint16_t HeadSteer_KP_Item = (uint16_t)(10000 + 10*LabVIEW_info->HeadSteer_KP_Item);
	uint16_t HeadSteer_KI_Item = (uint16_t)(10000 + 10*LabVIEW_info->HeadSteer_KI_Item);
	uint16_t Steer_Head_Item = (uint16_t)(10000 + 10*LabVIEW_info->Steer_Head_Item);
	uint16_t Steer_Err_Item = (uint16_t)(10000 + 10*LabVIEW_info->Steer_Err_Item);
	
	uint16_t Motor_KP_Item = (uint16_t)(10000 + 10*LabVIEW_info->Motor_KP_Item);
	uint16_t Motor_KI_Item = (uint16_t)(10000 + 10*LabVIEW_info->Motor_KI_Item);
	uint16_t Motor_KD_Item = (uint16_t)(10000 + 10*LabVIEW_info->Motor_KD_Item);
	
	switch(Speed_info->SpeedMode)
	{
		case 1:
			{
				LineError = (uint16_t)(10000 + 10*CCD1_info->LineError[0]);
			}
			break;
		case 2:
			{
				LineError = (uint16_t)(10000 + 10*CCD2_info->LineError[0]);
			}
			break;
		case 3:
			{
				LineError = (uint16_t)(10000 + 10*CCD2_info->LineError[0]);
			}
			break;
		default:
			break;
	}
	
	/*-------------------------------֡ͷ'M'----------------7-----------------*/
	DMA_Variable->CCD_Image[302] = (uint8_t)(((uint16)(Speed_info->Speed_Now + 1000))&0x00ff);
	DMA_Variable->CCD_Image[303] = (uint8_t)((((uint16)(Speed_info->Speed_Now + 1000))>>8)&0x00ff);
	DMA_Variable->CCD_Image[304] = (uint8_t)(((uint16)(Speed_info->TargetSpeed[0] + 1000))&0x00ff);
	DMA_Variable->CCD_Image[305] = (uint8_t)((((uint16)(Speed_info->TargetSpeed[0] + 1000))>>8)&0x00ff);
	DMA_Variable->CCD_Image[306] = (uint8_t)(((uint16)(Motor_info->PWMOutputDuty + 10000))&0x00ff);
	DMA_Variable->CCD_Image[307] = (uint8_t)((((uint16)(Motor_info->PWMOutputDuty + 10000))>>8)&0x00ff);
	
	//----------------------------֡ͷ��h��---------11--------------//
	DMA_Variable->CCD_Image[309] = (uint8_t)(LineError & 0x00ff);
	DMA_Variable->CCD_Image[310] = (uint8_t)((LineError >> 8) & 0x00ff);
	
	DMA_Variable->CCD_Image[311] = (uint8_t)(HeadSteer_KP_Item & 0x00ff);
	DMA_Variable->CCD_Image[312] = (uint8_t)((HeadSteer_KP_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[313] = 10;			//�Ŵ���Ϊ10
	
	DMA_Variable->CCD_Image[314] = (uint8_t)(HeadSteer_KI_Item & 0x00ff);
	DMA_Variable->CCD_Image[315] = (uint8_t)((HeadSteer_KI_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[316] = 10;			//�Ŵ���Ϊ10
	
	DMA_Variable->CCD_Image[317] = (uint8_t)((uint16_t)HeadSteer_info->Steer_PWMOutputDuty & 0x00ff);
	DMA_Variable->CCD_Image[318] = (uint8_t)(((uint16_t)HeadSteer_info->Steer_PWMOutputDuty >> 8) & 0x00ff);
	
	//----------------------------֡ͷ��d��--------------12---------//
	DMA_Variable->CCD_Image[320] = (uint8_t)(Speed_info->SpeedMode);//���͵�ǰ��λ
	
	DMA_Variable->CCD_Image[321] = (uint8_t)(LineError & 0x00ff);
	DMA_Variable->CCD_Image[322] = (uint8_t)((LineError >> 8) & 0x00ff);
	
	DMA_Variable->CCD_Image[323] = (uint8_t)(Steer_Head_Item & 0x00ff);
	DMA_Variable->CCD_Image[324] = (uint8_t)((Steer_Head_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[325] = 10;			//�Ŵ���Ϊ10
	
	DMA_Variable->CCD_Image[326] = (uint8_t)(Steer_Err_Item & 0x00ff);
	DMA_Variable->CCD_Image[327] = (uint8_t)((Steer_Err_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[328] = 10;			//�Ŵ���Ϊ10
	
	DMA_Variable->CCD_Image[329] = (uint8_t)((uint16_t)Steer_info->Steer_PWMOutputDuty & 0x00ff);
	DMA_Variable->CCD_Image[330] = (uint8_t)(((uint16_t)Steer_info->Steer_PWMOutputDuty >> 8) & 0x00ff);
	
	//----------------------------֡ͷ��m��----------12-------------//
	DMA_Variable->CCD_Image[339] = (uint8_t)(Motor_KP_Item & 0x00ff);
	DMA_Variable->CCD_Image[340] = (uint8_t)((Motor_KP_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[341] = 10;			//�Ŵ���Ϊ10
	
	DMA_Variable->CCD_Image[342] = (uint8_t)(Motor_KI_Item & 0x00ff);
	DMA_Variable->CCD_Image[343] = (uint8_t)((Motor_KI_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[344] = 10;			//�Ŵ���Ϊ10
	
	DMA_Variable->CCD_Image[345] = (uint8_t)(Motor_KD_Item & 0x00ff);
	DMA_Variable->CCD_Image[346] = (uint8_t)((Motor_KD_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[347] = 10;			//�Ŵ���Ϊ10
	
	DMA_Variable->CCD_Image[348] = (uint8_t)((uint16_t)(10000 + Motor_info->PWMOutputDuty) & 0x00ff);
	DMA_Variable->CCD_Image[349] = (uint8_t)(((uint16_t)(10000 + Motor_info->PWMOutputDuty) >> 8) & 0x00ff);
	
	/*-------------------------------֡ͷ'w'----------------2-----------------*/
	DMA_Variable->CCD_Image[351] = (uint8_t)(Road_condition_Item);
}

#endif

/*************************************************************************
*				��	��	ƫ	��  
*
*  �������ƣ�LDPY_DMA_UpdateFifo_Parameters
*  ����˵��������CCD��������ݸ���λ��,�����ٶȣ������������������������Ͳ���
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-05-24
*  ��    ע��
*************************************************************************/

void LDPY_DMA_UpdateFifo_Parameters(CCD_Info *CCD1_info,Steer_Info *Steer_info,\
									Motor_Info *Motor_info,Speed_Info *Speed_info,\
									Road_Condition *Road_condition)
{
		/*--------------------------֡ͷ'S'-----------------------------------*/
		DMA_Variable_1.CCD_Image[271] = (uint8_t)(((uint16)Steer_info->Steer_PWMOutputDuty)&0xff);
		DMA_Variable_1.CCD_Image[272] = (uint8_t)((((uint16)Steer_info->Steer_PWMOutputDuty)>>8)&0x00ff);
		DMA_Variable_1.CCD_Image[273] = (uint8_t)(((uint16)(CCD1_info->LineError[0] + 1000))&0x00ff);
		DMA_Variable_1.CCD_Image[274] = (uint8_t)((((uint16)(CCD1_info->LineError[0] + 1000))>>8)&0x00ff);
	
	    DMA_Variable_1.CCD_Image[275] = (uint8_t)(((uint16)CCD1_info->LeftLine[0] + 1000)&0xff);
		DMA_Variable_1.CCD_Image[276] = (uint8_t)((((uint16)CCD1_info->LeftLine[0] + 1000)>>8)&0x00ff);
	    DMA_Variable_1.CCD_Image[277] = (uint8_t)(((uint16)CCD1_info->CentralLine[0] + 1000)&0xff);
		DMA_Variable_1.CCD_Image[278] = (uint8_t)((((uint16)CCD1_info->CentralLine[0] + 1000)>>8)&0x00ff);	
	    DMA_Variable_1.CCD_Image[279] = (uint8_t)(((uint16)CCD1_info->RightLine[0] + 1000)&0xff);
		DMA_Variable_1.CCD_Image[280] = (uint8_t)((((uint16)CCD1_info->RightLine[0] + 1000)>>8)&0x00ff);	
	
		DMA_Variable_1.CCD_Image[281] = 'x';
		DMA_Variable_1.CCD_Image[282] = 'x';
	
		/*--------------------------֡ͷ'F'-----------------------------------*/
		DMA_Variable_1.CCD_Image[284] = (uint8_t)((Gyro_info.Gyroscope_Fifo[0])&0xff);
		DMA_Variable_1.CCD_Image[285] = (uint8_t)(((Gyro_info.Gyroscope_Fifo[0])>>8)&0x00ff);
	
		/*-------------------------------֡ͷ'M'---------------------------------*/
		DMA_Variable_1.CCD_Image[289] = (uint8_t)(((uint16)(Speed_info->Speed_Now + 1000))&0x00ff);
		DMA_Variable_1.CCD_Image[290] = (uint8_t)((((uint16)(Speed_info->Speed_Now + 1000))>>8)&0x00ff);
		DMA_Variable_1.CCD_Image[291] = (uint8_t)(((uint16)(Speed_info->TargetSpeed[0] + 1000))&0x00ff);
		DMA_Variable_1.CCD_Image[292] = (uint8_t)((((uint16)(Speed_info->TargetSpeed[0] + 1000))>>8)&0x00ff);
		DMA_Variable_1.CCD_Image[293] = (uint8_t)(((uint16)(Motor_info->PWMOutputDuty + 10000))&0x00ff);
		DMA_Variable_1.CCD_Image[294] = (uint8_t)((((uint16)(Motor_info->PWMOutputDuty + 10000))>>8)&0x00ff);
		
		/*-----------------------------֡ͷ'w'-------------------------------------*/
		DMA_Variable_1.CCD_Image[296] = *Road_condition;
}

/*************************************************************************
*				��	��	ƫ	��  
*
*  �������ƣ�LDPY_DMA_UpdateFifo_PhotoValue
*  ����˵������������CCDѹ�����ͼ������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-08
*  ��    ע��
*************************************************************************/

void LDPY_DMA_UpdateFifo_PhotoValue(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info)
{
	uint8 ii = 0;
	
	//ѹ��CCD��ֵ���������
	for(ii = 0;ii < 16;ii ++)
	{
		CCD1_info->CCD_PhotoValue[ii] = 	(CCD1_info->PixelBinary[7 + 8*ii] << 7)
										  | (CCD1_info->PixelBinary[6 + 8*ii] << 6)
										  | (CCD1_info->PixelBinary[5 + 8*ii] << 5)
										  | (CCD1_info->PixelBinary[4 + 8*ii] << 4)
										  | (CCD1_info->PixelBinary[3 + 8*ii] << 3)
										  | (CCD1_info->PixelBinary[2 + 8*ii] << 2)
										  | (CCD1_info->PixelBinary[1 + 8*ii] << 1)
										  | (CCD1_info->PixelBinary[0 + 8*ii])
										;
	
		CCD2_info->CCD_PhotoValue[ii] = 	(CCD2_info->PixelBinary[7 + 8*ii] << 7)
										  | (CCD2_info->PixelBinary[6 + 8*ii] << 6)
										  | (CCD2_info->PixelBinary[5 + 8*ii] << 5)
										  | (CCD2_info->PixelBinary[4 + 8*ii] << 4)
										  | (CCD2_info->PixelBinary[3 + 8*ii] << 3)
										  | (CCD2_info->PixelBinary[2 + 8*ii] << 2)
										  | (CCD2_info->PixelBinary[1 + 8*ii] << 1)
										  | (CCD2_info->PixelBinary[0 + 8*ii])
										;
		
		CCD3_info->CCD_PhotoValue[ii] = 	(CCD3_info->PixelBinary[7 + 8*ii] << 7)
										  | (CCD3_info->PixelBinary[6 + 8*ii] << 6)
										  | (CCD3_info->PixelBinary[5 + 8*ii] << 5)
										  | (CCD3_info->PixelBinary[4 + 8*ii] << 4)
										  | (CCD3_info->PixelBinary[3 + 8*ii] << 3)
										  | (CCD3_info->PixelBinary[2 + 8*ii] << 2)
										  | (CCD3_info->PixelBinary[1 + 8*ii] << 1)
										  | (CCD3_info->PixelBinary[0 + 8*ii])
										;
	}
	
	/*---------------֡ͷ��G��---------------------*/
	DMA_Variable_1.CCD_Image[303] = CCD1_info->CCD_PhotoValue[0];
	DMA_Variable_1.CCD_Image[304] = CCD1_info->CCD_PhotoValue[1];
	DMA_Variable_1.CCD_Image[305] = CCD1_info->CCD_PhotoValue[2];
	DMA_Variable_1.CCD_Image[306] = CCD1_info->CCD_PhotoValue[3];
	DMA_Variable_1.CCD_Image[307] = CCD1_info->CCD_PhotoValue[4];
	DMA_Variable_1.CCD_Image[308] = CCD1_info->CCD_PhotoValue[5];
	DMA_Variable_1.CCD_Image[309] = CCD1_info->CCD_PhotoValue[6];
	DMA_Variable_1.CCD_Image[310] = CCD1_info->CCD_PhotoValue[7];
	DMA_Variable_1.CCD_Image[311] = CCD1_info->CCD_PhotoValue[8];
	DMA_Variable_1.CCD_Image[312] = CCD1_info->CCD_PhotoValue[9];
	DMA_Variable_1.CCD_Image[313] = CCD1_info->CCD_PhotoValue[10];
	DMA_Variable_1.CCD_Image[314] = CCD1_info->CCD_PhotoValue[11];
	DMA_Variable_1.CCD_Image[315] = CCD1_info->CCD_PhotoValue[12];
	DMA_Variable_1.CCD_Image[316] = CCD1_info->CCD_PhotoValue[13];
	DMA_Variable_1.CCD_Image[317] = CCD1_info->CCD_PhotoValue[14];
	DMA_Variable_1.CCD_Image[318] = CCD1_info->CCD_PhotoValue[15];	
	
	/*---------------֡ͷ��g��---------------------*/
	DMA_Variable_1.CCD_Image[320] = CCD2_info->CCD_PhotoValue[0];
	DMA_Variable_1.CCD_Image[321] = CCD2_info->CCD_PhotoValue[1];
	DMA_Variable_1.CCD_Image[322] = CCD2_info->CCD_PhotoValue[2];
	DMA_Variable_1.CCD_Image[323] = CCD2_info->CCD_PhotoValue[3];
	DMA_Variable_1.CCD_Image[324] = CCD2_info->CCD_PhotoValue[4];
	DMA_Variable_1.CCD_Image[325] = CCD2_info->CCD_PhotoValue[5];
	DMA_Variable_1.CCD_Image[326] = CCD2_info->CCD_PhotoValue[6];
	DMA_Variable_1.CCD_Image[327] = CCD2_info->CCD_PhotoValue[7];
	DMA_Variable_1.CCD_Image[328] = CCD2_info->CCD_PhotoValue[8];
	DMA_Variable_1.CCD_Image[329] = CCD2_info->CCD_PhotoValue[9];
	DMA_Variable_1.CCD_Image[330] = CCD2_info->CCD_PhotoValue[10];
	DMA_Variable_1.CCD_Image[331] = CCD2_info->CCD_PhotoValue[11];
	DMA_Variable_1.CCD_Image[332] = CCD2_info->CCD_PhotoValue[12];
	DMA_Variable_1.CCD_Image[333] = CCD2_info->CCD_PhotoValue[13];
	DMA_Variable_1.CCD_Image[334] = CCD2_info->CCD_PhotoValue[14];
	DMA_Variable_1.CCD_Image[335] = CCD2_info->CCD_PhotoValue[15];
	
	/*---------------֡ͷ��H��---------------------*/
	DMA_Variable_1.CCD_Image[337] = CCD3_info->CCD_PhotoValue[0];
	DMA_Variable_1.CCD_Image[338] = CCD3_info->CCD_PhotoValue[1];
	DMA_Variable_1.CCD_Image[339] = CCD3_info->CCD_PhotoValue[2];
	DMA_Variable_1.CCD_Image[340] = CCD3_info->CCD_PhotoValue[3];
	DMA_Variable_1.CCD_Image[341] = CCD3_info->CCD_PhotoValue[4];
	DMA_Variable_1.CCD_Image[342] = CCD3_info->CCD_PhotoValue[5];
	DMA_Variable_1.CCD_Image[343] = CCD3_info->CCD_PhotoValue[6];
	DMA_Variable_1.CCD_Image[344] = CCD3_info->CCD_PhotoValue[7];
	DMA_Variable_1.CCD_Image[345] = CCD3_info->CCD_PhotoValue[8];
	DMA_Variable_1.CCD_Image[346] = CCD3_info->CCD_PhotoValue[9];
	DMA_Variable_1.CCD_Image[347] = CCD3_info->CCD_PhotoValue[10];
	DMA_Variable_1.CCD_Image[348] = CCD3_info->CCD_PhotoValue[11];
	DMA_Variable_1.CCD_Image[349] = CCD3_info->CCD_PhotoValue[12];
	DMA_Variable_1.CCD_Image[350] = CCD3_info->CCD_PhotoValue[13];
	DMA_Variable_1.CCD_Image[351] = CCD3_info->CCD_PhotoValue[14];
	DMA_Variable_1.CCD_Image[352] = CCD3_info->CCD_PhotoValue[15];
}

/*************************************************************************
*				��	��	ƫ	��  
*
*  �������ƣ�DMA_InitAndChoice
*  ����˵�������ͷ�ʽѡ��
*  ����˵����CH0���Ͳ���,CH1���Ͳ���,CH3����ѹ��ͼ��
*  �������أ�1
*  �޸�ʱ�䣺2014-06-08
*  ��    ע��CH0���Ͳ���,CH1���Ͳ���,CH3����ѹ��ͼ��
*************************************************************************/
uint8_t LDPY_DMA_InitAndChoice(DMA_InitTypeDef  *dma_init_struct,DMA_Required_Variable *DMA_Required_Variable,\
                          CCD_Info *CCD_info)
{
	#ifdef  DMA_CH0_Enable
	
		dma_init_struct->DMA_CHx = DMA_CH0; 	
		dma_init_struct->DMA_SourceAddr = (uint32_t)&DMA_Required_Variable->ThreeCCDs_Image[0];
		dma_init_struct->DMA_LastSourceAddrAdj = -420; 
		dma_init_struct->DMA_MajorLoopCnt = 420;
		dma_init_struct->DMA_MinorByteCnt = 1;//��ѭ�������ֽ���
		dma_init_struct->DMA_BufferSize = 420;

	    /*----------------��ʱ��-------------------*/
	    dma_init_struct->DMA_AutoClose = TRUE;
		dma_init_struct->DMA_AutoDisableReq = TRUE;
	
		FrameHeader_Config(&DMA_Variable_1);
        LDPY_DMA_StructInit(dma_init_struct,CCD_info,DMA_Required_Variable);
        LDPY_DMA_init(dma_init_struct);
	
	#endif
	
	#ifdef	 DMA_CH1_Enable
	
	    dma_init_struct->DMA_CHx = DMA_CH1;
		dma_init_struct->DMA_SourceAddr = (uint32_t)&DMA_Required_Variable->CCD_Image[270];
		dma_init_struct->DMA_LastSourceAddrAdj = -27;
		dma_init_struct->DMA_MajorLoopCnt = 27;
		dma_init_struct->DMA_MinorByteCnt = 1;//��ѭ�������ֽ���
		dma_init_struct->DMA_BufferSize = 27;
		
		/*----------------��ʱ��-------------------*/
		dma_init_struct->DMA_AutoClose = TRUE;//��ѭ�����������ٵ���ʱ�ر�DMA
		dma_init_struct->DMA_AutoDisableReq = TRUE;

		FrameHeader_Config(&DMA_Variable_1);
        LDPY_DMA_StructInit(dma_init_struct,CCD_info,DMA_Required_Variable);
        LDPY_DMA_init(dma_init_struct);
	
	#endif
    
    #ifdef  DMA_CH2_Enable
		
	    dma_init_struct->DMA_CHx = DMA_CH2;

		dma_init_struct->DMA_SourceAddr = (uint32_t)&DMA_Required_Variable->CCD_Image[301];
		dma_init_struct->DMA_LastSourceAddrAdj = -51;
		dma_init_struct->DMA_MajorLoopCnt = 51;
		dma_init_struct->DMA_MinorByteCnt = 1;//��ѭ�������ֽ���
		dma_init_struct->DMA_BufferSize = 51;

		
		/*----------------��ʱ��-------------------*/
		dma_init_struct->DMA_AutoClose = TRUE;//��ѭ�����������ٵ���ʱ�ر�DMA
		dma_init_struct->DMA_AutoDisableReq = TRUE;

		FrameHeader_Config(&DMA_Variable_1);
        LDPY_DMA_StructInit(dma_init_struct,CCD_info,DMA_Required_Variable);
        LDPY_DMA_init(dma_init_struct);
		
	#endif
	
    return 1;
}


