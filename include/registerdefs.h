/** Written by Burton O Sumner With guidance from USC Columbia's Dr. William Hoskins and the Tonc GBA-Dev Libraries (https://www.coranac.com/tonc/text/toc.htm) */

//DEFINE REGISTERS AND THEIR ASSOCIATED VALID VALUES AS #DEF CONSTANTS AND MACROS HERE.
//240x160
//ARM7TDMI 16.78 MHz
//Color word is 16bits
//0bXbbbbbgggggrrrrr
//0x00000000 -> 0x00003fff (System ROM, exe but not readable)
//0x02000000 -> 0x02020000 (External RAM, 256 KB)
//0x03000000 -> 0x03007fff (Internal RAM, 32 KB)
//0x04000000 -> 0x04003fff (IO registers)
//0x05000000 -> 0x05003fff (Color Palette)
//0x06000000 -> 0x06017fff (VRAM 96 KB)
//0x07000000 -> 0x070003ff (OAM RAM)
//0x08000000 -> 0x???????? (Gamepak ROM up to 32MB)
//0x0E000000 -> 0x???????? (Gamepak RAM)







#ifndef REGISTERDEFS_H
#define REGISTERDEFS_H


//! Put variable in IWRAM (default).
#define IWRAM_DATA __attribute__((section(".iwram")))

//! Put variable in EWRAM.
#define EWRAM_DATA __attribute__((section(".ewram")))

//! Put <b>non</b>-initialized variable in EWRAM.
#define  EWRAM_BSS __attribute__((section(".sbss")))

//! Put function in IWRAM.
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))

//! Put function in EWRAM.
#define EWRAM_CODE __attribute__((section(".ewram"), long_call))


#define ALIGN(n)    __attribute__((aligned(n)))

#define ALIGN4		__attribute__((aligned(4)))

#define PACKED		__attribute__((packed))




#define SCREEN_WIDTH                            240 //Define bakes these constants into the instructions, vs declaring consts which takes up valuable memory.
#define SCREEN_HEIGHT                           160

//#define IWRAM

#define MEMIO                                   0x04000000
#define VRAM                                    0x06000000  

#define VIDEO_MODE                              0x0003
#define BG_MODE                                 0x0400

#define REG_DISPLAY                             (* (volatile uint32 *) (MEMIO)) //IO Register for video out.
#define REG_DISPLAY_VCOUNT                      (* (volatile uint32 *) (MEMIO + 0x0006)) //IO Register for vcount (used to find out what pixel is being drawn at the moment)
#define REG_DISPLAY_INPUT                       (* (volatile uint32 *) (MEMIO + 0x0130)) //IO Register for input

/**
 * @brief Defining Sound Out Master Control Registers, valid modes for these registers, and functions using bitwise ops to organize the bits to write to these regs.
 * WRITTEN WITH GREAT HELP FROM TONC LIBRARY WEBSITE'S WEBPAGE ON GBA AUDIO PROGRAMMING. 
 * [Credit/Citation]: https://www.coranac.com/tonc/text/sndsqr.htm
 * 
 */

/*
 * REG_SOUND_DMG_COUNT Offset from IO Registers = 0x0080.
 * Bit number   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 * Designations |R4|R3|R2|R1|L4|L3|L2|L1|- |   RV   |- |   LV   |
 * R4-R1 = Right speaker sound channel mode
 * L4-L1 = Left speaker sound channel mode
 * (L/R)V = (Left/Right) speaker volume
 * 
 */
#define REG_SOUND_DMG_COUNT (* (volatile uint16 *) (MEMIO + 0x0080))
#define SOUND_DMG_SQUARE_1  0x01
#define SOUND_DMG_SQUARE_2  0x02
#define SOUND_DMG_WAVE      0x04
#define SOUND_DMG_NOISE     0x08


//Macro to spit out an integer formatted to be placed into SOUND_DMG_COUNT register in a way the GBA can interpret.
#define SOUND_DMG_BUILD(_left_mode, _right_mode, _left_volume, _right_volume)   \
                ( ((_left_volume) & 7) | (((_right_volume) & 7) << 4) | ((_left_mode) << 8) | ((_right_mode)<< 12)  )

