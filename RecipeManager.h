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
    const std::vector<Recipe*>& getRecipes() const;
    Recipe* findRecipeById(int id) const;
    void searchByTitle(const std::string& keyword) const;
    bool deleteRecipeById(int id);
    void clearRecipes();
    void rateRecipe(int id, int rating);
    void filterByDifficulty(const std::string& difficulty) const;
    void filterByCategory(const std::string& category) const;
    void sortByCookingTime();
    void sortByRating();
    void showRecipeDetailsById(int id) const;
    void updateNextId(int id);
    int generateId();
    void addSampleRecipes();
};