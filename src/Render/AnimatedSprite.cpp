/*
**  File        :	AnimatedSprite.cpp
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#include "AnimatedSprite.h"

namespace Render
{

	/*============================================================*/
	/*конструктор*/
	AnimatedSprite::AnimatedSprite(std::shared_ptr<Sprite> pSprite) : 
			_pSprite(std::move(pSprite)),
			_currFrame(0),
			_currFrameDuration(pSprite->getFrameDuration(0)),
			_currAnimTine(0)
	{ }

	/*============================================================*/
	void AnimatedSprite::update(const uint64_t del)
	{
		_currAnimTine += del;
		while (_currAnimTine >= _currFrameDuration)
		{
			_currAnimTine -= _currFrameDuration;
			++_currFrame;

			if (_currFrame == _pSprite->getFramesCount())
				_currFrame = 0;

			_currFrameDuration = _pSprite->getFrameDuration(_currFrame);
		}
	}

}