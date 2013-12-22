//
// Generated file, do not edit! Created by opp_msgc 4.3 from DataFrame.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "FuncionesExtras.h"
#include "DataFrame_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(DataFrame);

DataFrame::DataFrame(const char *name, int kind) : cMessage(name,kind)
{
    for (unsigned int i=0; i<8; i++)
        this->address_var[i] = 0;
    for (unsigned int i=0; i<8; i++)
        this->control_var[i] = 0;
    information_arraysize = 0;
    this->information_var = 0;
    for (unsigned int i=0; i<16; i++)
        this->FCS_var[i] = 0;
}

DataFrame::DataFrame(const DataFrame& other) : cMessage(other)
{
    information_arraysize = 0;
    this->information_var = 0;
    copy(other);
}

DataFrame::~DataFrame()
{
    delete [] information_var;
}

DataFrame& DataFrame::operator=(const DataFrame& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void DataFrame::copy(const DataFrame& other)
{
    for (unsigned int i=0; i<8; i++)
        this->address_var[i] = other.address_var[i];
    for (unsigned int i=0; i<8; i++)
        this->control_var[i] = other.control_var[i];
    delete [] this->information_var;
    this->information_var = (other.information_arraysize==0) ? NULL : new int[other.information_arraysize];
    information_arraysize = other.information_arraysize;
    for (unsigned int i=0; i<information_arraysize; i++)
        this->information_var[i] = other.information_var[i];
    for (unsigned int i=0; i<16; i++)
        this->FCS_var[i] = other.FCS_var[i];
}

void DataFrame::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->address_var,8);
    doPacking(b,this->control_var,8);
    b->pack(information_arraysize);
    doPacking(b,this->information_var,information_arraysize);
    doPacking(b,this->FCS_var,16);
}

void DataFrame::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->address_var,8);
    doUnpacking(b,this->control_var,8);
    delete [] this->information_var;
    b->unpack(information_arraysize);
    if (information_arraysize==0) {
        this->information_var = 0;
    } else {
        this->information_var = new int[information_arraysize];
        doUnpacking(b,this->information_var,information_arraysize);
    }
    doUnpacking(b,this->FCS_var,16);
}

unsigned int DataFrame::getAddressArraySize() const
{
    return 8;
}

int DataFrame::getAddress(unsigned int k) const
{
    if (k>=8) throw cRuntimeError("Array of size 8 indexed by %lu", (unsigned long)k);
    return address_var[k];
}

void DataFrame::setAddress(unsigned int k, int address)
{
    if (k>=8) throw cRuntimeError("Array of size 8 indexed by %lu", (unsigned long)k);
    this->address_var[k] = address;
}

unsigned int DataFrame::getControlArraySize() const
{
    return 8;
}

int DataFrame::getControl(unsigned int k) const
{
    if (k>=8) throw cRuntimeError("Array of size 8 indexed by %lu", (unsigned long)k);
    return control_var[k];
}

void DataFrame::setControl(unsigned int k, int control)
{
    if (k>=8) throw cRuntimeError("Array of size 8 indexed by %lu", (unsigned long)k);
    this->control_var[k] = control;
}

void DataFrame::setInformationArraySize(unsigned int size)
{
    int *information_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = information_arraysize < size ? information_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        information_var2[i] = this->information_var[i];
    for (unsigned int i=sz; i<size; i++)
        information_var2[i] = 0;
    information_arraysize = size;
    delete [] this->information_var;
    this->information_var = information_var2;
}

unsigned int DataFrame::getInformationArraySize() const
{
    return information_arraysize;
}

int* DataFrame::getInformation(){
    return information_var;
}

int DataFrame::getInformation(unsigned int k) const
{
    if (k>=information_arraysize) throw cRuntimeError("Array of size %d indexed by %d", information_arraysize, k);
    return information_var[k];
}

void DataFrame::setInformation(unsigned int k, int information)
{
    if (k>=information_arraysize) throw cRuntimeError("Array of size %d indexed by %d", information_arraysize, k);
    this->information_var[k] = information;
}

unsigned int DataFrame::getFCSArraySize() const
{
    return 16;
}

int DataFrame::getFCS(unsigned int k) const
{
    if (k>=16) throw cRuntimeError("Array of size 16 indexed by %lu", (unsigned long)k);
    return FCS_var[k];
}

void DataFrame::setFCS(unsigned int k, int FCS)
{
    if (k>=16) throw cRuntimeError("Array of size 16 indexed by %lu", (unsigned long)k);
    this->FCS_var[k] = FCS;
}

