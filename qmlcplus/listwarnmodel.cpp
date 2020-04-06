#include "listwarnmodel.h"
#include <QVariant>
ListWarnModel::ListWarnModel(QObject *parent) : QObject(parent)
{

}


void ListWarnModel::setListwarn(const QVariant &list)
{

    m_listmodel = list.toList();
    emit listwarnChanged();
}

QVariant ListWarnModel::listwarn() const
{

    return QVariant::fromValue(m_listmodel);
}
