#include "GeneralDeclare.h"
#include "FLASH.h"
#include "ProjectMath.h"

#define BOOTSECTOR     (FLASH_SECTOR_NUM - 1)

#if defined(MK60DZ10)
#define     FCMD        FTFL_FCCOB0         //FTFL ����

#define     FADDR2      FTFL_FCCOB1         //Flash address [23:16]
#define     FADDR1      FTFL_FCCOB2         //Flash address [15:8]
#define     FADDR0      FTFL_FCCOB3         //Flash address [7:0]

#define     FDATA0      FTFL_FCCOB4         //Data Byte 0       //ע��һ�㣬4�ֽ�����FDATA3�����λ��FDATA0�����λ
#define     FDATA1      FTFL_FCCOB5         //Data Byte 1
#define     FDATA2      FTFL_FCCOB6         //Data Byte 2
#define     FDATA3      FTFL_FCCOB7         //Data Byte 3
#define     FDATA4      FTFL_FCCOB8         //Data Byte 4
#define     FDATA5      FTFL_FCCOB9         //Data Byte 5
#define     FDATA6      FTFL_FCCOBA         //Data Byte 6
#define     FDATA7      FTFL_FCCOBB         //Data Byte 7

//FCMD ����
#define     RD1BLK    0x00   // ������Flash
#define     RD1SEC    0x01   // ����������
#define     PGMCHK    0x02   // д����
#define     RDRSRC    0x03   // ��Ŀ������(4�ֽ�)
#define     PGM4      0x06   // д�볤��(4�ֽ�)
#define     ERSBLK    0x08   // ��������Flash
#define     ERSSCR    0x09   // ����Flash����
#define     PGMSEC    0x0B   // д������
#define     RD1ALL    0x40   // �����еĿ�
#define     RDONCE    0x41   // ֻ��һ��
#define     PGMONCE   0x43   // ֻдһ��
#define     ERSALL    0x44   // �������п�
#define     VFYKEY    0x45   // ��֤���ŷ���Կ��
#define     PGMPART   0x80   // д�����
#define     SETRAM    0x81   // �趨FlexRAM����

#elif defined(MK66F18)
#define     FCMD        FTFE_FCCOB0         //FTFL ����

#define     FADDR2      FTFE_FCCOB1         //Flash address [23:16]
#define     FADDR1      FTFE_FCCOB2         //Flash address [15:8]
#define     FADDR0      FTFE_FCCOB3         //Flash address [7:0]
//Must be 64-bit aligned (Flash address [2:0] = 000)  8�ֽڶ���

#define     FDATA0      FTFE_FCCOB4         //Data Byte 0       //ע��һ�㣬4�ֽ�����FDATA3�����λ��FDATA0�����λ
#define     FDATA1      FTFE_FCCOB5         //Data Byte 1
#define     FDATA2      FTFE_FCCOB6         //Data Byte 2
#define     FDATA3      FTFE_FCCOB7         //Data Byte 3
#define     FDATA4      FTFE_FCCOB8         //Data Byte 4
#define     FDATA5      FTFE_FCCOB9         //Data Byte 5
#define     FDATA6      FTFE_FCCOBA         //Data Byte 6
#define     FDATA7      FTFE_FCCOBB         //Data Byte 7

//FCMD ����
#define     RD1BLK    0x00   // ������Flash
#define     RD1SEC    0x01   // ����������
#define     PGMCHK    0x02   // д����
#define     RDRSRC    0x03   // ��Ŀ������(8�ֽ�)
#define     PGM8      0x07   // д�볤��(8�ֽ�)
#define     ERSBLK    0x08   // ��������Flash
#define     ERSSCR    0x09   // ����Flash����
#define     PGMSEC    0x0B   // д������
#define     RD1ALL    0x40   // �����еĿ�
#define     RDONCE    0x41   // ֻ��һ��
#define     PGMONCE   0x43   // ֻдһ��
#define     ERSALL    0x44   // �������п�
#define     VFYKEY    0x45   // ��֤���ŷ���Կ��
#define     PGMPART   0x80   // д�����
#define     SETRAM    0x81   // �趨FlexRAM����

#endif


FLASH_WRITE_TYPE      FLASH_DATA[FLASHNUM];

__ramfunc uint8 FLASH_CMD()
{
#if defined(MK60DZ10)
    FTFL_FSTAT =    (0
                     |  FTFL_FSTAT_CCIF_MASK        // ָ����ɱ�־(д1��0)
                     |  FTFL_FSTAT_RDCOLERR_MASK    // ����ͻ�����־(д1��0)
                     |  FTFL_FSTAT_ACCERR_MASK      // ���ʴ����־λ(д1��0)
                     |  FTFL_FSTAT_FPVIOL_MASK      // �Ƿ����ʱ�����־λ(д1��0)
                    );
    while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));    // �ȴ��������
    return 'T';

