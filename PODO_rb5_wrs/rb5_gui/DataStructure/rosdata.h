#ifndef ROSDATA_H
#define ROSDATA_H

enum {
    BREAK = 0,
    ACCEPT,
    DONE,
    STATE_ERROR,
    INPUT_ERROR,
    ERROR_STOP,
    EXT_COLLISION,
    HUMAN_DETECTED
};


struct Update{
    int     robot_state;            //state of robot motion [1:idle   2:paused/stopped by accident   3:moving]
    int     power_state;            //power state
    int     program_mode;           //current program mode [0:real mode   1:simulation mode]

    int     collision_detect;       //collision dectect onoff [0:off   1:on]
    int     freedrive_mode;         //current freedrive status [0:off   1:on]

    float   speed;                  //overriding speed [0~1]

    float   joint_angles[6];        //[deg]
    float   joint_references[6];    //[deg]
    float   joint_current[6];       //[mA]
    float   joint_temperature[6];   //[celcius]
    int     joint_information[6];   //look mSTAT

    float   tcp_reference[6];
    float   tcp_position[6];

    float   tool_reference;         //reference voltage of tool flalnge board [0, 12, 24]

    float   ft_sensor[6];           //mx,my,fz,fx,fy,mz
    float   pir_detected[4];        //pir sensor (digital in)
};



struct Result
{
    int     rb5_result = BREAK;
    int     wheel_result = BREAK;
};
struct RB5toROS
{
    Update message;
    Result result;
};
struct command
{
    char type;
    int d0;
    int d1;
    float data;
    float coordinate[6];
    float spd;
    float acc;
    float wheel[3];
};
#endif // ROSDATA_H
