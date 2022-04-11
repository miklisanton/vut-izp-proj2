//   Setcal
//   IZP 2021
//   2. project

//    Authors >>   Novosád Denis xnovos14 
//                 Pekárek Jakub xpekar19
//                 Zelinka Tadeáš xzelin28 
//                 Anton Miklis xmikli05

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_ELEMENT_SIZE 31
#define MAX_OPER_ELEMENT_SIZE 30

typedef struct{
    int *elements;
    unsigned int elements_count;
}set_t;

typedef struct{
    char **elements;
    unsigned int elements_amount;
}universe_t;

typedef struct{
    int first;
    int second;
}pair_t;  

typedef struct {
    pair_t *pairs;
    int size;
    int capacity;
}relation_t;

typedef struct {
    pair_t *relations;
    int size;
    int capacity;
}relations_array_t;

typedef struct {
    int reading_universe_flag;    // 0 - universe is not created, 1 - reading universe now, 2 - universe was initialized
    int reading_element_flag;     // 0 - not reading element currently 1 - started reading element, 2 - element was read
    int reading_set_flag;         // 0 - not reading set currently, 1 - reading set now
    int reading_relation_flag;    // 0 - not reading relation currently, 1 - reading relation now
    int reading_pair_flag;        // 0 - not reading pair currently, 1 - reading pair now
    int reading_oper_flag;        // 0 - not reading operator currently, 1 - reading operator now
    int section_flag;             // 0  - sets/relations section 1 - operations section
}cfg_t;


void set_constructor(set_t *set){
    if (set == NULL){
        exit(1);
    }
    set->elements = malloc(1 * sizeof(int ));
    if (set->elements == NULL){
        printf("Out of memory!");
        exit(1);
    }
    set->elements_count = 0;
}

void set_destructor(set_t *set){
    if (set == NULL || set->elements == NULL){
        return;
    }
    free(set->elements);
}

void set_add_element(set_t *set, int new_element){
    int *tmp = realloc(set->elements,(set->elements_count + 1) * sizeof(int));
    if (tmp == NULL){
        printf("Out of memory!");
        exit(1);
    }
    set->elements = tmp;
    set->elements[set->elements_count] = new_element;
    set->elements_count++;
}

void set_print(set_t set, universe_t universe){
    printf("\nS");
    for (unsigned int i = 0; i < set.elements_count; ++i) {
        printf(" %s", universe.elements[set.elements[i]]);
    }
}

void sets_array_dtor(set_t *sets_arr, int set_index){
    for (int i = 0; i < set_index; ++i) {
        set_destructor(&sets_arr[i]);
    }
    free(sets_arr);
}
bool is_element_restricted(char *string){
    char *ret;
    char *restricted_elements[30];
    restricted_elements[0] = "empty";
    restricted_elements[1] = "card";
    restricted_elements[2] = "complement";
    restricted_elements[3] = "union";
    restricted_elements[4] = "intersect";
    restricted_elements[5] = "minus";
    restricted_elements[6] = "subseteq";
    restricted_elements[7] = "subset";
    restricted_elements[8] = "equals";
    restricted_elements[9] = "reflexive";
    restricted_elements[10] = "symmetric";
    restricted_elements[11] = "antisymmetric";
    restricted_elements[12] = "transitive";
    restricted_elements[13] = "function";
    restricted_elements[14] = "domain";
    restricted_elements[15] = "codomain";
    restricted_elements[16] = "injective";
    restricted_elements[17] = "surjective";
    restricted_elements[18] = "bijective";
    restricted_elements[19] = "true";
    restricted_elements[20] = "false";

    for (int i = 0; i < 21; ++i) {
        ret = strstr(string, restricted_elements[i]);
        if (ret){
            return 1;
        }
    }
    return 0;
}

void universe_constructor(universe_t *universe){
    if (universe == NULL){
        exit(1);
    }
    universe->elements = (char **)malloc(sizeof(char*));
    if (universe->elements == NULL){
        exit(1);
    }
    universe->elements_amount = 0;
}

void universe_destructor(universe_t *universe){
    for (unsigned int i = 0; i < universe->elements_amount; ++i) {
        free(universe->elements[i]);
    }
    free(universe->elements);
}

