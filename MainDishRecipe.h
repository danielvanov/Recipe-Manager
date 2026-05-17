#pragma once

#include "Recipe.h"

class MainDishRecipe : public Recipe {
private:
    bool vegetarian;

public:
    MainDishRecipe();
    MainDishRecipe(int id, const std::string& title, const std::string& description, int cookingTime, const std::string& difficulty, bool vegetarian, double calories = 0.0);
    bool isVegetarian() const;
    void setVegetarian(bool vegetarian);
    void displayInfo() const override;
};