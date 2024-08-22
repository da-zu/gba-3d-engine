#ifndef _INCAGB_
#define _INCAGB_

typedef     unsigned char           u8;
typedef     unsigned short int      u16;
typedef     unsigned int            u32;
typedef     unsigned long long int  u64;

typedef     signed char             s8;
typedef     signed short int        s16;
typedef     signed int              s32;
typedef     signed long long int    s64;

typedef     float                   f32;
typedef     double                  f64;


#define     vl                      volatile

typedef     vl u8                   vu8;
typedef     vl u16                  vu16;
typedef     vl u32                  vu32;
typedef     vl u64                  vu64;

typedef     vl s8                   vs8;
typedef     vl s16                  vs16;
typedef     vl s32                  vs32;
typedef     vl s64                  vs64;

typedef     vl f32                  vf32;
typedef     vl f64                  vf64;

#define REG_BASE                0x04000000          // Registers
#define VRAM                    0x06000000          // VRAM


#define REG_WAITCNT     (REG_BASE + 0x204)  // Game Pak Wait Control

#define CST_ROM0_1ST_3WAIT      0x0004      //                 3 wait
#define CST_ROM0_2ND_1WAIT      0x0010      //                 1 wait
#define CST_PREFETCH_ENABLE     0x4000      // Enable prefetch buffer

#define OBJ_MODE4_VRAM          (VRAM +    0x14000)
#define OBJ_MODE4_VRAM_SIZE     0x04000

#define A_BUTTON                0x0001      // A Button
#define B_BUTTON                0x0002      // B Button
#define SELECT_BUTTON           0x0004      // SELECT
#define START_BUTTON            0x0008      // START
#define R_KEY                   0x0010      // Right Button
#define L_KEY                   0x0020      // Left Button
#define U_KEY                   0x0040      // Up Button
#define D_KEY                   0x0080      // Down Button
#define R_BUTTON                0x0100      // R Button
#define L_BUTTON                0x0200      // L Button

#define REG_KEYINPUT    (REG_BASE + 0x130)  // Key Input
#define REG_KEYCNT      (REG_BASE + 0x132)  // Key Control

#define V_BLANK_INTR_FLAG       0x0001      // V Blank Interrupt
#define H_BLANK_INTR_FLAG       0x0002      // H Blank Interrupt
#define V_COUNT_INTR_FLAG       0x0004      // V Counter Corresponding Interrupt
#define TIMER0_INTR_FLAG        0x0008      // Timer 0 Interrupt
#define TIMER1_INTR_FLAG        0x0010      // Timer 1 Interrupt
#define TIMER2_INTR_FLAG        0x0020      // Timer 2 Interrupt
#define TIMER3_INTR_FLAG        0x0040      // Timer 3 Interrupt
#define SIO_INTR_FLAG           0x0080      // Serial Communication Interrupt
#define DMA0_INTR_FLAG          0x0100      // DMA0 Interrupt
#define DMA1_INTR_FLAG          0x0200      // DMA1 Interrupt
#define DMA2_INTR_FLAG          0x0400      // DMA2 Interrupt
#define DMA3_INTR_FLAG          0x0800      // DMA3 Interrupt
#define KEY_INTR_FLAG           0x1000      // Key Interrupt
#define CASSETTE_INTR_FLAG      0x2000      // Game Pak Interrupt


#define REG_IME         (REG_BASE + 0x208)  // Interrupt Master Enable
#define REG_IE          (REG_BASE + 0x200)  // Interrupt Enable
#define REG_IF          (REG_BASE + 0x202)  // Interrupt Request

#define EX_WRAM                 0x02000000          // CPU External Work RAM
#define EX_WRAM_SIZE            0x40000             // CPU External Work RAM

#define CPU_WRAM                0x03000000          // CPU Internal Work RAM
#define CPU_WRAM_SIZE           0x8000              // CPU Internal Work RAM

#define VRAM_SIZE               0x18000             // VRAM

#define OAM                     0x07000000          // OAM

