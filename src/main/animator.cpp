#include "animator.h"

const void GameAnimator::DrawAnimationFrame(const olc::vf2d pos, const float angle, olc::TransformedView* tv)
{
	for (auto& a : anims)
		if (a.bIsPlaying)
		{
			if (a.nCurrentFrame > a.nNumberOfFrames - 1) a.nCurrentFrame = a.nNumberOfFrames - 1;

			if (a.bBillboardAnimation)
			{
				// translate pos based on rotation around origin
				float s = angle == 0.0f ? 0.0f : sinf(angle);
				float c = angle == 0.0f ? 1.0f : cosf(angle);

				olc::vf2d vecBillboardPos;

				vecBillboardPos.x = c * a.vecFrameDisplayOffset.x - s * a.vecFrameDisplayOffset.y + a.vecOrigin.x;
				vecBillboardPos.y = s * a.vecFrameDisplayOffset.x + c * a.vecFrameDisplayOffset.y + a.vecOrigin.y;

				// offset to account for frame size
				vecBillboardPos.x -= a.vecFrameSize.x * 0.5f * a.vecScale.x;
				vecBillboardPos.y -= a.vecFrameSize.y * a.vecScale.y;
				if (tv)
				{
					tv->DrawPartialDecal(pos + vecBillboardPos + (-a.vecMirrorImage * a.vecFrameSize), a.decAnimDecal, a.vecFramePos + (a.vecNextFrameOffset * (float)a.nCurrentFrame), a.vecFrameSize, { a.vecMirrorImage.x < 0.0f ? -1.0f * a.vecScale.x : 1.0f * a.vecScale.x, a.vecMirrorImage.y < 0.0f ? -1.0f * a.vecScale.y : 1.0f * a.vecScale.y }, a.pTint);
				}
				else
				{
					pge->DrawPartialDecal(pos + vecBillboardPos + (-a.vecMirrorImage * a.vecFrameSize), a.decAnimDecal, a.vecFramePos + (a.vecNextFrameOffset * (float)a.nCurrentFrame), a.vecFrameSize, { a.vecMirrorImage.x < 0.0f ? -1.0f * a.vecScale.x : 1.0f * a.vecScale.x, a.vecMirrorImage.y < 0.0f ? -1.0f * a.vecScale.y : 1.0f * a.vecScale.y }, a.pTint);
				}
			}
			else
				if(tv)
				{
					tv->DrawPartialRotatedDecal(pos, a.decAnimDecal, angle, a.vecOrigin - a.vecFrameDisplayOffset * a.vecScale, a.vecFramePos + (a.vecNextFrameOffset * (float)a.nCurrentFrame), a.vecFrameSize, { a.vecMirrorImage.x < 0.0f ? -1.0f * a.vecScale.x : 1.0f * a.vecScale.x, a.vecMirrorImage.y < 0.0f ? -1.0f * a.vecScale.y : 1.0f * a.vecScale.y }, a.pTint);
				}
				else 
				{
					pge->DrawPartialRotatedDecal(pos, a.decAnimDecal, angle, a.vecOrigin - a.vecFrameDisplayOffset * a.vecScale, a.vecFramePos + (a.vecNextFrameOffset * (float)a.nCurrentFrame), a.vecFrameSize, { a.vecMirrorImage.x < 0.0f ? -1.0f * a.vecScale.x : 1.0f * a.vecScale.x, a.vecMirrorImage.y < 0.0f ? -1.0f * a.vecScale.y : 1.0f * a.vecScale.y }, a.pTint);
				}
		}
}

