#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTabWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QStatusBar>
#include <QGroupBox>

#include "Vehicle.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // ── Signals & Slots ───────────────────────────────────────────────────────
    void onAddVehicle();
    void onSearchVehicle();
    void onClearSearch();
    void onRentVehicle();
    void onReturnVehicle();
    void onRemoveVehicle();
    void onTypeChanged(int index);       // custom slot: show/hide extra fields
    void onTableSelectionChanged();      // custom slot: sync selected ID field
    void onFilterChanged(int index);     // custom slot: filter table by type/status
    void onRefreshTable();
    void onShowSummary();

private:
    // ── Fleet ─────────────────────────────────────────────────────────────────
    QList<Vehicle *> m_fleet;            // Qt container
    QString          m_dataFile;

    // ── File I/O ──────────────────────────────────────────────────────────────
    void loadFromFile();
    void saveToFile() const;

    // ── UI builders ───────────────────────────────────────────────────────────
    void buildUI();
    QWidget *buildAddPanel();
    QWidget *buildManagePanel();
    QWidget *buildSummaryPanel();
    void     applyStyleSheet();

    // ── Helpers ───────────────────────────────────────────────────────────────
    void     refreshTable(const QList<Vehicle *> &list);
    void     updateSummary();
    Vehicle *selectedVehicle() const;
    Vehicle *findById(const QString &id) const;
    bool     idExists(const QString &id) const;
    void     setStatus(const QString &msg, bool ok = true);

    // ── Widgets ───────────────────────────────────────────────────────────────
    QTabWidget     *m_tabs;

    // Add panel
    QComboBox      *m_typeCombo;
    QLineEdit      *m_idEdit;
    QLineEdit      *m_brandEdit;
    QLineEdit      *m_modelEdit;
    QDoubleSpinBox *m_priceEdit;
    QSpinBox       *m_doorsEdit;
    QSpinBox       *m_seatsEdit;
    QSpinBox       *m_ccEdit;
    QGroupBox      *m_carGroup;
    QGroupBox      *m_motoGroup;

    // Manage panel
    QTableWidget   *m_table;
    QLineEdit      *m_searchEdit;
    QLineEdit      *m_selectedIdEdit;
    QComboBox      *m_filterCombo;

    // Summary panel
    QLabel         *m_summaryLabel;

    // Status bar label
    QLabel         *m_statusLabel;
};

#endif // MAINWINDOW_H
