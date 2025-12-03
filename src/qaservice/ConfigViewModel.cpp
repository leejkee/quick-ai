//
// Created by 31305 on 2025/12/3.
//
#include <ConfigViewModel/ConfigViewModel.h>

namespace QA::Service
{
ConfigViewModel::ConfigViewModel(ConfigService* service, QObject* parent) : QObject(parent), m_service(service)
{
}


}
