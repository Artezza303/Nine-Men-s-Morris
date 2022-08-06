#include "Main.h"

BEGIN_EVENT_TABLE(NineMensMorris, wxPanel)
EVT_PAINT(NineMensMorris::paintEvent)
EVT_LEFT_DOWN(NineMensMorris::LeftClickPlayer1)
EVT_CLOSE(NineMensMorris::OnClose)
END_EVENT_TABLE()

//EVT_RIGHT_DOWN(cMain::RightClickPlace)
//EVT_LEFT_DOWN(cMain::LeftClickRemove)


NineMensMorris::NineMensMorris() : wxFrame(nullptr, wxID_ANY, "Nine Men's Morris", wxDefaultPosition, wxSize(1280, 720))
{
    Centre();
    SetBackgroundColour(wxNullColour);
    //SetCursor(wxCursor(wxCURSOR_HAND));
    //SetCursor(wxCursor(wxCURSOR_ARROW));

    makeSquareVector();
    makeCircleVector();
}

NineMensMorris::~NineMensMorris()
{

}

void NineMensMorris::OnClose(wxCloseEvent& event)
{
    Destroy();
}

void NineMensMorris::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    drawTheBoard(dc);
}

void NineMensMorris::LeftClickPlayer1(wxMouseEvent& event)
{
    wxClientDC dc(this);

    auto  xDiff = event.GetPosition().x;
    auto  yDiff = event.GetPosition().y;


    for (int i = 0; i < 24; i++)
    {
        if ((get<0>(squareCoordinates[i]) < xDiff&& xDiff < get<1>(squareCoordinates[i])) && (get<2>(squareCoordinates[i]) < yDiff && yDiff < get<3>(squareCoordinates[i])))
        {
            if (gameBoard.at(i) == -1)
            {
                break;
            }
            drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxBLUE_BRUSH);
            gameBoard.at(i) = 1;
            playerOnePiecesLeft -= 1;
            playerOnePiecesOnBoard += 1;

            playerOneMillLocation = i;
            if (checkPlayerMillBlue(gameBoard) == true)
            {
                wxLogMessage("Blue Player Mill, Remove Red Piece");
                playerOneEnemies += 1;
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer1, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer1Phase1, this);
            }
            else
            {
                //wxLogMessage("Got Here");
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer1, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
            }
        }

    }

    DrawPlayerOnePieces(dc);
}

/*
void NineMensMorris::LeftClickPlayer2(wxMouseEvent& event)
{
    wxClientDC dc(this);

    auto  xDiff = event.GetPosition().x;
    auto  yDiff = event.GetPosition().y;

    for (int i = 0; i < 24; i++)
    {
        if ((get<0>(squareCoordinates[i]) < xDiff&& xDiff < get<1>(squareCoordinates[i])) && (get<2>(squareCoordinates[i]) < yDiff && yDiff < get<3>(squareCoordinates[i])))
        {
            if (gameBoard.at(i) == 1)
            {
                break;
            }
            drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxRED_BRUSH);
            gameBoard.at(i) = -1;
            playerTwoPiecesLeft -= 1;
            playerTwoPiecesOnBoard += 1;

            playerTwoMillLocation = i;
            if (checkPlayerMillRed(gameBoard) == true)
            {
                wxLogMessage("Red Player Mill, Remove Blue Piece");
                playerTwoEnemies += 1;
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase1, this);

            }
            else
            {
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer1, this);
            }

            if (playerTwoPiecesLeft == 0)
            {
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMove, this);
            }

            //ADDED TO TEST IF ITS THE LAST PIECE RED IS PLACING AND IS ALSO A MILL SO THE CORREECT FUNCTION GETS CALLED NEEDS TEST
            if (playerTwoPiecesLeft == 0 && checkPlayerMillRed(gameBoard) == true)
            {
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase2, this);
            }

        }
    }

    DrawPlayerTwoPieces(dc);
}
*/







void NineMensMorris::LeftClickPlayer2(wxMouseEvent& event)
{
    wxClientDC dc(this);

    int test = TESTING_COMPUTER_OPPONENT_PLACE(gameBoard);




    if (test == 0)
    {
        test = returnRandomSpot();

    }






    drawCircle(dc, get<0>(circleCoordinates[test]), get<1>(circleCoordinates[test]), *wxRED_BRUSH);

    gameBoard.at(test) = -1;
    playerTwoPiecesLeft -= 1;
    playerTwoPiecesOnBoard += 1;



    playerOneMillLocation = test;




    if (checkPlayerMillRed(gameBoard) == true)
    {
        wxLogMessage("Red Player Mill, Remove Blue Piece");
        playerTwoEnemies += 1;
        Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
        Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase1, this);

    }
    else
    {
        Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
        Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer1, this);
    }

    if (playerTwoPiecesLeft == 0)
    {
        Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
        Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMove, this);
    }

    //ADDED TO TEST IF ITS THE LAST PIECE RED IS PLACING AND IS ALSO A MILL SO THE CORREECT FUNCTION GETS CALLED NEEDS TEST
    if (playerTwoPiecesLeft == 0 && checkPlayerMillRed(gameBoard) == true)
    {
        Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
        Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase2, this);
    }

    /*
    for (int i = 0; i < 24; i++)
    {
        if ((get<0>(squareCoordinates[i]) < xDiff && xDiff < get<1>(squareCoordinates[i])) && (get<2>(squareCoordinates[i]) < yDiff && yDiff < get<3>(squareCoordinates[i])))
        {
            if (gameBoard.at(i) == 1)
            {
                break;
            }
            drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxRED_BRUSH);
            gameBoard.at(i) = -1;
            playerTwoPiecesLeft -= 1;
            playerTwoPiecesOnBoard += 1;

            millLocation = i;
            if (checkPlayerMillRed(gameBoard) == true)
            {
                wxLogMessage("Red Player Mill, Remove Blue Piece");
                playerTwoEnemies += 1;
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase1, this);

            }
            else
            {
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer1, this);
            }

            if (playerTwoPiecesLeft == 0)
            {
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMove, this);
            }

            //ADDED TO TEST IF ITS THE LAST PIECE RED IS PLACING AND IS ALSO A MILL SO THE CORREECT FUNCTION GETS CALLED NEEDS TEST
            if (playerTwoPiecesLeft == 0 && checkPlayerMillRed(gameBoard) == true)
            {
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase2, this);
            }

        }
    }
    */

    DrawPlayerTwoPieces(dc);
}













void NineMensMorris::LeftClickPlayerOneMove(wxMouseEvent& event)
{
    wxClientDC dc(this);

    auto  xDiff = event.GetPosition().x;
    auto  yDiff = event.GetPosition().y;


    //TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST
    if (checkLegalMove(gameBoard, 1) == true)
    {
        wxLogMessage("Player 2 Wins, Player 1 No Legal Move");
        Destroy();
    }


    for (int i = 0; i < 24; i++)
    {
        if ((get<0>(squareCoordinates[i]) < xDiff&& xDiff < get<1>(squareCoordinates[i])) && (get<2>(squareCoordinates[i]) < yDiff && yDiff < get<3>(squareCoordinates[i])))
        {
            if (gameBoard.at(i) == 1)
            {
                //wxLogMessage("One Move");

                getLocation = i;
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMove, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMovePart2, this);
            }
        }
    }
}

//fix loc 16 move, 13 mill
void NineMensMorris::LeftClickPlayerOneMovePart2(wxMouseEvent& event)
{
    wxClientDC dc(this);

    auto  xDiff = event.GetPosition().x;
    auto  yDiff = event.GetPosition().y;


    for (int i = 0; i < 24; i++)
    {
        if ((get<0>(squareCoordinates[i]) < xDiff&& xDiff < get<1>(squareCoordinates[i])) && (get<2>(squareCoordinates[i]) < yDiff && yDiff < get<3>(squareCoordinates[i])))
        {
            if (gameBoard.at(i) == 0)
            {
                if (playerOnePiecesOnBoard >= 4)
                {
                    getCurrentLocation = i;
                    if (checkPlayerAdjacent(gameBoard) == true)
                    {
                        //wxLogMessage("One Move 2");

                        drawCircle(dc, get<0>(circleCoordinates[getLocation]), get<1>(circleCoordinates[getLocation]), *wxWHITE_BRUSH);
                        gameBoard.at(getLocation) = 0;

                        drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxBLUE_BRUSH);
                        gameBoard.at(i) = 1;

                        playerOneMillLocation = i;
                        if (checkPlayerMillBlue(gameBoard) == true)
                        {
                            wxLogMessage("Blue Player Mill, Remove Red Piece");
                            playerOneEnemies += 1;
                            Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMovePart2, this);
                            Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer1Phase2, this);
                        }
                        else
                        {
                            Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMovePart2, this);
                            Bind(wxEVT_LEFT_DOWN, &NineMensMorris::MoveComputer, this);
                        }
                    }
                    else
                    {
                        Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMovePart2, this);
                        Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMove, this);
                    }
                }
                else
                {
                    // wxLogMessage("One Move 2");

                    drawCircle(dc, get<0>(circleCoordinates[getLocation]), get<1>(circleCoordinates[getLocation]), *wxWHITE_BRUSH);
                    gameBoard.at(getLocation) = 0;

                    drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxBLUE_BRUSH);
                    gameBoard.at(i) = 1;

                    playerOneMillLocation = i;
                    if (checkPlayerMillBlue(gameBoard) == true)
                    {
                        wxLogMessage("Blue Player Mill, Remove Red Piece");
                        playerOneEnemies += 1;
                        Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMovePart2, this);
                        Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer1Phase2, this);
                    }
                    else
                    {
                        Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMovePart2, this);
                        Bind(wxEVT_LEFT_DOWN, &NineMensMorris::MoveComputer, this);
                    }
                }
            }
            else if (gameBoard.at(i) == 1)
            {
                getLocation = i;
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMovePart2, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMovePart2, this);
            }
        }
    }
}


void NineMensMorris::MoveComputer(wxMouseEvent& event) {
    wxClientDC dc(this);

    int redPiece = returnRedSpot();
    getLocation = redPiece;
    getCurrentLocation = getLocation;
    //wxLogMessage("hello");
    wxString stringnumber = wxString::Format(wxT("%d"), (int)redPiece);
    wxLogMessage("Red Piece" + stringnumber);

    if (checkLegalMove(gameBoard, -1) == true)
    {
        wxLogMessage("Player 1 Wins, Player 2 No Legal Move");
        Destroy();
    }
    if (playerTwoPiecesOnBoard >= 4)
    {


        //wxLogMessage("Two Move 2");
        int moveRed = ReturnComputerAdjacent(gameBoard);
        wxString stringnumber = wxString::Format(wxT("%d"), (int)moveRed);
        wxLogMessage("Move Red" + stringnumber);
        drawCircle(dc, get<0>(circleCoordinates[getLocation]), get<1>(circleCoordinates[getLocation]), *wxWHITE_BRUSH);
        gameBoard.at(getLocation) = 0;

        drawCircle(dc, get<0>(circleCoordinates[moveRed]), get<1>(circleCoordinates[moveRed]), *wxRED_BRUSH);
        gameBoard.at(moveRed) = -1;

        playerTwoMillLocation = moveRed;
        if (checkPlayerMillRed(gameBoard) == true)
        {
            wxLogMessage("Red Player Mill, Remove Blue Piece");
            playerTwoEnemies += 1;
            Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::MoveComputer, this);
            Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase2, this);
        }
        else
        {
            // wxLogMessage("Got Here");
            Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::MoveComputer, this);
            Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMove, this);
        }


    }
    else
    {
        //wxLogMessage("Two Move 2");

        drawCircle(dc, get<0>(circleCoordinates[getLocation]), get<1>(circleCoordinates[getLocation]), *wxWHITE_BRUSH);
        gameBoard.at(getLocation) = 0;

        drawCircle(dc, get<0>(circleCoordinates[redPiece]), get<1>(circleCoordinates[redPiece]), *wxRED_BRUSH);
        gameBoard.at(redPiece) = -1;

        playerTwoMillLocation = redPiece;
        if (checkPlayerMillRed(gameBoard) == true)
        {
            wxLogMessage("Red Player Mill, Remove Blue Piece");
            playerTwoEnemies += 1;
            Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::MoveComputer, this);
            Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase2, this);
        }
        else
        {
            // wxLogMessage("Got Here");
            Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::MoveComputer, this);
            Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMove, this);
        }
    }

    /*  else if (gameBoard.at(redPiece) == -1)
      {
          getLocation = redPiece;
          Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMovePart2, this);
          Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMovePart2, this);
      }*/
}


