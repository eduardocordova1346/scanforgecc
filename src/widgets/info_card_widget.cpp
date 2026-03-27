#include "widgets/info_card_widget.h"

InfoCardWidget::InfoCardWidget(const QString& title, QWidget* parent)
    : QGroupBox(title, parent)
{
    m_contentLayout = new QVBoxLayout(this);
    m_contentLayout->setContentsMargins(8, 16, 8, 8);
}

QVBoxLayout* InfoCardWidget::contentLayout() const
{
    return m_contentLayout;
}

QLabel* InfoCardWidget::addLabel(const QString& text)
{
    auto* label = new QLabel(text, this);
    m_contentLayout->addWidget(label);
    return label;
}

QProgressBar* InfoCardWidget::addProgressBar(int min, int max)
{
    auto* bar = new QProgressBar(this);
    bar->setMinimum(min);
    bar->setMaximum(max);
    bar->setTextVisible(true);
    m_contentLayout->addWidget(bar);
    return bar;
}