#define PLTT                    0x05000000          // Palette RAM
#define PLTT_SIZE               (2*256*2)           // Palette RAM

#define REG_TM0CNT      (REG_BASE + 0x100)  // Timer 0
#define REG_TM0CNT_L    (REG_BASE + 0x100)
#define REG_TM0CNT_H    (REG_BASE + 0x102)
#define REG_TM1CNT      (REG_BASE + 0x104)  // Timer 1
#define REG_TM1CNT_L    (REG_BASE + 0x104)
#define REG_TM1CNT_H    (REG_BASE + 0x106)
#define REG_TM2CNT      (REG_BASE + 0x108)  // Timer 2
#define REG_TM2CNT_L    (REG_BASE + 0x108)
#define REG_TM2CNT_H    (REG_BASE + 0x10a)
#define REG_TM3CNT      (REG_BASE + 0x10c)  // Timer 3
#define REG_TM3CNT_L    (REG_BASE + 0x10c)
#define REG_TM3CNT_H    (REG_BASE + 0x10e)

#define BG_PLTT                 (PLTT +        0x0) // BG Palette RAM
#define OBJ_PLTT                (PLTT +      0x200) // OBJ Palette RAM

#define REG_STAT        (REG_BASE + 0x4)    // Status

#define STAT_V_BLANK_IF_ENABLE  0x0008      // V Blank Interrupt Request, Enable
#define STAT_H_BLANK_IF_ENABLE  0x0010      // H Blank Interrupt Request, Enable

#define TMR_PRESCALER_1CK       0x00000000  // Prescaler 1 Clock
#define TMR_PRESCALER_64CK      0x00010000  //            64 clocks
#define TMR_PRESCALER_256CK     0x00020000  //          256 clocks
#define TMR_PRESCALER_1024CK    0x00030000  //        1024 clocks
#define TMR_IF_ENABLE           0x00400000  // Interrupt Request Enable
#define TMR_ENABLE              0x00800000  // Run Timer

#define REG_DISPCNT     (REG_BASE + 0x0)    // Display Control

#define DISP_MODE_4             0x0004      // BG Mode 4
#define DISP_BG2_ON             0x0400      // BG2 ON
#define DISP_OBJ_CHAR_1D_MAP    0x0040      // OBJ Character Data 1D Mapping
#define DISP_OBJ_ON             0x1000      // OBJ ON

#define REG_WINCNT      (REG_BASE + 0x40)   // Window Control
#define REG_WIN0H       (REG_BASE + 0x40)   // Window 0 Horizontal Area
#define REG_WIN1H       (REG_BASE + 0x42)   // Window 1 Horizontal Area
#define REG_WIN0V       (REG_BASE + 0x44)   // Window 0 Vertical Area
#define REG_WIN1V       (REG_BASE + 0x46)   // WIndow 1 Vertical Area
#define REG_WININ       (REG_BASE + 0x48)   // Internal Window Control
#define REG_WIN0        (REG_BASE + 0x48)   // Window 0 Control
#define REG_WIN1        (REG_BASE + 0x49)   // Window 1 Control
#define REG_WINOUT      (REG_BASE + 0x4a)   // External Window Control
#define REG_OBJWIN      (REG_BASE + 0x4b)   // OBJ Window Control

#define WIN_ALL_ON              0x003f      // All ON
#define DISP_WIN0_ON            0x2000      // Window 0 ON

#define BG_BITMAP0_VRAM         (VRAM +        0x0) // BG Bitmap 0 RAM
#define BG_BITMAP1_VRAM         (VRAM +     0xa000) // BG Bitmap 1 RAM

#define REG_VCOUNT      (REG_BASE + 0x6)    // V Counter

#define DMA_TIMMING_MASK        0x30000000  // Start Timing
#define DMA_COUNT_MASK          0x0000ffff  // Transmission Count

#define DMA_TIMMING_SHIFT       30
#define DMA_COUNT_SHIFT         0

