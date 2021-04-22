#pragma once
#include <vector>

enum class TaskState {
	Finished,
	NotFinished
};

struct GlobalVariable
{
	GlobalVariable(int taskCount) :currentTime(0)
	{
		taskState.resize(taskCount);
		for (int i = 0; i != taskCount; i++) {
			taskState[i] = TaskState::NotFinished;
		}
	}
	float currentTime;

	// 5 min to finish
	const float totalTime = 300;

	/*!
	 * 0. ����ˢ��
	 * 1. ����
	 * 2. �޲�ƺ
	 * 3. ��������
	 * 4. ��ʰ����
	*/
	std::vector<TaskState> taskState;
};
