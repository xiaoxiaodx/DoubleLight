#ifndef WARNMODEL_H
#define WARNMODEL_H
#include "warnmodeldata.h"
#include <QAbstractListModel>

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
    Q_INVOKABLE void funFlushWarnInfo(QString capturePaht,QString logFileName);
    Q_INVOKABLE void funDeleteIndex(int index);
    Q_INVOKABLE int funFindIndex(QString h,QString m,QString s);
    Q_INVOKABLE void funSetAllSelect(bool isSelect);
    Q_INVOKABLE void funDeleteSelect();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role ) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    void removeIndex(int index);
    void removeAll();

    QList<WarnModelData*> m_listWarn;
};

#endif // WARNMODEL_H
