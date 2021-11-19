#include<stdio.h>
#include<conio.h>
#include<windows.h>
#pragma warning(disable : 4996)
const bool PositiveResult = 1; 
const bool negativeResult = 0; 
const int numberOfMonths = 12;
const int minimumMonth = 1; 
const int minYear = 1600;

class consolEditor
{
public:
    consolEditor();
    ~consolEditor();
    void SetColor(int ForgC);
    void ClearConsoleToColors(int ForgC, int BackC); 
    void SetColorAndBackground(int ForgC, int BackC); 

private:

};

consolEditor::consolEditor()
{
}

consolEditor::~consolEditor()
{
}
consolEditor consol; 
struct Date {
    int day;
    int month;
    int year;
};
struct Date date;

struct Remainder {
    int day;
    int month;
    char note[50];
};
struct Remainder remainder;


COORD xy = { 0, 0 };
void gotoxy(int x, int y)
{
    xy.X = x; xy.Y = y; 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}

void consolEditor::SetColor(int ForgC) 
{
    WORD wordColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wordColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wordColor);
    }
    return;
}


//////////////////////////////////////////////////////////////////////////////////////////////
void consolEditor::ClearConsoleToColors(int ForgC, int shiftBackColor)
{
    int numberOfShifting_ForgC = 4; 
    
    WORD setFontColor = ((shiftBackColor & 0x0F) //// take last 4 bits of BackC
        << numberOfShifting_ForgC)// and shift 4 to the left
        +
        (
            ForgC & 0x0F // plus take last 4 bits of ForgC
            );

    //Get the handle to the current output buffer...
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    //This is used to reset the carat/cursor to the top left.
    COORD coord = { 0, 0 };
    //A return value... indicating how many chars were written
    //   not used but we need to capture this since it will be
    //   written anyway (passing NULL causes an access violation).
    DWORD count;
    //This is a structure containing all of the console info
    // it is used here to find the size of the console.
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    //Here we will set the current color
    SetConsoleTextAttribute(hStdOut, setFontColor);
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //This fills the buffer with a given character (in this case 32=space).
        FillConsoleOutputCharacter(hStdOut, (TCHAR)32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
        FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
        //This will set our cursor position for the next print statement.
        SetConsoleCursorPosition(hStdOut, coord);
    }
    return;
}
//////////////////////////////////////////////////////////////////////////////////////////////
void consolEditor::SetColorAndBackground(int ForgC, int BackC)
{
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
    return;
}
int isLeapYear(int interedYear) { 
    const int resultOfRemainder = 0;

    if (interedYear % 400 == resultOfRemainder || (interedYear % 100 != resultOfRemainder && interedYear % 4 == resultOfRemainder))
        return PositiveResult;
    return negativeResult;
}
void increaseMonth_ByOne(int* month, int* year) { 
    
    ++* month;
    if (*month > numberOfMonths) {
        ++* year;
        *month = *month - numberOfMonths;
    }
}
void decreaseMonth_ByOne(int* month, int* year) {
    --* month;
    if (*month < 1) {
        --* year;
        if (*year < 1600) {
            printf("No record available");
            return;
        }
        *month = *month + numberOfMonths;
    }
}
int getNumberOfDays(int month, int year) { 
    // Check for 31 Days
    if (month == 1 || month == 3 || month == 5
    || month == 7 || month == 8 || month == 10
    || month == 12){
    return (31);}
    // Check for 30 Days
    else if (month == 4 || month == 6
        || month == 9 || month == 11) {
        return (30); }
    else if(isLeapYear(year) == PositiveResult) {// Check for leap year
        return(29);}
    else return(28);     
}

const char* getNameOfDay(int enterdDay) { 
    switch (enterdDay) {
    case 0:return("Sunday");
    case 1:return("Monday");
    case 2:return("Tuesday");
    case 3:return("Wednesday");
    case 4:return("Thursday");
    case 5:return("Friday");
    case 6:return("Saturday");
    default:return("Error: Invalid DAY Passed");
    }
}
int getDayNumber(int day, int month, int year) { 
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    year -= month < 3;
    return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) %7;
}
//////////////////

const char* getDay(int day, int month, int year) {
    int day;
     
    
    if (!(month >= minimumMonth && month <= numberOfMonths)) {
        return("Invalid month value");
    }
    
    if (!(day >= 1 && day <= getNumberOfDays(month, year))) {
        return("Invalid date");
    }
    
    if (year >= minYear) {
        day = getDayNumber(day, month, year);
        return(getNameOfDay(day));
    }
    else {
        return("Please give year more than 1600");
    }
}

int checkNote(int day, int month) {
    FILE* fp;
    fp = fopen("note.dat", "rb");
    if (fp == NULL) {
        printf("Error in Opening the file");
    }
    while (fread(&remainder, sizeof(remainder), 1, fp) == PositiveResult) {
        if (remainder.day == day && remainder.month == month) {
            fclose(fp);
            return PositiveResult;
        }
    }
    fclose(fp);
    return negativeResult;
}

