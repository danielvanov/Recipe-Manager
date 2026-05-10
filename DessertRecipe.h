#pragma once

#include "Recipe.h"

class DessertRecipe : public Recipe {
private:
    int sweetnessLevel;

public:
    DessertRecipe();
    DessertRecipe(int id, const std::string& title, const std::string& description, int cookingTime, const std::string& difficulty, int sweetnessLevel);
    int getSweetnessLevel() const;
    void setSweetnessLevel(int sweetnessLevel);
    void displayInfo() const override;
};