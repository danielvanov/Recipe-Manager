#include "FileManager.h"
#include "RecipeManager.h"
#include "Recipe.h"
#include "DessertRecipe.h"
#include "MainDishRecipe.h"
#include "Ingredient.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_set>

static std::string trim(const std::string& text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    size_t end = text.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : text.substr(start, end - start + 1);
}

static std::string getValue(const std::string& line) {
    size_t pos = line.find('=');
    if (pos == std::string::npos) {
        return "";
    }
    return trim(line.substr(pos + 1));
}

void FileManager::saveRecipesToFile(const std::vector<std::unique_ptr<Recipe>>& recipes, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Unable to open file '" << filename << "' for writing. Check file permissions." << std::endl;
        return;
    }

    if (recipes.empty()) {
        std::cout << "WARNING: No recipes to save. File '" << filename << "' will be empty." << std::endl;
    }

    int savedCount = 0;
    for (const auto& recipe : recipes) {
        if (!recipe) {
            std::cerr << "ERROR: Encountered null recipe pointer during save. Skipping." << std::endl;
            continue;
        }

        file << "RECIPE\n";

        const DessertRecipe* dessert = dynamic_cast<const DessertRecipe*>(recipe.get());
        const MainDishRecipe* mainDish = dynamic_cast<const MainDishRecipe*>(recipe.get());
        if (dessert) {
            file << "TYPE=Dessert\n";
        } else if (mainDish) {
            file << "TYPE=MainDish\n";
        } else {
            file << "TYPE=General\n";
        }

        file << "ID=" << recipe->getId() << "\n";
        file << "TITLE=" << recipe->getTitle() << "\n";
        file << "DESCRIPTION=" << recipe->getDescription() << "\n";
        file << "TIME=" << recipe->getCookingTime() << "\n";
        file << "DIFFICULTY=" << recipe->getDifficulty() << "\n";
        file << "CATEGORY=" << recipe->getCategory() << "\n";
        file << "CALORIES=" << recipe->getCalories() << "\n";

        if (dessert) {
            file << "SWEETNESS=" << dessert->getSweetnessLevel() << "\n";
        }
        if (mainDish) {
            file << "VEGETARIAN=" << (mainDish->isVegetarian() ? 1 : 0) << "\n";
        }

        file << "RATING=" << recipe->getRating() << "\n";
        file << "RATING_COUNT=" << recipe->getRatingCount() << "\n";
        file << "INGREDIENTS=" << recipe->getIngredients().size() << "\n";
        for (const auto& ingredient : recipe->getIngredients()) {
            file << ingredient.getName() << "|" << ingredient.getQuantity() << "\n";
        }
        file << "END\n";
        savedCount++;
    }

    if (!file.good()) {
        std::cerr << "ERROR: Write error occurred while saving to '" << filename << "'. Some data may not have been written." << std::endl;
    }

    file.close();
    std::cout << "SUCCESS: Saved " << savedCount << " recipe(s) to '" << filename << "'." << std::endl;
}

