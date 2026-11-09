#ifndef VEHICLE_H
#define VEHICLE_H

#include <QString>

// ── Abstract base class ───────────────────────────────────────────────────────
class Vehicle
{
public:
    Vehicle();
    Vehicle(const QString &id, const QString &brand, const QString &model,
            double pricePerDay, bool isRented = false);
    virtual ~Vehicle();

    // Getters
    QString getId()          const;
    QString getBrand()       const;
    QString getModel()       const;
    double  getPricePerDay() const;
    bool    getIsRented()    const;

    // Setters
    void setId(const QString &id);
    void setBrand(const QString &brand);
    void setModel(const QString &model);
    void setPricePerDay(double price);
    void setIsRented(bool rented);

    // Pure virtual – abstraction
    virtual QString typeName()     const = 0;
    virtual QString toFileString() const = 0;

    // Virtual – polymorphism
    virtual QStringList toTableRow() const;

private:
    QString m_id;
    QString m_brand;
    QString m_model;
    double  m_pricePerDay;
    bool    m_isRented;
};

#endif // VEHICLE_H
