#define _VERSION "7.6"
#define _HARDWARE "9.0"

const char brand[] = "MDtronix";

/*------------sensor setting-----------------*/
// #define SENSOR_1 true // sonar
// #define SENSOR_2 true // UART
#define SENSOR_3 true // PWM
/*------------wifi setting-------------------*/
#define AP_MODE false // AP Mode

#if !AP_MODE
#define WM_SET true // wifimanager
// #define ESP_CON true // espconnect
#endif
/*------------API setting--------------------*/
#if !AP_MODE
#define HA_INIT false
#endif
#if HA_INIT
#define BROKER_ADDR IPAddress(192, 168, 1, 7)
#endif
/*-------------------------------------------*/

#define OLED true
#define DryRun false
#define debugData true
#define Buzzer false
#define relayPin_inverted true

#define on_delay 500
#define off_delay 1000

#if WM_SET
#define WM_ASYNC
#endif
#if SENSOR_1
#define SENSOR_DISP 1
#elif SENSOR_2
#define SENSOR_DISP 2
#elif SENSOR_3
#define SENSOR_DISP 3
#endif