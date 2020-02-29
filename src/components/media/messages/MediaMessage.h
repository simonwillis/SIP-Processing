//
// Created by Simon Willis on 09/06/2016.
//

#ifndef MEDIA_MEDIAMESSAGE_H
#define MEDIA_MEDIAMESSAGE_H

#include <string>

#include "../../../../lib/json/json.h"

class MediaMessage {

public:
    enum class Type {
        UNKNOWN,
        REQUEST,
        RESPONSE
    };
    enum class Method {
        UNKNOWN,
        REGISTER,
        ALLOCATE,
        RELEASE,
        RECORD_START,
        RECORD_STOP,
        PLAY_START,
        PLAY_STOP,
        EVENTS
    };

    MediaMessage(int dialogueId, uint32_t callbackId)
            : dialogueId(dialogueId), callbackId(callbackId) {
        fprintf(stderr, "MediaMessage::CTOR dialogueId=%d, callbackId=%u\n", dialogueId, callbackId);
    }

    MediaMessage(std::string json);

    std::string toJson();

    int getDialogueId() { return dialogueId; }

    uint32_t getCallbackId() { return callbackId; }

    const char * getTypeStr() {
        return getTypeText(getType());
    }

    const char * getDomainStr() {
        return "media";
    }

    const char * getMethodStr() {
        return getMethodText(getMethod());
    }

    virtual const Type getType() = 0;
    virtual const Method getMethod() = 0;

protected:

    virtual bool hasData() { return false; }
    virtual Json::Value getData() { return Json::Value::nullRef; }

    virtual bool hasStatus() = 0;
    virtual Json::Value getStatus() = 0;

public:

    MediaMessage() {
        fprintf(stderr, "Using empty what should be private CTOR of MediaMessage\n");
    }

    static MediaMessage * build_message(const char * json);

private:
    const static char * getTypeText(Type type);
    const static char * getMethodText(Method method);

private:

    int dialogueId;
    uint32_t callbackId;
    std::string jsonData;
};


#endif //MEDIA_MEDIAMESSAGE_H
