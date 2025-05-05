// SPDX-License-Identifier: Apache-2.0
// Copyright chargebyte GmbH and Contributors to EVerest
#ifndef DCSUPPLY_MQTT_HPP
#define DCSUPPLY_MQTT_HPP

//
// AUTO GENERATED - MARKED REGIONS WILL BE KEPT
// template version 2
//

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

#include "ld-ev.hpp"

// headers for provided interface implementations
#include <generated/interfaces/power_supply_DC/Implementation.hpp>

//#include <generated/interfaces/ISO15118_charger/Interface.hpp>


// ev@4bf81b14-a215-475c-a1d3-0a484ae48918:v1
// insert your custom include headers here
// ev@4bf81b14-a215-475c-a1d3-0a484ae48918:v1

//class DCSupply {
//public:
//    DCSupply(Everest::MqttProvider& mqtt);
//
//    void write_vc_mqtt();
//    Everest::MqttProvider& mqtt;
//
//private:
////    float voltage;
////    float current;
//};

namespace module {



struct Conf {
    std::string device;
    int device_address;
    double power_limit_W;
    double current_limit_A;
    double voltage_limit_V;
    double min_current_limit_A;
    double min_voltage_limit_V;
    std::string series_parallel_mode;
    std::string discharge_gpio_chip;
    int discharge_gpio_line;
    bool discharge_gpio_polarity;
    bool debug_print_all_telemetry;
};

class DCSupplyMQTT : public Everest::ModuleBase {
public:
    DCSupplyMQTT() = delete;
    DCSupplyMQTT(const ModuleInfo& info, Everest::MqttProvider& mqtt_provider, std::unique_ptr<power_supply_DCImplBase> p_main, Conf& config) :
        ModuleBase(info), mqtt(mqtt_provider), p_main(std::move(p_main)), config(config){};


    Everest::MqttProvider& mqtt;
    const std::unique_ptr<power_supply_DCImplBase> p_main;

    const Conf& config;

    // ev@1fce4c5e-0ab8-41bb-90f7-14277703d2ac:v1
    // insert your public definitions here
    // ev@1fce4c5e-0ab8-41bb-90f7-14277703d2ac:v1

//    void write_vc_mqtt(float voltage, float current);

protected:
    // ev@4714b2ab-a24f-4b95-ab81-36439e1478de:v1
    // insert your protected definitions here
    // ev@4714b2ab-a24f-4b95-ab81-36439e1478de:v1

private:
    friend class LdEverest;
    void init();
    void ready();

//    float voltage;
//    float current;
    // ev@211cfdbe-f69a-4cd6-a4ec-f8aaa3d1b6c8:v1
    // insert your private definitions here
    // ev@211cfdbe-f69a-4cd6-a4ec-f8aaa3d1b6c8:v1
};

// ev@087e516b-124c-48df-94fb-109508c7cda9:v1
// insert other definitions here
// ev@087e516b-124c-48df-94fb-109508c7cda9:v1

} // namespace module

#endif // DCSUPPLY_SIMULATOR_HPP
