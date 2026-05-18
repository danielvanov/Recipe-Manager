#pragma once

#include "RecipeManager.h"

class Menu {
private:
    RecipeManager manager;
    void initializeRecipes();
    void finalizeRecipes();
    int readInt(const std::string& message) const;
    int readIntInRange(const std::string& message, int min, int max) const;
    double readDouble(const std::string& message) const;
    double readNonNegativeDouble(const std::string& message) const;
    std::string readNonEmptyString(const std::string& message) const;
    std::string readDifficulty() const;
    bool readYesNo(const std::string& message) const;

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
    void handleFilterByCategory() const;
    void handleFilterByMenuType() const;
    void handleSortByCookingTime();
    void handleSortByRating();
    void handleShowDetailsById() const;
    void handleAddSampleRecipes();
    void handleSaveRecipes();
    void handleLoadRecipes();
    void handleShowHealthyRecipes() const;
    void handleShowUnhealthyRecipes() const;
};