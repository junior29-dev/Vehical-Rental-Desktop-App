#include "MainWindow.h"
#include "Car.h"
#include "Motorcycle.h"
#include "VehicleUtils.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QFont>
#include <QFrame>
#include <QSizePolicy>
#include <QIcon>

// ─────────────────────────────────────────────────────────────────────────────
//  CONSTRUCTOR / DESTRUCTOR
// ─────────────────────────────────────────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_dataFile("vehicles.dat")
{
    setWindowTitle(QString("Vehicle Rental Management System"));
    setMinimumSize(1100, 700);
    resize(1200, 760);

    buildUI();
    applyStyleSheet();
    loadFromFile();
    refreshTable(m_fleet);
    updateSummary();
    setStatus(QString("Loaded %1 vehicle(s) from file.").arg(m_fleet.size()));
}

MainWindow::~MainWindow()
{
    saveToFile();
    qDeleteAll(m_fleet);
    m_fleet.clear();
}

// ─────────────────────────────────────────────────────────────────────────────
//  UI BUILDERS
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::buildUI()
{
    // Central widget + main layout
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Header bar
    QFrame *header = new QFrame(this);
    header->setObjectName("headerBar");
    header->setFixedHeight(60);
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(20, 0, 20, 0);
    QLabel *titleLabel = new QLabel(QString("Vehicle Rental Management System"), header);
    titleLabel->setObjectName("appTitle");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    mainLayout->addWidget(header);

    // Tab widget
    m_tabs = new QTabWidget(central);
    m_tabs->setObjectName("mainTabs");
    m_tabs->addTab(buildAddPanel(),     QString("  Add Vehicle  "));
    m_tabs->addTab(buildManagePanel(),  QString("  Manage Fleet  "));
    m_tabs->addTab(buildSummaryPanel(), QString("  Summary  "));
    mainLayout->addWidget(m_tabs, 1);

    // Status bar
    m_statusLabel = new QLabel(QString("Ready"), this);
    m_statusLabel->setObjectName("statusLabel");
    statusBar()->addWidget(m_statusLabel, 1);
    statusBar()->setObjectName("appStatusBar");
}

