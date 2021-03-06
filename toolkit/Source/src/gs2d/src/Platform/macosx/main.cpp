#include <SDL/SDL.h>

#include <Video.h>
#include <Input.h>
#include <Audio.h>
#include <Platform/macosx/MacOSXFileIOHub.h>
#include <Platform/StdAnsiFileManager.h>
#include <Math/Randomizer.h>

#include "main.h"

int SDL_main (int argc, char **argv)
{
	Platform::FileManagerPtr fileManager(new Platform::StdAnsiFileManager(""));
	Platform::MacOSXFileIOHubPtr fileIOHub(new Platform::MacOSXFileIOHub(fileManager, "resources/fonts/"));

	using namespace gs2d;
	using namespace gs2d::math;

	gs2d::VideoPtr video = gs2d::CreateVideo(1280, 720, "Hello GS2D!", true, true, fileIOHub, gs2d::Texture::PF_UNKNOWN, true);
	gs2d::InputPtr input = gs2d::CreateInput(0, true);
	gs2d::AudioPtr audio = gs2d::CreateAudio(0);
	{
		AudioSamplePtr music = audio->LoadSampleFromFile(fileIOHub->GetResourceDirectory() + "resources/banjo.mp3", fileManager, GSST_MUSIC);
		if (music)
			music->Play();

		gs2d::SpritePtr tileset(video->CreateSprite(fileIOHub->GetResourceDirectory() + "resources/tileset.png"));
		tileset->SetupSpriteRects(2, 2);
		tileset->SetRect(2);

		gs2d::str_type::stringstream ss;
		ss << "video.x " << video->GetClientScreenSize().x << std::endl;
		ss << "video.y " << video->GetClientScreenSize().y << std::endl;
		ShowMessage(ss, GSMT_INFO);

		gs2d::SpritePtr skull(video->CreateSprite(fileIOHub->GetResourceDirectory() + "resources/cool_skull.png"));
		gs2d::SpritePtr planets(video->CreateSprite(fileIOHub->GetResourceDirectory() + "resources/planets.png"));
		gs2d::SpritePtr black(video->CreateSprite(fileIOHub->GetResourceDirectory() + "resources/planets_black.png"));
		gs2d::SpritePtr road(video->CreateSprite(fileIOHub->GetResourceDirectory() + "resources/road.jpg"));
	
		road->SetSpriteDensityValue(2.0f);

		road->SetOrigin(Vector2(1.0f, 1.0f));

		gs2d::ShaderPtr ps = video->LoadShaderFromFile(fileIOHub->GetResourceDirectory() + "resources/shaders/pixelShaderTest.cg", Shader::SF_PIXEL);

		SpritePtr target = video->CreateRenderTarget(300, 256);
		target->SetOrigin(Vector2(0.0f, 1.0f));
		video->SetRenderTarget(target);
		video->BeginTargetScene(0xFF000000);
			skull->SetOrigin(Sprite::EO_CENTER);
			for (int t = 0; t < 100; t++)
			{
				const Color color(255, static_cast<GS_BYTE>(Randomizer::Int(255)), static_cast<GS_BYTE>(Randomizer::Int(255)), static_cast<GS_BYTE>(Randomizer::Int(255)));
				skull->Draw(Vector2(Randomizer::Float(256.0f), Randomizer::Float(256.0f)), color);
			}
			skull->SetOrigin(Sprite::EO_DEFAULT);
		video->EndTargetScene();
		target->GenerateBackup();
		target->SetSpriteDensityValue(2.0f);

		video->SetBGColor(0xFF003366);

		gs2d::Video::APP_STATUS status;
		str_type::stringstream inputStr;
		while ((status = video->HandleEvents()) != gs2d::Video::APP_QUIT)
		{
			if (status == gs2d::Video::APP_SKIP)
				continue;

			input->Update();

			if (input->GetLastCharInput() != '\0')
				inputStr << (input->GetLastCharInput());

			const Vector2 screenSize(video->GetScreenSizeF());
			{
				GS_KEY_STATE state = input->GetKeyState(GSK_SPACE);
				switch (state)
				{
				case GSKS_DOWN:
					std::cout << "-";
					break;
				case GSKS_HIT:
					std::cout << "<";
					break;
				case GSKS_RELEASE:
					std::cout << ">\n\n";
					break;
				case GSKS_UP:
					break;
				};
				if (input->GetKeyState(GSK_LMOUSE) == GSKS_HIT)
				{
					input->SetCursorPosition((screenSize * 0.5f).ToVector2i());
				}
			}

			if (input->GetKeyState(GSK_P) == GSKS_HIT)
			{
				music->Pause();
			}
			if (input->GetKeyState(GSK_S) == GSKS_HIT)
			{
				music->Stop();
			}
			if (input->GetKeyState(GSK_G) == GSKS_HIT)
			{
				music->Play();
			}
			if (music->GetStatus() == GSSS_PAUSED)
				ShowMessage("paused\n", GSMT_INFO);
			if (music->GetStatus() == GSSS_STOPPED)
				ShowMessage("stopped\n", GSMT_INFO);
			if (music->GetStatus() == GSSS_PLAYING)
				ShowMessage("playing\n", GSMT_INFO);

			video->BeginSpriteScene();

			tileset->SetRect(3);
			skull->SetAsTexture(1);
			tileset->Draw(Vector2(664, 600));
			video->UnsetTexture(1);

			tileset->Draw(Vector2(600, 600));

			video->SetAlphaMode(gs2d::Video::AM_NONE);
			skull->SetAsTexture(1);
			video->SetBlendMode(1, gs2d::Video::BM_ADD);
			black->Draw(Vector2(256,256));
			video->SetBlendMode(1, gs2d::Video::BM_MODULATE);
			
			planets->Draw(Vector2(600,256));
			
			black->SetAsTexture(1);
			planets->Draw(Vector2(256, 600));

			video->UnsetTexture(1);

			video->SetAlphaMode(gs2d::Video::AM_MODULATE);
			planets->Draw(Vector2(600,600));

			video->SetAlphaMode(gs2d::Video::AM_PIXEL);

			video->SetPixelShader(ps);
			ps->SetTexture("secondTexture", target->GetTexture());
			road->Draw(screenSize);
			video->SetPixelShader(ShaderPtr());

			video->DrawRectangle(
				Vector2(10,10),
				Vector2(32,32),
				gs2d::constant::BLUE, gs2d::constant::GREEN,
				gs2d::constant::RED, gs2d::constant::YELLOW);

			video->DrawBitmapText(screenSize * 0.8f, "Scaled", "Verdana20_shadow.fnt", 0xFFFFFFFF, 4.0f);

			planets->SetAsTexture(1);
			skull->Draw(Vector2(200,400));

			video->UnsetTexture(1);
			skull->Draw(Vector2(400,100));

			video->DrawRectangle(
				input->GetCursorPositionF(video),
				Vector2(64,256),
				gs2d::constant::YELLOW);

			tileset->SetRect(0);
			tileset->Draw(Vector2(600, 200), 0xAAFF0000);

			static Vector2 pos(screenSize * 0.5f);
			pos += input->GetTouchMove(0);
			video->DrawRectangle(
				pos,
				Vector2(64,64),
				0xFFFF00FF, gs2d::constant::WHITE,
				gs2d::constant::GREEN, gs2d::constant::BLACK);

			video->DrawBitmapText(Vector2(0.0f, screenSize.y - 20.0f), "Powered by Ethanon Engine", "Verdana20_shadow.fnt", 0xFFFFFFFF, 1.0f);
			
			video->SetLineWidth(1.0f);
			video->DrawLine(Vector2(0,0), video->GetScreenSizeF(), 0xFFFFFFFF, 0xFFFF00FF);
			video->DrawLine(Vector2(100,0), Vector2(200,100), 0xFFFFFFFF, 0xFFFF00FF);
			video->SetLineWidth(3);
			video->DrawLine(Vector2(screenSize.x/2,0), Vector2(screenSize.x/2,screenSize.y), 0xFFFFFFFF, 0xFFFF00FF);
			video->SetLineWidth(10);
			video->DrawLine(Vector2(screenSize.x,0), Vector2(0,screenSize.y), 0xFF0000FF, 0xFF00FF00);

			static float angle = 0.0f; angle += 0.1f;
			angle += input->GetWheelState();
			video->DrawRectangle(
				Vector2(200,400),
				Vector2(96,32),
				gs2d::constant::BLUE,
				angle);
				
			tileset->SetRect(1);
			tileset->Draw(Vector2(200, 600));
			
			video->DrawBitmapText(screenSize * 0.2f, str_type::string("Oh my god WTF barbecue\n") + inputStr.str(), "Verdana20_shadow.fnt", 0xAAFFFF00, 1.0f);

			video->DrawRectangle(
				Vector2(600,300),
				Vector2(100,10),
				gs2d::constant::RED, gs2d::constant::YELLOW,
				0x000000FF, gs2d::constant::GREEN,
				45.0f);

			video->DrawRectangle(
				Vector2(300,600),
				Vector2(100,10),
				gs2d::constant::RED, gs2d::constant::YELLOW,
				gs2d::constant::BLUE, 0x0,
				200.0f);

			tileset->SetRect(2);
			tileset->Draw(screenSize * 0.5f);

			gs2d::str_type::stringstream ss; ss << video->GetFPSRate();
			Vector2 textSize(video->ComputeTextBoxSize("Verdana20_shadow.fnt", ss.str()));
			video->DrawBitmapText(Vector2(screenSize.x - textSize.x, 0.0f), ss.str(), "Verdana20_shadow.fnt", 0xFF336699);

			target->Draw(Vector2(900, 300), gs2d::constant::WHITE, 45.0f);
			video->EndSpriteScene();
		}
	}
	return 0;
}
