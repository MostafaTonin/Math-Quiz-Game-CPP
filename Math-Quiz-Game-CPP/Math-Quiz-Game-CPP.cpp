/*****************************************************************************************
*                               Math Quiz Game
*
*  Author      : Mostafa Tonin
*  Language    : C++
*  Description : Console Math Quiz Game
*
*  This project generates random mathematical questions based on:
*      - Difficulty Level
*      - Operation Type
*
*  The player answers the questions and receives a final score.
*
*****************************************************************************************/


//==========================================================
// Libraries
//==========================================================

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#define NOMINMAX
#include <windows.h>

using namespace std;


//==========================================================
// Constants
//==========================================================

const int MAX_QUESTIONS = 100;
const int LABEL_WIDTH = 24;

const string SEPARATOR_LINE = "==========================================================";

const WORD WHITE_TEXT =
FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

const WORD BLACK_TEXT = 0;

const WORD DEFAULT_COLOR = WHITE_TEXT;
const WORD SUCCESS_COLOR = BACKGROUND_GREEN | WHITE_TEXT;
const WORD ERROR_COLOR = BACKGROUND_RED | WHITE_TEXT;
const WORD RESULT_COLOR = BACKGROUND_RED | BACKGROUND_BLUE | WHITE_TEXT;
const WORD QUESTION_COLOR = BACKGROUND_GREEN | BACKGROUND_BLUE | WHITE_TEXT;
const WORD MENU_COLOR = BACKGROUND_BLUE | WHITE_TEXT;

const WORD ADDITION_COLOR = BACKGROUND_BLUE | WHITE_TEXT;
const WORD SUBTRACTION_COLOR = BACKGROUND_RED | BACKGROUND_GREEN | BLACK_TEXT;
const WORD MULTIPLICATION_COLOR = BACKGROUND_RED | BACKGROUND_BLUE | WHITE_TEXT;
const WORD DIVISION_COLOR = BACKGROUND_GREEN | BACKGROUND_BLUE | WHITE_TEXT;
const WORD MIXED_COLOR = BACKGROUND_INTENSITY | WHITE_TEXT;

WORD CurrentConsoleColor = DEFAULT_COLOR;


//==========================================================
// Enums
//==========================================================

enum enQuestionsLevel
{
    EasyLevel = 1,
    MiddleLevel = 2,
    HardLevel = 3,
    MixedLevel = 4
};

enum enOperationType
{
    Addition = 1,
    Subtraction = 2,
    Multiplication = 3,
    Division = 4,
    Mixed = 5
};


//==========================================================
// Structures
//==========================================================

struct stQuestionInfo
{
    int QuestionNumber = 0;

    int Number1 = 0;
    int Number2 = 0;

    enQuestionsLevel QuestionLevel = EasyLevel;
    enOperationType QuestionOperationType = Addition;

    int CorrectResult = 0;
    int PlayerResult = 0;

    bool IsRightAnswer = true;
};

struct stQuizInfo
{
    stQuestionInfo Questions[MAX_QUESTIONS];

    int QuestionsNumber = 0;

    enQuestionsLevel QuizzLevel = EasyLevel;
    enOperationType QuizzOperationType = Addition;

    int RightAnswersNumber = 0;
    int WrongAnswersNumber = 0;

    bool IsPass = true;
};


//==========================================================
// Function Declarations
//==========================================================

void ShowWelcomeScreen();
void AskQuestion(const stQuestionInfo& Question, int TotalQuestions);
void ShowAnswerResult(bool IsCorrect, int CorrectAnswer);
void ShowGameOverScreen();
void PrintQuizzInfo(const stQuizInfo& QuizInfo);
void PrintQuestionsInfo(const stQuestionInfo Questions[], int QuestionsNumber);
void ShowGoodbyeScreen();


//==========================================================
// Utility Functions
//==========================================================

//----------------------------------------------------------
// Function: SetConsoleColor
// Purpose: Changes the console color immediately by using Windows API.
// Parameters: ColorAttribute - The foreground and background color values.
// Returns: Nothing.
//----------------------------------------------------------
void SetConsoleColor(WORD ColorAttribute)
{
    HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(ConsoleHandle, ColorAttribute);

    CurrentConsoleColor = ColorAttribute;
}

