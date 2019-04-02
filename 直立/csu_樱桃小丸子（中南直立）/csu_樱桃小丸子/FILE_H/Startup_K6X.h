#ifndef STARTUP_K6X_H
#define STARTUP_K6X_H

///////////////////时钟部分
#if   (defined(MK60DZ10))
#define OCXOCLK     50       //对于K60DN, coreclk = ocxoclk * (VDIV + 24) / (PRDIV +1)
#define OCXOPRDIV   14
#define OCXOVDIV    24       
#define CORECLK     160      //50 * （24 + 24）/(14 +1) = 160
#define COREDIV     0        //不对核心频率分频
#define BUSDIV      1        //BUS = CORE/(BUSDIV+1)以下同理 BUS = 80MHZ
#define FLEXDIV     5        //FLEX = 26.67MHZ
#define FLASHDIV    5        //FLASH = 26.67MHZ
#elif (defined(MK66F18))
#define OCXOCLK     50       //对于K66FN, coreclk = ocxoclk * (VDIV + 16) / (PRDIV +1)/2
#define OCXOPRDIV   4
#define OCXOVDIV    28       
#define CORECLK     220      //50 * （26 + 16）/(4+1)/2 = 210
#define COREDIV     0        //不对核心频率分频
#define BUSDIV      1        //BUS = CORE/(BUSDIV+1)以下同理 BUS = 70MHZ
#define FLEXDIV     6        //FLEX = 30MHZ
#define FLASHDIV    6        //FLASH = 30MHZ
#endif

//////以上设置K60为主频160M   K66主频210M
//////DATE：2017/1/7
extern int core_clk_khz;      //核心时钟KHZ
extern int core_clk_mhz;      //核心时钟MHZ
extern int bus_clk_khz;       //总线时钟KHZ
////////////////////////////////////////////////////



//////////////////中断向量部分
extern void default_isr(void);                //定义默认中断服务函数
extern void Reset_Handler(void);             //从汇编启动文件获得
extern unsigned long __BOOT_STACK_ADDRESS[];  //声明栈地址，从ICF文件获得
typedef void (*vector_entry)(void);           //声明中断向量类型

