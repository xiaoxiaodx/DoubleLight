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
    DataModelData(const bool &isSelect,const QString &avatarPath,const QString &numbering,const QString &time,QString name,QObject *parent=nullptr)
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
    Q_INVOKABLE void funSetDeviceInfo(QString ip,QString did);



    Q_INVOKABLE void funUpdateCurCalendarDate(QString datetime);
    Q_INVOKABLE void funSetInitSelectFalse();
    Q_INVOKABLE void funSetAllSelect(bool isSelect);
    Q_INVOKABLE void funSendDeleteSelectRequest();
    Q_INVOKABLE void funSendReviseRequest(int index,QString name,QString number);
    Q_INVOKABLE void funSendDeleteRequest(int index);


signals:
    void signal_createFaceImportWork(QString ip,int port);
    void signal_sendMsg(QVariantMap map);
    void signal_destroyConnect();

    void signal_singleAdd(bool isSucc);
    void signal_batchAdd(bool isSucc);
    void signal_batchAmount(int amount);
public slots:
    void slot_importCallback(QVariantMap map);
    void slot_sendtimerout();
protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role ) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    void rewriteLogFile();
    void removeIndex(int index);
    void reviseIndex(int index,QString name,QString number);
    void removeAll();
    void createFaceImport(QString ip,int port);

    QString transferImportFile(bool isSucc,QString path,QString newname);
    void writeLogInfo(QString filepath,QString name,QString number,QDateTime importdatetime);
    void addInfoInList(bool isBatch,bool isSucc,QString name,QString number,QString filePath);

    QString createMsgId(QString cmd);
    QVariantMap removeAlreadySend(QString cmd,QString msgid1);

    bool curSelect = false;
    QList<DataModelData*> m_listData;

    FaceImport *faceImport = nullptr;
    QThread *faceImportThread = nullptr;

    QString mip = "10.67.1.155";
    QString mdid = "test";

    int port = 8866;
    bool isIpChange = false;

    bool isConnected = false;
    QTimer sendTimer;
    QList<QVariantMap> listMsg;

    QString curCalendarDate = "";
};

#endif // DATAMODEL_H