//increases universe size and adds element to the end of universe
void add_element_to_universe(universe_t *universe, char buffer[], int length){
    char **temp = (char **)realloc(universe->elements, (universe->elements_amount + 1) * sizeof(char*));
    if (temp == NULL){
        universe_destructor(universe);
        printf("Out of memory");
        exit(1);
    }
    universe->elements = temp;
    universe->elements[universe->elements_amount] = (char*)malloc((length + 1) * sizeof(char));
    if (universe->elements[universe->elements_amount] == NULL){
        universe_destructor(universe);
        printf("Out of memory");
        exit(1);
    }
    for (int i = 0; i < length; ++i) {
        universe->elements[universe->elements_amount][i] = buffer[i];
    }
    universe->elements_amount++;
}

void print_universe(universe_t universe){
    printf("U");
    for (unsigned int i = 0; i < universe.elements_amount; ++i) {
        printf(" %s", universe.elements[i]);
    }
}

void relation_constructor(relation_t *relation){
    relation->capacity = 0;
    relation->size = 0;
    relation->pairs = NULL;
}

void relation_destructor(relation_t *relation){
    if (relation->capacity){
        free(relation->pairs);
    }
    relation->pairs = NULL;
    relation->capacity = 0;
    relation->size = 0;
}

//increases relation size by 2 * current capacity
void relation_resize(relation_t *relation, int new_capacity){
    pair_t *resized = realloc(relation->pairs, new_capacity * sizeof(pair_t));
    if (resized == NULL){
        printf("Out of memory");
        exit(1);
    }
    relation->pairs = resized;
    relation->capacity = new_capacity;
}

//adds pair to the end of relation
void relation_append(relation_t *relation, pair_t item){
    if (relation->size == relation->capacity){
        relation_resize(relation, relation->capacity ? relation->capacity * 2 : 1);
    }
    if (relation->capacity > relation->size){
        relation->pairs[relation->size++] = item;
    }
}

void relation_print(relation_t relation, universe_t univ){
    printf("\nR");
    for (int i = 0; i < relation.size; ++i) {
        printf(" (%s %s)", univ.elements[relation.pairs[i].first], univ.elements[relation.pairs[i].second]);
    }
}

void relations_array_dtor(relation_t *relations_arr, int relation_index){
    for (int i = 0; i < relation_index; ++i) {
        relation_destructor(&relations_arr[i]);
    }
    free(relations_arr);
}

//returns index of element in universe, if element is not in universe returns -1
int index_in_universe(universe_t universe, char element[], int length){
    for (int unsigned i = 0; i < universe.elements_amount; ++i) {
        if(strncmp(universe.elements[i],element, length * sizeof(char)) == 0){
            return i;
        }
    }
    return -1;
}

//parses string and creates pair
pair_t get_pair_from_string(char *string, int length, universe_t universe){
    pair_t pair;
    pair.first = -1;
    pair.second = -1;
    char first[MAX_ELEMENT_SIZE];
    char second[MAX_ELEMENT_SIZE];
    int first_flag = 1;
    int second_flag = 0;
    int j = 0;

    for (int i = 0; i < length; ++i) {
        if (first_flag == 1){
            first[i] = string[i];
        }
        if (second_flag == 1){
            second[j++] = string[i];
        }
        if (first_flag == 1 && string[i] == ' '){
            first[i] = '\0';
            pair.first = index_in_universe(universe, first, i);
            first_flag = 0;
            second_flag = 1;
        }
        if (second_flag == 1 && string[i] == '\0'){
            pair.second = index_in_universe(universe, second, j);
        }
    }
    return pair;
}

//checks if element is in set
bool is_element_in_set(int element, set_t set){
    for (unsigned int i = 0; i < set.elements_count; ++i) {
        if (set.elements[i] == element){
            return 1;
        }
    }
    return 0;
}

// prints first_set minus second_set
void set_minus(set_t first_set, set_t second_set, universe_t universe)
{
    printf("S");
    for (unsigned int i = 0; i < second_set.elements_count; ++i){
        if (!is_element_in_set(second_set.elements[i], first_set)){
            printf(" %s", universe.elements[second_set.elements[i]]);
        }
    }

}
//calls destructors and closes files
void call_dtors(FILE *fp, universe_t *univ, set_t *sets, int sets_amount, relation_t *relatons, int relations_amount){
    fclose(fp);
    universe_destructor(univ);
    sets_array_dtor(sets, sets_amount);
    relations_array_dtor(relatons, relations_amount);
}

