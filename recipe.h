#ifndef RECIPE_HACKER_TENDIES_H
#define  RECIPE_HACKER_TENDIES_H
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#ifdef __unix__
#include <unistd.h>  // If compiling on wndows, use windows.h instead... this supplies sleep()
#define do_sleep sleep
#elif defined(_WIN32) || defined(_WIN32) || defined(WIN64)
#include <Windows.h>
#define do_sleep(x) (Sleep(x*1000));
#endif
#include <string.h>
#define nullptr NULL
#define MEMZERO(obj,sz) (memset(obj,0,sz))
typedef enum {
    GRAMS = 0b1,
    CUPS = 0b10,
    MILLILITRES = 0b100,
    DROPS = 0b1000,
    SLICES = 0b10000,
    LOAVES = 0b100000,
    PIECES = 0b1000000,
    TEMPERATURE = 0b10000000
}MEASURE;
typedef struct {
    char* name;
    MEASURE measurement;
    float   quantity;
}Ingredient;
struct IngredientList {
    struct IngredientList* next_ingredient;
    Ingredient* curr_ingredient;
}IngredientList;
void pushback_ingredient(struct IngredientList* list, Ingredient* item) {
    if ((list->curr_ingredient && list->next_ingredient) == nullptr) //decide if our list is empty.
        list->curr_ingredient = item;
    else {
        struct IngredientList* curr_item = list->next_ingredient;
        while (curr_item != nullptr)  //iterate thru ingredients until we get an empty item
            curr_item = list->next_ingredient;
        /*we have reached the end of our ingredient list.  add one to it*/
        curr_item = (struct IngredientList*)malloc(sizeof(Ingredient));
        assert(curr_item != 0);
        assert(MEMZERO(curr_item, sizeof(IngredientList)) != nullptr);
        curr_item->curr_ingredient = item;
    }
    return;
}
struct IngredientList* next_ingredient(struct IngredientList* list) {
    assert(list->curr_ingredient != nullptr);  // sanity check. we should have an ingredient.
    if (list->next_ingredient != nullptr) {  //is there more than 1 ingredient in the list?
        struct IngredientList* next_ingredient = list->next_ingredient;
        assert(next_ingredient->curr_ingredient != nullptr);  //sanity check again.  the next ingredient should not be empty.
            return next_ingredient;
    }
    else
        return nullptr;
}
void heat_ingredient(Ingredient* ingredient, int goal) {
    /*farenheit scale for {int goal}*/
    assert(ingredient->measurement == TEMPERATURE); //we cant heat an item without temperature
    while (ingredient->quantity > (float)goal)
        ++ingredient->quantity;
}
void dehydrate_item(Ingredient* bread) {
    /*
        Bit of a misnomer here, we will not be toasting the bread.
            we will actually be dehydrating it.  WE want it dry and crumbly.
    */
    MEASURE tmp_m = bread->measurement;
    float tmp_q = bread->quantity;
    bread->measurement = TEMPERATURE;
    bread->quantity = 0x44; /* assumption bread should start at room temp, Farenheit scale.  actual starting temp notimportant though. */
    heat_ingredient(bread, 200);
    printf("\n*****Please wait while the bread is dehydrated*****\n\n   (0_0) ..    \n\n");
    do_sleep(tmp_q * 5 * 60); //5 minutes per item to dehydrate... assuming slices of bread, this should be good, give    or take a few minutes
        bread->measurement = tmp_m;
    bread->quantity = tmp_q;
}
void make_breadcrubs_from_toasted_bread(Ingredient* bread) {
    assert(bread->measurement == SLICES);  //we want to start with slices of bread!
    float num_crumbs = 10000 * bread->quantity;
    bread->measurement = PIECES;
    bread->quantity = num_crumbs;
}
void mix_ingredients(struct IngredientList* list, Ingredient* new_ingredient) {
    struct IngredientList* tmp = next_ingredient(list);
    MEMZERO(new_ingredient, sizeof(Ingredient));
    size_t count = 0;
    while (tmp != nullptr) {  //count how many ingredients we will bemixing
        ++count;
        tmp = next_ingredient(tmp);
    }
    new_ingredient->name = (char*)malloc(sizeof(char*) * count);// an array of ingredient names
    new_ingredient->measurement = PIECES;
    new_ingredient->quantity = 0;
    count = 0;
    while (tmp != nullptr) {
        char* ingredient_name = (char*)malloc(strlen(tmp->curr_ingredient->name));
        memcpy(ingredient_name, tmp->curr_ingredient->name, strlen(tmp->curr_ingredient->name)); //get ingredient name we are mixing
            new_ingredient->name = ingredient_name;
        tmp->curr_ingredient->quantity = 0;
        ++new_ingredient->quantity;
        tmp = tmp->next_ingredient;
    }
    return;
}
#endif