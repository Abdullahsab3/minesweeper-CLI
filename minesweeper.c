#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "helpers.h"

/*

          __  ____                                                   
         /  |/  (_)___  ___  ______      _____  ___  ____  ___  _____
        / /|_/ / / __ \/ _ \/ ___/ | /| / / _ \/ _ \/ __ \/ _ \/ ___/
       / /  / / / / / /  __(__  )| |/ |/ /  __/  __/ /_/ /  __/ /    
      /_/  /_/_/_/ /_/\___/____/ |__/|__/\___/\___/ .___/\___/_/     
                                                 /_/                 
                 
                          Abdullah Sabaa Allil
                      abdullah.sabaa.allil@vub.be
                           Rolnummer: 0575278
                       Vrije Universiteit Brussel
                               2021-2022


*/

enum dimensions
{
    NUMBEROFROWS = 10,
    NUMBEROFCOLS = 7
};

enum input
{
    INPUT_ROW,
    INPUT_COL
};

#define UNREVEALED -1
#define INIT_NOT_MINE 0
#define MINE 'M'

// hoe hoger dit getal, hoe minder mijnen in het veld
#define MINES_RATIO 2

int is_mine(int el);
int is_init_not_mine(int el);
int is_flag(int el);
int is_unrevealed(int el);
int is_revealed(int el);
int all_not_mines_revealed(int AmountOfNotMines);
int all_mines_flagged(int AmountOfMines);
void print_intro(void);
void print_field(int el);
void print_game(int fields[NUMBEROFROWS][NUMBEROFCOLS], int *AmountOfFlags);
void gameover(int *exitflag);
void gameWon(int *exitflag);
void set_flag(int fields[NUMBEROFROWS][NUMBEROFCOLS], int outfields[NUMBEROFROWS][NUMBEROFCOLS], int row, int col, int *AmountOfFlags, int *AmountofMines);
int get_operands(int operands[]);
void consume_wrong_input(void);
void command_prompt(char *operator, int operands[]);
int is_out_of_boundaries(int operands[]);
void process_command(char *operator, int operands[], int fields[NUMBEROFROWS][NUMBEROFCOLS], int outfields[NUMBEROFROWS][NUMBEROFCOLS], int *AmountOfNotMines, int *AmountofFlags, int *AmountOfMines, int *exitflag);
void process_first_command(char *operator, int operands[], int fields[NUMBEROFROWS][NUMBEROFCOLS], int outfields[NUMBEROFROWS][NUMBEROFCOLS], int *AmountOfNotMines, int *AmountofFlags, int *AmountOfMines, int *exitflag);
void swap_mine_with_not_mine(int fields[NUMBEROFROWS][NUMBEROFCOLS], int row, int col);
void reveal_surroundings(int fields[NUMBEROFROWS][NUMBEROFCOLS], int outfields[NUMBEROFROWS][NUMBEROFCOLS], int row, int col, int *AmountOfNotMines, int *AmountOfFlags, int *AmountOfMines);
void reveal(int fields[NUMBEROFROWS][NUMBEROFCOLS], int outfields[NUMBEROFROWS][NUMBEROFCOLS], int row, int col, int *AmountOfNotMines, int *AmountOfFlags, int *AmountOfMines);
int how_many_surrounding_mines(int fields[NUMBEROFROWS][NUMBEROFCOLS], int row, int col);
int mine_or_yours(void);
void fill_fields(int fields[NUMBEROFROWS][NUMBEROFCOLS], int *AmountOfMines);
void initialize_fields(int fields[NUMBEROFROWS][NUMBEROFCOLS]);
void fill_outfields(int outfields[NUMBEROFROWS][NUMBEROFCOLS]);
void initialize_game(char *operator, int operands[], int fields[NUMBEROFROWS][NUMBEROFCOLS], int outfields[NUMBEROFROWS][NUMBEROFCOLS], int *AmountOfNotMines, int *AmountOfFlags, int *AmountOfMines, int *exitflag);
int is_gamewon(int *AmountOfNotMines, int *AmountOfMines);
void minesweeper(void);

int main(void)
{
    //seed voor de rand functie.
    srand(time(NULL));
    minesweeper();
    return 0;
}

// predikaat om na te gaan of een element een mijn is.
int is_mine(int el)
{
    return (el == MINE);
}

