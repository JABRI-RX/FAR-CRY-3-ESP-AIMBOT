#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "MemMan.h"
#include <string>

#define WINDOW_WIDTH 800.f
#define WINDOW_HEIGHT 600.f
enum Game
{
    FAR_CRY_3 = 0
};
struct Color {
    float r{}, g{}, b{}, a{};
    Color(){}
    Color(float r, float g, float b, float a) { 
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
};
struct Vec3 {
    float x{0}, y{0}, z{0};
    Vec3(){}
    Vec3(float X, float Y, float Z) :x(X), y(Y), z(Z) {}
    void setVector(Vec3& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }
 
};
struct Vec2 {
    float x, y;
    Vec2() { x = 0, y = 0; }
    Vec2(float X, float Y ) :x(X), y(Y){}
};

struct Matrix {
    float matrix[24];
    Matrix()
    {
        memset(matrix, 0, sizeof(matrix));
    }
 
};
//public vars
MemMan m;
//end
void ShowMenu(GLFWwindow* window)
{
    glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, false);
}
void HideMenu(GLFWwindow* window)
{
    glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, true);
}
void setCursorPos2(const float x, const float y)
{
    INPUT inputs[3] = { 0 };
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dx = x;
    inputs[0].mi.dy = y;
    inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    SendInput(3, inputs, sizeof(INPUT));
}
bool H_WorldToScreen(const Vec3& origin, Vec2& screen, float* matrix)
{
    Vec2 _res(WINDOW_WIDTH, WINDOW_HEIGHT);
    screen.x = origin.x * matrix[0] + origin.y * matrix[1] + origin.z * matrix[2] + matrix[3];
    screen.y = origin.x * matrix[4] + origin.y * matrix[5] + origin.z * matrix[6] + matrix[7];
    //float w = origin.x * matrix[8] + origin.y * matrix[9] + origin.z * matrix[10] + matrix[11];
    float w = origin.x * matrix[12] + origin.y * matrix[13] + origin.z * matrix[14] + matrix[15];
    //matrix[4] may not be working 
    if (w < 0.01f)
        return false;

    Vec2 ndc;
    ndc.x = screen.x / w;
    ndc.y = screen.y / w;

    screen.x = (_res.x / 2 * ndc.x) + (ndc.x + _res.x / 2);
    screen.y = -(_res.y / 2 * ndc.y) + (ndc.y + _res.y / 2);
    //ConvertToRange(screen,_res);
    return true;
}
bool V_WorldToScreen(const Vec3& origin, Vec2& screen, float* matrix)
{
    Vec2 _res(800,600);
    screen.x = origin.x * matrix[0] + origin.y * matrix[4] + origin.z * matrix[8] + matrix[12];
    screen.y = origin.x * matrix[1] + origin.y * matrix[5] + origin.z * matrix[9] + matrix[13];
    //float w = origin.x * matrix[8] + origin.y * matrix[9] + origin.z * matrix[10] + matrix[11];
    float w = origin.x * matrix[2] + origin.y * matrix[6] + origin.z * matrix[10] + matrix[14];
    //matrix[4] may not be working 
    if (w < 0.01f)
        return false;

    Vec2 ndc;
    ndc.x = screen.x / w;
    ndc.y = screen.y / w;

    screen.x = (_res.x / 2 * ndc.x) + (ndc.x + _res.x / 2);
    screen.y = -(_res.y/ 2 * ndc.y) + (ndc.y + _res.y / 2);
    //ConvertToRange(screen,_res);
    return true;
}
float Distance(const Vec3& player, const Vec3& target)
{
    float a = target.x - player.x;
    float b = target.y - player.y;
    float c = target.z - player.z;
    return sqrt(a * a + b * b + c * c);
}
float CalcAngle(const Vec3& player, const Vec3& target)
{
     
    //Calculate Yaw
    float x = target.x - player.x;
    float y = target.y - player.y;
    float z = target.z - player.z;
    float magnitude = sqrt(x * x + y * x + z * z);
    float yaw = atan(x / y);
    return  yaw ;//return Yaw
     
}
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
inline void drawESPLines(const Vec2& screen)
{
    //start ESP drawing
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
    auto pDrawList = ImGui::GetWindowDrawList();
    pDrawList->AddLine(ImVec2(WINDOW_WIDTH / 2.f, 768), ImVec2(screen.x, screen.y), ImColor(255, 0, 0), 3);
    
   

    ImGui::End();
    //End ESP drawing
}
inline void drawESPText(const Vec2& screen, const char* distance)
{
    //start ESP drawing
    ImGui::SetNextWindowPos(ImVec2(0, 0)); ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT)); ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
    auto pDrawList = ImGui::GetWindowDrawList();
    //pDrawList->AddText(ImVec2(screen.x, screen.y), ImColor(255, 0, 0), distance);
    pDrawList->AddText(ImGui::GetFont(), 15, ImVec2(screen.x,screen.y), ImColor(255, 0, 0), distance);
    
    
    ImGui::End();
    //End ESP drawing
}
inline void printMatrix(float* matrix)
{
    ImGui::Text("[%f][%f][%f][%f]", matrix[0], matrix[1], matrix[2], matrix[3]);
    ImGui::Text("[%f][%f][%f][%f]", matrix[4], matrix[5], matrix[6], matrix[7]);
    ImGui::Text("[%f][%f][%f][%f]", matrix[0], matrix[1], matrix[2], matrix[11]);
    ImGui::Text("[%f][%f][%f][%f]",matrix[12], matrix[13], matrix[14], matrix[15]);
}
typedef void (*_FCE_DrawCube)(Vec3 , float , float , Color , float , Color );
DWORD _FCE_DrawCube_Addresse = 0x5E1DD1AE;
_FCE_DrawCube FCE_DrawCube = (_FCE_DrawCube)_FCE_DrawCube_Addresse;
//getFov
typedef void (*_FCE_DrawCube)(Vec3, float, float, Color, float, Color);
DWORD _FCE_DrawCube_Addresse = 0x5E1DD1AE;
_FCE_DrawCube FCE_DrawCube = (_FCE_DrawCube)_FCE_DrawCube_Addresse;
void drawSquare()
{
    Vec3 pos(-1489.289795f, -1348.137817f, 124.7655945f);
    float radius = 10;
    float penWidth = 100;
    Color red(255, 0, 0, 255);
    float zOrder = 10;
    Color green(0, 255, 0, 255);
    //make the area executable
    DWORD oldprotect;
    //VirtualProtect((byte*)_FCE_DrawCube_Addresse, 408, PAGE_EXECUTE_READWRITE, &oldprotect);
    VirtualProtectEx(m.handle, (byte*)_FCE_DrawCube_Addresse, 408, PAGE_EXECUTE_READWRITE, &oldprotect);
    FCE_DrawCube(pos, radius, penWidth, red, zOrder, green);
    //VirtualProtect((byte*)_FCE_DrawCube_Addresse, 408, oldprotect, &oldprotect);

    
}
// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    
    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, true);
    glfwWindowHint(GLFW_DECORATED, false);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
 
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ESP", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
 

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    ImGui::StyleColorsDark();
 
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Users\\R00T\\Documents\\brogramming\\c++\\Far Cry3\\depen\\font\\con_bold.TTF",13.f,NULL,io.Fonts->GetGlyphRangesDefault());
    //ImFont* font = io.Fonts->AddFontFromFileTTF("");
    // Our state
    std::wstring windows[1]{L"Far Cry® 3"};
    std::wstring process[1]{L"farcry3.exe"};
    int game_index = Game::FAR_CRY_3;
    std::wstring w = windows[game_index];
    std::wstring p = process[game_index];


    MoveWindow(GetConsoleWindow(), 1500, 0, 450, 400, TRUE);
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    HWND FGW = FindWindow(NULL,w.c_str());
   
    m.getProcess(p.c_str());
    RECT rect;
    
   
    Matrix matrix{};
 
    //DWORD playerPosAdress = {};
    //Vec3 PlayerPos;

    Vec3 FC3Pos(-1497.062988f, -1645.148193f, 188.8372498f);
     
     
    Vec3 Pos; Pos.setVector(FC3Pos);
    Vec3 MyPos;
    Vec2 screen;
    //bools
    bool showMenu = true;
    bool aimbot_on = false;
    int esp_on =  0;
    drawSquare();
    while (!glfwWindowShouldClose(window))
 
    {
        GetWindowRect(FGW, &rect);
        glfwSetWindowPos(window, rect.left + 3, rect.top+20 );
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            showMenu = !showMenu;
            if (showMenu)
                ShowMenu(window);
            else
                HideMenu(window);
            
            //exit(0);
        }  
        if (GetAsyncKeyState(VK_MENU) & 1)
            //aimbot_on = !aimbot_on;
        if (aimbot_on)
        {
            Vec2 pos = screen;
            //glfwSetCursorPos(window, screen.x, screen.y);
            //Sleep(3);
            setCursorPos2(pos.x, pos.y);
            //SetCursorPos(0 ,0);
        }
        int  i = 0;
        if (esp_on == 0)
        {
            //ImGui::Text("Hi there");
            DWORD matrices[2] = { 0x093360B0 ,m.getAddress(0x6FC5B588, { 0x0C }) };
            const DWORD matrixAddress = matrices[0];
            DWORD starting_entity_list = m.getAddress(m.getModule(m.pid,L"FC3.dll")+ 0x1E7E3B0, {0x68 ,0x0});
            //read the 4x4 matrix 
            matrix = m.readMem<Matrix>(matrixAddress);
            for (i = 0; i <150;i++)
            {
                DWORD pos_add = m.getAddress(starting_entity_list + i * 0x4, { 0x504 });
                //DWORD pos_add = 0x32DB8C00 +i *0x1C;
                Pos = m.readMem<Vec3>(pos_add);
                MyPos = m.readMem<Vec3>(0x7E98B7E0);
                float distance = Distance(MyPos, Pos);
                if (Pos.z <= 0.f || distance > 2000.f )
                    continue;
                 /*if (distance > 1000.f)
                    continue;*/
 
             
               
                /*if (distance > 200.f)
                    continue;*/
                ImGui::Text("the distance between you and [%i] is [%f m]", i, distance);
                if (V_WorldToScreen(Pos, screen, matrix.matrix))
                {
                    char dist[32];
                    snprintf(dist, sizeof(dist), "%.2f m", distance);
                    //drawESPLines(screen);
                    drawESPText(screen, dist);
                }
            }
           
            
        }
        //here we draw falloutnew vegas

        // Rendering
        ImGui::Render();glClear(GL_COLOR_BUFFER_BIT);ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());glfwSwapBuffers(window);
    }
 
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplGlfw_Shutdown();ImGui::DestroyContext();glfwDestroyWindow(window);glfwTerminate();

    return 0;
}
