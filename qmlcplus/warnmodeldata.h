#ifndef WARNMODELDATA_H
#define WARNMODELDATA_H

#include <QObject>

class WarnModelData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isSelect READ isSelect WRITE setIsSelect NOTIFY isSelectChanged)
    Q_PROPERTY(QString warnTime READ warnTime WRITE setWarnTime NOTIFY warnTimeChanged)
    Q_PROPERTY(QString warnTemp READ warnTime WRITE setWarnTemp NOTIFY warnTempChanged)
    Q_PROPERTY(QString absolutePath READ absolutePath WRITE setAbsolutePath NOTIFY absolutePathChanged)
    Q_PROPERTY(QString imgName READ imgName WRITE setImgName NOTIFY imgNameChanged)

    Q_PROPERTY(QString numering READ numering WRITE setNumering NOTIFY numeringChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString maskRecognition READ maskRecognition WRITE setMaskRecognition NOTIFY maskRecognitionChanged)

public:
    WarnModelData(const bool &isSelect,const QString &warnTime,const QString &warnTemp,QString imgName,QString path,QObject *parent=nullptr);

    bool isSelect()const;
    void setIsSelect(const bool &v);
    QString warnTime()const;
    void  setWarnTime(const QString &w);
    QString warnTemp()const;
    void setWarnTemp(const QString &w);
    QString imgName()const;
    void setImgName(const QString &w);
    QString absolutePath()const;
    void setAbsolutePath(const QString &w);

    QString numering() const;
    void setNumering(const QString &w);
    QString name() const;
    void setName(const QString &w);
    QString maskRecognition() const;
    void setMaskRecognition(const QString &w);
signals:
    void isSelectChanged();
    void warnTimeChanged();
    void warnTempChanged();
    void imgNameChanged();
    void absolutePathChanged();
    void numeringChanged();
    void nameChanged();
    void maskRecognitionChanged();
private:
    bool m_isSelect;
    QString m_warnTime;
    QString m_warnTemp;
    QString m_imgName;
    QString m_absolutePath;

    QString m_name;
    QString m_numbering;
    QString m_maskRecognition;
};

#endif // WARNMODELDATA_H
