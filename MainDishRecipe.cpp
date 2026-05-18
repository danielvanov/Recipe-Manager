#include "MainDishRecipe.h"
#include <iostream>

MainDishRecipe::MainDishRecipe() : Recipe(), vegetarian(false) {}

MainDishRecipe::MainDishRecipe(int id, const std::string& title, const std::string& description, int cookingTime, const std::string& difficulty, bool vegetarian, double calories, bool vegan)
    : Recipe(id, title, description, cookingTime, difficulty, "Main Dish", calories, vegan), vegetarian(vegetarian) {}

bool MainDishRecipe::isVegetarian() const {
    return vegetarian;
}

void MainDishRecipe::setVegetarian(bool vegetarian) {
    this->vegetarian = vegetarian;
}

void MainDishRecipe::displayInfo() const {
    Recipe::displayInfo();
    std::cout << "Vegetarian: " << (vegetarian ? "Yes" : "No") << std::endl;
}