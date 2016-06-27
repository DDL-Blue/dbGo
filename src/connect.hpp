/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */


#ifndef GOPA2_CONNECT_HPP_B16B00B135A93A55555555M01189998819991197253
#define GOPA2_CONNECT_HPP_B16B00B135A93A55555555M01189998819991197253

#include <list>
#include <cstdint>
#include <pthread.h>
#include <string>
class CConnect;
#include "game.hpp"
#include "constants.hpp"
#include "structures.hpp"

/***
 * This class does all the network communication.
 * You can just push CEvents to it's internal buffer, or get CEvents from the other side.
 * This object will send&receive all the data asynchronously.
 * */
class CConnect {
public:
    /**Constructor*/
    CConnect();
    virtual ~CConnect();

    /**Get current status of the connection.
     * @return current connection status.
     * */
    ConnectionStatus getStatus();

    /**
     * Get received event.
     * @return gets pointer to the received event, or nullptr if nothing new received. These pointers will not be freed.
     * */
    CEvent* getEvent ();

    /**
     * Send event to other player.
     * @param [in] event Pointer to the event you want to send. The pointer will be freed.
     * */
    void SendEvent ( CEvent * event);

    /**
     * Creates a communication socket. No transmission will happen if you don't call this.
     * Do not call this if connection already established
     * @param [in] srvName Name of the server.
     * @param [in] srvPort Connection port.
     * @return Returns connection status.
     * */
    virtual ConnectionStatus CreateSocket( const char * srvName, int srvPort ) = 0;

protected:
    /**Inicialize the communication thread and start the data transmission*/
    void StartComunication();

    /**Communication thread*/
    pthread_t m_thread;

    /** comunication socket*/
    int m_socket;
    /**Current connection status. Protected by m_mutexStatus*/
    ConnectionStatus m_status;
    /**List of recived CEvents, protected by m_mutexLists*/
    std::list<CEvent*> m_recivedEvents;
    /**List of CEvents that are waiting to be sent, protected by m_mutexLists*/
    std::list<CEvent*> m_toSendEvents;

    /**Mutex that protects shared lists*/
    pthread_mutex_t m_mutexLists;
    /**Mutex that protects m_status*/
    pthread_mutex_t m_mutexStatus;

    /**
     * Function run by the communication thread. Receives and sends all the events.
     * @param [in] connection Pointer to CConnection object that owns the thread.
     * @return Undefined.
     * */
    friend void *ConnectThread(void *connection);
};

/*############################################################################################################################*/
/**
 * Function run by the communication thread. Receives and sends all the events.
 * @param [in] connection Pointer to CConnection object that owns the thread.
 * @return Undefined.
 * */
void* ConnectThread( void * connection);

/*############################################################################################################################*/
/**Client version of the connection*/
class CConnectClient: public CConnect{
public:
    /**
     * Creates a communication socket. No transmission will happen if you don't call this.
     * Do not call this if connection already established
     * This is a client version of the socket.
     * @param [in] srvName Name of the server.
     * @param [in] srvPort Connection port.
     * @return Returns connection status.
     * */
    virtual ConnectionStatus CreateSocket( const char * srvName, int srvPort ) override;
};

/*############################################################################################################################*/
/**Server version of the connection*/
class CConnectServer: public CConnect{
public:
    /**
     * Creates a communication socket. No transmission will happen if you don't call this.
     * Do not call this if connection already established
     * This is a server version of the socket. Will stuck until a client connects.
     * @param [in] srvName Name or IP it will only listen to. Set to 0.0.0.0 to allow access from any address.
     * @param [in] srvPort Connection port.
     * @return Returns connection status.
     * */
    virtual ConnectionStatus CreateSocket( const char * srvName, int srvPort ) override;
};

#endif //GOPA2_CONNECT_HPP_B16B00B135A93A55555555M01189998819991197253
