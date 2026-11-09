#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"

class Car : public Vehicle
{
public:
    Car();
    Car(const QString &id, const QString &brand, const QString &model,
        double pricePerDay, int numberOfDoors, int numberOfSeats,
        bool isRented = false);

    int  getNumberOfDoors() const;
    int  getNumberOfSeats() const;
    void setNumberOfDoors(int doors);
    void setNumberOfSeats(int seats);

    QString     typeName()     const override;
    QString     toFileString() const override;
    QStringList toTableRow()   const override;

private:
    int m_numberOfDoors;
    int m_numberOfSeats;
};

#endif // CAR_H