/*void NineMensMorris::LeftClickPlayerTwoMove(wxMouseEvent& event)
{
    wxClientDC dc(this);





    auto  xDiff = event.GetPosition().x;
    auto  yDiff = event.GetPosition().y;



    //TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST
    if (checkLegalMove(gameBoard, -1) == true)
    {
        wxLogMessage("Player 1 Wins, Player 2 No Legal Move");
        Destroy();
    }


    for (int i = 0; i < 24; i++)
    {
        if ((get<0>(squareCoordinates[i]) < xDiff&& xDiff < get<1>(squareCoordinates[i])) && (get<2>(squareCoordinates[i]) < yDiff && yDiff < get<3>(squareCoordinates[i])))
        {
            if (gameBoard.at(i) == -1)
            {
                //wxLogMessage("Two Move");

                getLocation = i;
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMove, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMovePart2, this);
            }
        }
    }
}*\

/*void NineMensMorris::LeftClickPlayerTwoMovePart2(wxMouseEvent& event)
{
    wxClientDC dc(this);



    auto  xDiff = event.GetPosition().x;
    auto  yDiff = event.GetPosition().y;



    for (int i = 0; i < 24; i++)
    {
        if ((get<0>(squareCoordinates[i]) < xDiff&& xDiff < get<1>(squareCoordinates[i])) && (get<2>(squareCoordinates[i]) < yDiff && yDiff < get<3>(squareCoordinates[i])))
        {
            if (gameBoard.at(i) == 0)
            {
                if (playerTwoPiecesOnBoard >= 4)
                {
                    getCurrentLocation = i;
                    if (checkPlayerAdjacent(gameBoard) == true)
                    {
                        //wxLogMessage("Two Move 2");

                        drawCircle(dc, get<0>(circleCoordinates[getLocation]), get<1>(circleCoordinates[getLocation]), *wxWHITE_BRUSH);
                        gameBoard.at(getLocation) = 0;

                        drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxRED_BRUSH);
                        gameBoard.at(i) = -1;

                        playerTwoMillLocation = i;
                        if (checkPlayerMillRed(gameBoard) == true)
                        {
                            wxLogMessage("Red Player Mill, Remove Blue Piece");
                            playerTwoEnemies += 1;
                            Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMovePart2, this);
                            Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase2, this);
                        }
                        else
                        {
                            // wxLogMessage("Got Here");
                            Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMovePart2, this);
                            Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMove, this);
                        }
                    }
                    else //TEST
                    {
                        Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMovePart2, this);
                        Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMove, this);
                    }
                }
                else
                {
                    //wxLogMessage("Two Move 2");

                    drawCircle(dc, get<0>(circleCoordinates[getLocation]), get<1>(circleCoordinates[getLocation]), *wxWHITE_BRUSH);
                    gameBoard.at(getLocation) = 0;

                    drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxRED_BRUSH);
                    gameBoard.at(i) = -1;

                    playerTwoMillLocation = i;
                    if (checkPlayerMillRed(gameBoard) == true)
                    {
                        wxLogMessage("Red Player Mill, Remove Blue Piece");
                        playerTwoEnemies += 1;
                        Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMovePart2, this);
                        Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase2, this);
                    }
                    else
                    {
                        // wxLogMessage("Got Here");
                        Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMovePart2, this);
                        Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMove, this);
                    }
                }
            }
            else if (gameBoard.at(i) == -1)
            {
                getLocation = i;
                Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMovePart2, this);
                Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMovePart2, this);
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////

check(i)
{
    if(getLocation == 1)
    {
        if (x.at(15) == 0 || x.at(17) == 0 || x.at(19) == 0)
            return true;
    }
}



void cMain::RightClickPlace(wxMouseEvent& event)
{
    wxClientDC dc(this);

    auto  xDiff = event.GetPosition().x;
    auto  yDiff = event.GetPosition().y;

    for (int i = 0; i < 24; i++)
    {
        if ((get<0>(squareCoordinates[i]) < xDiff&& xDiff < get<1>(squareCoordinates[i])) && (get<2>(squareCoordinates[i]) < yDiff && yDiff < get<3>(squareCoordinates[i])))
        {
            drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxRED_BRUSH);
            gameBoard.at(i) = -1;
        }
    }
}
*/

void NineMensMorris::LeftClickRemovePlayer1Phase1(wxMouseEvent& event)
{
    wxClientDC dc(this);

    auto  xDiff = event.GetPosition().x;
    auto  yDiff = event.GetPosition().y;


    for (int i = 0; i < 24; i++)
    {
        if ((get<0>(squareCoordinates[i]) < xDiff&& xDiff < get<1>(squareCoordinates[i])) && (get<2>(squareCoordinates[i]) < yDiff && yDiff < get<3>(squareCoordinates[i])))
        {
            if (gameBoard.at(i) == -1)
            {
                removeLocation = i;

                if (checkRemoveBlue(gameBoard))
                {
                    Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer1Phase1, this);


                    Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer1Phase1, this);

                    //wxLogMessage("Click Again");
                }
                else
                {

                    playerTwoPiecesOnBoard -= 1;

                    /*
                    if (playerTwoPiecesOnBoard == 2)
                    {
                        wxLogMessage("Player 1 Wins");
                        Destroy();
                    }
                    */
                    drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxWHITE_BRUSH);
                    gameBoard.at(i) = 0;

                    DrawPlayerOnePiecesTaken(dc);


                    Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer2, this);

                }
            }
        }
    }


}



void NineMensMorris::LeftClickRemovePlayer2Phase1(wxMouseEvent& event)
{
    wxClientDC dc(this);

    auto  xDiff = event.GetPosition().x;
    auto  yDiff = event.GetPosition().y;

    for (int i = 0; i < 24; i++)
    {
        if ((get<0>(squareCoordinates[i]) < xDiff&& xDiff < get<1>(squareCoordinates[i])) && (get<2>(squareCoordinates[i]) < yDiff && yDiff < get<3>(squareCoordinates[i])))
        {
            if (gameBoard.at(i) == 1)
            {
                removeLocation = i;

                if (checkRemoveRed(gameBoard))
                {
                    Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase1, this);


                    Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase1, this);

                    //wxLogMessage("Click Again");
                }
                else
                {

                    playerOnePiecesOnBoard -= 1;

                    /*
                    if (playerOnePiecesOnBoard == 2)
                    {
                        wxLogMessage("Player 2 Wins");
                        Destroy();
                    }
                    */
                    drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxWHITE_BRUSH);
                    gameBoard.at(i) = 0;

                    DrawPlayerTwoPiecesTaken(dc);


                    Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayer1, this);

                }
            }
        }
    }


}




void NineMensMorris::LeftClickRemovePlayer1Phase2(wxMouseEvent& event)
{
    wxClientDC dc(this);

    auto  xDiff = event.GetPosition().x;
    auto  yDiff = event.GetPosition().y;


    for (int i = 0; i < 24; i++)
    {
        if ((get<0>(squareCoordinates[i]) < xDiff&& xDiff < get<1>(squareCoordinates[i])) && (get<2>(squareCoordinates[i]) < yDiff && yDiff < get<3>(squareCoordinates[i])))
        {
            if (gameBoard.at(i) == -1)
            {
                removeLocation = i;

                if (checkRemoveBlue(gameBoard))
                {
                    Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer1Phase2, this);


                    Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer1Phase2, this);

                    //wxLogMessage("Click Again");
                }
                else
                {
                    playerTwoPiecesOnBoard -= 1;

                    if (playerTwoPiecesOnBoard == 2)
                    {
                        wxLogMessage("Player 1 Wins");
                        Destroy();
                    }


                    drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxWHITE_BRUSH);
                    gameBoard.at(i) = 0;

                    DrawPlayerOnePiecesTaken(dc);


                    Bind(wxEVT_LEFT_DOWN, &NineMensMorris::MoveComputer, this);
                }


                /*

                playerTwoPiecesOnBoard -= 1;

                if (playerTwoPiecesOnBoard == 2)
                {
                    wxLogMessage("Player 1 Wins");
                    Destroy();
                }


                drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxWHITE_BRUSH);
                gameBoard.at(i) = 0;

                if (checkPlayerMillBlue(gameBoard) == true)
                {
                    Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerTwoMove, this);
                }
                else if (checkPlayerMillRed(gameBoard) == true)
                {
                    Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMove, this);
                }

                */
            }
        }
    }

    //TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST
    if (checkLegalMove(gameBoard, -1) == true)
    {
        wxLogMessage("Player 1 Wins, Player 2 No Legal Move");
        Destroy();
    }
}



void NineMensMorris::LeftClickRemovePlayer2Phase2(wxMouseEvent& event)
{
    wxClientDC dc(this);

    auto  xDiff = event.GetPosition().x;
    auto  yDiff = event.GetPosition().y;

    for (int i = 0; i < 24; i++)
    {
        if ((get<0>(squareCoordinates[i]) < xDiff&& xDiff < get<1>(squareCoordinates[i])) && (get<2>(squareCoordinates[i]) < yDiff && yDiff < get<3>(squareCoordinates[i])))
        {
            if (gameBoard.at(i) == 1)
            {
                removeLocation = i;

                if (checkRemoveRed(gameBoard))
                {
                    Unbind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase2, this);


                    Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickRemovePlayer2Phase2, this);

                    //wxLogMessage("Click Again");
                }
                else
                {
                    playerOnePiecesOnBoard -= 1;

                    if (playerOnePiecesOnBoard == 2)
                    {
                        wxLogMessage("Player 2 Wins");
                        Destroy();
                    }

                    drawCircle(dc, get<0>(circleCoordinates[i]), get<1>(circleCoordinates[i]), *wxWHITE_BRUSH);
                    gameBoard.at(i) = 0;

                    DrawPlayerTwoPiecesTaken(dc);


                    Bind(wxEVT_LEFT_DOWN, &NineMensMorris::LeftClickPlayerOneMove, this);

                    /*
                    //TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST
                    if (checkLegalMoveBlue(gameBoard, 1) == true)
                    {
                        wxLogMessage("Player 2 Wins, Player 1 No Legal Move");
                        Destroy();
                    }
                    */
                }
            }
        }
    }
    //TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST
    if (checkLegalMove(gameBoard, 1) == true)
    {
        wxLogMessage("Player 2 Wins, Player 1 No Legal Move");
        Destroy();
    }
}

