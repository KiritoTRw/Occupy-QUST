/** Occupy!QUST
*   OCQ Project
*   GameHarbor , GH Cloud Platform powered.
*   HC TECH 2016
*/

/** OCQ Client Side
*   1.Receive XML-Request From Local Browser and give requests...
*   2.Game Logic
*/

#ifdef _Client_Or_Server_Only
#error Cannot act as server and client in one program.
#else
#define _Client_Or_Server_Only 1
#endif // _Client_Or_Server_Only


namespace OCQClient
{


int main()
{
    printf("[Main] Main Thread Started.\n");

    /// Start A Daemon
    return 0;
}



}/// End of OCQClient
