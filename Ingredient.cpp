#include "Ingredient.h"
#include <iostream>

Ingredient::Ingredient() : name(""), quantity("") {}

Ingredient::Ingredient(const std::string& name, const std::string& quantity) : name(name), quantity(quantity) {}

std::string Ingredient::getName() const {
    return name;
}

std::string Ingredient::getQuantity() const {
    return quantity;
}

void Ingredient::setName(const std::string& name) {
    this->name = name;
}

void Ingredient::setQuantity(const std::string& quantity) {
    this->quantity = quantity;
}

void Ingredient::displayInfo() const {
    std::cout << "Ingredient: " << name << " - " << quantity << std::endl;
}