// ── Add Vehicle Panel ─────────────────────────────────────────────────────────
QWidget *MainWindow::buildAddPanel()
{
    QWidget *panel = new QWidget();
    QHBoxLayout *outer = new QHBoxLayout(panel);
    outer->setContentsMargins(30, 24, 30, 24);
    outer->setSpacing(24);

    // ── Left: form ────────────────────────────────────────────────────────────
    QGroupBox *formBox = new QGroupBox(QString("Vehicle Details"), panel);
    formBox->setObjectName("formBox");
    QGridLayout *grid = new QGridLayout(formBox);
    grid->setVerticalSpacing(14);
    grid->setHorizontalSpacing(16);
    grid->setContentsMargins(20, 24, 20, 20);

    auto lbl = [&](const QString &text) {
        QLabel *l = new QLabel(text, formBox);
        l->setObjectName("formLabel");
        return l;
    };

    // Type
    grid->addWidget(lbl(QString("Vehicle Type")), 0, 0);
    m_typeCombo = new QComboBox(formBox);
    m_typeCombo->addItems({QString("Car"), QString("Motorcycle")});
    m_typeCombo->setObjectName("formCombo");
    grid->addWidget(m_typeCombo, 0, 1);

    // ID
    grid->addWidget(lbl(QString("Vehicle ID")), 1, 0);
    m_idEdit = new QLineEdit(formBox);
    m_idEdit->setPlaceholderText(QString("e.g. C031 or M011"));
    m_idEdit->setObjectName("formEdit");
    grid->addWidget(m_idEdit, 1, 1);

    // Brand
    grid->addWidget(lbl(QString("Brand")), 2, 0);
    m_brandEdit = new QLineEdit(formBox);
    m_brandEdit->setPlaceholderText(QString("e.g. Toyota"));
    m_brandEdit->setObjectName("formEdit");
    grid->addWidget(m_brandEdit, 2, 1);

    // Model
    grid->addWidget(lbl(QString("Model")), 3, 0);
    m_modelEdit = new QLineEdit(formBox);
    m_modelEdit->setPlaceholderText(QString("e.g. Corolla"));
    m_modelEdit->setObjectName("formEdit");
    grid->addWidget(m_modelEdit, 3, 1);

    // Price
    grid->addWidget(lbl(QString("Price per Day (R)")), 4, 0);
    m_priceEdit = new QDoubleSpinBox(formBox);
    m_priceEdit->setRange(1.0, 99999.0);
    m_priceEdit->setDecimals(2);
    m_priceEdit->setPrefix(QString("R "));
    m_priceEdit->setValue(500.0);
    m_priceEdit->setObjectName("formSpin");
    grid->addWidget(m_priceEdit, 4, 1);

    // ── Car extras ────────────────────────────────────────────────────────────
    m_carGroup = new QGroupBox(QString("Car Specifications"), formBox);
    m_carGroup->setObjectName("specBox");
    QGridLayout *carGrid = new QGridLayout(m_carGroup);
    carGrid->setContentsMargins(14, 16, 14, 14);
    carGrid->setSpacing(12);

    carGrid->addWidget(lbl(QString("Number of Doors")), 0, 0);
    m_doorsEdit = new QSpinBox(m_carGroup);
    m_doorsEdit->setRange(2, 6);
    m_doorsEdit->setValue(4);
    m_doorsEdit->setObjectName("formSpin");
    carGrid->addWidget(m_doorsEdit, 0, 1);

    carGrid->addWidget(lbl(QString("Number of Seats")), 1, 0);
    m_seatsEdit = new QSpinBox(m_carGroup);
    m_seatsEdit->setRange(1, 9);
    m_seatsEdit->setValue(5);
    m_seatsEdit->setObjectName("formSpin");
    carGrid->addWidget(m_seatsEdit, 1, 1);

    grid->addWidget(m_carGroup, 5, 0, 1, 2);

    // ── Motorcycle extras ─────────────────────────────────────────────────────
    m_motoGroup = new QGroupBox(QString("Motorcycle Specifications"), formBox);
    m_motoGroup->setObjectName("specBox");
    QGridLayout *motoGrid = new QGridLayout(m_motoGroup);
    motoGrid->setContentsMargins(14, 16, 14, 14);
    motoGrid->setSpacing(12);

    motoGrid->addWidget(lbl(QString("Engine Capacity (CC)")), 0, 0);
    m_ccEdit = new QSpinBox(m_motoGroup);
    m_ccEdit->setRange(50, 2500);
    m_ccEdit->setValue(600);
    m_ccEdit->setSuffix(QString(" CC"));
    m_ccEdit->setObjectName("formSpin");
    motoGrid->addWidget(m_ccEdit, 0, 1);

    grid->addWidget(m_motoGroup, 6, 0, 1, 2);
    m_motoGroup->setVisible(false);

    grid->setRowStretch(7, 1);
    outer->addWidget(formBox, 3);

    // ── Right: action buttons ─────────────────────────────────────────────────
    QGroupBox *actBox = new QGroupBox(QString("Actions"), panel);
    actBox->setObjectName("formBox");
    QVBoxLayout *actLayout = new QVBoxLayout(actBox);
    actLayout->setContentsMargins(20, 24, 20, 20);
    actLayout->setSpacing(12);

    auto makeBtn = [&](const QString &text, const QString &objName) {
        QPushButton *btn = new QPushButton(text, actBox);
        btn->setObjectName(objName);
        btn->setMinimumHeight(42);
        btn->setCursor(Qt::PointingHandCursor);
        return btn;
    };

    QPushButton *addBtn    = makeBtn(QString("Add Vehicle"),   QString("btnPrimary"));
    QPushButton *clearBtn  = makeBtn(QString("Clear Form"),    QString("btnSecondary"));

    actLayout->addWidget(addBtn);
    actLayout->addWidget(clearBtn);
    actLayout->addStretch();

    // Instructions box
    QGroupBox *infoBox = new QGroupBox(QString("Instructions"), actBox);
    infoBox->setObjectName("infoBox");
    QVBoxLayout *infoLayout = new QVBoxLayout(infoBox);
    infoLayout->setContentsMargins(14, 14, 14, 14);
    QLabel *infoLabel = new QLabel(
        QString("1. Choose vehicle type.\n"
                "2. Enter a unique ID.\n"
                "3. Fill in brand, model,\n"
                "   and price per day.\n"
                "4. Fill in type-specific\n"
                "   fields.\n"
                "5. Click Add Vehicle."), infoBox);
    infoLabel->setObjectName("infoText");
    infoLabel->setWordWrap(true);
    infoLayout->addWidget(infoLabel);
    actLayout->addWidget(infoBox);

    outer->addWidget(actBox, 1);

    // Connections
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onTypeChanged);
    connect(addBtn,  &QPushButton::clicked, this, &MainWindow::onAddVehicle);
    connect(clearBtn, &QPushButton::clicked, this, [this]() {
        m_idEdit->clear(); m_brandEdit->clear(); m_modelEdit->clear();
        m_priceEdit->setValue(500.0); m_doorsEdit->setValue(4);
        m_seatsEdit->setValue(5);     m_ccEdit->setValue(600);
        m_typeCombo->setCurrentIndex(0);
        setStatus(QString("Form cleared."), true);
    });

    return panel;
}