//prints union of first_set and second_set sets
void set_union(set_t first_set, set_t second_set, universe_t universe)
{
    for (unsigned i = 0; i < first_set.elements_count; i++){
        printf(" %s", universe.elements[first_set.elements[i]]);
    }
    //printf("%d$$$\n", union_arr_lenght);
    for (unsigned i = 0; i < second_set.elements_count; i++){
        //printf("\n%d ", second_set.elements[i]);
        if (!is_element_in_set(second_set.elements[i], first_set)){
            printf(" %s", universe.elements[second_set.elements[i]]);
        }
    }
}
//prints intersect of two sets
void set_intersect(set_t first_set, set_t second_set, universe_t universe)
{
    for (unsigned i = 0; i < first_set.elements_count; i++){
         for (unsigned j = 0; j < second_set.elements_count; j++){
            if(first_set.elements[i] == second_set.elements[j])
            printf(" %s", universe.elements[first_set.elements[i]]);
        }  
    }
}

// subseteq, subset and equals functions in one
// prints true if first_set is a subset of second_set, otherwise returns false
void set_subset(set_t first_set, set_t second_set, int OP)
{
    bool fFlag = 0;
    // returns false since subset can't have more elements than superset
    if(first_set.elements_count > second_set.elements_count && OP == 6){
        printf("false");
    }
    // returns false since proper subset can't have equal or more elements than superset
    else if((first_set.elements_count >= second_set.elements_count) && OP == 7){
        printf("false");
    }
    // returns false if sets don't have equal number of elements
    else if((first_set.elements_count != second_set.elements_count) && OP == 8){
        printf("false");
    }
    else{
        // iterates through the subset
        for(unsigned i = 0; i < first_set.elements_count; i++){
            fFlag = 0;
            // iterates through the superset
            for (unsigned j = 0; j < second_set.elements_count; j++){
                // if an element from subset is found in superset fFlag is set to true
                if(first_set.elements[i] == second_set.elements[j]){
                    fFlag = 1;
                }
            }
            // if no element from subset was found in superset returns false and breaks the cycle
            if(fFlag == 0){
                printf("false");
                break;
            }
        }
        if(fFlag != 0){
            printf("true");
        }
    }
}
//prints true if relation is reflexive/ false if not
void rel_reflexive(relation_t rel, universe_t uni)
{
    unsigned count = 0;
    for (int i = 0; i < rel.size; i++){
        if (rel.pairs[i].first != rel.pairs[i].second) continue;
        count++;
    }
    //printf("%d %d", count, uni.elements_amount);
    if (count == uni.elements_amount)
        printf("true");
    else 
        printf("false");
}

void rel_symmetric(relation_t rel)
{
    for (int i = 0; i < rel.size; i++){
        bool found = false;
        for (int j = 0; j < rel.size; j++){
            if (rel.pairs[j].second == rel.pairs[i].first && rel.pairs[j].first == rel.pairs[i].second){
                found = true;
                break;
            }
        }
        if (!found) {
            printf("false");
            return;
        }
    }
    printf("true");
}

void rel_antisymmetric(relation_t rel)
{
    for(int i = 0; i < rel.size; i++){
        // if both elements are the same, skips pair
        if(rel.pairs[i].first == rel.pairs[i].second){
            continue;
        }
        bool found = true;

        // checks for symmetric pairs, sets 'found' flag to false
        for(int j = 0; j < rel.size; j += 2){
            if(rel.pairs[i].first == rel.pairs[j].second && rel.pairs[i].second == rel.pairs[j].first){
                found = false;
                break;
            }
        }
        
        if(!found){
            printf("false");
            return;
        }
    }
    printf("true");
}

void rel_transitive(relation_t rel)
{
    for(int i = 0; i < rel.size; i++){
        // if both elements are the same, skips pair
        if(rel.pairs[i].first == rel.pairs[i].second){
            continue;
        }
        bool found = false;

        // checks for symmetric pairs
        for(int j = 0; j < rel.size; j += 2){
            if(rel.pairs[i].second == rel.pairs[j].first){
                // tries to find transitive closure, if found relation is transitive
                for(int k = 0; k < rel.size; k += 2){
                    if(rel.pairs[i].first == rel.pairs[k].first && rel.pairs[j].second == rel.pairs[k].second){
                        found = true;
                        break;
                    }
                }
                if(!found){
                    printf("false");
                    return;
                }
            }
        }
    }
    printf("true");
}

void rel_function(relation_t rel)
{
    for(int i = 0; i < rel.size; i++){
        bool found = true;
        for (int j = 0; j < rel.size; j++){
            if ((rel.pairs[i].first == rel.pairs[j].first) && (rel.pairs[i].second != rel.pairs[j].second)){
                found = false;
                break;
            }
        }
        if(!found){
            printf("false");
            return;
        }
    }
    printf("true");
}

