/*--------------------------------------------------------------------------------------
 Ethanon Engine (C) Copyright 2008-2012 Andre Santee
 http://www.asantee.net/ethanon/

	Permission is hereby granted, free of charge, to any person obtaining a copy of this
	software and associated documentation files (the "Software"), to deal in the
	Software without restriction, including without limitation the rights to use, copy,
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so, subject to the
	following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
	OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--------------------------------------------------------------------------------------*/

#ifndef GS2D_GLES2_H_
#define GS2D_GLES2_H_

#include "../../gs2d.h"
#include "../../Platform/Platform.h"
#include "../../Platform/FileLogger.h"
#include "gs2dGLES2Texture.h"

#include "../../Platform/FileIOHub.h"
#include <map>
#include <sys/time.h>


#define _GS2D_GLES2_MAX_MULTI_TEXTURES 2

namespace gs2d {

class GLES2Shader;
typedef boost::shared_ptr<GLES2Shader> GLES2ShaderPtr;
class GLES2ShaderContext;
typedef boost::shared_ptr<GLES2ShaderContext> GLES2ShaderContextPtr;

class GLES2Video : public Video, public Platform::NativeCommandForwarder
{
	GLES2Video(const unsigned int width, const unsigned int height,
			const str_type::string& winTitle, const Platform::FileIOHubPtr& fileIOHub);

	boost::weak_ptr<GLES2Video> weak_this;

public:

	static const float ZNEAR;
	static const float ZFAR;
	static const str_type::string VIDEO_LOG_FILE;
	static const unsigned long ALPHAREF;

	static boost::shared_ptr<GLES2Video> Create(const unsigned int width, const unsigned int height,
			const str_type::string& winTitle, const Platform::FileIOHubPtr& fileIOHub);

	static void CheckGLError(const str_type::string& op, const Platform::FileLogger& logger);

	TexturePtr CreateTextureFromFileInMemory(const void *pBuffer,
			const unsigned int bufferLength, GS_COLOR mask,
			const unsigned int width = 0, const unsigned int height = 0,
			const unsigned int nMipMaps = 0);

	TexturePtr LoadTextureFromFile(const str_type::string& fileName,
			GS_COLOR mask, const unsigned int width = 0,
			const unsigned int height = 0, const unsigned int nMipMaps = 0);

	TexturePtr CreateRenderTargetTexture(const unsigned int width,
			const unsigned int height, const GS_TARGET_FORMAT fmt);

	SpritePtr CreateSprite(GS_BYTE *pBuffer, const unsigned int bufferLength,
			GS_COLOR mask = gs2d::constant::ZERO, const unsigned int width = 0,
			const unsigned int height = 0);

	SpritePtr CreateSprite(const str_type::string& fileName,
			GS_COLOR mask = gs2d::constant::ZERO, const unsigned int width = 0,
			const unsigned int height = 0);

	SpritePtr CreateRenderTarget(const unsigned int width,
			const unsigned int height,
			const GS_TARGET_FORMAT format = GSTF_DEFAULT);

	ShaderPtr LoadShaderFromFile(const str_type::string& fileName,
			const GS_SHADER_FOCUS focus,
			const GS_SHADER_PROFILE profile = GSSP_HIGHEST,
			const char *entry = 0);

	ShaderPtr LoadShaderFromString(const str_type::string& shaderName,
			const std::string& codeAsciiString, const GS_SHADER_FOCUS focus,
			const GS_SHADER_PROFILE profile = GSSP_HIGHEST,
			const char *entry = 0);

	GLES2ShaderPtr LoadGLES2ShaderFromFile(const str_type::string& fileName, const GS_SHADER_FOCUS focus);

	GLES2ShaderPtr LoadGLES2ShaderFromString(const str_type::string& shaderName, const std::string& codeAsciiString, const GS_SHADER_FOCUS focus);

	boost::any GetVideoInfo();

	ShaderPtr GetFontShader();
	ShaderPtr GetDefaultVS();
	ShaderPtr GetOptimalVS();
	ShaderPtr GetVertexShader();

	GLES2ShaderPtr GetDefaultPS();

