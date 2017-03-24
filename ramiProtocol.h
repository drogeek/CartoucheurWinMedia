#ifndef RAMICARTRIDGE_H
#define RAMICARTRIDGE_H
#include <iostream>
#include <regex>
#include <bitset>

/* this class allows you to communicate using the RAMI brand cartridge.
 * To use, get an instance of the class using the instance() method, then
 * create a Params structure containing your parameters, or call it directly
 * with your parameters as arguments
 * You'll get a Data structure as an answer that you should be able to send as is
 * You should be able to print the result using the print(Data) static method for
 * test purposes
*/

class RamiProtocol
{

public:
    typedef struct{
        char adressColum;
        char lineState;
        char ack;
    }Data;

    typedef struct{
        char adress;
        char line;
        char column;
        bool state;
        char ack;
    }Params;

    static RamiProtocol& instance();
    static void print(const Data&);
    static void print(const Params&);
    Data encrypt(char adress, char line, char column, bool state, char ack);
    Data encrypt(const Params& params);
    Params decrypt(const Data&);
    Params decrypt(const std::string&);

private:
    static RamiProtocol m_instance;
    RamiProtocol();
    ~RamiProtocol();
};

#endif // RAMICARTRIDGE_H
