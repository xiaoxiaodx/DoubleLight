#ifndef WARNMODEL_H
#define WARNMODEL_H
#include "warnmodeldata.h"
#include <QAbstractListModel>
#include <QQuickWindow>
#include <QDateTime>
#include <QDir>
class WarnModel : public QAbstractListModel
{
    Q_OBJECT
public:

    WarnModel(QObject *parent = nullptr);

    enum datasourceRoles {
        IsSelect = Qt::UserRole ,
        WarnTime,
        WarnTemp,
        ImgName,
        AbsolutePath
    };

    Q_INVOKABLE void setClipbord(QString str);

    Q_INVOKABLE void funFlushWarnInfo(QString capturePaht,QString logFileName);
    Q_INVOKABLE void funDeleteIndex(int index);
    Q_INVOKABLE int funFindIndex(QString h,QString m,QString s);
    Q_INVOKABLE void funSetAllSelect(bool isSelect);//选中所有数据
    Q_INVOKABLE void funSetInitSelectFalse();//将数据初始化选中属性修改为false
    Q_INVOKABLE void funDeleteSelect();

    Q_INVOKABLE void funProcessPushAlarm(QString path,QVariantMap map);
    Q_INVOKABLE void funProcessPushAlarm1(QString path,QVariantMap map);
    Q_INVOKABLE void funProcessPushAlarm2(QString path,QVariantMap map);
    //截屏
    Q_INVOKABLE bool funScreenShoot(QString path,QQuickWindow *quic,int capx,int capy,int capw,int caph,float warnTemp);
    Q_INVOKABLE bool funScreenShoot1(QString path,QQuickWindow *quic,int capx,int capy,int capw,int caph,float warnTemp,int tempType);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role ) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
signals:
    void signal_sendWarnMsg(int type,QString path,QString time,float tempvalue);
private:
    void removeIndex(int index);
    void removeAll();

    void writeLogInfo(QString );

    QList<WarnModelData*> m_listWarn;
    QString curDate = "";
    QString curPath = "";
    bool curSelect = false;
};

#endif // WARNMODEL_H
