#include "widgets/system_widget.h"
#include "services/system_info_service.h"
#include "utils/formatter.h"

#include <QString>

SystemWidget::SystemWidget(QWidget* parent)
    : InfoCardWidget("Sistema", parent)
{
    m_osLabel = addLabel();
    m_versionLabel = addLabel();
    m_hostnameLabel = addLabel();
    m_archLabel = addLabel();
    m_uptimeLabel = addLabel();
}

void SystemWidget::refresh()
{
    auto info = SystemInfoService::fetch();

    m_osLabel->setText(QString("SO: %1").arg(QString::fromStdString(info.osName)));
    m_versionLabel->setText(QString("Versão: %1").arg(QString::fromStdString(info.osVersion)));
    m_hostnameLabel->setText(QString("Hostname: %1").arg(QString::fromStdString(info.hostname)));
    m_archLabel->setText(QString("Arquitetura: %1").arg(QString::fromStdString(info.architecture)));
    m_uptimeLabel->setText(QString("Uptime: %1").arg(QString::fromStdString(Formatter::secondsToUptime(info.uptimeSeconds))));
}
