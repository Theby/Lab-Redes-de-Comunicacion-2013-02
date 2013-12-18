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

void DataFrame::createFrame(const char* name){

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


