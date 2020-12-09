#pragma once

#include "ChatSocket.h"
#include <thread>


namespace ChatForm {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ sendBtn;
	private: System::Windows::Forms::TextBox^ inputBox;
	private: System::Windows::Forms::TextBox^ msgBox;
	private: System::Windows::Forms::Timer^ recvMsgTimer;

	private: System::ComponentModel::IContainer^ components;

	protected:

	protected:


	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->sendBtn = (gcnew System::Windows::Forms::Button());
			this->inputBox = (gcnew System::Windows::Forms::TextBox());
			this->msgBox = (gcnew System::Windows::Forms::TextBox());
			this->recvMsgTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// sendBtn
			// 
			this->sendBtn->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->sendBtn->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->sendBtn->Location = System::Drawing::Point(535, 513);
			this->sendBtn->Margin = System::Windows::Forms::Padding(0);
			this->sendBtn->Name = L"sendBtn";
			this->sendBtn->Size = System::Drawing::Size(95, 32);
			this->sendBtn->TabIndex = 0;
			this->sendBtn->Text = L"Send";
			this->sendBtn->UseVisualStyleBackColor = true;
			this->sendBtn->Click += gcnew System::EventHandler(this, &Form1::sendBtn_Click);
			// 
			// inputBox
			// 
			this->inputBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->inputBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->inputBox->Location = System::Drawing::Point(1, 515);
			this->inputBox->MaxLength = 140;
			this->inputBox->Name = L"inputBox";
			this->inputBox->Size = System::Drawing::Size(533, 28);
			this->inputBox->TabIndex = 1;
			this->inputBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::inputBox_KeyPress);
			// 
			// msgBox
			// 
			this->msgBox->AcceptsReturn = true;
			this->msgBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->msgBox->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->msgBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->msgBox->Location = System::Drawing::Point(1, 1);
			this->msgBox->MaxLength = 327670;
			this->msgBox->Multiline = true;
			this->msgBox->Name = L"msgBox";
			this->msgBox->ReadOnly = true;
			this->msgBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->msgBox->Size = System::Drawing::Size(629, 509);
			this->msgBox->TabIndex = 2;
			// 
			// recvMsgTimer
			// 
			this->recvMsgTimer->Interval = 2000;
			this->recvMsgTimer->Tick += gcnew System::EventHandler(this, &Form1::recvMsgTimer_Tick);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(630, 546);
			this->Controls->Add(this->msgBox);
			this->Controls->Add(this->inputBox);
			this->Controls->Add(this->sendBtn);
			this->Name = L"Form1";
			this->Text = L"Chat";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e) {
		msgBox->AppendText(clientStartup());
		recvMsgTimer->Enabled = true;
	}
	
	private: System::Void recvMsgTimer_Tick(System::Object^ sender, System::EventArgs^ e) {
		String^ msg = recvMsg();
		if (msg->Length > 0) {
			msgBox->Text += ("\r\n" + msg);
		}
	}

	private: System::Void sendBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		msgBox->AppendText("\r\n" + sendMsg(inputBox->Text));
		inputBox->Text = "";
	}

	private: System::Void inputBox_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
		if (e->KeyChar == '\r') {
			msgBox->AppendText("\r\n" + sendMsg(inputBox->Text));
			inputBox->Text = "";
		}	
	}
};
}