// ── Manage Fleet Panel ────────────────────────────────────────────────────────
QWidget *MainWindow::buildManagePanel()
{
    QWidget *panel = new QWidget();
    QVBoxLayout *vbox = new QVBoxLayout(panel);
    vbox->setContentsMargins(20, 16, 20, 16);
    vbox->setSpacing(12);

    // ── Toolbar row ───────────────────────────────────────────────────────────
    QHBoxLayout *toolbar = new QHBoxLayout();
    toolbar->setSpacing(10);

    m_searchEdit = new QLineEdit(panel);
    m_searchEdit->setPlaceholderText(QString("Search by ID, brand, or model…"));
    m_searchEdit->setObjectName("searchEdit");
    m_searchEdit->setMinimumHeight(36);

    QPushButton *searchBtn = new QPushButton(QString("Search"), panel);
    searchBtn->setObjectName("btnPrimary");
    searchBtn->setMinimumHeight(36);
    searchBtn->setCursor(Qt::PointingHandCursor);

    QPushButton *clearSearchBtn = new QPushButton(QString("Clear"), panel);
    clearSearchBtn->setObjectName("btnSecondary");
    clearSearchBtn->setMinimumHeight(36);
    clearSearchBtn->setCursor(Qt::PointingHandCursor);

    QLabel *filterLbl = new QLabel(QString("Filter:"), panel);
    filterLbl->setObjectName("formLabel");
    m_filterCombo = new QComboBox(panel);
    m_filterCombo->addItems({QString("All"), QString("Cars Only"),
                              QString("Motorcycles Only"), QString("Available"),
                              QString("Rented")});
    m_filterCombo->setObjectName("formCombo");
    m_filterCombo->setMinimumHeight(36);

    toolbar->addWidget(m_searchEdit, 3);
    toolbar->addWidget(searchBtn);
    toolbar->addWidget(clearSearchBtn);
    toolbar->addSpacing(16);
    toolbar->addWidget(filterLbl);
    toolbar->addWidget(m_filterCombo, 1);

    vbox->addLayout(toolbar);

    // ── Table ─────────────────────────────────────────────────────────────────
    m_table = new QTableWidget(panel);
    m_table->setObjectName("vehicleTable");
    m_table->setColumnCount(9);
    m_table->setHorizontalHeaderLabels({
        QString("Type"), QString("ID"), QString("Brand"), QString("Model"),
        QString("Price / Day"), QString("Status"),
        QString("Doors"), QString("Seats"), QString("Engine CC")
    });
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    vbox->addWidget(m_table, 1);

    // ── Action row ────────────────────────────────────────────────────────────
    QHBoxLayout *actionRow = new QHBoxLayout();
    actionRow->setSpacing(10);

    QLabel *selLbl = new QLabel(QString("Selected ID:"), panel);
    selLbl->setObjectName("formLabel");
    m_selectedIdEdit = new QLineEdit(panel);
    m_selectedIdEdit->setPlaceholderText(QString("Click a row or type ID…"));
    m_selectedIdEdit->setObjectName("searchEdit");
    m_selectedIdEdit->setMaximumWidth(160);
    m_selectedIdEdit->setMinimumHeight(36);

    auto makeBtn = [&](const QString &text, const QString &obj) {
        QPushButton *b = new QPushButton(text, panel);
        b->setObjectName(obj);
        b->setMinimumHeight(36);
        b->setCursor(Qt::PointingHandCursor);
        return b;
    };

    QPushButton *rentBtn    = makeBtn(QString("Rent"),    QString("btnRent"));
    QPushButton *returnBtn  = makeBtn(QString("↩Return"),  QString("btnReturn"));
    QPushButton *removeBtn  = makeBtn(QString("Remove"),  QString("btnDanger"));
    QPushButton *refreshBtn = makeBtn(QString("Refresh"),  QString("btnSecondary"));
    QPushButton *summaryBtn = makeBtn(QString("Summary"),  QString("btnSecondary"));

    actionRow->addWidget(selLbl);
    actionRow->addWidget(m_selectedIdEdit);
    actionRow->addSpacing(8);
    actionRow->addWidget(rentBtn);
    actionRow->addWidget(returnBtn);
    actionRow->addWidget(removeBtn);
    actionRow->addStretch();
    actionRow->addWidget(refreshBtn);
    actionRow->addWidget(summaryBtn);

    vbox->addLayout(actionRow);

    // Connections
    connect(searchBtn,    &QPushButton::clicked, this, &MainWindow::onSearchVehicle);
    connect(clearSearchBtn,&QPushButton::clicked,this, &MainWindow::onClearSearch);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearchVehicle);
    connect(rentBtn,      &QPushButton::clicked, this, &MainWindow::onRentVehicle);
    connect(returnBtn,    &QPushButton::clicked, this, &MainWindow::onReturnVehicle);
    connect(removeBtn,    &QPushButton::clicked, this, &MainWindow::onRemoveVehicle);
    connect(refreshBtn,   &QPushButton::clicked, this, &MainWindow::onRefreshTable);
    connect(summaryBtn,   &QPushButton::clicked, this, &MainWindow::onShowSummary);
    connect(m_table,      &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onTableSelectionChanged);
    connect(m_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFilterChanged);

    return panel;
}

