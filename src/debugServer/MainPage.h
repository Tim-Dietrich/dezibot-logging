#ifndef MAINPAGE_H
#define MAINPAGE_H

#include "PageProvider.h"

class MainPage : public PageProvider {
private:
    WebServer* serverPointer;
public:
    explicit MainPage(WebServer* server);
    void handler() override;
};

#endif //MAINPAGE_H
