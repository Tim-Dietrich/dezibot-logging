#ifndef LOGGINGPAGE_H
#define LOGGINGPAGE_H

#include "PageProvider.h"

class LoggingPage : public PageProvider{
private:
    WebServer* serverPointer;
public:
    explicit LoggingPage(WebServer* server);
    void handler() override;
};

#endif //LOGGINGPAGE_H
