#include "widgets/process_widget.h"
#include "services/process_service.h"
#include "utils/formatter.h"

#include <QString>
#include <QHeaderView>

ProcessWidget::ProcessWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);

    auto* titleLabel = new QLabel("Processos (por uso de memória)");
    QFont font = titleLabel->font();
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    m_countLabel = new QLabel();
    layout->addWidget(m_countLabel);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({"PID", "Nome", "Memória", "CPU %"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    layout->addWidget(m_table);
}

void ProcessWidget::refresh()
{
    auto list = ProcessService::fetch();

    int totalCount = static_cast<int>(list.entries.size());
    int displayCount = std::min(totalCount, MAX_DISPLAY_ROWS);

    m_countLabel->setText(QString("Total: %1 processos (exibindo top %2)")
        .arg(totalCount).arg(displayCount));

    m_table->setRowCount(displayCount);

    for (int i = 0; i < displayCount; ++i) {
        const auto& proc = list.entries[i];

        m_table->setItem(i, 0, new QTableWidgetItem(QString::number(proc.pid)));
        m_table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(proc.name)));
        m_table->setItem(i, 2, new QTableWidgetItem(
            QString::fromStdString(Formatter::bytesToHuman(proc.memoryBytes))));
        m_table->setItem(i, 3, new QTableWidgetItem(
            proc.cpuPercent > 0 ? QString("%1%").arg(proc.cpuPercent, 0, 'f', 1) : "—"));
    }
}
