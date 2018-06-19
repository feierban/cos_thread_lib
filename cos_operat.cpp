#include "cos_operat.h"
#include <qfileinfo.h>

cos_md5::cos_md5()
{
    ch = new QCryptographicHash(QCryptographicHash::Md5);
}

bool cos_md5::set_cos_file(std::string *filepathname)
{
    if (!ch) {
        return false;
    }
    QFile file(QString::fromStdString(*filepathname));
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray cont = file.readAll();
        ch->addData(cont.constData(), cont.count());
        file.close();
        return true;
    }
    return false;
}

cos_operat::cos_operat()
{
    //public
    m_initialized = false;

    //private
    m_config = NULL;
    m_cos = NULL;
}

cos_operat::cos_operat(std::string config)
{
    m_initialized = false;
    m_config = NULL;
    m_cos = NULL;

    this->loadconfig(config);
}

bool cos_operat::loadconfig(std::string str)
{    
    if (m_config) {
        delete m_config;
        m_config = NULL;
    }
    if (m_cos) {
        delete m_cos;
        m_cos = NULL;
    }
    m_initialized = false;

    QString file = QString::fromStdString(str);
    QFileInfo fileInfo(file);
    if (!fileInfo.exists()) {
        return false;
    }

    m_config = new qcloud_cos::CosConfig(str);
    m_cos = new qcloud_cos::CosAPI(*m_config);
    m_initialized = true;

    return true;
}

bool cos_operat::getserviceBucketsName(str_vec *bucketname)
{
    if (!m_initialized)
        return false;

    qcloud_cos::GetServiceReq req;
    qcloud_cos::GetServiceResp resp;
    qcloud_cos::CosResult result = m_cos->GetService(req, &resp);

    if (!result.IsSucc()) {
        return false;
    }

    const qcloud_cos::Owner& owner = resp.GetOwner();
    const std::vector<qcloud_cos::Bucket>& buckets = resp.GetBuckets();
    std::cout << "owner.m_id=" << owner.m_id << ", owner.display_name=" << owner.m_display_name << std::endl;
    for (std::vector<qcloud_cos::Bucket>::const_iterator itr = buckets.begin(); itr != buckets.end(); ++itr) {
        const qcloud_cos::Bucket& bucket = *itr;
        bucketname->clear();
        bucketname->push_back(bucket.m_name);
//        std::cout << "Bucket name=" << bucket.m_name << ", location="
//            << bucket.m_location << ", create_date=" << bucket.m_create_date << std::endl;
    }
    return true;
}

bool cos_operat::PutObject(std::string bucket, std::string obj, std::string content)
{
    if (!m_initialized)
        return false;

    std::istringstream cont(content);
    qcloud_cos::PutObjectByStreamReq req(bucket, obj, cont);
    qcloud_cos::PutObjectByStreamResp resp;
    qcloud_cos::CosResult result = m_cos->PutObject(req, &resp);

    if (!result.IsSucc()) {
        return false;
    }

    return true;
}

bool cos_operat::PutFile(std::string bucket, std::string obj, std::string file, QString *md5)
{
    if (!m_initialized)
        return false;

    qcloud_cos::PutObjectByFileReq req(bucket, obj, file);
    qcloud_cos::PutObjectByFileResp resp;
    qcloud_cos::CosResult result = m_cos->PutObject(req, &resp);

    if (!result.IsSucc()) {
        return false;
    }

    cos_md5 qbyam5;
    qbyam5.set_cos_file(&file);
    QByteArray m5_ = qbyam5.get_cos_md5().toHex();
    md5->append(m5_);

    return true;

}
