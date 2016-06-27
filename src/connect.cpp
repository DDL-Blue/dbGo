/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */

#include "connect.hpp"
#include <sys/types.h>

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>

void* ConnectThread( void * connection){
    CConnect *conn = (CConnect *) connection;

    uint8_t buffer[EVENT_DATA_MAX_SIZE];
    int len;

    while (true) {
        pthread_mutex_lock(&(conn->m_mutexStatus));
        CEvent * toSend;
        if (conn->m_status == CONN_OK) {
            pthread_mutex_unlock(&(conn->m_mutexStatus));
            //send stuff
            pthread_mutex_lock(&(conn->m_mutexLists));
            if (conn->m_toSendEvents.size()){
                toSend = conn->m_toSendEvents.back(); //take an event
                conn->m_toSendEvents.pop_back();
            }else{
                toSend = new CEvent(); //create empty event
                toSend->WriteData (EV_NONE);
            }
            pthread_mutex_unlock(&(conn->m_mutexLists));

            //send it
            len = toSend->m_data.size();
            toSend->ResetCursor();
            for (int32_t i = 0; i < len; ++i) {
                toSend->ReadData(buffer[i]);
            }
            send (conn->m_socket, buffer, len, 0);
            delete toSend;

            //recive stuff
            len = recv (conn->m_socket, buffer, EVENT_DATA_MAX_SIZE, 0);
            if (len <=0 ) { //end of stream
                pthread_mutex_lock(&(conn->m_mutexStatus));
                conn->m_status = CONN_ENDED;
                pthread_mutex_unlock(&(conn->m_mutexStatus));
                return nullptr;
            }

            CEvent * ev = new CEvent();
            int bufferCursor = 0;
            for (int i = 0; i < len; ++i) {
                ev->WriteData( buffer[bufferCursor] );
                ++bufferCursor;
            }
            ;
            pthread_mutex_lock(&(conn->m_mutexLists));

            conn->m_recivedEvents.push_front(ev);
            pthread_mutex_unlock(&(conn->m_mutexLists));
        } else {
            pthread_mutex_unlock(&(conn->m_mutexStatus)); //end of the stream
            return nullptr;
        }
    sleep (1);
    }
}


/*############################################################################################################################*/
CConnect::CConnect() : m_socket (-1), m_status (CONN_UNINITIALIZED) {
    m_mutexLists = PTHREAD_MUTEX_INITIALIZER;
    m_mutexStatus = PTHREAD_MUTEX_INITIALIZER;

    CEvent * ev = new CEvent;
    ev->WriteData (EV_NONE);
    m_toSendEvents.push_front(ev);
}

/*----------------------------------------------------------------------*/
CConnect::~CConnect() {
    pthread_mutex_lock ( &m_mutexStatus );
    if (m_status==CONN_OK || m_status == CONN_ENDED) {
        m_status = CONN_UNINITIALIZED;
        pthread_mutex_unlock ( &m_mutexStatus );
        pthread_join(m_thread, NULL);
    }else{
        pthread_mutex_unlock ( &m_mutexStatus );
    }
    close (m_socket);


    for (auto s : m_recivedEvents){
        delete s;
    }
    for (auto s : m_toSendEvents){
        delete s;
    }

}

/*----------------------------------------------------------------------*/
void CConnect::SendEvent(CEvent *event) {

    pthread_mutex_lock (&m_mutexLists);
    m_toSendEvents.push_front (event);
    pthread_mutex_unlock (&m_mutexLists);
}

/*----------------------------------------------------------------------*/
CEvent* CConnect::getEvent() {

    CEvent * rtrn;
    pthread_mutex_lock (&m_mutexLists);
    if (m_recivedEvents.size()<=0) {
        pthread_mutex_unlock (&m_mutexLists);
        return nullptr;
    }
    rtrn = m_recivedEvents.front();
    m_recivedEvents.pop_front();
    pthread_mutex_unlock (&m_mutexLists);

    return rtrn;
}

/*----------------------------------------------------------------------*/
void CConnect::StartComunication() {
    int ret = pthread_create( &m_thread, NULL, ConnectThread, (void*) this);
    if (ret){throw "CConnect::StartComunication::pthread_create failed";}
}

/*----------------------------------------------------------------------*/
ConnectionStatus CConnect::getStatus() {
    ConnectionStatus rtrn;
    pthread_mutex_lock (&m_mutexStatus);
    rtrn = m_status;
    pthread_mutex_unlock (&m_mutexStatus);
    return rtrn;
}


/*############################################################################################################################*/

ConnectionStatus CConnectServer::CreateSocket( const char * srvName, int srvPort ) {
    /*Function is an edited version of TCP/IP example 2015/16 at FIT CTU Edux page
     * https://edux.fit.cvut.cz/courses/BI-PA2/semestralka
     * by L. Vagner
     * */

    struct addrinfo * ai;
    char srvPortText[10];

    snprintf ( srvPortText, sizeof ( srvPortText ), "%d", srvPort );

    if ( getaddrinfo ( srvName, srvPortText, NULL, &ai ) != 0 )
    {
        m_status = CONN_ERR_ADDRINFO;
        return m_status;
    }
    int s = socket ( ai -> ai_family, SOCK_STREAM, 0 );
    if ( s == -1 )
    {
        freeaddrinfo ( ai );
        m_status = CONN_ERR_SOCKET;
        return m_status;
    }

    if ( bind ( s, ai -> ai_addr, ai -> ai_addrlen ) != 0 )
    {
        close ( s );
        freeaddrinfo ( ai );
        m_status = CONN_ERR_BIND;
        return m_status;
    }

    if ( listen ( s, 5 ) != 0 )
    {
        close ( s );
        freeaddrinfo ( ai );
        m_status = CONN_ERR_LISTEN;
        return m_status;
    }
    freeaddrinfo ( ai );

    //accept
    struct sockaddr addr;
    socklen_t addrLen = sizeof ( addr );
    m_socket = accept ( s, &addr, &addrLen );
    m_status = CONN_OK;

    StartComunication();

    return m_status;
}

/*############################################################################################################################*/
ConnectionStatus CConnectClient::CreateSocket( const char * srvName, int srvPort ){
    /*Function is an edited version of TCP/IP example 2015/16 at FIT CTU Edux page
     * https://edux.fit.cvut.cz/courses/BI-PA2/semestralka
     * by L. Vagner
     * */
    struct addrinfo * ai;
    char srvPortText[10];

    snprintf ( srvPortText, sizeof ( srvPortText ), "%d", srvPort );

    if ( getaddrinfo ( srvName, srvPortText, NULL, &ai ) != 0 )
    {
        m_status = CONN_ERR_ADDRINFO;
        return m_status;
    }

    int s = socket ( ai -> ai_family, SOCK_STREAM, 0 );
    if ( s == -1 )
    {
        freeaddrinfo ( ai );
        m_status = CONN_ERR_SOCKET;
        return m_status;
    }

    if ( connect ( s, ai -> ai_addr, ai -> ai_addrlen ) != 0 )
    {
        close ( s );
        freeaddrinfo ( ai );
        m_status = CONN_ERR_CONN;
        return m_status;
    }
    freeaddrinfo ( ai );
    m_socket = s;
    m_status = CONN_OK;
    StartComunication();

    return m_status;
}
