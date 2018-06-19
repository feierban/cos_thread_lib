#include "cos_update_lib.h"
#include <QDir>

static bool getpathfiles(QString path, str_vec *files)
{
    QDir dir(path);
    foreach (QFileInfo mfi, dir.entryInfoList()) {
        if(mfi.isFile()) {
            files->push_back(mfi.absoluteFilePath().toStdString());
        } else {
            if(mfi.fileName()=="." || mfi.fileName() == "..")continue;
            getpathfiles(mfi.absoluteFilePath(), files);
        }
    }
    return true;
}

Cos_update_lib::Cos_update_lib()
{
    m_cos = new cos_operat();
    m_type = PUT_FILE;
}

void Cos_update_lib::run()
{
    switch (m_type) {
        case TYPE_NORMAL:{
        }break;
        case SET_CONFIG: {
            setConfigFunc();
        }break;
        case PUT_OBJECT: {
            putObjectFunc();
        }break;
        case PUT_FILE: {
        }break;
        case DO_UPLOAD: {
            doUpLoadFunc();
        }break;
        default: {
        }break;
    }
    m_type = TYPE_NORMAL;
}

void Cos_update_lib::runSetConfig(QString file)
{
    m_config = file;
    m_type = SET_CONFIG;
    this->run();
}

void Cos_update_lib::runUpObject(QString ver, QString cor)
{
    if (!ver.endsWith('/'))
        ver += '/';
    if (cor.length()!=0 && !cor.endsWith('/'))
        cor += '/';

    m_object = ver+cor;
    m_type = PUT_OBJECT;
    this->run();
}

void Cos_update_lib::runDoUpLoad(QString path)
{
    m_path = path;
    m_type = DO_UPLOAD;
    this->run();
}

void Cos_update_lib::runDoUpLoad(QString path, QString ver, QString core)
{
    m_path = path;
    m_ver = ver;
    m_core = core;
    if (!ver.endsWith('/')) {
        ver += '/';
    }
    if (core.length()!=0 && !core.endsWith('/')) {
        core += '/';
    }
    m_object = ver + core;
    m_type = DO_UPLOAD;
    this->run();
}

void Cos_update_lib::putObjectFunc()
{
    std::string content;
    bool res = m_cos->PutObject(m_bucket.toStdString(), m_object.toStdString(), content);
    if (res) {
        sig_message(MSG_About, "success to build version path\n or find version path");
    } else {
        sig_message(MSG_Warning, "failed to build version path");
    }
}

void Cos_update_lib::setConfigFunc()
{
    std::cout << "miao miao miao?"  << std::endl;
    if (!m_cos) {
        m_cos = new cos_operat();
    }
    if (!m_cos) {
        return;
    }
    m_cos->loadconfig(m_config.toStdString());

    str_vec buckets;
    if (!m_cos->getserviceBucketsName(&buckets)) {
        sig_message(MSG_Warning, "no buckets available");
    } else {
        m_bucket = QString::fromStdString(buckets[0]);
        sig_message(MSG_About, "get bucket");
    }
    return;
}

void Cos_update_lib::getBucketsNameFunc()
{
    return;
}

void Cos_update_lib::doUpLoadFunc()
{
    str_vec files;
    if (!getpathfiles(m_path, &files)) {
        sig_message(MSG_Error, "failed to get path files");
        return;
    }

    for(auto a = files.begin(); a != files.end(); a++) {
        QString f = QString::fromStdString(*a);
        QString relative_path = f.right(f.length() - m_path.length() -1);
        QString fullpath = m_object + relative_path;
        QString md5;
        std::cout << *a << std::endl;

        bool res = m_cos->PutFile(m_bucket.toStdString(), fullpath.toStdString(), (*a), &md5);
        if (res) {
            UploadStruct info;
            info.suc = true;
            QFileInfo finfo(f);
            info.filename = finfo.fileName();
            info.md5 = md5;
            info.pos = relative_path;
            info.url = fullpath;
            sig_UploadInfo(info);
        } else {
            UploadStruct info;
            info.suc = false;
            QFileInfo finfo(f);
            info.filename = finfo.fileName();
            info.md5 = md5;
            info.pos = relative_path;
            info.url = fullpath;
            sig_UploadInfo(info);
        }
    }
}


