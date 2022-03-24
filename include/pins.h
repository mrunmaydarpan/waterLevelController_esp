#define _Mode 3 // RX
#define PB 0    // D3
#define led 16  // D0
#define buzz 2  // D4
/*............Relays ................*/
#define Relay_ON 10
#define Relay_ON_2 15
#define Relay_OFF 13
/*...................................*/
#if SENSOR_1
#define EchoPin 12    // D6
#define TriggerPin 14 // D5     need to swap
#elif SENSOR_2
#define sensorRX 14
#define sensorTX 12
#elif SENSOR_3
#define TRIGGER_PIN 14
#define PWM_OUTPUT_PIN 12
#endif