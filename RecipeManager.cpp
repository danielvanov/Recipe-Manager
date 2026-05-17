#include "RecipeManager.h"
#include "Recipe.h"
#include "DessertRecipe.h"
#include "MainDishRecipe.h"
#include <iostream>
#include <algorithm>
#include <cctype>

static std::string toLower(const std::string& text) {
    std::string result = text;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return result;
}

RecipeManager::RecipeManager() : nextId(1) {}

RecipeManager::~RecipeManager() {
    clearRecipes();
}

void RecipeManager::addRecipe(Recipe* recipe) {
    if (!recipe) {
        std::cerr << "ERROR: Attempted to add null recipe pointer." << std::endl;
        return;
    }

    std::unique_ptr<Recipe> owned(recipe);
    if (recipe->getId() <= 0) {
        std::cerr << "ERROR: Recipe ID must be a positive integer. Recipe not added." << std::endl;
        return;
    }

    for (const auto& existing : recipes) {
        if (existing && existing->getId() == recipe->getId()) {
            std::cerr << "ERROR: Duplicate recipe ID " << recipe->getId() << ". Recipe not added." << std::endl;
            return;
        }
    }

    updateNextId(recipe->getId());
    recipes.push_back(std::move(owned));
    std::cout << "Recipe '" << recipe->getTitle() << "' added successfully with ID: " << recipe->getId() << "." << std::endl;
}

void RecipeManager::replaceRecipes(std::vector<std::unique_ptr<Recipe>>&& newRecipes) {
    clearRecipes();
    recipes = std::move(newRecipes);
    nextId = 1;
    for (const auto& recipe : recipes) {
        if (recipe) {
            updateNextId(recipe->getId());
        }
    }
}

const std::vector<std::unique_ptr<Recipe>>& RecipeManager::getRecipes() const {
    return recipes;
}

void RecipeManager::listAllRecipes() const {
    if (recipes.empty()) {
        std::cout << "No recipes found." << std::endl;
        return;
    }
    std::cout << "Listing " << recipes.size() << " recipe(s):" << std::endl;
    for (const auto& recipe : recipes) {
        recipe->displayInfo();
    }
}

Recipe* RecipeManager::findRecipeById(int id) const {
    for (const auto& recipe : recipes) {
        if (recipe && recipe->getId() == id) {
            return recipe.get();
        }
    }
    return nullptr;
}

