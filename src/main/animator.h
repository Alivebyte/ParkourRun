#ifndef ANIMATOR_H
#define ANIMATOR_H
// Simple override to make use of TransformedView
#include "olcPixelGameEngine.h"
#include "extensions/olcPGEX_TransformedView.h"
#include "olcPGEX_Animator2D.h"

class GameAnimator : public olcPGEX_Animator2D
{
public:
	struct Animation
	{
		std::string		strName = "";
		std::string		strPlayNext = "";			// v1.1

		bool			bIsPlaying = false;
		bool			bIsPaused = false;			// v1.5
		bool			bHasStopped = false;			// v1.6
		bool			bStopAfterComplete = false;
		bool			bStopNextAfterComplete = false;			// v1.1
		bool			bBillboardAnimation = false;
		bool			bPlayInReverse = false;			// v1.2 (updated v1.3)
		bool			bPingPong = false;			// v1.2

		float			fDuration = -1.0f;
		int			nNumberOfFrames = -1;
		float			fFrameLength = 0.0f;
		int			nCurrentFrame = 0;
		int			nFrameIncrement = 1;
		float			fFrameTick = 0.0f;
		float			fPlayAfterSeconds = 0.0f;			// v1.1

		olc::vf2d		vecFramePos{};
		olc::vf2d		vecFrameSize{};
		olc::vf2d		vecNextFrameOffset{};
		olc::vf2d		vecFrameDisplayOffset = {};
		olc::vf2d		vecOrigin = {};
		olc::vf2d		vecNoRotationPos = {};
		olc::vf2d		vecMirrorImage = {};			// v1.3 - set either axis or both to -1.0f to mirror the sprite image
		olc::vf2d		vecScale = { 1.0f, 1.0f }; 	// v1.4
		olc::Pixel		pTint = olc::WHITE;		// v1.4

		olc::Decal* decAnimDecal = nullptr;
	};

private:
	std::vector<Animation> anims;

public:
	std::string		errorMessage = "";			// you can access the last recorded error message from your parent classes in order to troubleshoot animation errors

	// Add a standard animation that can rotate around an origin (default)
	const void		AddAnimation(const std::string& animName, const float duration, const int numFrames, olc::Decal* decal, const olc::vf2d firstFramePos, const olc::vf2d frameSize, const olc::vf2d origin = { 0.0f, 0.0f }, const olc::vf2d frameDisplayOffset = { 0.0f, 0.0f }, const bool horizontalSprite = true, const bool playInReverse = false, const bool pingpong = false, const olc::vf2d mirrorImage = { 0.0f, 0.0f });


	// Add an animation that does not play by default, used for manually switching between frames on demand (ie switching between items on a HUD...)
	const void		AddStaticAnimation(const std::string& animName, const int numFrames, olc::Decal* decal, const olc::vf2d firstFramePos, const olc::vf2d frameSize, const olc::vf2d nextFrameOffset, const bool horizontalSprite = true, const bool playInReverse = false, const bool pingpong = false, const olc::vf2d mirrorImage = { 0.0f, 0.0f });


	// Add an animation that can rotate around an origin, but will always display upright (ie particle effects, flame, etc...)
	const void		AddBillboardAnimation(const std::string& animName, const float duration, const int numFrames, olc::Decal* decal, const olc::vf2d firstFramePos, const olc::vf2d frameSize, const olc::vf2d frameDisplayOffset = { 0.0f, 0.0f }, const bool horizontalSprite = true, const bool playInReverse = false, const bool pingpong = false, const olc::vf2d mirrorImage = { 0.0f, 0.0f });


	const void		SetNextAnimation(const std::string& animName, const std::string& nextAnimName, const bool bPlayOnce = false); // v1.1
	Animation* GetAnim(const std::string& name);

	const void		Play(const std::string& name, const bool bPlayOnce = false, const int startFrame = 0);
	const void		PlayAfterSeconds(const std::string& name, const float seconds, const bool bPlayOnce = false, const int startFrame = 0); // v1.1
	const bool		IsAnyAnimationPlaying();
	const void		Stop(const std::string& name, const bool bAfterCompletion = false);
	const void		StopAll();
	const void		Pause(const std::string& name, const bool bPaused = true); // v1.5
	const void		UpdateAnimations(const float fElapsedTime);

	const void		DrawAnimationFrame(const olc::vf2d pos, const float angle = 0.0f);
	const void		DrawAnimationFrame(const olc::vf2d pos, const float angle = 0.0f, olc::TransformedView* tv = nullptr);

	const void		ScaleAnimation(const std::string& animToScale, const olc::vf2d scale); // v1.6
	const void		TintAnimation(const std::string& animToTint, const olc::Pixel tint); // v1.6
	const void		AdjustAnimationDuration(const std::string& animToAdjust, const float newDuration); // v1.7

private:
	const bool		DuplicateAnimationExists(const std::string& name);
	
};

#endif