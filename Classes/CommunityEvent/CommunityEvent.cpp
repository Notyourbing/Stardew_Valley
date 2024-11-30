#include "CommunityEvent.h"

CommunityEvent* CommunityEvent::create(const std::string& name, const std::string& description, const std::string& date) {
    // ���� CommunityEvent ����
    CommunityEvent* newEvent = new CommunityEvent();
    // ��ʼ������
    if (newEvent && newEvent->init(name, description, date)) {
        return newEvent;
    }
    else {
        delete newEvent;  // ��ʼ��ʧ��ʱ���ͷ��ڴ�
        return nullptr;
    }
}

bool CommunityEvent::init(const std::string& name, const std::string& description, const std::string& date) {
    // ��ʼ������
    eventName = name;
    eventDescription = description;
    eventDate = date;
    return true;  // �����ʼ���ɹ�
}
void CommunityEvent::startEvent(DateManage* dateManage) {
    // �¼���ʼʱ���߼�����ʾ�����
    displayEventDetails();
}

void CommunityEvent::displayEventDetails() const {
    CCLOG("Event: %s\n", eventName);
    CCLOG("Description: %s\n", eventDescription);
    CCLOG("Date: %s\n", eventDate);
}

std::string CommunityEvent::getEventName() const {
    return eventName;
}

std::string CommunityEvent::getEventDescription() const {
    return eventDescription;
}

std::string CommunityEvent::getEventDate() const {
    return eventDate;
}