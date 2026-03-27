#include "mainwindow.h"
#include "widgets/cpu_widget.h"
#include "widgets/memory_widget.h"
#include "widgets/system_widget.h"
#include "widgets/temperature_widget.h"
#include "widgets/disk_widget.h"
#include "widgets/process_widget.h"

#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();
    setupTimer();
    refreshData();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi()
{
    setWindowTitle("ScanForge CC — Diagnóstico de Sistema");
    resize(900, 700);

    auto* centralWidget = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(centralWidget);

    // Título
    auto* titleLabel = new QLabel("ScanForge CC");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    auto* subtitleLabel = new QLabel("Ferramenta de diagnóstico para técnicos de informática");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subtitleLabel);

    mainLayout->addSpacing(8);

    // Abas
    m_tabs = new QTabWidget(this);

    // --- Aba 1: Visão Geral (CPU, RAM, Sistema, Temperatura) ---
    auto* overviewTab = new QWidget();
    auto* grid = new QGridLayout(overviewTab);
    grid->setSpacing(12);

    m_cpuWidget = new CpuWidget(overviewTab);
    m_memoryWidget = new MemoryWidget(overviewTab);
    m_systemWidget = new SystemWidget(overviewTab);
    m_temperatureWidget = new TemperatureWidget(overviewTab);

    grid->addWidget(m_cpuWidget, 0, 0);
    grid->addWidget(m_memoryWidget, 0, 1);
    grid->addWidget(m_systemWidget, 1, 0);
    grid->addWidget(m_temperatureWidget, 1, 1);

    m_tabs->addTab(overviewTab, "Visão Geral");

    // --- Aba 2: Disco ---
    m_diskWidget = new DiskWidget();
    m_tabs->addTab(m_diskWidget, "Disco");

    // --- Aba 3: Processos ---
    m_processWidget = new ProcessWidget();
    m_tabs->addTab(m_processWidget, "Processos");

    mainLayout->addWidget(m_tabs);
    setCentralWidget(centralWidget);

    // Barra de status
    statusBar()->showMessage(
        QString("Atualização automática a cada %1s").arg(REFRESH_INTERVAL_MS / 1000));
}

void MainWindow::setupTimer()
{
    connect(&m_refreshTimer, &QTimer::timeout, this, &MainWindow::refreshData);
    m_refreshTimer.start(REFRESH_INTERVAL_MS);
}

void MainWindow::refreshData()
{
    m_cpuWidget->refresh();
    m_memoryWidget->refresh();
    m_systemWidget->refresh();
    m_temperatureWidget->refresh();
    m_diskWidget->refresh();
    m_processWidget->refresh();
}
