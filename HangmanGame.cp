 /*
 Description: Allows multiple users to play a hangman game with words taken from an input file, and stores game results in an output file.
 */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

class gameHangman
{
public:
string solutionWord; //solution word
char starWord[20] = "";//hint word made of *
char letters[20] = "";//letters guessed so far (right or wrong)
int letterCount = 0;//number of letters guessed so far (right or wrong)
char letterGuess = ' ';//current letter guess
int incorrectCount = 0; //keep track of the number of incorrect guesses
int maxGuesses = 6; //six guesses for hangman
int wordLength = 0;//length of the solution word
bool correctLetter = false;//flag to set if the letter is in the word
string name; //name of the user
bool wonOrLost = 0; //true or false for the current round of the game
};

//Precondition: User has started the program.
//Postcondition: The rules are displayed to the user.
void HangmanRules();

//Precondition: The user selects yes when asked if they would like to play.
//Postcondition: One full round is played and ends when the user has guessed the word correctly, or has guessed incorrecly 6 times.
void PlayOneRound(ifstream&, const string&, gameHangman&);

//Precondition: The user has selected yes to playing a round.
//Postcondition: Resets all the game objects, finds the solution word, and calls the function used to create the hint word.
void SetUpOneRound(ifstream&, gameHangman&);

//Precondition: The user has selected yes to playing the game and the SetUpRound function is used.
//Postcondition: Creats a hint word full of the '*' character.
void CreateStarArray(gameHangman&);

//Precondition: The user has selected yes to playing the game and is ready to input a guess.
//Postcondition: The user has entered a guess.
void GetLetterGuess(gameHangman&);

//Precondition: The user has entered a guess.
//Postcondition: Adds the guess to the array of letters guessed.
void UpdateLettersArray(gameHangman&);
 
//Precondition: The user has entered a guess.
//Postcondition: Determines whether the guess was part of the word and returns true if it is.
bool CorrectLetterGuess(gameHangman&);
 
//Precondition: The user has entered a guess.
//Postcondition: Adds a body part for incorrect guesses made.
void DrawHangman(gameHangman&);
 
//Precondition: The user has played the game, and has chosen to exit the game.
//Postcondition: Prints game results onto the display, and stores results in an output file.
void PrintPlayerResults(vector <gameHangman> gameList, int);

int main()
{
    string yesOrno = "y"; //String to determine whether the user wants to play or not.
    gameHangman currentPlayer; //Current player of the current round.
    vector <gameHangman> gameList; //vector of game objects.
    int gameCount = 0; // determines which round is being played.
    
   // displays all rules of the hangman game.
    HangmanRules();

    //connecting to and opening an input file.
    ifstream in;
    in.open("words.txt");
    
    // exits in case input file does not properly open
    if(in.fail())
    {
        cout << "Could not connect to the input file. " << endl;
        exit(1);
    }
    
    //prompting user to see if they want to play
    cout << "Would you like to play? ('Y(y)' or 'N(n)') " << endl;
    cin >> yesOrno;
    
    //loop to continue playing while the user says y or Y.
    while(yesOrno != "n" && yesOrno != "N")
    {
        cout << "Great! Please enter your first name: " << endl;
        cin >> currentPlayer.name; //storing player's name.
        
        gameList.push_back(currentPlayer); //storing the name in the vector.
        
        PlayOneRound(in, gameList[gameCount].name, gameList[gameCount]); // calling the play one round function.
        gameCount++; //adds one to the gamecount after a round is played.
        
        //prompting user to see if they would like to continue.
        cout << "Would you like to play again? ('Y(y)' or 'N(n)') " << endl;
        cin >> yesOrno;
    }
    
    cout << "Thanks for playing! " << endl;
    PrintPlayerResults(gameList, gameCount); //displays results to the screen and in the output file.
    return 0;
}

//Description: Displays the rules of the game on the screen.
void HangmanRules()
{
    cout << "\t Welcome to the hangman game! " << endl;
    cout << "Please read the following instructions before playing: " << endl;
    cout << "-You will be given a word to try and guess correctly. " << endl;
    cout << "-You can only make up to 6 incorrect guesses. " << endl;
    cout << "-Guesses have to be made one letter at a time. " << endl;
    cout << "-You may guess the entire word after you have correctly guessed one letter. " << endl;
    cout << "-The game ends if you guess the word correctly or make 6 incorrected guesses. " << endl;
    cout << "\t Have fun playing! " << endl;
    
}

//Description: Plays one round of the hangman game and determines whether the round was won or lost.
void PlayOneRound(ifstream& in, const string& name, gameHangman& currentGame)
{
    SetUpOneRound(in, currentGame); //calling for the round to be reset and set up.
    
    while(currentGame.incorrectCount < currentGame.maxGuesses && currentGame.wonOrLost == false)
    {
        GetLetterGuess(currentGame); //calling for user to enter a guess.
        UpdateLettersArray(currentGame); //calling for the guess to be added to the letters array.
        
        if(CorrectLetterGuess(currentGame) == true)
        {
            cout << "The letter was in the word! " << endl; // Lets user know a correct guess was made.
            if(currentGame.starWord == currentGame.solutionWord)
            {
                cout << "Congratulations! You won that round! " << endl;
                currentGame.wonOrLost = true;
            }
        }else
        {
            cout << "The letter was not in the word. " << endl; //letter user know an incorrect guess was made.
            cout << "You added a body part! " << endl;
            currentGame.incorrectCount++; // adds 1 to the incorrect guess count.
            DrawHangman(currentGame); // calls for a body part to be added for the incorrect guess.
        }
    }
    cout << "The solution was " << currentGame.solutionWord << "." << endl; // displays the solution word.
    cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
}