// het vakje is initieel (= voor dat we de vakjes die geen mijnen bevatten gaan invullen) geen mijn.
int is_init_not_mine(int el)
{
    return (el == INIT_NOT_MINE);
}
// predikaat om na te gaan of een element een vlag is.
int is_flag(int el)
{
    return (el == 'F');
}

// predikaat om na te gaan of een element nog niet onthuld is.
// niet onthulde elementen worden voorgesteld door -1
int is_unrevealed(int el)
{
    return (el < 0);
}

// predikaat om na te gaan of het vakje al reeds gereveald is, en geen vlag is (zie set_flag )
int is_revealed(int el)
{
    return (!(is_unrevealed(el) || is_flag(el)));
}

// predikaat om na te gaan of alle vakjes die geen mijn bevatten onthuld zijn.
int all_not_mines_revealed(int AmountOfNotMines)
{
    return (AmountOfNotMines == 0);
}

// predikaat om na te gaan of alle mijnen geflagged zijn.
int all_mines_flagged(int AmountOfMines)
{
    return (AmountOfMines == 0);
}

// een predikaat om na te gaan of de speler gewonnen heeft.
int is_gamewon(int *AmountOfNotMines, int *AmountOfMines)
{
    return (all_not_mines_revealed(*AmountOfNotMines) || all_mines_flagged(*AmountOfMines));
}

void print_intro(void)
{
    printf("\nWelcome to Minesweeper!\n");
    printf("\nUsage:\n\n");
    printf(" R r c to reveal the square at the r'th row and the c'th column.\n");
    printf(" F r c to (de)flag the square at the r'th row and the c'th column.\n");
    printf(" P to print the revealed game (for testing purposes, obviously).\n");
    printf(" Q to exit the game.\n\n");
}
// deze functie zal een vakje van het spel printen.
void print_field(int el)
{
    // als het vakje een mijn bevat (voorgesteld door M) of een vlag (voorgesteld door F) dan moet je het als letter printen
    if (is_mine(el) || is_flag(el))
    {
        printf("%c", el);
    }
    // als het vakje niet gevrealed is moet je het printen als een spatie
    else if (is_unrevealed(el))
    {
        printf(" ");
    }
    // als het vakje geen mijn bevat moet je het getal erin printen. Dat getal is het aantal naburige mijnen
    else
    {
        printf("%d", el);
    }

    printf(" || ");
}

// heel het spel printen.
void print_game(int fields[NUMBEROFROWS][NUMBEROFCOLS], int *AmountOfFlags)
{
    printf("Remaining flags: %d\n", *AmountOfFlags);

    for (int i = 0; i < NUMBEROFCOLS; i++)
    {
        // print de kolomindex
        printf("    %d", i);
    }
    printf("\n");

    for (int i = 0; i < NUMBEROFROWS; i++)
    {
        // print de rijindex
        printf("%d | ", i);
        for (int j = 0; j < NUMBEROFCOLS; j++)
        {
            // print de elementen van de ide rij
            print_field(fields[i][j]);
        }
        printf("\n");
    }
}

// het spel verlaten wanneer de speler verliest.
void gameover(int *exitflag)
{
    printf("You stepped on a mine, whoops!\n");
    *exitflag = 1;
}

// het spel verlaten wanneer de speler wint.
void gameWon(int *exitflag)
{
    printf("Congrats! You are a true minesweeper. Consider a job at the army.\n");
    *exitflag = 1;
}

void set_flag(int fields[NUMBEROFROWS][NUMBEROFCOLS], int outfields[NUMBEROFROWS][NUMBEROFCOLS], int row, int col, int *AmountOfFlags, int *AmountofMines)
{

    // als het element reeds gevreveald is, dan moet je hem niet kunnen flaggen.
    if (is_revealed(outfields[row][col]))
    {
        printf("This field is already revealed.\n");
    }
    // als er al een flag is, deflag het vakje.
    else if (is_flag(outfields[row][col]))
    {
        outfields[row][col] = UNREVEALED;
        (*AmountOfFlags)++;

        // als het geflagde vakje een mijn bevat, dan wordt het aantal mijnen opnieuw vermeerderd.
        if (is_mine(fields[row][col]))
            (*AmountofMines)++;
    }
    else if (*AmountOfFlags == 0)
        // als de gebruiker al zijn vlaggen heeft gebruikt, kan hij geen vlaggen meer zetten.
        printf("You don't have any more flags\n");
    else
    {
        outfields[row][col] = 'F';
        (*AmountOfFlags)--;
        // als het vakje inderdaad een mijn bevat, verminder het aantal nog aanwezige mijnen.
        if (is_mine(fields[row][col]))
        {
            (*AmountofMines)--;
        }
    }
}

