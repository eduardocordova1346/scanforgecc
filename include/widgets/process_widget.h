#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLabel>

class ProcessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessWidget(QWidget* parent = nullptr);
    void refresh();

private:
    QTableWidget* m_table{};
    QLabel* m_countLabel{};

    static constexpr int MAX_DISPLAY_ROWS = 50;
};
