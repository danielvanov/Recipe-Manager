from pathlib import Path

base = Path('recipes.txt')
existing = [
    'RECIPE',
    'TYPE=General',
    'ID=1',
    'TITLE=Omelette',
    'DESCRIPTION=Quick egg omelette with herbs',
    'TIME=10',
    'DIFFICULTY=Easy',
    'CATEGORY=Breakfast',
    'CALORIES=250',
    'RATING=5',
    'RATING_COUNT=1',
    'INGREDIENTS=3',
    'Eggs|3',
    'Salt|1 pinch',
    'Butter|1 tbsp',
    'END',
    'RECIPE',
    'TYPE=Dessert',
    'ID=2',
    'TITLE=Chocolate Cake',
    'DESCRIPTION=Simple cake with chocolate glaze',
    'TIME=60',
    'DIFFICULTY=Medium',
    'CATEGORY=Dessert',
    'CALORIES=450',
    'SWEETNESS=8',
    'RATING=0',
    'RATING_COUNT=0',
    'INGREDIENTS=4',
    'Flour|200g',
    'Cocoa Powder|50g',
    'Sugar|150g',
    'Eggs|3',
    'END',
    'RECIPE',
    'TYPE=MainDish',
    'ID=3',
    'TITLE=Vegetable Pasta',
    'DESCRIPTION=Pasta with fresh vegetables',
    'TIME=30',
    'DIFFICULTY=Easy',
    'CATEGORY=Main Dish',
    'CALORIES=350',
    'VEGETARIAN=1',
    'RATING=0',
    'RATING_COUNT=0',
    'INGREDIENTS=4',
    'Pasta|300g',
    'Tomato Sauce|200ml',
    'Zucchini|1',
    'Olive Oil|2 tbsp',
    'END',
    'RECIPE',
    'TYPE=MainDish',
    'ID=4',
    'TITLE=Steak',
    'DESCRIPTION=Grilled steak with garlic butter',
    'TIME=25',
    'DIFFICULTY=Medium',
    'CATEGORY=Main Dish',
    'CALORIES=650',
    'VEGETARIAN=0',
    'RATING=0',
    'RATING_COUNT=0',
    'INGREDIENTS=3',
    'Beef Steak|250g',
    'Garlic|2 cloves',
    'Butter|1 tbsp',
    'END',
]

starter_categories = ['Starter', 'Appetizer', 'Salad', 'Soup', 'Snack', 'Finger Food']
main_titles = ['Pasta', 'Curry', 'Stew', 'Bowl', 'Stir Fry', 'Casserole', 'Burger', 'Tacos', 'Risotto', 'Paella']
dessert_titles = ['Berry Tart', 'Fruit Parfait', 'Chocolate Mousse', 'Cookie Plate', 'Ice Cream Scoop', 'Panna Cotta', 'Brownie', 'Cupcake', 'Creme Brulee', 'Rice Pudding']
difficulties = ['Easy', 'Medium', 'Hard']
ingredients = [
    ('Tomatoes','100g'), ('Lettuce','100g'), ('Olive Oil','1 tbsp'), ('Garlic','2 cloves'), ('Onion','1'),
    ('Salt','1 pinch'), ('Pepper','1 pinch'), ('Flour','100g'), ('Sugar','50g'), ('Milk','100ml'),
    ('Butter','20g'), ('Eggs','2'), ('Cheese','50g'), ('Rice','150g'), ('Beans','100g'), ('Potato','150g'),
    ('Carrot','100g'), ('Celery','50g'), ('Mushrooms','100g'), ('Cucumber','100g'), ('Spinach','100g')
]

lines = list(existing)
next_id = 5

def append_recipe(block):
    lines.extend(block)

