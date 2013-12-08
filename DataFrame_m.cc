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
    this->flags_1_var = 0;
    this->address_var = 0;
    this->control_var = 0;
    this->information_var = 0;
    this->FCS_var = 0;
    this->flags_2_var = 0;
}

DataFrame::DataFrame(const DataFrame& other) : cMessage(other)
{
    copy(other);
}

DataFrame::~DataFrame()
{
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
    this->flags_1_var = other.flags_1_var;
    this->address_var = other.address_var;
    this->control_var = other.control_var;
    this->information_var = other.information_var;
    this->FCS_var = other.FCS_var;
    this->flags_2_var = other.flags_2_var;
}

void DataFrame::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->flags_1_var);
    doPacking(b,this->address_var);
    doPacking(b,this->control_var);
    doPacking(b,this->information_var);
    doPacking(b,this->FCS_var);
    doPacking(b,this->flags_2_var);
}

void DataFrame::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->flags_1_var);
    doUnpacking(b,this->address_var);
    doUnpacking(b,this->control_var);
    doUnpacking(b,this->information_var);
    doUnpacking(b,this->FCS_var);
    doUnpacking(b,this->flags_2_var);
}

int DataFrame::getFlags_1() const
{
    return flags_1_var;
}

void DataFrame::setFlags_1(int flags_1)
{
    this->flags_1_var = flags_1;
}

int DataFrame::getAddress() const
{
    return address_var;
}

void DataFrame::setAddress(int address)
{
    this->address_var = address;
}

int DataFrame::getControl() const
{
    return control_var;
}

void DataFrame::setControl(int control)
{
    this->control_var = control;
}

int DataFrame::getInformation() const
{
    return information_var;
}

void DataFrame::setInformation(int information)
{
    this->information_var = information;
}

int DataFrame::getFCS() const
{
    return FCS_var;
}

void DataFrame::setFCS(int FCS)
{
    this->FCS_var = FCS;
}

int DataFrame::getFlags_2() const
{
    return flags_2_var;
}

void DataFrame::setFlags_2(int flags_2)
{
    this->flags_2_var = flags_2;
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
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
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
        "flags_1",
        "address",
        "control",
        "information",
        "FCS",
        "flags_2",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int DataFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flags_1")==0) return base+0;
    if (fieldName[0]=='a' && strcmp(fieldName, "address")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "control")==0) return base+2;
    if (fieldName[0]=='i' && strcmp(fieldName, "information")==0) return base+3;
    if (fieldName[0]=='F' && strcmp(fieldName, "FCS")==0) return base+4;
    if (fieldName[0]=='f' && strcmp(fieldName, "flags_2")==0) return base+5;
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
        "int",
        "int",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
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
        case 0: return long2string(pp->getFlags_1());
        case 1: return long2string(pp->getAddress());
        case 2: return long2string(pp->getControl());
        case 3: return long2string(pp->getInformation());
        case 4: return long2string(pp->getFCS());
        case 5: return long2string(pp->getFlags_2());
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
        case 0: pp->setFlags_1(string2long(value)); return true;
        case 1: pp->setAddress(string2long(value)); return true;
        case 2: pp->setControl(string2long(value)); return true;
        case 3: pp->setInformation(string2long(value)); return true;
        case 4: pp->setFCS(string2long(value)); return true;
        case 5: pp->setFlags_2(string2long(value)); return true;
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
        NULL,
        NULL,
    };
    return (field>=0 && field<6) ? fieldStructNames[field] : NULL;
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


