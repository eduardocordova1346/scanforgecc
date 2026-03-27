#include "widgets/disk_widget.h"
#include "services/disk_service.h"
#include "utils/formatter.h"

#include <QString>

DiskWidget::DiskWidget(QWidget* parent)
    : InfoCardWidget("Disco", parent)
{
}

void DiskWidget::refresh()
{
    auto info = DiskService::fetch();

    // Recria rows se a quantidade de partições mudar
    if (m_rows.size() != info.partitions.size()) {
        for (auto& row : m_rows) {
            contentLayout()->removeWidget(row.label);
            contentLayout()->removeWidget(row.bar);
            delete row.label;
            delete row.bar;
        }
        m_rows.clear();

        for (size_t i = 0; i < info.partitions.size(); ++i) {
            PartitionRow row;
            row.label = addLabel();
            row.bar = addProgressBar();
            m_rows.push_back(row);
        }
    }

    for (size_t i = 0; i < info.partitions.size(); ++i) {
        const auto& part = info.partitions[i];
        m_rows[i].label->setText(
            QString("%1 [%2] — %3 / %4")
                .arg(QString::fromStdString(part.mountPoint))
                .arg(QString::fromStdString(part.fileSystem))
                .arg(QString::fromStdString(Formatter::bytesToHuman(part.usedBytes)))
                .arg(QString::fromStdString(Formatter::bytesToHuman(part.totalBytes))));
        m_rows[i].bar->setValue(static_cast<int>(part.usagePercent));
        m_rows[i].bar->setFormat(QString("%1%").arg(part.usagePercent, 0, 'f', 1));
    }
}
