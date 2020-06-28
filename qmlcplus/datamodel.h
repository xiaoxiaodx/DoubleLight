#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "faceimport.h"
#define QML_PROPERTY(type,name) \
    Q_PROPERTY(type name READ name WRITE set##name NOTIFY name##Change) \
    public: type name() const { return m_##name;} \
    public Q_SLOTS: void set##name(type arg) { m_##name = arg; emit name##Change(arg);} \
    Q_SIGNALS:  \
    void name##Change(type arg);\
    private: \
    type m_##name; \


#include <QThread>
class DataModelData: public QObject
{

    Q_OBJECT
public:
    DataModelData(const bool &isSelect,const QString &avatarPath,const QString &numbering,const QString &time,QString imgName,QString name,QObject *parent=nullptr)
    :QObject(parent),m_isSelect(isSelect),m_numbering(numbering),m_time(time),m_name(name),m_avatarPath(avatarPath){  }
    QML_PROPERTY(bool,isSelect)
    QML_PROPERTY(QString,numbering)
    QML_PROPERTY(QString,time)
    QML_PROPERTY(QString,name)
    QML_PROPERTY(QString,avatarPath)
};



class DataModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);

    enum datasourceRoles {
        IsSelect = Qt::UserRole ,
        AvatarPath,
        Numbering,
        Name,
        Time,
    };
    Q_INVOKABLE void funSendCmd(QVariantMap map);

    Q_INVOKABLE void funImportSingle(QString name,QString number,QString imgPath);
    Q_INVOKABLE void funImportBatch(QString folderPath);
    Q_INVOKABLE void funSetIp(QString ip);
signals:
    void signal_createFaceImportWork(QString ip,int port);
    void signal_sendMsg(QVariantMap map);
    void signal_destroyConnect();
public slots:
    void slot_importCallback(QVariantMap map);
protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role ) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;


private:
    void removeIndex(int index);
    void removeAll();


    QList<DataModelData*> m_listData;

    FaceImport *faceImport = nullptr;
    QThread *faceImportThread = nullptr;
    QString mip = "";
    int port = 8866;
    bool isIpChange = false;

};

#endif // DATAMODEL_H
