#include <QStringList>
#include "Vehicle.h"

Vehicle::Vehicle()
    : m_id(""), m_brand(""), m_model(""), m_pricePerDay(0.0), m_isRented(false) {}

Vehicle::Vehicle(const QString &id, const QString &brand, const QString &model,
                 double pricePerDay, bool isRented)
    : m_id(id), m_brand(brand), m_model(model),
      m_pricePerDay(pricePerDay), m_isRented(isRented) {}

Vehicle::~Vehicle() {}

QString Vehicle::getId()          const { return m_id; }
QString Vehicle::getBrand()       const { return m_brand; }
QString Vehicle::getModel()       const { return m_model; }
double  Vehicle::getPricePerDay() const { return m_pricePerDay; }
bool    Vehicle::getIsRented()    const { return m_isRented; }

void Vehicle::setId(const QString &id)       { m_id = id; }
void Vehicle::setBrand(const QString &brand) { m_brand = brand; }
void Vehicle::setModel(const QString &model) { m_model = model; }
void Vehicle::setPricePerDay(double price)   { m_pricePerDay = price; }
void Vehicle::setIsRented(bool rented)       { m_isRented = rented; }

// Default table row: Type | ID | Brand | Model | Price | Status | Extra
QStringList Vehicle::toTableRow() const
{
    return {
        typeName(),
        m_id,
        m_brand,
        m_model,
        QString("R%1").arg(m_pricePerDay, 0, 'f', 2),
        m_isRented ? QString("Rented") : QString("Available"),
        QString("-"),
        QString("-"),
        QString("-")
    };
}
