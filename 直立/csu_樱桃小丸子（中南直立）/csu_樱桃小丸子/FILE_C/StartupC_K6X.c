/*****************************************************/      
/*Auther: ���ϴ�ѧ�ƾ���                             */       
/*DATE  �� 2016/12/10                                */      
/*DISCRIBE:Kinetis MK6Xϵ������C�ļ�                 */
/*TEST 1:  2016/12/23/22:50                          */
/*TEST 2:  2016/1/6/23:42   ����ΪMK66�����ļ�       */
/*                                                   */
/*****************************************************/


//////ADD��2M��FLASH��  FLEXNVM���Բ�ʹ�ã�����FLASH�Ĳ�����FLASHNVMҪ���ѣ�ʵ����ʹ��FLEXNVM����һ��
/////�˴����ΪK66�����������������MAIN�û����棬���Բ���������κ�������ֱ��ʹ��
/////
#include "GeneralDeclare.h"
#include "Startup_K6X.h"

////////////////����Σ�  ���޸�ICF�����ļ����Ժ��ٸ��
#pragma section = ".data"
#pragma section = ".data_init"
#pragma section = ".bss"
#pragma section = "CodeRelocate"
#pragma section = "CodeRelocateRam"

#pragma location = ".intvec"                   //ָ�� ����������� intvec �μ�ICF�ļ�ָ����RAM��ַ//ֻ��
const vector_entry  __vector_table[] =          //@ ".intvec" =
{
    VECTOR_000,           /* ��ֵ��λ����ʱ���Ƶ� SP ����ָ��ջ��ַ          */
    VECTOR_001,           /* ��ֵ��λ����ʱ���Ƶ� PC ������������ִ�еĺ���  */////��Reset����
    VECTOR_002,
    VECTOR_003,
    VECTOR_004,
    VECTOR_005,
    VECTOR_006,
    VECTOR_007,
    VECTOR_008,
    VECTOR_009,
    VECTOR_010,
    VECTOR_011,
    VECTOR_012,
    VECTOR_013,
    VECTOR_014,
    VECTOR_015,
    VECTOR_016,
    VECTOR_017,
    VECTOR_018,
    VECTOR_019,
    VECTOR_020,
    VECTOR_021,
    VECTOR_022,
    VECTOR_023,
    VECTOR_024,
    VECTOR_025,
    VECTOR_026,
    VECTOR_027,
    VECTOR_028,
    VECTOR_029,
    VECTOR_030,
    VECTOR_031,
    VECTOR_032,
    VECTOR_033,
    VECTOR_034,
    VECTOR_035,
    VECTOR_036,
    VECTOR_037,
    VECTOR_038,
    VECTOR_039,
    VECTOR_040,
    VECTOR_041,
    VECTOR_042,
    VECTOR_043,
    VECTOR_044,
    VECTOR_045,
    VECTOR_046,
    VECTOR_047,
    VECTOR_048,
    VECTOR_049,
    VECTOR_050,
    VECTOR_051,
    VECTOR_052,
    VECTOR_053,
    VECTOR_054,
    VECTOR_055,
    VECTOR_056,
    VECTOR_057,
    VECTOR_058,
    VECTOR_059,
    VECTOR_060,
    VECTOR_061,
    VECTOR_062,
    VECTOR_063,
    VECTOR_064,
    VECTOR_065,
    VECTOR_066,
    VECTOR_067,
    VECTOR_068,
    VECTOR_069,
    VECTOR_070,
    VECTOR_071,
    VECTOR_072,
    VECTOR_073,
    VECTOR_074,
    VECTOR_075,
    VECTOR_076,
    VECTOR_077,
    VECTOR_078,
    VECTOR_079,
    VECTOR_080,
    VECTOR_081,
    VECTOR_082,
    VECTOR_083,
    VECTOR_084,
    VECTOR_085,
    VECTOR_086,
    VECTOR_087,
    VECTOR_088,
    VECTOR_089,
    VECTOR_090,
    VECTOR_091,
    VECTOR_092,
    VECTOR_093,
    VECTOR_094,
    VECTOR_095,
    VECTOR_096,
    VECTOR_097,
    VECTOR_098,
    VECTOR_099,
    VECTOR_100,
    VECTOR_101,
    VECTOR_102,
    VECTOR_103,
    VECTOR_104,
    VECTOR_105,
    VECTOR_106,
    VECTOR_107,
    VECTOR_108,
    VECTOR_109,
    VECTOR_110,
    VECTOR_111,
    VECTOR_112,
    VECTOR_113,
    VECTOR_114,
    VECTOR_115,
    VECTOR_116,
    VECTOR_117,
    VECTOR_118,
    VECTOR_119,
    VECTOR_120,
    VECTOR_121,
    VECTOR_122,
    VECTOR_123,
    VECTOR_124,
    VECTOR_125,
    VECTOR_126,
    VECTOR_127,
    VECTOR_128,
    VECTOR_129,
    VECTOR_130,
    VECTOR_131,
    VECTOR_132,
    VECTOR_133,
    VECTOR_134,
    VECTOR_135,
    VECTOR_136,
    VECTOR_137,
    VECTOR_138,
    VECTOR_139,
    VECTOR_140,
    VECTOR_141,
    VECTOR_142,
    VECTOR_143,
    VECTOR_144,
    VECTOR_145,
    VECTOR_146,
    VECTOR_147,
    VECTOR_148,
    VECTOR_149,
    VECTOR_150,
    VECTOR_151,
    VECTOR_152,
    VECTOR_153,
    VECTOR_154,
    VECTOR_155,
    VECTOR_156,
    VECTOR_157,
    VECTOR_158,
    VECTOR_159,
    VECTOR_160,
    VECTOR_161,
    VECTOR_162,
    VECTOR_163,
    VECTOR_164,
    VECTOR_165,
    VECTOR_166,
    VECTOR_167,
    VECTOR_168,
    VECTOR_169,
    VECTOR_170,
    VECTOR_171,
    VECTOR_172,
    VECTOR_173,
    VECTOR_174,
    VECTOR_175,
    VECTOR_176,
    VECTOR_177,
    VECTOR_178,
    VECTOR_179,
    VECTOR_180,
    VECTOR_181,
    VECTOR_182,
    VECTOR_183,
    VECTOR_184,
    VECTOR_185,
    VECTOR_186,
    VECTOR_187,
    VECTOR_188,
    VECTOR_189,
    VECTOR_190,
    VECTOR_191,
    VECTOR_192,
    VECTOR_193,
    VECTOR_194,
    VECTOR_195,
    VECTOR_196,
    VECTOR_197,
    VECTOR_198,
    VECTOR_199,
    VECTOR_200,
    VECTOR_201,
    VECTOR_202,
    VECTOR_203,
    VECTOR_204,
    VECTOR_205,
    VECTOR_206,
    VECTOR_207,
    VECTOR_208,
    VECTOR_209,
    VECTOR_210,
    VECTOR_211,
    VECTOR_212,
    VECTOR_213,
    VECTOR_214,
    VECTOR_215,
    VECTOR_216,
    VECTOR_217,
    VECTOR_218,
    VECTOR_219,
    VECTOR_220,
    VECTOR_221,
    VECTOR_222,
    VECTOR_223,
    VECTOR_224,
    VECTOR_225,
    VECTOR_226,
    VECTOR_227,
    VECTOR_228,
    VECTOR_229,
    VECTOR_230,
    VECTOR_231,
    VECTOR_232,
    VECTOR_233,
    VECTOR_234,
    VECTOR_235,
    VECTOR_236,
    VECTOR_237,
    VECTOR_238,
    VECTOR_239,
    VECTOR_240,
    VECTOR_241,
    VECTOR_242,
    VECTOR_243,
    VECTOR_244,
    VECTOR_245,
    VECTOR_246,
    VECTOR_247,
    VECTOR_248,
    VECTOR_249,
    VECTOR_250,
    VECTOR_251,
    VECTOR_252,
    VECTOR_253,
    VECTOR_254,
    VECTOR_255,
    CONFIG_1,        /* Flash configuration field values */
    CONFIG_2,        /* Flash configuration field values */
    CONFIG_3,        /* Flash configuration field values */
    CONFIG_4,        /* Flash configuration field values */
};

