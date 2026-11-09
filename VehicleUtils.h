#ifndef VEHICLEUTILS_H
#define VEHICLEUTILS_H

// ── Qt Generics (Templates) – Assignment requirement ──────────────────────────
#include <QList>
#include <QString>

// Template 1: count items in a QList matching a predicate
template <typename T, typename Predicate>
int countIf(const QList<T*> &list, Predicate pred)
{
    int count = 0;
    for (const T* item : list)
        if (pred(item)) ++count;
    return count;
}

// Template 2: find the first item matching a predicate (returns nullptr if none)
template <typename T, typename Predicate>
T* findFirst(const QList<T*> &list, Predicate pred)
{
    for (T* item : list)
        if (pred(item)) return item;
    return nullptr;
}

// Template 3: collect all matching items into a new QList
template <typename T, typename Predicate>
QList<T*> filterList(const QList<T*> &list, Predicate pred)
{
    QList<T*> result;
    for (T* item : list)
        if (pred(item)) result.append(item);
    return result;
}

// Template 4: sum pricePerDay across every vehicle in a list
template <typename T>
double totalPricePerDay(const QList<T*> &list)
{
    double total = 0.0;
    for (const T* item : list)
        total += item->getPricePerDay();
    return total;
}

#endif // VEHICLEUTILS_H
