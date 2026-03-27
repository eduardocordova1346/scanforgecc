#include "widgets/memory_widget.h"
#include "services/memory_service.h"
#include "utils/formatter.h"

#include <QString>

MemoryWidget::MemoryWidget(QWidget* parent)
    : InfoCardWidget("Memória RAM", parent)
{
    m_totalLabel = addLabel();
    m_usedLabel = addLabel();
    m_availableLabel = addLabel();
    m_usageBar = addProgressBar();
}

void MemoryWidget::refresh()
{
    auto info = MemoryService::fetch();

    m_totalLabel->setText(QString("Total: %1").arg(QString::fromStdString(Formatter::bytesToHuman(info.totalBytes))));
    m_usedLabel->setText(QString("Em uso: %1").arg(QString::fromStdString(Formatter::bytesToHuman(info.usedBytes))));
    m_availableLabel->setText(QString("Disponível: %1").arg(QString::fromStdString(Formatter::bytesToHuman(info.availableBytes))));
    m_usageBar->setValue(static_cast<int>(info.usagePercent));
    m_usageBar->setFormat(QString("Uso: %1%").arg(info.usagePercent, 0, 'f', 1));
}
