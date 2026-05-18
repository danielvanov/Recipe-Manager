// generate_recipes.cpp
// Simple C++17 recipe generator that writes recipes.json

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

struct Ingredient {
    std::string name;
    std::string qty;
};

struct Recipe {
    std::string type;
    int id;
    std::string title;
    std::string description;
    int time;
    std::string difficulty;
    std::string category;
    double calories;
    bool vegetarian = false;
    int sweetness = 0; // for desserts
    bool vegan = false;
    int rating = 0;
    int rating_count = 0;
    std::vector<Ingredient> ingredients;
};

static std::string escape_json(const std::string &s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c; break;
        }
    }
    return out;
}

void write_json(const std::vector<Recipe>& recipes, const std::string& path) {
    std::ofstream ofs(path);
    if (!ofs) {
        std::cerr << "ERROR: Unable to open " << path << " for writing." << std::endl;
        return;
    }

    ofs << "[\n";
    for (size_t i = 0; i < recipes.size(); ++i) {
        const Recipe &r = recipes[i];
        ofs << "  {\n";
        ofs << "    \"type\": \"" << escape_json(r.type) << "\"," << "\n";
        ofs << "    \"id\": " << r.id << ",\n";
        ofs << "    \"title\": \"" << escape_json(r.title) << "\",\n";
        ofs << "    \"description\": \"" << escape_json(r.description) << "\",\n";
        ofs << "    \"time\": " << r.time << ",\n";
        ofs << "    \"difficulty\": \"" << escape_json(r.difficulty) << "\",\n";
        ofs << "    \"category\": \"" << escape_json(r.category) << "\",\n";
        ofs << "    \"calories\": " << r.calories << ",\n";
        ofs << "    \"vegan\": " << (r.vegan ? "true" : "false") << ",\n";
        if (r.type == "MainDish" || r.type == "Main Dish") {
            ofs << "    \"vegetarian\": " << (r.vegetarian ? "true" : "false") << ",\n";
        }
        if (r.type == "Dessert") {
            ofs << "    \"sweetness\": " << r.sweetness << ",\n";
        }
        ofs << "    \"rating\": " << r.rating << ",\n";
        ofs << "    \"rating_count\": " << r.rating_count << ",\n";
        ofs << "    \"ingredients\": [\n";
        for (size_t j = 0; j < r.ingredients.size(); ++j) {
            const auto &ing = r.ingredients[j];
            ofs << "      { \"name\": \"" << escape_json(ing.name) << "\", \"quantity\": \"" << escape_json(ing.qty) << "\" }";
            if (j + 1 < r.ingredients.size()) ofs << ",";
            ofs << "\n";
        }
        ofs << "    ]\n";
        ofs << "  }";
        if (i + 1 < recipes.size()) ofs << ",";
        ofs << "\n";
    }
    ofs << "]\n";
}

