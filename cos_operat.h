#ifndef COS_OPERAT_H
#define COS_OPERAT_H
#include <cos_config.h>
#include <cos_api.h>
#include <sstream>
#include <QThread>
#include <QCryptographicHash>
#include <QByteArray>

typedef std::vector<std::string> str_vec;

class cos_md5
{
public:
    cos_md5();
    bool set_cos_file(std::string *filepathname);
    QByteArray get_cos_md5() {
        return ch->result();
    }
private:
    QCryptographicHash *ch;
};

class cos_operat :public cos_md5
{
public:
    //function
    cos_operat();
    cos_operat(std::string config);
    bool loadconfig(std::string);
    void set_bucketname(std::string name) {
        m_bucketname = name;
    }

    bool getserviceBucketsName(str_vec *bucketname);
    bool PutObject(std::string bucket, std::string obj, std::string content);
    bool PutFile(std::string bucket, std::string obj, std::string file, QString *md5);

    bool m_initialized;

private:
    qcloud_cos::CosConfig *m_config;
    qcloud_cos::CosAPI *m_cos;
    std::string m_bucketname;
};

#endif // COS_OPERAT_H
