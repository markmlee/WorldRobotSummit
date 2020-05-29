// This JointInformation.h is for OmniWheel4

#ifndef JOINT_INFORMATION_H
#define JOINT_INFORMATION_H

#include <QVector>
#include <QString>



enum JointSequentialNumber
{
    FLW = 0, FRW, BRW, BLW, NO_OF_JOINTS
};

const QString JointNameList[NO_OF_JOINTS] = {
    "FLW", "FRW", "BRW", "BLW"
};

const struct {
    int id;
    int ch;
} MC_ID_CH_Pairs[NO_OF_JOINTS] = {
    {0,0}, {0,1},
    {1,0}, {1,1}
};

inline int MC_GetID(int jnum){
    return MC_ID_CH_Pairs[jnum].id;
}
inline int MC_GetCH(int jnum){
    return MC_ID_CH_Pairs[jnum].ch;
}

#endif // JOINT_INFORMATION_H
