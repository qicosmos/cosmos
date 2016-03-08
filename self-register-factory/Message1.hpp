#pragma once

#include "MsgFactory.hpp"
#include "Message.hpp"

class Message1 : public Message
{
public:

	Message1()
	{
		std::cout << "message1" << std::endl;
	}

	Message1(int a)
	{
		std::cout << "message1" << std::endl;
	}

	~Message1()
	{
	}

	void foo() override
	{
		std::cout << "message1" << std::endl;
	}
};

//REGISTER_MESSAGE(Message1, "message1", 2);
REGISTER_MESSAGE(Message1, "message1");
