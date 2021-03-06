#pragma once
/*
    FILE:
       recipe.h
    DESCRIPTION:
       Header and operation definitions of standard cooking procedures.
       Can be used by any recipe with some simply porting.
       There are a few assumptions, such as the developer will be dehydrating only bread.
*/

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


/*  This can be defined as needed.  if additional metrics need to be added for future use such as (for exampe) radians, hogsheds, miner's inches or microfortnights, they can be added here */
/*  only byte indexes 0-7 are utilized, leaving indexes 8-31 (8-63 if compiling on g++ !!) reserved for future use */
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

/*  A single ingredient.  An ingredient is characterized by its name, metric of measurement (ie cups, grams, etc), and the magnitude of that measurement metric  */
typedef struct {
    char* name;
    MEASURE measurement;
    float   quantity;
}Ingredient;

/*  Our struct which will hold an ingredient list would more accuratley be defined as a "Singly-Linked List of Ingredients" */
/*  For ease, we will simply name it an IngredientList  */
struct IngredientList {
    struct IngredientList* next_ingredient;
    Ingredient* curr_ingredient;
}IngredientList;

/* Add an ingredient to a list of ingredients */
/* The ingredient is added in a FIFO-style operation */
/*  Thus, new ingredients will naturally be written to the end of our list, and not simply 'pushed' on top  */
/*  Afterall, this is an IngredientList we are using, not an IngredientStack or an IngredientHeap */
inline void add_ingredient_to_list(struct IngredientList* list, Ingredient* item) {
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
inline void heat_ingredient(Ingredient* ingredient, const int goal) {
    /*farenheit scale for {int goal}*/
    assert(ingredient->measurement == TEMPERATURE); //we cant heat an item without temperature
    if (ingredient->quantity >= goal)  //is it already too hot?  if so, then lets cool it down so that it doesnt burn any of the food
        while (ingredient->quantity >= goal)
            --ingredient->quantity;
    else   //heat up the ingredient!
        while (ingredient->quantity < (float)goal)
            ++ingredient->quantity;
}

/* Take an item, heat it slowly, and wait for it to dehydrate! */
inline void dehydrate_item(Ingredient* bread) {
    MEASURE tmp_m = bread->measurement;
    float tmp_q = bread->quantity;
    bread->measurement = TEMPERATURE;
    bread->quantity = 0x44; /* assumption bread should start at room temp, Farenheit scale.  actual starting temp notimportant though. */
    heat_ingredient(bread, 200);
    /*  5 minutes per item to dehydrate... assuming slices of bread, this should be good, give or take a few minutes  */
    /*  If you are trying to dehydrate something besides slices of bread, you should adjust the following line accordingly  */
    do_sleep(tmp_q * 5 * 60);
    bread->measurement = tmp_m;
    bread->quantity = tmp_q;
}

/* for every slice of bread, turn it into 10000 breadcrumbs*/
/* Technically, this will turn slices of anything into 10000 bits, it doesnt need to strictly be bread.  */
/* It may be a bad asumption, but as a design choice im assuming you will only make breadcrumbs with this function */
inline void make_breadcrubs_from_toasted_bread(Ingredient* bread) {
    assert(bread->measurement == SLICES);  //we want to start with slices of bread!
    float num_crumbs = 10000 * bread->quantity;
    bread->measurement = PIECES;
    bread->quantity = num_crumbs;
}

/* Using an IngredientList, mix all ingredients form that list, and produce a new Ingredient.  The single new ingredient, the result of mixing all ingredients in the list, will be returned */
inline Ingredient mix_ingredients(struct IngredientList* list) {
    Ingredient new_ingredient = { 0 };
    struct IngredientList* tmp = list;
    struct IngredientList* prev;
    size_t ingredients_count = 0;
    size_t len_components = 0;
    while (tmp != nullptr) {  //count how many ingredients we will be mixing
        ++ingredients_count;
        len_components += strlen(tmp->curr_ingredient->name);
        tmp = tmp->next_ingredient;
    }
    if (ingredients_count) {  //if its only a single ingredient, then nothing to mix!!
        size_t len_new_name = len_components + (strlen(" + ") * ingredients_count) + 1;  //our new ingredient will have naming scheme "[ingredient a] + [ingredient b] + ..."
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
            if (ingredients_mixed < ingredients_count)
                strcat(new_ingredient.name, " + ");
            free(tmp->curr_ingredient->name);
            tmp = tmp->next_ingredient;  //advance to next ingredient on list

        }
    }
    return new_ingredient;
}
#endif
