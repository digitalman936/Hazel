#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();

	HZ_CORE_INFO("Welcome to Hazel Game Engine!");
	HZ_INFO("This is the Client logging!");

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif