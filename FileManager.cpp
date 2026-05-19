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

static std::string removeTrailingComma(const std::string& text) {
    std::string trimmed = trim(text);
    if (!trimmed.empty() && trimmed.back() == ',') {
        trimmed.pop_back();
    }
    return trim(trimmed);
}

static std::string parseJsonStringValue(const std::string& line) {
    size_t pos = line.find(':');
    if (pos == std::string::npos) {
        return "";
    }
    std::string value = removeTrailingComma(line.substr(pos + 1));
    value = trim(value);
    if (!value.empty() && value.front() == '"') {
        value.erase(0, 1);
    }
    if (!value.empty() && value.back() == '"') {
        value.pop_back();
    }
    return value;
}

static bool parseJsonBoolValue(const std::string& line) {
    std::string value = removeTrailingComma(line.substr(line.find(':') + 1));
    return trim(value) == "true";
}

static int parseJsonIntValue(const std::string& line) {
    std::string value = removeTrailingComma(line.substr(line.find(':') + 1));
    value = trim(value);
    try {
        return std::stoi(value);
    } catch (...) {
        return 0;
    }
}

static double parseJsonDoubleValue(const std::string& line) {
    std::string value = removeTrailingComma(line.substr(line.find(':') + 1));
    value = trim(value);
    try {
        return std::stod(value);
    } catch (...) {
        return 0.0;
    }
}

static bool startsWith(const std::string& text, const std::string& prefix) {
    return text.size() >= prefix.size() && text.compare(0, prefix.size(), prefix) == 0;
}