int get_operands(int operands[])
{

    return scanf("%d %d", &operands[INPUT_ROW], &operands[INPUT_COL]);
}

// alle overbodige input na een error wordt genegeerd.
void consume_wrong_input(void)
{
    for (;;)
    {
        char c = getchar();
        if (c == '\n')
        {
            break;
        }
    }
}

// een error genereren bij het ingeven van foute commando's of bij het geven van verkeerd aantal argumenten.
void wrong_input(char errormessage[], char *operator, int operands[])
{

    printf("Error: %s\n", errormessage);
    consume_wrong_input();
    command_prompt(operator, operands);
}

void command_prompt(char *operator, int operands[])
{
    printf("Enter a command: ");
    scanf(" %c", operator);

    switch (*operator)
    {
    case 'R':
    case 'F':
        // als scanf geen 2 succesvolle assignments heeft gedaan, dan worden de operanden niet correct ingegeven.
        if (get_operands(operands) != 2)
        {
            wrong_input("Wrong type of arguments given.", operator, operands);
        }
        break;
    case 'P':
    case 'Q':
        break;
    default:
        wrong_input("Unknown command.", operator, operands);
    }
}

int is_out_of_boundaries(int operands[])
{
    return (operands[INPUT_ROW] > (NUMBEROFROWS - 1) || operands[INPUT_COL] > (NUMBEROFCOLS - 1));
}

// de gebruikerinput toepassen op het spel
void process_command(char *operator, int operands[], int fields[NUMBEROFROWS][NUMBEROFCOLS], int outfields[NUMBEROFROWS][NUMBEROFCOLS], int *AmountOfNotMines, int *AmountofFlags, int *AmountOfMines, int *exitflag)
{
    switch (*operator)
    {

    case 'R':
        if (is_out_of_boundaries(operands))
        {
            printf("Error: The given coordinates are too good to be true.\n");
            break;
        }
        // als het vakje een mijn bevatte is het spel klaar en heeft de speler verloren.
        if (is_mine(fields[operands[INPUT_ROW]][operands[INPUT_COL]]))
        {
            gameover(exitflag);
        }
        else
        {
            reveal(fields, outfields, operands[INPUT_ROW], operands[INPUT_COL], AmountOfNotMines, AmountofFlags, AmountOfMines);
        }

        break;

    case 'F':
        if (is_out_of_boundaries(operands))
        {
            printf("The given coordinates are too good to be true.\n");
            break;
        }
        set_flag(fields, outfields, operands[INPUT_ROW], operands[INPUT_COL], AmountofFlags, AmountOfMines);
        break;

    case 'P':
        print_game(fields, AmountofFlags);
        break;

    case 'Q':
        *exitflag = 1;
        break;
    }
}

// het eerst ingevoerde commando wordt apart afgehandeld.
// Ik ga er van uit dat: eersrte beurt = eerste commando dat de gebruiker ingeeft.
void process_first_command(char *operator, int operands[], int fields[NUMBEROFROWS][NUMBEROFCOLS], int outfields[NUMBEROFROWS][NUMBEROFCOLS], int *AmountOfNotMines, int *AmountofFlags, int *AmountOfMines, int *exitflag)
{
    switch (*operator)
    {
    case 'R':
        if (is_out_of_boundaries(operands))
        {
            printf("The given coordinates are too good to be true.\n");
            break;
        }
        if (is_mine(fields[operands[INPUT_ROW]][operands[INPUT_COL]]))
            swap_mine_with_not_mine(fields, operands[INPUT_ROW], operands[INPUT_COL]);
        break;
    case 'P':
        break;
    // de resterende commando's worden afgehandeld zoals de gewone.
    default:
        process_command(operator, operands, fields, outfields, AmountOfNotMines, AmountofFlags, AmountOfMines, exitflag);
    }
}