///////////////////中断向量表初始化
#define VECTOR_000      (vector_entry)__BOOT_STACK_ADDRESS  //          ARM core        Initial Supervisor SP
#define VECTOR_001      Reset_Handler   // 0x0000_0004 1 -          ARM core        Initial Program Counter
#define VECTOR_002      default_isr     // 0x0000_0008 2 -          ARM core        Non-maskable Interrupt (NMI)
#define VECTOR_003      default_isr     // 0x0000_000C 3 -          ARM core        Hard Fault
#define VECTOR_004      default_isr     // 0x0000_0010 4 -
#define VECTOR_005      default_isr     // 0x0000_0014 5 -          ARM core         Bus Fault
#define VECTOR_006      default_isr     // 0x0000_0018 6 -          ARM core         Usage Fault
#define VECTOR_007      default_isr     // 0x0000_001C 7 -                           
#define VECTOR_008      default_isr     // 0x0000_0020 8 -                           
#define VECTOR_009      default_isr     // 0x0000_0024 9 -
#define VECTOR_010      default_isr     // 0x0000_0028 10 -
#define VECTOR_011      default_isr     // 0x0000_002C 11 -         ARM core         Supervisor call (SVCall)
#define VECTOR_012      default_isr     // 0x0000_0030 12 -         ARM core         Debug Monitor
#define VECTOR_013      default_isr     // 0x0000_0034 13 -                          
#define VECTOR_014      default_isr     // 0x0000_0038 14 -         ARM core         Pendable request for system service (PendableSrvReq)
#define VECTOR_015      default_isr     // 0x0000_003C 15 -         ARM core         System tick timer (SysTick)
#define VECTOR_016      default_isr     // 0x0000_0040 16     0     DMA              DMA Channel 0 transfer complete
#define VECTOR_017      default_isr     // 0x0000_0044 17     1     DMA              DMA Channel 1 transfer complete
#define VECTOR_018      default_isr     // 0x0000_0048 18     2     DMA              DMA Channel 2 transfer complete
#define VECTOR_019      default_isr     // 0x0000_004C 19     3     DMA              DMA Channel 3 transfer complete
#define VECTOR_020      default_isr     // 0x0000_0050 20     4     DMA              DMA Channel 4 transfer complete
#define VECTOR_021      default_isr     // 0x0000_0054 21     5     DMA              DMA Channel 5 transfer complete
#define VECTOR_022      default_isr     // 0x0000_0058 22     6     DMA              DMA Channel 6 transfer complete
#define VECTOR_023      default_isr     // 0x0000_005C 23     7     DMA              DMA Channel 7 transfer complete
#define VECTOR_024      default_isr     // 0x0000_0060 24     8     DMA              DMA Channel 8 transfer complete
#define VECTOR_025      default_isr     // 0x0000_0064 25     9     DMA              DMA Channel 9 transfer complete
#define VECTOR_026      default_isr     // 0x0000_0068 26    10     DMA              DMA Channel 10 transfer complete
#define VECTOR_027      default_isr     // 0x0000_006C 27    11     DMA              DMA Channel 11 transfer complete
#define VECTOR_028      default_isr     // 0x0000_0070 28    12     DMA              DMA Channel 12 transfer complete
#define VECTOR_029      default_isr     // 0x0000_0074 29    13     DMA              DMA Channel 13 transfer complete
#define VECTOR_030      default_isr     // 0x0000_0078 30    14     DMA              DMA Channel 14 transfer complete
#define VECTOR_031      default_isr     // 0x0000_007C 31    15     DMA              DMA Channel 15 transfer complete
#define VECTOR_032      default_isr     // 0x0000_0080 32    16     DMA              DMA Error Interrupt Channels 0-15
#define VECTOR_033      default_isr     // 0x0000_0084 33    17     MCM              Normal interrupt
#define VECTOR_034      default_isr     // 0x0000_0088 34    18     Flash memory     Command Complete
#define VECTOR_035      default_isr     // 0x0000_008C 35    19     Flash memory     Read Collision
#define VECTOR_036      default_isr     // 0x0000_0090 36    20     Mode Controller  Low Voltage Detect,Low Voltage Warning, Low Leakage Wakeup
#define VECTOR_037      default_isr     // 0x0000_0094 37    21     LLWU
#define VECTOR_038      default_isr     // 0x0000_0098 38    22     WDOG
#define VECTOR_039      default_isr     // 0x0000_009C 39    23     RNGB
#define VECTOR_040      default_isr     // 0x0000_00A0 40    24     I2C0
#define VECTOR_041      default_isr     // 0x0000_00A4 41    25     I2C1
#define VECTOR_042      default_isr     // 0x0000_00A8 42    26     SPI0             Single interrupt vector for all sources
#define VECTOR_043      default_isr     // 0x0000_00AC 43    27     SPI1             Single interrupt vector for all sources
#define VECTOR_044      default_isr     // 0x0000_00B0 44    28     I2S0             Single interrupt vector for all sources
#define VECTOR_045      default_isr     // 0x0000_00B4 45    29     I2S0             OR'ed Message buffer (0-15)
#define VECTOR_046      default_isr     // 0x0000_00B8 46    30     --             Bus Off
#define VECTOR_047      default_isr     // 0x0000_00BC 47    31     UART0             Error
#define VECTOR_048      default_isr     // 0x0000_00C0 48    32     UART0             Transmit Warning
#define VECTOR_049      default_isr     // 0x0000_00C4 49    33     UART1             Receive Warning
#define VECTOR_050      default_isr     // 0x0000_00C8 50    34     UART1             Wake Up
#define VECTOR_051      default_isr     // 0x0000_00CC 51    35     UART2             Individual Matching Elements Update (IMEU)
#define VECTOR_052      default_isr     // 0x0000_00D0 52    36     UART2            Lost receive
#define VECTOR_053      default_isr     // 0x0000_00D4 53    37     UART3             OR'ed Message buffer (0-15)
#define VECTOR_054      default_isr     // 0x0000_00D8 54    38     UART3             Bus off
#define VECTOR_055      default_isr     // 0x0000_00DC 55    39     ADC0             Error
#define VECTOR_056      default_isr     // 0x0000_00E0 56    40     CMP0             Transmit Warning
#define VECTOR_057      default_isr     // 0x0000_00E4 57    41     CMP1             Receive Warning
#define VECTOR_058      default_isr     // 0x0000_00E8 58    42     FTM0             Wake Up
#define VECTOR_059      default_isr     // 0x0000_00EC 59    43     FTM1             Individual Matching Elements Update (IMEU)
#define VECTOR_060      default_isr     // 0x0000_00F0 60    44     FTM2             Lost receive 
#define VECTOR_061      default_isr     // 0x0000_00F4 61    45     CMT            Single interrupt vector for UART status sources
#define VECTOR_062      default_isr     // 0x0000_00F8 62    46     RTC            Single interrupt vector for UART error sources
#define VECTOR_063      default_isr     // 0x0000_00FC 63    47     RTC            Single interrupt vector for UART status sources
#define VECTOR_064      default_isr     // 0x0000_0100 64    48     PIT0            Single interrupt vector for UART error sources
#define VECTOR_065      default_isr     // 0x0000_0104 65    49     PIT1            Single interrupt vector for UART status sources
#define VECTOR_066      default_isr     // 0x0000_0108 66    50     PIT2            Single interrupt vector for UART error sources
#define VECTOR_067      default_isr     // 0x0000_010C 67    51     PIT3            Single interrupt vector for UART status sources
#define VECTOR_068      default_isr     // 0x0000_0110 68    52     PDB            Single interrupt vector for UART error sources
#define VECTOR_069      default_isr     // 0x0000_0114 69    53     USB OTG            Single interrupt vector for UART status sources
#define VECTOR_070      default_isr     // 0x0000_0118 70    54     USB CHARGE           Single interrupt vector for UART error sources
#define VECTOR_071      default_isr     // 0x0000_011C 71    55     --            Single interrupt vector for UART status sources
#define VECTOR_072      default_isr     // 0x0000_0120 72    56     DAC0            Single interrupt vector for UART error sources
#define VECTOR_073      default_isr     // 0x0000_0124 73    57     MCG
#define VECTOR_074      default_isr     // 0x0000_0128 74    58     LPTMR
#define VECTOR_075      default_isr     // 0x0000_012C 75    59     PORTA             High-speed comparator 
#define VECTOR_076      default_isr     // 0x0000_0130 76    60     PORTB
#define VECTOR_077      default_isr     // 0x0000_0134 77    61     PORTC
#define VECTOR_078      default_isr     // 0x0000_0138 78    62     PORTD             Single interrupt vector for all sources
#define VECTOR_079      default_isr     // 0x0000_013C 79    63     PORTE             Single interrupt vector for all sources
#define VECTOR_080      default_isr     // 0x0000_0140 80    64     SOFTWARE             Single interrupt vector for all sources
#define VECTOR_081      default_isr     // 0x0000_0144 81    65     SPI2
#define VECTOR_082      default_isr     // 0x0000_0148 82    66     UART4
#define VECTOR_083      default_isr     // 0x0000_014C 83    67     UART4
#define VECTOR_084      default_isr     // 0x0000_0150 84    68     --
#define VECTOR_085      default_isr     // 0x0000_0154 85    69     --
#define VECTOR_086      default_isr     // 0x0000_0158 86    70     CMP2
#define VECTOR_087      default_isr     // 0x0000_015C 87    71     FTM3
#define VECTOR_088      default_isr     // 0x0000_0160 88    72     DAC1
#define VECTOR_089      default_isr     // 0x0000_0164 89    73     ADC1
#define VECTOR_090      default_isr     // 0x0000_0168 90    74     I2C2
#define VECTOR_091      default_isr     // 0x0000_016C 91    75     CAN0             IEEE 1588 Timer interrupt           
#define VECTOR_092      default_isr     // 0x0000_0170 92    76     CAN0             Transmit interrupt
#define VECTOR_093      default_isr     // 0x0000_0174 93    77     CAN0             Receive interrupt
#define VECTOR_094      default_isr     // 0x0000_0178 94    78     CAN0             Error and miscellaneous interrupt
#define VECTOR_095      default_isr     // 0x0000_017C 95    79     CAN0
#define VECTOR_096      default_isr     // 0x0000_0180 96    80     CAN0
#define VECTOR_097      default_isr     // 0x0000_0184 97    81     SDCH
#define VECTOR_098      default_isr     // 0x0000_0188 98    82     MAC
#define VECTOR_099      default_isr     // 0x0000_018C 99    83     MAC              Single interrupt vector for all sources
#define VECTOR_100      default_isr     // 0x0000_0190 100   84     MAC
#define VECTOR_101      default_isr     // 0x0000_0194 101   85     MAC 
#define VECTOR_102      default_isr     // 0x0000_0198 102   86     LPUART0      Single interrupt vector for all sources
#define VECTOR_103      default_isr     // 0x0000_019C 103   87     TSI0
#define VECTOR_104      default_isr     // 0x0000_01A0 104   88     TPM1
#define VECTOR_105      default_isr     // 0x0000_01A4 105   89     TPM2
#define VECTOR_106      default_isr     // 0x0000_01A8 106   90     USBHS
#define VECTOR_107      default_isr     // 0x0000_01AC 107   91     I2C3
#define VECTOR_108      default_isr     // 0x0000_01B0 108   92     CMP3
#define VECTOR_109      default_isr     // 0x0000_01B4 109   93     USBOTG
#define VECTOR_110      default_isr     // 0x0000_01B8 110   94     CAN1
#define VECTOR_111      default_isr     // 0x0000_01BC 111   95     CAN1
#define VECTOR_112      default_isr     // 0x0000_01C0 112   96     CAN1
#define VECTOR_113      default_isr     // 0x0000_01C4 113   97     CAN1
#define VECTOR_114      default_isr     // 0x0000_01C8 114   98     CAN1
#define VECTOR_115      default_isr     // 0x0000_01CC 115   99     CAN1
#define VECTOR_116      default_isr     // 0x0000_01D0 116   100
#define VECTOR_117      default_isr     // 0x0000_01D4 117   101
#define VECTOR_118      default_isr     // 0x0000_01D8 118   102
#define VECTOR_119      default_isr     // 0x0000_01DC 119   103
#define VECTOR_120      default_isr     // 
#define VECTOR_121      default_isr     // 
#define VECTOR_122      default_isr     // 
#define VECTOR_123      default_isr     // 
#define VECTOR_124      default_isr     // 
#define VECTOR_125      default_isr     // 
#define VECTOR_126      default_isr     // 
#define VECTOR_127      default_isr     // 
#define VECTOR_128      default_isr     // 
#define VECTOR_129      default_isr     // 
#define VECTOR_130      default_isr     // 
#define VECTOR_131      default_isr     // 
#define VECTOR_132      default_isr     // 
#define VECTOR_133      default_isr     // 
#define VECTOR_134      default_isr     // 
#define VECTOR_135      default_isr     // 
#define VECTOR_136      default_isr     // 
#define VECTOR_137      default_isr     // 
#define VECTOR_138      default_isr     // 
#define VECTOR_139      default_isr     // 
#define VECTOR_140      default_isr     // 
#define VECTOR_141      default_isr     // 
#define VECTOR_142      default_isr     // 
#define VECTOR_143      default_isr     // 
#define VECTOR_144      default_isr     // 
#define VECTOR_145      default_isr     // 
#define VECTOR_146      default_isr     // 
#define VECTOR_147      default_isr     // 
#define VECTOR_148      default_isr     // 
#define VECTOR_149      default_isr     // 
#define VECTOR_150      default_isr     // 
#define VECTOR_151      default_isr     // 
#define VECTOR_152      default_isr     // 
#define VECTOR_153      default_isr     // 
#define VECTOR_154      default_isr     // 
#define VECTOR_155      default_isr     // 
#define VECTOR_156      default_isr     // 
#define VECTOR_157      default_isr     // 
#define VECTOR_158      default_isr     // 
#define VECTOR_159      default_isr     // 
#define VECTOR_160      default_isr     // 
#define VECTOR_161      default_isr     // 
#define VECTOR_162      default_isr     // 
#define VECTOR_163      default_isr     // 
#define VECTOR_164      default_isr     // 
#define VECTOR_165      default_isr     // 
#define VECTOR_166      default_isr     // 
#define VECTOR_167      default_isr     // 
#define VECTOR_168      default_isr     // 
#define VECTOR_169      default_isr     // 
#define VECTOR_170      default_isr     // 
#define VECTOR_171      default_isr     // 
#define VECTOR_172      default_isr     // 
#define VECTOR_173      default_isr     // 
#define VECTOR_174      default_isr     // 
#define VECTOR_175      default_isr     // 
#define VECTOR_176      default_isr     // 
#define VECTOR_177      default_isr     // 
#define VECTOR_178      default_isr     // 
#define VECTOR_179      default_isr     // 
#define VECTOR_180      default_isr     // 
#define VECTOR_181      default_isr     // 
#define VECTOR_182      default_isr     // 
#define VECTOR_183      default_isr     // 
#define VECTOR_184      default_isr     // 
#define VECTOR_185      default_isr     // 
#define VECTOR_186      default_isr     // 
#define VECTOR_187      default_isr     // 
#define VECTOR_188      default_isr     // 
#define VECTOR_189      default_isr     // 
#define VECTOR_190      default_isr     // 
#define VECTOR_191      default_isr     // 
#define VECTOR_192      default_isr     // 
#define VECTOR_193      default_isr     // 
#define VECTOR_194      default_isr     // 
#define VECTOR_195      default_isr     // 
#define VECTOR_196      default_isr     // 
#define VECTOR_197      default_isr     // 
#define VECTOR_198      default_isr     // 
#define VECTOR_199      default_isr     // 
#define VECTOR_200      default_isr     // 
#define VECTOR_201      default_isr     // 
#define VECTOR_202      default_isr     // 
#define VECTOR_203      default_isr     // 
#define VECTOR_204      default_isr     // 
#define VECTOR_205      default_isr     // 
#define VECTOR_206      default_isr     // 
#define VECTOR_207      default_isr     // 
#define VECTOR_208      default_isr     // 
#define VECTOR_209      default_isr     // 
#define VECTOR_210      default_isr     // 
#define VECTOR_211      default_isr     // 
#define VECTOR_212      default_isr     // 
#define VECTOR_213      default_isr     // 
#define VECTOR_214      default_isr     // 
#define VECTOR_215      default_isr     // 
#define VECTOR_216      default_isr     // 
#define VECTOR_217      default_isr     // 
#define VECTOR_218      default_isr     // 
#define VECTOR_219      default_isr     // 
#define VECTOR_220      default_isr     // 
#define VECTOR_221      default_isr     // 
#define VECTOR_222      default_isr     // 
#define VECTOR_223      default_isr     // 
#define VECTOR_224      default_isr     // 
#define VECTOR_225      default_isr     // 
#define VECTOR_226      default_isr     // 
#define VECTOR_227      default_isr     // 
#define VECTOR_228      default_isr     // 
#define VECTOR_229      default_isr     // 
#define VECTOR_230      default_isr     // 
#define VECTOR_231      default_isr     // 
#define VECTOR_232      default_isr     // 
#define VECTOR_233      default_isr     // 
#define VECTOR_234      default_isr     // 
#define VECTOR_235      default_isr     // 
#define VECTOR_236      default_isr     // 
#define VECTOR_237      default_isr     // 
#define VECTOR_238      default_isr     // 
#define VECTOR_239      default_isr     // 
#define VECTOR_240      default_isr     // 
#define VECTOR_241      default_isr     // 
#define VECTOR_242      default_isr     // 
#define VECTOR_243      default_isr     // 
#define VECTOR_244      default_isr     // 
#define VECTOR_245      default_isr     // 
#define VECTOR_246      default_isr     // 
#define VECTOR_247      default_isr     // 
#define VECTOR_248      default_isr     // 
#define VECTOR_249      default_isr     // 
#define VECTOR_250      default_isr     // 
#define VECTOR_251      default_isr     // 
#define VECTOR_252      default_isr     // 
#define VECTOR_253      default_isr     // 
#define VECTOR_254      default_isr     // 
#define VECTOR_255      default_isr     // 
#define CONFIG_1        (vector_entry)0xffffffff
#define CONFIG_2        (vector_entry)0xffffffff
#define CONFIG_3        (vector_entry)0xffffffff
#define CONFIG_4        (vector_entry)0xfffffffe

