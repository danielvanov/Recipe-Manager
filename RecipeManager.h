#pragma once

#include "Recipe.h"
#include <vector>

class RecipeManager {
private:
    std::vector<Recipe*> recipes;
    int nextId;

public:
    RecipeManager();
    ~RecipeManager();
    void addRecipe(Recipe* recipe);
    void listAllRecipes() const;
    Recipe* findRecipeById(int id) const;
    void searchByTitle(const std::string& keyword) const;
    bool deleteRecipeById(int id);
    void rateRecipe(int id, int rating);
    void filterByDifficulty(const std::string& difficulty) const;
    int generateId();
    void loadSampleRecipes();
};