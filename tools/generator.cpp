#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

struct RecipeData {
    std::string title;
    std::string description;
    int time;
    std::string difficulty;
    std::string category;
    double calories;
    std::vector<std::string> ingredients;
};

std::vector<RecipeData> generateRecipes(int count) {
    std::vector<RecipeData> recipes;
    std::vector<std::string> titles = {"Spaghetti Bolognese", "Chicken Curry", "Beef Stew", "Fish Tacos", "Vegetable Stir Fry", "Pancakes", "Grilled Cheese", "Caesar Salad", "Tomato Soup", "Chicken Nuggets"};
    std::vector<std::string> descriptions = {"Delicious pasta dish", "Spicy curry", "Hearty stew", "Fresh tacos", "Quick stir fry", "Fluffy pancakes", "Melted cheese", "Crisp salad", "Warm soup", "Crispy nuggets"};
    std::vector<std::string> difficulties = {"Easy", "Medium", "Hard"};
    std::vector<std::string> categories = {"Main Dish", "Dessert", "Breakfast", "Lunch", "Dinner"};
    std::vector<std::string> ing = {"Flour|200g", "Eggs|2", "Milk|100ml", "Sugar|50g", "Salt|1tsp", "Butter|50g", "Chicken|200g", "Beef|300g", "Fish|150g", "Vegetables|200g"};

    srand(time(0));
    for(int i = 0; i < count; i++) {
        RecipeData r;
        r.title = titles[rand() % titles.size()] + " " + std::to_string(i+1);
        r.description = descriptions[rand() % descriptions.size()];
        r.time = 10 + rand() % 60;
        r.difficulty = difficulties[rand() % difficulties.size()];
        r.category = categories[rand() % categories.size()];
        r.calories = 100 + rand() % 800;
        int ingCount = 2 + rand() % 4;
        for(int j = 0; j < ingCount; j++) {
            r.ingredients.push_back(ing[rand() % ing.size()]);
        }
        recipes.push_back(r);
    }
    return recipes;
}

void saveRecipes(const std::vector<RecipeData>& recipes, const std::string& filename) {
    std::ofstream file(filename, std::ios::app); // append
    int id = 7; // start from 7
    for(const auto& r : recipes) {
        file << "RECIPE\n";
        file << "TYPE=General\n";
        file << "ID=" << id++ << "\n";
        file << "TITLE=" << r.title << "\n";
        file << "DESCRIPTION=" << r.description << "\n";
        file << "TIME=" << r.time << "\n";
        file << "DIFFICULTY=" << r.difficulty << "\n";
        file << "CATEGORY=" << r.category << "\n";
        file << "CALORIES=" << r.calories << "\n";
        file << "RATING=0\n";
        file << "RATING_COUNT=0\n";
        file << "INGREDIENTS=" << r.ingredients.size() << "\n";
        for(const auto& ing : r.ingredients) {
            file << ing << "\n";
        }
        file << "END\n";
    }
}

int main() {
    auto recipes = generateRecipes(500);
    saveRecipes(recipes, "recipes.txt");
    std::cout << "Added 500 recipes to recipes.txt" << std::endl;
    return 0;
}