void default_isr(void)                       //Ĭ���жϷ�����
{ 
   #define VECTORNUM     (*(volatile uint8_t*)(0xE000ED04)) 
  PTD_BASE_PTR->PDOR &= ~(1 << 2);
  PTD_BASE_PTR->PDOR &= ~(1 << 3);
  /*������ִ�и����жϳ����ĳ�����������BUG*/
  /**/
  
  return;               
}

int core_clk_khz;      //����ʱ��KHZ
int core_clk_mhz;      //����ʱ��MHZ
int bus_clk_khz;       //����ʱ��KHZ

extern void main(void);   //�û�ִ��������
void start(void)                              //���ݷ�˼�����ṩ��׼���������޸�
{
  DisableInterrupts;                          //�ر�ȫ���ж�
  WDOG_UNLOCK = 0xC520;                       //�������Ź�
  WDOG_UNLOCK = 0xD928;
  EnableInterrupts;                           //������ɿ���ȫ���ж�
  WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;  //���ÿ��Ź�
  //STEP ONE
  
  uint32 n;
  uint8 *data_ram, * data_rom, * data_rom_end;//Freescale
  uint8 *bss_start, * bss_end;
  extern uint32 __VECTOR_TABLE[];                 //��ICF�ļ��л�ȡ��ַ
  extern uint32 __VECTOR_RAM[];
  
  if (__VECTOR_RAM != __VECTOR_TABLE)             //�������RAM����������Ҫ�����ж�������
  {
     for(n = 0; n < 0x410; n++)                   //������Ƶ��ж��������Ӧ��ַ��
       __VECTOR_RAM[n] = __VECTOR_TABLE[n];
  }
  write_vtor((uint32)__VECTOR_RAM);               //ָ���µ��ж��������ַ

  data_ram = __section_begin(".data");            //�Ѹ���ֵ�ı����ĵ�ַ��RAM��
  data_rom = __section_begin(".data_init");       //�Ѹ���ֵ�ı��������ݴ����ROM���Ҫ��ֵ��RAM��
  data_rom_end = __section_end(".data_init");
  n = data_rom_end - data_rom;
  while (n--)
    *data_ram++ = *data_rom++;
  bss_start = __section_begin(".bss");
  bss_end = __section_end(".bss");
  n = bss_end - bss_start;
  while(n--)
    *bss_start++ = 0;
  uint8 *code_relocate_ram = __section_begin("CodeRelocateRam");
  uint8 *code_relocate = __section_begin("CodeRelocate");
  uint8 *code_relocate_end = __section_end("CodeRelocate");
  n = code_relocate_end - code_relocate;
  while (n--)
    *code_relocate_ram++ = *code_relocate++;
  //STEP TWO
   
  MCG_C2 &= ~MCG_C2_LP_MASK;                 //FEI -> FBE
  MCG_C2 |= MCG_C2_RANGE(1);
  MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(7);
  while (MCG_S & MCG_S_IREFST_MASK);                                //�ȴ�FLL�ο�ʱ�� Ϊ �ⲿ�ο�ʱ�ӣ�S[IREFST]=0����ʾʹ���ⲿ�ο�ʱ�ӣ���
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2); //�ȴ�ѡ���ⲿ�ο�ʱ��
  
  set_sys_dividers(COREDIV, BUSDIV, FLEXDIV, FLASHDIV);           //FBE -> PBE
  
  MCG_C5 = MCG_C5_PRDIV(OCXOPRDIV);                              //��Ƶ�� EXTAL_IN_MHz/( PRDIV+1)
  MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(OCXOVDIV) ;             //��Ƶ�� EXTAL_IN_MHz/( PRDIV+1)  * (VDIV+16)/2
  while(!(MCG_S & MCG_S_PLLST_MASK));                            //�ȴ�ʱ��Դѡ��PLL
  while(!(MCG_S & MCG_S_LOCK0_MASK));                             //�ȴ����໷
  
   MCG_C1 &= ~MCG_C1_CLKS_MASK;                                    //PBE -> PEE     
   while(((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3);//�ȴ�ѡ�����PLL
   //STEP THREE       
   
   core_clk_mhz = CORECLK;                                        //��¼ʱ���Թ�����
   core_clk_khz = CORECLK * 1000;
   bus_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24) + 1);
   
   //SCB->CPACR |=((3UL << 10*2)|(3UL << 11*2));   ////����Ӳ�����㣬C������ʽ��ֱ���ڻ�������
   ////////////////////ִ���û�����
   main();
   while(1); /////////��ѭ��
}

void write_vtor(int vtor)                                          //��д�ж�����
{
    SCB->VTOR = vtor;
}

void set_vector_handler(VECTORn_t vector , void pfunc_handler(void))//�����жϸ�λ����
{
    extern uint32 __VECTOR_RAM[];
    __VECTOR_RAM[vector] = (uint32)pfunc_handler;
}

//////////Freescale�ṩ  ʱ�ӷ�Ƶ����  �������RAMִ��
__ramfunc  void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4)
{
    uint32 temp_reg;
    uint8 i;

    temp_reg = FMC_PFAPR;

    FMC_PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
                 | FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
                 | FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;

    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) | SIM_CLKDIV1_OUTDIV2(outdiv2)
                  | SIM_CLKDIV1_OUTDIV3(outdiv3) | SIM_CLKDIV1_OUTDIV4(outdiv4);

    for (i = 0 ; i < outdiv4 ; i++);

    FMC_PFAPR = temp_reg; 

    return;
}