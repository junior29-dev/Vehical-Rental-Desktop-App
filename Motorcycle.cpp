#include <QStringList>
#include "Motorcycle.h"

Motorcycle::Motorcycle() : Vehicle(), m_engineCapacityCC(125) {}

Motorcycle::Motorcycle(const QString &id, const QString &brand, const QString &model,
                       double pricePerDay, int engineCapacityCC, bool isRented)
    : Vehicle(id, brand, model, pricePerDay, isRented),
      m_engineCapacityCC(engineCapacityCC) {}

int  Motorcycle::getEngineCapacityCC() const  { return m_engineCapacityCC; }
void Motorcycle::setEngineCapacityCC(int cc)  { m_engineCapacityCC = cc; }

QString Motorcycle::typeName() const { return QString("Motorcycle"); }

QString Motorcycle::toFileString() const {
    return QString("Motorcycle|%1|%2|%3|%4|%5|%6")
        .arg(getId()).arg(getBrand()).arg(getModel())
        .arg(getPricePerDay()).arg(getIsRented() ? 1 : 0)
        .arg(m_engineCapacityCC);
}

QStringList Motorcycle::toTableRow() const {
    return {
        typeName(), getId(), getBrand(), getModel(),
        QString("R%1").arg(getPricePerDay(), 0, 'f', 2),
        getIsRented() ? QString("Rented") : QString("Available"),
        QString("-"), QString("-"),
        QString::number(m_engineCapacityCC)
    };
}