// ── Summary Panel ─────────────────────────────────────────────────────────────
QWidget *MainWindow::buildSummaryPanel()
{
    QWidget *panel = new QWidget();
    QVBoxLayout *vbox = new QVBoxLayout(panel);
    vbox->setContentsMargins(30, 24, 30, 24);
    vbox->setSpacing(16);

    QLabel *heading = new QLabel(QString("Fleet Summary"), panel);
    heading->setObjectName("sectionHeading");
    vbox->addWidget(heading);

    m_summaryLabel = new QLabel(panel);
    m_summaryLabel->setObjectName("summaryLabel");
    m_summaryLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_summaryLabel->setWordWrap(true);
    vbox->addWidget(m_summaryLabel);
    vbox->addStretch();

    QPushButton *refreshSummary = new QPushButton(QString("Refresh Summary"), panel);
    refreshSummary->setObjectName("btnSecondary");
    refreshSummary->setMinimumHeight(38);
    refreshSummary->setCursor(Qt::PointingHandCursor);
    refreshSummary->setMaximumWidth(200);
    vbox->addWidget(refreshSummary);

    connect(refreshSummary, &QPushButton::clicked, this, [this]() {
        updateSummary();
        setStatus(QString("Summary refreshed."));
    });

    return panel;
}

// ─────────────────────────────────────────────────────────────────────────────
//  SLOTS
// ─────────────────────────────────────────────────────────────────────────────

// Custom slot: toggle Car/Motorcycle spec groups
void MainWindow::onTypeChanged(int index)
{
    m_carGroup->setVisible(index == 0);
    m_motoGroup->setVisible(index == 1);
}

// Custom slot: populate selected ID field from table click
void MainWindow::onTableSelectionChanged()
{
    QList<QTableWidgetItem *> sel = m_table->selectedItems();
    if (!sel.isEmpty()) {
        // ID is in column 1
        QTableWidgetItem *idItem = m_table->item(m_table->currentRow(), 1);
        if (idItem) m_selectedIdEdit->setText(idItem->text());
    }
}

// Custom slot: filter table
void MainWindow::onFilterChanged(int index)
{
    // Use Qt iterator to build filtered list
    QList<Vehicle *> filtered;
    QList<Vehicle *>::const_iterator it;
    for (it = m_fleet.constBegin(); it != m_fleet.constEnd(); ++it) {
        Vehicle *v = *it;
        switch (index) {
        case 0: filtered.append(v); break;                              // All
        case 1: if (v->typeName() == QLatin1String("Car"))         filtered.append(v); break;
        case 2: if (v->typeName() == QLatin1String("Motorcycle"))  filtered.append(v); break;
        case 3: if (!v->getIsRented())  filtered.append(v); break;     // Available
        case 4: if (v->getIsRented())   filtered.append(v); break;     // Rented
        }
    }
    refreshTable(filtered);
    setStatus(QString("Filter applied: %1 vehicle(s) shown.").arg(filtered.size()));
}