const void GameAnimator::AddAnimation(const std::string& animName, const float duration, const int numFrames, olc::Decal* decal, const olc::vf2d firstFramePos, const olc::vf2d frameSize, const olc::vf2d origin, const olc::vf2d frameDisplayOffset, const bool horizontalSprite, const bool playInReverse, const bool pingpong, const olc::vf2d mirrorImage)
{
	// Prevent multiple animations with the same name
	if (!DuplicateAnimationExists(animName))
		return;

	// Add new animation to list of animations
	Animation newAnim;

	newAnim.strName = animName;
	newAnim.fDuration = duration;
	newAnim.nNumberOfFrames = numFrames;
	newAnim.fFrameLength = duration / numFrames;

	newAnim.vecFramePos = firstFramePos;
	newAnim.vecFrameSize = frameSize;
	newAnim.bPlayInReverse = playInReverse;

	if (horizontalSprite)
		newAnim.vecNextFrameOffset = newAnim.bPlayInReverse ? olc::vf2d(-frameSize.x, 0.0f) : olc::vf2d(frameSize.x, 0.0f);
	else
		newAnim.vecNextFrameOffset = newAnim.bPlayInReverse ? olc::vf2d(0.0f, -frameSize.y) : olc::vf2d(0.0f, frameSize.y);

	newAnim.vecFrameDisplayOffset = frameDisplayOffset;
	newAnim.vecOrigin = origin;
	newAnim.decAnimDecal = decal;
	newAnim.bPingPong = pingpong;
	newAnim.vecMirrorImage = mirrorImage;

	anims.push_back(newAnim);
}

const void GameAnimator::AddStaticAnimation(const std::string& animName, const int numFrames, olc::Decal* decal, const olc::vf2d firstFramePos, const olc::vf2d frameSize, const olc::vf2d nextFrameOffset, const bool horizontalSprite, const bool playInReverse, const bool pingpong, const olc::vf2d mirrorImage)
{
	// Prevent multiple animations with the same name
	if (!DuplicateAnimationExists(animName))
		return;

	// Add new animation to list of animations
	Animation newAnim;

	newAnim.strName = animName;
	newAnim.nNumberOfFrames = numFrames;

	newAnim.vecFramePos = firstFramePos;
	newAnim.vecFrameSize = frameSize;
	newAnim.bPlayInReverse = playInReverse;

	if (horizontalSprite)
		newAnim.vecNextFrameOffset = newAnim.bPlayInReverse ? olc::vf2d(-frameSize.x, 0.0f) : olc::vf2d(frameSize.x, 0.0f);
	else
		newAnim.vecNextFrameOffset = newAnim.bPlayInReverse ? olc::vf2d(0.0f, -frameSize.y) : olc::vf2d(0.0f, frameSize.y);

	newAnim.vecNextFrameOffset = nextFrameOffset;
	newAnim.decAnimDecal = decal;
	newAnim.bPingPong = pingpong;
	newAnim.vecMirrorImage = mirrorImage;

	anims.push_back(newAnim);
}

const void GameAnimator::AddBillboardAnimation(const std::string& animName, const float duration, const int numFrames, olc::Decal* decal, const olc::vf2d firstFramePos, const olc::vf2d frameSize, const olc::vf2d frameDisplayOffset, const bool horizontalSprite, const bool playInReverse, const bool pingpong, const olc::vf2d mirrorImage)
{
	// Prevent multiple animations with the same name
	if (!DuplicateAnimationExists(animName))
		return;

	// Add new animation to list of animations
	Animation newAnim;

	newAnim.strName = animName;
	newAnim.fDuration = duration;
	newAnim.nNumberOfFrames = numFrames;
	newAnim.fFrameLength = duration / numFrames;

	newAnim.vecFramePos = firstFramePos;
	newAnim.vecFrameSize = frameSize;
	newAnim.bPlayInReverse = playInReverse;

	if (horizontalSprite)
		newAnim.vecNextFrameOffset = newAnim.bPlayInReverse ? olc::vf2d(-frameSize.x, 0.0f) : olc::vf2d(frameSize.x, 0.0f);
	else
		newAnim.vecNextFrameOffset = newAnim.bPlayInReverse ? olc::vf2d(0.0f, -frameSize.y) : olc::vf2d(0.0f, frameSize.y);

	newAnim.vecFrameDisplayOffset = frameDisplayOffset;
	newAnim.decAnimDecal = decal;
	newAnim.bBillboardAnimation = true;
	newAnim.bPingPong = pingpong;
	newAnim.vecMirrorImage = mirrorImage;

	anims.push_back(newAnim);
}

const void GameAnimator::SetNextAnimation(const std::string& animName, const std::string& nextAnimName, const bool bPlayOnce)
{
	for (auto& a : anims)
		if (a.strName == animName)
		{
			a.strPlayNext = nextAnimName;
			a.bStopNextAfterComplete = bPlayOnce;
		}

	return;

	errorMessage = "Animation (" + animName + ") - not a valid animation name... [SetNextAnimation]";
}