//Macro that uses SOUND_DMG_BUILD(...), but for mono sounds (i.e same mode and volume for both left and right speaker.) 
#define SOUND_DMG_BUILD_LR(_mode, _vol) SOUND_DMG_BUILD(_mode, _mode, _vol, _vol)

/* REG_SOUND_DS_COUNT offset from IO Register by 0x082
 * Bit Number   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 * Designations |BF|BT|BL|BR|AF|AT|AL|AR|   -    |BV|AV|DMG Vol.|
 * (A/B)(F/T/L/R) = Direct Sound (A/B)(FIFO Reset/Timer/Left Enable/Right Enable) Respectively
 * (A/B)V = Direct Sound (A/B) Volume ratio (50% if clear | 100% if set)
 * 
 */
#define REG_SOUND_DS_COUNT  (* (volatile uint16 *) (MEMIO + 0x0082))
#define DMG_VOL_25          0x0000
#define DMG_VOL_50          0x0001
#define DMG_VOL_100         0x0002
// 0b11                    (0x0011) FORBIDDEN


/* REG_SOUND_STAT Offset from IO register by 0x084
 * Bit Number   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 * Designations |           -           |ME|   --   |4A|3A|2A|1A|
 * ME = Master Sound Enable
 * 4/3/2/1)A = (SQUARE1)
 * 
 */
#define REG_SOUND_STAT          (* (volatile uint16 *) (MEMIO + 0x0084))
#define MASTER_SOUND_ENABLE     0x0080

/* REG_SOUND1_SWEEP
 * Bit Number   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 * Designations |             -            |    T   | M|    N   |
 * T = Step-time: Time between sweeps measured at 128 Hz Therefore DeltaTime = T/128ms. If T = 0, sweep disabled.
 * M = Sweep mode: Sweep can either move rate up or down. (up is default)
 * N = Sweep shift number: Amount of bits to shift the Step Time until rate reaches either 0 or the max of 2047.
 * 
 */
#define REG_SOUND1_SWEEP        (* (volatile uint16 *) (MEMIO + 0x0060))
#define SOUND1_SWEEP_OFF         0x0008

/* REG_SOUND1_COUNT
 * Bit Number   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|

 * Designations |    EIV    |ED|   EST  |  D  |        L        |
 * EIV = Envelope initial value, kind of akin to a volume setting. 0 = silent; 15 = full blast.
 * ED = Envelope direction; whether envelope decreases or increases with each step. (decreases by default).
 * EST = Envelope step-time: time between envelope changes such that DeltaT = EST/64sec
 * D = Duty cycle (ratio between on and off times of the sqr wave)
 * L = Sound length (Write-only. Only works if channel is timed)
 * 
 */
#define REG_SOUND1_COUNT        (* (volatile uint16 *) (MEMIO + 0x0062))
#define SOUND_SQUARE_DUTY1_8TH  0
#define SOUND_SQUARE_DUTY1_4TH  0x0040
#define SOUND_SQUARE_DUTY1_2ND  0x0080
#define SOUND_SQUARE_DUTY3_4TH  0x00C0



/* REG_SOUND1_FREQUENCY
 * Bit Number   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 * Designations |Re|T |   --   |                R               |
 * RE = Sound rate = (the initial rate before sweep).
 * T = Timed flag = Whether or not It is timed, affects whether or not S1_COUNT bits 0-6 are used, as those are the sound length bits.
 * Re = Reset flag = Resets the sound to the initial volume (and sweep) settings.
 * 
 */
#define REG_SOUND1_FREQUENCY    (* (volatile uint16 *) (MEMIO + 0x0064))
#define SOUND_FREQUENCY_RESET   0x8000
#define SOUND_TIMED             0x4000

/**
 * @brief Register builder for REG_SOUND1_SWEEP
 * Validity Ranges:
 * 0 <= _shift      <= 7 (i.e 3 bits)
 * 0 <= _direction  <= 1 (i.e 1 bits)
 * 0 <= _time       <= 7 (i.e 3 bits)
 * ALT:
 * Or just turn it off with SOUND1_SWEEP_OFF
 * */
#define S1_SWEEP_BUILD(_shift, _direction, _time) \
    ( ((_time & 7) << 4) | ((_direction & 1) << 3) | ((_shift) & 7) )