void MainWindow::onAddVehicle()
{
    QString id    = m_idEdit->text().trimmed();
    QString brand = m_brandEdit->text().trimmed();
    QString model = m_modelEdit->text().trimmed();
    double  price = m_priceEdit->value();

    // Business logic validation
    if (id.isEmpty() || brand.isEmpty() || model.isEmpty()) {
        setStatus(QString("Error: ID, Brand and Model cannot be empty."), false);
        QMessageBox::warning(this, QString("Validation Error"),
                             QString("Please fill in all required fields (ID, Brand, Model)."));
        return;
    }
    if (idExists(id)) {
        setStatus(QString("Error: Vehicle ID \"%1\" already exists.").arg(id), false);
        QMessageBox::warning(this, QString("Duplicate ID"),
                             QString("A vehicle with ID \"%1\" already exists.").arg(id));
        return;
    }

    if (m_typeCombo->currentIndex() == 0) {
        m_fleet.append(new Car(id, brand, model, price,
                               m_doorsEdit->value(), m_seatsEdit->value()));
    } else {
        m_fleet.append(new Motorcycle(id, brand, model, price, m_ccEdit->value()));
    }

    saveToFile();
    refreshTable(m_fleet);
    updateSummary();
    setStatus(QString("Vehicle \"%1\" (%2 %3) added successfully.").arg(id, brand, model));

    // Clear form
    m_idEdit->clear(); m_brandEdit->clear(); m_modelEdit->clear();
}

void MainWindow::onSearchVehicle()
{
    QString query = m_searchEdit->text().trimmed().toLower();
    if (query.isEmpty()) { refreshTable(m_fleet); return; }

    // Use template filterList + Qt iterator
    QList<Vehicle *> results;
    QList<Vehicle *>::const_iterator it;
    for (it = m_fleet.constBegin(); it != m_fleet.constEnd(); ++it) {
        Vehicle *v = *it;
        if (v->getId().toLower().contains(query)    ||
            v->getBrand().toLower().contains(query) ||
            v->getModel().toLower().contains(query)) {
            results.append(v);
        }
    }

    refreshTable(results);
    setStatus(QString("Search found %1 result(s) for \"%2\".").arg(results.size()).arg(query));
}

void MainWindow::onClearSearch()
{
    m_searchEdit->clear();
    m_filterCombo->setCurrentIndex(0);
    refreshTable(m_fleet);
    setStatus(QString("Search cleared. Showing all %1 vehicle(s).").arg(m_fleet.size()));
}

void MainWindow::onRentVehicle()
{
    QString id = m_selectedIdEdit->text().trimmed();
    if (id.isEmpty()) {
        setStatus(QString("Error: Select a vehicle or enter an ID first."), false);
        return;
    }
    Vehicle *v = findById(id);
    if (!v) {
        setStatus(QString("Error: No vehicle found with ID \"%1\".").arg(id), false);
        return;
    }
    if (v->getIsRented()) {
        setStatus(QString("Error: \"%1\" is already rented.").arg(id), false);
        QMessageBox::warning(this, QString("Already Rented"),
                             QString("Vehicle \"%1\" is already rented.").arg(id));
        return;
    }
    v->setIsRented(true);
    saveToFile();
    onFilterChanged(m_filterCombo->currentIndex());
    updateSummary();
    setStatus(QString("Vehicle \"%1\" rented successfully.").arg(id));
}

void MainWindow::onReturnVehicle()
{
    QString id = m_selectedIdEdit->text().trimmed();
    if (id.isEmpty()) {
        setStatus(QString("Error: Select a vehicle or enter an ID first."), false);
        return;
    }
    Vehicle *v = findById(id);
    if (!v) {
        setStatus(QString("Error: No vehicle found with ID \"%1\".").arg(id), false);
        return;
    }
    if (!v->getIsRented()) {
        setStatus(QString("Error: \"%1\" is not currently rented.").arg(id), false);
        QMessageBox::warning(this, QString("Not Rented"),
                             QString("Vehicle \"%1\" is not currently rented.").arg(id));
        return;
    }
    v->setIsRented(false);
    saveToFile();
    onFilterChanged(m_filterCombo->currentIndex());
    updateSummary();
    setStatus(QString("Vehicle \"%1\" returned successfully.").arg(id));
}

