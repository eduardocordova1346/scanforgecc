#pragma once

#include "widgets/info_card_widget.h"

#include <vector>

class TemperatureWidget : public InfoCardWidget
{
    Q_OBJECT

public:
    explicit TemperatureWidget(QWidget* parent = nullptr);
    void refresh();

private:
    std::vector<QLabel*> m_sensorLabels;
};