int random(int a, int b)
{
    srand(time(NULL));

    int r = rand() % 2;

    if (r == 0)
        return a;
    else
        return b;
}
int NineMensMorris::ReturnComputerAdjacent(vector<int> x)
{
    //Von
    if (getLocation == 0) {
        if ((x.at(9) == 0 || x.at(1) == 0))
        {
            if (x.at(9) == 0)
                return 9;
            else
                return 1;
        }
    }

    if (getLocation == 1) {
        if ((x.at(0) == 0 || x.at(2) == 0 || x.at(4) == 0))
        {
            if (x.at(0) == 0)
                return 0;
            else if (x.at(2) == 0)
                return 2;
            else
                return 4;

        }
    }
    if (getLocation == 2) {
        if ((x.at(1) == 0 || x.at(14) == 0))
        {
            if (x.at(1) == 0)
                return 0;
            else
                return 14;

        }
    }
    if (getLocation == 3) {
        if ((x.at(10) == 0 || x.at(4) == 0))
        {
            if (x.at(10) == 0)
                return 10;
            else
                return 4;

        }
    }
    if (getLocation == 4) {
        if ((x.at(1) == 0 || x.at(3) == 0 || x.at(5) == 0 || x.at(7) == 0))
        {
            if (x.at(1) == 0)
                return 1;
            else if (x.at(2) == 0)
                return 2;
            else if (x.at(5) == 0)
                return 5;
            else
                return 7;

        }
    }
    if (getLocation == 5) {
        if ((x.at(4) == 0 || x.at(13) == 0))
        {
            if (x.at(4) == 0)
                return 4;
            else
                return 13;

        }
    }

    //munia
    if (getLocation == 6) {
        if ((x.at(7) == 0 || x.at(11) == 0))
        {
            if (x.at(7) == 0)
                return 7;
            else
                return 11;

        }
    }
    if (getLocation == 7) {
        if ((x.at(4) == 0 || x.at(8) == 0 || x.at(6) == 0))
        {
            if (x.at(4) == 0)
                return 4;
            else if (x.at(8) == 0)
                return 8;
            else
                return 6;

        }
    }
    if (getLocation == 8) {
        if ((x.at(7) == 0 || x.at(12) == 0))
        {
            if (x.at(7) == 0)
                return 7;
            else
                return 12;

        }
    }

    if (getLocation == 9) {
        if ((x.at(0) == 0 || x.at(10) == 0 || x.at(21) == 0))
        {
            if (x.at(0) == 0)
                return 0;
            else if (x.at(10) == 0)
                return 10;
            else
                return 21;

        }
    }

    if (getLocation == 10) {
        if ((x.at(3) == 0 || x.at(9) == 0 || x.at(18) == 0 || x.at(11) == 0))
        {
            if (x.at(3) == 0)
                return 3;
            else if (x.at(9) == 0)
                return 9;
            else
                return 11;

        }
    }
    if (getLocation == 11) {
        if ((x.at(6) == 0 || x.at(10) == 0 || x.at(15) == 0))
        {
            if (x.at(6) == 0)
                return 6;
            else if (x.at(10) == 0)
                return 10;
            else
                return 15;

        }
    }

    //mariam
    if (getLocation == 12) {
        if ((x.at(8) == 0 || x.at(17) == 0 || x.at(13) == 0))
        {
            if (x.at(12) == 0)
                return 12;
            else if (x.at(17) == 0)
                return 17;
            else
                return 13;

        }
    }

    if (getLocation == 13) {
        if ((x.at(5) == 0 || x.at(20) == 0 || x.at(14) == 0 || x.at(12) == 0))
        {
            if (x.at(5) == 0)
                return 5;
            else if (x.at(20) == 0)
                return 20;
            else
                return 12;

        }
    }

    if (getLocation == 14) {
        if ((x.at(2) == 0 || x.at(23) == 0 || x.at(13) == 0))
        {
            if (x.at(2) == 0)
                return 2;
            else if (x.at(23) == 0)
                return 23;
            else
                return 13;

        }
    }
    if (getLocation == 15) {
        if ((x.at(11) == 0 || x.at(16) == 0))
        {
            if (x.at(11) == 0)
                return 11;
            else
                return 16;

        }
    }
    if (getLocation == 16) {
        if ((x.at(15) == 0 || x.at(17) == 0 || x.at(19) == 0))
        {
            if (x.at(15) == 0)
                return 15;
            else if (x.at(17) == 0)
                return 17;
            else
                return 19;

        }
    }
    if (getLocation == 17) {
        if ((x.at(12) == 0 || x.at(16) == 0))
        {
            if (x.at(12) == 0)
                return 12;
            else
                return 16;

        }
    }

    //Fiza(29 lines)
    if (getLocation == 18) {
        if ((x.at(10) == 0 || x.at(19) == 0))
        {
            if (x.at(10) == 0)
                return 10;

            else
                return 19;

        }
    }
    if (getLocation == 19) {
        if ((x.at(18) == 0 || x.at(20) == 0 || x.at(22) == 0 || x.at(16) == 0))
        {
            if (x.at(18) == 0)
                return 18;
            else if (x.at(22) == 0)
                return 22;
            else
                return 16;

        }
    }
    if (getLocation == 20) {
        if ((x.at(13) == 0 || x.at(19) == 0))
        {
            if (x.at(13) == 0)
                return 13;

            else
                return 19;

        }
    }
    if (getLocation == 21) {
        if ((x.at(9) == 0 || x.at(22) == 0))
        {
            if (x.at(21) == 0)
                return 21;

            else
                return 22;

        }
    }
    if (getLocation == 22) {
        if ((x.at(19) == 0 || x.at(21) == 0 || x.at(23) == 0))
        {
            if (x.at(19) == 0)
                return 19;
            else if (x.at(21) == 0)
                return 21;
            else
                return 23;

        }
    }
    if (getLocation == 23) {
        if ((x.at(22) == 0 || x.at(14) == 0))
        {
            if (x.at(22) == 0)
                return 22;
            else
                return 14;

        }
    }

}
bool NineMensMorris::checkPlayerAdjacent(vector<int> x)
{
    //Von
    if (getLocation == 0) {
        if ((x.at(9) == 0 || x.at(1) == 0) && (getCurrentLocation == 9) || (getCurrentLocation == 1))
        {

            return true;
        }
    }

    if (getLocation == 1) {
        if ((x.at(0) == 0 || x.at(2) == 0 || x.at(4) == 0) && (getCurrentLocation == 0) || (getCurrentLocation == 2) || (getCurrentLocation == 4))
        {
            return true;
        }
    }
    if (getLocation == 2) {
        if ((x.at(1) == 0 || x.at(14) == 0) && (getCurrentLocation == 1) || (getCurrentLocation == 14))
        {
            return true;
        }
    }
    if (getLocation == 3) {
        if ((x.at(10) == 0 || x.at(4) == 0) && (getCurrentLocation == 10) || (getCurrentLocation == 4))
        {
            return true;
        }
    }
    if (getLocation == 4) {
        if ((x.at(1) == 0 || x.at(3) == 0 || x.at(5) == 0 || x.at(7) == 0) && (getCurrentLocation == 1) || (getCurrentLocation == 3) || (getCurrentLocation == 5) || (getCurrentLocation == 7))
        {
            return true;
        }
    }
    if (getLocation == 5) {
        if ((x.at(4) == 0 || x.at(13) == 0) && (getCurrentLocation == 4) || (getCurrentLocation == 13))
        {
            return true;
        }
    }

    //munia
    if (getLocation == 6) {
        if ((x.at(7) == 0 || x.at(11) == 0) && (getCurrentLocation == 7) || (getCurrentLocation == 11))
        {
            return true;
        }
    }
    if (getLocation == 7) {
        if ((x.at(4) == 0 || x.at(8) == 0 || x.at(6) == 0) && (getCurrentLocation == 4) || (getCurrentLocation == 8) || (getCurrentLocation == 6))
        {
            return true;
        }
    }
    if (getLocation == 8) {
        if ((x.at(7) == 0 || x.at(12) == 0) && (getCurrentLocation == 7) || (getCurrentLocation == 12))
        {
            return true;
        }
    }

    if (getLocation == 9) {
        if ((x.at(0) == 0 || x.at(10) == 0 || x.at(21) == 0) && (getCurrentLocation == 0) || (getCurrentLocation == 10) || (getCurrentLocation == 21))
        {
            return true;
        }
    }

    if (getLocation == 10) {
        if ((x.at(3) == 0 || x.at(9) == 0 || x.at(18) == 0 || x.at(11) == 0) && (getCurrentLocation == 3) || (getCurrentLocation == 9) || (getCurrentLocation == 18) || (getCurrentLocation == 11))
        {
            return true;
        }
    }
    if (getLocation == 11) {
        if ((x.at(6) == 0 || x.at(10) == 0 || x.at(15) == 0) && (getCurrentLocation == 6) || (getCurrentLocation == 10) || (getCurrentLocation == 5) || (getCurrentLocation == 15))
        {
            return true;
        }
    }

    //mariam
    if (getLocation == 12) {
        if ((x.at(8) == 0 || x.at(17) == 0 || x.at(13) == 0) && (getCurrentLocation == 8) || (getCurrentLocation == 17) || (getCurrentLocation == 13))
        {
            return true;
        }
    }

    if (getLocation == 13) {
        if ((x.at(5) == 0 || x.at(20) == 0 || x.at(14) == 0 || x.at(12) == 0) && (getCurrentLocation == 5) || (getCurrentLocation == 20) || (getCurrentLocation == 14) || (getCurrentLocation == 12))
        {
            return true;
        }
    }

    if (getLocation == 14) {
        if ((x.at(2) == 0 || x.at(23) == 0 || x.at(13) == 0) && (getCurrentLocation == 2) || (getCurrentLocation == 23) || (getCurrentLocation == 13))
        {
            return true;
        }
    }
    if (getLocation == 15) {
        if ((x.at(11) == 0 || x.at(16) == 0) && (getCurrentLocation == 11) || (getCurrentLocation == 16))
        {
            return true;
        }
    }
    if (getLocation == 16) {
        if ((x.at(15) == 0 || x.at(17) == 0 || x.at(19) == 0) && (getCurrentLocation == 15) || (getCurrentLocation == 17) || (getCurrentLocation == 19))
        {
            return true;
        }
    }
    if (getLocation == 17) {
        if ((x.at(12) == 0 || x.at(16) == 0) && (getCurrentLocation == 12) || (getCurrentLocation == 16))
        {
            return true;
        }
    }

    //Fiza(29 lines)
    if (getLocation == 18) {
        if ((x.at(10) == 0 || x.at(19) == 0) && (getCurrentLocation == 10) || (getCurrentLocation == 19))
        {
            return true;
        }
    }
    if (getLocation == 19) {
        if ((x.at(18) == 0 || x.at(20) == 0 || x.at(22) == 0 || x.at(16) == 0) && (getCurrentLocation == 18) || (getCurrentLocation == 20) || (getCurrentLocation == 22) || (getCurrentLocation == 16))
        {
            return true;
        }
    }
    if (getLocation == 20) {
        if ((x.at(13) == 0 || x.at(19) == 0) && (getCurrentLocation == 13) || (getCurrentLocation == 19))
        {
            return true;
        }
    }
    if (getLocation == 21) {
        if ((x.at(9) == 0 || x.at(22) == 0) && (getCurrentLocation == 9) || (getCurrentLocation == 22))
        {
            return true;
        }
    }
    if (getLocation == 22) {
        if ((x.at(19) == 0 || x.at(21) == 0 || x.at(23) == 0) && (getCurrentLocation == 19) || (getCurrentLocation == 21) || (getCurrentLocation == 23))
        {
            return true;
        }
    }
    if (getLocation == 23) {
        if ((x.at(22) == 0 || x.at(14) == 0) && (getCurrentLocation == 22) || (getCurrentLocation == 14))
        {
            return true;
        }
    }

}

