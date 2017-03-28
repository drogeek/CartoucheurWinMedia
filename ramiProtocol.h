#ifndef RAMICARTRIDGE_H
#define RAMICARTRIDGE_H
#include <QDebug>
#include <iostream>
#include <regex>
#include <bitset>

/* this class allows you to communicate using the RAMI brand cartridge.
 * To use, get an instance of the class using the instance() method, then
 * create a Params structure containing your parameters via le getParams() static method,
 * or call it directly with your parameters as arguments
 * You'll get a Data structure as an answer that you should be able to send as is
 * You should be able to print the result using the print(Data) or print(Params) static methods
*/

class RamiProtocol
{

typedef char unsigned uchar;
public:
    typedef struct{
        uchar adressColum;
        uchar lineState;
        uchar ack;
    }Data;

    typedef struct{
        const uchar adress;
        const uchar ack;
        uchar line;
        uchar column;
        bool state;
    }Params;

    static RamiProtocol& instance();
    static void print(const Data&);
    static void print(const Params&);
    static Params getParams();
    static Data encrypt(uchar adress, uchar line, uchar column, bool state, uchar ack);
    static Data encrypt(const Params& params);
    static Params decrypt(const Data&);
    static Params decrypt(const std::string&);

private:

    static RamiProtocol m_instance;
    RamiProtocol();
    ~RamiProtocol();
};

#endif // RAMICARTRIDGE_H
