#ifndef COS_UPDATE_LIB_H
#define COS_UPDATE_LIB_H

#include <cos_operat.h>
#include <QThread>

#include "cos_update_lib_global.h"

class COS_UPDATE_LIBSHARED_EXPORT Cos_update_lib:
                            public QThread
{
    Q_OBJECT

public:
    Cos_update_lib();


public:
    enum MessageType{
        MSG_Error,
        MSG_Warning,
        MSG_About,
    };
    typedef struct{
        bool suc;
        QString filename;
        QString md5;
        QString pos;
        QString url;
    }UploadStruct;

public:
    virtual void run() Q_DECL_OVERRIDE;

public:
    void runSetConfig(QString file);
    void runUpObject(QString ver, QString core="");
    void runDoUpLoad(QString path);
    void runDoUpLoad(QString path, QString ver, QString core="");

signals:
    void sig_message(MessageType type, QString str);
    void sig_UploadInfo(UploadStruct up);

private:
    enum ActionType{
        TYPE_NORMAL,
        SET_CONFIG,
        PUT_OBJECT,
        DO_UPLOAD,
        PUT_FILE,
        SET_END,
    };

    void putObjectFunc();
    void setConfigFunc();
    void getBucketsNameFunc();
    void doUpLoadFunc();

    cos_operat *m_cos;
    ActionType m_type;
    QString m_ver;
    QString m_core;
    QString m_bucket;

    QString m_object;
    QString m_path;
    QString m_config;

};

#endif // COS_UPDATE_LIB_H
