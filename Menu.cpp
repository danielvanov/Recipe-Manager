#include "Menu.h"
#include "Recipe.h"
#include "DessertRecipe.h"
#include "MainDishRecipe.h"
#include "FileManager.h"
#include <iostream>
#include <limits>
#include <string>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <cstdlib>

static std::string trim(const std::string& text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    size_t end = text.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : text.substr(start, end - start + 1);
}

static bool getInputLine(std::string& input) {
    if (!std::getline(std::cin, input)) {
        if (std::cin.eof()) {
            std::cout << std::endl << "INFO: End of input reached. Exiting." << std::endl;
            std::exit(0);
        }
        std::cin.clear();
        return false;
    }
    return true;
}

static std::string toLower(const std::string& text) {
    std::string result = text;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return result;
}

Menu::Menu() {
    initializeRecipes();
}

void Menu::start() {
    while (true) {
        showMainMenu();
        int choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                handleAddRecipe();
                break;
            case 2:
                handleListRecipes();
                break;
            case 3:
                handleSearchRecipe();
                break;
            case 4:
                handleDeleteRecipe();
                break;
            case 5:
                handleRateRecipe();
                break;
            case 6:
                handleFilterByDifficulty();
                break;
            case 7:
                handleFilterByCategory();
                break;
            case 8:
                handleSortByCookingTime();
                break;
            case 9:
                handleSortByRating();
                break;
            case 10:
                handleShowDetailsById();
                break;
            case 11:
                handleAddSampleRecipes();
                break;
            case 12:
                handleSaveRecipes();
                break;
            case 13:
                handleLoadRecipes();
                break;
            case 14:
                handleShowHealthyRecipes();
                break;
            case 15:
                handleShowUnhealthyRecipes();
                break;
            case 16:
                handleFilterByMenuType();
                break;
            case 0:
                finalizeRecipes();
                return;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }
}