const char* getMonth(int enterMonth) {
    switch (enterMonth) {
    case 1: printf("January"); break;
    case 2: printf("February"); break;
    case 3: printf("March"); break;
    case 4: printf("April"); break;
    case 5: printf("May"); break;
    case 6: printf("June"); break;
    case 7: printf("July"); break;
    case 8: printf("August"); break;
    case 9: printf("September"); break;
    case 10: printf("October"); break;
    case 11: printf("November"); break;
    case 12: printf("December"); break;
    }

}
int locatesStartingDay(int day, int x) {
    int nod, day, cnt, d = 1, x1 = x, isNote = 0;
    switch (day) { //locates the starting day in calender
    case 0:
        x = x;
        cnt = 1;
        break;
    case 1:
        x = x + 4;
        cnt = 2;
        break;
    case 2:
        x = x + 8;
        cnt = 3;
        break;
    case 3:
        x = x + 12;
        cnt = 4;
        break;
    case 4:
        x = x + 16;
        cnt = 5;
        break;
    case 5:
        x = x + 20;
        cnt = 6;
        break;
    case 6:
        x = x + 24;
        cnt = 7;
        break;
    default:
        printf("INVALID DATA FROM THE getOddNumber()MODULE");
        return;

    }
}
void printMonth(int month, int year, int x, int y) { //prints the monthth with all days
    int nod, day, cnt, d = 1, x1 = x, y1 = y, isNote = 0;
    if (!(month >= minimumMonth && month <= numberOfMonths)) {
        printf("INVALID MONTH");
        _getch();
        return;
    }
    if (!(year >= minYear)) {
        printf("INVALID YEAR");
        _getch();
        return;
    }
    gotoxy(20, y);
    // Print date 
    printf("---------------------------\n");
    gotoxy(25, 6);
    

    getMonth( month);

    printf(" , %d", year);
    gotoxy(20, 7);
    printf("---------------------------");
    y += 3;
    gotoxy(x, y);
    printf("S   M   T   W   T   F   S   ");
    y++;
    nod = getNumberOfDays(month, year);
    day = getDayNumber(d, month, year);
    locatesStartingDay(day, x); 
    
    gotoxy(x, y);
    if (cnt == 1) {
        consol.SetColor(12);
    }
    if (checkNote(d, month) == 1) {
        consol.SetColorAndBackground(10, 12);
    }
    printf("%02d", d);
    consol.SetColorAndBackground(15, 1);
    for (d = 2; d <= nod; d++) {
        if (cnt % 7 == 0) {
            y++;
            cnt = 0;
            x = x1 - 4;
        }
        x = x + 4;
        cnt++;
        gotoxy(x, y);
        if (cnt == 1) {
            consol.SetColor(12);
        }
        else {
            //Clear color
            consol.SetColor(15);
        }
        if (checkNote(d, month) == 1) {
            consol.SetColorAndBackground(15, 12);
        }
        printf("%02d", d);
        consol.SetColorAndBackground(15, 1);
    }
    gotoxy(8, y + 2);
    consol.SetColor(14);
    printf("Press 'n'  to Next, \n Press 'p' to Previous \n and 'q' to Quit");
    gotoxy(8, y + 3);
    printf("\nRed Background indicates the NOTE,\n Press 's' to see note: ");
    
    consol.SetColor(15);
}
void AddNote() {
    FILE* fp;
    fp = fopen("note.dat", "ab+");
    system("cls");
    gotoxy(5, 7);
    printf("Enter the date(DD MM): ");
    scanf_s("%d %d", &remainder.day, &remainder.month);
    gotoxy(5, 8);
    printf("Enter the Note(50 character max): ");
    fflush(stdin);
    scanf(" %[^\n]", remainder.note);
    if (fwrite(&remainder, sizeof(remainder), 1, fp)) {
        gotoxy(5, 12);
        puts("Note is saved sucessfully");
        fclose(fp);
    }
    else {
        gotoxy(5, 12);
        consol.SetColor(14);
        puts("\aFail to save!!\a");
        
        consol.SetColor(15);
    }
    gotoxy(5, 15);
    printf("Press any key............");
    _getch();
    fclose(fp);
}
void showNote(int month) {
    FILE* fp;
    int i = 0, isFound = 0;
    system("cls");
    fp = fopen("note.dat", "rb");
    if (fp == NULL) {
        printf("Error in opening the file");
    }
    while (fread(&remainder, sizeof(remainder), 1, fp) == 1) {
        if (remainder.month == month) {
            gotoxy(10, 5 + i);
            printf("Note %d Day = %d: %s", i + 1, remainder.day, remainder.note);
            isFound = 1;
            i++;
        }
    }
    if (isFound == 0) {
        gotoxy(10, 5);
        printf("This Month contains no note");
    }
    gotoxy(10, 7 + i);
    printf("Press any key to back.......");
    _getch();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main() {
    consolEditor C; 
    C.ClearConsoleToColors(15, 1);
    SetConsoleTitleA("Calender Project - Programming-technique.blogspot.com");
    int choice;
    char choiceAfter = 'a';
    while (1) {
        system("cls");
        printf("1. Find Out the Day\n");
        printf("2. Print all the day of month\n");
        printf("3. Add Note\n");
        printf("4. EXIT\n");
        printf("ENTER YOUR CHOICE : ");
        scanf_s("%d", &choice);
        system("cls");

        switch (choice) {
        case 1:
            printf("Enter date (DD MM YYYY) : ");
            scanf_s("%d %d %d", &date.day, &date.month, &date.year);
            printf("Day is : %s", getDay(date.day, date.month, date.year));
            printf("\nPress any key to continue......");
            _getch();
            break;
        case 2:
            printf("Enter month and year (MM YYYY) : ");
            scanf_s("%d %d", &date.month, &date.year);
            system("cls");
            while (choiceAfter != 'q') {
                printMonth(date.month, date.year, 20, 5);
                choiceAfter = _getch();
                if (choiceAfter == 'n') {
                    increaseMonth_ByOne(&date.month, &date.year);
                    system("cls");
                    printMonth(date.month, date.year, 20, 5);
                }
                else if (choiceAfter == 'p') {
                    increaseMonth_ByOne(&date.month, &date.year);
                    system("cls");
                    printMonth(date.month, date.year, 20, 5);
                }
                else if (choiceAfter == 's') {
                    showNote(date.month);
                    system("cls");
                }
            }
            break;
        case 3:
            AddNote();
            break;
        case 4:
            exit(0);
        }
    }
    return 0;
}