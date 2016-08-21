/** Occupy!QUST Client Standalone Local Server */
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
#include<signal.h>

/// Receive XML-Request From Local Browser.
void procQRServer()
{

}

int main()
{
    #ifdef _C4DROID_
    daemon(0,0);
    #endif // _C4DROID_
}
