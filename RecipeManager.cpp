#include "RecipeManager.h"
#include "Recipe.h"
#include "DessertRecipe.h"
#include "MainDishRecipe.h"
#include <iostream>
#include <algorithm>

RecipeManager::RecipeManager() : nextId(1) {}

RecipeManager::~RecipeManager() {
    for (auto recipe : recipes) {
        delete recipe;
    }
    recipes.clear();
}

void RecipeManager::addRecipe(Recipe* recipe) {
    recipes.push_back(recipe);
}

void RecipeManager::listAllRecipes() const {
    if (recipes.empty()) {
        std::cout << "No recipes found." << std::endl;
        return;
    }
    for (const auto& recipe : recipes) {
        recipe->displayInfo();
    }
}

Recipe* RecipeManager::findRecipeById(int id) const {
    for (const auto& recipe : recipes) {
        if (recipe->getId() == id) {
            return recipe;
        }
    }
    return nullptr;
}

void RecipeManager::searchByTitle(const std::string& keyword) const {
    bool found = false;
    for (const auto& recipe : recipes) {
        if (recipe->getTitle().find(keyword) != std::string::npos) {
            recipe->displayInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No recipes found with title containing '" << keyword << "'." << std::endl;
    }
}

bool RecipeManager::deleteRecipeById(int id) {
    auto it = std::find_if(recipes.begin(), recipes.end(), [id](Recipe* r) { return r->getId() == id; });
    if (it != recipes.end()) {
        delete *it;
        recipes.erase(it);
        return true;
    }
    return false;
}

void RecipeManager::rateRecipe(int id, int rating) {
    Recipe* recipe = findRecipeById(id);
    if (recipe) {
        recipe->addRating(rating);
        std::cout << "Rating added successfully." << std::endl;
    } else {
        std::cout << "Recipe with ID " << id << " not found." << std::endl;
    }
}

void RecipeManager::filterByDifficulty(const std::string& difficulty) const {
    bool found = false;
    for (const auto& recipe : recipes) {
        if (recipe->getDifficulty() == difficulty) {
            recipe->displayInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No recipes found with difficulty '" << difficulty << "'." << std::endl;
    }
}

int RecipeManager::generateId() {
    return nextId++;
}

void RecipeManager::loadSampleRecipes() {
    // Sample general recipe
    Recipe* pasta = new Recipe(generateId(), "Spaghetti Carbonara", "Classic Italian pasta with creamy sauce", 20, "Easy", "General");
    pasta->addIngredient(Ingredient("Spaghetti", "400g"));
    pasta->addIngredient(Ingredient("Eggs", "3"));
    pasta->addIngredient(Ingredient("Bacon", "200g"));
    pasta->addIngredient(Ingredient("Parmesan Cheese", "100g"));
    addRecipe(pasta);

    // Sample dessert recipe
    DessertRecipe* chocolate = new DessertRecipe(generateId(), "Chocolate Mousse", "Delicious and light chocolate dessert", 15, "Easy", 9);
    chocolate->addIngredient(Ingredient("Dark Chocolate", "200g"));
    chocolate->addIngredient(Ingredient("Eggs", "4"));
    chocolate->addIngredient(Ingredient("Sugar", "50g"));
    chocolate->addIngredient(Ingredient("Cream", "200ml"));
    addRecipe(chocolate);

    // Sample main dish recipe
    MainDishRecipe* steak = new MainDishRecipe(generateId(), "Grilled Steak", "Perfectly grilled beef steak with seasonings", 45, "Medium", false);
    steak->addIngredient(Ingredient("Steak", "500g"));
    steak->addIngredient(Ingredient("Salt", "to taste"));
    steak->addIngredient(Ingredient("Black Pepper", "to taste"));
    steak->addIngredient(Ingredient("Olive Oil", "2 tbsp"));
    steak->addIngredient(Ingredient("Garlic", "2 cloves"));
    addRecipe(steak);

    // Sample vegetarian main dish
    MainDishRecipe* veggies = new MainDishRecipe(generateId(), "Vegetable Stir Fry", "Fresh vegetables with tofu and ginger sauce", 25, "Easy", true);
    veggies->addIngredient(Ingredient("Tofu", "300g"));
    veggies->addIngredient(Ingredient("Broccoli", "200g"));
    veggies->addIngredient(Ingredient("Bell Pepper", "2"));
    veggies->addIngredient(Ingredient("Soy Sauce", "3 tbsp"));
    veggies->addIngredient(Ingredient("Ginger", "1 tbsp"));
    addRecipe(veggies);
}