/** Occupy!QUST
*   OCQ Project
*   GameHarbor , GH Cloud Platform powered.
*   HC TECH 2016
*/

/** OCQ Server Side
*   Receive requests and reply clients.
*/

#ifdef _Client_Or_Server_Only
#error Cannot act as server and client in one program.
#else
#define _Client_Or_Server_Only 2
#endif // _Client_Or_Server_Only

#include <cpplib/cpplib#gsock>
#include <thread>
using namespace std;



#define SERVER_QRCODE_PORT 53401

namespace OCQServer
{

char qrbuff[1024];
char cachedWebPage_QRCode[4096];
bool procQRCodeReply_flag_have_to_shutdown;
bool procQRCodeReply_flag_isRunning;
/// QR-Code Scan & Reply Thread Main Function
void procQRCodeReply()
{
    printf("[QR Code Service] Service Started.\n");
    printf("[QR Code Service] Configuring Flags...\n");
    procQRCodeReply_flag_have_to_shutdown=false;
    procQRCodeReply_flag_isRunning=true;
    printf("[QR Code Service] Caching Web Page...\n");
    FILE* fp=fopen("D:\\index.html","r");
    if(fp==NULL)
    {
        printf("[QR Code Service] Web Page Not Found.\n");
        procQRCodeReply_flag_isRunning=false;
        return;
    }
    memset(cachedWebPage_QRCode,0,4096);
    strcpy(cachedWebPage_QRCode,"HTTP/1.0 200 OK\r\nContent-Type:text/html\r\n\r\n");
    while(fgets(qrbuff,1024,fp)!=nullptr)
    {
        strcat(cachedWebPage_QRCode,qrbuff);
        memset(qrbuff,0,1024);
    }
    strcat(cachedWebPage_QRCode,"\r\n\r\n");
    fclose(fp);
    int cachedWebPage_QRCode_sz=strlen(cachedWebPage_QRCode);
    printf("[QR Code Service] Web Page Cache Done.\n");
    serversock t;
    int ret=t.bind(SERVER_QRCODE_PORT);
    if(ret<0)
    {
        printf("[QR Code Service] Bind Failed.\n");
        procQRCodeReply_flag_isRunning=false;
        return;
    }
    ret=t.listen(10);
    if(ret<0)
    {
        printf("[QR Code Service] Listen Failed.\n");
        procQRCodeReply_flag_isRunning=false;
        return;
    }
    printf("[QR Code Service] Waiting For accept...\n");
    while(1)
    {
        sock s=t.accept();
        printf("[QR Code Service] Accepted 1 Request.\n");
        ret=s.recv(qrbuff,1024);
        if(ret<=0)
        {
            printf("[QR Code Service] Receive Failed.\n");
        }
        int done=0;
        while(done<cachedWebPage_QRCode_sz)
        {
            ret=s.send(cachedWebPage_QRCode+done,cachedWebPage_QRCode_sz-done);
            if(ret<=0) break;
            done+=ret;
        }
        if(ret<=0)
        {
            printf("[QR Code Service] Server -> Client (WebPage) Send Failed. Connection Shutdown.\n");
        }
        else
        {
            printf("[QR Code Service] Web Page Sent. Now Disconnect.\n");
        }
        /// GSOCK will shutdown this connection automatically.

        printf("[QR Code Service] Checking Shutdown Flags...\n");
        if(procQRCodeReply_flag_have_to_shutdown)
        {
            printf("[QR Code Service] Notified To Shutdown. Now Quit..\n");
            procQRCodeReply_flag_isRunning=false;
            return;
        }
    }
}

/// Game Logic Thread
void procGameLogic()
{

}

/// Server Watcher : Terminal Control
char cmdbuff[1024];
void procServerWatcher()
{
    printf("[Watcher] Server Watcher Service Started.\n");
    int cmd;
    while(scanf("%d%*c",&cmd)==1)
    {
        printf("[Watcher] Command Read-in.\n");
        switch(cmd)
        {
        case 1:/// Stop QR Service
            {
                if(procQRCodeReply_flag_isRunning==false)
                {
                    printf("[Watcher] QR Service is not running.\n");
                    break;
                }

                /// Set Shutdown flag
                printf("[Watcher] Setting Shutdown Flag...\n");
                procQRCodeReply_flag_have_to_shutdown=true;
                printf("[Watcher] Waiting For Shutdown...\n");
                this_thread::sleep_for(chrono::seconds(5));
                printf("[Watcher] Checking Running Flag...\n");
                if(procQRCodeReply_flag_isRunning==false)
                {
                    printf("[Watcher] QR Service Successfully Shutdown.\n");
                    break;
                }
                printf("[Watcher] QR Service Not Shutdown. Try another way...\n");
                {
                    sock s;
                    int ret=s.connect("127.0.0.1",SERVER_QRCODE_PORT);
                    if(ret<0)
                    {
                        printf("[Watcher] QR Service Server-Side Port is not connectible.\n");
                        printf("[Watcher] Failed to stop QR Service \n");
                        break;
                    }
                    s.send("Please ShutDown, Will you?",strlen("Please ShutDown, Will you?"));
                    s.recv(cmdbuff,1024);
                    printf("[Watcher] Now waiting For QR Service...\n");
                    this_thread::sleep_for(chrono::seconds(3));
                }
                if(procQRCodeReply_flag_isRunning==false)
                {
                    printf("[Watcher] QR Service Stopped Successfully.\n");
                }
                else
                {
                    printf("[Watcher] Failed to stop QR Service.\n");
                }
            }
            break;
        default:
            printf("[Watcher] Command not understood.\n");
            break;
        }
    }
    printf("[Watcher] Watcher Service Shutdown.\n");
}

int main()
{
    printf("[Main] Main Thread Started.\n");
    /// Start QR-Code Reply Thread
    thread tda(procQRCodeReply);
    tda.detach();
    /// Start Game Logic Thread
    thread tdb(procGameLogic);
    tdb.detach();
    /// Start Server Watcher Thread
    thread tdc(procServerWatcher);

    /// Join Watcher Thread
    tdc.join();

    /// Shutdown
    return 0;
}


}/// End of OCQ Server
