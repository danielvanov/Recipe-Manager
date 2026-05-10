#include "BaseEntity.h"

BaseEntity::BaseEntity(int id) : id(id) {}

BaseEntity::~BaseEntity() {}

int BaseEntity::getId() const {
    return id;
}

void BaseEntity::setId(int id) {
    this->id = id;
}