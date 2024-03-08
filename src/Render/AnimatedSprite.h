/*
**  File        :	AnimatedSprite.h
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#pragma once

#include "Sprite.h"

#include <memory>

namespace Render
{

	class AnimatedSprite
	{
	public:
		AnimatedSprite(std::shared_ptr<Sprite> pSprite);

		size_t getCurrentFrame() const
		{
			return _currFrame;
		}
		void update(const uint64_t del);
	private:
		std::shared_ptr<Sprite> _pSprite;
		size_t _currFrame;
		uint64_t _currFrameDuration;
		uint64_t _currAnimTine;
	};


}