void RecipeManager::searchByTitle(const std::string& keyword) const {
    if (recipes.empty()) {
        std::cout << "INFO: No recipes in database. Cannot search." << std::endl;
        return;
    }
    
    bool found = false;
    std::string searchText = toLower(keyword);
    for (const auto& recipe : recipes) {
        if (toLower(recipe->getTitle()).find(searchText) != std::string::npos) {
            recipe->displayInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "INFO: No recipes found with title containing '" << keyword << "'. Try a different search term." << std::endl;
    }
}

bool RecipeManager::deleteRecipeById(int id) {
    if (id <= 0) {
        std::cerr << "ERROR: Invalid recipe ID: " << id << ". ID must be positive." << std::endl;
        return false;
    }

    auto it = std::find_if(recipes.begin(), recipes.end(), [id](const std::unique_ptr<Recipe>& r) { return r && r->getId() == id; });
    if (it != recipes.end()) {
        std::string title = (*it)->getTitle();
        recipes.erase(it);
        std::cout << "SUCCESS: Recipe '" << title << "' (ID " << id << ") deleted successfully." << std::endl;
        return true;
    }
    std::cout << "ERROR: Recipe with ID " << id << " not found." << std::endl;
    return false;
}

void RecipeManager::clearRecipes() {
    recipes.clear();
    nextId = 1;
}

void RecipeManager::rateRecipe(int id, int rating) {
    if (id <= 0) {
        std::cerr << "ERROR: Invalid recipe ID: " << id << ". ID must be positive." << std::endl;
        return;
    }
    
    if (rating < 1 || rating > 5) {
        std::cerr << "ERROR: Invalid rating: " << rating << ". Rating must be between 1 and 5." << std::endl;
        return;
    }

    Recipe* recipe = findRecipeById(id);
    if (recipe) {
        recipe->addRating(rating);
        std::cout << "SUCCESS: Rating " << rating << " added to '" << recipe->getTitle() << "'. Average rating: " << recipe->getRating() << "." << std::endl;
    } else {
        std::cout << "ERROR: Recipe with ID " << id << " not found. Cannot rate." << std::endl;
    }
}

void RecipeManager::filterByDifficulty(const std::string& difficulty) const {
    if (recipes.empty()) {
        std::cout << "INFO: No recipes in database. Cannot filter." << std::endl;
        return;
    }

    bool found = false;
    std::string filter = toLower(difficulty);
    for (const auto& recipe : recipes) {
        if (recipe && toLower(recipe->getDifficulty()) == filter) {
            recipe->displayInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "INFO: No recipes found with difficulty '" << difficulty << "'." << std::endl;
    }
}

void RecipeManager::filterByCategory(const std::string& category) const {
    if (recipes.empty()) {
        std::cout << "INFO: No recipes in database. Cannot filter." << std::endl;
        return;
    }

    bool found = false;
    std::string filter = toLower(category);
    bool vegetarianQuery = filter.find("veg") != std::string::npos;

    for (const auto& recipe : recipes) {
        if (!recipe) {
            continue;
        }
        std::string recipeCategory = toLower(recipe->getCategory());
        if (recipeCategory.find(filter) != std::string::npos) {
            recipe->displayInfo();
            found = true;
            continue;
        }

        if (vegetarianQuery) {
            const MainDishRecipe* mainDish = dynamic_cast<const MainDishRecipe*>(recipe.get());
            if (mainDish && mainDish->isVegetarian()) {
                recipe->displayInfo();
                found = true;
            }
        }
    }

    if (!found) {
        std::cout << "INFO: No recipes found with category '" << category << "'." << std::endl;
    }
}

void RecipeManager::sortByCookingTime() {
    if (recipes.empty()) {
        std::cout << "INFO: No recipes to sort." << std::endl;
        return;
    }

    if (recipes.size() == 1) {
        std::cout << "INFO: Only one recipe. Sorting not necessary." << std::endl;
        return;
    }

    std::sort(recipes.begin(), recipes.end(), [](const std::unique_ptr<Recipe>& a, const std::unique_ptr<Recipe>& b) {
        if (!a || !b) return false;
        return a->getCookingTime() < b->getCookingTime();
    });
    std::cout << "SUCCESS: Sorted " << recipes.size() << " recipe(s) by cooking time." << std::endl;
}

void RecipeManager::sortByRating() {
    if (recipes.empty()) {
        std::cout << "INFO: No recipes to sort." << std::endl;
        return;
    }

    if (recipes.size() == 1) {
        std::cout << "INFO: Only one recipe. Sorting not necessary." << std::endl;
        return;
    }

    std::sort(recipes.begin(), recipes.end(), [](const std::unique_ptr<Recipe>& a, const std::unique_ptr<Recipe>& b) {
        if (!a || !b) return false;
        return a->getRating() > b->getRating();
    });
    std::cout << "SUCCESS: Sorted " << recipes.size() << " recipe(s) by rating." << std::endl;
}

void RecipeManager::showRecipeDetailsById(int id) const {
    if (id <= 0) {
        std::cerr << "ERROR: Invalid recipe ID: " << id << ". ID must be positive." << std::endl;
        return;
    }

    Recipe* recipe = findRecipeById(id);
    if (recipe) {
        recipe->displayInfo();
    } else {
        std::cout << "ERROR: Recipe with ID " << id << " not found." << std::endl;
    }
}

void RecipeManager::updateNextId(int id) {
    if (id >= nextId) {
        nextId = id + 1;
    }
}

int RecipeManager::generateId() {
    return nextId++;
}

void RecipeManager::addSampleRecipes() {
    // Sample general recipe
    Recipe* omelette = new Recipe(generateId(), "Omelette", "Quick egg omelette with herbs", 10, "Easy", "Breakfast", 250);
    if (!omelette) {
        std::cerr << "ERROR: Memory allocation failed for sample recipe." << std::endl;
        return;
    }
    omelette->addIngredient(Ingredient("Eggs", "3"));
    omelette->addIngredient(Ingredient("Salt", "1 pinch"));
    omelette->addIngredient(Ingredient("Butter", "1 tbsp"));
    addRecipe(omelette);

    // Sample dessert recipe
    DessertRecipe* cake = new DessertRecipe(generateId(), "Chocolate Cake", "Simple cake with chocolate glaze", 60, "Medium", 8, 450);
    if (!cake) {
        std::cerr << "ERROR: Memory allocation failed for sample recipe." << std::endl;
        return;
    }
    cake->addIngredient(Ingredient("Flour", "200g"));
    cake->addIngredient(Ingredient("Cocoa Powder", "50g"));
    cake->addIngredient(Ingredient("Sugar", "150g"));
    cake->addIngredient(Ingredient("Eggs", "3"));
    addRecipe(cake);

    // Sample main dish recipe
    MainDishRecipe* pasta = new MainDishRecipe(generateId(), "Vegetable Pasta", "Pasta with fresh vegetables", 30, "Easy", true, 350);
    if (!pasta) {
        std::cerr << "ERROR: Memory allocation failed for sample recipe." << std::endl;
        return;
    }
    pasta->addIngredient(Ingredient("Pasta", "300g"));
    pasta->addIngredient(Ingredient("Tomato Sauce", "200ml"));
    pasta->addIngredient(Ingredient("Zucchini", "1"));
    pasta->addIngredient(Ingredient("Olive Oil", "2 tbsp"));
    addRecipe(pasta);

    // Sample unhealthy main dish recipe
    MainDishRecipe* steak = new MainDishRecipe(generateId(), "Steak", "Grilled steak with garlic butter", 25, "Medium", false, 650);
    if (!steak) {
        std::cerr << "ERROR: Memory allocation failed for sample recipe." << std::endl;
        return;
    }
    steak->addIngredient(Ingredient("Beef Steak", "250g"));
    steak->addIngredient(Ingredient("Garlic", "2 cloves"));
    steak->addIngredient(Ingredient("Butter", "1 tbsp"));
    addRecipe(steak);

    std::cout << "SUCCESS: Added 4 sample recipes." << std::endl;
}
