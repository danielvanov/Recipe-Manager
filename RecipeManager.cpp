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

void RecipeManager::addRecipe(std::unique_ptr<Recipe> recipe) {
    if (!recipe) {
        std::cerr << "ERROR: Attempted to add null recipe pointer." << std::endl;
        return;
    }

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
    std::cout << "Recipe '" << recipe->getTitle() << "' added successfully with ID: " << recipe->getId() << "." << std::endl;
    recipes.push_back(std::move(recipe));
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

void RecipeManager::filterByMenuType(const std::string& menuType) const {
    if (recipes.empty()) {
        std::cout << "INFO: No recipes in database. Cannot filter." << std::endl;
        return;
    }

    std::string filter = toLower(menuType);
    bool healthy = filter == "healthy";
    bool unhealthy = filter == "unhealthy";
    bool vegan = filter == "vegan";

    const Recipe* starter = nullptr;
    const Recipe* entree = nullptr;
    const Recipe* dessert = nullptr;

    auto containsLower = [&](const std::string& text, const std::string& keyword) {
        return toLower(text).find(keyword) != std::string::npos;
    };

    auto isStarterRecipe = [&](const Recipe* recipe) {
        if (!recipe) {
            return false;
        }
        if (dynamic_cast<const DessertRecipe*>(recipe) || dynamic_cast<const MainDishRecipe*>(recipe)) {
            return false;
        }
        std::string category = recipe->getCategory();
        std::string title = recipe->getTitle();
        std::string description = recipe->getDescription();

        return containsLower(category, "starter") || containsLower(category, "appetizer") || containsLower(category, "salad") || containsLower(category, "soup") || containsLower(category, "snack") ||
               containsLower(title, "starter") || containsLower(title, "appetizer") || containsLower(title, "salad") || containsLower(title, "soup") ||
               containsLower(description, "starter");
    };

    auto isDessertRecipe = [&](const Recipe* recipe) {
        if (!recipe) {
            return false;
        }
        if (dynamic_cast<const DessertRecipe*>(recipe)) {
            return true;
        }
        std::string category = recipe->getCategory();
        std::string title = recipe->getTitle();
        std::string description = recipe->getDescription();

        return containsLower(category, "dessert") || containsLower(category, "sweet") || containsLower(category, "treat") ||
               containsLower(title, "dessert") || containsLower(title, "sweet") || containsLower(title, "treat") ||
               containsLower(description, "dessert");
    };

    auto isMainRecipe = [&](const Recipe* recipe) {
        if (!recipe) {
            return false;
        }
        if (dynamic_cast<const MainDishRecipe*>(recipe)) {
            return true;
        }
        std::string category = recipe->getCategory();
        std::string title = recipe->getTitle();
        std::string description = recipe->getDescription();

        return containsLower(category, "main") || containsLower(category, "entree") || containsLower(title, "main") ||
               containsLower(title, "entree") || containsLower(description, "main") || containsLower(description, "entree");
    };

    auto matchesDiet = [&](const Recipe* recipe) {
        if (!recipe) {
            return false;
        }
        if (healthy) {
            return recipe->getCalories() < 500.0;
        }
        if (unhealthy) {
            return recipe->getCalories() >= 500.0;
        }
        if (vegan) {
            if (recipe->isVegan()) {
                return true;
            }
            std::string title = recipe->getTitle();
            std::string description = recipe->getDescription();
            std::string category = recipe->getCategory();
            if (containsLower(title, "vegan") || containsLower(description, "vegan") || containsLower(category, "vegan")) {
                return true;
            }
            return false;
        }
        return true;
    };

    for (const auto& recipe : recipes) {
        if (!recipe) {
            continue;
        }

        if (!dessert && isDessertRecipe(recipe.get()) && matchesDiet(recipe.get())) {
            dessert = recipe.get();
            continue;
        }

        if (!entree && isMainRecipe(recipe.get()) && matchesDiet(recipe.get())) {
            entree = recipe.get();
            continue;
        }

        if (!starter && isStarterRecipe(recipe.get()) && matchesDiet(recipe.get())) {
            starter = recipe.get();
            continue;
        }
    }

    std::cout << "Dietary Menu: " << (healthy ? "Healthy" : unhealthy ? "Unhealthy" : vegan ? "Vegan" : menuType) << std::endl;

    if (starter) {
        std::cout << "\nStarter:" << std::endl;
        starter->displayInfo();
    } else {
        std::cout << "\nStarter: No suitable starter recipe found." << std::endl;
    }

    if (entree) {
        std::cout << "\nMain Dish:" << std::endl;
        entree->displayInfo();
    } else {
        std::cout << "\nMain Dish: No suitable main dish recipe found." << std::endl;
    }

    if (dessert) {
        std::cout << "\nDessert:" << std::endl;
        dessert->displayInfo();
    } else {
        std::cout << "\nDessert: No suitable dessert recipe found." << std::endl;
    }
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
    auto alreadyExists = [&](const std::string& title) {
        std::string lowerTitle = toLower(title);
        return std::any_of(recipes.begin(), recipes.end(), [&](const std::unique_ptr<Recipe>& r) {
            return r && toLower(r->getTitle()) == lowerTitle;
        });
    };

    struct SampleRecipe {
        std::unique_ptr<Recipe> recipe;
        std::vector<Ingredient> ingredients;
        bool vegan = false;
    };

    std::vector<SampleRecipe> samples;
    samples.push_back({
        std::make_unique<Recipe>(generateId(), "Vegan Garden Salad", "Fresh vegan salad with leafy greens and herbs", 10, "Easy", "Starter", 180.0),
        {Ingredient("Lettuce", "100g"), Ingredient("Cucumber", "100g"), Ingredient("Tomatoes", "100g"), Ingredient("Olive Oil", "1 tbsp")},
        true
    });
    samples.push_back({
        std::make_unique<DessertRecipe>(generateId(), "Chocolate Cake", "Simple cake with chocolate glaze", 60, "Medium", 8, 450.0),
        {Ingredient("Flour", "200g"), Ingredient("Cocoa Powder", "50g"), Ingredient("Sugar", "150g"), Ingredient("Eggs", "3")},
        false
    });
    samples.push_back({
        std::make_unique<MainDishRecipe>(generateId(), "Vegetable Pasta", "Pasta with fresh vegetables", 30, "Easy", true, 350.0),
        {Ingredient("Pasta", "300g"), Ingredient("Tomato Sauce", "200ml"), Ingredient("Zucchini", "1"), Ingredient("Olive Oil", "2 tbsp")},
        false
    });
    samples.push_back({
        std::make_unique<MainDishRecipe>(generateId(), "Steak", "Grilled steak with garlic butter", 25, "Medium", false, 650.0),
        {Ingredient("Beef Steak", "250g"), Ingredient("Garlic", "2 cloves"), Ingredient("Butter", "1 tbsp")},
        false
    });
    samples.push_back({
        std::make_unique<DessertRecipe>(generateId(), "Vegan Berry Tart", "A vegan berry tart with fruit filling", 20, "Easy", 6, 220.0),
        {Ingredient("Flour", "100g"), Ingredient("Berries", "120g"), Ingredient("Sugar", "50g"), Ingredient("Coconut Milk", "100ml")},
        true
    });

    int added = 0;
    for (auto& sample : samples) {
        if (!sample.recipe) {
            continue;
        }
        if (alreadyExists(sample.recipe->getTitle())) {
            continue;
        }
        sample.recipe->setVegan(sample.vegan);
        for (auto& ingredient : sample.ingredients) {
            sample.recipe->addIngredient(ingredient);
        }
        addRecipe(std::move(sample.recipe));
        added++;
    }

    std::cout << "SUCCESS: Added " << added << " sample recipe(s)." << std::endl;
}
