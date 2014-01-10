//
// Generated file, do not edit! Created by opp_msgc 4.3 from Informacion.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "Informacion_m.h"
#include "FuncionesExtras.h"

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




Register_Class(Informacion);

Informacion::Informacion(const char *name, int kind) : cMessage(name,kind)
{
    for (unsigned int i=0; i<8; i++)
        this->address_dest_var[i] = 0;
    informacion_arraysize = 0;
    this->informacion_var = 0;
}

Informacion::Informacion(const Informacion& other) : cMessage(other)
{
    informacion_arraysize = 0;
    this->informacion_var = 0;
    copy(other);
}

Informacion::~Informacion()
{
    delete [] informacion_var;
}

Informacion& Informacion::operator=(const Informacion& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void Informacion::copy(const Informacion& other)
{
    for (unsigned int i=0; i<8; i++)
        this->address_dest_var[i] = other.address_dest_var[i];
    delete [] this->informacion_var;
    this->informacion_var = (other.informacion_arraysize==0) ? NULL : new int[other.informacion_arraysize];
    informacion_arraysize = other.informacion_arraysize;
    for (unsigned int i=0; i<informacion_arraysize; i++)
        this->informacion_var[i] = other.informacion_var[i];
}

void Informacion::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->address_dest_var,8);
    b->pack(informacion_arraysize);
    doPacking(b,this->informacion_var,informacion_arraysize);
}

void Informacion::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->address_dest_var,8);
    delete [] this->informacion_var;
    b->unpack(informacion_arraysize);
    if (informacion_arraysize==0) {
        this->informacion_var = 0;
    } else {
        this->informacion_var = new int[informacion_arraysize];
        doUnpacking(b,this->informacion_var,informacion_arraysize);
    }
}

unsigned int Informacion::getAddress_destArraySize() const
{
    return 8;
}

int* Informacion::getAddress(){
    return address_dest_var;
}

int Informacion::getAddress_dest(unsigned int k) const
{
    if (k>=8) throw cRuntimeError("Array of size 8 indexed by %lu", (unsigned long)k);
    return address_dest_var[k];
}

void Informacion::setAddress_dest(unsigned int k, int address_dest)
{
    if (k>=8) throw cRuntimeError("Array of size 8 indexed by %lu", (unsigned long)k);
    this->address_dest_var[k] = address_dest;
}

void Informacion::setInformacionArraySize(unsigned int size)
{
    int *informacion_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = informacion_arraysize < size ? informacion_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        informacion_var2[i] = this->informacion_var[i];
    for (unsigned int i=sz; i<size; i++)
        informacion_var2[i] = 0;
    informacion_arraysize = size;
    delete [] this->informacion_var;
    this->informacion_var = informacion_var2;
}

unsigned int Informacion::getInformacionArraySize() const
{
    return informacion_arraysize;
}

int* Informacion::getInformacion(){
    return informacion_var;
}

int Informacion::getInformacion(unsigned int k) const
{
    if (k>=informacion_arraysize) throw cRuntimeError("Array of size %d indexed by %d", informacion_arraysize, k);
    return informacion_var[k];
}

void Informacion::setInformacion(unsigned int k, int informacion)
{
    if (k>=informacion_arraysize) throw cRuntimeError("Array of size %d indexed by %d", informacion_arraysize, k);
    this->informacion_var[k] = informacion;
}

void Informacion::setDestino(int dest){
    this->destino = dest;
}

int Informacion::getDestino(){
    return destino;
}

void Informacion::createFrame(int destino,int* informacion, int tamInfo){
    //Inicio Address
        //Asigna la direccion al sector address de la trama
        this->setDestino(destino);
    //Fin Address

    //Inicio Informacion
        //Tamaño
        this->setInformacionArraySize(tamInfo);

        //Asignación
        for(int i=0;i<tamInfo;i++){
            this->setInformacion(i,informacion[i]);
        }
    //Fin Informacion
}

class InformacionDescriptor : public cClassDescriptor
{
  public:
    InformacionDescriptor();
    virtual ~InformacionDescriptor();

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

Register_ClassDescriptor(InformacionDescriptor);

InformacionDescriptor::InformacionDescriptor() : cClassDescriptor("Informacion", "cMessage")
{
}

InformacionDescriptor::~InformacionDescriptor()
{
}

bool InformacionDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Informacion *>(obj)!=NULL;
}

const char *InformacionDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int InformacionDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int InformacionDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *InformacionDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "address_dest",
        "informacion",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int InformacionDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='a' && strcmp(fieldName, "address_dest")==0) return base+0;
    if (fieldName[0]=='i' && strcmp(fieldName, "informacion")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *InformacionDescriptor::getFieldTypeString(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *InformacionDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int InformacionDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Informacion *pp = (Informacion *)object; (void)pp;
    switch (field) {
        case 0: return 8;
        case 1: return pp->getInformacionArraySize();
        default: return 0;
    }
}

std::string InformacionDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Informacion *pp = (Informacion *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getAddress_dest(i));
        case 1: return long2string(pp->getInformacion(i));
        default: return "";
    }
}

bool InformacionDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Informacion *pp = (Informacion *)object; (void)pp;
    switch (field) {
        case 0: pp->setAddress_dest(i,string2long(value)); return true;
        case 1: pp->setInformacion(i,string2long(value)); return true;
        default: return false;
    }
}

const char *InformacionDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *InformacionDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Informacion *pp = (Informacion *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


