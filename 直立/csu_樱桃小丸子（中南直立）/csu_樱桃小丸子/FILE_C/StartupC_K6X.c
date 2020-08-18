/*****************************************************/      
/*Auther: 中南大学黄竞辉                             */       
/*DATE  ： 2016/12/10                                */      
/*DISCRIBE:Kinetis MK6X系列启动C文件                 */
/*TEST 1:  2016/12/23/22:50                          */
/*TEST 2:  2016/1/6/23:42   更改为MK66启动文件       */
/*                                                   */
/*****************************************************/


//////ADD：2M的FLASH，  FLEXNVM可以不使用，但是FLASH的擦除比FLASHNVM要困难，实际上使用FLEXNVM更好一点
/////此代码仅为K66启动程序，启动后进入MAIN用户界面，可以不管这里的任何描述，直接使用
/////
#include "GeneralDeclare.h"
#include "Startup_K6X.h"

////////////////代码段，  可修改ICF链接文件，以后再搞吧
#pragma section = ".data"
#pragma section = ".data_init"
#pragma section = ".bss"
#pragma section = "CodeRelocate"
#pragma section = "CodeRelocateRam"

#pragma location = ".intvec"                   //指定 如下数组放入 intvec 段即ICF文件指定的RAM首址//只读
const vector_entry  __vector_table[] =          //@ ".intvec" =
{
    VECTOR_000,           /* 此值复位启动时复制到 SP ，即指定栈地址          */
    VECTOR_001,           /* 此值复位启动时复制到 PC ，即启动马上执行的函数  */////即Reset函数
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

void default_isr(void)                       //默认中断服务函数
{ 
   #define VECTORNUM     (*(volatile uint8_t*)(0xE000ED04)) 
  PTD_BASE_PTR->PDOR &= ~(1 << 2);
  PTD_BASE_PTR->PDOR &= ~(1 << 3);
  /*可用以执行各种中断出错后的程序来检查程序BUG*/
  /**/
  
  return;               
}

int core_clk_khz;      //核心时钟KHZ
int core_clk_mhz;      //核心时钟MHZ
int bus_clk_khz;       //总线时钟KHZ

extern void main(void);   //用户执行主函数
void start(void)                              //根据飞思卡尔提供标准启动程序修改
{
  DisableInterrupts;                          //关闭全部中断
  WDOG_UNLOCK = 0xC520;                       //解锁看门狗
  WDOG_UNLOCK = 0xD928;
  EnableInterrupts;                           //解锁完成开启全部中断
  WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;  //禁用看门狗
  //STEP ONE
  
  uint32 n;
  uint8 *data_ram, * data_rom, * data_rom_end;//Freescale
  uint8 *bss_start, * bss_end;
  extern uint32 __VECTOR_TABLE[];                 //从ICF文件中获取地址
  extern uint32 __VECTOR_RAM[];
  
  if (__VECTOR_RAM != __VECTOR_TABLE)             //如果不是RAM启动，则需要复制中断向量表
  {
     for(n = 0; n < 0x410; n++)                   //逐个复制到中断向量表对应地址中
       __VECTOR_RAM[n] = __VECTOR_TABLE[n];
  }
  write_vtor((uint32)__VECTOR_RAM);               //指定新的中断向量表地址

  data_ram = __section_begin(".data");            //已赋初值的变量的地址在RAM里
  data_rom = __section_begin(".data_init");       //已赋初值的变量的数据存放在ROM里，需要赋值到RAM里
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
  while (MCG_S & MCG_S_IREFST_MASK);                                //等待FLL参考时钟 为 外部参考时钟（S[IREFST]=0，表示使用外部参考时钟，）
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2); //等待选择外部参考时钟
  
  set_sys_dividers(COREDIV, BUSDIV, FLEXDIV, FLASHDIV);           //FBE -> PBE
  
  MCG_C5 = MCG_C5_PRDIV(OCXOPRDIV);                              //分频， EXTAL_IN_MHz/( PRDIV+1)
  MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(OCXOVDIV) ;             //倍频， EXTAL_IN_MHz/( PRDIV+1)  * (VDIV+16)/2
  while(!(MCG_S & MCG_S_PLLST_MASK));                            //等待时钟源选择PLL
  while(!(MCG_S & MCG_S_LOCK0_MASK));                             //等待锁相环
  
   MCG_C1 &= ~MCG_C1_CLKS_MASK;                                    //PBE -> PEE     
   while(((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3);//等待选择输出PLL
   //STEP THREE       
   
   core_clk_mhz = CORECLK;                                        //记录时钟以供调用
   core_clk_khz = CORECLK * 1000;
   bus_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24) + 1);
   
   //SCB->CPACR |=((3UL << 10*2)|(3UL << 11*2));   ////开启硬件浮点，C语言形式，直接在汇编中完成
   ////////////////////执行用户函数
   main();
   while(1); /////////死循环
}

void write_vtor(int vtor)                                          //重写中断向量
{
    SCB->VTOR = vtor;
}

void set_vector_handler(VECTORn_t vector , void pfunc_handler(void))//设置中断复位函数
{
    extern uint32 __VECTOR_RAM[];
    __VECTOR_RAM[vector] = (uint32)pfunc_handler;
}

//////////Freescale提供  时钟分频函数  必须放在RAM执行
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