GameAnimator::Animation* GameAnimator::GetAnim(const std::string& name)
{
	for (auto& a : anims)
		if (a.strName == name)
			return &a;

	errorMessage = "Unable to get animation (" + name + ") - not a valid animation name... [GetAnim]";
	return nullptr;
}

const void GameAnimator::Play(const std::string& name, const bool bPlayOnce, const int startFrame)
{
	for (auto& a : anims)
		if (a.strName == name)
		{
			a.bIsPlaying = true;
			a.bIsPaused = false;

			// Prevent starting on an invalid frame
			if (a.fPlayAfterSeconds == -1.0f)
			{
				if (a.nCurrentFrame > a.nNumberOfFrames) a.nCurrentFrame = 0;
				a.fPlayAfterSeconds = 0.0f;
			}
			else
			{
				a.nCurrentFrame = startFrame < a.nNumberOfFrames ? startFrame : 0;
				a.bStopAfterComplete = bPlayOnce;
			}

			a.fFrameTick = 0.0f;
			a.nFrameIncrement = 1;

			return;
		}

	errorMessage = "Unable to play animation (" + name + ") - not a valid animation name... [Play]";
}

const void GameAnimator::PlayAfterSeconds(const std::string& name, const float seconds, const bool bPlayOnce, const int startFrame)
{
	for (auto& a : anims)
		if (a.strName == name)
		{
			a.fPlayAfterSeconds = seconds;
			a.nCurrentFrame = startFrame < a.nNumberOfFrames ? startFrame : 0;
			a.bStopAfterComplete = bPlayOnce;
		}

	return;

	errorMessage = "Unable to play animation (" + name + ") - not a valid animation name... [PlayAfterSeconds]";
}

const bool GameAnimator::IsAnyAnimationPlaying()
{
	for (auto& a : anims)
		if (a.bIsPlaying)
			return true;

	return false;
}

const void GameAnimator::Stop(const std::string& name, const bool bAfterCompletion)
{
	for (auto& a : anims)
		if (a.strName == name)
		{
			if (bAfterCompletion)
			{
				a.bStopAfterComplete = true;
				return;
			}
			else
			{
				a.bIsPlaying = false;
				a.bIsPaused = false;
				a.bHasStopped = true;

				if (a.strPlayNext != "")
					Play(a.strPlayNext, a.bStopNextAfterComplete);

				return;
			}
		}

	errorMessage = "Unable to stop animation (" + name + ") - not a valid animation name... [Stop]";
}

const void GameAnimator::StopAll()
{
	for (auto& a : anims)
		Stop(a.strName);
}

const void GameAnimator::Pause(const std::string& name, const bool bPaused)
{
	for (auto& a : anims)
		if (a.strName == name)
			if (a.bIsPlaying)
			{
				if (a.bIsPaused)
					a.bIsPaused = false;
				else
					a.bIsPaused = true;

				return;
			}

	errorMessage = "Unable to pause/resume animation (" + name + ") - not a valid animation name... [Pause]";
}

const void GameAnimator::UpdateAnimations(const float fElapsedTime)
{
	for (auto& a : anims)
	{
		a.bHasStopped = false;			// Reset the HasStopped value each update frame (as this is a trigger)

		if (a.fPlayAfterSeconds > 0.0f)
		{
			a.fPlayAfterSeconds -= fElapsedTime;
			if (a.fPlayAfterSeconds <= 0.0f)
			{
				a.fPlayAfterSeconds = -1.0f;			// Tell the play function to use existing Play presets from previous call...
				Play(a.strName);
			}
		}

		if (a.bIsPlaying && a.fDuration >= 0.0f)
		{
			if (!a.bIsPaused) a.fFrameTick += fElapsedTime;
			if (a.fFrameTick > a.fFrameLength)
			{
				a.fFrameTick -= a.fFrameTick;
				a.nCurrentFrame += a.nFrameIncrement;
				if (a.nCurrentFrame == a.nNumberOfFrames)
				{
					if (a.bPingPong)
					{
						a.nCurrentFrame--;
						a.nFrameIncrement = -1;
					}
					else
					{
						a.nCurrentFrame = 0;
						if (a.bStopAfterComplete)
						{
							a.bIsPlaying = false;
							a.bIsPaused = false;
							a.bHasStopped = true;

							if (a.strPlayNext != "")
								Play(a.strPlayNext, a.bStopNextAfterComplete);

						}
					}
				}

				if (a.bPingPong && a.nCurrentFrame == 0)
				{
					if (a.bStopAfterComplete)
					{
						a.bIsPlaying = false;
						a.bIsPaused = false;
						a.bHasStopped = true;

						if (a.strPlayNext != "")
							Play(a.strPlayNext, a.bStopNextAfterComplete);

					}
					else
					{
						a.nCurrentFrame++;
						a.nFrameIncrement = 1;
					}
				}
			}
		}
	}
}

