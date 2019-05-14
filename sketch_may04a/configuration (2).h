/*Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.*/
//Define L298N Dual H-Bridge Motor Controller Pins
#define dir1PinL  11    //Motor direction
#define dir2PinL  9   //Motor direction
#define speedPinL 5    // Needs to be a PWM pin to be able to control motor speed

#define dir1PinR   7    //Motor direction
#define dir2PinR    8//Motor direction
#define speedPinR   6  // Needs to be a PWM pin to be able to control motor speed
