#include "pch.h"

using namespace System;

#include "Chat.h"

using namespace System::Windows::Forms;

[STAThread]
int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew ChatForm::Form1()); 
	return 0;
}