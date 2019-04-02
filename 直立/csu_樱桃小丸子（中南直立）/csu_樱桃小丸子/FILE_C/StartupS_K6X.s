/*****************************************************/      
/*Auther: 中南大学黄竞辉                             */       
/*DATE  ： 2016/12/01                                */      
/*DISCRIBE:Kinetis MK6X系列启动汇编文件(不带硬件浮点)*/     
/*****************************************************/    
 
       SECTION .noinit : CODE        
       EXPORT  Reset_Handler    ;声明Reset复位函数
Reset_Handler
       MOV r1, #0
       MOV r2, #0
       MOV r3, #0
       MOV r4, #0
       MOV r5, #0
       MOV r6, #0
       MOV r7, #0
       MOV r8, #0
       MOV r9, #0
       MOV r10, #0
       MOV r11, #0
       MOV r12, #0                ;清空通用寄存器
       CPSIE   i
       
       LDR.W   R0,=0xE000ED88          ; //开启FPU浮点单元，具体查看ARM_CORTEX-M4核心手册，即将CPACR[23:20]置1
       LDR     R1,[R0]                 ;
       ORR     R1,R1, #(0xF << 20)     ;
       STR     R1,[R0]                 ;
       
       
       import start                   ;从文件中调用start启动C函数
       BL     start                    ;转到start
__done
       B       __done       
       END