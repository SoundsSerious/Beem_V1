

//Version:
//#MajorRelease.Incremental.Build
#define VERSION "0.2.1"

#define SSID_AP "BEEM"
#define AP_CHANNEL 8

#define BEEMO_PORT 18330
#define MESH_PORT 18333
#define MESH_PASSWORD "beemmeup"
#define TELEM_PORT 8080

#define   BLINK_PERIOD    3000 // milliseconds until cycle repeat
#define   BLINK_DURATION  100  // milliseconds LED is on for

#define PRINT_MAINLOOP false
#define PRINT_COM true
#define LOG_DEBUG true //5ms log delay
#define COM_EVT_DEBUG true
#define MODE_LOOP_DEBUG false

#define SLEEP_INTERVAL 2000

//Pin Definition ESP32 - Thinnng
#define BUTTON 0


//LED DIRECTION:
// START CCW ON RIGHT, PROCEED 360, REVERSE, PROCESED CW 360
// #define NUM_LEDS 50
// #define LEDS_OUTSIDE_START 0
// #define LEDS_OUTSIDE_END 49
// #define LEDS_INSIDE_START 49
// #define LEDS_INSIDE_END 50

#define NUM_LEDS 115
#define LEDS_OUTSIDE_START 0
#define LEDS_OUTSIDE_END 114
#define LEDS_INSIDE_START 114
#define LEDS_INSIDE_END 115


#define LEDS_OUTSIDE_TOP 14
#define LEDS_OUTSIDE_BOTTOM 38
#define LEDS_INSIDE_TOP 78
#define LEDS_INSIDE_BOTTOM 54

//Lighting Order Positive Increase Incriment:
//RIGHT OUTSIDE LEDS: OUTSIDE_TOP -> OUTSIDE_BOTOTM
//LEFT OUTSIDE LEDS: 0 -> OUTSIDE TOP + OUTSIDE_BOTTOM -> OUTSIDE END

//LEFT INSIDE LEDS: INSIDE_START -> INSIDE_BOTTOM + INSIDE_TOP -> INSIDE_END
//RIGHT INSIDE LEDS:LEDS_INSIDE_BOTTOM -> INSIDE_TOP

// #ifndef SYSINFO_H
// #define SYSINFO_H
// uint8_t INSIDE_START(0);
// uint8_t INSIDE_END(60);
// uint8_t OUTSIDE_START( 60 );
// uint8_t OUTSIDE_END(120);
// uint8_t NUM_STRIPS(1);
// // #define TCA9548_CLOCK (uint32_t)100000
// //
// // #define LED_SPI_CLOCK (uint32_t)4000000
// // #define LED_DATA_PIN 13
// // #define LED_CLOCK_PIN 14
// // #define LED_MISO_PIN 12
// // #define LED_CS_PIN 15
// //
// // #define MPU_SPI_CLOCK (uint32_t)4000000
// // #define MPU_DATA_PIN 23
// // #define MPU_SLAVE_OUT 19
// // #define MPU_CLOCK_PIN 18
// // #define MPU_CS 27 //5
// // #define HSPI_DFLT_CS 2
// //
// // #define S_C0_MUXPIN 4
// // #define S_C1_MUXPIN 17
// // #define S_D0_MUXPIN 32
// // #define S_D1_MUXPIN 33
//
// #endif //SYSINFO_H