//Description: Resets game objects for a new one and finds the solution word. Calls for star array to be created.
void SetUpOneRound(ifstream& in, gameHangman& currentGame)
{
    currentGame.solutionWord = "";//clear the solution word
    strcpy(currentGame.starWord, ""); //clear the hint word
    strcpy(currentGame.letters, ""); //clear letters guessed
    currentGame.letterCount = 0; //letter count starts at 0
    currentGame.letterGuess = ' ';
    currentGame.incorrectCount = 0;
    currentGame.maxGuesses = 6;
    currentGame.wordLength = 0;
    currentGame.correctLetter = false; //reset to false
    currentGame.wonOrLost = false; //reset to false
    
    // finds the solution word from the input file.
    in >> currentGame.solutionWord;
    
    CreateStarArray(currentGame); // calling the star word array to be created.
}

//Description: Creates an array that is filled with the '*' character. Star word is used as a hint for the user.
void CreateStarArray(gameHangman& currentGame)
{
    currentGame.wordLength = (int)currentGame.solutionWord.length();
    
    int i;
    
    //this loop creates the star word array
    for(i = 0; i < currentGame.wordLength; i++)
    {
        currentGame.starWord[i] = '*';
    }
    currentGame.starWord[currentGame.wordLength] = '\0'; // adding null character
    
}

//Description: Prompts user to enter a guess while also showing them the star word and the letters guessed.
void GetLetterGuess(gameHangman& currentGame)
{
    cout << endl << "Guess this word: " << currentGame.starWord << endl;
    
    cout << "Letters Guessed: " << currentGame.letters << endl;
    
    //prompting user for guess.
    cout << "Enter a letter: " << endl;
    cin >> currentGame.letterGuess;
}

//Description: Adds letters guessed by the user to the letters array.
void UpdateLettersArray(gameHangman& currentGame)
{
    currentGame.letters[currentGame.letterCount] = currentGame.letterGuess;
    currentGame.letterCount++;
    currentGame.letters[currentGame.letterCount] = '\0'; // adding null character.
}
 
//Description: Determines whether or not a guess was in the solution.
bool CorrectLetterGuess(gameHangman& currentGame)
{
    bool rv = false;
    int i;
    
    // this loops set to true if the guess was part of the solution
    for(i = 0; i < currentGame.wordLength; i++)
    {
        if(currentGame.letterGuess == currentGame.solutionWord[i])
        {
            rv = true;
            currentGame.starWord[i] = currentGame.letterGuess;
        }
    }
    return rv;
}
 
//Description: Adds body parts for incorrect guesses.
void DrawHangman(gameHangman& currentGame)
{
    cout << "\n___________________________________________________\n";
    cout << " O-|--< O-|--< O-|--< O-|--< O-|--< O-|--< O-|--< ";
    cout << "\n___________________________________________________\n";
    
    // this loop adds body parts depending on the amount of incorrect guesses.
    if(currentGame.incorrectCount == 0)
    {
        cout << "\n You have no incorrect guesses! ";
    }else if (currentGame.incorrectCount == 1)
    {
        cout << "\n HEAD \n - 1 of 6 incorrect guesses! ";
    }else if (currentGame.incorrectCount == 2)
    {
        cout << "\n HEAD \n BODY \n - 2 of 6 incorrect guesses! ";
    }else if (currentGame.incorrectCount == 3)
    {
        cout << "\n HEAD \n BODY \n ARM1 \n- 3 of 6 incorrect guesses! ";
    }else if (currentGame.incorrectCount == 4)
    {
        cout << "\n HEAD \n BODY \n ARM1 \n ARM2 \n - 4 of 6 incorrect guesses! ";
    }else if (currentGame.incorrectCount == 5)
    {
        cout << "\n HEAD \n BODY \n ARM1 \n ARM2 \n LEG1 \n - 5 of 6 incorrect guesses! ";
    }else if (currentGame.incorrectCount == 6)
    {
        cout << "\n HEAD \n BODY \n ARM1 \n ARM2 \n LEG1 \n LEG2 \n - 6 of 6 incorrect guesses! " << endl;
        cout << "You lost that round! " << endl;
    }

}
 
//Description: Prints game results on the display and into an output file.
void PrintPlayerResults(vector <gameHangman> gameList, int gameCount)
{
    
    //Object for the ouput file
    ofstream out;
    
    //Initializing the output file and asking the user to input the name of it.
    char filename[40];
    cout << "Enter the file name(with .txt extension): " << endl;
    cin >> filename;
    out.open(filename, ios::app);
    
    cout << "\n *********************************** \n";
    cout << "Name: \t" << "Won or Lost: \t" << "Incorrect Count: " << endl;
    
    //This for loop prints out info to the screen
    for(int i = 0; i < gameCount; i++)
    {
        cout << gameList[i].name << " \t " << gameList[i].wonOrLost << " \t " << gameList[i].incorrectCount << endl;
    }
    
    out << "\n *********************************** \n";
    out << "Name: \t" << "Won or Lost: \t" << "Incorrect Count: " << endl;
    
    //This for loop prints out info to the output file
    for(int i = 0; i < gameCount; i++)
    {
        out << gameList[i].name <<  " \t "  << gameList[i].wonOrLost <<  " \t "  << gameList[i].incorrectCount << endl;
    }

    out.close();
}
    


