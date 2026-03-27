#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QTabWidget>

class CpuWidget;
class MemoryWidget;
class SystemWidget;
class TemperatureWidget;
class DiskWidget;
class ProcessWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void refreshData();

private:
    void setupUi();
    void setupTimer();

    QTabWidget* m_tabs{};

    CpuWidget* m_cpuWidget{};
    MemoryWidget* m_memoryWidget{};
    SystemWidget* m_systemWidget{};
    TemperatureWidget* m_temperatureWidget{};
    DiskWidget* m_diskWidget{};
    ProcessWidget* m_processWidget{};

    QTimer m_refreshTimer;

    // Intervalo de atualização configurável (ms)
    static constexpr int REFRESH_INTERVAL_MS = 2000;
};
