#include "Controller.h"
#include <iostream>

int main()
{
    //Setup
    initscr();
    cbreak();
    noecho();
    timeout(-1);
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    Controller *controller = new Controller();
    controller->updateScreen();
    std::string filename = "";
    switch( controller->getMainMenuChoice() )
    {
        case 0:
            //Create a new blank board
            controller->createNewBoard(true);
            break;
        case 1:
            //Load a saved board
            //ADD WAY TO GET FILENAME
            filename = controller->getStringInput();
            controller->createNewBoard(filename);
            break;
        case 2:
            //Load a random board
            controller->createNewBoard(true);
            controller->randomizeBoard();
            break;
        case 3:
            //Load the pattern editor
            break;
    }
    controller->setState("Paused");
    wchar_t input;
    std::cout << "Printing board." << std::endl;
    controller->printBoard();
    controller->updateScreen();
    int count = 0;
    while((input = getch()) != 27)
    {
        std::cout << "Iteration count: " << ++count << std::endl;

        if(input == '[')
        {
            controller->setSpeed(-1);
            /*note: continue is used to make sure the simulation doesn't
            stutter after adjusting the speed... it adds a simple pause since
            it skips an iteration*/
            continue;
        }
        if(input == ']')
        {
            controller->setSpeed(1);
            continue;
        }
        if(input == 'p')
        {
            if(controller->getState() == "Paused")
            {
                controller->setState("Running");
            }
            else
            {
                timeout(-1);
                controller->setState("Paused");
            }
        }
        //&& is used for an "advance one iteration" when paused
        if(controller->getState() == "Paused" && input != ' ')
            continue;
        timeout(1.0/controller->getSpeed() * 1000);
        /*if( mvwinch(boardWin, 1, 1) == '+')
            mvwprintw(boardWin, 1, 1, "-");
        else
            mvwprintw(boardWin, 1, 1, "+");*/
        for (int i = 1; (controller->getSpeed() > 1000) && (i < controller->getSpeed() / 1000); i++) {
            controller-> runIteration();
            count++;
        }
        controller->runIteration();
        controller->printBoard();
        controller->updateScreen();
    }
    endwin();
    return 0;
}
