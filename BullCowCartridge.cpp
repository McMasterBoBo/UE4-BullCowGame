/*
    Game Instruction: 
    
        The bull cow game is to guess a hidden word. If the word is guessed right you will win the game.
        Otherwise, there will be a hint by showing how many bulls and cows to let you try guessing the 
        answer. The number of bulls means your answer have letters that their index number are same as the 
        hidden word. The number of cows means your answer have letters that are same as the hidden word but
        different index numbers. Please see the examples below:

        Example 1:
        User Input: dog
        Hidden Word: god
        Output: Wrong answer, you have 1 bulls, 2 cows

        Example 2:
        User Input: tank
        Hidden Word: bank
        Output: Wrong answer, you have 3 bulls, 0 cows

        Constrains: The answer and the hidden word should not have repeating letters, no numbers are allowed. 

        The length of the hidden word represents the number of chances for you to guess, think carefully and have fun!

*/
#include "BullCowCartridge.h"
#include "CoreMinimal.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay()  //When the game starts
{
    Super::BeginPlay();

    //Debug line    
    //PrintLine(TEXT("The number of valid word is %i"), GetValidWords(Words).Num());
    
    Isograms = GetValidWords(Words); // Pass the array function into an array, which is used to check the hidden word whether has repeating letters

    // Setting Up Game
    SetupGame();
 
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);       
    }
    
}

void UBullCowCartridge::SetupGame() // Game Setup
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; // Set the HiddenWord
    Lives = HiddenWord.Len(); // Set Lives
    bGameOver = false;

    // Welcoming The Player 
    PrintLine(TEXT("Welcome to the Bull Cow Game!\n"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len()); // Magic Number Remove!
    PrintLine(TEXT("You have %i lives in total."), Lives);
    PrintLine(TEXT("These words are animals that we usually \nsee in TV or daliy life"));
    PrintLine(TEXT("Type in your guess. \nPress enter to continue..."));

}

void UBullCowCartridge::EndGame() // use a function to loop the game
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again!"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) // Game Process
{
    if (Guess == HiddenWord) // Check if it's the right answer
    {
        PrintLine(TEXT("You Win!"));
        EndGame();
        return;
    }

    // Check If there are repeating letters
    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    // Check Right Number of characters
    if (Guess.Len() != HiddenWord.Len())
    {                    
        PrintLine(TEXT("The answer is %i letters long."), HiddenWord.Len());
        PrintLine(TEXT("You won't lose life.\nTry to guess again!"));
        PrintLine(TEXT("You still have %i lives left."), Lives);
        return;                
    }
    
    // Remove Life
    PrintLine(TEXT("\nWrong answer, you just loose a live!"));
    --Lives;

    // Check If lives > 0
    if(Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("Game Over!"));
        PrintLine(TEXT("The hidden word was: %s\n"), *HiddenWord);
        EndGame();
        return;
    }

    // Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const //Isogram Function to check
{
    for (int32 i = 0, b = i+1; b < Word.Len(); b++)
    {
       if(Word[i] == Word[b])
       {
           return false;
       }
    }

    return true;
}

// TArray function to get valid hidden words
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Words) const
{
    TArray<FString> ValidWords;

    for(FString Word : Words)
    {
        if (Word.Len() >= 3 && Word.Len() <= 8)
        {
            if(IsIsogram(Word))
            {
                ValidWords.Emplace(Word);
            }           
        }  
    }

    return ValidWords;
}

// Structure Function to get bulls and cows
FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    // for the index guess is same as index Hidden. bullcount ++
    // if not a bull was it a cow? if yes cowcount ++

    for (int32 i = 0; i < Guess.Len(); i++) // O(n)
    {
        if(Guess[i] == HiddenWord[i]) // Check if every index position is same as the hidden word
        {
            Count.Bulls++;
            continue;
        }

        for (int32 j = 0; j < HiddenWord.Len(); j++) // O(n)
        {
            if(Guess[i] == HiddenWord[j]) // Check if every index is in the hidden word but different position
            {
                Count.Cows++;
                break;
            }
        }
        
    }
    return Count;
}