bool NineMensMorris::checkPlayerMillBlue(vector<int> x)
{
    //Harold
    if (playerOneMillLocation == 0) {
        if ((x.at(0) == 1 && x.at(1) == 1 && x.at(2) == 1) || (x.at(0) == 1 && x.at(9) == 1 && x.at(21) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 1) {
        if ((x.at(0) == 1 && x.at(1) == 1 && x.at(2) == 1) || (x.at(1) == 1 && x.at(4) == 1 && x.at(7) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 2) {
        if ((x.at(0) == 1 && x.at(1) == 1 && x.at(2) == 1) || (x.at(2) == 1 && x.at(14) == 1 && x.at(23) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 3) {
        if ((x.at(3) == 1 && x.at(10) == 1 && x.at(18) == 1) || (x.at(3) == 1 && x.at(4) == 1 && x.at(5) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 4) {
        if ((x.at(3) == 1 && x.at(4) == 1 && x.at(5) == 1) || (x.at(7) == 1 && x.at(4) == 1 && x.at(1) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 5) {
        if ((x.at(3) == 1 && x.at(4) == 1 && x.at(5) == 1) || (x.at(5) == 1 && x.at(13) == 1 && x.at(20) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 6) {
        if ((x.at(6) == 1 && x.at(11) == 1 && x.at(15) == 1) || (x.at(6) == 1 && x.at(7) == 1 && x.at(8) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 7) {
        if ((x.at(7) == 1 && x.at(4) == 1 && x.at(1) == 1) || (x.at(6) == 1 && x.at(8) == 1 && x.at(7) == 1))
        {
            return true;
        }
    }

    if (playerOneMillLocation == 8) {
        if ((x.at(6) == 1 && x.at(7) == 1 && x.at(8) == 1) || (x.at(8) == 1 && x.at(12) == 1 && x.at(17) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 9) {
        if ((x.at(0) == 1 && x.at(9) == 1 && x.at(21) == 1) || (x.at(9) == 1 && x.at(10) == 1 && x.at(11) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 10) {
        if ((x.at(9) == 1 && x.at(10) == 1 && x.at(11) == 1) || (x.at(3) == 1 && x.at(10) == 1 && x.at(18) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 11) {
        if ((x.at(6) == 1 && x.at(11) == 1 && x.at(15) == 1) || (x.at(9) == 1 && x.at(10) == 1 && x.at(11) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 12) {
        if ((x.at(8) == 1 && x.at(12) == 1 && x.at(17) == 1) || (x.at(12) == 1 && x.at(13) == 1 && x.at(14) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 13) {
        if ((x.at(5) == 1 && x.at(13) == 1 && x.at(20) == 1) || (x.at(12) == 1 && x.at(13) == 1 && x.at(14) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 14) {
        if ((x.at(2) == 1 && x.at(14) == 1 && x.at(23) == 1) || (x.at(12) == 1 && x.at(13) == 1 && x.at(14) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 15) {
        if ((x.at(6) == 1 && x.at(11) == 1 && x.at(15) == 1) || (x.at(15) == 1 && x.at(16) == 1 && x.at(17) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 16) {
        if ((x.at(16) == 1 && x.at(19) == 1 && x.at(22) == 1) || (x.at(15) == 1 && x.at(16) == 1 && x.at(17) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 17) {
        if ((x.at(8) == 1 && x.at(12) == 1 && x.at(17) == 1) || (x.at(15) == 1 && x.at(16) == 1 && x.at(17) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 18) {
        if ((x.at(18) == 1 && x.at(19) == 1 && x.at(20) == 1) || (x.at(18) == 1 && x.at(10) == 1 && x.at(3) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 19) {
        if ((x.at(18) == 1 && x.at(19) == 1 && x.at(20) == 1) || (x.at(16) == 1 && x.at(19) == 1 && x.at(22) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 20) {
        if ((x.at(18) == 1 && x.at(19) == 1 && x.at(20) == 1) || (x.at(5) == 1 && x.at(13) == 1 && x.at(20) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 21) {
        if ((x.at(21) == 1 && x.at(22) == 1 && x.at(23) == 1) || (x.at(0) == 1 && x.at(9) == 1 && x.at(21) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 22) {
        if ((x.at(21) == 1 && x.at(22) == 1 && x.at(23) == 1) || (x.at(16) == 1 && x.at(19) == 1 && x.at(22) == 1))
        {
            return true;
        }
    }
    if (playerOneMillLocation == 23) {
        if ((x.at(21) == 1 && x.at(22) == 1 && x.at(23) == 1) || (x.at(2) == 1 && x.at(14) == 1 && x.at(23) == 1))
        {
            return true;
        }
    }
}

bool NineMensMorris::checkPlayerMillRed(vector<int> x)
{
    //fiza
    if (playerTwoMillLocation == 0) {
        if ((x.at(0) == -1 && x.at(1) == -1 && x.at(2) == -1) || (x.at(0) == -1 && x.at(9) == -1 && x.at(21) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 1) {
        if ((x.at(0) == -1 && x.at(1) == -1 && x.at(2) == -1) || (x.at(1) == -1 && x.at(4) == -1 && x.at(7) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 2) {
        if ((x.at(0) == -1 && x.at(1) == -1 && x.at(2) == -1) || (x.at(2) == -1 && x.at(14) == -1 && x.at(23) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 3) {
        if ((x.at(3) == -1 && x.at(10) == -1 && x.at(18) == -1) || (x.at(3) == -1 && x.at(4) == -1 && x.at(5) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 4) {
        if ((x.at(1) == -1 && x.at(4) == -1 && x.at(7) == -1) || (x.at(3) == -1 && x.at(4) == -1 && x.at(5) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 5) {
        if ((x.at(3) == -1 && x.at(4) == -1 && x.at(5) == -1) || (x.at(5) == -1 && x.at(13) == -1 && x.at(20) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 6) {
        if ((x.at(6) == -1 && x.at(11) == -1 && x.at(15) == -1) || (x.at(6) == -1 && x.at(7) == -1 && x.at(8) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 7) {
        if ((x.at(6) == -1 && x.at(7) == -1 && x.at(8) == -1) || (x.at(7) == -1 && x.at(4) == -1 && x.at(1) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 8) {
        if ((x.at(6) == -1 && x.at(7) == -1 && x.at(8) == -1) || (x.at(8) == -1 && x.at(12) == -1 && x.at(17) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 9) {
        if ((x.at(0) == -1 && x.at(9) == -1 && x.at(21) == -1) || (x.at(9) == -1 && x.at(10) == -1 && x.at(11) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 10) {
        if ((x.at(9) == -1 && x.at(10) == -1 && x.at(11) == -1) || (x.at(3) == -1 && x.at(10) == -1 && x.at(18) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 11) {
        if ((x.at(6) == -1 && x.at(11) == -1 && x.at(15) == -1) || (x.at(9) == -1 && x.at(10) == -1 && x.at(11) == -1))
        {
            return true;
        }
    }

    //munia
    if (playerTwoMillLocation == 12) {
        if ((x.at(8) == -1 && x.at(12) == -1 && x.at(17) == -1) || (x.at(12) == -1 && x.at(13) == -1 && x.at(14) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 13) {
        if ((x.at(5) == -1 && x.at(13) == -1 && x.at(20) == -1) || (x.at(12) == -1 && x.at(13) == -1 && x.at(14) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 14) {
        if ((x.at(2) == -1 && x.at(14) == -1 && x.at(23) == -1) || (x.at(12) == -1 && x.at(13) == -1 && x.at(14) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 15) {
        if ((x.at(6) == -1 && x.at(11) == -1 && x.at(15) == -1) || (x.at(15) == -1 && x.at(16) == -1 && x.at(17) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 16) {
        if ((x.at(16) == -1 && x.at(19) == -1 && x.at(22) == -1) || (x.at(15) == -1 && x.at(16) == -1 && x.at(17) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 17) {
        if ((x.at(8) == -1 && x.at(12) == -1 && x.at(17) == -1) || (x.at(15) == -1 && x.at(16) == -1 && x.at(17) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 18) {
        if ((x.at(18) == -1 && x.at(19) == -1 && x.at(20) == -1) || (x.at(18) == -1 && x.at(10) == -1 && x.at(3) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 19) {
        if ((x.at(18) == -1 && x.at(19) == -1 && x.at(20) == -1) || (x.at(16) == -1 && x.at(19) == -1 && x.at(22) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 20) {
        if ((x.at(18) == -1 && x.at(19) == -1 && x.at(20) == -1) || (x.at(5) == -1 && x.at(13) == -1 && x.at(20) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 21) {
        if ((x.at(21) == -1 && x.at(22) == -1 && x.at(23) == -1) || (x.at(0) == -1 && x.at(9) == -1 && x.at(21) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 22) {
        if ((x.at(21) == -1 && x.at(22) == -1 && x.at(23) == -1) || (x.at(16) == -1 && x.at(19) == -1 && x.at(22) == -1))
        {
            return true;
        }
    }
    if (playerTwoMillLocation == 23) {
        if ((x.at(21) == -1 && x.at(22) == -1 && x.at(23) == -1) || (x.at(2) == -1 && x.at(14) == -1 && x.at(23) == -1))
        {
            return true;
        }
    }
}


bool NineMensMorris::checkRemoveBlue(vector<int> x)
{
    //fiza
    if (removeLocation == 0) {
        if ((x.at(removeLocation) == -1 && x.at(1) == -1 && x.at(2) == -1) || (x.at(removeLocation) == -1 && x.at(9) == -1 && x.at(21) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 1) {
        if ((x.at(0) == -1 && x.at(removeLocation) == -1 && x.at(2) == -1) || (x.at(removeLocation) == -1 && x.at(4) == -1 && x.at(7) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 2) {
        if ((x.at(0) == -1 && x.at(1) == -1 && x.at(removeLocation) == -1) || (x.at(removeLocation) == -1 && x.at(14) == -1 && x.at(23) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 3) {
        if ((x.at(removeLocation) == -1 && x.at(10) == -1 && x.at(18) == -1) || (x.at(removeLocation) == -1 && x.at(4) == -1 && x.at(5) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 4) {
        if ((x.at(1) == -1 && x.at(removeLocation) == -1 && x.at(7) == -1) || (x.at(3) == -1 && x.at(removeLocation) == -1 && x.at(5) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 5) {
        if ((x.at(3) == -1 && x.at(4) == -1 && x.at(removeLocation) == -1) || (x.at(removeLocation) == -1 && x.at(13) == -1 && x.at(20) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 6) {
        if ((x.at(removeLocation) == -1 && x.at(11) == -1 && x.at(15) == -1) || (x.at(removeLocation) == -1 && x.at(7) == -1 && x.at(8) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 7) {
        if ((x.at(6) == -1 && x.at(removeLocation) == -1 && x.at(8) == -1) || (x.at(removeLocation) == -1 && x.at(4) == -1 && x.at(1) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 8) {
        if ((x.at(6) == -1 && x.at(7) == -1 && x.at(removeLocation) == -1) || (x.at(removeLocation) == -1 && x.at(12) == -1 && x.at(17) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 9) {
        if ((x.at(0) == -1 && x.at(removeLocation) == -1 && x.at(21) == -1) || (x.at(removeLocation) == -1 && x.at(10) == -1 && x.at(11) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 10) {
        if ((x.at(9) == -1 && x.at(removeLocation) == -1 && x.at(11) == -1) || (x.at(3) == -1 && x.at(removeLocation) == -1 && x.at(18) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 11) {
        if ((x.at(6) == -1 && x.at(removeLocation) == -1 && x.at(15) == -1) || (x.at(9) == -1 && x.at(10) == -1 && x.at(removeLocation) == -1))
        {
            return true;
        }
    }

    //munia
    if (removeLocation == 12) {
        if ((x.at(8) == -1 && x.at(removeLocation) == -1 && x.at(17) == -1) || (x.at(removeLocation) == -1 && x.at(13) == -1 && x.at(14) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 13) {
        if ((x.at(5) == -1 && x.at(removeLocation) == -1 && x.at(20) == -1) || (x.at(12) == -1 && x.at(removeLocation) == -1 && x.at(14) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 14) {
        if ((x.at(2) == -1 && x.at(removeLocation) == -1 && x.at(23) == -1) || (x.at(12) == -1 && x.at(13) == -1 && x.at(removeLocation) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 15) {
        if ((x.at(6) == -1 && x.at(11) == -1 && x.at(removeLocation) == -1) || (x.at(removeLocation) == -1 && x.at(16) == -1 && x.at(17) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 16) {
        if ((x.at(removeLocation) == -1 && x.at(19) == -1 && x.at(22) == -1) || (x.at(15) == -1 && x.at(removeLocation) == -1 && x.at(17) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 17) {
        if ((x.at(8) == -1 && x.at(12) == -1 && x.at(removeLocation) == -1) || (x.at(15) == -1 && x.at(16) == -1 && x.at(removeLocation) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 18) {
        if ((x.at(removeLocation) == -1 && x.at(19) == -1 && x.at(20) == -1) || (x.at(removeLocation) == -1 && x.at(10) == -1 && x.at(3) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 19) {
        if ((x.at(18) == -1 && x.at(removeLocation) == -1 && x.at(20) == -1) || (x.at(16) == -1 && x.at(removeLocation) == -1 && x.at(22) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 20) {
        if ((x.at(18) == -1 && x.at(19) == -1 && x.at(removeLocation) == -1) || (x.at(5) == -1 && x.at(13) == -1 && x.at(removeLocation) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 21) {
        if ((x.at(removeLocation) == -1 && x.at(22) == -1 && x.at(23) == -1) || (x.at(0) == -1 && x.at(9) == -1 && x.at(removeLocation) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 22) {
        if ((x.at(21) == -1 && x.at(removeLocation) == -1 && x.at(23) == -1) || (x.at(16) == -1 && x.at(19) == -1 && x.at(removeLocation) == -1))
        {
            return true;
        }
    }
    if (removeLocation == 23) {
        if ((x.at(21) == -1 && x.at(22) == -1 && x.at(removeLocation) == -1) || (x.at(2) == -1 && x.at(14) == -1 && x.at(removeLocation) == -1))
        {
            return true;
        }
    }
}

bool NineMensMorris::checkRemoveRed(vector<int> x)
{
    //fiza
    if (removeLocation == 0) {
        if ((x.at(removeLocation) == 1 && x.at(1) == 1 && x.at(2) == 1) || (x.at(removeLocation) == 1 && x.at(9) == 1 && x.at(21) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 1) {
        if ((x.at(0) == 1 && x.at(removeLocation) == 1 && x.at(2) == 1) || (x.at(removeLocation) == 1 && x.at(4) == 1 && x.at(7) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 2) {
        if ((x.at(0) == 1 && x.at(1) == 1 && x.at(removeLocation) == 1) || (x.at(removeLocation) == 1 && x.at(14) == 1 && x.at(23) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 3) {
        if ((x.at(removeLocation) == 1 && x.at(10) == 1 && x.at(18) == 1) || (x.at(removeLocation) == 1 && x.at(4) == 1 && x.at(5) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 4) {
        if ((x.at(1) == 1 && x.at(removeLocation) == 1 && x.at(7) == 1) || (x.at(3) == 1 && x.at(removeLocation) == 1 && x.at(5) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 5) {
        if ((x.at(3) == 1 && x.at(4) == 1 && x.at(removeLocation) == 1) || (x.at(removeLocation) == 1 && x.at(13) == 1 && x.at(20) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 6) {
        if ((x.at(removeLocation) == 1 && x.at(11) == 1 && x.at(15) == 1) || (x.at(removeLocation) == 1 && x.at(7) == 1 && x.at(8) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 7) {
        if ((x.at(6) == 1 && x.at(removeLocation) == 1 && x.at(8) == 1) || (x.at(removeLocation) == 1 && x.at(4) == 1 && x.at(1) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 8) {
        if ((x.at(6) == 1 && x.at(7) == 1 && x.at(removeLocation) == 1) || (x.at(removeLocation) == 1 && x.at(12) == 1 && x.at(17) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 9) {
        if ((x.at(0) == 1 && x.at(removeLocation) == 1 && x.at(21) == 1) || (x.at(removeLocation) == 1 && x.at(10) == 1 && x.at(11) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 10) {
        if ((x.at(9) == 1 && x.at(removeLocation) == 1 && x.at(11) == 1) || (x.at(3) == 1 && x.at(removeLocation) == 1 && x.at(18) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 11) {
        if ((x.at(6) == 1 && x.at(removeLocation) == 1 && x.at(15) == 1) || (x.at(9) == 1 && x.at(10) == 1 && x.at(removeLocation) == 1))
        {
            return true;
        }
    }

    //munia
    if (removeLocation == 12) {
        if ((x.at(8) == 1 && x.at(removeLocation) == 1 && x.at(17) == 1) || (x.at(removeLocation) == 1 && x.at(13) == 1 && x.at(14) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 13) {
        if ((x.at(5) == 1 && x.at(removeLocation) == 1 && x.at(20) == 1) || (x.at(12) == 1 && x.at(removeLocation) == 1 && x.at(14) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 14) {
        if ((x.at(2) == 1 && x.at(removeLocation) == 1 && x.at(23) == 1) || (x.at(12) == 1 && x.at(13) == 1 && x.at(removeLocation) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 15) {
        if ((x.at(6) == 1 && x.at(11) == 1 && x.at(removeLocation) == 1) || (x.at(removeLocation) == 1 && x.at(16) == 1 && x.at(17) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 16) {
        if ((x.at(removeLocation) == 1 && x.at(19) == 1 && x.at(22) == 1) || (x.at(15) == 1 && x.at(removeLocation) == 1 && x.at(17) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 17) {
        if ((x.at(8) == 1 && x.at(12) == 1 && x.at(removeLocation) == 1) || (x.at(15) == 1 && x.at(16) == 1 && x.at(removeLocation) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 18) {
        if ((x.at(removeLocation) == 1 && x.at(19) == 1 && x.at(20) == 1) || (x.at(removeLocation) == 1 && x.at(10) == 1 && x.at(3) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 19) {
        if ((x.at(18) == 1 && x.at(removeLocation) == 1 && x.at(20) == 1) || (x.at(16) == 1 && x.at(removeLocation) == 1 && x.at(22) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 20) {
        if ((x.at(18) == 1 && x.at(19) == 1 && x.at(removeLocation) == 1) || (x.at(5) == 1 && x.at(13) == 1 && x.at(removeLocation) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 21) {
        if ((x.at(removeLocation) == 1 && x.at(22) == 1 && x.at(23) == 1) || (x.at(0) == 1 && x.at(9) == 1 && x.at(removeLocation) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 22) {
        if ((x.at(21) == 1 && x.at(removeLocation) == 1 && x.at(23) == 1) || (x.at(16) == 1 && x.at(19) == 1 && x.at(removeLocation) == 1))
        {
            return true;
        }
    }
    if (removeLocation == 23) {
        if ((x.at(21) == 1 && x.at(22) == 1 && x.at(removeLocation) == 1) || (x.at(2) == 1 && x.at(14) == 1 && x.at(removeLocation) == 1))
        {
            return true;
        }
    }
}

bool NineMensMorris::checkLegalMove(vector<int> x, int player)
{
    vector<bool> blueLegalMove(24, false);
    vector<bool> redLegalMove(24, false);

    vector<bool> blue;
    vector<bool> red;


    if (x.at(0) == player)
        if (x.at(1) != 0 && x.at(9) != 0)
            if (player == 1)
                blueLegalMove.at(0) = true;
            else
                redLegalMove.at(0) = true;
    if (x.at(1) == player)
        if (x.at(0) != 0 && x.at(2) != 0 && x.at(4) != 0)
            if (player == 1)
                blueLegalMove.at(1) = true;
            else
                redLegalMove.at(1) = true;
    if (x.at(2) == player)
        if (x.at(1) != 0 && x.at(14) != 0)
            if (player == 1)
                blueLegalMove.at(2) = true;
            else
                redLegalMove.at(2) = true;
    if (x.at(3) == player)
        if (x.at(10) != 0 && x.at(3) != 0)
            if (player == 1)
                blueLegalMove.at(3) = true;
            else
                redLegalMove.at(3) = true;
    if (x.at(4) == player)
        if (x.at(1) != 0 && x.at(3) != 0 && x.at(5) != 0 && x.at(7) != 0)
            if (player == 1)
                blueLegalMove.at(4) = true;
            else
                redLegalMove.at(4) = true;
    if (x.at(5) == player)
        if (x.at(4) != 0 && x.at(13) != 0)
            if (player == 1)
                blueLegalMove.at(5) = true;
            else
                redLegalMove.at(5) = true;
    if (x.at(6) == player)
        if (x.at(7) != 0 && x.at(11) != 0)
            if (player == 1)
                blueLegalMove.at(6) = true;
            else
                redLegalMove.at(6) = true;
    if (x.at(7) == player)
        if (x.at(4) != 0 && x.at(6) != 0 && x.at(8) != 0)
            if (player == 1)
                blueLegalMove.at(7) = true;
            else
                redLegalMove.at(7) = true;
    if (x.at(8) == player)
        if (x.at(7) != 0 && x.at(12) != 0)
            if (player == 1)
                blueLegalMove.at(8) = true;
            else
                redLegalMove.at(8) = true;
    if (x.at(9) == player)
        if (x.at(0) != 0 && x.at(10) != 0 && x.at(21) != 0)
            if (player == 1)
                blueLegalMove.at(9) = true;
            else
                redLegalMove.at(9) = true;
    if (x.at(10) == player)
        if (x.at(3) != 0 && x.at(9) != 0 && x.at(11) != 0 && x.at(18) != 0)
            if (player == 1)
                blueLegalMove.at(10) = true;
            else
                redLegalMove.at(10) = true;
    if (x.at(11) == player)
        if (x.at(6) != 0 && x.at(10) != 0 && x.at(15) != 0)
            if (player == 1)
                blueLegalMove.at(11) = true;
            else
                redLegalMove.at(11) = true;
    if (x.at(12) == player)
        if (x.at(8) != 0 && x.at(13) != 0 && x.at(17) != 0)
            if (player == 1)
                blueLegalMove.at(12) = true;
            else
                redLegalMove.at(12) = true;
    if (x.at(13) == player)
        if (x.at(5) != 0 && x.at(12) != 0 && x.at(14) != 0 && x.at(20) != 0)
            if (player == 1)
                blueLegalMove.at(13) = true;
            else
                redLegalMove.at(13) = true;
    if (x.at(14) == player)
        if (x.at(2) != 0 && x.at(13) != 0 && x.at(23) != 0)
            if (player == 1)
                blueLegalMove.at(14) = true;
            else
                redLegalMove.at(14) = true;
    if (x.at(15) == player)
        if (x.at(11) != 0 && x.at(16) != 0)
            if (player == 1)
                blueLegalMove.at(15) = true;
            else
                redLegalMove.at(15) = true;
    if (x.at(16) == player)
        if (x.at(15) != 0 && x.at(17) != 0)
            if (player == 1)
                blueLegalMove.at(16) = true;
            else
                redLegalMove.at(16) = true;
    if (x.at(17) == player)
        if (x.at(12) != 0 && x.at(16) != 0)
            if (player == 1)
                blueLegalMove.at(17) = true;
            else
                redLegalMove.at(17) = true;
    if (x.at(18) == player)
        if (x.at(10) != 0 && x.at(19) != 0)
            if (player == 1)
                blueLegalMove.at(18) = true;
            else
                redLegalMove.at(18) = true;
    if (x.at(19) == player)
        if (x.at(16) != 0 && x.at(18) != 0 && x.at(20) != 0 && x.at(22) != 0)
            if (player == 1)
                blueLegalMove.at(19) = true;
            else
                redLegalMove.at(19) = true;
    if (x.at(20) == player)
        if (x.at(13) != 0 && x.at(19) != 0)
            if (player == 1)
                blueLegalMove.at(20) = true;
            else
                redLegalMove.at(20) = true;
    if (x.at(21) == player)
        if (x.at(9) != 0 && x.at(22) != 0)
            if (player == 1)
                blueLegalMove.at(21) = true;
            else
                redLegalMove.at(21) = true;
    if (x.at(22) == player)
        if (x.at(19) != 0 && x.at(21) != 0 && x.at(23) != 0)
            if (player == 1)
                blueLegalMove.at(22) = true;
            else
                redLegalMove.at(22) = true;
    if (x.at(23) == player)
        if (x.at(22) != 0 && x.at(14) != 0)
            if (player == 1)
                blueLegalMove.at(23) = true;
            else
                redLegalMove.at(23) = true;



    if (player == 1)
    {
        for (int i = 0; i < blueLegalMove.size(); i++)
        {
            if (x.at(i) == 1 && blueLegalMove.at(i) == true)
                blue.push_back(true);
            else if (x.at(i) == 0 || x.at(i) == -1)
                continue;
            else
                blue.push_back(false);
        }
    }
    else if (player == -1)
    {
        for (int i = 0; i < redLegalMove.size(); i++)
        {
            if (x.at(i) == -1 && redLegalMove.at(i) == true)
                red.push_back(true);
            else if (x.at(i) == 0 || x.at(i) == 1)
                continue;
            else
                red.push_back(false);
        }
    }

    if (player == 1)
    {
        if (all_of(blue.begin(), blue.end(), [](bool v) { return v; }))
        {
            blue.clear();
            return true;
        }
        else
        {
            blue.clear();
            return false;
        }
    }
    else if (player == -1)
    {
        if (all_of(red.begin(), red.end(), [](bool v) { return v; }))
        {
            red.clear();
            return true;
        }
        else
        {
            red.clear();
            return false;
        }
    }
}
































void NineMensMorris::drawTheBoard(wxDC& dc)
{

    wxSize frameSize = GetClientSize();

    dc.SetBrush(*wxWHITE_BRUSH);
    dc.SetPen(wxPen(wxColor(0, 0, 0), 2));

    //Draw with float coordinates
    //Inside
    dc.DrawCircle(wxPoint(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.35), circleSize);            //d5
    dc.DrawCircle(wxPoint(frameSize.x * 0.45 - (circleSize / 2), frameSize.y * 0.35), circleSize);           //c5
    dc.DrawCircle(wxPoint(frameSize.x * 0.55 - (circleSize / 2), frameSize.y * 0.35), circleSize);           //e5

    dc.DrawCircle(wxPoint(frameSize.x * 0.45 - (circleSize / 2), frameSize.y * 0.45), circleSize);           //c4
    dc.DrawCircle(wxPoint(frameSize.x * 0.55 - (circleSize / 2), frameSize.y * 0.45), circleSize);           //e4

    dc.DrawCircle(wxPoint(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.55), circleSize);            //d3
    dc.DrawCircle(wxPoint(frameSize.x * 0.45 - (circleSize / 2), frameSize.y * 0.55), circleSize);           //c3
    dc.DrawCircle(wxPoint(frameSize.x * 0.55 - (circleSize / 2), frameSize.y * 0.55), circleSize);           //e3

    //Inbetween
    dc.DrawCircle(wxPoint(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.23), circleSize);            //d6
    dc.DrawCircle(wxPoint(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.67), circleSize);            //d2

    dc.DrawCircle(wxPoint(frameSize.x * 0.62 - (circleSize / 2), frameSize.y * 0.23), circleSize);           //f6
    dc.DrawCircle(wxPoint(frameSize.x * 0.38 - (circleSize / 2), frameSize.y * 0.67), circleSize);           //b2

    dc.DrawCircle(wxPoint(frameSize.x * 0.38 - (circleSize / 2), frameSize.y * 0.23), circleSize);           //b6
    dc.DrawCircle(wxPoint(frameSize.x * 0.62 - (circleSize / 2), frameSize.y * 0.67), circleSize);           //f2

    dc.DrawCircle(wxPoint(frameSize.x * 0.62 - (circleSize / 2), frameSize.y * 0.45), circleSize);           //f4
    dc.DrawCircle(wxPoint(frameSize.x * 0.38 - (circleSize / 2), frameSize.y * 0.45), circleSize);           //b4

    //Outside
    dc.DrawCircle(wxPoint(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.11), circleSize);            //d7                 
    dc.DrawCircle(wxPoint(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.79), circleSize);            //d1

    dc.DrawCircle(wxPoint(frameSize.x * 0.69 - (circleSize / 2), frameSize.y * 0.11), circleSize);           //g7 
    dc.DrawCircle(wxPoint(frameSize.x * 0.31 - (circleSize / 2), frameSize.y * 0.79), circleSize);           //a1

    dc.DrawCircle(wxPoint(frameSize.x * 0.31 - (circleSize / 2), frameSize.y * 0.11), circleSize);           //a7
    dc.DrawCircle(wxPoint(frameSize.x * 0.69 - (circleSize / 2), frameSize.y * 0.79), circleSize);           //g1

    dc.DrawCircle(wxPoint(frameSize.x * 0.31 - (circleSize / 2), frameSize.y * 0.45), circleSize);           //a4
    dc.DrawCircle(wxPoint(frameSize.x * 0.69 - (circleSize / 2), frameSize.y * 0.45), circleSize);           //g4


    //Outside
    dc.SetPen(wxPen(wxColor(0, 0, 0), 3)); // black line, 3 pixels thick
    dc.DrawLine(frameSize.x * 0.31 + (circleSize / 2), frameSize.y * 0.11, frameSize.x * 0.5 - circleSize * 1.5, frameSize.y * 0.11); // a7 to d7
    dc.DrawLine(frameSize.x * 0.5 + (circleSize / 2), frameSize.y * 0.11, frameSize.x * 0.69 - circleSize * 1.5, frameSize.y * 0.11); // d7 to g7

    dc.DrawLine(frameSize.x * 0.31 + (circleSize / 2), frameSize.y * 0.79, frameSize.x * 0.5 - circleSize * 1.5, frameSize.y * 0.79); // a1 to d1
    dc.DrawLine(frameSize.x * 0.5 + (circleSize / 2), frameSize.y * 0.79, frameSize.x * 0.69 - circleSize * 1.5, frameSize.y * 0.79); // d1 to g1

    dc.DrawLine(frameSize.x * 0.31 - (circleSize / 2), frameSize.y * 0.11 + circleSize, frameSize.x * 0.31 - (circleSize / 2), frameSize.y * 0.45 - circleSize); // a7 to a4
    dc.DrawLine(frameSize.x * 0.31 - (circleSize / 2), frameSize.y * 0.45 + circleSize, frameSize.x * 0.31 - (circleSize / 2), frameSize.y * 0.79 - circleSize); // a4 to a1

    dc.DrawLine(frameSize.x * 0.69 - (circleSize / 2), frameSize.y * 0.11 + circleSize, frameSize.x * 0.69 - (circleSize / 2), frameSize.y * 0.45 - circleSize); // g7 to g4
    dc.DrawLine(frameSize.x * 0.69 - (circleSize / 2), frameSize.y * 0.45 + circleSize, frameSize.x * 0.69 - (circleSize / 2), frameSize.y * 0.79 - circleSize); // g4 to g1

    //Inbetween
    dc.DrawLine(frameSize.x * 0.38 - (circleSize / 2), frameSize.y * 0.23 + circleSize, frameSize.x * 0.38 - (circleSize / 2), frameSize.y * 0.45 - circleSize); // b6 to b4
    dc.DrawLine(frameSize.x * 0.38 - (circleSize / 2), frameSize.y * 0.45 + circleSize, frameSize.x * 0.38 - (circleSize / 2), frameSize.y * 0.67 - circleSize); // b4 to b2

    dc.DrawLine(frameSize.x * 0.62 - (circleSize / 2), frameSize.y * 0.23 + circleSize, frameSize.x * 0.62 - (circleSize / 2), frameSize.y * 0.45 - circleSize); // f6 to f4
    dc.DrawLine(frameSize.x * 0.62 - (circleSize / 2), frameSize.y * 0.45 + circleSize, frameSize.x * 0.62 - (circleSize / 2), frameSize.y * 0.67 - circleSize); // f4 to f2

    dc.DrawLine(frameSize.x * 0.38 + (circleSize / 2), frameSize.y * 0.23, frameSize.x * 0.5 - circleSize * 1.5, frameSize.y * 0.23); // b6 to d6
    dc.DrawLine(frameSize.x * 0.5 + (circleSize / 2), frameSize.y * 0.23, frameSize.x * 0.62 - circleSize * 1.5, frameSize.y * 0.23); // d6 to f6

    dc.DrawLine(frameSize.x * 0.38 + (circleSize / 2), frameSize.y * 0.67, frameSize.x * 0.5 - circleSize * 1.5, frameSize.y * 0.67); // b2 to d2
    dc.DrawLine(frameSize.x * 0.5 + (circleSize / 2), frameSize.y * 0.67, frameSize.x * 0.62 - circleSize * 1.5, frameSize.y * 0.67); // d2 to f2

    //Inside
    dc.DrawLine(frameSize.x * 0.45 - (circleSize / 2), frameSize.y * 0.35 + circleSize, frameSize.x * 0.45 - (circleSize / 2), frameSize.y * 0.45 - circleSize); // c5 to c4
    dc.DrawLine(frameSize.x * 0.45 - (circleSize / 2), frameSize.y * 0.45 + circleSize, frameSize.x * 0.45 - (circleSize / 2), frameSize.y * 0.55 - circleSize); // c4 to c3

    dc.DrawLine(frameSize.x * 0.55 - (circleSize / 2), frameSize.y * 0.35 + circleSize, frameSize.x * 0.55 - (circleSize / 2), frameSize.y * 0.45 - circleSize); // e5 to e4
    dc.DrawLine(frameSize.x * 0.55 - (circleSize / 2), frameSize.y * 0.45 + circleSize, frameSize.x * 0.55 - (circleSize / 2), frameSize.y * 0.55 - circleSize); // e4 to e3

    dc.DrawLine(frameSize.x * 0.45 + (circleSize / 2), frameSize.y * 0.35, frameSize.x * 0.5 - circleSize * 1.5, frameSize.y * 0.35); // c5 to d5
    dc.DrawLine(frameSize.x * 0.5 + (circleSize / 2), frameSize.y * 0.35, frameSize.x * 0.55 - circleSize * 1.5, frameSize.y * 0.35); // d5 to e5

    dc.DrawLine(frameSize.x * 0.45 + (circleSize / 2), frameSize.y * 0.55, frameSize.x * 0.5 - circleSize * 1.5, frameSize.y * 0.55); // c3 to d3
    dc.DrawLine(frameSize.x * 0.5 + (circleSize / 2), frameSize.y * 0.55, frameSize.x * 0.55 - circleSize * 1.5, frameSize.y * 0.55); // d3 to e3

    //The others
    dc.DrawLine(frameSize.x * 0.31 + (circleSize / 2), frameSize.y * 0.45, frameSize.x * 0.38 - circleSize * 1.5, frameSize.y * 0.45); // a4 to b4
    dc.DrawLine(frameSize.x * 0.38 + (circleSize / 2), frameSize.y * 0.45, frameSize.x * 0.45 - circleSize * 1.5, frameSize.y * 0.45); // b4 to c4

    dc.DrawLine(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.11 + circleSize, frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.23 - circleSize); // d7 to d6
    dc.DrawLine(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.23 + circleSize, frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.35 - circleSize); // d6 to d5

    dc.DrawLine(frameSize.x * 0.55 + (circleSize / 2), frameSize.y * 0.45, frameSize.x * 0.62 - circleSize * 1.5, frameSize.y * 0.45); // e4 to f4
    dc.DrawLine(frameSize.x * 0.62 + (circleSize / 2), frameSize.y * 0.45, frameSize.x * 0.69 - circleSize * 1.5, frameSize.y * 0.45); // f4 to g4

    dc.DrawLine(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.55 + circleSize, frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.67 - circleSize); // d3 to d2
    dc.DrawLine(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.67 + circleSize, frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.79 - circleSize); // d2 to d1



    double x;
    double y;
    string Player;
    string Enemies;

    x = frameSize.x * .05;
    y = frameSize.y * .1;

    //DRAW PIECES LEFT PLAYER 1
    Player = to_string(playerOnePiecesLeft);
    dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
    dc.DrawRectangle(x, y, width, height);
    dc.DrawText(wxString(Left), x + 5, y + 10);
    dc.DrawText(wxString(Player), x + 130, y + 10);

    //DRAW PIECES TAKEN PLAYER 1
    dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
    dc.DrawRectangle(x, y + 100, width, height);
    dc.DrawText(wxString(Left2), x + 5, y + 110);
    Enemies = to_string(playerOneEnemies);
    dc.DrawText(wxString(Enemies), x + 130, y + 110);


    x = frameSize.x * .85;
    y = frameSize.y * .1;

    //DRAW PIECES LEFT PLAYER 2
    Player = to_string(playerTwoPiecesLeft);
    dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
    dc.DrawRectangle(x, y, width, height);
    dc.DrawText(wxString(Right), x + 5, y + 10);
    dc.DrawText(wxString(Player), x + 130, y + 10);

    //DRAW PIECES TAKEN PLAYER 2
    dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
    dc.DrawRectangle(x, y + 100, width, height);
    dc.DrawText(wxString(Left2), x + 5, y + 110);
    Enemies = to_string(playerTwoEnemies);
    dc.DrawText(wxString(Enemies), x + 130, y + 110);





    //DRAW TURNS AND ACTIONS

    /*
    x = frameSize.x * .49;
    y = frameSize.y * .75;

    //DRAE MORE STUFF
    dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
    dc.DrawRectangle(x - (width * 2) / 2, y + 100, width * 2 , height);
    dc.DrawText(wxString("Current Player Turn 1"), x + 5, y + 110);

    */

    //draw a rectangle
    //dc.SetBrush(*wxBLUE_BRUSH); // blue filling
    //dc.SetPen(wxPen(wxColor(255, 175, 175), 10)); // 10-pixels-thick pink outline


    // draw a line
    //dc.SetPen(wxPen(wxColor(0, 0, 0), 3)); // black line, 3 pixels thick
    //dc.DrawLine(300, 100, 700, 300); // draw line across the rectangle

    //OVERRIDE COLORS

    //dc.SetBrush(/**wxGREEN_BRUSH*/wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK)); // green filling
    //dc.SetPen(wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK))); // 5-pixels-thick red outline
    //dc.DrawCircle(wxPoint(400, 450), 25);
}

void NineMensMorris::DrawPlayerOnePieces(wxDC& dc)
{
    wxSize frameSize = GetClientSize();
    double x = frameSize.x * .05;
    double y = frameSize.y * .1;

    dc.SetBrush(*wxWHITE_BRUSH);
    dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
    dc.DrawRectangle(x, y, width, height);

    string Player = to_string(playerOnePiecesLeft);
    dc.DrawText(wxString(Left), x + 5, y + 10);
    dc.DrawText(wxString(Player), x + 130, y + 10);
}


void NineMensMorris::DrawPlayerTwoPieces(wxDC& dc)
{
    wxSize frameSize = GetClientSize();
    double x = frameSize.x * .85;
    double y = frameSize.y * .1;

    dc.SetBrush(*wxWHITE_BRUSH);
    dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
    dc.DrawRectangle(x, y, width, height);

    string Player = to_string(playerTwoPiecesLeft);
    dc.DrawText(wxString(Right), x + 5, y + 10);
    dc.DrawText(wxString(Player), x + 130, y + 10);
}



void NineMensMorris::DrawPlayerOnePiecesTaken(wxDC& dc)
{
    wxSize frameSize = GetClientSize();
    double x = frameSize.x * .05;
    double y = frameSize.y * .1;
    string Enemies;

    dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
    dc.DrawRectangle(x, y + 100, width, height);
    dc.DrawText(wxString(Left2), x + 5, y + 110);
    Enemies = to_string(playerOneEnemies);
    dc.DrawText(wxString(Enemies), x + 130, y + 110);
}

void NineMensMorris::DrawPlayerTwoPiecesTaken(wxDC& dc)
{
    wxSize frameSize = GetClientSize();
    double x = frameSize.x * .85;
    double y = frameSize.y * .1;
    string Enemies;

    dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
    dc.DrawRectangle(x, y + 100, width, height);
    dc.DrawText(wxString(Left2), x + 5, y + 110);
    Enemies = to_string(playerTwoEnemies);
    dc.DrawText(wxString(Enemies), x + 130, y + 110);
}

























void NineMensMorris::drawCircle(wxDC& dc, float x, float y, wxBrush color)
{
    dc.SetBrush(color);
    dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
    dc.DrawCircle(wxPoint(x, y), circleSize);
}

void NineMensMorris::makeSquareVector()
{
    wxSize frameSize = GetClientSize();

    //array of strings "a0"

    //arrange the arrray in order 0-23 
    //make an array of booleans 0-23 under draw circle

    squareCoordinates.push_back(make_tuple(frameSize.x * 0.31 - circleSize * 1.42, frameSize.x * 0.31 - circleSize * 1.42 + 50, frameSize.y * 0.79 - circleSize * 0.95, frameSize.y * 0.79 - circleSize * 0.95 + 50));  //a1
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.5 - circleSize * 1.42, frameSize.x * 0.5 - circleSize * 1.42 + 50, frameSize.y * 0.79 - circleSize * 0.95, frameSize.y * 0.79 - circleSize * 0.95 + 50));    //d1
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.69 - circleSize * 1.42, frameSize.x * 0.69 - circleSize * 1.42 + 50, frameSize.y * 0.79 - circleSize * 0.95, frameSize.y * 0.79 - circleSize * 0.95 + 50));  //g1

    squareCoordinates.push_back(make_tuple(frameSize.x * 0.38 - circleSize * 1.42, frameSize.x * 0.38 - circleSize * 1.42 + 50, frameSize.y * 0.67 - circleSize * 0.95, frameSize.y * 0.67 - circleSize * 0.95 + 50));  //b2
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.5 - circleSize * 1.42, frameSize.x * 0.5 - circleSize * 1.42 + 50, frameSize.y * 0.67 - circleSize * 0.95, frameSize.y * 0.67 - circleSize * 0.95 + 50));    //d2
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.62 - circleSize * 1.42, frameSize.x * 0.62 - circleSize * 1.42 + 50, frameSize.y * 0.67 - circleSize * 0.95, frameSize.y * 0.67 - circleSize * 0.95 + 50));  //f2

    squareCoordinates.push_back(make_tuple(frameSize.x * 0.45 - circleSize * 1.42, frameSize.x * 0.45 - circleSize * 1.42 + 50, frameSize.y * 0.55 - circleSize * 0.95, frameSize.y * 0.55 - circleSize * 0.95 + 50));  //c3
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.5 - circleSize * 1.42, frameSize.x * 0.5 - circleSize * 1.42 + 50, frameSize.y * 0.55 - circleSize * 0.95, frameSize.y * 0.55 - circleSize * 0.95 + 50));    //d3
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.55 - circleSize * 1.42, frameSize.x * 0.55 - circleSize * 1.42 + 50, frameSize.y * 0.55 - circleSize * 0.95, frameSize.y * 0.55 - circleSize * 0.95 + 50));  //e3

    squareCoordinates.push_back(make_tuple(frameSize.x * 0.31 - circleSize * 1.42, frameSize.x * 0.31 - circleSize * 1.42 + 50, frameSize.y * 0.45 - circleSize * 0.95, frameSize.y * 0.45 - circleSize * 0.95 + 50));  //a4
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.38 - circleSize * 1.42, frameSize.x * 0.38 - circleSize * 1.42 + 50, frameSize.y * 0.45 - circleSize * 0.95, frameSize.y * 0.45 - circleSize * 0.95 + 50));  //b4
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.45 - circleSize * 1.42, frameSize.x * 0.45 - circleSize * 1.42 + 50, frameSize.y * 0.45 - circleSize * 0.95, frameSize.y * 0.45 - circleSize * 0.95 + 50));  //c4
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.55 - circleSize * 1.42, frameSize.x * 0.55 - circleSize * 1.42 + 50, frameSize.y * 0.45 - circleSize * 0.95, frameSize.y * 0.45 - circleSize * 0.95 + 50));  //e4
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.62 - circleSize * 1.42, frameSize.x * 0.62 - circleSize * 1.42 + 50, frameSize.y * 0.45 - circleSize * 0.95, frameSize.y * 0.45 - circleSize * 0.95 + 50));  //f4
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.69 - circleSize * 1.42, frameSize.x * 0.69 - circleSize * 1.42 + 50, frameSize.y * 0.45 - circleSize * 0.95, frameSize.y * 0.45 - circleSize * 0.95 + 50));  //g4

    squareCoordinates.push_back(make_tuple(frameSize.x * 0.45 - circleSize * 1.42, frameSize.x * 0.45 - circleSize * 1.42 + 50, frameSize.y * 0.35 - circleSize * 0.95, frameSize.y * 0.35 - circleSize * 0.95 + 50));  //c5
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.5 - circleSize * 1.42, frameSize.x * 0.5 - circleSize * 1.42 + 50, frameSize.y * 0.35 - circleSize * 0.95, frameSize.y * 0.35 - circleSize * 0.95 + 50));    //d5
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.55 - circleSize * 1.42, frameSize.x * 0.55 - circleSize * 1.42 + 50, frameSize.y * 0.35 - circleSize * 0.95, frameSize.y * 0.35 - circleSize * 0.95 + 50));  //e5

    squareCoordinates.push_back(make_tuple(frameSize.x * 0.38 - circleSize * 1.42, frameSize.x * 0.38 - circleSize * 1.42 + 50, frameSize.y * 0.23 - circleSize * 0.95, frameSize.y * 0.23 - circleSize * 0.95 + 50));  //b6
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.5 - circleSize * 1.42, frameSize.x * 0.5 - circleSize * 1.42 + 50, frameSize.y * 0.23 - circleSize * 0.95, frameSize.y * 0.23 - circleSize * 0.95 + 50));    //d6
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.62 - circleSize * 1.42, frameSize.x * 0.62 - circleSize * 1.42 + 50, frameSize.y * 0.23 - circleSize * 0.95, frameSize.y * 0.23 - circleSize * 0.95 + 50));  //f6

    squareCoordinates.push_back(make_tuple(frameSize.x * 0.31 - circleSize * 1.42, frameSize.x * 0.31 - circleSize * 1.42 + 50, frameSize.y * 0.11 - circleSize * 0.95, frameSize.y * 0.11 - circleSize * 0.95 + 50));  //a7
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.5 - circleSize * 1.42, frameSize.x * 0.5 - circleSize * 1.42 + 50, frameSize.y * 0.11 - circleSize * 0.95, frameSize.y * 0.11 - circleSize * 0.95 + 50));    //d7
    squareCoordinates.push_back(make_tuple(frameSize.x * 0.69 - circleSize * 1.42, frameSize.x * 0.69 - circleSize * 1.42 + 50, frameSize.y * 0.11 - circleSize * 0.95, frameSize.y * 0.11 - circleSize * 0.95 + 50));  //g7
}

void NineMensMorris::makeCircleVector()
{
    wxSize frameSize = GetClientSize();

    circleCoordinates.push_back(make_tuple(frameSize.x * 0.31 - (circleSize / 2), frameSize.y * 0.79));  //a1
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.79));   //d1
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.69 - (circleSize / 2), frameSize.y * 0.79));  //g1

    circleCoordinates.push_back(make_tuple(frameSize.x * 0.38 - (circleSize / 2), frameSize.y * 0.67));  //b2
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.67));   //d2
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.62 - (circleSize / 2), frameSize.y * 0.67));  //f2

    circleCoordinates.push_back(make_tuple(frameSize.x * 0.45 - (circleSize / 2), frameSize.y * 0.55));  //c3
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.55));   //d3
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.55 - (circleSize / 2), frameSize.y * 0.55));  //e3

    circleCoordinates.push_back(make_tuple(frameSize.x * 0.31 - (circleSize / 2), frameSize.y * 0.45));  //a4
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.38 - (circleSize / 2), frameSize.y * 0.45));  //b4
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.45 - (circleSize / 2), frameSize.y * 0.45));  //c4
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.55 - (circleSize / 2), frameSize.y * 0.45));  //e4
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.62 - (circleSize / 2), frameSize.y * 0.45));  //f4
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.69 - (circleSize / 2), frameSize.y * 0.45));  //g4

    circleCoordinates.push_back(make_tuple(frameSize.x * 0.45 - (circleSize / 2), frameSize.y * 0.35));  //c5
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.35));   //d5
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.55 - (circleSize / 2), frameSize.y * 0.35));  //e5

    circleCoordinates.push_back(make_tuple(frameSize.x * 0.38 - (circleSize / 2), frameSize.y * 0.23));  //b6
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.23));   //d6
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.62 - (circleSize / 2), frameSize.y * 0.23));  //f6

    circleCoordinates.push_back(make_tuple(frameSize.x * 0.31 - (circleSize / 2), frameSize.y * 0.11));  //a7
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.5 - (circleSize / 2), frameSize.y * 0.11));   //d7
    circleCoordinates.push_back(make_tuple(frameSize.x * 0.69 - (circleSize / 2), frameSize.y * 0.11));  //g7
}


int NineMensMorris::returnRedSpot() {

    vector<int> redSpots;


    for (int i = 0; i < gameBoard.size(); i++)
    {
        if (gameBoard.at(i) == -1)
        {
            redSpots.push_back(i);
        }
    }
    srand((unsigned)time(0));
    int randomNumber;


    randomNumber = (rand() % redSpots.size());
    getLocation = randomNumber;
    getCurrentLocation = getLocation;
    while (checkPlayerAdjacent(gameBoard) == false) {
        wxLogMessage("Hello");
        randomNumber = (rand() % redSpots.size());
        getLocation = randomNumber;
        //getCurrentLocation = getLocation;
    }


    int num = redSpots.at(randomNumber);

    return num;

}






int NineMensMorris::returnRandomSpot()
{
    vector<int> emptySpots;

    for (int i = 0; i < gameBoard.size(); i++)
    {
        if (gameBoard.at(i) == 0)
        {
            emptySpots.push_back(i);
        }
    }


    srand((unsigned)time(0));
    int randomNumber;

    randomNumber = (rand() % emptySpots.size());

    int num = emptySpots.at(randomNumber);

    return num;
}






//(vector<int> x) this is the millLocation
//This is "technically" not checking a full mill. it is telling where red should go so that player one does not create a mill
int NineMensMorris::TESTING_COMPUTER_OPPONENT_PLACE(vector<int> x)
{
    //wxString stringnumber = wxString::Format(wxT("%d"), (int)playerOneMillLocation);
   // wxLogMessage(stringnumber);

    if (playerOneMillLocation == 0) {

        if ((x.at(0) == 1 && x.at(1) == 1 && x.at(2) == 0))
        {
            playerOneMillLocation = 2;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {

                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;

        }

        //this is checking to see if which playerOneMillLocation equals zero.  Whichever is zero then change the playerOneMillLocation to said number and return new playerOneMillLocation
        if ((x.at(0) == 1 && x.at(1) == 0 && x.at(2) == 1)) {
            playerOneMillLocation = 1;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                ;
                return x;
            }
            else
                return playerOneMillLocation;

        }

        if ((x.at(0) == 1 && x.at(9) == 0 && x.at(21) == 1)) {
            playerOneMillLocation = 9;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;

        }

        if ((x.at(0) == 1 && x.at(9) == 1 && x.at(21) == 0)) {
            playerOneMillLocation = 21;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;

        }
    }

    if (playerOneMillLocation == 1) {

        if ((x.at(0) == 1 && x.at(1) == 1 && x.at(2) == 0)) {
            playerOneMillLocation = 2;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;

        }

        if ((x.at(0) == 0 && x.at(1) == 1 && x.at(2) == 1)) {
            playerOneMillLocation = 0;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(1) == 1 && x.at(4) == 1 && x.at(7) == 0)) {
            playerOneMillLocation = 7;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(1) == 1 && x.at(4) == 0 && x.at(7) == 1)) {
            playerOneMillLocation = 4;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation; return playerOneMillLocation;
        }
    }

    if (playerOneMillLocation == 2) {

        if ((x.at(0) == 0 && x.at(1) == 1 && x.at(2) == 1)) {
            playerOneMillLocation = 0;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(0) == 1 && x.at(1) == 0 && x.at(2) == 1)) {
            playerOneMillLocation = 1;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(2) == 1 && x.at(14) == 0 && x.at(23) == 1)) {
            playerOneMillLocation = 14;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(2) == 1 && x.at(14) == 1 && x.at(23) == 0)) {
            playerOneMillLocation = 23;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 3) {

        if ((x.at(3) == 1 && x.at(10) == 0 && x.at(18) == 1)) {
            playerOneMillLocation = 10;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(3) == 1 && x.at(10) == 1 && x.at(18) == 0)) {
            playerOneMillLocation = 18;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();


                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(3) == 1 && x.at(4) == 0 && x.at(5) == 1)) {
            playerOneMillLocation = 4;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(3) == 1 && x.at(4) == 1 && x.at(5) == 0)) {
            playerOneMillLocation = 5;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 4) {

        if ((x.at(3) == 0 && x.at(4) == 1 && x.at(5) == 1)) {
            playerOneMillLocation = 3;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(3) == 1 && x.at(4) == 1 && x.at(5) == 0)) {
            playerOneMillLocation = 5;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(7) == 0 && x.at(4) == 1 && x.at(1) == 1)) {
            playerOneMillLocation = 7;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(7) == 1 && x.at(4) == 1 && x.at(1) == 0)) {
            playerOneMillLocation = 1;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 5) {

        if ((x.at(3) == 0 && x.at(4) == 1 && x.at(5) == 1)) {
            playerOneMillLocation = 3;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(3) == 1 && x.at(4) == 0 && x.at(5) == 1)) {
            playerOneMillLocation = 4;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(5) == 1 && x.at(13) == 0 && x.at(20) == 1)) {
            playerOneMillLocation = 13;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(5) == 1 && x.at(13) == 1 && x.at(20) == 0)) {
            playerOneMillLocation = 20;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 6) {

        if ((x.at(6) == 1 && x.at(11) == 0 && x.at(15) == 1)) {
            playerOneMillLocation = 11;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(6) == 1 && x.at(11) == 1 && x.at(15) == 0)) {
            playerOneMillLocation = 15;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(6) == 1 && x.at(7) == 0 && x.at(8) == 1)) {
            playerOneMillLocation = 7;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(6) == 1 && x.at(7) == 1 && x.at(8) == 0)) {
            playerOneMillLocation = 8;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 7) {

        if ((x.at(7) == 1 && x.at(4) == 0 && x.at(1) == 1)) {
            playerOneMillLocation = 4;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(7) == 1 && x.at(4) == 1 && x.at(1) == 0)) {
            playerOneMillLocation = 1;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(6) == 0 && x.at(8) == 1 && x.at(7) == 1)) {
            playerOneMillLocation = 6;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(6) == 1 && x.at(8) == 0 && x.at(7) == 1)) {
            playerOneMillLocation = 8;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 8) {

        if ((x.at(6) == 0 && x.at(7) == 1 && x.at(8) == 1)) {
            playerOneMillLocation = 6;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(6) == 1 && x.at(7) == 0 && x.at(8) == 1)) {
            playerOneMillLocation = 7;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(8) == 1 && x.at(12) == 0 && x.at(17) == 1)) {
            playerOneMillLocation = 12;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(8) == 1 && x.at(12) == 1 && x.at(17) == 0)) {
            playerOneMillLocation = 17;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 9) {

        if ((x.at(0) == 0 && x.at(9) == 1 && x.at(21) == 1)) {
            playerOneMillLocation = 0;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(0) == 1 && x.at(9) == 1 && x.at(21) == 0)) {
            playerOneMillLocation = 21;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(9) == 1 && x.at(10) == 0 && x.at(11) == 1)) {
            playerOneMillLocation = 10;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(9) == 1 && x.at(10) == 1 && x.at(11) == 0)) {
            playerOneMillLocation = 11;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 10) {

        if ((x.at(9) == 0 && x.at(10) == 1 && x.at(11) == 1)) {
            playerOneMillLocation = 9;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(9) == 1 && x.at(10) == 1 && x.at(11) == 0)) {
            playerOneMillLocation = 11;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(3) == 0 && x.at(10) == 1 && x.at(18) == 1)) {
            playerOneMillLocation = 3;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();

                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(3) == 1 && x.at(10) == 1 && x.at(18) == 0)) {
            playerOneMillLocation = 18;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 11) {

        if ((x.at(6) == 0 && x.at(11) == 1 && x.at(15) == 1)) {
            playerOneMillLocation = 6;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(6) == 1 && x.at(11) == 1 && x.at(15) == 0)) {
            playerOneMillLocation = 15;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(9) == 0 && x.at(10) == 1 && x.at(11) == 1)) {
            playerOneMillLocation = 9;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(9) == 1 && x.at(10) == 0 && x.at(11) == 1)) {
            playerOneMillLocation = 10;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 12) {

        if ((x.at(8) == 0 && x.at(12) == 1 && x.at(17) == 1)) {
            playerOneMillLocation = 8;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(8) == 1 && x.at(12) == 1 && x.at(17) == 0)) {
            playerOneMillLocation = 17;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(12) == 1 && x.at(13) == 0 && x.at(14) == 1)) {
            playerOneMillLocation = 13;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(12) == 1 && x.at(13) == 1 && x.at(14) == 0)) {
            playerOneMillLocation = 14;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 13) {

        if ((x.at(5) == 0 && x.at(13) == 1 && x.at(20) == 1)) {
            playerOneMillLocation = 5;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(5) == 1 && x.at(13) == 1 && x.at(20) == 0)) {
            playerOneMillLocation = 20;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(12) == 0 && x.at(13) == 1 && x.at(14) == 1)) {
            playerOneMillLocation = 12;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(12) == 1 && x.at(13) == 1 && x.at(14) == 0)) {
            playerOneMillLocation = 14;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 14) {

        if ((x.at(2) == 0 && x.at(14) == 1 && x.at(23) == 1)) {
            playerOneMillLocation = 2;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(2) == 1 && x.at(14) == 1 && x.at(23) == 0)) {
            playerOneMillLocation = 23;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(12) == 0 && x.at(13) == 1 && x.at(14) == 1)) {
            playerOneMillLocation = 12;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(12) == 1 && x.at(13) == 0 && x.at(14) == 1)) {
            playerOneMillLocation = 13;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 15) {

        if ((x.at(6) == 0 && x.at(11) == 1 && x.at(15) == 1)) {
            playerOneMillLocation = 6;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(6) == 1 && x.at(11) == 0 && x.at(15) == 1)) {
            playerOneMillLocation = 11;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(15) == 1 && x.at(16) == 0 && x.at(17) == 1)) {
            playerOneMillLocation = 16;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(15) == 1 && x.at(16) == 1 && x.at(17) == 0)) {
            playerOneMillLocation = 17;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 16) {

        if ((x.at(16) == 1 && x.at(19) == 0 && x.at(22) == 1)) {
            playerOneMillLocation = 19;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(16) == 1 && x.at(19) == 1 && x.at(22) == 0)) {
            playerOneMillLocation = 22;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(15) == 0 && x.at(16) == 1 && x.at(17) == 1)) {
            playerOneMillLocation = 15;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(15) == 1 && x.at(16) == 1 && x.at(17) == 0)) {
            playerOneMillLocation = 17;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }


    }

    if (playerOneMillLocation == 17) {

        if ((x.at(8) == 0 && x.at(12) == 1 && x.at(17) == 1)) {
            playerOneMillLocation = 8;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(8) == 1 && x.at(12) == 0 && x.at(17) == 1)) {
            playerOneMillLocation = 12;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(15) == 0 && x.at(16) == 1 && x.at(17) == 1)) {
            playerOneMillLocation = 15;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(15) == 1 && x.at(16) == 0 && x.at(17) == 1)) {
            playerOneMillLocation = 16;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 18) {

        if ((x.at(18) == 1 && x.at(19) == 0 && x.at(20) == 1)) {
            playerOneMillLocation = 19;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(18) == 1 && x.at(19) == 1 && x.at(20) == 0)) {
            playerOneMillLocation = 20;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(18) == 1 && x.at(10) == 0 && x.at(3) == 1)) {
            playerOneMillLocation = 10;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(18) == 1 && x.at(10) == 1 && x.at(3) == 0)) {
            playerOneMillLocation = 3;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

    }
    if (playerOneMillLocation == 19) {

        if ((x.at(18) == 0 && x.at(19) == 1 && x.at(20) == 1)) {
            playerOneMillLocation = 18;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(18) == 1 && x.at(19) == 1 && x.at(20) == 0)) {
            playerOneMillLocation = 20;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(16) == 0 && x.at(19) == 1 && x.at(22) == 1)) {
            playerOneMillLocation = 16;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(16) == 1 && x.at(19) == 1 && x.at(22) == 0)) {
            playerOneMillLocation = 22;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 20) {

        if ((x.at(18) == 0 && x.at(19) == 1 && x.at(20) == 1)) {
            playerOneMillLocation = 18;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(18) == 1 && x.at(19) == 0 && x.at(20) == 1)) {
            playerOneMillLocation = 19;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(5) == 0 && x.at(13) == 1 && x.at(20) == 1)) {
            playerOneMillLocation = 5;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(5) == 1 && x.at(13) == 0 && x.at(20) == 1)) {
            playerOneMillLocation = 13;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 21) {

        if ((x.at(21) == 1 && x.at(22) == 0 && x.at(23) == 1)) {
            playerOneMillLocation = 22;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(21) == 1 && x.at(22) == 1 && x.at(23) == 0)) {
            playerOneMillLocation = 23;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(0) == 0 && x.at(9) == 1 && x.at(21) == 1)) {
            playerOneMillLocation = 0;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(0) == 1 && x.at(9) == 0 && x.at(21) == 1)) {
            playerOneMillLocation = 9;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

    }

    if (playerOneMillLocation == 22) {
        if ((x.at(21) == 0 && x.at(22) == 1 && x.at(23) == 1)) {
            playerOneMillLocation = 21;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(21) == 1 && x.at(22) == 1 && x.at(23) == 0)) {
            playerOneMillLocation = 23;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;
        }

        if ((x.at(16) == 0 && x.at(19) == 1 && x.at(22) == 1)) {
            playerOneMillLocation = 16;
            if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
            {
                int x = returnRandomSpot();
                return x;
            }
            else
                return playerOneMillLocation;


            if ((x.at(16) == 1 && x.at(19) == 0 && x.at(22) == 1)) {
                playerOneMillLocation = 19;
                if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
                {
                    int x = returnRandomSpot();
                    return x;
                }
                else
                    return playerOneMillLocation;
            }
        }

        if (playerOneMillLocation == 23) {

            if ((x.at(21) == 0 && x.at(22) == 1 && x.at(23) == 1)) {
                playerOneMillLocation = 21;
                if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
                {
                    int x = returnRandomSpot();
                    return x;
                }
                else
                    return playerOneMillLocation;
            }

            if ((x.at(21) == 1 && x.at(22) == 0 && x.at(23) == 1)) {
                playerOneMillLocation = 22;
                if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
                {
                    int x = returnRandomSpot();
                    return x;
                }
                else
                    return playerOneMillLocation;
            }

            if ((x.at(2) == 0 && x.at(14) == 1 && x.at(23) == 1)) {
                playerOneMillLocation = 2;
                if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
                {
                    int x = returnRandomSpot();
                    return x;
                }
                else
                    return playerOneMillLocation;
            }

            if ((x.at(2) == 1 && x.at(14) == 0 && x.at(23) == 1)) {
                playerOneMillLocation = 14;
                if (gameBoard.at(playerOneMillLocation) == 1 || gameBoard.at(playerOneMillLocation) == -1)
                {
                    int x = returnRandomSpot();
                    return x;
                }
                else
                    return playerOneMillLocation;
            }
        }
    }
}