int main() {
    std::vector<Recipe> recipes;
    recipes.reserve(1100);

    auto add = [&](Recipe r) { recipes.push_back(std::move(r)); };

    int id = 1;

    // Add a few base recipes (matching the previous text file samples)
    {
        Recipe r;
        r.type = "General";
        r.id = id++;
        r.title = "Omelette";
        r.description = "Quick egg omelette with herbs";
        r.time = 10;
        r.difficulty = "Easy";
        r.category = "Breakfast";
        r.calories = 250;
        r.rating = 5;
        r.rating_count = 1;
        r.ingredients = { {"Eggs","3"}, {"Salt","1 pinch"}, {"Butter","1 tbsp"} };
        add(r);
    }
    {
        Recipe r;
        r.type = "Dessert";
        r.id = id++;
        r.title = "Chocolate Cake";
        r.description = "Simple cake with chocolate glaze";
        r.time = 60;
        r.difficulty = "Medium";
        r.category = "Dessert";
        r.calories = 450;
        r.sweetness = 8;
        r.ingredients = { {"Flour","200g"}, {"Cocoa Powder","50g"}, {"Sugar","150g"}, {"Eggs","3"} };
        add(r);
    }
    {
        Recipe r;
        r.type = "MainDish";
        r.id = id++;
        r.title = "Vegetable Pasta";
        r.description = "Pasta with fresh vegetables";
        r.time = 30;
        r.difficulty = "Easy";
        r.category = "Main Dish";
        r.calories = 350;
        r.vegetarian = true;
        r.ingredients = { {"Pasta","300g"}, {"Tomato Sauce","200ml"}, {"Zucchini","1"}, {"Olive Oil","2 tbsp"} };
        add(r);
    }
    {
        Recipe r;
        r.type = "MainDish";
        r.id = id++;
        r.title = "Steak";
        r.description = "Grilled steak with garlic butter";
        r.time = 25;
        r.difficulty = "Medium";
        r.category = "Main Dish";
        r.calories = 650;
        r.vegetarian = false;
        r.ingredients = { {"Beef Steak","250g"}, {"Garlic","2 cloves"}, {"Butter","1 tbsp"} };
        add(r);
    }

    // Generator lists
    std::vector<std::string> starter_categories = {"Starter","Appetizer","Salad","Soup","Snack","Finger Food"};
    std::vector<std::string> starter_titles = {"Garden Salad","Tomato Soup","Bruschetta","Stuffed Mushrooms","Avocado Toast","Quinoa Salad","Spring Rolls","Fruit Platter","Garlic Bread","Caprese Skewers"};
    std::vector<std::string> main_titles = {"Pasta","Curry","Stew","Bowl","Stir Fry","Casserole","Burger","Tacos","Risotto","Paella"};
    std::vector<std::string> dessert_titles = {"Berry Tart","Fruit Parfait","Chocolate Mousse","Cookie Plate","Ice Cream Scoop","Panna Cotta","Brownie","Cupcake","Creme Brulee","Rice Pudding"};
    std::vector<std::string> difficulties = {"Easy","Medium","Hard"};
    std::vector<Ingredient> ingredients = {
        {"Tomatoes","100g"}, {"Lettuce","100g"}, {"Olive Oil","1 tbsp"}, {"Garlic","2 cloves"}, {"Onion","1"},
        {"Salt","1 pinch"}, {"Pepper","1 pinch"}, {"Flour","100g"}, {"Sugar","50g"}, {"Milk","100ml"},
        {"Butter","20g"}, {"Eggs","2"}, {"Cheese","50g"}, {"Rice","150g"}, {"Beans","100g"}, {"Potato","150g"},
        {"Carrot","100g"}, {"Celery","50g"}, {"Mushrooms","100g"}, {"Cucumber","100g"}, {"Spinach","100g"}
    };

    // Create many starters
    for (int idx = 0; idx < 375; ++idx) {
        Recipe r;
        r.type = "General";
        r.id = id++;
        std::string base_name = starter_titles[idx % starter_titles.size()];
        std::string category_type = starter_categories[idx % starter_categories.size()];
        std::string variant;
        if (idx % 5 == 0) variant = "Vegan";
        else if (idx % 7 == 0) variant = "Unhealthy";
        else if (idx % 3 == 0) variant = "Healthy";
        r.title = (variant.empty() ? "" : variant + " ") + base_name;
        r.category = (variant.empty() ? "" : variant + " ") + category_type;
        std::string lower = base_name;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        if (variant == "Vegan") r.description = "A vegan " + lower + " made with plant-based ingredients and fresh greens.";
        else if (variant == "Healthy") r.description = "A healthy " + lower + " with bright vegetables and light seasoning.";
        else if (variant == "Unhealthy") r.description = "An indulgent " + lower + " packed with rich flavors and hearty ingredients.";
        else r.description = "A classic " + lower + " starter with tasty ingredients.";
        r.time = 10 + (idx % 35);
        r.difficulty = difficulties[idx % difficulties.size()];
        r.calories = 180 + (idx % 5) * 30;
        if (variant == "Unhealthy") r.calories += 220;
        else if (variant == "Healthy") r.calories -= 20;
        else if (variant == "Vegan") r.calories -= 10;
        if (r.calories < 120) r.calories = 120;
        r.vegan = (variant == "Vegan");
        int ingrCount = 4;
        for (int j = 0; j < ingrCount; ++j) r.ingredients.push_back(ingredients[(idx + j) % ingredients.size()]);
        add(r);
    }

    // Create mains
    for (int idx = 0; idx < 325; ++idx) {
        Recipe r;
        r.type = "MainDish";
        r.id = id++;
        std::string base_name = main_titles[idx % main_titles.size()];
        std::string variant;
        if (idx % 4 == 0) variant = "Vegan";
        else if (idx % 6 == 0) variant = "Unhealthy";
        else if (idx % 3 == 0) variant = "Healthy";
        r.title = (variant.empty() ? "" : variant + " ") + base_name + " " + std::to_string(r.id);
        r.category = "Main Dish";
        r.description = "Satisfying main dish number " + std::to_string(r.id) + ".";
        r.time = 25 + (idx % 40);
        r.difficulty = difficulties[(idx + 1) % difficulties.size()];
        r.vegetarian = (variant == "Vegan" || (idx % 2 == 0));
        r.vegan = (variant == "Vegan");
        r.calories = 380 + (idx % 6) * 40;
        if (variant == "Unhealthy") r.calories += 180;
        if (variant == "Healthy") r.calories -= 80;
        if (r.calories < 280) r.calories = 280;
        int ingrCount = 5;
        for (int j = 0; j < ingrCount; ++j) r.ingredients.push_back(ingredients[(idx + j + 1) % ingredients.size()]);
        add(r);
    }

    // Create desserts
    for (int idx = 0; idx < 300; ++idx) {
        Recipe r;
        r.type = "Dessert";
        r.id = id++;
        std::string base_name = dessert_titles[idx % dessert_titles.size()];
        std::string variant;
        if (idx % 5 == 0) variant = "Vegan";
        else if (idx % 7 == 0) variant = "Healthy";
        else if (idx % 6 == 0) variant = "Unhealthy";
        r.title = (variant.empty() ? "" : variant + " ") + base_name;
        r.category = (variant.empty() ? "Dessert" : variant + " Dessert");
        std::string lower = base_name;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        if (variant == "Vegan") r.description = "A vegan " + lower + " crafted with wholesome plant-based ingredients.";
        else if (variant == "Healthy") r.description = "A healthy " + lower + " with light sweetness and nourishing flavors.";
        else if (variant == "Unhealthy") r.description = "An indulgent " + lower + " with rich sweetness and decadent texture.";
        else r.description = "A classic " + lower + " dessert with delightful flavors.";
        r.time = 20 + (idx % 30);
        r.difficulty = difficulties[(idx + 2) % difficulties.size()];
        r.sweetness = 4 + (idx % 6);
        r.vegan = (variant == "Vegan");
        r.calories = 190 + (idx % 7) * 35;
        if (variant == "Unhealthy") r.calories += 180;
        if (variant == "Healthy") r.calories -= 80;
        if (variant == "Vegan") r.calories -= 20;
        if (r.calories < 130) r.calories = 130;
        int ingrCount = 4;
        for (int j = 0; j < ingrCount; ++j) r.ingredients.push_back(ingredients[(idx + j + 2) % ingredients.size()]);
        add(r);
    }

    write_json(recipes, "recipes.json");
    std::cout << "Generated recipes.json with " << recipes.size() << " recipes\n";
    return 0;
}
