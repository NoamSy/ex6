#include "ex6.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
name: Noam Sayada
id: 216308825
exercise - 6
*/
# define INT_BUFFER 128

// ================================================
// Basic struct definitions from ex6.h assumed:
//   PokemonData { int id; char *name; PokemonType TYPE; int hp; int attack; EvolutionStatus CAN_EVOLVE; }
//   PokemonNode { PokemonData* data; PokemonNode* left, *right; }
//   OwnerNode   { char* ownerName; PokemonNode* pokedexRoot; OwnerNode *next, *prev; }
//   OwnerNode* ownerHead;
//   const PokemonData pokedex[];
// ================================================

// --------------------------------------------------------------
// 1) Safe integer reading
// --------------------------------------------------------------

void trimWhitespace(char *str)
{
    // Remove leading spaces/tabs/\r
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')
        start++;

    if (start > 0)
    {
        int idx = 0;
        while (str[start])
            str[idx++] = str[start++];
        str[idx] = '\0';
    }

    // Remove trailing spaces/tabs/\r
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r'))
    {
        str[--len] = '\0';
    }
}

char *myStrdup(const char *src)
{
    if (!src)
        return NULL;
    size_t len = strlen(src);
    char *dest = (char *)malloc(len + 1);
    if (!dest)
    {
        printf("Memory allocation failed in myStrdup.\n");
        return NULL;
    }
    strcpy(dest, src);
    return dest;
}

int readIntSafe(const char *prompt)
{
    char buffer[INT_BUFFER];
    int value;
    int success = 0;

    while (!success)
    {
        printf("%s", prompt);

        // If we fail to read, treat it as invalid
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            printf("Invalid input.\n");
            clearerr(stdin);
            continue;
        }

        // 1) Strip any trailing \r or \n
        //    so "123\r\n" becomes "123"
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
            buffer[--len] = '\0';

        // 2) Check if empty after stripping
        if (len == 0)
        {
            printf("Invalid input.\n");
            continue;
        }

        // 3) Attempt to parse integer with strtol
        char *endptr;
        value = (int)strtol(buffer, &endptr, 10);

        // If endptr didn't point to the end => leftover chars => invalid
        // or if buffer was something non-numeric
        if (*endptr != '\0')
        {
            printf("Invalid input.\n");
        }
        else
        {
            // We got a valid integer
            success = 1;
        }
    }
    return value;
}

// --------------------------------------------------------------
// 2) Utility: Get type name from enum
// --------------------------------------------------------------
const char *getTypeName(PokemonType type)
{
    switch (type)
    {
    case GRASS:
        return "GRASS";
    case FIRE:
        return "FIRE";
    case WATER:
        return "WATER";
    case BUG:
        return "BUG";
    case NORMAL:
        return "NORMAL";
    case POISON:
        return "POISON";
    case ELECTRIC:
        return "ELECTRIC";
    case GROUND:
        return "GROUND";
    case FAIRY:
        return "FAIRY";
    case FIGHTING:
        return "FIGHTING";
    case PSYCHIC:
        return "PSYCHIC";
    case ROCK:
        return "ROCK";
    case GHOST:
        return "GHOST";
    case DRAGON:
        return "DRAGON";
    case ICE:
        return "ICE";
    default:
        return "UNKNOWN";
    }
}

// --------------------------------------------------------------
// Utility: getDynamicInput (for reading a line into malloc'd memory)
// --------------------------------------------------------------
char *getDynamicInput()
{
    char *input = NULL;
    size_t size = 0, capacity = 1;
    input = (char *)malloc(capacity);
    if (!input)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (size + 1 >= capacity)
        {
            capacity *= 2;
            char *temp = (char *)realloc(input, capacity);
            if (!temp)
            {
                printf("Memory reallocation failed.\n");
                free(input);
                return NULL;
            }
            input = temp;
        }
        input[size++] = (char)c;
    }
    input[size] = '\0';

    // Trim any leading/trailing whitespace or carriage returns
    trimWhitespace(input);

    return input;
}