#if defined(MK66F18)
typedef enum                   //中断号
{
    /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
    NonMaskableInt_IRQn             = -14,      /*!< 2 Non Maskable Interrupt                                                                                    */
    HardFault_IRQn                  = -13 ,     /*!< 3 Hard Fault                                                                                */
    MemoryManagement_IRQn           = -12,      /*!< 4 Cortex-M4 Memory Management Interrupt                                                     */
    BusFault_IRQn                   = -11,      /*!< 5 Cortex-M4 Bus Fault Interrupt                                                                     */
    UsageFault_IRQn                 = -10,      /*!< 6 Cortex-M4 Usage Fault Interrupt                                                               */
    SVCall_IRQn                     = -5,       /*!< 11 Cortex-M4 SV Call Interrupt                                                                      */
    DebugMonitor_IRQn               = -4,       /*!< 12 Cortex-M4 Debug Monitor Interrupt                                                            */
    PendSV_IRQn                     = -2,       /*!< 14 Cortex-M4 Pend SV Interrupt                                                                      */
    SysTick_IRQn                    = -1,       /*!< 15 Cortex-M4 System Tick Interrupt                                                              */
    /******  Kinetis 60 specific Interrupt Numbers **********************************************************************/
    DMA0_IRQn                       = 0,        // DMA Channel 0 Transfer Complete
    DMA1_IRQn                       = 1,        // DMA Channel 1 Transfer Complete
    DMA2_IRQn                       = 2,        // DMA Channel 2 Transfer Complete
    DMA3_IRQn                       = 3,        // DMA Channel 3 Transfer Complete
    DMA4_IRQn                       = 4,        // DMA Channel 4 Transfer Complete
    DMA5_IRQn                       = 5,        // DMA Channel 5 Transfer Complete
    DMA6_IRQn                       = 6,        // DMA Channel 6 Transfer Complete
    DMA7_IRQn                       = 7,        // DMA Channel 7 Transfer Complete
    DMA8_IRQn                       = 8,        // DMA Channel 8 Transfer Complete
    DMA9_IRQn                       = 9,        // DMA Channel 9 Transfer Complete
    DMA10_IRQn                      = 10,       // DMA Channel 10 Transfer Complete
    DMA11_IRQn                      = 11,       // DMA Channel 11 Transfer Complete
    DMA12_IRQn                      = 12,       // DMA Channel 12 Transfer Complete
    DMA13_IRQn                      = 13,       // DMA Channel 13 Transfer Complete
    DMA14_IRQn                      = 14,       // DMA Channel 14 Transfer Complete
    DMA15_IRQn                      = 15,       // DMA Channel 15 Transfer Complete
    DMA_Error_IRQn                  = 16,       // DMA Error Interrupt
    MCM_IRQn                        = 17,       // Normal Interrupt
    FTFL_IRQn                       = 18,       // FTFL Interrupt
    Read_Collision_IRQn             = 19,       // Read Collision Interrupt
    LVD_LVW_IRQn                    = 20,       // Low Voltage Detect, Low Voltage Warning
    LLW_IRQn                        = 21,       // Low Leakage Wakeup
    Watchdog_IRQn                   = 22,       // WDOG Interrupt
    RNG_IRQn                        = 23,       // RNGB Interrupt
    I2C0_IRQn                       = 24,       // I2C0 interrupt
    I2C1_IRQn                       = 25,       // I2C1 interrupt
    SPI0_IRQn                       = 26,       // SPI0 Interrupt
    SPI1_IRQn                       = 27,       // SPI1 Interrupt
    I2S0_IRQn                       = 28,       // SPI2 Interrupt
    I2S01_IRQn                      = 29,       // CAN0 OR'd Message Buffers Interrupt
    Reserved_IRQn                   = 30,       // CAN0 Bus Off Interrupt
    UART0_RX_TX_IRQn                = 31,       // CAN0 Error Interrupt
    UART0_IRQn                      = 32,       // CAN0 Tx Warning Interrupt
    UART1_RX_TX_IRQn                = 33,       // CAN0 Rx Warning Interrupt
    UART1_IRQn                      = 34,       // CAN0 Wake Up Interrupt
    UART2_RX_TX_IRQn                = 35,       // 保留 interrupt 51
    UART2_IRQn                      = 36,
    UART3_RX_TX_IRQn                = 37,       // 保留 interrupt 52
    UART3_IRQn                      = 38,       // CAN1 OR'd Message Buffers Interrupt
    ADC0_IRQn                       = 39,       // CAN1 Bus Off Interrupt
    CMP0_IRQn                       = 40,       // CAN1 Error Interrupt
    CMP1_IRQn                       = 41,       // CAN1 Tx Warning Interrupt
    FTM0_IRQn                       = 42,       // CAN1 Rx Warning Interrupt
    FTM1_IRQn                       = 43,       // CAN1 Wake Up Interrupt
    FTM2_IRQn                       = 44,       // 保留 interrupt 59
    CMT_IRQn                        = 45,       // 保留 interrupt 60
    RTC0_IRQn                       = 46,       // UART0 Receive/Transmit interrupt
    RTC1_IRQn                       = 47,       // UART0 Error interrupt
    PIT0_IRQn                       = 48,       // UART1 Receive/Transmit interrupt
    PIT1_IRQn                       = 49,       // UART1 Error interrupt
    PIT2_IRQn                       = 50,       // UART2 Receive/Transmit interrupt
    PIT3_IRQn                       = 51,       // UART3 Receive/Transmit interrupt
    PDB_IRQn                        = 52,       // UART3 Error interrupt
    USBOTG_IRQn                     = 53,       // UART4 Receive/Transmit interrupt
    USGCHARGE_IRQn                  = 54,       // UART4 Error interrupt
    Reserved1_IRQn                  = 55,       // UART5 Receive/Transmit interrupt
    DAC0_IRQn                       = 56,       // UART5 Error interrupt
    MCG_IRQn                        = 57,       // ADC0 interrupt
    LPTMR_IRQn                       = 58,       // ADC1 interrupt
    PORTA_IRQn                       = 59,       // CMP0 interrupt
    PORTB_IRQn                       = 60,       // CMP1 interrupt
    PORTC_IRQn                       = 61,       // CMP2 interrupt
    PORTD_IRQn                       = 62,       // FTM0 fault, overflow and channels interrupt
    PORTE_IRQn                       = 63,       // FTM1 fault, overflow and channels interrupt
    SOFT_IRQn                        = 64,       // FTM2 fault, overflow and channels interrupt
    SPI2_IRQn                        = 65,       // CMT interrupt
    UART4_RX_TX_IRQn                 = 66,       // RTC interrupt
    UART4_IRQn                       = 67,       // 保留 interrupt 83
    Reserved2_IRQn                   = 68,       // PIT timer channel 0 interrupt
    Reserved3_IRQn                   = 69,       // PIT timer channel 1 interrupt
    CMP2_IRQn                       = 70,       // PIT timer channel 2 interrupt
    FTM3_IRQn                       = 71,       // PIT timer channel 3 interrupt
    DAC1_IRQn                       = 72,       // PDB0 Interrupt
    ADC1_IRQn                       = 73,       // USB0 interrupt
    I2C2_IRQn                     = 74,       // USBDCD Interrupt
    CAN01_IRQn                    = 75,       // Ethernet MAC IEEE 1588 Timer Interrupt
    CAN02_IRQn                    = 76,       // Ethernet MAC Transmit Interrupt
    CAN03_IRQn                    = 77,       // Ethernet MAC Receive Interrupt
    CAN04_IRQn                    = 78,       // Ethernet MAC Error and miscelaneous Interrupt
    CAN05_IRQn                    = 79,       // I2S0 Interrupt
    CAN06_IRQn                    = 80,       // SDHC Interrupt
    SDCH_IRQn                     = 81,       // DAC0 interrupt
    MAC0_IRQn                     = 82,       // DAC1 interrupt
    MAC1_IRQn                 = 83,       // TSI0 Interrupt
    MAC2_IRQn                 = 84,       // MCG Interrupt
    MAC3_IRQn                 = 85,       // LPTimer interrupt
    LPUART0_IRQn              = 86,       // 保留 interrupt 102
    TSI0_IRQn                 = 87,       // Port A interrupt
    TPM1_IRQn                 = 88,       // Port B interrupt
    TPM2_IRQn                 = 89,       // Port C interrupt
    USBHS_IRQn                = 90,       // Port D interrupt
    I2C3_IRQn                 = 91,       // Port E interrupt
    CMP3_IRQn                 = 92,       // 保留 interrupt 108
    USBOTG1_IRQn              = 93,       // 保留 interrupt 109
    CAN10_IRQn                = 94,       // 保留 interrupt 110
    CAN11_IRQn                = 95,       // 保留 interrupt 111
    CAN12_IRQn                = 96,       // 保留 interrupt 112
    CAN13_IRQn                = 97,       // 保留 interrupt 113
    CAN14_IRQn                = 98,       // 保留 interrupt 114
    CAN15_IRQn                = 99,       // 保留 interrupt 115
    Reserved116_IRQn          = 100,      // 保留 interrupt 116
    Reserved117_IRQn          = 101,      // 保留 interrupt 117
    Reserved118_IRQn          = 102,      // 保留 interrupt 118
    Reserved119_IRQn          = 103,      // 保留 interrupt 119
} IRQn_t, IRQn_Type;

