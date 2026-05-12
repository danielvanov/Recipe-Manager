#include "Recipe.h"
#include <iostream>

Recipe::Recipe() : BaseEntity(0), title(""), description(""), cookingTime(0), difficulty(""), category(""), rating(0.0), ratingCount(0) {}

Recipe::Recipe(int id, const std::string& title, const std::string& description, int cookingTime, const std::string& difficulty, const std::string& category)
    : BaseEntity(id), title(title), description(description), cookingTime(cookingTime), difficulty(difficulty), category(category), rating(0.0), ratingCount(0) {}

Recipe::~Recipe() {}

std::string Recipe::getTitle() const {
    return title;
}

std::string Recipe::getDescription() const {
    return description;
}

int Recipe::getCookingTime() const {
    return cookingTime;
}

std::string Recipe::getDifficulty() const {
    return difficulty;
}

std::string Recipe::getCategory() const {
    return category;
}

double Recipe::getRating() const {
    return rating;
}

int Recipe::getRatingCount() const {
    return ratingCount;
}

const std::vector<Ingredient>& Recipe::getIngredients() const {
    return ingredients;
}

void Recipe::setTitle(const std::string& title) {
    this->title = title;
}

void Recipe::setDescription(const std::string& description) {
    this->description = description;
}

void Recipe::setCookingTime(int cookingTime) {
    if (cookingTime >= 0) {
        this->cookingTime = cookingTime;
    }
}

void Recipe::setDifficulty(const std::string& difficulty) {
    this->difficulty = difficulty;
}

void Recipe::setCategory(const std::string& category) {
    this->category = category;
}

void Recipe::setRatingData(double rating, int count) {
    if (count >= 0 && rating >= 0.0) {
        this->rating = rating;
        this->ratingCount = count;
    }
}

void Recipe::addIngredient(const Ingredient& ingredient) {
    ingredients.push_back(ingredient);
}

void Recipe::addRating(int value) {
    if (value >= 1 && value <= 5) {
        rating = (rating * ratingCount + value) / (ratingCount + 1);
        ratingCount++;
    } else {
        std::cout << "Invalid rating. Please enter a value between 1 and 5." << std::endl;
    }
}

void Recipe::displayInfo() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Title: " << title << std::endl;
    std::cout << "Description: " << description << std::endl;
    std::cout << "Cooking Time: " << cookingTime << " minutes" << std::endl;
    std::cout << "Difficulty: " << difficulty << std::endl;
    std::cout << "Category: " << category << std::endl;
    std::cout << "Rating: " << rating << " (" << ratingCount << " ratings)" << std::endl;
    std::cout << "Ingredients:" << std::endl;
    for (const auto& ing : ingredients) {
        ing.displayInfo();
    }
    std::cout << std::endl;
}