void DataFrame::createFrame(int destino, int tamInfo, int* informacion, int NS, int PF, int NR){
    std::string name = FuncionesExtras::getNombreTrama(this->getName());
    std::vector<int> destino_binario;

    //Transforma destino a un número binario
    int * destinoBin = FuncionesExtras::intToBitArray(destino,8);

    //Lo pasa a vector para tener un manejo más directo
    for(int i=0;i<8;i++){
        destino_binario.push_back(destinoBin[i]);
    }

    if(name == "I"){
        // Transforman N(R) y N(S) a binario
        int * nsBin = FuncionesExtras::intToBitArray(NS,3);
        int * nrBin = FuncionesExtras::intToBitArray(NR,3);
        std::vector<int> NS_binario;
        std::vector<int> NR_binario;

        //Se pasan a vector para tener un mejor manejo de estos
        for(int i=0;i<3;i++){
            NS_binario.push_back(nsBin[i]);
            NR_binario.push_back(nrBin[i]);
        }

        this->tramaI(destino_binario, tamInfo, informacion, NS_binario, PF, NR_binario);
    }else if(name == "RR"){       
        // Transforman N(R) a binario
        int *nrBin = FuncionesExtras::intToBitArray(NR,3);
        std::vector<int> NR_binario;

        //Se pasan a vector para tener un mejor manejo de estos
        for(int i=0;i<3;i++){
            NR_binario.push_back(nrBin[i]);
        } 

        this->tramaRR(destino_binario, PF, NR_binario);
    }else if(name == "REJ"){
        PF = 1;

        // Transforman N(R) a binario
        int *nrBin = FuncionesExtras::intToBitArray(NR,3);
        std::vector<int> NR_binario;

        //Se pasan a vector para tener un mejor manejo de estos
        for(int i=0;i<3;i++){
            NR_binario.push_back(nrBin[i]);
        }

        this->tramaREJ(destino_binario, PF, NR_binario);
    }else if(name == "RNR"){
        this->tramaRNR();
    }else if(name == "SREJ"){
        this->tramaSREJ();
    }else if(name == "UI"){
        this->tramaUI();
    }else if(name == "SNRM"){
        this->tramaSNRM();
    }else if(name == "DISC"){
        //solo necesita destino
        this->tramaDISC(destino_binario);
    }else if(name == "RD"){
        this->tramaRD();
    }else if(name == "UP"){
        //solo necesita destino
        this->tramaUP(destino_binario);
    }else if(name == "UA"){
        //solo necesita destino
        this->tramaUA(destino_binario);
    }else if(name == "NR0"){
        this->tramaNR0();
    }else if(name == "NR1"){
        this->tramaNR1();
    }else if(name == "NR2"){
        this->tramaNR2();
    }else if(name == "NR3"){
        this->tramaNR3();
    }else if(name == "SIM"){
        this->tramaSIM();
    }else if(name == "RIM"){
        this->tramaRIM();
    }else if(name == "FRMR"){
        this->tramaFRMR();
    }else if(name == "SARM"){
        this->tramaSARM();
    }else if(name == "DM"){
        this->tramaDM();
    }else if(name == "RSET"){
        this->tramaRSET();
    }else if(name == "SARME"){
        this->tramaSARME();
    }else if(name == "SNRME"){
        this->tramaSNRME();
    }else if(name == "SABM"){
        //solo necesita destino
        this->tramaSABM(destino_binario);
    }else if(name == "XID"){
        this->tramaXID();
    }else if(name == "SABME"){
        this->tramaSABME();
    }
}

void DataFrame::tramaI(std::vector<int> destino, int tamInfo, int* informacion, std::vector<int> NS, int PF, std::vector<int> NR){
    std::string nombre;

    //Inicio Address
        //Copiando Address
        for(unsigned int i=0;i<destino.size();i++){
            this->setAddress(i,destino[i]);
        }
    //Fin Address

    //Inicio Control: Informacion
        //Informacion
        this->setControl(0,0);

        //N(S)
        this->setControl(1,NS[0]);
        this->setControl(2,NS[1]);
        this->setControl(3,NS[2]);

        //bit P/F en 1 para solicitar respuesta
        this->setControl(4,PF);
            //Asignando nombre completo
                nombre = this->getName();
                nombre += ',';
                this->setName(FuncionesExtras::nombrando(nombre.c_str(),PF));

        //N(R)
        this->setControl(5,NR[0]);
        this->setControl(6,NR[1]);
        this->setControl(7,NR[2]);
    //Fin Control

    //Inicio Informacion
        //Copiando Información
        this->setInformationArraySize(tamInfo);
        for(unsigned int i=0;i<this->getInformationArraySize();i++){
            this->setInformation(i,informacion[i]);
        }
    //Fin Informacion

    //Inicio FCS
        for(int i=0;i<16;i++){
            this->setFCS(i,0);
        }
    //Fin FCS
}