void rel_domain(relation_t rel, universe_t universe)
{
    printf("S");
    for(int i = 0; i < rel.size; i++){
        bool flag = false;
        // if theres identical element in another pair, flag is set to true and thus skipped
        for(int j = i + 1; j < rel.size; j++){
            if(rel.pairs[i].first == rel.pairs[j].first){
                flag = true;
                break;
            }
        }
        // if theres no identical element found, element is printed
        if(!flag){
            printf(" %s", universe.elements[rel.pairs[i].first]);
        }
    }
}

void rel_codomain(relation_t rel, universe_t universe)
{
    printf("S");
    for(int i = 0; i < rel.size; i++){
        bool flag = false;

        // if theres identical element in another pair, flag is set to true and thus skipped
        for(int j = i + 1; j < rel.size; j++){
            if(rel.pairs[i].second == rel.pairs[j].second){
                flag = true;
                break;
            }
        }
        // if theres no identical element found, element is printed
        if(!flag){
            printf(" %s", universe.elements[rel.pairs[i].second]);
        }
    }
}
void rel_injective(relation_t rel){

bool found_injection_break = false;
    //if second pair of pairs is contained 2x == false
    for(int i = 0; i < rel.size; i++){
        for(int j = i + 1; j < rel.size; j++){
            if(rel.pairs[i].second == rel.pairs[j].second){
                printf("false"); 
                found_injection_break = true;
            }
        }
    }
    if(found_injection_break == false){
        printf("true");
    }
}
//always true
void rel_surjective(){
    printf("true");
}
void rel_bijective(relation_t rel){
   //if second pair of pairs is contained 2x == false
   bool found_injection_break = false;

    for(int i = 0; i < rel.size; i++){
        for(int j = i + 1; j < rel.size; j++){
            if(rel.pairs[i].second == rel.pairs[j].second){
                printf("false"); 
                found_injection_break = true;
            }
        }
    }
    if(found_injection_break == false){
        printf("true");
    }
}