// swap een vakje dat een mijn bevat met een ander vakje dat geen bevat. Dit wordt gebruikt bij eerste commando van de gebruiker
// indien de gebruiker R r c ingeeft.
void swap_mine_with_not_mine(int fields[NUMBEROFROWS][NUMBEROFCOLS], int row, int col)
{
    int found = 0;
    // we beginnen op een willekeurige rij van het spelveld.
    for (int i = myrandom(0, NUMBEROFROWS - 1); i < NUMBEROFROWS; i++)
    {
        if (found)
            break;
        else
            for (int j = 0; j < NUMBEROFCOLS; j++)
            {
                if (is_init_not_mine(fields[i][j]))
                {
                    fields[i][j] = MINE;
                    fields[row][col] = INIT_NOT_MINE;
                    found = 1;
                    break;
                }
            }
    }
}

// als er geen naburige mijnen zijn, worden de naburige vakjes onthuld en wordt hetzelfde toegepast op de naburige vakjes.
void reveal_surroundings(int fields[NUMBEROFROWS][NUMBEROFCOLS], int outfields[NUMBEROFROWS][NUMBEROFCOLS], int row, int col, int *AmountOfNotMines, int *AmountOfFlags, int *AmountOfMines)
{
    // voor alle rijen vanaf de rij boven het element tot de rij onder het element.
    for (int row_offset = -1; row_offset < 2; row_offset++)
    {
        // voor alle kolommen vanaf de kolom links van het element tot de kolom rechts van het element.
        for (int col_offset = -1; col_offset < 2; col_offset++)
        {
            // de randgevallen: wanneer het vakje aan de rand van het spel zit
            if ((row_offset == col_offset) && (row_offset == 0))
                continue;
            else if ((row_offset + row) < 0 || (row_offset + row >= NUMBEROFROWS))
                continue;
            else if ((col_offset + col) < 0 || (col_offset + col >= NUMBEROFCOLS))
                continue;

            // als er een vlag is in het vakje, dan moet je het eerst deflaggen.
            if (is_flag(outfields[row_offset + row][col_offset + col]))
                set_flag(fields, outfields, row_offset + row, col_offset + col, AmountOfFlags, AmountOfMines);
            // als het vakje al onthuld is, dan moet je het niet opnieuw onthullen.
            if (is_unrevealed(outfields[row_offset + row][col_offset + col]))
                reveal(fields, outfields, row_offset + row, col_offset + col, AmountOfNotMines, AmountOfFlags, AmountOfMines);
        }
    }
}

// de inhoud van het vakje tonen.
void reveal(int fields[NUMBEROFROWS][NUMBEROFCOLS], int outfields[NUMBEROFROWS][NUMBEROFCOLS], int row, int col, int *AmountOfNotMines, int *AmountOfFlags, int *AmountOfMines)
{
    // als er een flag op het vakje stond, dan wordt de flag eerst verwijderd.
    if (is_flag(outfields[row][col]))
    {
        set_flag(fields, outfields, row, col, AmountOfFlags, AmountOfMines);
    }

    if (is_unrevealed(outfields[row][col]))
    {
        outfields[row][col] = fields[row][col];
        (*AmountOfNotMines)--;
    }

    // onthul naburige vakjes als het vakje geen naburige mijnen heeft.
    if (fields[row][col] == 0)
    {
        reveal_surroundings(fields, outfields, row, col, AmountOfNotMines, AmountOfFlags, AmountOfMines);
    }
}

// tellen hoeveel mijnen er zijn rond elk vakje dat geen mijn bevat.
int how_many_surrounding_mines(int fields[NUMBEROFROWS][NUMBEROFCOLS], int row, int col)
{
    int res = 0;
    for (int row_offset = -1; row_offset < 2; row_offset++)
    {
        for (int col_offset = -1; col_offset < 2; col_offset++)
        {
            // je hebt het element zelf vast => skip
            if ((row_offset == col_offset) && (row_offset == 0))
                continue;
            // randgeval voor de rijen
            else if ((row_offset + row) < 0 || (row_offset + row >= NUMBEROFROWS))
                continue;
            // randgeval voor de kolommen
            else if ((col_offset + col) < 0 || (col_offset + col >= NUMBEROFCOLS))
                continue;

            if (is_mine(fields[row_offset + row][col_offset + col]))
                res++;
        }
    }
    return res;
}

