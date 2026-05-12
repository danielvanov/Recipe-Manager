#pragma once

#include <string>
#include <vector>

class Recipe;
class RecipeManager;

class FileManager {
public:
    static void saveRecipesToFile(const std::vector<Recipe*>& recipes, const std::string& filename);
    static void loadRecipesFromFile(RecipeManager& manager, const std::string& filename);
};