#include "widgets/cpu_widget.h"
#include "services/cpu_service.h"

#include <QString>

CpuWidget::CpuWidget(QWidget* parent)
    : InfoCardWidget("CPU", parent)
{
    m_modelLabel = addLabel();
    m_coresLabel = addLabel();
    m_frequencyLabel = addLabel();
    m_usageBar = addProgressBar();
}

void CpuWidget::refresh()
{
    auto info = CpuService::fetch();

    m_modelLabel->setText(QString("Modelo: %1").arg(QString::fromStdString(info.modelName)));
    m_coresLabel->setText(QString("Núcleos: %1 | Threads: %2").arg(info.coreCount).arg(info.threadCount));
    m_frequencyLabel->setText(QString("Frequência: %1 MHz").arg(info.frequencyMhz, 0, 'f', 0));
    m_usageBar->setValue(static_cast<int>(info.usagePercent));
    m_usageBar->setFormat(QString("Uso: %1%").arg(info.usagePercent, 0, 'f', 1));
}
