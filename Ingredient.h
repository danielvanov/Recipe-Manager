#pragma once

#include <string>

class Ingredient {
private:
    std::string name;
    std::string quantity;

public:
    Ingredient();
    Ingredient(const std::string& name, const std::string& quantity);
    std::string getName() const;
    std::string getQuantity() const;
    void setName(const std::string& name);
    void setQuantity(const std::string& quantity);
    void displayInfo() const;
};