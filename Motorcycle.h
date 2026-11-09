#ifndef MOTORCYCLE_H
#define MOTORCYCLE_H

#include "Vehicle.h"

class Motorcycle : public Vehicle
{
public:
    Motorcycle();
    Motorcycle(const QString &id, const QString &brand, const QString &model,
               double pricePerDay, int engineCapacityCC, bool isRented = false);

    int  getEngineCapacityCC() const;
    void setEngineCapacityCC(int cc);

    QString     typeName()     const override;
    QString     toFileString() const override;
    QStringList toTableRow()   const override;

private:
    int m_engineCapacityCC;
};

#endif // MOTORCYCLE_H
