#pragma once
#include <vector>
#include <thirdEngine/Renderer/Texture.h>

enum class TaskState {
	Finished,
	NotFinished
};

struct GlobalVariable
{
	GlobalVariable(int taskCount) :currentTime(0), clothes()
	{
		taskState.resize(taskCount);
		for (int i = 0; i != taskCount; i++) {
			taskState[i] = TaskState::NotFinished;
		}

		clothIndex = 3;
	}
	float currentTime;

	// 5 min to finish
	const float totalTime = 300;

	/*!
	 * 0. ����ˢ��
	 * 1. ����
	 * 2. ��ʰ����
	 * 3. ���·�
	 * 4. �����
	 * 5. �ռ�ʳ��
	*/
	std::vector<TaskState> taskState;

	std::vector<Texture> clothes;
	unsigned int clothIndex;
};