	ShaderPtr GetPixelShader();
	ShaderContextPtr GetShaderContext();
	bool SetVertexShader(ShaderPtr pShader);
	bool SetPixelShader(ShaderPtr pShader);
	GS_SHADER_PROFILE GetHighestVertexProfile() const;
	GS_SHADER_PROFILE GetHighestPixelProfile() const;

	boost::any GetGraphicContext();

	VIDEO_MODE GetVideoMode(const unsigned int modeIdx) const;
	unsigned int GetVideoModeCount();
	bool ResetVideoMode(const VIDEO_MODE& mode, const bool toggleFullscreen = false);
	bool ResetVideoMode(const unsigned int width, const unsigned int height, const GS_PIXEL_FORMAT pfBB, const bool toggleFullscreen = false);

	bool SetRenderTarget(SpritePtr pTarget, const unsigned int target = 0);
	unsigned int GetMaxRenderTargets() const;
	unsigned int GetMaxMultiTextures() const;
	bool SetBlendMode(const unsigned int passIdx, const GS_BLEND_MODE mode);
	GS_BLEND_MODE GetBlendMode(const unsigned int passIdx) const;
	bool UnsetTexture(const unsigned int passIdx);

	void SetZBuffer(const bool enable);
	bool GetZBuffer() const;

	void SetZWrite(const bool enable);
	bool GetZWrite() const;

	bool SetClamp(const bool set);
	bool GetClamp() const;

	bool SetSpriteDepth(const float depth);
	float GetSpriteDepth() const;

	void SetLineWidth(const float width);
	float GetLineWidth() const;

	bool SetCameraPos(const math::Vector2 &pos);
	bool MoveCamera(const math::Vector2 &dir);
	math::Vector2 GetCameraPos() const;

	void RoundUpPosition(const bool roundUp);
	bool IsRoundingUpPosition() const;

	bool SetScissor(const bool& enable);
	bool SetScissor(const math::Rect2D& rect);
	math::Rect2D GetScissor() const;
	void UnsetScissor();

	math::Vector2 ComputeCarretPosition(const str_type::string& font, const str_type::string& text, const unsigned int pos);
	math::Vector2 ComputeTextBoxSize(const str_type::string& font, const str_type::string& text);
	unsigned int FindClosestCarretPosition(const str_type::string& font, const str_type::string &text, const math::Vector2 &textPos,
			const math::Vector2 &reference);
	bool DrawBitmapText(const math::Vector2 &v2Pos, const str_type::string& text, const str_type::string& font, const GS_COLOR& color, const float scale);
	bool DrawLine(const math::Vector2 &p1, const math::Vector2 &p2, const GS_COLOR& color1, const GS_COLOR& color2);
	bool DrawRectangle(const math::Vector2 &v2Pos, const math::Vector2 &v2Size,	const GS_COLOR& color, const float angle = 0.0f,
			const GS_ENTITY_ORIGIN origin = GSEO_DEFAULT);
	bool DrawRectangle(const math::Vector2 &v2Pos, const math::Vector2 &v2Size,	const GS_COLOR& color0, const GS_COLOR& color1,
			const GS_COLOR& color2, const GS_COLOR& color3, const float angle = 0.0f,	const GS_ENTITY_ORIGIN origin = GSEO_DEFAULT);

	void SetBGColor(const GS_COLOR& backgroundColor);
	GS_COLOR GetBGColor() const;

	bool BeginSpriteScene(const GS_COLOR& dwBGColor = gs2d::constant::ZERO);
	bool EndSpriteScene();
	bool BeginTargetScene(const GS_COLOR& dwBGColor = gs2d::constant::ZERO, const bool clear = true);
	bool EndTargetScene();

	bool SetAlphaMode(const GS_ALPHA_MODE mode);
	GS_ALPHA_MODE GetAlphaMode() const;

	bool SetFilterMode(const GS_TEXTUREFILTER_MODE tfm);
	GS_TEXTUREFILTER_MODE GetFilterMode() const;

	bool Rendering() const;

	bool SaveScreenshot(const wchar_t *wcsName,	const GS_BITMAP_FORMAT fmt = GSBF_BMP, math::Rect2D rect = math::Rect2D(0, 0, 0, 0));
	
