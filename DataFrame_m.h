//
// Generated file, do not edit! Created by opp_msgc 4.3 from DataFrame.msg.
//

#ifndef _DATAFRAME_M_H_
#define _DATAFRAME_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0403
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif



/**
 * Class generated from <tt>DataFrame.msg</tt> by opp_msgc.
 * <pre>
 * message DataFrame {
 *     int address[8];
 * 	int control[8];
 *     int information[];    
 *     int FCS[16];
 * }
 * </pre>
 */
class DataFrame : public ::cMessage
{
  protected:
    int address_var[8];
    int control_var[8];
    int *information_var; // array ptr
    unsigned int information_arraysize;
    int FCS_var[16];

  private:
    void copy(const DataFrame& other);

        void tramaI(int* informacion, int tamInfo);

        void tramaRR();
        void tramaREJ();
        void tramaRNR();
        void tramaSREJ();

        void tramaUI();
        void tramaSNRM();
    void tramaDISC(std::vector<int> destino);   
        void tramaRD();
        void tramaUP();
        void tramaUA();
        void tramaNR0();
        void tramaNR1();
        void tramaNR2();
        void tramaNR3();
        void tramaSIM();
        void tramaRIM();
        void tramaFRMR();
        void tramaSARM();
        void tramaDM();
        void tramaRSET();
        void tramaSARME();
        void tramaSNRME();
    void tramaSABM(std::vector<int> destino);
        void tramaXID();
        void tramaSABME();

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const DataFrame&);

  public:
    DataFrame(const char *name=NULL, int kind=0);
    DataFrame(const DataFrame& other);
    virtual ~DataFrame();
    DataFrame& operator=(const DataFrame& other);
    virtual DataFrame *dup() const {return new DataFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual unsigned int getAddressArraySize() const;
    virtual int getAddress(unsigned int k) const;
    virtual void setAddress(unsigned int k, int address);
    virtual unsigned int getControlArraySize() const;
    virtual int getControl(unsigned int k) const;
    virtual void setControl(unsigned int k, int control);
    virtual void setInformationArraySize(unsigned int size);
    virtual unsigned int getInformationArraySize() const;
    virtual int getInformation(unsigned int k) const;
    virtual void setInformation(unsigned int k, int information);
    virtual unsigned int getFCSArraySize() const;
    virtual int getFCS(unsigned int k) const;
    virtual void setFCS(unsigned int k, int FCS);

    /*
     * Crea la trama seg�n su nombre, asignando el campo de control seg�n corresponda
     */
    void createFrame(int destino, int tamInfo=0, int* informacion=NULL, int NS=0, int PF=0, int NR=0);
};

inline void doPacking(cCommBuffer *b, DataFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, DataFrame& obj) {obj.parsimUnpack(b);}


#endif // _DATAFRAME_M_H_
