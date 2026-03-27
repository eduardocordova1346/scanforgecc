#include "widgets/temperature_widget.h"
#include "services/temperature_service.h"

#include <QString>

TemperatureWidget::TemperatureWidget(QWidget* parent)
    : InfoCardWidget("Temperatura", parent)
{
}

void TemperatureWidget::refresh()
{
    auto info = TemperatureService::fetch();

    // Recria labels se a quantidade de sensores mudar
    if (m_sensorLabels.size() != info.sensors.size()) {
        for (auto* label : m_sensorLabels) {
            contentLayout()->removeWidget(label);
            delete label;
        }
        m_sensorLabels.clear();

        for (size_t i = 0; i < info.sensors.size(); ++i)
            m_sensorLabels.push_back(addLabel());
    }

    for (size_t i = 0; i < info.sensors.size(); ++i) {
        const auto& sensor = info.sensors[i];
        if (sensor.temperatureCelsius < 0) {
            m_sensorLabels[i]->setText(
                "Temperatura indisponível neste SO.\n"
                "Requer integração com LibreHardwareMonitor (Windows)\n"
                "ou lm-sensors (Linux).");
        } else {
            m_sensorLabels[i]->setText(
                QString("%1: %2 °C")
                    .arg(QString::fromStdString(sensor.label))
                    .arg(sensor.temperatureCelsius, 0, 'f', 1));
        }
    }
}
