//
// Created by 31305 on 2026/2/10.
//
#include <ChatService/SessionService.h>

namespace QA::Service
{

 SessionService::SessionService(ChatService* chatService, QObject* parent) : QObject(parent), m_chatService(chatService)
 {

 }

}