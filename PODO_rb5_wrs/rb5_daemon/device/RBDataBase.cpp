#include "RBDataBase.h"

RBDataBase::RBDataBase()
{
    dbCore = QSqlDatabase::addDatabase("QSQLITE");
}

void RBDataBase::SetFilename(QString name){
    filename = name;
}

bool RBDataBase::OpenDB(){
    dbCore.setDatabaseName(filename);
    if(!dbCore.open()){
        FILE_LOG(logERROR) << "Can't open database file [" << filename.toStdString().data() << "]";
        return false;
    }

    QString strQuery;
    QSqlQuery query(dbCore);
    query.setForwardOnly(true);

    // General ----
    query.exec("SELECT * from General");
    if(query.next()){
        _DB_GENERAL.VERSION         = query.value(0).toInt();
        _DB_GENERAL.NO_OF_COMM_CH   = query.value(1).toInt();
        _DB_GENERAL.NO_OF_MC        = query.value(2).toInt();
        _DB_GENERAL.NO_OF_FT        = query.value(3).toInt();
    }

    // Motor Controller ----
    strQuery.sprintf("SELECT * from MotionController");
    query.exec(strQuery);
    for(int i=0; i<_DB_GENERAL.NO_OF_MC; i++){
        if(query.next()){
            _DB_MC[i].BOARD_ID          = query.value(0).toInt();
            _DB_MC[i].BOARD_NAME        = query.value(1).toString();
            _DB_MC[i].BOARD_TYPE        = query.value(2).toInt();
            _DB_MC[i].MOTOR_CHANNEL     = _DB_MC[i].BOARD_TYPE;
            _DB_MC[i].CAN_CHANNEL       = query.value(3).toInt();
            _DB_MC[i].ID_SEND_REF       = query.value(4).toInt();
            _DB_MC[i].ID_RCV_ENC        = query.value(5).toInt();
            _DB_MC[i].ID_RCV_STAT       = query.value(6).toInt();
            _DB_MC[i].ID_RCV_INFO       = query.value(7).toInt();
            _DB_MC[i].ID_RCV_PARA       = query.value(8).toInt();
            _DB_MC[i].ID_SEND_GENERAL   = query.value(9).toInt();

            for(int j=0; j<MAX_JOINT; j++){
                _DB_MC[i].JOINTS[j].HARMONIC = query.value(10+j*6).toDouble();
                _DB_MC[i].JOINTS[j].PULLY_DRIVE = query.value(11+j*6).toDouble();
                _DB_MC[i].JOINTS[j].PULLY_DRIVEN = query.value(12+j*6).toDouble();
                _DB_MC[i].JOINTS[j].ENCODER_RESOLUTION = query.value(13+j*6).toDouble();
                _DB_MC[i].JOINTS[j].PPR = _DB_MC[i].JOINTS[j].HARMONIC * _DB_MC[i].JOINTS[j].PULLY_DRIVEN / _DB_MC[i].JOINTS[j].PULLY_DRIVE * _DB_MC[i].JOINTS[j].ENCODER_RESOLUTION / 360.0;
                _DB_MC[i].JOINTS[j].FRIC_PARAM1 = query.value(14+j*6).toDouble();
                _DB_MC[i].JOINTS[j].FRIC_PARAM2 = query.value(15+j*6).toDouble();
            }
        }
    }

    // FT Sensor ----
    strQuery.sprintf("SELECT * from FTSensor");
    query.exec(strQuery);
    for(int i=0; i<_DB_GENERAL.NO_OF_FT; i++){
        if(query.next()){
            _DB_FT[i].BOARD_ID      = query.value(0).toInt();
            _DB_FT[i].BOARD_NAME    = query.value(1).toString();
            _DB_FT[i].SENSOR_ID     = query.value(2).toInt();
            _DB_FT[i].CAN_CHANNEL   = query.value(3).toInt();
            _DB_FT[i].SENSOR_TYPE   = query.value(4).toInt();
            _DB_FT[i].ID_RCV_DATA1  = query.value(5).toInt();
            _DB_FT[i].ID_RCV_DATA2  = query.value(6).toInt();
            _DB_FT[i].ID_RCV_ACC    = query.value(7).toInt();
            _DB_FT[i].ID_RCV_STAT   = query.value(8).toInt();
            _DB_FT[i].ID_RCV_INFO   = query.value(9).toInt();
            _DB_FT[i].ID_RCV_PARA   = query.value(10).toInt();
        }
    }

    dbCore.close();
    return true;
}
