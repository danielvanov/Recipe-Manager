#include "DessertRecipe.h"
#include <iostream>

DessertRecipe::DessertRecipe() : Recipe(), sweetnessLevel(1) {}

DessertRecipe::DessertRecipe(int id, const std::string& title, const std::string& description, int cookingTime, const std::string& difficulty, int sweetnessLevel)
    : Recipe(id, title, description, cookingTime, difficulty, "Dessert"), sweetnessLevel(sweetnessLevel) {}

int DessertRecipe::getSweetnessLevel() const {
    return sweetnessLevel;
}

void DessertRecipe::setSweetnessLevel(int sweetnessLevel) {
    if (sweetnessLevel >= 1 && sweetnessLevel <= 10) {
        this->sweetnessLevel = sweetnessLevel;
    }
}

void DessertRecipe::displayInfo() const {
    Recipe::displayInfo();
    std::cout << "Sweetness Level: " << sweetnessLevel << "/10" << std::endl;
}