void DataFrame::tramaRR(std::vector<int> destino, int PF, std::vector<int> NR){
    std::string nombre;

    //Inicio Address
        //Asigna la direccion al sector address de la trama
        for(unsigned int i=0;i<destino.size();i++){
            this->setAddress(i,destino[i]);
        }
    //Fin Address

    //Inicio Control: Supervisory RR
        //Supervisory
        this->setControl(0,1);
        this->setControl(1,0);

        //Supervisory Funcion bits
        this->setControl(2,0);
        this->setControl(3,0);

        //bit P/F en 1 para dar respuesta
        this->setControl(4,PF);
        //Asignando nombre completo
            nombre = this->getName();
            nombre += ',';
            this->setName(FuncionesExtras::nombrando(nombre.c_str(),this->getControl(4)));

        //N(R)
        this->setControl(5,NR[0]);
        this->setControl(6,NR[1]);
        this->setControl(7,NR[2]);
    //Fin Control

    //Inicio FCS
        for(int i=0;i<16;i++){
            this->setFCS(i,0);
        }
    //Fin FCS
}

void DataFrame::tramaREJ(std::vector<int> destino, int PF, std::vector<int> NR){
    std::string name;

    //Inicio Address
        //Asigna la direccion al sector address de la trama
        for(unsigned int i=0;i<destino.size();i++){
            this->setAddress(i,destino[i]);
        }
    //Fin Address

    //Inicio Control: Supervisory RR
        //Supervisory
        this->setControl(0,1);
        this->setControl(1,0);

        //Supervisory Funcion bits
        this->setControl(2,0);
        this->setControl(3,1);

        //bit P/F en 1 para dar respuesta
        this->setControl(4,1);
        //Asignando nombre completo
            name = this->getName();
            name += ',';
            this->setName(FuncionesExtras::nombrando(name.c_str(),this->getControl(4)));

        //N(R)
        this->setControl(5,NR[0]);
        this->setControl(6,NR[1]);
        this->setControl(7,NR[2]);
    //Fin Control

    //Inicio FCS
        for(int i=0;i<16;i++){
            this->setFCS(i,0);
        }
    //Fin FCS
}

void DataFrame::tramaRNR(){
    //No Implementado
}

void DataFrame::tramaSREJ(){
    //No Implementado
}

void DataFrame::tramaUI(){
    //No Implementado
}

void DataFrame::tramaSNRM(){
    //No Implementado
}

void DataFrame::tramaDISC(std::vector<int> destino){
    //Inicio Address
        //Asigna la direccion al sector address de la trama
        for(unsigned int i=0;i<destino.size();i++){
            this->setAddress(i,destino[i]);
        }
    //Fin Address

    //Inicio Control: Unnumbered DISC
        //Unnumbered
        this->setControl(0,1);
        this->setControl(1,1);

        //Unnumbered Funcion bits
        this->setControl(2,0);
        this->setControl(3,0);

        //bit P/F en 1 para solicitar respuesta
        this->setControl(4,1);

        //Unnumbered Funcion bits
        this->setControl(5,0);
        this->setControl(6,1);
        this->setControl(7,0);
    //Fin Control

    //Inicio FCS
        for(int i=0;i<16;i++){
            this->setFCS(i,0);
        }
    //Fin FCS
}

void DataFrame::tramaRD(){
    //No Implementado
}

void DataFrame::tramaUP(std::vector<int> destino){
    //Inicio Address
        //Asigna la direccion al sector address de la trama
        for(unsigned int i=0;i<destino.size();i++){
            this->setAddress(i,destino[i]);
        }
    //Fin Address

    //Inicio Control: Unnumbered UP
        //Unnumbered
        this->setControl(0,1);
        this->setControl(1,1);

        //Unnumbered Funcion bits
        this->setControl(2,0);
        this->setControl(3,0);

        //bit P/F en 1 para dar respuesta
        this->setControl(4,1);

        //Unnumbered Funcion bits
        this->setControl(5,1);
        this->setControl(6,0);
        this->setControl(7,0);
    //Fin Control

    //Inicio FCS
        for(int i=0;i<16;i++){
            this->setFCS(i,0);
        }
    //Fin FCS
}

