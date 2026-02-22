//Eoin Hamill D00258444
//Joseph Bryne D00255161

#pragma once
#include <queue>
#include "command.hpp"

class CommandQueue
{
public:
	void Push(const Command& command);
	Command Pop();
	bool IsEmpty() const;

private:
	std::queue<Command> m_queue;
};

