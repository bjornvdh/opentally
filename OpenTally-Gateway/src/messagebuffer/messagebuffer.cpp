#include <Arduino.h>
#include <RingBuf.h>

#include "messagebuffer.h"

RingBuf<MessageStruct, 128> _messageRingBuffer;
static SemaphoreHandle_t msgBufMutex = xSemaphoreCreateMutex();
void messagebuffer_queuemessage(MessageStruct msg)
{
    xSemaphoreTake(msgBufMutex, portMAX_DELAY);
    _messageRingBuffer.push(msg);
    xSemaphoreGive(msgBufMutex);
}

bool messagebuffer_ismessagewaiting()
{
    return !_messageRingBuffer.isEmpty();
}

MessageStruct messagebuffer_getnextmessage()
{
    if(!_messageRingBuffer.isEmpty())
    {
        MessageStruct msg;
        xSemaphoreTake(msgBufMutex, portMAX_DELAY);
        _messageRingBuffer.pop(msg);
        xSemaphoreGive(msgBufMutex);
        return msg;
    }
}
