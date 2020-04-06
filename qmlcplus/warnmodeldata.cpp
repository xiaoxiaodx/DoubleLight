#include "warnmodeldata.h"

WarnModelData::WarnModelData(const bool &isSelect,const QString &warnTime,const QString &warnTemp,QString imgName,QString path,QObject *parent)
    :QObject(parent),m_isSelect(isSelect),m_warnTime(warnTime),m_warnTemp(warnTemp),m_imgName(imgName),m_absolutePath(path)

{

}

bool WarnModelData::isSelect() const
{
    return m_isSelect;
}

void WarnModelData::setIsSelect(const bool &v)
{
    if(m_isSelect!=v)
        m_isSelect=v;
    emit isSelectChanged();
}

QString WarnModelData::warnTemp() const
{
    return m_warnTemp;
}

void WarnModelData::setWarnTemp(const QString &w)
{
    if(m_warnTemp != w)
        m_warnTemp = w;
    emit warnTempChanged();
}

QString WarnModelData::warnTime() const
{
    return m_warnTime;
}

void WarnModelData::setWarnTime(const QString &w)
{
    if(m_warnTime != w)
        m_warnTime = w;
    emit warnTimeChanged();
}

QString WarnModelData::imgName() const
{
    return m_imgName;
}

void WarnModelData::setImgName(const QString &w)
{
    if(m_imgName != w)
        m_imgName = w;
    emit imgNameChanged();
}

QString WarnModelData::absolutePath()const
{
    return m_absolutePath;
}
void WarnModelData::setAbsolutePath(const QString &w)
{
    if(m_absolutePath != w)
        m_absolutePath = w;
    emit absolutePathChanged();
}

