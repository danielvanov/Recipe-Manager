CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic
APP := recipe-manager
APP_SRCS := main.cpp BaseEntity.cpp Ingredient.cpp Recipe.cpp DessertRecipe.cpp MainDishRecipe.cpp RecipeManager.cpp Menu.cpp FileManager.cpp
APP_OBJS := $(APP_SRCS:.cpp=.o)

GENERATOR := tools/generator
GENERATOR_SRC := tools/generator.cpp

.PHONY: all clean generator

all: $(APP)

$(APP): $(APP_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

generator: $(GENERATOR)

$(GENERATOR): $(GENERATOR_SRC)
	mkdir -p tools
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(APP) recipe-manager-test generator *.o tools/generator
