// thirdModelViewer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "ModelViewer/ModelViewer.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, ".UTF8");

	ModelViewer viewer;
	int result = viewer.Run();
	return result;
}