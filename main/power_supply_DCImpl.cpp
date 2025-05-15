// SPDX-License-Identifier: Apache-2.0
// Copyright Pionix GmbH and Contributors to EVerest

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  WARNING!!! THIS MODULE IS ONLY FOR EXPERIMENTAL USE AND NOT FOR A REAL CHARGING STATION !!!! WARNING ///////////////
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  WARNING!!! THIS MODULE IS ONLY FOR EXPERIMENTAL USE AND NOT FOR A REAL CHARGING STATION !!!! WARNING ///////////////
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  WARNING!!! THIS MODULE IS ONLY FOR EXPERIMENTAL USE AND NOT FOR A REAL CHARGING STATION !!!! WARNING ///////////////
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  WARNING!!! THIS MODULE IS ONLY FOR EXPERIMENTAL USE AND NOT FOR A REAL CHARGING STATION !!!! WARNING ///////////////
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "power_supply_DCImpl.hpp"

#include <memory>

#include "mqtt_broker.hpp"

#include <fmt/core.h>
#include <utils/formatter.hpp>

std::unique_ptr<MqttBroker> mqtt_broker;

namespace module {
namespace main {


void power_supply_DCImpl::init() {
    current = 0;
    voltage = 0;

    config_current_limit = mod->config.current_limit_A;
    config_voltage_limit = mod->config.voltage_limit_V;
    config_min_current_limit = mod->config.min_current_limit_A;
    config_min_voltage_limit = mod->config.min_voltage_limit_V;
    config_power_limit = mod->config.power_limit_W;

    if (!mod->config.discharge_gpio_chip.empty()) {
        discharge_gpio.open(mod->config.discharge_gpio_chip, mod->config.discharge_gpio_line,
                            !mod->config.discharge_gpio_polarity);
        discharge_gpio.set_output(false);
    }
    //mqtt_broker = std::make_unique<MqttBroker>
    //can_broker = std::make_unique<CanBroker>(mod->config.device, mod->config.device_address);

    // ensure the module is switched off
    //can_broker->set_state(false);

    // Configure module for series or parallel mode
    // 0 is automatic switching mode
    float series_parallel_mode = 0.;

    if (mod->config.series_parallel_mode == "Series") {
        series_parallel_mode = 1050.;
//        config_min_voltage_limit = 10.;
        parallel_mode = false;
    } else if (mod->config.series_parallel_mode == "Parallel") {
        series_parallel_mode = 520.;
        if (config_voltage_limit > 520) {
            config_voltage_limit = 520;
        }
        parallel_mode = true;
    }

    // WTF: This really uses a float to set one of the three modes automatic, series or parallel.
    //auto status = can_broker->set_data(dpm1000::def::SetValueType::SERIES_PARALLEL_MODE, series_parallel_mode);
    //log_status_on_fail("Set current limit failed", status);
}

void power_supply_DCImpl::ready() {
    types::power_supply_DC::Capabilities caps;
    
    caps.bidirectional = true;
    caps.max_export_current_A = config_current_limit;
    caps.max_export_voltage_V = config_voltage_limit;
    caps.min_export_current_A = config_min_current_limit;
    caps.min_export_voltage_V = config_min_voltage_limit;
    caps.max_export_power_W = config_power_limit;
    caps.current_regulation_tolerance_A = 0.5;
    caps.peak_current_ripple_A = 1;
    caps.conversion_efficiency_export = 0.95;
    publish_capabilities(caps);
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        // Send voltage, current and power limits
        mqtt_broker->write_voltage_current(voltage, current);
//        EVLOG_info << "voltage & current of EV "
//                    << voltage
//                    << " "
//                    << current;

        float v_tmp;
        float c_tmp;
        // Read voltage and current
        types::power_supply_DC::VoltageCurrent vc;
        mqtt_broker->read_voltage(v_tmp);
        mqtt_broker->read_current(c_tmp);
        vc.voltage_V = v_tmp;
        vc.current_A = c_tmp;
//        EVLOG_info << "voltage & current of SE "
//                   << v_tmp
//                   << " "
//                   << c_tmp;
        publish_voltage_current(vc);

         //read alarm flags

         //Discharge output if it is higher then setpoint voltage.
         //Note that this has no timeout, so HW must be designed to sustain the worst case load (e.g. 1000V) continously
        if (vc.voltage_V > (voltage + 10)) {
            discharge_gpio.set(true);
        } else {
            discharge_gpio.set(false);
        }


    }
}


void power_supply_DCImpl::handle_setMode(types::power_supply_DC::Mode& value,
                                        types::power_supply_DC::ChargingPhase& phase) {
//    if (value == types::power_supply_DC::Mode::Export) {
//        can_broker->set_state(true);
//    } else {
//        can_broker->set_state(false);
//    }
}

void power_supply_DCImpl::handle_setExportVoltageCurrent(double& voltage, double& current) {
    if (voltage <= config_voltage_limit && voltage >= config_min_voltage_limit && current <= config_current_limit && current >= config_min_current_limit) {
        this->voltage = voltage;
        this->current = current;
    } else {
        EVLOG_error << fmt::format("Out of range voltage/current settings ignored: {}V / {}A", voltage, current);
    }
}

void power_supply_DCImpl::handle_setImportVoltageCurrent(double& voltage, double& current) {
    if (voltage <= config_voltage_limit && voltage >= config_min_voltage_limit && current <= config_current_limit && current >= config_min_current_limit) {
        this->voltage = voltage;
        this->current = current;
    } else {
        EVLOG_error << fmt::format("Out of range voltage/current settings ignored: {}V / {}A", voltage, current);
    }
}

} // namespace main
} // namespace module