void FileManager::loadRecipesFromFile(RecipeManager& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: File not found: '" << filename << "'. Cannot load recipes." << std::endl;
        return;
    }

    std::vector<std::unique_ptr<Recipe>> loadedRecipes;
    std::unordered_set<int> loadedIds;
    std::string line;
    int loadedCount = 0;
    int skippedCount = 0;
    int lineNum = 0;

    while (std::getline(file, line)) {
        lineNum++;
        line = trim(line);
        if (line != "RECIPE") {
            continue;
        }

        std::string type;
        int id = 0;
        std::string title;
        std::string description;
        int cookingTime = 0;
        std::string difficulty;
        std::string category;
        double calories = 0.0;
        int sweetnessLevel = 0;
        bool vegetarian = false;
        double rating = 0.0;
        int ratingCount = 0;
        int ingredientCount = 0;
        std::vector<Ingredient> ingredients;
        bool parsingError = false;

        while (std::getline(file, line)) {
            lineNum++;
            line = trim(line);
            if (line == "END" || line.empty()) {
                break;
            }
            try {
                if (line.rfind("TYPE=", 0) == 0) {
                    type = getValue(line);
                } else if (line.rfind("ID=", 0) == 0) {
                    id = std::stoi(getValue(line));
                    if (id <= 0) {
                        std::cerr << "WARNING: Invalid ID at line " << lineNum << ": " << id << ". Skipping recipe." << std::endl;
                        parsingError = true;
                        break;
                    }
                } else if (line.rfind("TITLE=", 0) == 0) {
                    title = getValue(line);
                    if (title.empty()) {
                        std::cerr << "WARNING: Empty title at line " << lineNum << ". Skipping recipe." << std::endl;
                        parsingError = true;
                        break;
                    }
                } else if (line.rfind("DESCRIPTION=", 0) == 0) {
                    description = getValue(line);
                } else if (line.rfind("TIME=", 0) == 0) {
                    cookingTime = std::stoi(getValue(line));
                    if (cookingTime <= 0) {
                        std::cerr << "WARNING: Invalid cooking time at line " << lineNum << ": " << cookingTime << ". Skipping recipe." << std::endl;
                        parsingError = true;
                        break;
                    }
                } else if (line.rfind("DIFFICULTY=", 0) == 0) {
                    difficulty = getValue(line);
                } else if (line.rfind("CATEGORY=", 0) == 0) {
                    category = getValue(line);
                } else if (line.rfind("CALORIES=", 0) == 0) {
                    calories = std::stod(getValue(line));
                    if (calories < 0.0) {
                        std::cerr << "WARNING: Negative calories at line " << lineNum << ": " << calories << ". Setting to 0." << std::endl;
                        calories = 0.0;
                    }
                } else if (line.rfind("SWEETNESS=", 0) == 0) {
                    sweetnessLevel = std::stoi(getValue(line));
                    if (sweetnessLevel < 1 || sweetnessLevel > 10) {
                        std::cerr << "WARNING: Sweetness level out of range at line " << lineNum << ": " << sweetnessLevel << ". Using default." << std::endl;
                        sweetnessLevel = 5;
                    }
                } else if (line.rfind("VEGETARIAN=", 0) == 0) {
                    vegetarian = std::stoi(getValue(line)) != 0;
                } else if (line.rfind("RATING=", 0) == 0) {
                    rating = std::stod(getValue(line));
                    if (rating < 0.0) {
                        std::cerr << "WARNING: Negative rating at line " << lineNum << ": " << rating << ". Setting to 0." << std::endl;
                        rating = 0.0;
                    }
                } else if (line.rfind("RATING_COUNT=", 0) == 0) {
                    ratingCount = std::stoi(getValue(line));
                    if (ratingCount < 0) {
                        std::cerr << "WARNING: Negative rating count at line " << lineNum << ". Setting to 0." << std::endl;
                        ratingCount = 0;
                    }
                } else if (line.rfind("INGREDIENTS=", 0) == 0) {
                    ingredientCount = std::stoi(getValue(line));
                    for (int i = 0; i < ingredientCount && std::getline(file, line); ++i) {
                        lineNum++;
                        line = trim(line);
                        size_t sep = line.find('|');
                        if (sep != std::string::npos) {
                            std::string name = trim(line.substr(0, sep));
                            std::string quantity = trim(line.substr(sep + 1));
                            if (name.empty() || quantity.empty()) {
                                std::cerr << "WARNING: Empty ingredient name or quantity at line " << lineNum << ". Skipping ingredient." << std::endl;
                                continue;
                            }
                            ingredients.emplace_back(name, quantity);
                        } else {
                            std::cerr << "WARNING: Malformed ingredient at line " << lineNum << " (missing '|' separator). Skipping." << std::endl;
                        }
                    }
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "WARNING: Invalid numeric value at line " << lineNum << ": " << e.what() << ". Skipping recipe." << std::endl;
                parsingError = true;
                break;
            } catch (const std::out_of_range& e) {
                std::cerr << "WARNING: Numeric value out of range at line " << lineNum << ": " << e.what() << ". Skipping recipe." << std::endl;
                parsingError = true;
                break;
            } catch (...) {
                std::cerr << "WARNING: Unexpected error at line " << lineNum << ". Skipping recipe." << std::endl;
                parsingError = true;
                break;
            }
        }

        if (parsingError) {
            skippedCount++;
            continue;
        }

        if (title.empty()) {
            std::cerr << "WARNING: Recipe with ID " << id << " has no title. Skipping." << std::endl;
            skippedCount++;
            continue;
        }

        if (ingredients.empty()) {
            std::cerr << "WARNING: Recipe '" << title << "' (ID " << id << ") has no ingredients. Skipping." << std::endl;
            skippedCount++;
            continue;
        }

        if (type != "Dessert" && type != "MainDish" && category.empty()) {
            std::cerr << "WARNING: General recipe '" << title << "' (ID " << id << ") requires a category. Skipping." << std::endl;
            skippedCount++;
            continue;
        }

        if (!loadedIds.insert(id).second) {
            std::cerr << "WARNING: Duplicate recipe ID " << id << " found in file. Skipping." << std::endl;
            skippedCount++;
            continue;
        }

        std::unique_ptr<Recipe> recipe;
        try {
            if (type == "Dessert") {
                recipe = std::make_unique<DessertRecipe>(id, title, description, cookingTime, difficulty, sweetnessLevel);
            } else if (type == "MainDish") {
                recipe = std::make_unique<MainDishRecipe>(id, title, description, cookingTime, difficulty, vegetarian);
            } else {
                recipe = std::make_unique<Recipe>(id, title, description, cookingTime, difficulty, category);
            }

            if (!recipe) {
                std::cerr << "ERROR: Memory allocation failed for recipe '" << title << "'. Skipping." << std::endl;
                skippedCount++;
                continue;
            }

            for (const auto& ingredient : ingredients) {
                recipe->addIngredient(ingredient);
            }
            recipe->setRatingData(rating, ratingCount);
            recipe->setCalories(calories);
            loadedRecipes.push_back(std::move(recipe));
            loadedCount++;
        } catch (...) {
            std::cerr << "ERROR: Failed to create recipe '" << title << "'. Skipping." << std::endl;
            skippedCount++;
        }
    }

    file.close();
    if (loadedCount > 0) {
        manager.replaceRecipes(std::move(loadedRecipes));
        std::cout << "SUCCESS: Loaded " << loadedCount << " recipe(s) from '" << filename << "'";
        if (skippedCount > 0) {
            std::cout << " (" << skippedCount << " recipe(s) skipped due to errors)";
        }
        std::cout << "." << std::endl;
    } else {
        std::cout << "WARNING: No valid recipes loaded from '" << filename << "'. Existing data preserved." << std::endl;
    }
}