#ifndef LISTWARNMODEL_H
#define LISTWARNMODEL_H

#include <QObject>
#include <warnmodeldata.h>
class ListWarnModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant listwarn read listwarn WRITE setListwarn NOTIFY listwarnChanged)
public:
    explicit ListWarnModel(QObject *parent = nullptr);

    QVariant listwarn() const;
    void setListwarn(const QVariant &list);
signals:
    void listwarnChanged();
public slots:

private:
    QList<QVariant> m_listmodel;

};

#endif // LISTWARNMODEL_H
