#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include <WebServer.h>
#include <SPIFFS.h>
#include <Arduino.h>

class PageProvider
{
public:
    virtual ~PageProvider() = default;
    virtual void handler() = 0;

protected:
    virtual String readHtmlFromFile(const char* filename);

    static void serveFileFromSpiffs(WebServer *server, const char *filename, const char *contentType);
};
#endif //PAGEPROVIDER_H