//----------------------------------------------------------
// Function: SetDefaultColor
// Purpose: Restores the default console color.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void SetDefaultColor()
{
    SetConsoleColor(DEFAULT_COLOR);
}

//----------------------------------------------------------
// Function: SetSuccessColor
// Purpose: Sets the console color used for correct answers and passing results.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void SetSuccessColor()
{
    SetConsoleColor(SUCCESS_COLOR);
}

//----------------------------------------------------------
// Function: SetErrorColor
// Purpose: Sets the console color used for wrong answers and invalid input.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void SetErrorColor()
{
    SetConsoleColor(ERROR_COLOR);
}

//----------------------------------------------------------
// Function: SetQuestionColor
// Purpose: Sets the console color used while displaying questions.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void SetQuestionColor()
{
    SetConsoleColor(QUESTION_COLOR);
}

//----------------------------------------------------------
// Function: SetResultColor
// Purpose: Sets the console color used for final result screens.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void SetResultColor()
{
    SetConsoleColor(RESULT_COLOR);
}

//----------------------------------------------------------
// Function: SetMenuColor
// Purpose: Sets the console color used for menus and titles.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void SetMenuColor()
{
    SetConsoleColor(MENU_COLOR);
}

//----------------------------------------------------------
// Function: SetOperationBackgroundColor
// Purpose: Sets a different background color for each math operation.
// Parameters: OperationType - The operation used in the current question.
// Returns: Nothing.
//----------------------------------------------------------
void SetOperationBackgroundColor(enOperationType OperationType)
{
    switch (OperationType)
    {
    case Addition:
        SetConsoleColor(ADDITION_COLOR);
        break;

    case Subtraction:
        SetConsoleColor(SUBTRACTION_COLOR);
        break;

    case Multiplication:
        SetConsoleColor(MULTIPLICATION_COLOR);
        break;

    case Division:
        SetConsoleColor(DIVISION_COLOR);
        break;

    case Mixed:
        SetConsoleColor(MIXED_COLOR);
        break;

    default:
        SetQuestionColor();
    }
}

//----------------------------------------------------------
// Function: ResetScreen
// Purpose: Clears the console screen and restores the default color.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void ResetScreen()
{
    SetDefaultColor();
    system("cls");
}

//----------------------------------------------------------
// Function: PrintSeparator
// Purpose: Prints a separator line to make the console output cleaner.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void PrintSeparator()
{
    cout << SEPARATOR_LINE << "\n";
}

//----------------------------------------------------------
// Function: PrintTitle
// Purpose: Prints a simple title between separator lines.
// Parameters: Title - The title to display.
// Returns: Nothing.
//----------------------------------------------------------
void PrintTitle(string Title)
{
    PrintSeparator();
    cout << "\t\t" << Title << "\n";
    PrintSeparator();
}

//----------------------------------------------------------
// Function: PrintInfoLine
// Purpose: Prints a label and value in a clean two-column format.
// Parameters: Label - The text before the colon, Value - The text after it.
// Returns: Nothing.
//----------------------------------------------------------
void PrintInfoLine(string Label, string Value)
{
    cout << left << setw(LABEL_WIDTH) << Label
        << ": "
        << Value
        << right << "\n";
}

