#include "Menu.h"
#include "Recipe.h"
#include "DessertRecipe.h"
#include "MainDishRecipe.h"
#include <iostream>
#include <limits>
#include <string>

Menu::Menu() {
    manager.loadSampleRecipes();
}

void Menu::start() {
    while (true) {
        showMainMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
            case 0:
                return;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
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
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

void Menu::handleAddRecipe() {
    std::cout << "Choose recipe type:" << std::endl;
    std::cout << "1. General recipe" << std::endl;
    std::cout << "2. Dessert recipe" << std::endl;
    std::cout << "3. Main dish recipe" << std::endl;
    int type;
    std::cin >> type;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string title, description, difficulty;
    int cookingTime;

    std::cout << "Enter title: ";
    std::getline(std::cin, title);
    std::cout << "Enter description: ";
    std::getline(std::cin, description);
    std::cout << "Enter cooking time (minutes): ";
    std::cin >> cookingTime;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter difficulty: ";
    std::getline(std::cin, difficulty);

    Recipe* recipe = nullptr;

    if (type == 1) {
        recipe = new Recipe(manager.generateId(), title, description, cookingTime, difficulty, "General");
    } else if (type == 2) {
        int sweetnessLevel;
        std::cout << "Enter sweetness level (1-10): ";
        std::cin >> sweetnessLevel;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        recipe = new DessertRecipe(manager.generateId(), title, description, cookingTime, difficulty, sweetnessLevel);
    } else if (type == 3) {
        bool vegetarian;
        std::cout << "Is it vegetarian? (1 for yes, 0 for no): ";
        std::cin >> vegetarian;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        recipe = new MainDishRecipe(manager.generateId(), title, description, cookingTime, difficulty, vegetarian);
    } else {
        std::cout << "Invalid type." << std::endl;
        return;
    }

    int ingredientCount;
    std::cout << "Enter number of ingredients: ";
    std::cin >> ingredientCount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < ingredientCount; ++i) {
        std::string name, quantity;
        std::cout << "Ingredient " << (i + 1) << " name: ";
        std::getline(std::cin, name);
        std::cout << "Ingredient " << (i + 1) << " quantity: ";
        std::getline(std::cin, quantity);
        recipe->addIngredient(Ingredient(name, quantity));
    }

    manager.addRecipe(recipe);
    std::cout << "Recipe added successfully." << std::endl;
}

void Menu::handleListRecipes() const {
    manager.listAllRecipes();
}

void Menu::handleSearchRecipe() const {
    std::string keyword;
    std::cout << "Enter title keyword: ";
    std::getline(std::cin, keyword);
    manager.searchByTitle(keyword);
}

void Menu::handleDeleteRecipe() {
    int id;
    std::cout << "Enter recipe ID to delete: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (manager.deleteRecipeById(id)) {
        std::cout << "Recipe deleted successfully." << std::endl;
    } else {
        std::cout << "Recipe not found." << std::endl;
    }
}

void Menu::handleRateRecipe() {
    int id, rating;
    std::cout << "Enter recipe ID: ";
    std::cin >> id;
    std::cout << "Enter rating (1-5): ";
    std::cin >> rating;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    manager.rateRecipe(id, rating);
}

void Menu::handleFilterByDifficulty() const {
    std::string difficulty;
    std::cout << "Enter difficulty: ";
    std::getline(std::cin, difficulty);
    manager.filterByDifficulty(difficulty);
}