/**
 * @brief Register builder for envelope of REG_SOUND(1/2/4)_COUNT
 * _env_vol             = Envelope volume                       (4 bits     (0-15))
 * _env_dir             = Envelope direction                    (single bit (0-1))
 * _env_max_steptime    = Envelope maximum step time b4 cutoff  (3 bits     (0-7))
 *
 * */
#define SOUND_SQUARE_ENVELOPE_BUILD(_env_vol, _env_dir, _env_max_steptime)  \
( ((_env_vol & 15) << 12) | ((_env_dir & 1) << 11) | ((_env_max_steptime & 7) << 8) )

/** 
 * @brief Register builder for REG_SOUND(1/2/4)_COUNT
 * __env_X = Plugs into SOUND_SQUARE_ENVELOPE_BUILD
 * _duty 
 *
 * */
#define SOUND_SQUARE_BUILD(__env_vol, __env_dir, __env_max_steptime, _duty, _length) \
    ( SOUND_SQUARE_ENVELOPE_BUILD(__env_vol, __env_dir, __env_max_steptime) | ((_duty & 3) << 6 ) | (_length & 63) )


/**
 * @brief Register builder for REG_SOUND(1/2/3)_FREQUENCY
 * */
#define SOUND_FREQUENCY_BUILD(_rate, _timed, _reset) \
    ( (rate & 0x7FF) | ((timed & 1) << 14) | ((reset & 1) << 15) )


/** 
 * @brief Register builder for REG_SOUND4_FREQUENCY
 * */
#define SOUND_NOISE_FRQ_BUILD(_timed, _counter_prestep_freq, _counter_stages, _div_freq) \
    (((_timed & 1) << 14) | ((_counter_prestep_freq & 15) << 4) | ((_counter_stages & 1) << 3) | (_div_freq & 7) )




#define REG_SOUND2_COUNT        (* (volatile uint16 *) (MEMIO + 0x0068))
#define REG_SOUND2_FREQUENCY    (* (volatile uint16 *) (MEMIO + 0x006C))

#define REG_SOUND3_SELECT       (* (volatile uint16 *) (MEMIO + 0x0070))
#define REG_SOUND3_COUNT        (* (volatile uint16 *) (MEMIO + 0x0072))
#define REG_SOUND3_FREQUENCY    (* (volatile uint16 *) (MEMIO + 0x0074))

#define REG_SOUND4_COUNT        (* (volatile uint16 *) (MEMIO + 0x0078))
#define REG_SOUND4_FREQUENCY    (* (volatile uint16 *) (MEMIO + 0x007C))




typedef enum
{
    C_NOTE=0,   CS_NOTE,    D_NOTE,     DS_NOTE,
    E_NOTE,     F_NOTE,     FS_NOTE,    G_NOTE,
    GS_NOTE,    A_NOTE,     AS_NOTE,    B_NOTE
} enumerableSoundNote;

//Sound rates for notes at the 5th octave in traditional music
static const uint32 __sound_rates[12] = 
{
    8013, 7566, 7144, 6742, // C , C#, D , D#
    6362, 6005, 5666, 5346, // E , F , F#, G
    5048, 4766, 4499, 4246  // G#, A , A#, B
};

//Example usage : REG_SOUND1_FREQUENCY = SOUND_FREQUENCY_RESET | SND_RATE(NOTE_A, 0);
#define SOUND_RATE(note, octave) ( 2048-(__sound_rates[note]>>(4+(octave))) )







/* Input register active low.
 * 1 1 1 1 1 1 1 1 1 1
 * _ _ _ _ _ _ _ _ _ _
 *             ... B A
 */

/* If A being pressed
 * 1 1 1 1 1 1 1 1 1 0
 * &                 1
 * _ _ _ _ _ _ _ _ _ _
 * 0 0 0 0 0 0 0 0 0 0 
 *             ... B A
 * If Input Reg value&1==0, we know A is being pressed. (Basic gist of masking.)
 */

#define A       1
#define B       2
#define SELECT  4
#define START   8
#define RIGHT   16
#define LEFT    32
#define UP      64
#define DOWN    128
#define R       256
#define L       512

#define SCREENBUFFER        ((volatile uint16 *) (VRAM))

#endif