//----------------------------------------------------------
// Function: WaitForEnter
// Purpose: Pauses the program until the player presses ENTER.
// Parameters: Message - The message shown before waiting.
// Returns: Nothing.
//----------------------------------------------------------
void WaitForEnter(string Message)
{
    cout << "\n" << Message;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

//----------------------------------------------------------
// Function: GetOperationSymbol
// Purpose: Converts an operation type to its mathematical symbol.
// Parameters: OperationType - The selected operation type.
// Returns: The symbol that represents the operation.
//----------------------------------------------------------
char GetOperationSymbol(enOperationType OperationType)
{
    switch (OperationType)
    {
    case Addition:
        return '+';

    case Subtraction:
        return '-';

    case Multiplication:
        return '*';

    case Division:
        return '/';

    default:
        return '?';
    }
}

//----------------------------------------------------------
// Function: GetLevelText
// Purpose: Converts a question level to readable text.
// Parameters: Level - The selected question level.
// Returns: The level name as text.
//----------------------------------------------------------
string GetLevelText(enQuestionsLevel Level)
{
    switch (Level)
    {
    case EasyLevel:
        return "Easy";

    case MiddleLevel:
        return "Middle";

    case HardLevel:
        return "Hard";

    case MixedLevel:
        return "Mixed";

    default:
        return "Unknown";
    }
}

//----------------------------------------------------------
// Function: GetOperationTypeText
// Purpose: Converts an operation type to readable text.
// Parameters: OperationType - The selected operation type.
// Returns: The operation name as text.
//----------------------------------------------------------
string GetOperationTypeText(enOperationType OperationType)
{
    switch (OperationType)
    {
    case Addition:
        return "Addition (+)";

    case Subtraction:
        return "Subtraction (-)";

    case Multiplication:
        return "Multiplication (*)";

    case Division:
        return "Division (/)";

    case Mixed:
        return "Mixed";

    default:
        return "Unknown";
    }
}

//----------------------------------------------------------
// Function: GetQuizzResultText
// Purpose: Converts the final pass/fail value to readable text.
// Parameters: IsPass - True if the player passed, false otherwise.
// Returns: PASS or FAIL.
//----------------------------------------------------------
string GetQuizzResultText(bool IsPass)
{
    if (IsPass)
        return "PASS";
    else
        return "FAIL";
}

//----------------------------------------------------------
// Function: GetQuestionResultText
// Purpose: Converts a question result to readable text.
// Parameters: Result - True if the answer is correct, false otherwise.
// Returns: Correct or Wrong.
//----------------------------------------------------------
string GetQuestionResultText(bool Result)
{
    if (Result)
        return "Correct";
    else
        return "Wrong";
}


//==========================================================
// Input Functions
//==========================================================

//----------------------------------------------------------
// Function: ClearInputBuffer
// Purpose: Clears invalid input so the next read can work correctly.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void ClearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//----------------------------------------------------------
// Function: PrintInputError
// Purpose: Displays a clear input error message.
// Parameters: Message - The error message to display.
// Returns: Nothing.
//----------------------------------------------------------
void PrintInputError(string Message)
{
    WORD PreviousColor = CurrentConsoleColor;

    SetErrorColor();

    cout << "\nInvalid Input!\n";
    cout << Message << "\n";

    SetConsoleColor(PreviousColor);
}

//----------------------------------------------------------
// Function: ReadInteger
// Purpose: Reads an integer and handles non-numeric input.
// Parameters: Message - The prompt shown to the player.
// Returns: The integer entered by the player.
//----------------------------------------------------------
int ReadInteger(string Message)
{
    int Number = 0;

    cout << "\n" << Message;
    cin >> Number;

    while (cin.fail())
    {
        ClearInputBuffer();
        PrintInputError("Please enter a valid integer.");

        cout << "\n" << Message;
        cin >> Number;
    }

    return Number;
}

//----------------------------------------------------------
// Function: ReadPositiveInt
// Purpose: Reads a positive integer from the player.
// Parameters: Message - The prompt shown to the player.
// Returns: A positive integer.
//----------------------------------------------------------
int ReadPositiveInt(string Message)
{
    int Number = ReadInteger(Message);

    while (Number <= 0)
    {
        PrintInputError("Please enter a positive number.");
        Number = ReadInteger(Message);
    }

    return Number;
}

//----------------------------------------------------------
// Function: ReadIntInRange
// Purpose: Reads an integer within a specific range.
// Parameters: Message - The prompt shown to the player, From - The minimum value, To - The maximum value.
// Returns: An integer between From and To.
//----------------------------------------------------------
int ReadIntInRange(string Message, int From, int To)
{
    int Number = ReadInteger(Message);

    while (Number < From || Number > To)
    {
        PrintInputError(
            "Please enter a value between "
            + to_string(From)
            + " and "
            + to_string(To)
            + ".");

        Number = ReadInteger(Message);
    }

    return Number;
}

//----------------------------------------------------------
// Function: ReadQuizzLevel
// Purpose: Reads the quiz difficulty level from the player.
// Parameters: None.
// Returns: The selected question level.
//----------------------------------------------------------
enQuestionsLevel ReadQuizzLevel()
{
    SetMenuColor();

    string Message;

    Message += "================ Difficulty Level ================\n\n";
    Message += "1) Easy\n";
    Message += "2) Middle\n";
    Message += "3) Hard\n";
    Message += "4) Mixed\n\n";
    Message += "Your Choice : ";

    return (enQuestionsLevel)ReadIntInRange(Message, 1, 4);
}

//----------------------------------------------------------
// Function: ReadOperationType
// Purpose: Reads the operation type from the player.
// Parameters: None.
// Returns: The selected operation type.
//----------------------------------------------------------
enOperationType ReadOperationType()
{
    SetMenuColor();

    string Message;

    Message += "================ Operation Type ==================\n\n";
    Message += "1) Addition (+)\n";
    Message += "2) Subtraction (-)\n";
    Message += "3) Multiplication (*)\n";
    Message += "4) Division (/)\n";
    Message += "5) Mixed\n\n";
    Message += "Your Choice : ";

    return (enOperationType)ReadIntInRange(Message, 1, 5);
}

//----------------------------------------------------------
// Function: ReadAnswer
// Purpose: Reads the player's answer to the current question.
// Parameters: None.
// Returns: The player's answer.
//----------------------------------------------------------
int ReadAnswer()
{
    return ReadInteger("Your Answer : ");
}

//----------------------------------------------------------
// Function: AskToPlayMore
// Purpose: Asks the player if they want to add more questions.
// Parameters: None.
// Returns: True if the player chooses Yes, false otherwise.
//----------------------------------------------------------
bool AskToPlayMore()
{
    SetMenuColor();

    string Message;

    Message += "Play More?\n\n";
    Message += "1) Yes\n";
    Message += "2) No\n\n";
    Message += "Choice : ";

    return ReadIntInRange(Message, 1, 2) == 1;
}

//----------------------------------------------------------
// Function: AskToShowQuestionsInfo
// Purpose: Asks the player if they want to see all question details.
// Parameters: None.
// Returns: True if the player chooses Yes, false otherwise.
//----------------------------------------------------------
bool AskToShowQuestionsInfo()
{
    SetMenuColor();

    string Message;

    Message += "Show Questions Details?\n\n";
    Message += "1) Yes\n";
    Message += "2) No\n\n";
    Message += "Choice : ";

    return ReadIntInRange(Message, 1, 2) == 1;
}


//==========================================================
// Question Functions
//==========================================================

//----------------------------------------------------------
// Function: RandomIntInRange
// Purpose: Generates a random integer between two values.
// Parameters: From - The minimum value, To - The maximum value.
// Returns: A random integer between From and To.
//----------------------------------------------------------
int RandomIntInRange(int From, int To)
{
    return rand() % (To - From + 1) + From;
}

//----------------------------------------------------------
// Function: GetRandomOperationType
// Purpose: Selects a random operation type for mixed-operation quizzes.
// Parameters: None.
// Returns: A random operation type.
//----------------------------------------------------------
enOperationType GetRandomOperationType()
{
    return (enOperationType)RandomIntInRange(1, 4);
}

//----------------------------------------------------------
// Function: GetRandomQuestionsLevel
// Purpose: Selects a random difficulty level for mixed-level quizzes.
// Parameters: None.
// Returns: A random question level.
//----------------------------------------------------------
enQuestionsLevel GetRandomQuestionsLevel()
{
    return (enQuestionsLevel)RandomIntInRange(1, 3);
}

//----------------------------------------------------------
// Function: NumberAccordingToLevel
// Purpose: Generates a number that matches the selected difficulty level.
// Parameters: Level - The selected question level.
// Returns: A random number for that level.
//----------------------------------------------------------
int NumberAccordingToLevel(enQuestionsLevel Level)
{
    switch (Level)
    {
    case EasyLevel:
        return RandomIntInRange(1, 10);

    case MiddleLevel:
        return RandomIntInRange(11, 50);

    case HardLevel:
        return RandomIntInRange(51, 100);

    case MixedLevel:
        return RandomIntInRange(1, 100);

    default:
        return RandomIntInRange(1, 20);
    }
}

//----------------------------------------------------------
// Function: SimpleCalculator
// Purpose: Calculates the correct answer for one question.
// Parameters: Number1 - First number, Number2 - Second number, OperationType - Operation to apply.
// Returns: The result of the calculation.
//----------------------------------------------------------
int SimpleCalculator(int Number1, int Number2, enOperationType OperationType)
{
    switch (OperationType)
    {
    case Addition:
        return Number1 + Number2;

    case Subtraction:
        return Number1 - Number2;

    case Multiplication:
        return Number1 * Number2;

    case Division:
        if (Number2 == 0)
            return 0;

        return Number1 / Number2;

    default:
        return Number1 + Number2;
    }
}

//----------------------------------------------------------
// Function: MathematicalExpression
// Purpose: Builds the expression text for one math question.
// Parameters: Number1 - First number, Number2 - Second number, OperationType - Operation to show.
// Returns: The expression as text.
//----------------------------------------------------------
string MathematicalExpression(int Number1, int Number2, enOperationType OperationType)
{
    string Expression;

    Expression += to_string(Number1);
    Expression += " ";
    Expression += GetOperationSymbol(OperationType);
    Expression += " ";
    Expression += to_string(Number2);
    Expression += " = ";

    return Expression;
}

//----------------------------------------------------------
// Function: GenerateQuestion
// Purpose: Creates one random question using the selected settings.
// Parameters: Level - Difficulty level, OperationType - Operation type.
// Returns: A complete question with its correct answer.
//----------------------------------------------------------
stQuestionInfo GenerateQuestion(enQuestionsLevel Level, enOperationType OperationType)
{
    stQuestionInfo Question;

    if (Level == MixedLevel)
        Level = GetRandomQuestionsLevel();

    if (OperationType == Mixed)
        OperationType = GetRandomOperationType();

    Question.QuestionLevel = Level;
    Question.QuestionOperationType = OperationType;

    Question.Number1 = NumberAccordingToLevel(Level);
    Question.Number2 = NumberAccordingToLevel(Level);

    Question.CorrectResult =
        SimpleCalculator(
            Question.Number1,
            Question.Number2,
            OperationType);

    return Question;
}

//----------------------------------------------------------
// Function: GenerateNewQuestionsList
// Purpose: Generates the first list of quiz questions.
// Parameters: QuizInfo - The quiz information to update.
// Returns: Nothing.
//----------------------------------------------------------
void GenerateNewQuestionsList(stQuizInfo& QuizInfo)
{
    for (int i = 0; i < QuizInfo.QuestionsNumber; i++)
    {
        QuizInfo.Questions[i] =
            GenerateQuestion(
                QuizInfo.QuizzLevel,
                QuizInfo.QuizzOperationType);

        QuizInfo.Questions[i].QuestionNumber = i + 1;
    }
}

//----------------------------------------------------------
// Function: UpdateQuestionsList
// Purpose: Generates newly added questions after the first round.
// Parameters: QuizInfo - The quiz information to update, NewQuestionsNumber - Number of new questions.
// Returns: Nothing.
//----------------------------------------------------------
void UpdateQuestionsList(stQuizInfo& QuizInfo, int NewQuestionsNumber)
{
    int StartIndex =
        QuizInfo.QuestionsNumber - NewQuestionsNumber;

    for (int i = StartIndex; i < QuizInfo.QuestionsNumber; i++)
    {
        QuizInfo.Questions[i] =
            GenerateQuestion(
                QuizInfo.QuizzLevel,
                QuizInfo.QuizzOperationType);

        QuizInfo.Questions[i].QuestionNumber = i + 1;
    }
}

//----------------------------------------------------------
// Function: CheckAnswer
// Purpose: Checks whether the player's answer is correct.
// Parameters: CorrectAnswer - The right answer, PlayerAnswer - The answer entered by the player.
// Returns: True if the answer is correct, false otherwise.
//----------------------------------------------------------
bool CheckAnswer(int CorrectAnswer, int PlayerAnswer)
{
    return CorrectAnswer == PlayerAnswer;
}

//----------------------------------------------------------
// Function: UpdateQuizStatistics
// Purpose: Updates right answers, wrong answers, and pass/fail result.
// Parameters: QuizInfo - The quiz information to update, IsCorrect - Whether the last answer was correct.
// Returns: Nothing.
//----------------------------------------------------------
void UpdateQuizStatistics(stQuizInfo& QuizInfo, bool IsCorrect)
{
    if (IsCorrect)
        QuizInfo.RightAnswersNumber++;
    else
        QuizInfo.WrongAnswersNumber++;

    QuizInfo.IsPass =
        QuizInfo.RightAnswersNumber >=
        QuizInfo.WrongAnswersNumber;
}


//==========================================================
// Game Functions
//==========================================================

//----------------------------------------------------------
// Function: AskQuestions
// Purpose: Asks a selected range of questions and corrects each answer.
// Parameters: QuizInfo - Quiz data, StartIndex - First question index, EndIndex - Last question index.
// Returns: Nothing.
//----------------------------------------------------------
void AskQuestions(stQuizInfo& QuizInfo, int StartIndex, int EndIndex)
{
    for (int i = StartIndex; i <= EndIndex; i++)
    {
        AskQuestion(QuizInfo.Questions[i], QuizInfo.QuestionsNumber);

        QuizInfo.Questions[i].PlayerResult =
            ReadAnswer();

        cout << '\a';

        QuizInfo.Questions[i].IsRightAnswer =
            CheckAnswer(
                QuizInfo.Questions[i].CorrectResult,
                QuizInfo.Questions[i].PlayerResult);

        UpdateQuizStatistics(
            QuizInfo,
            QuizInfo.Questions[i].IsRightAnswer);

        ShowAnswerResult(
            QuizInfo.Questions[i].IsRightAnswer,
            QuizInfo.Questions[i].CorrectResult);
    }
}

//----------------------------------------------------------
// Function: AskAndCorrectQuestions
// Purpose: Starts asking all questions in the first quiz round.
// Parameters: QuizInfo - The quiz information to use and update.
// Returns: Nothing.
//----------------------------------------------------------
void AskAndCorrectQuestions(stQuizInfo& QuizInfo)
{
    AskQuestions(
        QuizInfo,
        0,
        QuizInfo.QuestionsNumber - 1);
}

//----------------------------------------------------------
// Function: UpdateAskAndCorrectQuestions
// Purpose: Asks only the questions that were newly added.
// Parameters: QuizInfo - The quiz information to update, NewQuestionsNumber - Number of new questions.
// Returns: Nothing.
//----------------------------------------------------------
void UpdateAskAndCorrectQuestions(stQuizInfo& QuizInfo, int NewQuestionsNumber)
{
    AskQuestions(
        QuizInfo,
        QuizInfo.QuestionsNumber - NewQuestionsNumber,
        QuizInfo.QuestionsNumber - 1);
}

//----------------------------------------------------------
// Function: PlayGame
// Purpose: Reads quiz settings, generates questions, and starts the first round.
// Parameters: None.
// Returns: The quiz information after the first round.
//----------------------------------------------------------
stQuizInfo PlayGame()
{
    stQuizInfo QuizInfo;

    SetMenuColor();
    PrintTitle("QUIZ CONFIGURATION");

    QuizInfo.QuestionsNumber =
        ReadIntInRange(
            "How many questions do you want? (1 - 100) : ",
            1,
            MAX_QUESTIONS);

    QuizInfo.QuizzLevel =
        ReadQuizzLevel();

    QuizInfo.QuizzOperationType =
        ReadOperationType();

    GenerateNewQuestionsList(QuizInfo);

    AskAndCorrectQuestions(QuizInfo);

    return QuizInfo;
}

//----------------------------------------------------------
// Function: PlayMoreUpdate
// Purpose: Adds more questions to the same quiz and asks them.
// Parameters: QuizInfo - The quiz information to update.
// Returns: Nothing.
//----------------------------------------------------------
void PlayMoreUpdate(stQuizInfo& QuizInfo)
{
    int RemainingQuestions =
        MAX_QUESTIONS - QuizInfo.QuestionsNumber;

    int NewQuestions =
        ReadIntInRange(
            "How many additional questions do you want? (1 - "
            + to_string(RemainingQuestions)
            + ") : ",
            1,
            RemainingQuestions);

    QuizInfo.QuestionsNumber += NewQuestions;

    UpdateQuestionsList(
        QuizInfo,
        NewQuestions);

    UpdateAskAndCorrectQuestions(
        QuizInfo,
        NewQuestions);
}

//----------------------------------------------------------
// Function: StartGame
// Purpose: Controls the full game flow from welcome screen to goodbye screen.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void StartGame()
{
    ShowWelcomeScreen();

    stQuizInfo QuizInfo;

    bool PlayMore = true;
    bool ShowQuestionsDetails = false;

    QuizInfo = PlayGame();

    WaitForEnter(
        "\nPress ENTER to view the quiz result...");

    cout << '\a';

    ShowGameOverScreen();
    PrintQuizzInfo(QuizInfo);

    while (QuizInfo.QuestionsNumber < MAX_QUESTIONS)
    {
        PlayMore = AskToPlayMore();

        if (!PlayMore)
            break;

        ResetScreen();

        PlayMoreUpdate(QuizInfo);

        WaitForEnter(
            "\nPress ENTER to view the updated result...");

        cout << '\a';

        ShowGameOverScreen();
        PrintQuizzInfo(QuizInfo);
    }

    ShowQuestionsDetails =
        AskToShowQuestionsInfo();

    if (ShowQuestionsDetails)
    {
        ResetScreen();

        PrintQuestionsInfo(
            QuizInfo.Questions,
            QuizInfo.QuestionsNumber);

        WaitForEnter(
            "\nPress ENTER to exit...");
    }

    ResetScreen();
    ShowGoodbyeScreen();
}


//==========================================================
// Display Functions
//==========================================================

//----------------------------------------------------------
// Function: ShowWelcomeScreen
// Purpose: Displays the welcome screen at the start of the game.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void ShowWelcomeScreen()
{
    SetMenuColor();

    cout << "\n\n";
    PrintTitle("WELCOME TO MATH QUIZ GAME");
    cout << "Sharpen your arithmetic skills and enjoy the challenge.\n";
    PrintSeparator();
}

//----------------------------------------------------------
// Function: AskQuestion
// Purpose: Displays one question before reading the player's answer.
// Parameters: Question - The question to display, TotalQuestions - Number of questions in the quiz.
// Returns: Nothing.
//----------------------------------------------------------
void AskQuestion(const stQuestionInfo& Question, int TotalQuestions)
{
    SetOperationBackgroundColor(Question.QuestionOperationType);

    cout << "\n";
    PrintSeparator();
    cout << "\t\tQUESTION "
        << Question.QuestionNumber
        << " OF "
        << TotalQuestions
        << "\n";
    PrintSeparator();

    cout << "\n";
    PrintInfoLine("Difficulty", GetLevelText(Question.QuestionLevel));
    PrintInfoLine("Operation", GetOperationTypeText(Question.QuestionOperationType));

    cout << "\n";
    cout << MathematicalExpression(
        Question.Number1,
        Question.Number2,
        Question.QuestionOperationType)
        << "?\n";

    PrintSeparator();
}

//----------------------------------------------------------
// Function: ShowAnswerResult
// Purpose: Displays whether the player's answer was correct or wrong.
// Parameters: IsCorrect - Whether the answer is correct, CorrectAnswer - The right answer.
// Returns: Nothing.
//----------------------------------------------------------
void ShowAnswerResult(bool IsCorrect, int CorrectAnswer)
{
    cout << "\n";

    if (IsCorrect)
    {
        SetSuccessColor();

        PrintSeparator();
        cout << "\t\tCORRECT ANSWER\n";
        PrintSeparator();
    }
    else
    {
        SetErrorColor();

        PrintSeparator();
        cout << "\t\tWRONG ANSWER\n";
        PrintSeparator();
        cout << "\n";
        PrintInfoLine("Correct Answer", to_string(CorrectAnswer));
    }

    cout << "\n";
}

//----------------------------------------------------------
// Function: ShowGameOverScreen
// Purpose: Displays the quiz finished screen.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void ShowGameOverScreen()
{
    SetResultColor();

    cout << "\n\n";
    PrintTitle("QUIZ FINISHED");
}

//----------------------------------------------------------
// Function: PrintQuizzInfo
// Purpose: Displays the final quiz summary.
// Parameters: QuizInfo - The completed quiz information.
// Returns: Nothing.
//----------------------------------------------------------
void PrintQuizzInfo(const stQuizInfo& QuizInfo)
{
    if (QuizInfo.IsPass)
        SetSuccessColor();
    else
        SetErrorColor();

    cout << "\n";
    PrintSeparator();
    PrintInfoLine("Questions", to_string(QuizInfo.QuestionsNumber));
    PrintInfoLine("Difficulty", GetLevelText(QuizInfo.QuizzLevel));
    PrintInfoLine("Operation", GetOperationTypeText(QuizInfo.QuizzOperationType));
    PrintInfoLine("Correct Answers", to_string(QuizInfo.RightAnswersNumber));
    PrintInfoLine("Wrong Answers", to_string(QuizInfo.WrongAnswersNumber));
    PrintInfoLine("Final Result", GetQuizzResultText(QuizInfo.IsPass));
    PrintSeparator();
}

//----------------------------------------------------------
// Function: PrintQuestionsInfo
// Purpose: Displays full details for every question after the quiz ends.
// Parameters: Questions - The questions array, QuestionsNumber - Number of used questions.
// Returns: Nothing.
//----------------------------------------------------------
void PrintQuestionsInfo(const stQuestionInfo Questions[], int QuestionsNumber)
{
    SetMenuColor();

    cout << "\n";
    PrintTitle("QUESTIONS DETAILS");

    for (int i = 0; i < QuestionsNumber; i++)
    {
        cout << "\n";
        PrintSeparator();
        cout << "\t\tQUESTION #" << Questions[i].QuestionNumber << "\n";
        PrintSeparator();

        PrintInfoLine(
            "Expression",
            MathematicalExpression(
                Questions[i].Number1,
                Questions[i].Number2,
                Questions[i].QuestionOperationType));

        PrintInfoLine(
            "Difficulty",
            GetLevelText(Questions[i].QuestionLevel));

        PrintInfoLine(
            "Operation",
            GetOperationTypeText(Questions[i].QuestionOperationType));

        PrintInfoLine(
            "Correct Answer",
            to_string(Questions[i].CorrectResult));

        PrintInfoLine(
            "Player Answer",
            to_string(Questions[i].PlayerResult));

        PrintInfoLine(
            "Result",
            GetQuestionResultText(Questions[i].IsRightAnswer));
    }

    PrintSeparator();
}

//----------------------------------------------------------
// Function: ShowGoodbyeScreen
// Purpose: Displays the goodbye screen before the program exits.
// Parameters: None.
// Returns: Nothing.
//----------------------------------------------------------
void ShowGoodbyeScreen()
{
    SetResultColor();

    cout << "\n\n";
    PrintTitle("THANKS FOR PLAYING");
    cout << "Hope you enjoyed the Math Quiz Game.\n";
    cout << "See you again.\n";
    PrintSeparator();

    SetDefaultColor();
}


//==========================================================
// Main
//==========================================================

//----------------------------------------------------------
// Function: main
// Purpose: Starts the program.
// Parameters: None.
// Returns: 0 when the program finishes successfully.
//----------------------------------------------------------
int main()
{
    srand((unsigned int)time(nullptr));

    StartGame();

    return 0;
}