// Function to print a single Pokemon node
void printPokemonNode(PokemonNode *node)
{
    if (!node)
        return;
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
           node->data->id,
           node->data->name,
           getTypeName(node->data->TYPE),
           node->data->hp,
           node->data->attack,
           (node->data->CAN_EVOLVE == CAN_EVOLVE) ? "Yes" : "No");
}















OwnerNode *findOwnerByName(const char *name)
{
    //no owners therfore no owners with that name
    if (ownerHead == NULL)
        return NULL;
    //if we have only one owner, check only for him if he has the same name
    if (ownerHead->next == ownerHead) {
        if (strcmp(ownerHead->ownerName, name) == 0)
            return ownerHead;
        return NULL;
    }
    //check for the first one
    if (strcmp(ownerHead->ownerName, name) == 0)
        return ownerHead;
    //i will go from the beginning to the ending and check if i can find a owner with the name, if so i will return
    //him, if not i will return NULL
   OwnerNode *node = ownerHead->next;
   while (node != ownerHead) {
       if (strcmp(node->ownerName, name) == 0)
           return node;
       node = node->next;
   }
    return NULL;
}

//create a search tree for a new owner(which is the pokedox)
//PokemonNode *createPokemonNode(const PokemonData *data);
PokemonNode*  createPokemonNode(PokemonData *data)
{
    PokemonNode *node = (PokemonNode *)malloc(sizeof(PokemonNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}
//check validity before crating the Pokedox and get the inputs, and then create the new owner in the circle node
void openPokedexMenu()
{
    printf("Your name: ");
    char *ownerName= getDynamicInput();
    //if that name already exists, then we cannot create a new pokedox
    if(findOwnerByName(ownerName) != NULL)
    {
        printf("Owner '%s' already exists. Not creating a new Pokedex.\n", ownerName);
        free(ownerName);
        return;
    }
    printf("Choose starter:\n1. Bulbasaur\n2. Charmander\n3. Squirtle\nYour choice: ");
    int choice = readIntSafe("Your choice: ");
    int id;
    //find the chosen pokimon's id
    switch (choice)
    {
    case 1:
        id = 1;
        break;
    case 2:
        id = 4;
        break;
    case 3:
        id = 7;
        break;
        //if invalid input then return nothing, and get out of function
    default:
        printf("Invalid choice.\n");
        free(ownerName);
        return;
    }
    //get all information about the pokimon on the pokedex array
     PokemonData *temp = &pokedex[id-1];
    //create the new owner
    OwnerNode *newOwner = createOwner(ownerName, createPokemonNode(temp));
    //if no owners exist, just make owner head the newOwner
    if (ownerHead == NULL) {
        ownerHead = newOwner;
        ownerHead->next = ownerHead->prev = ownerHead;
        //else we already have a user, we need first user's previous to be the new last owner, and the new owners next
        // to be the first owner, and the new owners prev to be the owner that was last before
    } else {
        ///get the last node
        OwnerNode *last = ownerHead->prev;
        //put after the next node newOwner
        last->next = newOwner;
        //set the previous of our current owner as our one before last
        newOwner->prev = last;
        //set the last owner's next to the first Owner
        newOwner->next = ownerHead;
        //set our finrt owner previous to the new last owner
        ownerHead->prev = newOwner;
    }
    printf("New Pokedex created for %s with starter %s\n", ownerName, temp->name);

}
//function that will create a new owner
OwnerNode *createOwner(char *ownerName, PokemonNode *newOwner) {
    OwnerNode *createOwner = (OwnerNode *)malloc(sizeof(OwnerNode));
    if (newOwner == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    createOwner->ownerName = ownerName;
    createOwner->pokedexRoot = newOwner;
    createOwner->next = NULL;
    createOwner->prev = NULL;
    return createOwner;
}
void enterExistingPokedexMenu() {
    if (ownerHead == NULL){
        printf("Invalid.\n");
            return;
            }
    //i will go from the start to the end, and print each existing pokedexes
    //i will make a pointer that starts from the beggingig untill reaching ownerHead->previous as it is the last
    printf("Existing Pokedexes (circular list):\n");
    OwnerNode *owner = ownerHead->next;
    int option = 1;
    //print first owner
    printf("%d. %s\n", option, ownerHead->ownerName);
    //print remaining owners
    while (owner != ownerHead) {
        option++;
        printf("%d. %s\n", option, owner->ownerName);
        owner = owner->next;
    }
    int choice = readIntSafe("Choose a Pokedex by number: ");
    //choice must be atleast 1
    if (choice < 1) {
        printf("Invalid choice.\n");
        return;
    }
    //make owner the head of the node again
    owner = ownerHead->next;
    for (int i = 1; i < choice; ++i) {
        //if we reached  the beggining of the node before the loop ending, it means we dont have a owner in that
        //position
        if (owner == ownerHead) {
            printf("Invalid.\n");
            return;
        }

        owner = owner->next;
    }
    //in the loop we went one too much, therfore go to the previous one
    owner = owner->prev;

    char *name = owner->ownerName;
    printPokemonNode(owner->pokedexRoot);
    //read a int again for the pokedex menu
    int subChoice;
    do
    {
        printf("\n-- %s's Pokedex Menu --\n", name);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");

        subChoice = readIntSafe("Your choice: ");

        switch (subChoice)
        {
            case 1://add pokemon
                addPokemon(owner);
                break;
            case 3: //remove pokemon
                freePokemon(owner);
                break;
            case 4://pokemon fight
                pokemonFight(owner);
                break;
            case 5:
                evolvePokemon(owner);
                break;
            case 6:
                printf("Back to Main Menu.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (subChoice != 6);
}

PokemonNode *searchPokemonBFS(PokemonNode *root, int id) {
    if (root == NULL) // we reached edge, return null
        return NULL;
    if (root->data->id == id) //check if root has same id
        return root;

    //go through the left trees
    PokemonNode *left = searchPokemonBFS(root->left, id);
    if (left != NULL) //if found, return
        return left;

    return searchPokemonBFS(root->right, id);
}
//this tree with place the current node in the tree
//basically if the root is bigger, it goes right
//if the root is smaller, it goes left, up untill it goes to NULL, which is when we place it
PokemonNode *insertPokemonNode(PokemonNode *root, PokemonNode *newNode){
    // this will be where it needs to be placed, so place it
    if (root == NULL) {
        return newNode;
    }

    // ff the new pokemon id is smaller, go to the left
    if (newNode->data->id < root->data->id) {
        root->left = insertPokemonNode(root->left, newNode);
    }
    // ff the new pokemon id is bigger, go to the right
    else if (newNode->data->id > root->data->id) {
        root->right = insertPokemonNode(root->right, newNode);
    }

    // return root
    return root;
}
void addPokemon(OwnerNode *owner) {
    int choice = readIntSafe("Enter ID to add: ");
    //check if that pokemon exsits in our tree
    if (searchPokemonBFS(owner->pokedexRoot, choice) != NULL) {
        printf("Pokemon with ID %d is already in the Pokedex. No changes made.", choice);
        return;
    }
    //else i will add it, i will sort the tree in a way in which it will go that every id bigger than the root
    //will go to right tree, and every smaller will go to left root
    PokemonData *temp = &pokedex[choice-1];
    //add the pokemon to the pokedex
    owner->pokedexRoot = insertPokemonNode(owner->pokedexRoot, createPokemonNode(temp));
    printf("Pokemon %s (ID %d) added.", temp->name, temp->id);

}
PokemonNode *removePokemonByID(PokemonNode *root, int id) {
    if (root == NULL)
        return NULL;

    if (id < root->data->id)
        root->left = removePokemonByID(root->left, id);
    else if (id > root->data->id)
        root->right = removePokemonByID(root->right, id);
    //if we found the id, check if right is not null and then replace the data of the root with the right tree after
    //do for left if there is no right child
    if (root->data->id == id) {
        //if no parents return null
        if (root->right == NULL && root->left == NULL) {
            free(root);
            return NULL;
        }
        if (root->right == NULL) {
            //make the current roots have the highest after him
            PokemonNode *toReturn = root->left;
            free(root);
            return toReturn;
        }
        if (root->left == NULL) {
            //make the current roots have the highest after him
            PokemonNode *toReturn = root->right;
            free(root);
            return toReturn;
        }
        //now check two children
        PokemonNode *toReturn = root->right;
        //in the right branch go untill we find the minimum
        while (toReturn->left != NULL) {
            toReturn = toReturn->left;
        }
        //make the current data be equal to the smallest num in the right root child
        root->data = toReturn->data;
        //now we need to delete the lowest tree as it it written twice
        //no need to free because recursiion will free in later calls
        return removePokemonByID(root, toReturn->data->id);
    }

    return root;
}
void freePokemon(OwnerNode *owner) {
    //check if we even have pokemons
    if (owner->pokedexRoot == NULL) {
        printf("No Pokemon to release.\n");
        return;
    }
    int choice = readIntSafe("Enter Pokemon ID to realease: ");
    //check if the pokemon exists
    if (searchPokemonBFS(owner->pokedexRoot, choice) == NULL) {
        printf("No Pokemon with ID %d found.\n", choice);
        return;
    }
    printf("Removing Pokemon %s (ID %d).\n", pokedex[choice-1].name, choice);
    removePokemonByID(owner->pokedexRoot, choice);
}

void pokemonFight(OwnerNode *owner) {
    if (owner->pokedexRoot == NULL) {
        printf("Cannot evolve. Pokedex empty.\n");
        return;
    }
    int firstPokemon = readIntSafe("Enter ID of the first Pokemon: ");
    int secondPokemon = readIntSafe("Enter ID of the second Pokemon: ");
    //check if ids exist
    if (searchPokemonBFS(owner->pokedexRoot, firstPokemon) == NULL ||
        searchPokemonBFS(owner->pokedexRoot, secondPokemon) == NULL) {
        printf("One or both Pokemon IDs not found.\n");
        return;
    }
    double firstPokemonScore = (pokedex[firstPokemon-1].hp * 1.2) + (pokedex[firstPokemon-1].attack * 1.5);
    double secondPokemonScore = (pokedex[secondPokemon-1].hp * 1.2) + (pokedex[secondPokemon-1].attack * 1.5);
    printf("Pokemon 1: %s (Score = %0.2f)\n", pokedex[firstPokemon-1].name, firstPokemonScore);
    printf("Pokemon 2: %s (Score = %0.2f)\n", pokedex[secondPokemon-1].name, secondPokemonScore);
    if (firstPokemonScore > secondPokemonScore)
        printf("%s wins!\n", pokedex[firstPokemon-1].name);
    else if (secondPokemonScore > firstPokemonScore)
        printf("%s wins!\n", pokedex[secondPokemon-1].name);
    else
        printf("It's a tie!\n");
}
void evolvePokemon(OwnerNode *owner) {
    if (owner->pokedexRoot == NULL) {
        printf("Pokedex is empty.\n");
        return;
    }
    int choice = readIntSafe("Enter ID of Pokemon to evolve: ");
    //check if it exists in the tree
    if (searchPokemonBFS(owner->pokedexRoot, choice) == NULL) {
        printf("No Pokemon with ID %d found.", choice);
        return;
    }
    //check if it can evolve
    if (!(pokedex[choice-1].CAN_EVOLVE)) {
        printf("%s (ID %d) cannot evolve.\n", pokedex[choice-1].name, pokedex[choice-1].id);
        return;
    }
    //check if the evolved form is also in the tree
    //if so just remove the un-evolved form
    if (searchPokemonBFS(owner->pokedexRoot, choice+1) != NULL) {
        printf("Evolution ID %d (%s) already in the Pokedex. Releasing %s (ID %d).", pokedex[choice].id,
            pokedex[choice].name, pokedex[choice-1].name, pokedex[choice-1].id);
        printf("Removing Pokemon %s (ID %d).\n", pokedex[choice-1].name, pokedex[choice-1].id);
        removePokemonByID(owner->pokedexRoot, choice);
    }
    //if we dont have the evolved form, then just make the pokimon's data the id+1, as we know it doesnt exist in the
    //tree
    PokemonNode *toEvolve = searchPokemonBFS(owner->pokedexRoot, choice);
    toEvolve->data = &pokedex[choice];
    printf("Pokemon evolved from %s (ID %d) to %s (ID %d).\n", pokedex[choice-1].name, pokedex[choice-1].id,
     pokedex[choice].name, pokedex[choice].id);
}




// --------------------------------------------------------------
// Display Menu
// --------------------------------------------------------------
/*void displayMenu(OwnerNode *owner)
{
    if (!owner->pokedexRoot)
    {
        printf("Pokedex is empty.\n");
        return;
    }

    printf("Display:\n");
    printf("1. BFS (Level-Order)\n");
    printf("2. Pre-Order\n");
    printf("3. In-Order\n");
    printf("4. Post-Order\n");
    printf("5. Alphabetical (by name)\n");

    int choice = readIntSafe("Your choice: ");

    switch (choice)
    {
    case 1:
        displayBFS(owner->pokedexRoot);
        break;
    case 2:
        preOrderTraversal(owner->pokedexRoot);
        break;
    case 3:
        inOrderTraversal(owner->pokedexRoot);
        break;
    case 4:
        postOrderTraversal(owner->pokedexRoot);
        break;
    case 5:
        displayAlphabetical(owner->pokedexRoot);
        break;
    default:
        printf("Invalid choice.\n");
    }
}*/

// --------------------------------------------------------------
// Sub-menu for existing Pokedex
// --------------------------------------------------------------
/*void enterExistingPokedexMenu()
{
    // list owners
    printf("\nExisting Pokedexes:\n");
    // you need to implement a few things here :)

    printf("\nEntering %s's Pokedex...\n", cur->ownerName);

    int subChoice;
    do
    {
        printf("\n-- %s's Pokedex Menu --\n", cur->ownerName);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");

        subChoice = readIntSafe("Your choice: ");

        switch (subChoice)
        {
        case 1:
            addPokemon(cur);
            break;
        case 2:
            displayMenu(cur);
            break;
        case 3:
            freePokemon(cur);
            break;
        case 4:
            pokemonFight(cur);
            break;
        case 5:
            evolvePokemon(cur);
            break;
        case 6:
            printf("Back to Main Menu.\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (subChoice != 6);
}
*/
// --------------------------------------------------------------
// Main Menu
// --------------------------------------------------------------
void mainMenu()
{
    int choice;
    do
    {
        printf("\n=== Main Menu ===\n");
        printf("1. New Pokedex\n");
        printf("2. Existing Pokedex\n");
        printf("3. Delete a Pokedex\n");
        printf("4. Merge Pokedexes\n");
        printf("5. Sort Owners by Name\n");
        printf("6. Print Owners in a direction X times\n");
        printf("7. Exit\n");
        choice = readIntSafe("Your choice: ");
        switch (choice)
        {
        case 1:
            openPokedexMenu();
            break;
        case 2:
            enterExistingPokedexMenu();
            break;
        /*case 3:
            deletePokedex();
            break;
        case 4:
            mergePokedexMenu();
            break;
        case 5:
            sortOwners();
            break;
        case 6:
            printOwnersCircular();
            break;
        */case 7:
            printf("Goodbye!\n");
            break;
        default:
            printf("Invalid.\n");
        }
    } while (choice != 7);
}

int main()
{
    mainMenu();
    //freeAllOwners();
    return 0;
}