void FileManager::saveRecipesToFile(const std::vector<std::unique_ptr<Recipe>>& recipes, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Unable to open file '" << filename << "' for writing. Check file permissions." << std::endl;
        return;
    }

    file << "[\n";
    for (size_t i = 0; i < recipes.size(); ++i) {
        const auto& recipe = recipes[i];
        if (!recipe) {
            continue;
        }

        const DessertRecipe* dessert = dynamic_cast<const DessertRecipe*>(recipe.get());
        const MainDishRecipe* mainDish = dynamic_cast<const MainDishRecipe*>(recipe.get());
        file << "  {\n";
        file << "    \"type\": \"" << (dessert ? "Dessert" : mainDish ? "MainDish" : "General") << "\",\n";
        file << "    \"id\": " << recipe->getId() << ",\n";
        file << "    \"title\": \"" << recipe->getTitle() << "\",\n";
        file << "    \"description\": \"" << recipe->getDescription() << "\",\n";
        file << "    \"time\": " << recipe->getCookingTime() << ",\n";
        file << "    \"difficulty\": \"" << recipe->getDifficulty() << "\",\n";
        file << "    \"category\": \"" << recipe->getCategory() << "\",\n";
        file << "    \"calories\": " << recipe->getCalories() << ",\n";
        file << "    \"vegan\": " << (recipe->isVegan() ? "true" : "false") << ",\n";
        if (mainDish) {
            file << "    \"vegetarian\": " << (mainDish->isVegetarian() ? "true" : "false") << ",\n";
        }
        if (dessert) {
            file << "    \"sweetness\": " << dessert->getSweetnessLevel() << ",\n";
        }
        file << "    \"rating\": " << recipe->getRating() << ",\n";
        file << "    \"rating_count\": " << recipe->getRatingCount() << ",\n";
        file << "    \"ingredients\": [\n";
        const auto& ingredients = recipe->getIngredients();
        for (size_t j = 0; j < ingredients.size(); ++j) {
            const auto& ingredient = ingredients[j];
            file << "      { \"name\": \"" << ingredient.getName() << "\", \"quantity\": \"" << ingredient.getQuantity() << "\" }";
            if (j + 1 < ingredients.size()) {
                file << ",";
            }
            file << "\n";
        }
        file << "    ]\n";
        file << "  }";
        if (i + 1 < recipes.size()) {
            file << ",";
        }
        file << "\n";
    }
    file << "]\n";

    if (!file.good()) {
        std::cerr << "ERROR: Write error occurred while saving to '" << filename << "'. Some data may not have been written." << std::endl;
    }

    file.close();
    std::cout << "SUCCESS: Saved " << recipes.size() << " recipe(s) to '" << filename << "'." << std::endl;
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
    bool inRecipe = false;
    bool inIngredients = false;

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
    bool vegan = false;
    double rating = 0.0;
    int ratingCount = 0;
    std::vector<Ingredient> ingredients;
    bool parsingError = false;

    auto resetRecipe = [&]() {
        type.clear();
        id = 0;
        title.clear();
        description.clear();
        cookingTime = 0;
        difficulty.clear();
        category.clear();
        calories = 0.0;
        sweetnessLevel = 0;
        vegetarian = false;
        vegan = false;
        rating = 0.0;
        ratingCount = 0;
        ingredients.clear();
        parsingError = false;
        inIngredients = false;
    };

    resetRecipe();

    while (std::getline(file, line)) {
        lineNum++;
        std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed == "[" || trimmed == ",") {
            continue;
        }

        if (!inRecipe) {
            if (trimmed == "]") {
                continue;
            }
            if (trimmed == "{") {
                inRecipe = true;
                resetRecipe();
                continue;
            }
            continue;
        }

        if (trimmed == "}" || trimmed == "},") {
            inRecipe = false;
            if (parsingError) {
                skippedCount++;
                continue;
            }

            if (id <= 0 || title.empty() || ingredients.empty() || cookingTime <= 0) {
                std::cerr << "WARNING: Invalid recipe object ending at line " << lineNum << ". Skipping." << std::endl;
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
                    auto dessert = std::make_unique<DessertRecipe>(id, title, description, cookingTime, difficulty, sweetnessLevel);
                    dessert->setVegan(vegan);
                    recipe = std::move(dessert);
                } else if (type == "MainDish") {
                    auto mainDish = std::make_unique<MainDishRecipe>(id, title, description, cookingTime, difficulty, vegetarian);
                    mainDish->setVegan(vegan);
                    recipe = std::move(mainDish);
                } else {
                    auto general = std::make_unique<Recipe>(id, title, description, cookingTime, difficulty, category);
                    general->setVegan(vegan);
                    recipe = std::move(general);
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
            continue;
        }

        if (startsWith(trimmed, "\"ingredients\"")) {
            inIngredients = true;
            continue;
        }

        if (inIngredients) {
            if (trimmed == "]" || trimmed == "],") {
                inIngredients = false;
                continue;
            }
            std::string name;
            std::string qty;
            size_t namePos = trimmed.find("\"name\"");
            size_t qtyPos = trimmed.find("\"quantity\"");
            if (namePos != std::string::npos && qtyPos != std::string::npos) {
                size_t colon = trimmed.find(':', namePos);
                if (colon != std::string::npos) {
                    name = parseJsonStringValue(trimmed.substr(namePos, qtyPos - namePos));
                }
                colon = trimmed.find(':', qtyPos);
                if (colon != std::string::npos) {
                    qty = parseJsonStringValue(trimmed.substr(qtyPos));
                }
                if (!name.empty() && !qty.empty()) {
                    ingredients.emplace_back(name, qty);
                }
            }
            continue;
        }

        if (startsWith(trimmed, "\"type\"")) {
            type = parseJsonStringValue(trimmed);
        } else if (startsWith(trimmed, "\"id\"")) {
            id = parseJsonIntValue(trimmed);
            if (id <= 0) {
                std::cerr << "WARNING: Invalid ID at line " << lineNum << ". Skipping recipe." << std::endl;
                parsingError = true;
            }
        } else if (startsWith(trimmed, "\"title\"")) {
            title = parseJsonStringValue(trimmed);
            if (title.empty()) {
                std::cerr << "WARNING: Empty title at line " << lineNum << ". Skipping recipe." << std::endl;
                parsingError = true;
            }
        } else if (startsWith(trimmed, "\"description\"")) {
            description = parseJsonStringValue(trimmed);
        } else if (startsWith(trimmed, "\"time\"")) {
            cookingTime = parseJsonIntValue(trimmed);
            if (cookingTime <= 0) {
                std::cerr << "WARNING: Invalid cooking time at line " << lineNum << ". Skipping recipe." << std::endl;
                parsingError = true;
            }
        } else if (startsWith(trimmed, "\"difficulty\"")) {
            difficulty = parseJsonStringValue(trimmed);
        } else if (startsWith(trimmed, "\"category\"")) {
            category = parseJsonStringValue(trimmed);
        } else if (startsWith(trimmed, "\"calories\"")) {
            calories = parseJsonDoubleValue(trimmed);
            if (calories < 0.0) {
                std::cerr << "WARNING: Negative calories at line " << lineNum << ". Setting to 0." << std::endl;
                calories = 0.0;
            }
        } else if (startsWith(trimmed, "\"vegan\"")) {
            vegan = parseJsonBoolValue(trimmed);
        } else if (startsWith(trimmed, "\"vegetarian\"")) {
            vegetarian = parseJsonBoolValue(trimmed);
        } else if (startsWith(trimmed, "\"sweetness\"")) {
            sweetnessLevel = parseJsonIntValue(trimmed);
            if (sweetnessLevel < 1 || sweetnessLevel > 10) {
                std::cerr << "WARNING: Sweetness level out of range at line " << lineNum << ". Using default." << std::endl;
                sweetnessLevel = 5;
            }
        } else if (startsWith(trimmed, "\"rating\"")) {
            rating = parseJsonDoubleValue(trimmed);
            if (rating < 0.0) {
                std::cerr << "WARNING: Negative rating at line " << lineNum << ". Setting to 0." << std::endl;
                rating = 0.0;
            }
        } else if (startsWith(trimmed, "\"rating_count\"")) {
            ratingCount = parseJsonIntValue(trimmed);
            if (ratingCount < 0) {
                std::cerr << "WARNING: Negative rating count at line " << lineNum << ". Setting to 0." << std::endl;
                ratingCount = 0;
            }
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