for idx in range(375):
    rid = next_id
    next_id += 1
    base_name = starter_categories[idx % len(starter_categories)]
    variant = 'Vegan' if idx % 5 == 0 else 'Unhealthy' if idx % 7 == 0 else 'Healthy' if idx % 3 == 0 else ''
    title = f'{variant + " " if variant else ""}{base_name} {rid}'.strip()
    category = f'{variant + " " if variant else ""}{base_name}'.strip()
    desc = f'Tasty starter recipe number {rid}.'.strip()
    calories = 180 + (idx % 5) * 30
    if variant == 'Unhealthy':
        calories += 220
    elif variant == 'Healthy':
        calories -= 20
    elif variant == 'Vegan':
        calories -= 10
    calories = max(120, calories)
    difficulty = difficulties[idx % len(difficulties)]
    ingr = [ingredients[(idx + j) % len(ingredients)] for j in range(4)]
    block = [
        'RECIPE',
        'TYPE=General',
        f'ID={rid}',
        f'TITLE={title}',
        f'DESCRIPTION={desc}',
        f'TIME={10 + (idx % 35)}',
        f'DIFFICULTY={difficulty}',
        f'CATEGORY={category}',
        f'CALORIES={calories}',
        'RATING=0',
        'RATING_COUNT=0',
        f'INGREDIENTS={len(ingr)}',
    ]
    block += [f'{name}|{qty}' for name, qty in ingr]
    block.append('END')
    append_recipe(block)

for idx in range(325):
    rid = next_id
    next_id += 1
    base_name = main_titles[idx % len(main_titles)]
    variant = 'Vegan' if idx % 4 == 0 else 'Unhealthy' if idx % 6 == 0 else 'Healthy' if idx % 3 == 0 else ''
    title = f'{variant + " " if variant else ""}{base_name} {rid}'.strip()
    category = 'Main Dish'
    desc = f'Satisfying main dish number {rid}.'.strip()
    vegetarian = variant == 'Vegan' or idx % 2 == 0
    calories = 380 + (idx % 6) * 40
    if variant == 'Unhealthy':
        calories += 180
    if variant == 'Healthy':
        calories -= 80
    calories = max(280, calories)
    difficulty = difficulties[(idx + 1) % len(difficulties)]
    ingr = [ingredients[(idx + j + 1) % len(ingredients)] for j in range(5)]
    block = [
        'RECIPE',
        'TYPE=MainDish',
        f'ID={rid}',
        f'TITLE={title}',
        f'DESCRIPTION={desc}',
        f'TIME={25 + (idx % 40)}',
        f'DIFFICULTY={difficulty}',
        f'CATEGORY={category}',
        f'CALORIES={calories}',
        f'VEGETARIAN={1 if vegetarian else 0}',
        'RATING=0',
        'RATING_COUNT=0',
        f'INGREDIENTS={len(ingr)}',
    ]
    block += [f'{name}|{qty}' for name, qty in ingr]
    block.append('END')
    append_recipe(block)

for idx in range(300):
    rid = next_id
    next_id += 1
    base_name = dessert_titles[idx % len(dessert_titles)]
    if idx % 5 == 0:
        variant = 'Vegan'
    elif idx % 7 == 0:
        variant = 'Healthy'
    elif idx % 6 == 0:
        variant = 'Unhealthy'
    else:
        variant = ''
    title = f'{variant + " " if variant else ""}{base_name} {rid}'.strip()
    category = 'Dessert' if variant == '' else f'{variant} Dessert'
    desc = f'Sweet dessert number {rid}.'.strip()
    calories = 190 + (idx % 7) * 35
    if variant == 'Unhealthy':
        calories += 180
    if variant == 'Healthy':
        calories -= 80
    if variant == 'Vegan':
        calories -= 20
    calories = max(130, calories)
    difficulty = difficulties[(idx + 2) % len(difficulties)]
    sweetness = 4 + (idx % 6)
    ingr = [ingredients[(idx + j + 2) % len(ingredients)] for j in range(4)]
    block = [
        'RECIPE',
        'TYPE=Dessert',
        f'ID={rid}',
        f'TITLE={title}',
        f'DESCRIPTION={desc}',
        f'TIME={20 + (idx % 30)}',
        f'DIFFICULTY={difficulty}',
        f'CATEGORY={category}',
        f'CALORIES={calories}',
        f'SWEETNESS={sweetness}',
        'RATING=0',
        'RATING_COUNT=0',
        f'INGREDIENTS={len(ingr)}',
    ]
    block += [f'{name}|{qty}' for name, qty in ingr]
    block.append('END')
    append_recipe(block)

base.write_text('\n'.join(lines) + '\n')
print(f'Generated recipes.txt with {len([line for line in lines if line == "RECIPE"])} recipes')
