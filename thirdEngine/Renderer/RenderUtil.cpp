#include "RenderUtil.h"

#include <cstdio>
#include <windows.h>
#include <glad/glad.h>

GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;

	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		const wchar_t* error = L"";
		switch (errorCode)
		{
		case GL_INVALID_ENUM:
			error = L"INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = L"INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = L"INVALID_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			error = L"OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = L"INVALID_FRAMEBUFFER_OPERATION";
			break;
		}

		size_t num = MultiByteToWideChar(CP_ACP, 0, file, -1, NULL, 0);
		wchar_t* wword; //定义一个UNICODE的指针
		wword = new wchar_t[num];
		memset(wword, 0, num * sizeof(wchar_t)); //初始化动作
		MultiByteToWideChar(CP_ACP, 0, file, -1, wword, num);

		fwprintf(stderr, L"%s|%s:%d\n", error, wword, line);

		delete[] wword;
	}
	return errorCode;
}