void Menu::showMainMenu() const {
    std::cout << "\nRecipe Manager Menu:" << std::endl;
    std::cout << "1. Add recipe" << std::endl;
    std::cout << "2. Show all recipes" << std::endl;
    std::cout << "3. Search recipe by title" << std::endl;
    std::cout << "4. Delete recipe by id" << std::endl;
    std::cout << "5. Rate recipe" << std::endl;
    std::cout << "6. Filter recipes by difficulty" << std::endl;
    std::cout << "7. Filter recipes by category" << std::endl;
    std::cout << "8. Sort recipes by cooking time" << std::endl;
    std::cout << "9. Sort recipes by rating" << std::endl;
    std::cout << "10. Show recipe details by id" << std::endl;
    std::cout << "11. Add sample recipes" << std::endl;
    std::cout << "12. Save recipes to file" << std::endl;
    std::cout << "13. Load recipes from file" << std::endl;
    std::cout << "14. Show healthy recipes" << std::endl;
    std::cout << "15. Show unhealthy recipes" << std::endl;
    std::cout << "16. Show dietary menu (healthy/unhealthy/vegan) - starter, main dish, dessert" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

void Menu::initializeRecipes() {
    std::ifstream file("recipes.txt");
    if (file.good()) {
        if (readYesNo("A recipes.txt file was found. Load recipes from file? (y/n): ")) {
            FileManager::loadRecipesFromFile(manager, "recipes.txt");
        }
    } else {
        std::cout << "INFO: No recipes.txt found. Starting with an empty database." << std::endl;
    }
}

void Menu::finalizeRecipes() {
    if (readYesNo("Do you want to save recipes before exiting? (y/n): ")) {
        FileManager::saveRecipesToFile(manager.getRecipes(), "recipes.txt");
    }
}

int Menu::readInt(const std::string& message) const {
    while (true) {
        std::cout << message;
        std::string input;
        if (!getInputLine(input)) {
            std::cout << "Invalid number. Please enter a valid integer." << std::endl;
            continue;
        }
        input = trim(input);
        try {
            size_t pos;
            int value = std::stoi(input, &pos);
            if (pos == input.size()) {
                return value;
            }
        } catch (...) {
        }
        std::cout << "Invalid number. Please enter a valid integer." << std::endl;
    }
}

int Menu::readIntInRange(const std::string& message, int min, int max) const {
    while (true) {
        int value = readInt(message);
        if (value >= min && value <= max) {
            return value;
        }
        std::cout << "Please enter a value between " << min << " and " << max << "." << std::endl;
    }
}

double Menu::readDouble(const std::string& message) const {
    while (true) {
        std::cout << message;
        std::string input;
        if (!getInputLine(input)) {
            std::cout << "Invalid number. Please enter a valid number." << std::endl;
            continue;
        }
        input = trim(input);
        try {
            size_t pos;
            double value = std::stod(input, &pos);
            if (pos == input.size()) {
                return value;
            }
        } catch (...) {
        }
        std::cout << "Invalid number. Please enter a valid number." << std::endl;
    }
}

double Menu::readNonNegativeDouble(const std::string& message) const {
    while (true) {
        double value = readDouble(message);
        if (value >= 0.0) {
            return value;
        }
        std::cout << "Invalid input. Please enter a non-negative number." << std::endl;
    }
}

std::string Menu::readNonEmptyString(const std::string& message) const {
    while (true) {
        std::cout << message;
        std::string input;
        if (!getInputLine(input)) {
            std::cout << "This field cannot be empty. Please try again." << std::endl;
            continue;
        }
        input = trim(input);
        if (!input.empty()) {
            return input;
        }
        std::cout << "This field cannot be empty. Please try again." << std::endl;
    }
}

std::string Menu::readDifficulty() const {
    while (true) {
        std::string input = readNonEmptyString("Enter difficulty (Easy/Medium/Hard): ");
        std::string value = toLower(input);
        if (value == "easy") {
            return "Easy";
        } else if (value == "medium") {
            return "Medium";
        } else if (value == "hard") {
            return "Hard";
        }
        std::cout << "Difficulty must be Easy, Medium, or Hard." << std::endl;
    }
}

bool Menu::readYesNo(const std::string& message) const {
    while (true) {
        std::cout << message;
        std::string input;
        if (!getInputLine(input)) {
            std::cout << "Please enter 'y' or 'n'." << std::endl;
            continue;
        }
        std::string value = toLower(trim(input));
        if (value == "y" || value == "yes") {
            return true;
        }
        if (value == "n" || value == "no") {
            return false;
        }
        std::cout << "Please enter 'y' or 'n'." << std::endl;
    }
}

void Menu::handleAddRecipe() {
    std::cout << "Choose recipe type:" << std::endl;
    std::cout << "1. General recipe" << std::endl;
    std::cout << "2. Dessert recipe" << std::endl;
    std::cout << "3. Main dish recipe" << std::endl;
    int type = readIntInRange("Enter type (1-3): ", 1, 3);

    std::string title = readNonEmptyString("Enter title: ");
    std::string description = readNonEmptyString("Enter description: ");
    int cookingTime = readIntInRange("Enter cooking time (minutes): ", 1, 10000);
    std::string difficulty = readDifficulty();
    double calories = readNonNegativeDouble("Enter calories: ");

    Recipe* recipe = nullptr;
    try {
        if (type == 1) {
            std::string category = readNonEmptyString("Enter category: ");
            recipe = new Recipe(manager.generateId(), title, description, cookingTime, difficulty, category, calories);
        } else if (type == 2) {
            int sweetnessLevel = readIntInRange("Enter sweetness level (1-10): ", 1, 10);
            recipe = new DessertRecipe(manager.generateId(), title, description, cookingTime, difficulty, sweetnessLevel, calories);
        } else {
            bool vegetarian = readYesNo("Is it vegetarian? (y/n): ");
            recipe = new MainDishRecipe(manager.generateId(), title, description, cookingTime, difficulty, vegetarian, calories);
        }

        if (!recipe) {
            std::cerr << "ERROR: Memory allocation failed. Cannot create recipe." << std::endl;
            return;
        }

        int ingredientCount = readIntInRange("Enter number of ingredients (1-50): ", 1, 50);
        for (int i = 0; i < ingredientCount; ++i) {
            std::string name = readNonEmptyString("Ingredient " + std::to_string(i + 1) + " name: ");
            std::string quantity = readNonEmptyString("Ingredient " + std::to_string(i + 1) + " quantity: ");
            recipe->addIngredient(Ingredient(name, quantity));
        }

        if (recipe->getIngredients().empty()) {
            std::cerr << "ERROR: Recipe must have at least one ingredient. Recipe not added." << std::endl;
            delete recipe;
            return;
        }

        manager.addRecipe(recipe);
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Failed to add recipe: " << e.what() << std::endl;
        if (recipe) delete recipe;
    } catch (...) {
        std::cerr << "ERROR: Unexpected error while adding recipe." << std::endl;
        if (recipe) delete recipe;
    }
}

void Menu::handleListRecipes() const {
    if (manager.getRecipes().empty()) {
        std::cout << "INFO: No recipes in the database." << std::endl;
        return;
    }
    manager.listAllRecipes();
}

void Menu::handleSearchRecipe() const {
    if (manager.getRecipes().empty()) {
        std::cout << "INFO: No recipes in the database to search." << std::endl;
        return;
    }
    std::string keyword = readNonEmptyString("Enter title keyword to search: ");
    manager.searchByTitle(keyword);
}

void Menu::handleDeleteRecipe() {
    if (manager.getRecipes().empty()) {
        std::cout << "INFO: No recipes in the database to delete." << std::endl;
        return;
    }
    int id = readInt("Enter recipe ID to delete: ");
    manager.deleteRecipeById(id);
}

void Menu::handleRateRecipe() {
    if (manager.getRecipes().empty()) {
        std::cout << "INFO: No recipes in the database to rate." << std::endl;
        return;
    }
    int id = readInt("Enter recipe ID to rate: ");
    int rating = readIntInRange("Enter rating (1-5): ", 1, 5);
    manager.rateRecipe(id, rating);
}

void Menu::handleFilterByDifficulty() const {
    if (manager.getRecipes().empty()) {
        std::cout << "INFO: No recipes in the database to filter." << std::endl;
        return;
    }
    std::string difficulty = readDifficulty();
    manager.filterByDifficulty(difficulty);
}

void Menu::handleFilterByCategory() const {
    if (manager.getRecipes().empty()) {
        std::cout << "INFO: No recipes in the database to filter." << std::endl;
        return;
    }
    std::string category = readNonEmptyString("Enter category to filter by (Main Dish, Dessert, Breakfast, Lunch, Dinner, vegetarian): ");
    manager.filterByCategory(category);
}

void Menu::handleSortByCookingTime() {
    if (manager.getRecipes().empty()) {
        std::cout << "INFO: No recipes to sort." << std::endl;
        return;
    }
    manager.sortByCookingTime();
}

void Menu::handleSortByRating() {
    if (manager.getRecipes().empty()) {
        std::cout << "INFO: No recipes to sort." << std::endl;
        return;
    }
    manager.sortByRating();
}

void Menu::handleShowDetailsById() const {
    if (manager.getRecipes().empty()) {
        std::cout << "INFO: No recipes in the database." << std::endl;
        return;
    }
    int id = readInt("Enter recipe ID to view details: ");
    manager.showRecipeDetailsById(id);
}

void Menu::handleAddSampleRecipes() {
    manager.addSampleRecipes();
}

void Menu::handleSaveRecipes() {
    if (manager.getRecipes().empty()) {
        std::cout << "WARNING: No recipes to save." << std::endl;
        return;
    }
    FileManager::saveRecipesToFile(manager.getRecipes(), "recipes.txt");
}

void Menu::handleFilterByMenuType() const {
    if (manager.getRecipes().empty()) {
        std::cout << "INFO: No recipes in the database." << std::endl;
        return;
    }

    std::cout << "Select menu type:" << std::endl;
    std::cout << "1. Healthy" << std::endl;
    std::cout << "2. Unhealthy" << std::endl;
    std::cout << "3. Vegan" << std::endl;
    int type = readIntInRange("Enter choice (1-3): ", 1, 3);

    if (type == 1) {
        manager.filterByMenuType("healthy");
    } else if (type == 2) {
        manager.filterByMenuType("unhealthy");
    } else {
        manager.filterByMenuType("vegan");
    }
}

void Menu::handleLoadRecipes() {
    std::ifstream file("recipes.txt");
    if (!file.good()) {
        std::cout << "WARNING: File 'recipes.txt' does not exist. Cannot load recipes." << std::endl;
        return;
    }
    file.close();

    if (readYesNo("Loading from file will replace current recipes. Continue? (y/n): ")) {
        FileManager::loadRecipesFromFile(manager, "recipes.txt");
    } else {
        std::cout << "INFO: Load operation cancelled." << std::endl;
    }
}

void Menu::handleShowHealthyRecipes() const {
    if (manager.getRecipes().empty()) {
        std::cout << "INFO: No recipes in the database." << std::endl;
        return;
    }
    std::cout << "Healthy Recipes (Calories < 500):" << std::endl;
    bool found = false;
    for (const auto& recipe : manager.getRecipes()) {
        if (recipe->getCalories() < 500) {
            recipe->displayInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No healthy recipes found." << std::endl;
    }
}

void Menu::handleShowUnhealthyRecipes() const {
    if (manager.getRecipes().empty()) {
        std::cout << "INFO: No recipes in the database." << std::endl;
        return;
    }
    std::cout << "Unhealthy Recipes (Calories >= 500):" << std::endl;
    bool found = false;
    for (const auto& recipe : manager.getRecipes()) {
        if (recipe->getCalories() >= 500) {
            recipe->displayInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No unhealthy recipes found." << std::endl;
    }
}
