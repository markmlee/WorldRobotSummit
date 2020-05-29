// This inventory.h

#ifndef inventory_H
#define inventory_H

#include <string>


typedef struct _ITEM_DATA_
{
    int id;
    int object_id;
    int grasp_case;
    int expire_state;

    //current pose
    double cur_x; //current position x
    double cur_y;
    double cur_z;
    double cur_w;
    double cur_wx;
    double cur_wy;
    double cur_wz;

} ITEM_DATA;

typedef struct _ITEM_TABLE_
{
    int aruco_id;
    int object_id;
    int grasp_case;
    int expire_state;
    char character;

} ITEM_TABLE;



#endif // inventory_H

