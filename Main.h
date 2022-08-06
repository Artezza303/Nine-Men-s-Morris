#pragma once

#include "wx/wx.h"
#include <vector>
#include <utility>
#include <tuple>
#include <list>
#include <string>
#include <wx/log.h> 
#include "wx/image.h"

using namespace std;

class NineMensMorris : public wxFrame
{
public:
    //cMain(wxFrame* parent, wxString file, wxBitmapType format);
    NineMensMorris();
    ~NineMensMorris();

    void paintEvent(wxPaintEvent& evt); //Tigran
    void drawTheBoard(wxDC& dc);  //Tigran
    void OnClose(wxCloseEvent& event);  //Tigran


    void LeftClickPlayer1(wxMouseEvent& event);  //Tigran
    void LeftClickPlayer2(wxMouseEvent& event);  //Tigran

    void LeftClickRemovePlayer1Phase1(wxMouseEvent& event);  //Tigran
    void LeftClickRemovePlayer2Phase1(wxMouseEvent& event); //Tigran

    void LeftClickRemovePlayer1Phase2(wxMouseEvent& event);  //Tigran
    void LeftClickRemovePlayer2Phase2(wxMouseEvent& event);  //Tigran



    void makeSquareVector();  //Tigran
    void makeCircleVector();  //Tigran


    void drawCircle(wxDC& dc, float x, float y, wxBrush color);  //Tigran


    bool checkPlayerMillBlue(vector<int> x);  //Tigran
    bool checkPlayerMillRed(vector<int> x);  //Tigran


    void MoveComputer(wxMouseEvent& event);

    void LeftClickPlayerOneMove(wxMouseEvent& event); //TESTING
    void LeftClickPlayerTwoMove(wxMouseEvent& event); //TESTING



    void LeftClickPlayerOneMovePart2(wxMouseEvent& event); //TESTING
    void LeftClickPlayerTwoMovePart2(wxMouseEvent& event); //TESTING


    void DrawPlayerOnePieces(wxDC& dc);
    void DrawPlayerTwoPieces(wxDC& dc);

    void DrawPlayerOnePiecesTaken(wxDC& dc);
    void DrawPlayerTwoPiecesTaken(wxDC& dc);

    int ReturnComputerAdjacent(vector<int> x);

    bool checkPlayerAdjacent(vector<int>x);

    bool checkRemoveBlue(vector<int> x);
    bool checkRemoveRed(vector<int> x);



    bool checkLegalMove(vector<int> x, int player);


    int TESTING_COMPUTER_OPPONENT_PLACE(vector<int> x);
    int TESTING_COMPUTER_MILL_PLACE(vector<int> x);

    int returnRedSpot();
    int returnRandomSpot();


    DECLARE_EVENT_TABLE()









    //void RightClickPlace(wxMouseEvent& event);   //RIGHT CLICK WILL BE REMOVE AFTER AI  //Tigran
   //void SpaceBarRemove(wxKeyEvent& event); // WILL BE CHNANGED TO RIGHT CLICK AFTER AI  //Tigran


private:

    int getCurrentLocation;

    int getLocation;

    int millLocation;

    int removeLocation;



    //TEST
    int playerOneMillLocation;
    int playerTwoMillLocation;





    int circleSize = 25;

    int playerOnePiecesLeft = 9;
    int playerTwoPiecesLeft = 9;

    int playerOnePiecesOnBoard = 0;
    int playerTwoPiecesOnBoard = 0;

    int playerOneEnemies = 0;
    int playerTwoEnemies = 0;

    string Left = "Player 1 Pieces (Blue): ";
    string Right = "Player 2 Pieces (Red): ";

    string Left2 = "Enemy Pieces Taken: ";
    string Right2 = "Enemy Pieces Taken: ";

    int width = 150;
    int height = 40;



    vector<tuple<float, float, float, float>> squareCoordinates;
    vector<tuple<float, float>> circleCoordinates;

    vector<int> gameBoard = vector<int>(24, 0);
    vector<int> redPieces;






};