#elif defined(MKDZ10)
typedef enum IRQn {
  /* Auxiliary constants */
  NotAvail_IRQn                = -128,             /**< Not available device specific interrupt */

  /* Core interrupts */
  NonMaskableInt_IRQn          = -14,              /**< Non Maskable Interrupt */
  HardFault_IRQn               = -13,              /**< Cortex-M4 SV Hard Fault Interrupt */
  MemoryManagement_IRQn        = -12,              /**< Cortex-M4 Memory Management Interrupt */
  BusFault_IRQn                = -11,              /**< Cortex-M4 Bus Fault Interrupt */
  UsageFault_IRQn              = -10,              /**< Cortex-M4 Usage Fault Interrupt */
  SVCall_IRQn                  = -5,               /**< Cortex-M4 SV Call Interrupt */
  DebugMonitor_IRQn            = -4,               /**< Cortex-M4 Debug Monitor Interrupt */
  PendSV_IRQn                  = -2,               /**< Cortex-M4 Pend SV Interrupt */
  SysTick_IRQn                 = -1,               /**< Cortex-M4 System Tick Interrupt */

  /* Device specific interrupts */
  DMA0_DMA16_IRQn              = 0,                /**< DMA Channel 0, 16 Transfer Complete */
  DMA1_DMA17_IRQn              = 1,                /**< DMA Channel 1, 17 Transfer Complete */
  DMA2_DMA18_IRQn              = 2,                /**< DMA Channel 2, 18 Transfer Complete */
  DMA3_DMA19_IRQn              = 3,                /**< DMA Channel 3, 19 Transfer Complete */
  DMA4_DMA20_IRQn              = 4,                /**< DMA Channel 4, 20 Transfer Complete */
  DMA5_DMA21_IRQn              = 5,                /**< DMA Channel 5, 21 Transfer Complete */
  DMA6_DMA22_IRQn              = 6,                /**< DMA Channel 6, 22 Transfer Complete */
  DMA7_DMA23_IRQn              = 7,                /**< DMA Channel 7, 23 Transfer Complete */
  DMA8_DMA24_IRQn              = 8,                /**< DMA Channel 8, 24 Transfer Complete */
  DMA9_DMA25_IRQn              = 9,                /**< DMA Channel 9, 25 Transfer Complete */
  DMA10_DMA26_IRQn             = 10,               /**< DMA Channel 10, 26 Transfer Complete */
  DMA11_DMA27_IRQn             = 11,               /**< DMA Channel 11, 27 Transfer Complete */
  DMA12_DMA28_IRQn             = 12,               /**< DMA Channel 12, 28 Transfer Complete */
  DMA13_DMA29_IRQn             = 13,               /**< DMA Channel 13, 29 Transfer Complete */
  DMA14_DMA30_IRQn             = 14,               /**< DMA Channel 14, 30 Transfer Complete */
  DMA15_DMA31_IRQn             = 15,               /**< DMA Channel 15, 31 Transfer Complete */
  DMA_Error_IRQn               = 16,               /**< DMA Error Interrupt */
  MCM_IRQn                     = 17,               /**< Normal Interrupt */
  FTFE_IRQn                    = 18,               /**< FTFE Command complete interrupt */
  Read_Collision_IRQn          = 19,               /**< Read Collision Interrupt */
  LVD_LVW_IRQn                 = 20,               /**< Low Voltage Detect, Low Voltage Warning */
  LLWU_IRQn                    = 21,               /**< Low Leakage Wakeup Unit */
  WDOG_EWM_IRQn                = 22,               /**< WDOG Interrupt */
  RNG_IRQn                     = 23,               /**< RNG Interrupt */
  I2C0_IRQn                    = 24,               /**< I2C0 interrupt */
  I2C1_IRQn                    = 25,               /**< I2C1 interrupt */
  SPI0_IRQn                    = 26,               /**< SPI0 Interrupt */
  SPI1_IRQn                    = 27,               /**< SPI1 Interrupt */
  I2S0_Tx_IRQn                 = 28,               /**< I2S0 transmit interrupt */
  I2S0_Rx_IRQn                 = 29,               /**< I2S0 receive interrupt */
  Reserved46_IRQn              = 30,               /**< Reserved interrupt 46 */
  UART0_RX_TX_IRQn             = 31,               /**< UART0 Receive/Transmit interrupt */
  UART0_ERR_IRQn               = 32,               /**< UART0 Error interrupt */
  UART1_RX_TX_IRQn             = 33,               /**< UART1 Receive/Transmit interrupt */
  UART1_ERR_IRQn               = 34,               /**< UART1 Error interrupt */
  UART2_RX_TX_IRQn             = 35,               /**< UART2 Receive/Transmit interrupt */
  UART2_ERR_IRQn               = 36,               /**< UART2 Error interrupt */
  UART3_RX_TX_IRQn             = 37,               /**< UART3 Receive/Transmit interrupt */
  UART3_ERR_IRQn               = 38,               /**< UART3 Error interrupt */
  ADC0_IRQn                    = 39,               /**< ADC0 interrupt */
  CMP0_IRQn                    = 40,               /**< CMP0 interrupt */
  CMP1_IRQn                    = 41,               /**< CMP1 interrupt */
  FTM0_IRQn                    = 42,               /**< FTM0 fault, overflow and channels interrupt */
  FTM1_IRQn                    = 43,               /**< FTM1 fault, overflow and channels interrupt */
  FTM2_IRQn                    = 44,               /**< FTM2 fault, overflow and channels interrupt */
  CMT_IRQn                     = 45,               /**< CMT interrupt */
  RTC_IRQn                     = 46,               /**< RTC interrupt */
  RTC_Seconds_IRQn             = 47,               /**< RTC seconds interrupt */
  PIT0_IRQn                    = 48,               /**< PIT timer channel 0 interrupt */
  PIT1_IRQn                    = 49,               /**< PIT timer channel 1 interrupt */
  PIT2_IRQn                    = 50,               /**< PIT timer channel 2 interrupt */
  PIT3_IRQn                    = 51,               /**< PIT timer channel 3 interrupt */
  PDB0_IRQn                    = 52,               /**< PDB0 Interrupt */
  USB0_IRQn                    = 53,               /**< USB0 interrupt */
  USBDCD_IRQn                  = 54,               /**< USBDCD Interrupt */
  Reserved71_IRQn              = 55,               /**< Reserved interrupt 71 */
  DAC0_IRQn                    = 56,               /**< DAC0 interrupt */
  MCG_IRQn                     = 57,               /**< MCG Interrupt */
  LPTMR0_IRQn                  = 58,               /**< LPTimer interrupt */
  PORTA_IRQn                   = 59,               /**< Port A interrupt */
  PORTB_IRQn                   = 60,               /**< Port B interrupt */
  PORTC_IRQn                   = 61,               /**< Port C interrupt */
  PORTD_IRQn                   = 62,               /**< Port D interrupt */
  PORTE_IRQn                   = 63,               /**< Port E interrupt */
  SWI_IRQn                     = 64,               /**< Software interrupt */
  SPI2_IRQn                    = 65,               /**< SPI2 Interrupt */
  UART4_RX_TX_IRQn             = 66,               /**< UART4 Receive/Transmit interrupt */
  UART4_ERR_IRQn               = 67,               /**< UART4 Error interrupt */
  Reserved84_IRQn              = 68,               /**< Reserved interrupt 84 */
  Reserved85_IRQn              = 69,               /**< Reserved interrupt 85 */
  CMP2_IRQn                    = 70,               /**< CMP2 interrupt */
  FTM3_IRQn                    = 71,               /**< FTM3 fault, overflow and channels interrupt */
  DAC1_IRQn                    = 72,               /**< DAC1 interrupt */
  ADC1_IRQn                    = 73,               /**< ADC1 interrupt */
  I2C2_IRQn                    = 74,               /**< I2C2 interrupt */
  CAN0_ORed_Message_buffer_IRQn = 75,              /**< CAN0 OR'd message buffers interrupt */
  CAN0_Bus_Off_IRQn            = 76,               /**< CAN0 bus off interrupt */
  CAN0_Error_IRQn              = 77,               /**< CAN0 error interrupt */
  CAN0_Tx_Warning_IRQn         = 78,               /**< CAN0 Tx warning interrupt */
  CAN0_Rx_Warning_IRQn         = 79,               /**< CAN0 Rx warning interrupt */
  CAN0_Wake_Up_IRQn            = 80,               /**< CAN0 wake up interrupt */
  SDHC_IRQn                    = 81,               /**< SDHC interrupt */
  ENET_1588_Timer_IRQn         = 82,               /**< Ethernet MAC IEEE 1588 Timer Interrupt */
  ENET_Transmit_IRQn           = 83,               /**< Ethernet MAC Transmit Interrupt */
  ENET_Receive_IRQn            = 84,               /**< Ethernet MAC Receive Interrupt */
  ENET_Error_IRQn              = 85,               /**< Ethernet MAC Error and miscelaneous Interrupt */
  LPUART0_IRQn                 = 86,               /**< LPUART0 status/error interrupt */
  TSI0_IRQn                    = 87,               /**< TSI0 interrupt */
  TPM1_IRQn                    = 88,               /**< TPM1 fault, overflow and channels interrupt */
  TPM2_IRQn                    = 89,               /**< TPM2 fault, overflow and channels interrupt */
  USBHSDCD_IRQn                = 90,               /**< USBHSDCD, USBHS Phy Interrupt */
  I2C3_IRQn                    = 91,               /**< I2C3 interrupt */
  CMP3_IRQn                    = 92,               /**< CMP3 interrupt */
  USBHS_IRQn                   = 93,               /**< USB high speed OTG interrupt */
  CAN1_ORed_Message_buffer_IRQn = 94,              /**< CAN1 OR'd message buffers interrupt */
  CAN1_Bus_Off_IRQn            = 95,               /**< CAN1 bus off interrupt */
  CAN1_Error_IRQn              = 96,               /**< CAN1 error interrupt */
  CAN1_Tx_Warning_IRQn         = 97,               /**< CAN1 Tx warning interrupt */
  CAN1_Rx_Warning_IRQn         = 98,               /**< CAN1 Rx warning interrupt */
  CAN1_Wake_Up_IRQn            = 99                /**< CAN1 wake up interrupt */
} IRQn_Type;
#endif


