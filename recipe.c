
#include "recipe.h"

int main(void)
{
    struct IngredientList dry_ingredients;
    struct IngredientList wet_ingredients;
    MEMZERO(&dry_ingredients, sizeof(IngredientList));
    MEMZERO(&wet_ingredients, sizeof(IngredientList));
    Ingredient i_one, i_two, i_three, i_four, i_five, i_six, i_seven;

    i_one.name = (char*)malloc(sizeof(char) * strlen("JEWISH RYE BREAD\x00"));
    strcpy(i_one.name ,"JEWISH RYE BREAD\x00");
    i_one.measurement = SLICES;
    i_one.quantity = 0x3;

    i_two.name = (char*)malloc(sizeof(char) * strlen("WHOLE WHEAT FLOUR\x00"));
    strcpy(i_two.name , "WHOLE WHEAT FLOUR\x00");
    i_two.measurement = CUPS;
    i_two.quantity = 0.25;

    i_three.name = (char*)malloc(sizeof(char) * strlen("EGG\x00"));
    strcpy(i_three.name , "EGG\x00");
    i_three.measurement = PIECES;
    i_three.quantity = 0x2;

    i_four.name = (char*)malloc(sizeof(char) * strlen("ALMOND MILK, UNSWEETENED\x00"));
    strcpy(i_four.name , "ALMOND MILK, UNSWEETENED\x00");
    i_four.measurement = MILLILITRES;
    i_four.quantity = 0xc8;

    i_five.name = (char*)malloc(sizeof(char)*strlen("SCORPION PEPPER DEATH SALT\x00"));
    strcpy(i_five.name , "SCORPION PEPPER DEATH SALT\x00");
    i_five.measurement = GRAMS;
    i_five.quantity = 0x3;

    i_six.name = (char*)malloc(sizeof(char) * strlen("CHICKEN THIGHS, BONELESS, SKINLESS\x00"));
    strcpy(i_six.name , "CHICKEN THIGHS, BONELESS, SKINLESS\x00");
    i_six.measurement = PIECES;
    i_six.quantity = 0x4;

    i_seven.name = (char*)malloc(sizeof(char) * strlen("DEEP FRYING OIL\x00"));
    strcpy(i_seven.name , "DEEP FRYING OIL\x00");
    i_seven.measurement = TEMPERATURE;
    i_seven.quantity = 0x44; /* Farenheit scale */

   
    dehydrate_item(&i_one);
    make_breadcrubs_from_toasted_bread(&i_one);

    pushback_ingredient(&dry_ingredients, &i_one);
    pushback_ingredient(&dry_ingredients, &i_two);
    pushback_ingredient(&wet_ingredients, &i_three);
    pushback_ingredient(&wet_ingredients, &i_four);
    pushback_ingredient(&dry_ingredients, &i_five);

    Ingredient batter;
    Ingredient breading;
    mix_ingredients(&dry_ingredients, &breading); // the dry_ingredients are mixed to produce the breading
    mix_ingredients(&wet_ingredients, &batter); // the wet ingredients are mixe to produce the batter

    struct IngredientList chicken_to_be_breaded;
    MEMZERO(&chicken_to_be_breaded, sizeof(IngredientList));
    pushback_ingredient(&chicken_to_be_breaded, &i_six);
    pushback_ingredient(&chicken_to_be_breaded, &batter);
    pushback_ingredient(&chicken_to_be_breaded, &breading);

    Ingredient battered_chicken;
    mix_ingredients(&chicken_to_be_breaded, &battered_chicken); //the chicken_to_be_breaded (contining the batter and breading) is mixed to produce battered_chicken

    heat_ingredient(&i_seven, 0x15e);
    struct IngredientList battered_chicken_to_be_fried;
    MEMZERO(&battered_chicken_to_be_fried, sizeof(IngredientList));
    pushback_ingredient(&battered_chicken_to_be_fried, &battered_chicken);
    pushback_ingredient(&battered_chicken_to_be_fried, &i_seven);

    Ingredient hacker_chicken_tendies;
    mix_ingredients(&battered_chicken_to_be_fried, &hacker_chicken_tendies); // the battered_chicken_to_be_fried, which contains battered_chicken and the hot oil, are mixed to product the chicken tendies!
    printf("\n*****The Spicy Hacker Chicken Tendies are cooking! Please be patient!!*****\n\n  .. (0_0)     \n\n");
    do_sleep(0xa * 0x3c);  //10 minutes cook time should work!
    printf("\n*****The Spicy Hacker Chicken Tendies are ready!*****\n\n    ! \(^_^)/ !    \n\n");
    return 0;
}