//returns number of spoted ints in operation
// get ints to main
int oper_indentifier(char *string, int *oper_inderificator_1, int *oper_inderificator_2){

    int first_num_size = 0;
    int second_num_size = 0;
    int num_count_spliter = 0;
    bool counting_number_section = false;
    int begin_index;

    for(int i = 0; string[i] != '\0'; i++){
       if(num_count_spliter == 1){
            if (isdigit(string[i]) != 0){
                second_num_size++;
            }
            if(second_num_size > 0 && isdigit(string[i]) == 0 ){
                return 0;
            } 
        }
        if(num_count_spliter == 0){
            if (isdigit(string[i]) != 0){
                first_num_size++;
                counting_number_section = true;
            }
        }
        if(string[i] == ' ' && counting_number_section == true){
            num_count_spliter ++;
        }
        if(first_num_size > 0 && isdigit(string[i]) == 0 && string[i] != ' '){
            return 0;
        } 
    }
    if(num_count_spliter == 2 || counting_number_section == false){    
        return 0;
    }

    char string_to_int_1[first_num_size];
    char string_to_int_2[second_num_size];
    int val_1;
    int val_2;

    if(second_num_size > 0){
        begin_index = strlen(string) - first_num_size - second_num_size - 1;
        strncpy(string_to_int_1, string + begin_index, first_num_size);
        strncpy(string_to_int_2, string + begin_index + first_num_size + 1 , second_num_size);
        string_to_int_1[first_num_size]='\0';
        string_to_int_2[second_num_size]='\0';
        val_1 = atoi(string_to_int_1);
        val_2 = atoi(string_to_int_2);
        *oper_inderificator_1 = val_1;
        *oper_inderificator_2 = val_2;
        memset(string_to_int_1,'\0', sizeof string_to_int_1);
        memset(string_to_int_2,'\0', sizeof string_to_int_2);
        return 2;
    }
    if(second_num_size == 0){
        begin_index = strlen(string) - first_num_size;
        strncpy(string_to_int_1, string + begin_index, first_num_size);
        string_to_int_1[first_num_size]='\0';
        val_1 = atoi(string_to_int_1);
        *oper_inderificator_1 = val_1;
        memset(string_to_int_1,'\0', sizeof string_to_int_1);
        return 1;
    }
    return 0;
}
//returns number of operation
int oper_type(char *string){
    char *ret;
    char *operation[19];
    operation[0] = "empty";
    operation[1] = "card";
    operation[2] = "complement";
    operation[3] = "union";
    operation[4] = "intersect";
    operation[5] = "minus";
    operation[6] = "subseteq";
    operation[7] = "subset";
    operation[8] = "equals";
    //operations with realations
    operation[9] = "reflexive";
    operation[10] = "antisymmetric";
    operation[11] = "symmetric";
    operation[12] = "transitive";
    operation[13] = "function";
    operation[14] = "codomain";
    operation[15] = "domain";
    operation[16] = "injective";
    operation[17] = "surjective";
    operation[18] = "bijective";

    for(int i = 0; i <= 18; i++){
        ret = strstr(string,operation[i]);
        if(ret != NULL){
            return i;
        }  
    }
    return -1;
}
//function for aplaying operations 
int do_operation(int operation_ID, universe_t universe, unsigned first_index, unsigned second_index, set_t *sets_arr, relation_t *rels_arr){
    printf("\n");
    if(operation_ID == 0){
        if(sets_arr[first_index].elements_count == 0){
            printf("true");
        }
        else{
            printf("false");
        }
    }
    if(operation_ID == 1){
        printf("%d", sets_arr[first_index].elements_count);
    }
    if(operation_ID == 2){
        set_minus(sets_arr[first_index], sets_arr[second_index], universe);
    }
    if(operation_ID == 3){
        printf("S");
        set_union(sets_arr[first_index], sets_arr[second_index], universe);
    }
    if(operation_ID == 4){
        printf("S");
        set_intersect(sets_arr[first_index], sets_arr[second_index], universe);
    }
    if(operation_ID == 5){
        set_minus(sets_arr[second_index], sets_arr[first_index], universe);
    }
    if(operation_ID == 6){
        set_subset(sets_arr[first_index], sets_arr[second_index], operation_ID);
    }
    if(operation_ID == 7){
        set_subset(sets_arr[first_index], sets_arr[second_index], operation_ID);
    }
    if(operation_ID == 8){
        set_subset(sets_arr[first_index], sets_arr[second_index], operation_ID);
    }
    //realtion operations ============================================================================
    if(operation_ID == 9){
        rel_reflexive(rels_arr[first_index], universe);
    }
    if(operation_ID == 11){
        rel_symmetric(rels_arr[first_index]);
    }
    if(operation_ID == 10){
        rel_antisymmetric(rels_arr[first_index]);
    }
    if(operation_ID == 12){
        rel_transitive(rels_arr[first_index]);
    }
    if(operation_ID == 13){
        rel_function(rels_arr[first_index]);
    }
    if(operation_ID == 15){
        rel_domain(rels_arr[first_index], universe);
    }
    if(operation_ID == 14){
        rel_codomain(rels_arr[first_index], universe);
    }
    if(operation_ID == 16){
        rel_injective(rels_arr[first_index]);
    }
    if(operation_ID == 17){
        rel_surjective();
    }
    if(operation_ID == 18){
        rel_bijective(rels_arr[first_index]);
    }
    return 0;
}
//reads and process operations
int read_and_process_operation(char *p_operation, int operation_index, int line_counter, universe_t universe, bool R_S_Line_identyfier[], set_t *sets_arr, relation_t *rels_arr)
{
    int oper_inderificator_1;
    int oper_inderificator_2;
    int set_arr_counter = 0;
    int set_arr_counter2 = 0;
    char operation[strlen(p_operation)];
    
    for(int i = 0; i <= operation_index; i++){
        operation[i] = p_operation[i];
    }
    if(oper_type(operation) == -1){
        fprintf(stderr, "ERROR == wrong operation name\n");
        return 12;
    }
    int operation_ID = oper_type(operation);
        //one set operation
        if(operation_ID >= 0 &&
            operation_ID < 3 && 
            (oper_indentifier(operation, &oper_inderificator_1, &oper_inderificator_2) == 2 ||
             oper_indentifier(operation, &oper_inderificator_1, &oper_inderificator_2) == 0)){
                fprintf(stderr, "error only one line identificator(number) is available for this operation\n");
                return 13;
            }
        if(operation_ID >= 3 &&
            operation_ID <= 8 && 
             oper_indentifier(operation, &oper_inderificator_1, &oper_inderificator_2) == 0){
                fprintf(stderr, "error only two line identificators(numbers) are available for this operation\n");
                return 14;
            }

        else if(operation_ID >= 0 && operation_ID < 9 && oper_indentifier(operation, &oper_inderificator_1, &oper_inderificator_2) == 1){
            if(oper_inderificator_1 > line_counter){
                fprintf(stderr, "error greater identificator than Set and Relation section out\n");
                return 15;
            }
                

            // couning right index of set to be used
            if(R_S_Line_identyfier[oper_inderificator_1 - 1] == false){
                //printf("\n%d\n", oper_inderificator_1);
                fprintf(stderr, "wrong operation identificator ===> no set on this line or input contains no int char\n");
                return 16;
            } 
            
            if(R_S_Line_identyfier[oper_inderificator_1 - 1] == true){
                for(int i = 1; i <= oper_inderificator_1 -1 ; i++){
                    if(R_S_Line_identyfier[i] == true ){
                    set_arr_counter++;
                    }
                }
                do_operation(oper_type(operation), universe, set_arr_counter, set_arr_counter2, sets_arr, rels_arr);
                set_arr_counter = 17;
            } 

        }   
        //two set operation
        else if (operation_ID >= 0 && operation_ID < 9 && oper_indentifier(operation, &oper_inderificator_1, &oper_inderificator_2) == 2){
            if(oper_inderificator_1 > line_counter || oper_inderificator_2 > line_counter)
            {
                fprintf(stderr, "error greater identificator than Set and Relation section out\n");
                return 18;
            }
            if(R_S_Line_identyfier[oper_inderificator_1 - 1] == false || R_S_Line_identyfier[oper_inderificator_2 - 1] == false){
                fprintf(stderr, "error operation identificators are from multiple arreas\n");
                return 19;
            }

            if(R_S_Line_identyfier[oper_inderificator_1 - 1] == true){
                //printf("%d\n", R_S_Line_identyfier[oper_inderificator_1 - 1]);
                for(int i = 1; i <= oper_inderificator_1 -1 ; i++){
                    if(R_S_Line_identyfier[i] == true ){
                    set_arr_counter++;
                    }
                }
            }

            if(R_S_Line_identyfier[oper_inderificator_2 - 1] == true){
                for(int i = 1; i <= oper_inderificator_2 -1 ; i++){
                    if(R_S_Line_identyfier[i] == true ){
                    set_arr_counter2++;
                    }
                }
            }
            //do operation
            do_operation(oper_type(operation), universe, set_arr_counter, set_arr_counter2, sets_arr, rels_arr);
            set_arr_counter = 0;
            set_arr_counter2 = 0;
        }
        //relation operations
         if(operation_ID >= 9 &&
            operation_ID <= 18 && 
            (oper_indentifier(operation, &oper_inderificator_1, &oper_inderificator_2) == 2 ||
             oper_indentifier(operation, &oper_inderificator_1, &oper_inderificator_2) == 0)){
                fprintf(stderr, "error only one line identificator(number) is available for this operation\n");
                return 20;
            }

        else if (operation_ID >= 9 && operation_ID <= 18 && oper_indentifier(operation, &oper_inderificator_1, &oper_inderificator_2) == 1){
            if(oper_inderificator_1 > line_counter){
                fprintf(stderr, "error greater identificator than Set and Relation section out\n");
                return 21;
            }
                
            // couning right index of set to be used
            if(R_S_Line_identyfier[oper_inderificator_1 - 1] == true){
                fprintf(stderr, "wrong operation identificator ===> no realtion on this line\n");
                return 22;
            } 
            
            if(R_S_Line_identyfier[oper_inderificator_1 - 1] == false){
                for(int i = 1; i <= oper_inderificator_1 -1 ; i++){
                    if(R_S_Line_identyfier[i] == false ){
                        set_arr_counter++;
                    }
                }
                set_arr_counter--;
                do_operation(oper_type(operation), universe, set_arr_counter, set_arr_counter2, sets_arr, rels_arr);
                set_arr_counter = 0;
            } 
        }
    return 0;
}


