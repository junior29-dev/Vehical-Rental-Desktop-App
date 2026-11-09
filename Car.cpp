#include <QStringList>
#include "Car.h"

Car::Car() : Vehicle(), m_numberOfDoors(4), m_numberOfSeats(5) {}

Car::Car(const QString &id, const QString &brand, const QString &model,
         double pricePerDay, int numberOfDoors, int numberOfSeats, bool isRented)
    : Vehicle(id, brand, model, pricePerDay, isRented),
      m_numberOfDoors(numberOfDoors), m_numberOfSeats(numberOfSeats) {}

int  Car::getNumberOfDoors() const      { return m_numberOfDoors; }
int  Car::getNumberOfSeats() const      { return m_numberOfSeats; }
void Car::setNumberOfDoors(int doors)   { m_numberOfDoors = doors; }
void Car::setNumberOfSeats(int seats)   { m_numberOfSeats = seats; }

QString Car::typeName() const { return QString("Car"); }

QString Car::toFileString() const {
    return QString("Car|%1|%2|%3|%4|%5|%6|%7")
        .arg(getId()).arg(getBrand()).arg(getModel())
        .arg(getPricePerDay()).arg(getIsRented() ? 1 : 0)
        .arg(m_numberOfDoors).arg(m_numberOfSeats);
}

QStringList Car::toTableRow() const {
    return {
        typeName(), getId(), getBrand(), getModel(),
        QString("R%1").arg(getPricePerDay(), 0, 'f', 2),
        getIsRented() ? QString("Rented") : QString("Available"),
        QString::number(m_numberOfDoors),
        QString::number(m_numberOfSeats),
        QString("-")
    };
}