// willekeurige keuze tussen een mijn of een niet mijn
int mine_or_yours(void)
{
    return (myrandom(0, MINES_RATIO) == 0) ? MINE : INIT_NOT_MINE;
}

// het spelveld vullen met mijnen.
void fill_fields(int fields[NUMBEROFROWS][NUMBEROFCOLS], int *AmountOfMines)
{
    for (int i = 0; i < NUMBEROFROWS; i++)
    {
        for (int j = 0; j < NUMBEROFCOLS; j++)
        {
            fields[i][j] = mine_or_yours();
            if (is_mine(fields[i][j]))
            {
                (*AmountOfMines)++;
            }
        }
    }
}

// alle vakjes die geen mijnen bevatten invullen met het aantal naburige mijnen.
void initialize_fields(int fields[NUMBEROFROWS][NUMBEROFCOLS])
{
    for (int i = 0; i < NUMBEROFROWS; i++)
    {
        for (int j = 0; j < NUMBEROFCOLS; j++)
        {
            if (is_init_not_mine(fields[i][j]))
                fields[i][j] = how_many_surrounding_mines(fields, i, j);
        }
    }
}

// het spelveld dat geprint gaat worden initializeren.
void fill_outfields(int outfields[NUMBEROFROWS][NUMBEROFCOLS])
{
    for (int i = 0; i < NUMBEROFROWS; i++)
    {
        for (int j = 0; j < NUMBEROFCOLS; j++)
        {
            outfields[i][j] = UNREVEALED;
        }
    }
}

void initialize_game(char *operator, int operands[], int fields[NUMBEROFROWS][NUMBEROFCOLS], int outfields[NUMBEROFROWS][NUMBEROFCOLS], int *AmountOfNotMines, int *AmountOfFlags, int *AmountOfMines, int *exitflag)
{
    fill_outfields(outfields);
    fill_fields(fields, AmountOfMines);
    *AmountOfNotMines = (NUMBEROFROWS * NUMBEROFCOLS) - *AmountOfMines;
    *AmountOfFlags = *AmountOfMines;

    // de eerste beurt
    print_intro();
    print_game(outfields, AmountOfFlags);
    command_prompt(operator, operands);
    // eerste beurt wordt apart afgehandeld: Dit is om ervoor te zorgen dat de speler niet meteen na de eerste beurt verliest.
    process_first_command(operator, operands, fields, outfields, AmountOfNotMines, AmountOfFlags, AmountOfMines, exitflag);

    initialize_fields(fields);

    // dit moet gebeuren nadat het spelveld met de spellogica is geinitializeerd.
    if (*operator== 'R' && !is_out_of_boundaries(operands))
    {
        reveal(fields, outfields, operands[INPUT_ROW], operands[INPUT_COL], AmountOfNotMines, AmountOfFlags, AmountOfMines);
    }
    else if (*operator== 'P')
    {
        print_game(fields, AmountOfFlags);
    }
}

void minesweeper(void)
{
    // declaratie tweedimensionaal speelveld (spellogica)
    int fields[NUMBEROFROWS][NUMBEROFCOLS];
    // wat er geprint gaat worden. (tekenlogica)
    int outfields[NUMBEROFROWS][NUMBEROFCOLS];

    char operator;
    int operands[2];

    int AmountOfMines = 0;
    int AmountOfNotMines, AmountOfFlags;

    // exitflag geeft aan dat het spel gedaan is (wanneer de speler wint/verliest/commando Q ingeeft)
    int exitflag = 0;

    initialize_game(&operator, operands, fields, outfields, &AmountOfNotMines, &AmountOfFlags, &AmountOfMines, &exitflag);

    // De spellus
    for (;;)
    {

        if (exitflag)
            break;
        // eerste fase: het spelveld uitprinten
        print_game(outfields, &AmountOfFlags);
        // tweede fase: de speler vragen om input in te geven en errors in de input detecteren.
        command_prompt(&operator, operands);
        // derde fase: de input van de gebruiker analyseren en uitvoeren
        // fouten in de logica van de input detecteren (bvb. coordinaten buiten de grenzen).
        process_command(&operator, operands, fields, outfields, &AmountOfNotMines, &AmountOfFlags, &AmountOfMines, &exitflag);
        if (is_gamewon(&AmountOfNotMines, &AmountOfMines))
        {
            print_game(outfields, &AmountOfFlags);
            gameWon(&exitflag);
        }
    }
}