#define DMA_ENABLE              0x80000000  // DMA Enable
#define DMA_IF_ENABLE           0x40000000  // Interrupt Request Enable
#define DMA_TIMMING_IMM         0x00000000  // Run Immediately
#define DMA_TIMMING_V_BLANK     0x10000000  // Run V Blank
#define DMA_TIMMING_H_BLANK     0x20000000  // Run H Blank
#define DMA_TIMMING_DISP        0x30000000  // Run Display
#define DMA_TIMMING_SOUND       0x30000000  // Run Sound FIFO Request
#define DMA_DREQ_ON             0x08000000  // Data Request Synchronize Mode ON
#define DMA_16BIT_BUS           0x00000000  // Select Bus Size 16Bit
#define DMA_32BIT_BUS           0x04000000  // Select Bus Size 32Bit
#define DMA_CONTINUOUS_ON       0x02000000  // Continuous Mode ON
#define DMA_SRC_INC             0x00000000  // Select Source Increment
#define DMA_SRC_DEC             0x00800000  // Select Source Decrement
#define DMA_SRC_FIX             0x01000000  // Select Source Fixed
#define DMA_DEST_INC            0x00000000  // Select Destination Increment
#define DMA_DEST_DEC            0x00200000  // Select Destination Decrement
#define DMA_DEST_FIX            0x00400000  // Select Destination Fixed
#define DMA_DEST_RELOAD         0x00600000  // Select Destination 
                                            // Increment/Reload
#define REG_DMA0        (REG_BASE + 0xb0)   // DMA 0
#define REG_DMA0SAD     (REG_BASE + 0xb0)   // (Internal Memory -> 
                                            // Internal Memory)
#define REG_DMA0SAD_L   (REG_BASE + 0xb0)
#define REG_DMA0SAD_H   (REG_BASE + 0xb2)
#define REG_DMA0DAD     (REG_BASE + 0xb4)
#define REG_DMA0DAD_L   (REG_BASE + 0xb4)
#define REG_DMA0DAD_H   (REG_BASE + 0xb6)
#define REG_DMA0CNT     (REG_BASE + 0xb8)
#define REG_DMA0CNT_L   (REG_BASE + 0xb8)
#define REG_DMA0CNT_H   (REG_BASE + 0xba)
#define REG_DMA1        (REG_BASE + 0xbc)   // DMA 1
#define REG_DMA1SAD     (REG_BASE + 0xbc)   // (Internal Memory -> Sound 
                                            // FIFO/Internal Memory)
#define REG_DMA1SAD_L   (REG_BASE + 0xbc)
#define REG_DMA1SAD_H   (REG_BASE + 0xbe)
#define REG_DMA1DAD     (REG_BASE + 0xc0)
#define REG_DMA1DAD_L   (REG_BASE + 0xc0)
#define REG_DMA1DAD_H   (REG_BASE + 0xc2)
#define REG_DMA1CNT     (REG_BASE + 0xc4)
#define REG_DMA1CNT_L   (REG_BASE + 0xc4)
#define REG_DMA1CNT_H   (REG_BASE + 0xc6)
#define REG_DMA2        (REG_BASE + 0xc8)   // DMA 2
#define REG_DMA2SAD     (REG_BASE + 0xc8)   // (Internal Memory -> Sound 
                                            // FIFO/Internal Memory)
#define REG_DMA2SAD_L   (REG_BASE + 0xc8)
#define REG_DMA2SAD_H   (REG_BASE + 0xca)
#define REG_DMA2DAD     (REG_BASE + 0xcc)
#define REG_DMA2DAD_L   (REG_BASE + 0xcc)
#define REG_DMA2DAD_H   (REG_BASE + 0xce)
#define REG_DMA2CNT     (REG_BASE + 0xd0)
#define REG_DMA2CNT_L   (REG_BASE + 0xd0)
#define REG_DMA2CNT_H   (REG_BASE + 0xd2)
#define REG_DMA3        (REG_BASE + 0xd4)   // DMA 3
#define REG_DMA3SAD     (REG_BASE + 0xd4)   // (Game Pak/Internal Memory <--> 
                                            // Internal Memory)