const void GameAnimator::DrawAnimationFrame(const olc::vf2d pos, const float angle)
{
	for (auto& a : anims)
		if (a.bIsPlaying)
		{
			if (a.nCurrentFrame > a.nNumberOfFrames - 1) a.nCurrentFrame = a.nNumberOfFrames - 1;

			if (a.bBillboardAnimation)
			{
				// translate pos based on rotation around origin
				float s = angle == 0.0f ? 0.0f : sinf(angle);
				float c = angle == 0.0f ? 1.0f : cosf(angle);

				olc::vf2d vecBillboardPos;

				vecBillboardPos.x = c * a.vecFrameDisplayOffset.x - s * a.vecFrameDisplayOffset.y + a.vecOrigin.x;
				vecBillboardPos.y = s * a.vecFrameDisplayOffset.x + c * a.vecFrameDisplayOffset.y + a.vecOrigin.y;

				// offset to account for frame size
				vecBillboardPos.x -= a.vecFrameSize.x * 0.5f * a.vecScale.x;
				vecBillboardPos.y -= a.vecFrameSize.y * a.vecScale.y;

				pge->DrawPartialDecal(pos + vecBillboardPos + (-a.vecMirrorImage * a.vecFrameSize), a.decAnimDecal, a.vecFramePos + (a.vecNextFrameOffset * (float)a.nCurrentFrame), a.vecFrameSize, { a.vecMirrorImage.x < 0.0f ? -1.0f * a.vecScale.x : 1.0f * a.vecScale.x, a.vecMirrorImage.y < 0.0f ? -1.0f * a.vecScale.y : 1.0f * a.vecScale.y }, a.pTint);
			}
			else
				pge->DrawPartialRotatedDecal(pos, a.decAnimDecal, angle, a.vecOrigin - a.vecFrameDisplayOffset * a.vecScale, a.vecFramePos + (a.vecNextFrameOffset * (float)a.nCurrentFrame), a.vecFrameSize, { a.vecMirrorImage.x < 0.0f ? -1.0f * a.vecScale.x : 1.0f * a.vecScale.x, a.vecMirrorImage.y < 0.0f ? -1.0f * a.vecScale.y : 1.0f * a.vecScale.y }, a.pTint);
		}
}

const void GameAnimator::ScaleAnimation(const std::string& animToScale, const olc::vf2d scale)
{
	auto& a = GetAnim(animToScale)->vecScale = scale;
}

const void GameAnimator::TintAnimation(const std::string& animToTint, const olc::Pixel tint)
{
	auto& a = GetAnim(animToTint)->pTint = tint;
}

const void GameAnimator::AdjustAnimationDuration(const std::string& animToAdjust, const float newDuration)
{
	GameAnimator::Animation* a = GetAnim(animToAdjust);

	// Return early with an error if the specified animation is not found
	if (a == nullptr)
	{
		errorMessage = "Animation does not exist... [AdjustAnimationDuration]";
		return;
	}

	// Set the new duration and calculate the new frame length
	a->fDuration = newDuration;
	a->fFrameLength = a->fDuration / a->nNumberOfFrames;

	// If the animation is currently playing (or paused) then we need to adjust
	// the frameTick by the correct proportion based on the new frameLength
	if (a->bIsPlaying || a->bIsPaused)
	{
		const float fFractionOfCurrentFrame = a->fFrameTick / a->fFrameLength;
		a->fFrameTick = a->fFrameLength * fFractionOfCurrentFrame;
	}
}

const bool GameAnimator::DuplicateAnimationExists(const std::string& name)
{
	for (auto& a : anims)
		if (a.strName == name)
		{
			errorMessage = "Tried to create multiple animations with the same name... [AddAnimation_XX]";
			return false;
		}

	return true;
}