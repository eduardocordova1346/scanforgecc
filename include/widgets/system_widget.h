#pragma once

#include "widgets/info_card_widget.h"

class SystemWidget : public InfoCardWidget
{
    Q_OBJECT

public:
    explicit SystemWidget(QWidget* parent = nullptr);
    void refresh();

private:
    QLabel* m_osLabel{};
    QLabel* m_versionLabel{};
    QLabel* m_hostnameLabel{};
    QLabel* m_archLabel{};
    QLabel* m_uptimeLabel{};
};