void MainWindow::onRemoveVehicle()
{
    QString id = m_selectedIdEdit->text().trimmed();
    if (id.isEmpty()) {
        setStatus(QString("Error: Select a vehicle or enter an ID first."), false);
        return;
    }
    Vehicle *v = findById(id);
    if (!v) {
        setStatus(QString("Error: No vehicle found with ID \"%1\".").arg(id), false);
        return;
    }
    int ans = QMessageBox::question(this, QString("Confirm Remove"),
        QString("Remove vehicle \"%1\" (%2 %3)?").arg(id, v->getBrand(), v->getModel()),
        QMessageBox::Yes | QMessageBox::No);
    if (ans != QMessageBox::Yes) return;

    m_fleet.removeOne(v);
    delete v;
    m_selectedIdEdit->clear();
    saveToFile();
    onFilterChanged(m_filterCombo->currentIndex());
    updateSummary();
    setStatus(QString("Vehicle \"%1\" removed.").arg(id));
}

void MainWindow::onRefreshTable()
{
    m_filterCombo->setCurrentIndex(0);
    m_searchEdit->clear();
    refreshTable(m_fleet);
    updateSummary();
    setStatus(QString("Table refreshed. %1 vehicle(s) total.").arg(m_fleet.size()));
}

void MainWindow::onShowSummary()
{
    updateSummary();
    m_tabs->setCurrentIndex(2);
}

// ─────────────────────────────────────────────────────────────────────────────
//  FILE HANDLING
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::loadFromFile()
{
    QFile file(m_dataFile);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        QStringList p = line.split(QLatin1Char('|'));
        if (p.size() < 6) continue;

        QString type  = p[0]; QString id = p[1];
        QString brand = p[2]; QString model = p[3];
        double  price = p[4].toDouble();
        bool    rented = (p[5].toInt() == 1);

        if (type == QLatin1String("Car") && p.size() >= 8)
            m_fleet.append(new Car(id, brand, model, price,
                                   p[6].toInt(), p[7].toInt(), rented));
        else if (type == QLatin1String("Motorcycle") && p.size() >= 7)
            m_fleet.append(new Motorcycle(id, brand, model, price,
                                          p[6].toInt(), rented));
    }
    file.close();
}

