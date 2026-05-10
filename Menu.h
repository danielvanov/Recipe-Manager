#pragma once

#include "RecipeManager.h"

class Menu {
private:
    RecipeManager manager;

public:
    Menu();
    void start();
    void showMainMenu() const;
    void handleAddRecipe();
    void handleListRecipes() const;
    void handleSearchRecipe() const;
    void handleDeleteRecipe();
    void handleRateRecipe();
    void handleFilterByDifficulty() const;
};