#elif defined(MK66F18)
    FTFE_FSTAT =    (0
                     |  FTFE_FSTAT_CCIF_MASK        // ָ����ɱ�־(д1��0)
                     |  FTFE_FSTAT_RDCOLERR_MASK    // ����ͻ�����־(д1��0)
                     |  FTFE_FSTAT_ACCERR_MASK      // ���ʴ����־λ(д1��0)
                     |  FTFE_FSTAT_FPVIOL_MASK      // �Ƿ����ʱ�����־λ(д1��0)
                    );

    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));    // �ȴ��������

    return 'T';

#endif  
}

__ramfunc void FLASH_init()
{
#if defined( MK60DZ10)
    // ���FlashԤ��ȡ������
    FMC_PFB0CR |= FMC_PFB0CR_S_B_INV_MASK;      //
    FMC_PFB1CR |= FMC_PFB0CR_S_B_INV_MASK;

    while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));    // �ȴ��������

    FTFL_FSTAT =    (0
                     |  FTFL_FSTAT_CCIF_MASK        // ָ����ɱ�־(д1��0)
                     |  FTFL_FSTAT_RDCOLERR_MASK    // ����ͻ�����־(д1��0)
                     |  FTFL_FSTAT_ACCERR_MASK      // ���ʴ����־λ(д1��0)
                     |  FTFL_FSTAT_FPVIOL_MASK      // �Ƿ����ʱ�����־λ(д1��0)
                    );
#elif defined(MK66F18)
    // ���FlashԤ��ȡ������
    FMC_PFB01CR |= FMC_PFB01CR_S_B_INV_MASK;      //

    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));    // �ȴ��������

    FTFE_FSTAT =    (0
                     |  FTFE_FSTAT_CCIF_MASK        // ָ����ɱ�־(д1��0)
                     |  FTFE_FSTAT_RDCOLERR_MASK    // ����ͻ�����־(д1��0)
                     |  FTFE_FSTAT_ACCERR_MASK      // ���ʴ����־λ(д1��0)
                     |  FTFE_FSTAT_FPVIOL_MASK      // �Ƿ����ʱ�����־λ(д1��0)
                    );
#endif
}

__ramfunc uint8 FLASH_ERASE(uint16 No)
{
    uint32 addr = No * FLASH_SECTOR_SIZE;
    // ���ò�������
    FCMD = ERSSCR;
    // ����Ŀ���ַ
    FADDR2 = ((Dtype *)&addr)->B[2];
    FADDR1 = ((Dtype *)&addr)->B[1];
    FADDR0 = ((Dtype *)&addr)->B[0];
    while(FLASH_CMD() != 'T');
/*    
#if defined(MK60DZ10)
        return flash_write(No,0x00040C,0xFFFFFFFE);
#elif defined(MK66F18)
        return flash_write(No,0x000408,0xFFFFFFFEFFFFFFFF );
#endif*/
    return 'T';
}

__ramfunc uint8 FLASH_WRITE(uint16 No, uint16 offset, FLASH_WRITE_TYPE data)
{
    uint32 addr = No * FLASH_SECTOR_SIZE  + offset ;
    uint32 tmpdata;
    // ����Ŀ���ַ
    FADDR2 = ((Dtype *)&addr)->B[2];
    FADDR1 = ((Dtype *)&addr)->B[1];
    FADDR0 = ((Dtype *)&addr)->B[0];
    // ���� ��32λ����
    tmpdata = (uint32)data;
    FDATA0 = ((Dtype *)&tmpdata)->B[3];                    // ����д������
    FDATA1 = ((Dtype *)&tmpdata)->B[2];
    FDATA2 = ((Dtype *)&tmpdata)->B[1];
    FDATA3 = ((Dtype *)&tmpdata)->B[0];  
#if defined(MK60DZ10)
    FCMD = PGM4;
#elif defined(MK66F18)
    // ���ò�������
    FCMD = PGM8;

    // ���� ��32λ����
    tmpdata = (uint32)(data>>32);

    FDATA4 = ((Dtype *)&tmpdata)->B[3];                 // ����д������
    FDATA5 = ((Dtype *)&tmpdata)->B[2];
    FDATA6 = ((Dtype *)&tmpdata)->B[1];
    FDATA7 = ((Dtype *)&tmpdata)->B[0];
#endif
    while(FLASH_CMD() != 'T');
    return 'T';   
}

void FLASH_DELAY(uint32 ms)
{
  while(ms--);
}

uint8 My_FlashWrite(uint16 Boot)
{
    FLASH_ERASE(BOOTSECTOR - Boot);
    Data_Convert_Uint32();
    for(int n = 0; n < FLASHNUM; n++)
    {
       FLASH_WRITE((BOOTSECTOR - Boot), (FLASH_ALIGN_ADDR * n), FLASH_DATA[n]);
       FLASH_DELAY(4000);
    }
    return 'T';
}

uint8 My_FlashRead(uint16 Boot)
{
    for(int n = 0; n < FLASHNUM; n++)
    {
       FLASH_DATA[n] = FLASH_READ((BOOTSECTOR - Boot), (FLASH_ALIGN_ADDR * n), FLASH_WRITE_TYPE);
       FLASH_DELAY(4000);
    }
    Data_Convert_Float();
    return 'T';
}