void MainWindow::saveToFile() const
{
    QFile file(m_dataFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return;
    QTextStream out(&file);
    // Use Qt iterator for file saving
    QList<Vehicle *>::const_iterator it;
    for (it = m_fleet.constBegin(); it != m_fleet.constEnd(); ++it)
        out << (*it)->toFileString() << Qt::endl;
    file.close();
}

// ─────────────────────────────────────────────────────────────────────────────
//  HELPERS
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::refreshTable(const QList<Vehicle *> &list)
{
    m_table->setRowCount(0);
    // Use Qt iterator to populate table
    QList<Vehicle *>::const_iterator it;
    for (it = list.constBegin(); it != list.constEnd(); ++it) {
        Vehicle *v = *it;
        int row = m_table->rowCount();
        m_table->insertRow(row);
        QStringList cols = v->toTableRow();
        for (int c = 0; c < cols.size() && c < m_table->columnCount(); ++c) {
            QTableWidgetItem *item = new QTableWidgetItem(cols[c]);
            item->setTextAlignment(Qt::AlignCenter);
            // Colour-code status column
            if (c == 5) {
                if (cols[c] == QLatin1String("Rented"))
                    item->setForeground(QColor("#E05252"));
                else
                    item->setForeground(QColor("#3AA86E"));
            }
            m_table->setItem(row, c, item);
        }
    }
}

void MainWindow::updateSummary()
{
    // Use template functions from VehicleUtils.h
    int total     = m_fleet.size();
    int available = countIf(m_fleet, [](const Vehicle *v){ return !v->getIsRented(); });
    int rented    = countIf(m_fleet, [](const Vehicle *v){ return  v->getIsRented(); });
    int cars      = countIf(m_fleet, [](const Vehicle *v){ return v->typeName() == QLatin1String("Car"); });
    int motos     = countIf(m_fleet, [](const Vehicle *v){ return v->typeName() == QLatin1String("Motorcycle"); });

    // Use filterList template for available vehicles
    QList<Vehicle *> availList = filterList(m_fleet, [](const Vehicle *v){ return !v->getIsRented(); });
    QList<Vehicle *> rentList  = filterList(m_fleet, [](const Vehicle *v){ return  v->getIsRented(); });

    double availRevenue = totalPricePerDay(availList);
    double rentRevenue  = totalPricePerDay(rentList);

    // Find most expensive using findFirst template + iterator
    double maxPrice = 0.0;
    Vehicle *mostExpensive = nullptr;
    for (auto it = m_fleet.constBegin(); it != m_fleet.constEnd(); ++it)
        if ((*it)->getPricePerDay() > maxPrice) {
            maxPrice = (*it)->getPricePerDay(); mostExpensive = *it;
        }

    QString html;
    html += QString("<table cellspacing='12' style='font-size:14px;'>");

    auto row = [&](const QString &label, const QString &value, const QString &color = "") {
        html += QString("<tr><td style='color:#888;min-width:220px;'>%1</td>"
                        "<td style='font-weight:600;color:%2;'>%3</td></tr>")
                    .arg(label).arg(color.isEmpty() ? QString("inherit") : color).arg(value);
    };

    row(QString("Total Vehicles"),      QString::number(total));
    row(QString("Cars"),                QString::number(cars));
    row(QString("Motorcycles"),         QString::number(motos));
    row(QString("Available"),           QString::number(available), QString("#3AA86E"));
    row(QString("Currently Rented"),    QString::number(rented),    QString("#E05252"));
    row(QString("Potential Revenue / Day"), QString("R%1").arg(availRevenue, 0, 'f', 2), QString("#4A90D9"));
    row(QString("Active Rental Revenue / Day"), QString("R%1").arg(rentRevenue, 0, 'f', 2), QString("#4A90D9"));

    if (mostExpensive)
        row(QString("Most Expensive Vehicle"),
            QString("%1 %2 (R%3/day)").arg(mostExpensive->getBrand(),
                     mostExpensive->getModel()).arg(mostExpensive->getPricePerDay(), 0, 'f', 2));

    html += QString("</table>");
    m_summaryLabel->setText(html);
}

Vehicle *MainWindow::selectedVehicle() const
{
    return findById(m_selectedIdEdit->text().trimmed());
}

Vehicle *MainWindow::findById(const QString &id) const
{
    // Use findFirst template
    return findFirst(m_fleet, [&id](const Vehicle *v){
        return v->getId().compare(id, Qt::CaseInsensitive) == 0;
    });
}

bool MainWindow::idExists(const QString &id) const
{
    return findById(id) != nullptr;
}

void MainWindow::setStatus(const QString &msg, bool ok)
{
    m_statusLabel->setText(msg);
    m_statusLabel->setStyleSheet(ok ? QString("color:#3AA86E;")
                                    : QString("color:#E05252;"));
}

// ─────────────────────────────────────────────────────────────────────────────
//  STYLESHEET
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::applyStyleSheet()
{
    setStyleSheet(R"(
QMainWindow, QWidget {
    background-color: #1E1E2E;
    color: #CDD6F4;
    font-family: 'Segoe UI', Arial, sans-serif;
    font-size: 13px;
}
#headerBar {
    background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #2A2A3E, stop:1 #1E1E2E);
    border-bottom: 1px solid #313244;
}
#appTitle {
    font-size: 18px;
    font-weight: 600;
    color: #CBA6F7;
    letter-spacing: 0.5px;
}
#mainTabs {
    background: transparent;
    border: none;
}
QTabWidget::pane {
    border: 1px solid #313244;
    border-radius: 0px;
    background: #1E1E2E;
}
QTabBar::tab {
    background: #181825;
    color: #6C7086;
    padding: 10px 22px;
    border: 1px solid #313244;
    border-bottom: none;
    font-size: 13px;
}
QTabBar::tab:selected {
    background: #1E1E2E;
    color: #CBA6F7;
    border-top: 2px solid #CBA6F7;
}
QTabBar::tab:hover { background: #2A2A3E; color: #CDD6F4; }
#formBox, #specBox, #infoBox {
    background: #181825;
    border: 1px solid #313244;
    border-radius: 8px;
    color: #CDD6F4;
}
QGroupBox::title {
    color: #89B4FA;
    font-weight: 600;
    font-size: 13px;
    subcontrol-origin: margin;
    left: 12px;
    top: 4px;
    padding: 0 6px;
}
#formLabel { color: #A6ADC8; font-size: 13px; }
#infoText  { color: #6C7086; font-size: 12px; line-height: 1.6; }
#formEdit, #searchEdit {
    background: #313244;
    border: 1px solid #45475A;
    border-radius: 6px;
    color: #CDD6F4;
    padding: 6px 10px;
    font-size: 13px;
    selection-background-color: #CBA6F7;
}
#formEdit:focus, #searchEdit:focus {
    border: 1px solid #CBA6F7;
}
QDoubleSpinBox, QSpinBox {
    background: #313244;
    border: 1px solid #45475A;
    border-radius: 6px;
    color: #CDD6F4;
    padding: 5px 8px;
    font-size: 13px;
}
QDoubleSpinBox:focus, QSpinBox:focus {
    border: 1px solid #CBA6F7;
}
QComboBox {
    background: #313244;
    border: 1px solid #45475A;
    border-radius: 6px;
    color: #CDD6F4;
    padding: 5px 10px;
    font-size: 13px;
}
QComboBox:focus { border: 1px solid #CBA6F7; }
QComboBox::drop-down { border: none; width: 24px; }
QComboBox QAbstractItemView {
    background: #313244;
    border: 1px solid #45475A;
    color: #CDD6F4;
    selection-background-color: #CBA6F7;
    selection-color: #1E1E2E;
}
#btnPrimary {
    background: #CBA6F7;
    color: #1E1E2E;
    border: none;
    border-radius: 6px;
    font-weight: 600;
    padding: 0 16px;
    font-size: 13px;
}
#btnPrimary:hover    { background: #D4B8FA; }
#btnPrimary:pressed  { background: #B893F5; }
#btnSecondary {
    background: #313244;
    color: #CDD6F4;
    border: 1px solid #45475A;
    border-radius: 6px;
    padding: 0 14px;
    font-size: 13px;
}
#btnSecondary:hover  { background: #3D3D55; }
#btnRent {
    background: #A6E3A1;
    color: #1E1E2E;
    border: none;
    border-radius: 6px;
    font-weight: 600;
    padding: 0 14px;
    font-size: 13px;
}
#btnRent:hover   { background: #B4EDB0; }
#btnReturn {
    background: #89B4FA;
    color: #1E1E2E;
    border: none;
    border-radius: 6px;
    font-weight: 600;
    padding: 0 14px;
    font-size: 13px;
}
#btnReturn:hover { background: #9EC6FC; }
#btnDanger {
    background: #F38BA8;
    color: #1E1E2E;
    border: none;
    border-radius: 6px;
    font-weight: 600;
    padding: 0 14px;
    font-size: 13px;
}
#btnDanger:hover { background: #F5A0B5; }
#vehicleTable {
    background: #181825;
    alternate-background-color: #1E1E2E;
    border: 1px solid #313244;
    border-radius: 6px;
    color: #CDD6F4;
    gridline-color: #313244;
    font-size: 13px;
    selection-background-color: #CBA6F7;
    selection-color: #1E1E2E;
}
QHeaderView::section {
    background: #2A2A3E;
    color: #89B4FA;
    font-weight: 600;
    font-size: 12px;
    border: none;
    border-bottom: 1px solid #45475A;
    padding: 8px 6px;
}
#sectionHeading {
    font-size: 20px;
    font-weight: 700;
    color: #CBA6F7;
    margin-bottom: 8px;
}
#summaryLabel {
    color: #CDD6F4;
    font-size: 14px;
    background: #181825;
    border: 1px solid #313244;
    border-radius: 8px;
    padding: 20px;
    line-height: 2.0;
}
QStatusBar { background: #181825; border-top: 1px solid #313244; }
#statusLabel { font-size: 12px; padding: 4px 12px; }
QScrollBar:vertical {
    background: #181825; width: 10px; border-radius: 5px;
}
QScrollBar::handle:vertical {
    background: #45475A; border-radius: 5px; min-height: 20px;
}
QScrollBar::handle:vertical:hover { background: #6C7086; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
    )");
}