int main(int argc, char **argv)
{
    //open file
    FILE *fp;
    int c;
    if(argc != 2){
        fprintf(stderr, "Syntax error \nSyntax is ./[name of program] [text file with data]\n");
        return 100;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL){
        fprintf(stderr, "File cant be opened!\n");
        return 101;
    }

    //initializing flags
    cfg_t cfg;
    cfg.reading_universe_flag = 0;
    cfg.reading_set_flag = 0;
    cfg.reading_element_flag = 0;
    cfg.reading_pair_flag = 0;
    cfg.reading_relation_flag = 0;
    cfg.reading_oper_flag = 0;
    cfg.section_flag = 0;

    //init buffer for reading pairs and elements
    char buffer[MAX_ELEMENT_SIZE];
    int buffer_counter = 0;
    char pair_buffer[MAX_ELEMENT_SIZE * 2 + 4];
    int pair_buffer_counter = 0;

    char operation[MAX_OPER_ELEMENT_SIZE];
    int operation_index = 0;
    int line_counter = 0;
    bool R_S_Line_identyfier[999];                    //if true -> line == set //      // if false -> line == relation// 

    //init universe
    universe_t univ;
    universe_constructor(&univ);
    int universe_counter = 0;

    pair_t pair;
    pair.second = 1;
    pair.first = 2;

    int EXIT_CODE = 0;

    //init array of sets
    set_t *sets_arr = (set_t *) malloc(1 * sizeof(set_t));
    if (sets_arr == NULL){
        printf("Out of memory");
        exit(1);
    }
    int set_index = 0;

    //init array of relations
    relation_t *relation_arr = (relation_t *) malloc(1 * sizeof(relation_t));
    if (relation_arr == NULL){
        printf("Out of memory");
        exit(1);
    }
    int relation_index = 0;

    //reading file
    while((c = fgetc(fp)) != EOF) {
        if (line_counter == 1000){
            fprintf(stderr, "Too many lines");
            if (cfg.reading_set_flag == 1){
                set_index++;
            }
            if (cfg.reading_relation_flag == 1){
                relation_index++;
            }
            EXIT_CODE = 93;
            break;
        }
        //reading universe
        if (c == 'U' && cfg.reading_element_flag != 1 && cfg.reading_pair_flag != 1) {
            if (cfg.reading_universe_flag == 2){
                fprintf(stderr, "Universe was already created!\n");
                EXIT_CODE = 5;
                break;
            }
            R_S_Line_identyfier[line_counter++] = true;
            set_constructor(&sets_arr[set_index]);
            cfg.reading_universe_flag = 1;
            continue;
        }
        if (c && cfg.reading_universe_flag == 0){
            fprintf(stderr, "Universe was not initialized!");
            EXIT_CODE = 99;
            break;
        }
        //reading set
        if (c == 'S' && cfg.reading_element_flag != 1) {
            if (cfg.section_flag == 1){
                fprintf(stderr, "Relations/sets section was finished!\n");
                EXIT_CODE = 6;
                break;
            }
            R_S_Line_identyfier[line_counter++] = true;
            cfg.reading_set_flag = 1;
            sets_arr = (set_t *)realloc(sets_arr, (set_index + 1) * sizeof(set_t ));
            set_constructor(&sets_arr[set_index]);
            continue;
        }
        //reading relation
        if (c == 'R' && cfg.reading_element_flag != 1) {
            if (cfg.section_flag == 1){
                fprintf(stderr, "Relations/sets section was finished!\n");
                EXIT_CODE = 6;
                break;
            }
            R_S_Line_identyfier[line_counter++] = false;
            cfg.reading_relation_flag = 1;
            relation_arr = (relation_t *)realloc(relation_arr, (relation_index + 1) * sizeof(relation_t ));
            relation_constructor(&relation_arr[relation_index]);
            continue;
        }
        //reading operator
        if(c == 'C' && cfg.reading_oper_flag == 0 && cfg.reading_element_flag != 1) {
            cfg.reading_oper_flag = 1;
            cfg.section_flag = 1;
        }
        if (c == ' ' && cfg.reading_element_flag == 0 && cfg.reading_relation_flag == 0) {
            cfg.reading_element_flag = 1;
            buffer_counter = 0;
            continue;
        }
        if (c == '('  && cfg.reading_pair_flag == 0 && cfg.reading_relation_flag == 1) {
            cfg.reading_pair_flag = 1;
            pair_buffer_counter = 0;
            continue;
        }
        //adding character to pair buffer
        if (cfg.reading_pair_flag == 1){
            if (buffer_counter > MAX_ELEMENT_SIZE * 2 + 4) {
                relation_index++;
                fprintf(stderr, "Pair is longer than 60!\n");
                break;
            }
            pair_buffer[pair_buffer_counter++] = c;
        }
        //end of pair
        if (cfg.reading_pair_flag == 1 && c == ')'){
            pair_buffer[pair_buffer_counter - 1] = '\0';
            pair = get_pair_from_string(pair_buffer, pair_buffer_counter, univ);
            if (pair.first == -1 || pair.second == -1){
                relation_index++;
                fprintf(stderr, "Wrong input!\n");
                EXIT_CODE = 4;
                break;
            }
            relation_append(&relation_arr[relation_index], pair);
            cfg.reading_pair_flag = 0;
        }
        //adding character to buffer(element)
        if (cfg.reading_element_flag == 1) {
            if (cfg.reading_universe_flag == 1 && !isalpha(c) && c != ' ' && c != '\n'){
                fprintf(stderr, "Elements should contain only letters from abc");
                set_index++;
                EXIT_CODE = 88;
                break;
            }
            if (buffer_counter > MAX_ELEMENT_SIZE - 1) {
                fprintf(stderr,"Element is longer than 30!\n");
                set_index++;
                EXIT_CODE = 1;
                break;
            }
            buffer[buffer_counter++] = c;
        }
        //end of element
        if ((c == ' ' || c == '\n') && cfg.reading_element_flag == 1) {
            buffer[buffer_counter - 1] = '\0';
            //adding element to universe and  set(universe)
            if (cfg.reading_universe_flag == 1) {
                if (is_element_restricted(buffer)){
                    fprintf(stderr,"Element is restricted!\n");
                    set_index++;
                    EXIT_CODE = 11;
                    break;
                }
                add_element_to_universe(&univ, buffer, buffer_counter);
                if (is_element_in_set(index_in_universe(univ, buffer,buffer_counter), sets_arr[set_index])){
                    set_index++;
                    fprintf(stderr, "Element of universe is written twice\n");
                    EXIT_CODE = 2;
                    break;
                }
                set_add_element(&sets_arr[set_index], universe_counter++);
            }
            if (cfg.reading_set_flag == 1) {
                int element =  index_in_universe(univ, buffer, buffer_counter);
                if (element == -1) {
                    set_index++;
                    fprintf(stderr, "Element of set is not in universe! Or line is not finished properely\n");
                    EXIT_CODE = 3;
                    break;
                }
                if (is_element_in_set(element, sets_arr[set_index])) {
                    set_index++;
                    fprintf(stderr,"Element is already in set\n");
                    EXIT_CODE = 2;
                    break;
                }
                set_add_element(&sets_arr[set_index], element);
            }
            buffer_counter = 0;
            cfg.reading_element_flag = 2;
        }
        if (c && (cfg.reading_relation_flag != 1 &&
                  cfg.reading_universe_flag != 1 &&
                  cfg.reading_set_flag != 1 &&
                  cfg.reading_oper_flag != 1)){
            fprintf(stderr, "Wrong input!\n");
            EXIT_CODE = 9;
            break;
        }
        //element was read
        if (c == ' ' && cfg.reading_element_flag == 2) {
            cfg.reading_element_flag = 1;
        }
        //universe was read
        if (c == '\n' && cfg.reading_universe_flag == 1) {
            print_universe(univ);
            sets_arr = (set_t *)realloc(sets_arr, (set_index + 1) * sizeof(set_t ));
            set_index++;
            cfg.reading_universe_flag = 2;
        }
        //set was read
        if (c == '\n' && cfg.reading_set_flag == 1) {
            set_print(sets_arr[set_index], univ);
            set_index++;
            cfg.reading_set_flag = 0;
        }
        //relation was read
        if (c == '\n' && cfg.reading_relation_flag == 1){
            relation_print(relation_arr[relation_index], univ);
            relation_index++;
            cfg.reading_relation_flag = 0;
        }

        if(cfg.reading_oper_flag == 1){
            operation[operation_index++] = c;
            if(c == '\n'){
                operation[operation_index -1] = '\0';
                // line has been read 
                //process operations
                //printf("\n");
                EXIT_CODE = read_and_process_operation(operation, operation_index, line_counter, univ, R_S_Line_identyfier, sets_arr, relation_arr);
                //clear
                memset(operation, 0 , sizeof operation);
                operation_index = 0;
                cfg.reading_oper_flag = 0;
                if (EXIT_CODE){
                    break;
                }
            }   
        } 
    }

    if(c == EOF && cfg.reading_oper_flag == 1) {
        operation[operation_index] = '\0';
        // printf("%s\n", operation);
        EXIT_CODE = read_and_process_operation(operation, operation_index, line_counter, univ, R_S_Line_identyfier, sets_arr, relation_arr);
        memset(operation, 0, sizeof operation);
        operation_index = 0;
        cfg.reading_oper_flag = 0;
        printf("\n");
    }
    //freeing memory and closing file
    call_dtors(fp, &univ, sets_arr, set_index, relation_arr, relation_index);
    return EXIT_CODE;
}