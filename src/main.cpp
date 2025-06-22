#include "Solutions.h"
#include <iostream>
#include "Stat.h"
#include <wx/wx.h>

#define MULTIPLE 0
#define ALGO DFS
#define MAZE_SIZE 51


 // Define a custom application class inheriting from wxApp
 class MyApp : public wxApp
 {
 public:
     bool OnInit() override; // Override the OnInit method
 };

 // Define a custom frame (window) class inheriting from wxFrame
 class MainFrame final : public wxFrame
 {
 public:
     MainFrame(int size, int algo);

 private:
     void OnPaint(wxPaintEvent& event); // Paint event handler
     void paintMaze(wxDC& dc);         // Helper for painting the maze
     std::shared_ptr<Maze> maze;
     int size;
 };

 // Implement OnInit method of MyApp
 bool MyApp::OnInit()
 {
     int size = 55, algo = 2;
     auto mainFrame = std::make_unique<MainFrame>(MAZE_SIZE, ALGO);
     mainFrame->Show(true); // Show the main frame
     SetTopWindow(mainFrame.release());
     return true;
 }

 // Implement the constructor for MainFrame
 MainFrame::MainFrame(int size, int algo)
     : wxFrame(nullptr, wxID_ANY, "Basic wxWidgets App", wxDefaultPosition, wxSize(800, 800),wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX)
 {
     // if (size % 2 == 0)size += 1;
     this->size = size % 2 == 0 ? size +1 : size;
     maze = Generator::generateMaze(this->size , algo);

     // Bind the paint event
     Bind(wxEVT_PAINT, &MainFrame::OnPaint, this);
 }

 // Paint event handler
 void MainFrame::OnPaint(wxPaintEvent& event)
 {
     wxPaintDC dc(this); // Create the device context for painting
     paintMaze(dc);      // Call the painting logic
 }

 // Helper for painting the maze
 void MainFrame::paintMaze(wxDC& dc)
 {
     dc.SetBrush(*wxWHITE_BRUSH);
     dc.SetPen(*wxBLACK_PEN);

     // Get the size of the client area (drawable area)
     wxSize clientSize = GetClientSize();

     // Calculate the size of each cell (ensure floating-point precision)
     float cellSizeX = static_cast<float>(clientSize.x) / size;
     float cellSizeY = static_cast<float>(clientSize.y) / size;

     Generator::multipleSolutionsGenerator(maze,MULTIPLE);

     unsigned int nde , nv , steps , pathLength ;
     std::vector<coordinate> path = bfs(maze ,steps,nde , nv , pathLength);
     std::set<coordinate> pathSet(path.begin(), path.end());

     // Draw the maze
     for (int i = 0; i < size; i++) {
         for (int j = 0; j < size; j++) {
             // Set the appropriate brush color
             if (pathSet.contains({i, j})) {
                 // Cell is part of the path
                 dc.SetBrush(*wxGREEN_BRUSH);
             }
             else if ((*maze)({i, j})) {
                 // Cell is a wall
                 dc.SetBrush(*wxBLACK_BRUSH);
             } else {
                 // Cell is empty
                 dc.SetBrush(*wxWHITE_BRUSH);
             }
             // Calculate position and size of each rectangle
             int x = static_cast<int>(j * cellSizeX);
             int y = static_cast<int>(i * cellSizeY);
             int width = static_cast<int>(cellSizeX);
             int height = static_cast<int>(cellSizeY);

             // Adjust for rounding errors on the last cell
             if (j == size - 1) {
                 width = clientSize.x - x;
             }
             if (i == size - 1) {
                 height = clientSize.y - y;
             }

             // Draw the rectangle
             dc.DrawRectangle(x, y, width, height);
         }
     }
 }


int main() {
    std::cout << "Choose mode:\n";
    std::cout << "1. Generate CSV\n";
    std::cout << "2. Launch GUI\n";
    std::cout << "Enter choice (1 or 2): ";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        generateCsvFile(10, 11, 50, 0);
        return 0;
    } else if (choice == 2) {
        wxApp::SetInstance(new MyApp());

        // Create dummy argc and argv
        int argc = 1;
        char* argv[] = { const_cast<char*>("app") };

        return wxEntry(argc, argv);  // pass dummy argc and argv
    } else {
        std::cerr << "Invalid choice.\n";
        return 1;
    }
}