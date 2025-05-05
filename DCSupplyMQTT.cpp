// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2023 chargebyte GmbH
// Copyright (C) 2023 Contributors to EVerest


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

#include <iostream>
#include <thread>

#include "main/mqtt_broker.hpp"
#include "DCSupplyMQTT.hpp"

namespace module {

void DCSupplyMQTT::init() {
    invoke_init(*p_main);

        std::string dcs_base = "everest_dcs/power_supply/";
        std::string cmd_base = dcs_base + "cmd/";
        std::string cmd_voltage = cmd_base + "voltage_ps";

        float v=0;


        this->mqtt.subscribe(cmd_voltage, [this, &v](const std::string &data) {
            try {
                v = std::stof(data);
//                EVLOG_info << "Voltage DCS: "
//                           << v;
                voltage_ps_in=v;
            } catch (const std::invalid_argument &e) {
                EVLOG_warning << "Invalid value.";
            } catch (const std::out_of_range &e) {
                EVLOG_warning << "Invalid value: Out of range.";
            }
        });
        std::string cmd_current = cmd_base + "current_ps";
        float c=0;
        this->mqtt.subscribe(cmd_current, [this, &c](const std::string &data) {
            try {
                c = std::stof(data);
//                EVLOG_info << "Current of DCS: "
//                           << c;
                current_ps_in = c;
                float c_out = current_ev_out;
                float v_out = voltage_ev_out;

                json vc_json = json::object({{"DC_EVTargetCurrent",                    c_out},
                                             {"DC_EVTargetVoltage",                    v_out}});
//                EVLOG_info << "voltage & current of Vehicle "
//                           << v_out
//                           << " "
//                           << c_out;
                std::string var_voltage_current = "everest_dcs/power_supply/var/voltage_current_ev";
                this->mqtt.publish(var_voltage_current, vc_json.dump());
            } catch (const std::invalid_argument &e) {
                EVLOG_warning << "Invalid value.";
            } catch (const std::out_of_range &e) {
                EVLOG_warning << "Invalid value: Out of range.";
            }
        });
}

void DCSupplyMQTT::ready() {
    invoke_ready(*p_main);
}
} // namespace module
