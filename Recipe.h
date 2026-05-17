#pragma once

#include "BaseEntity.h"
#include "Ingredient.h"
#include <string>
#include <vector>

class Recipe : public BaseEntity {
protected:
    std::string title;
    std::string description;
    int cookingTime;
    std::string difficulty;
    std::string category;
    double rating;
    int ratingCount;
    std::vector<Ingredient> ingredients;
    double calories;

public:
    Recipe();
    Recipe(int id, const std::string& title, const std::string& description, int cookingTime, const std::string& difficulty, const std::string& category, double calories = 0.0);
    virtual ~Recipe();
    std::string getTitle() const;
    std::string getDescription() const;
    int getCookingTime() const;
    std::string getDifficulty() const;
    std::string getCategory() const;
    double getRating() const;
    int getRatingCount() const;
    const std::vector<Ingredient>& getIngredients() const;
    double getCalories() const;
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setCookingTime(int cookingTime);
    void setDifficulty(const std::string& difficulty);
    void setCategory(const std::string& category);
    void setRatingData(double rating, int count);
    void addIngredient(const Ingredient& ingredient);
    void addRating(int value);
    void setCalories(double calories);
    virtual void displayInfo() const;
};