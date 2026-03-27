#pragma once

#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>

class InfoCardWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit InfoCardWidget(const QString& title, QWidget* parent = nullptr);

protected:
    QVBoxLayout* contentLayout() const;
    QLabel* addLabel(const QString& text = "");
    QProgressBar* addProgressBar(int min = 0, int max = 100);

private:
    QVBoxLayout* m_contentLayout{};
};
