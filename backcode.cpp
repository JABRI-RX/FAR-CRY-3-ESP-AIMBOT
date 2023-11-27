//#include <iostream>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <Windows.h>
//using namespace std;
//
//struct Vec3 {
//public:
//	float x{}, y{}, z{};
//	Vec3() {}
//	Vec3(float X, float Y, float Z) {
//		x = X;
//		y = Y;
//		z = Z;
//	}
//	void print()
//	{
//		printf("[%f]\t[%f]\t[%f]\n", x, y, z);
//	}
//
//};
//float GetDistance(Vec3 player, Vec3 target)
//{
//	Vec3 delta = Vec3(target.x - player.x, target.y - player.y, target.z - player.z);
//	return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
//}
//float ToDegree(float rad)
//{
//	return rad * (180.0 / 3.141592653589793238463);
//}
//void AimAtTarget(HANDLE handl)
//{
//	DWORD ViewAngleAdd = 0x29BAF574;
//	DWORD PositionAdd = 0x3210AE60;
//	Vec3 MyPos;
//	Vec3 target(79.54898071f, -0.2006367892f, 184.162674f);
//	while (true)
//	{
//		ReadProcessMemory(handl, (PBYTE*)PositionAdd, &MyPos, sizeof(MyPos), 0);
//		printf("The Distance : %f\n", GetDistance(MyPos, target));
//		Sleep(100);
//	}
//
//
//}
//void Test(HANDLE handl)
//{
//	Sleep(3000);
//	DWORD x_pos_camera = 0x00B5AA44;
//	float i = 0;
//	int sign = -1;
//	while (true)
//	{
//		if (i >= 1)
//			sign = -1;
//		else if (i <= -1.f)
//			sign = 1;
//		i += sign * 0.1f;
//		cout << i << "\n";
//		WriteProcessMemory(handl, (PBYTE*)x_pos_camera, &i, sizeof(float), 0);
//		Sleep(10);
//	}
//}
//int main1()
//{
//
//	Test(gameHandle);
//	
//
//	GLFWwindow* window;
//
//	/* Initialize the library */
//	if (!glfwInit())
//		return -1;
//
//	glewInit();
//	glfwWindowHint(GLFW_FLOATING, true);
//	glfwWindowHint(GLFW_RESIZABLE, false);
//	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
//	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, true);
//	/* Create a windowed mode window and its OpenGL context */
//	window = glfwCreateWindow(800, 600, "ESP", NULL, NULL);
//	if (!window)
//	{
//		glfwTerminate();
//		return -1;
//	}
//	glfwSetWindowAttrib(window, GLFW_DECORATED, false);
//	/* Make the window's context current */
//	glfwMakeContextCurrent(window);
//
//	MoveWindow(GetConsoleWindow(), 1500, 0, 400, 400, TRUE);
//	HWND FGW = FindWindow(NULL,  L"FGUY");
//	DWORD pid;
//	GetWindowThreadProcessId(FGW, &pid);
//
//	HANDLE  gameHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
//	RECT rect;
//	/* Loop until the user closes the window */
//	while (!glfwWindowShouldClose(window))
//	{
//		GetWindowRect(FGW, &rect);
//		glfwSetWindowPos(window, rect.left + 3, rect.top + 26);
//		/* Render here */
//		glClear(GL_COLOR_BUFFER_BIT);
//		glBegin(GL_LINES);
//		glVertex2f(0.f, 1.f);
//		glVertex2f(0.f, -1.f);
//		glEnd();
//		/* Swap front and back buffers */
//		glfwSwapBuffers(window);
//
//		/* Poll for and process events */
//		glfwPollEvents();
//		Sleep(100);
//	}
//
//	glfwTerminate();
//	return 0;
//}
//
