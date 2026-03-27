#pragma once

#include "widgets/info_card_widget.h"

class MemoryWidget : public InfoCardWidget
{
    Q_OBJECT

public:
    explicit MemoryWidget(QWidget* parent = nullptr);
    void refresh();

private:
    QLabel* m_totalLabel{};
    QLabel* m_usedLabel{};
    QLabel* m_availableLabel{};
    QProgressBar* m_usageBar{};
};
