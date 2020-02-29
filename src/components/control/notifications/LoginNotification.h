//
// Created by Simon Willis on 20/05/2016.
//

#ifndef CORE_LOGINNOTIFICATION_H
#define CORE_LOGINNOTIFICATION_H

#include <string>

#include "Notification.hpp"


class LoginNotification : public Notification {

public:
    LoginNotification(int errorCode = 0, const char * errorMessage = "OK", int dialogueId = 0)
        : Notification("login", 0, errorCode, errorMessage, dialogueId) {

    }

};


#endif //CORE_LOGINNOTIFICATION_H