#if defined(MK60DZ10)
typedef enum                              //中断向量表编号
{
    /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
    NonMaskableInt_VECTORn          = 2 ,       /*!< 2 Non Maskable Interrupt                                                                                    */
    HardFault_VECTORn               = 3 ,       /*!< 3 Hard Fault                                                                                */
    MemoryManagement_VECTORn        = 4 ,       /*!< 4 Cortex-M4 Memory Management Interrupt                                                     */
    BusFault_VECTORn                = 5 ,       /*!< 5 Cortex-M4 Bus Fault Interrupt                                                                     */
    UsageFault_VECTORn              = 6 ,       /*!< 6 Cortex-M4 Usage Fault Interrupt
    */
    SVCall_VECTORn                  = 11,       /*!< 11 Cortex-M4 SV Call Interrupt                                                                      */
    DebugMonitor_VECTORn            = 12,       /*!< 12 Cortex-M4 Debug Monitor Interrupt
    */
    PendSV_VECTORn                  = 14,       /*!< 14 Cortex-M4 Pend SV Interrupt                                                                      */
    SysTick_VECTORn                 = 15,       /*!< 15 Cortex-M4 System Tick Interrupt                                                              */
    /******  Kinetis 60 specific Interrupt Numbers **********************************************************************/
    DMA0_VECTORn                        ,       // DMA Channel 0 Transfer Complete
    DMA1_VECTORn                        ,       // DMA Channel 1 Transfer Complete
    DMA2_VECTORn                        ,       // DMA Channel 2 Transfer Complete
    DMA3_VECTORn                        ,       // DMA Channel 3 Transfer Complete
    DMA4_VECTORn                        ,       // DMA Channel 4 Transfer Complete
    DMA5_VECTORn                        ,       // DMA Channel 5 Transfer Complete
    DMA6_VECTORn                        ,       // DMA Channel 6 Transfer Complete
    DMA7_VECTORn                        ,       // DMA Channel 7 Transfer Complete
    DMA8_VECTORn                        ,       // DMA Channel 8 Transfer Complete
    DMA9_VECTORn                        ,       // DMA Channel 9 Transfer Complete
    DMA10_VECTORn                       ,       // DMA Channel 10 Transfer Complete
    DMA11_VECTORn                       ,       // DMA Channel 11 Transfer Complete
    DMA12_VECTORn                       ,       // DMA Channel 12 Transfer Complete
    DMA13_VECTORn                       ,       // DMA Channel 13 Transfer Complete
    DMA14_VECTORn                       ,       // DMA Channel 14 Transfer Complete
    DMA15_VECTORn                       ,       // DMA Channel 15 Transfer Complete
    DMA_Error_VECTORn                   ,       // DMA Error Interrupt
    MCM_VECTORn                         ,       // Normal Interrupt
    FTFL_VECTORn                        ,       // FTFL Interrupt
    Read_Collision_VECTORn              ,       // Read Collision Interrupt
    LVD_LVW_VECTORn                     ,       // Low Voltage Detect, Low Voltage Warning
    LLW_VECTORn                         ,       // Low Leakage Wakeup
    Watchdog_VECTORn                    ,       // WDOG Interrupt
    RNG_VECTORn                         ,       // RNGB Interrupt
    I2C0_VECTORn                        ,       // I2C0 interrupt
    I2C1_VECTORn                        ,       // I2C1 interrupt
    SPI0_VECTORn                        ,       // SPI0 Interrupt
    SPI1_VECTORn                        ,       // SPI1 Interrupt
    SPI2_VECTORn                        ,       // SPI2 Interrupt
    CAN0_ORed_MB_VECTORn                ,       // CAN0 OR'd Message Buffers Interrupt
    CAN0_Bus_Off_VECTORn                ,       // CAN0 Bus Off Interrupt
    CAN0_Error_VECTORn                  ,       // CAN0 Error Interrupt
    CAN0_Tx_Warning_VECTORn             ,       // CAN0 Tx Warning Interrupt
    CAN0_Rx_Warning_VECTORn             ,       // CAN0 Rx Warning Interrupt
    CAN0_Wake_Up_VECTORn                ,       // CAN0 Wake Up Interrupt
    Reserved51_VECTORn                  ,       // 保留 interrupt 51
    Reserved52_VECTORn                  ,       // 保留 interrupt 52
    CAN1_ORed_MB_VECTORn                ,       // CAN1 OR'd Message Buffers Interrupt
    CAN1_Bus_Off_VECTORn                ,       // CAN1 Bus Off Interrupt
    CAN1_Error_VECTORn                  ,       // CAN1 Error Interrupt
    CAN1_Tx_Warning_VECTORn             ,       // CAN1 Tx Warning Interrupt
    CAN1_Rx_Warning_VECTORn             ,       // CAN1 Rx Warning Interrupt
    CAN1_Wake_Up_VECTORn                ,       // CAN1 Wake Up Interrupt
    Reserved59_VECTORn                  ,       // 保留 interrupt 59
    Reserved60_VECTORn                  ,       // 保留 interrupt 60
    UART0_RX_TX_VECTORn                 ,       // UART0 Receive/Transmit interrupt
    UART0_ERR_VECTORn                   ,       // UART0 Error interrupt
    UART1_RX_TX_VECTORn                 ,       // UART1 Receive/Transmit interrupt
    UART1_ERR_VECTORn                   ,       // UART1 Error interrupt
    UART2_RX_TX_VECTORn                 ,       // UART2 Receive/Transmit interrupt
    UART2_ERR_VECTORn                   ,       // UART2 Error interrupt
    UART3_RX_TX_VECTORn                 ,       // UART3 Receive/Transmit interrupt
    UART3_ERR_VECTORn                   ,       // UART3 Error interrupt
    UART4_RX_TX_VECTORn                 ,       // UART4 Receive/Transmit interrupt
    UART4_ERR_VECTORn                   ,       // UART4 Error interrupt
    UART5_RX_TX_VECTORn                 ,       // UART5 Receive/Transmit interrupt
    UART5_ERR_VECTORn                   ,       // UART5 Error interrupt
    ADC0_VECTORn                        ,       // ADC0 interrupt
    ADC1_VECTORn                        ,       // ADC1 interrupt
    CMP0_VECTORn                        ,       // CMP0 interrupt
    CMP1_VECTORn                        ,       // CMP1 interrupt
    CMP2_VECTORn                        ,       // CMP2 interrupt
    FTM0_VECTORn                        ,       // FTM0 fault, overflow and channels interrupt
    FTM1_VECTORn                        ,       // FTM1 fault, overflow and channels interrupt
    FTM2_VECTORn                        ,       // FTM2 fault, overflow and channels interrupt
    CMT_VECTORn                         ,       // CMT interrupt
    RTC_VECTORn                         ,       // RTC interrupt
    Reserved83_VECTORn                  ,       // 保留 interrupt 83
    PIT0_VECTORn                        ,       // PIT timer channel 0 interrupt
    PIT1_VECTORn                        ,       // PIT timer channel 1 interrupt
    PIT2_VECTORn                        ,       // PIT timer channel 2 interrupt
    PIT3_VECTORn                        ,       // PIT timer channel 3 interrupt
    PDB0_VECTORn                        ,       // PDB0 Interrupt
    USB0_VECTORn                        ,       // USB0 interrupt
    USBDCD_VECTORn                      ,       // USBDCD Interrupt
    ENET_1588_Timer_VECTORn             ,       // Ethernet MAC IEEE 1588 Timer Interrupt
    ENET_Transmit_VECTORn               ,       // Ethernet MAC Transmit Interrupt
    ENET_Receive_VECTORn                ,       // Ethernet MAC Receive Interrupt
    ENET_Error_VECTORn                  ,       // Ethernet MAC Error and miscelaneous Interrupt
    I2S0_VECTORn                        ,       // I2S0 Interrupt
    SDHC_VECTORn                        ,       // SDHC Interrupt
    DAC0_VECTORn                        ,       // DAC0 interrupt
    DAC1_VECTORn                        ,       // DAC1 interrupt
    TSI0_VECTORn                        ,       // TSI0 Interrupt
    MCG_VECTORn                         ,       // MCG Interrupt
    LPTMR_VECTORn                       ,       // LPTimer interrupt
    Reserved102_VECTORn                 ,       // 保留 interrupt 102
    PORTA_VECTORn                       ,       // Port A interrupt
    PORTB_VECTORn                       ,       // Port B interrupt
    PORTC_VECTORn                       ,       // Port C interrupt
    PORTD_VECTORn                       ,       // Port D interrupt
    PORTE_VECTORn                       ,       // Port E interrupt
    Reserved108_VECTORn                 ,       // 保留 interrupt 108
    Reserved109_VECTORn                 ,       // 保留 interrupt 109
    Reserved110_VECTORn                 ,       // 保留 interrupt 110
    Reserved111_VECTORn                 ,       // 保留 interrupt 111
    Reserved112_VECTORn                 ,       // 保留 interrupt 112
    Reserved113_VECTORn                 ,       // 保留 interrupt 113
    Reserved114_VECTORn                 ,       // 保留 interrupt 114
    Reserved115_VECTORn                 ,       // 保留 interrupt 115
    Reserved116_VECTORn                 ,       // 保留 interrupt 116
    Reserved117_VECTORn                 ,       // 保留 interrupt 117
    Reserved118_VECTORn                 ,       // 保留 interrupt 118
    Reserved119_VECTORn                 ,       // 保留 interrupt 119
} VECTORn_t;
#elif defined(MK66F18)
typedef enum                              //中断向量表编号
{
    /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
    NonMaskableInt_VECTORn          = 2 ,       /*!< 2 Non Maskable Interrupt                                                                                    */
    HardFault_VECTORn               = 3 ,       /*!< 3 Hard Fault                                                                                */
    MemoryManagement_VECTORn        = 4 ,       /*!< 4 Cortex-M4 Memory Management Interrupt                                                     */
    BusFault_VECTORn                = 5 ,       /*!< 5 Cortex-M4 Bus Fault Interrupt                                                                     */
    UsageFault_VECTORn              = 6 ,       /*!< 6 Cortex-M4 Usage Fault Interrupt
    */
    SVCall_VECTORn                  = 11,       /*!< 11 Cortex-M4 SV Call Interrupt                                                                      */
    DebugMonitor_VECTORn            = 12,       /*!< 12 Cortex-M4 Debug Monitor Interrupt
    */
    PendSV_VECTORn                  = 14,       /*!< 14 Cortex-M4 Pend SV Interrupt                                                                      */
    SysTick_VECTORn                 = 15,       /*!< 15 Cortex-M4 System Tick Interrupt                                                              */
    /******  Kinetis 60 specific Interrupt Numbers **********************************************************************/
    DMA0_VECTORn                      ,        // DMA Channel 0 Transfer Complete
    DMA1_VECTORn                      ,        // DMA Channel 1 Transfer Complete
    DMA2_VECTORn                      ,        // DMA Channel 2 Transfer Complete
    DMA3_VECTORn                      ,        // DMA Channel 3 Transfer Complete
    DMA4_VECTORn                      ,        // DMA Channel 4 Transfer Complete
    DMA5_VECTORn                      ,        // DMA Channel 5 Transfer Complete
    DMA6_VECTORn                      ,        // DMA Channel 6 Transfer Complete
    DMA7_VECTORn                      ,        // DMA Channel 7 Transfer Complete
    DMA8_VECTORn                      ,        // DMA Channel 8 Transfer Complete
    DMA9_VECTORn                      ,        // DMA Channel 9 Transfer Complete
    DMA10_VECTORn                     ,       // DMA Channel 10 Transfer Complete
    DMA11_VECTORn                     ,       // DMA Channel 11 Transfer Complete
    DMA12_VECTORn                     ,       // DMA Channel 12 Transfer Complete
    DMA13_VECTORn                     ,       // DMA Channel 13 Transfer Complete
    DMA14_VECTORn                     ,       // DMA Channel 14 Transfer Complete
    DMA15_VECTORn                     ,       // DMA Channel 15 Transfer Complete
    DMA_Error_VECTORn                 ,       // DMA Error Interrupt
    MCM_VECTORn                       ,       // Normal Interrupt
    FTFL_VECTORn                      ,       // FTFL Interrupt
    Read_Collision_VECTORn            ,       // Read Collision Interrupt
    LVD_LVW_VECTORn                   ,       // Low Voltage Detect, Low Voltage Warning
    LLW_VECTORn                       ,       // Low Leakage Wakeup
    Watchdog_VECTORn                  ,       // WDOG Interrupt
    RNG_VECTORn                       ,       // RNGB Interrupt
    I2C0_VECTORn                      ,       // I2C0 interrupt
    I2C1_VECTORn                      ,       // I2C1 interrupt
    SPI0_VECTORn                      ,       // SPI0 Interrupt
    SPI1_VECTORn                      ,       // SPI1 Interrupt
    I2S0_VECTORn                      ,       // SPI2 Interrupt
    I2S01_VECTORn                     ,       // CAN0 OR'd Message Buffers Interrupt
    Reserved_VECTORn                  ,       // CAN0 Bus Off Interrupt
    UART0_RX_TX_VECTORn               ,       // CAN0 Error Interrupt
    UART0_VECTORn                     ,       // CAN0 Tx Warning Interrupt
    UART1_RX_TX_VECTORn               ,       // CAN0 Rx Warning Interrupt
    UART1_VECTORn                     ,       // CAN0 Wake Up Interrupt
    UART2_RX_TX_VECTORn               ,       // 保留 interrupt 51
    UART2_VECTORn                     ,
    UART3_RX_TX_VECTORn               ,       // 保留 interrupt 52
    UART3_VECTORn                     ,       // CAN1 OR'd Message Buffers Interrupt
    ADC0_VECTORn                      ,       // CAN1 Bus Off Interrupt
    CMP0_VECTORn                      ,       // CAN1 Error Interrupt
    CMP1_VECTORn                      ,       // CAN1 Tx Warning Interrupt
    FTM0_VECTORn                      ,       // CAN1 Rx Warning Interrupt
    FTM1_VECTORn                      ,       // CAN1 Wake Up Interrupt
    FTM2_VECTORn                      ,       // 保留 interrupt 59
    CMT_VECTORn                       ,       // 保留 interrupt 60
    RTC0_VECTORn                      ,       // UART0 Receive/Transmit interrupt
    RTC1_VECTORn                      ,       // UART0 Error interrupt
    PIT0_VECTORn                      ,       // UART1 Receive/Transmit interrupt
    PIT1_VECTORn                      ,       // UART1 Error interrupt
    PIT2_VECTORn                      ,       // UART2 Receive/Transmit interrupt
    PIT3_VECTORn                      ,       // UART3 Receive/Transmit interrupt
    PDB_VECTORn                       ,       // UART3 Error interrupt
    USBOTG_VECTORn                    ,       // UART4 Receive/Transmit interrupt
    USGCHARGE_VECTORn                 ,       // UART4 Error interrupt
    Reserved1_VECTORn                 ,       // UART5 Receive/Transmit interrupt
    DAC0_VECTORn                      ,       // UART5 Error interrupt
    MCG_VECTORn                       ,       // ADC0 interrupt
    LPTMR_VECTORn                     ,       // ADC1 interrupt
    PORTA_VECTORn                     ,       // CMP0 interrupt
    PORTB_VECTORn                     ,       // CMP1 interrupt
    PORTC_VECTORn                     ,       // CMP2 interrupt
    PORTD_VECTORn                     ,       // FTM0 fault, overflow and channels interrupt
    PORTE_VECTORn                     ,       // FTM1 fault, overflow and channels interrupt
    SOFT_VECTORn                      ,       // FTM2 fault, overflow and channels interrupt
    SPI2_VECTORn                      ,       // CMT interrupt
    UART4_RX_TX_VECTORn               ,       // RTC interrupt
    UART4_VECTORn                     ,       // 保留 interrupt 83
    Reserved2_VECTORn                 ,       // PIT timer channel 0 interrupt
    Reserved3_VECTORn                 ,       // PIT timer channel 1 interrupt
    CMP2_VECTORn                      ,       // PIT timer channel 2 interrupt
    FTM3_VECTORn                      ,       // PIT timer channel 3 interrupt
    DAC1_VECTORn                      ,       // PDB0 Interrupt
    ADC1_VECTORn                      ,       // USB0 interrupt
    I2C2_VECTORn                      ,       // USBDCD Interrupt
    CAN01_VECTORn                     ,       // Ethernet MAC IEEE 1588 Timer Interrupt
    CAN02_VECTORn                     ,       // Ethernet MAC Transmit Interrupt
    CAN03_VECTORn                     ,       // Ethernet MAC Receive Interrupt
    CAN04_VECTORn                     ,       // Ethernet MAC Error and miscelaneous Interrupt
    CAN05_VECTORn                     ,       // I2S0 Interrupt
    CAN06_VECTORn                     ,       // SDHC Interrupt
    SDCH_VECTORn                      ,       // DAC0 interrupt
    MAC0_VECTORn                      ,       // DAC1 interrupt
    MAC1_VECTORn                      ,       // TSI0 Interrupt
    MAC2_VECTORn                      ,       // MCG Interrupt
    MAC3_VECTORn                      ,       // LPTimer interrupt
    LPUART0_VECTORn                   ,       // 保留 interrupt 102
    TSI0_VECTORn                      ,       // Port A interrupt
    TPM1_VECTORn                      ,       // Port B interrupt
    TPM2_VECTORn                      ,       // Port C interrupt
    USBHS_VECTORn                     ,       // Port D interrupt
    I2C3_VECTORn                      ,       // Port E interrupt
    CMP3_VECTORn                      ,       // 保留 interrupt 108
    USBOTG1_VECTORn                   ,       // 保留 interrupt 109
    CAN10_VECTORn                     ,       // 保留 interrupt 110
    CAN11_VECTORn                     ,       // 保留 interrupt 111
    CAN12_VECTORn                     ,       // 保留 interrupt 112
    CAN13_VECTORn                     ,       // 保留 interrupt 113
    CAN14_VECTORn                     ,       // 保留 interrupt 114
    CAN15_VECTORn                     ,       // 保留 interrupt 115
    Reserved116_VECTORn               ,      // 保留 interrupt 116
    Reserved117_VECTORn               ,      // 保留 interrupt 117
    Reserved118_VECTORn               ,      // 保留 interrupt 118
    Reserved119_VECTORn               ,      // 保留 interrupt 119
} VECTORn_t;
#endif
///////////////////////////////////////////////中断服务函数重写
extern void write_vtor(int vtor);
extern void set_vector_handler(VECTORn_t vector , void pfunc_handler(void));
extern __ramfunc void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4);
#endif