#define REG_DMA3SAD_L   (REG_BASE + 0xd4)
#define REG_DMA3SAD_H   (REG_BASE + 0xd6)
#define REG_DMA3DAD     (REG_BASE + 0xd8)
#define REG_DMA3DAD_L   (REG_BASE + 0xd8)
#define REG_DMA3DAD_H   (REG_BASE + 0xda)
#define REG_DMA3CNT     (REG_BASE + 0xdc)
#define REG_DMA3CNT_L   (REG_BASE + 0xdc)
#define REG_DMA3CNT_H   (REG_BASE + 0xde)

/*------------------------------------------------------------------*/
/*                            Set DMA                               */
/*------------------------------------------------------------------*/

//#define SIMULATOR
#ifndef SIMULATOR
#ifndef CODE32
#define DmaSet(DmaNo, Srcp, Destp, DmaCntData)          \
{                                                       \
    vu32 *(DmaCntp) = (vu32 *)REG_DMA##DmaNo;           \
    DmaCntp[0] = (vu32 )(Srcp);                         \
    DmaCntp[1] = (vu32 )(Destp);                        \
    DmaCntp[2] = (vu32 )(DmaCntData);                   \
    DmaCntp[2];                                         \
}
#else
#define DmaSet(DmaNo, Srcp, Destp, DmaCntData)                   \
{                                                                \
    vu32 *(RegBasep) = (vu32 *)REG_BASE;                         \
    RegBasep[OFFSET_REG_DMA##DmaNo##SAD/4] = (vu32 )(Srcp);      \
    RegBasep[OFFSET_REG_DMA##DmaNo##DAD/4] = (vu32 )(Destp);     \
    RegBasep[OFFSET_REG_DMA##DmaNo##CNT/4] = (vu32 )(DmaCntData);\
    RegBasep[OFFSET_REG_DMA##DmaNo##CNT/4];                      \
}

#endif
#else
#define DmaSet(DmaNo, Srcp, Destp, DmaCntData)                   \
{                                                                \
    int    i;                                                    \
    for (i=0; i<(DmaCntData & 0x1ffff); i++)                     \
        if ((DmaCntData) & DMA_SRC_FIX) {                        \
            if ((DmaCntData) & DMA_32BIT_BUS)                    \
                    ((vu32 *)(Destp))[i] = ((vu32 *)(Srcp))[0];  \
            else    ((vu16 *)(Destp))[i] = ((vu16 *)(Srcp))[0];  \
         } else {                                                \
            if ((DmaCntData) & DMA_32BIT_BUS)                    \
                ((vu32 *)(Destp))[i] = ((vu32 *)(Srcp))[i];      \
            else    ((vu16 *)(Destp))[i] = ((vu16 *)(Srcp))[i];  \
        }                                                        \
}
#endif

#define DmaClear(DmaNo, Data, Destp, Size, Bit)             \
{                                                           \
    vu##Bit Tmp = (vu##Bit )(Data);                         \
    DmaSet(DmaNo, &Tmp, Destp, (                            \
        DMA_ENABLE         | DMA_TIMMING_IMM |              \
        DMA_SRC_FIX        | DMA_DEST_INC    |              \
        DMA_##Bit##BIT_BUS | ((Size)/(Bit/8))));            \
}

#define DmaArrayClear(  DmaNo, Data, Destp, Bit)            \
        DmaClear(       DmaNo, Data, Destp, sizeof(Destp), Bit)


#define DmaCopy(DmaNo, Srcp, Destp, Size, Bit)              \
                                                            \
    DmaSet(DmaNo, Srcp, Destp,  (                           \
        DMA_ENABLE         | DMA_TIMMING_IMM |              \
        DMA_SRC_INC        | DMA_DEST_INC    |              \
        DMA_##Bit##BIT_BUS | ((Size)/((Bit)/8))))


#define DmaArrayCopy(  DmaNo, Srcp, Destp, Bit)             \
        DmaCopy(       DmaNo, Srcp, Destp, sizeof(Srcp), Bit)


#define DmaWait(DmaNo)                                      \
{                                                           \
    vu32 *(DmaCntp) = (vu32 *)REG_DMA##DmaNo;               \
    while (DmaCntp[2] & DMA_ENABLE) ;                       \
}


#ifdef  __GNUC__
#ifndef CODE32
#define SystemCall(No)   asm ("SWI    "#No)
#else
#define SystemCall(No)   asm ("SWI    "#No" << 16")
#endif
#else
#ifndef CODE32
#define SystemCall(No) __asm { SWI    No}
#else
#define SystemCall(No) __asm { SWI    No << 16}
#endif
#endif

#define Halt()                   SystemCall(2)

/*------------------------------------------------------------------*/
/*                      Blend Control                               */
/*------------------------------------------------------------------*/

#define BLD_PIXEL_MASK          0x003f      // Select Pixel
#define BLD_1ST_PIXEL_MASK      0x003f      // Select 1st Pixel
#define BLD_MODE_MASK           0x00c0      // Blend Mode
#define BLD_2ND_PIXEL_MASK      0x3f00      // Select 2nd Pixel

#define BLD_1ST_PIXEL_SHIFT     0
#define BLD_MODE_SHIFT          6
#define BLD_2ND_PIXEL_SHIFT     8

#define BLD_BG0                 0x0001      // Select BG0 Pixel
#define BLD_BG1                 0x0002      // Select BG1 Pixel
#define BLD_BG2                 0x0004      // Select BG2 Pixel
#define BLD_BG3                 0x0008      // Select BG3 Pixel
#define BLD_OBJ                 0x0010      // Select OBJ Pixel
#define BLD_BD                  0x0020      // Select Background Color Pixel
#define BLD_ALL                 0x003f      // Select All Pixels
#define BLD_BG0_1ST             0x0001      // Select BG0 1st Pixel
#define BLD_BG1_1ST             0x0002      // Select BG1 1st Pixel
#define BLD_BG2_1ST             0x0004      // Select BG2 1st Pixel
#define BLD_BG3_1ST             0x0008      // Select BG3 1st Pixel
#define BLD_OBJ_1ST             0x0010      // Select OBJ 1st Pixel
#define BLD_BD_1ST              0x0020      // Select Background Color 1st Pixel
#define BLD_1ST_ALL             0x003f      // Select All 1st Pixel
#define BLD_NORMAL_MODE         0x0000      // Normal Mode
#define BLD_A_BLEND_MODE        0x0040      // Semi-transparent Mode
#define BLD_UP_MODE             0x0080      // Brightness UP Mode
#define BLD_DOWN_MODE           0x00c0      // Brightness DOWN Mode
#define BLD_BG0_2ND             0x0100      // Select BG0 2nd Pixel
#define BLD_BG1_2ND             0x0200      // Select BG1 2nd Pixel
#define BLD_BG2_2ND             0x0400      // Select BG2 2nd Pixel
#define BLD_BG3_2ND             0x0800      // Select BG3 2nd Pixel
#define BLD_OBJ_2ND             0x1000      // Select OBJ 2nd Pixel
#define BLD_BD_2ND              0x2000      // Select Background color 2nd Pixel
#define BLD_2ND_ALL             0x3f00      // Select All 2nd Pixel


#define BLD_A_MASK              0x001f      // Blend Variable A
#define BLD_B_MASK              0x1f00      // Blend Variable B
#define BLD_Y_MASK              0x001f      // Blend Variable Y

#define BLD_A_SHIFT             16
#define BLD_B_SHIFT             24
#define BLD_Y_SHIFT             0

#define REG_BLDCNT      (REG_BASE + 0x50)   // Blend Mode Control
#define REG_BLDALPHA    (REG_BASE + 0x52)   // Semi-transparent Parameter
#define REG_BLDY        (REG_BASE + 0x54)   // Brightness Change Parameter

#endif