	bool ManageLoop();
	math::Vector2i GetClientScreenSize() const;
	APP_STATUS HandleEvents();
	float GetFPSRate() const;
	void Message(const str_type::string& text, const GS_MESSAGE_TYPE type = GSMT_ERROR) const;
	unsigned long GetElapsedTime(const TIME_UNITY unity = TU_MILLISECONDS) const;
	float GetElapsedTimeF(const TIME_UNITY unity = TU_MILLISECONDS) const;
	void ResetTimer();
	void Quit();
	void EnableQuitShortcuts(const bool enable);
	bool QuitShortcutsEnabled();
	bool SetWindowTitle(const str_type::string& title);
	str_type::string GetWindowTitle() const;
	void EnableMediaPlaying(const bool enable);
	bool IsWindowed() const;
	math::Vector2i GetScreenSize() const;
	math::Vector2 GetScreenSizeF() const;
	math::Vector2i GetWindowPosition();
	void SetWindowPosition(const math::Vector2i &v2);
	math::Vector2i ScreenToWindow(const math::Vector2i &v2Point) const;
	bool WindowVisible() const;
	bool WindowInFocus() const;
	bool HideCursor(const bool hide);
	bool IsCursorHidden() const;
	const Platform::FileLogger& GetLogger() const;

	void ForwardCommand(const str_type::string& cmd);
	str_type::string PullCommands();

	bool SetBlendTexture(const unsigned int passIdx, GLES2TexturePtr texture);
	void SetupMultitextureShader();
	void DisableMultitextureShader();

	bool IsTrue(const GLboolean& enabled);
	void SetBlend(const bool enable);

	str_type::string GetPlatformName() const;

private:
	GS_BLEND_MODE m_blendModes[_GS2D_GLES2_MAX_MULTI_TEXTURES];
	GLES2TexturePtr m_blendTextures[_GS2D_GLES2_MAX_MULTI_TEXTURES];

	math::Vector2 GetCurrentTargetSize() const;

	GS_COLOR m_backgroundColor;
	GS_ALPHA_MODE m_alphaMode;

	math::Vector2i m_screenSize;
	str_type::string m_externalStoragePath;
	str_type::string m_globalExternalStoragePath;
	str_type::string m_windowTitle;
	str_type::string m_commands;
	math::Rect2D m_scissor;
	bool m_quit;
	bool m_rendering;
	bool m_roundUpPosition;
	bool m_zWrite;
	bool m_zBuffer;
	bool m_blend;
	Platform::FileLogger m_logger;
	GLES2ShaderContextPtr m_shaderContext;
	GLES2ShaderPtr m_defaultVS, m_defaultPS,
		m_fastRenderVS, m_optimalVS, m_modulate1, m_add1;
	math::Matrix4x4 m_orthoMatrix;
	float m_fpsRate;
	timeval m_lastTime;

	GS_TEXTUREFILTER_MODE m_textureFilterMode;

	Platform::FileIOHubPtr m_fileIOHub;

	std::map<str_type::string, BitmapFontPtr> m_fonts;
	str_type::string m_defaultBitmapFontPath;
	
	TextureWeakPtr m_currentTarget;

	BitmapFontPtr LoadBitmapFont(const str_type::string& fullFilePath);
	math::Vector2 m_v2Camera;
	
	void ComputeFPSRate();

	void Enable2D(const int width, const int height, const bool flipY = false);

	BitmapFontPtr SeekBitmapFont(const str_type::string& font);
	
	bool StartApplication(const unsigned int width, const unsigned int height,
			const str_type::string& winTitle, const bool windowed,
			const bool sync, const GS_PIXEL_FORMAT pfBB = GSPF_UNKNOWN,
			const bool maximizable = false);

	Platform::FileIOHubPtr GetFileIOHub();

	#ifndef APPLE_IOS
		double GetElapsedTimeD(const TIME_UNITY unity = TU_MILLISECONDS) const;
	#endif

};
	
void UnbindFrameBuffer();

} // namespace gs2d

#endif