void DataFrame::tramaUA(std::vector<int> destino){
    //Inicio Address
        //Asigna la direccion al sector address de la trama
        for(unsigned int i=0;i<destino.size();i++){
            this->setAddress(i,destino[i]);
        }
    //Fin Address

    //Inicio Control: Unnumbered UA
        //Unnumbered
        this->setControl(0,1);
        this->setControl(1,1);

        //Unnumbered Funcion bits
        this->setControl(2,0);
        this->setControl(3,0);

        //bit P/F en 1 para dar respuesta
        this->setControl(4,1);

        //Unnumbered Funcion bits
        this->setControl(5,1);
        this->setControl(6,1);
        this->setControl(7,0);
    //Fin Control

    //Inicio FCS
        for(int i=0;i<16;i++){
            this->setFCS(i,0);
        }
    //Fin FCS
}

void DataFrame::tramaNR0(){
    //No Implementado
}

void DataFrame::tramaNR1(){
    //No Implementado
}

void DataFrame::tramaNR2(){
    //No Implementado
}

void DataFrame::tramaNR3(){
    //No Implementado
}

void DataFrame::tramaSIM(){
    //No Implementado
}

void DataFrame::tramaRIM(){
    //No Implementado
}

void DataFrame::tramaFRMR(){
    //No Implementado
}

void DataFrame::tramaSARM(){
    //No Implementado
}

void DataFrame::tramaDM(){
    //No Implementado
}

void DataFrame::tramaRSET(){
    //No Implementado
}

void DataFrame::tramaSARME(){
    //No Implementado
}

void DataFrame::tramaSNRME(){
    //No Implementado
}

void DataFrame::tramaSABM(std::vector<int> destino){
    //Inicio Address
        //Asigna la direccion al sector address de la trama
        for(unsigned int i=0;i<destino.size();i++){
            this->setAddress(i,destino[i]);
        }
    //Fin Address

    //Inicio Control: Unnumbered SABM
        //Unnumbered
        this->setControl(0,1);
        this->setControl(1,1);

        //Unnumbered Funcion bits
        this->setControl(2,1);
        this->setControl(3,1);

        //bit P/F en 1 para solicitar respuesta
        this->setControl(4,1);

        //Unnumbered Funcion bits
        this->setControl(5,1);
        this->setControl(6,0);
        this->setControl(7,0);
    //Fin Control

    //Inicio FCS
        for(int i=0;i<16;i++){
            this->setFCS(i,0);
        }
    //Fin FCS
}

void DataFrame::tramaXID(){
    //No Implementado
}

void DataFrame::tramaSABME(){
    //No Implementado
}

class DataFrameDescriptor : public cClassDescriptor
{
  public:
    DataFrameDescriptor();
    virtual ~DataFrameDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(DataFrameDescriptor);

DataFrameDescriptor::DataFrameDescriptor() : cClassDescriptor("DataFrame", "cMessage")
{
}

DataFrameDescriptor::~DataFrameDescriptor()
{
}

bool DataFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DataFrame *>(obj)!=NULL;
}

const char *DataFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DataFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int DataFrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *DataFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "address",
        "control",
        "information",
        "FCS",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int DataFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='a' && strcmp(fieldName, "address")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "control")==0) return base+1;
    if (fieldName[0]=='i' && strcmp(fieldName, "information")==0) return base+2;
    if (fieldName[0]=='F' && strcmp(fieldName, "FCS")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DataFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *DataFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int DataFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DataFrame *pp = (DataFrame *)object; (void)pp;
    switch (field) {
        case 0: return 8;
        case 1: return 8;
        case 2: return pp->getInformationArraySize();
        case 3: return 16;
        default: return 0;
    }
}

std::string DataFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DataFrame *pp = (DataFrame *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getAddress(i));
        case 1: return long2string(pp->getControl(i));
        case 2: return long2string(pp->getInformation(i));
        case 3: return long2string(pp->getFCS(i));
        default: return "";
    }
}

bool DataFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DataFrame *pp = (DataFrame *)object; (void)pp;
    switch (field) {
        case 0: pp->setAddress(i,string2long(value)); return true;
        case 1: pp->setControl(i,string2long(value)); return true;
        case 2: pp->setInformation(i,string2long(value)); return true;
        case 3: pp->setFCS(i,string2long(value)); return true;
        default: return false;
    }
}

const char *DataFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *DataFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DataFrame *pp = (DataFrame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


