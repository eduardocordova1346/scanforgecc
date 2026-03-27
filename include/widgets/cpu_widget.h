#pragma once

#include "widgets/info_card_widget.h"

class CpuWidget : public InfoCardWidget
{
    Q_OBJECT

public:
    explicit CpuWidget(QWidget* parent = nullptr);
    void refresh();

private:
    QLabel* m_modelLabel{};
    QLabel* m_coresLabel{};
    QLabel* m_frequencyLabel{};
    QProgressBar* m_usageBar{};
};
