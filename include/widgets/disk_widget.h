#pragma once

#include "widgets/info_card_widget.h"

#include <vector>

class DiskWidget : public InfoCardWidget
{
    Q_OBJECT

public:
    explicit DiskWidget(QWidget* parent = nullptr);
    void refresh();

private:
    struct PartitionRow {
        QLabel* label{};
        QProgressBar* bar{};
    };
    std::vector<PartitionRow> m_rows;
};
