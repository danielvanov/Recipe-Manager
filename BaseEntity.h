#pragma once

#include <string>

class BaseEntity {
protected:
    int id;

public:
    BaseEntity(int id);
    virtual ~BaseEntity();
    int getId() const;
    void setId(int id);
};