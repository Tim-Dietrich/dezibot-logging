/**
 * @file LoggingPage.h
 * @author Tim Dietrich, Felix Herrling
 * @brief This component implements the logging page for the webserver.
 * Log entries are read from the logdatabase and sent to the client.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef LOGGINGPAGE_H
#define LOGGINGPAGE_H

#include "PageProvider.h"

class LoggingPage : public PageProvider{
private:
    WebServer* serverPointer;

    /**
     * @brief Read log entries from logdatabase, format as json and send to the client.
     * @return void
     */
    void sendLogs() const;

    void sendNewLogs();

public:
    explicit LoggingPage(WebServer* server);

    /**
     * @brief provides html on initial request from client
     * @return void
     */
    void handler() override;
};

#endif //LOGGINGPAGE_H
