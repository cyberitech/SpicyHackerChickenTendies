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

/* Add an ingredient to a list of ingredients */
void pushback_ingredient(struct IngredientList* list, Ingredient* item) {
    if ((list->curr_ingredient || list->next_ingredient) == nullptr) //decide if our list is empty.
        list->curr_ingredient = item;
    else {
        struct IngredientList* curr_item = list;
        struct IngredientList* prev_item = list;
        do   //iterate thru ingredients until we get an empty item
        {
            prev_item = curr_item;
            curr_item = curr_item->next_ingredient;

        } while (curr_item != nullptr);

        /*we have reached the end of our ingredient list.  add one to it*/
        curr_item = (struct IngredientList*)malloc(sizeof(IngredientList));
        assert(curr_item != 0);
        assert(MEMZERO(curr_item, sizeof(IngredientList)) != nullptr);
        curr_item->curr_ingredient = item;
        curr_item->next_ingredient = nullptr;
        prev_item->next_ingredient = curr_item;
    }
    return;
}


/* Heat any ingredient with the measure of TEMPERATURE to quantity of GOAL... undefined behavior if the item is already hotter! it will melt turn into plasma and eventually freeze as the float overflows over! */
void heat_ingredient(Ingredient* ingredient, int goal) {
    /*farenheit scale for {int goal}*/
    assert(ingredient->measurement == TEMPERATURE); //we cant heat an item without temperature
    while (ingredient->quantity > (float)goal)
        ++ingredient->quantity;
}

/* Take an item, heat it slowly, and wait for it to dehydrate! */
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
    do_sleep(tmp_q * 5 * 60); //5 minutes per item to dehydrate... assuming slices of bread, this should be good, give    or take a few minutes
    bread->measurement = tmp_m;
    bread->quantity = tmp_q;
}
/* for eveyr slice of bread, turn it into 10000 breadcrumbs*/
void make_breadcrubs_from_toasted_bread(Ingredient* bread) {
    assert(bread->measurement == SLICES);  //we want to start with slices of bread!
    float num_crumbs = 10000 * bread->quantity;
    bread->measurement = PIECES;
    bread->quantity = num_crumbs;
}

/* Using an IngredientList, mix all ingredients form that list, and produce a new Ingredient.  The single new ingredient, the result of mixing all ingredients in the list, will be returned*/
Ingredient mix_ingredients(struct IngredientList* list) {
    Ingredient new_ingredient = { 0 };
    struct IngredientList* tmp = list;

    size_t ingredients_count = 0;
    size_t len_components = 0;
    while (tmp != nullptr) {  //count how many ingredients we will be mixing
        ++ingredients_count;
        len_components += strlen(tmp->curr_ingredient->name);
        tmp = tmp->next_ingredient;
    }
    if (ingredients_count)  {  //if its only a single ingredient, then nothing to mix!!
        size_t len_new_name = len_components + (strlen(" + ") * ingredients_count) +1;  //our new ingredient will have naming scheme "[ingredient a] + [ingredient b] + ..."
        new_ingredient.name = (char*)malloc(sizeof(char) * len_new_name);// reserve mem for the new name
        MEMZERO(new_ingredient.name, len_new_name);
        new_ingredient.measurement = PIECES;  // the new ingredient produced by a mixture of other ingredients will be described as having n-pieces, where n is the number of ingredients that were mixed to create it
        new_ingredient.quantity = 0;

        tmp = list;
        size_t ingredients_mixed = 0;
        while (tmp != nullptr) {
            
            tmp->curr_ingredient->quantity = 0;  //after weve mixed in the ingredient, we have no more left!! its now part of a whole new mixture of ingredients!!
            ++new_ingredient.quantity;  // our new mixture has 1 more ingredient in its mixture
            strcat(new_ingredient.name, tmp->curr_ingredient->name);  //copy the new ingredient name to the new ingredient mixture
            ++ingredients_mixed;
            if (ingredients_mixed < ingredients_count )
                strcat(new_ingredient.name, " + ");
            tmp = tmp->next_ingredient;  //advance to next ingredient on list
        }
    }
    return new_ingredient;
}
#endif
