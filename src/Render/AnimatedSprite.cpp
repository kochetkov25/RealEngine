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
	/*конструктор анимированного спрайта на основе обычного*/
	AnimatedSprite::AnimatedSprite(std::shared_ptr<Sprite> pSprite) : 
				_pSprite(std::move(pSprite)),
				_currFrame(0),
				_currFrameDuration(_pSprite->getFrameDuration(0)),
				_currAnimTime(0)
	{ }

	/*============================================================*/
	/*обновить состояние спрайта*/
	void AnimatedSprite::update(const uint64_t del)
	{
		_currAnimTime += del;
		while (_currAnimTime >= _currFrameDuration)
		{
			_currAnimTime -= _currFrameDuration;
			++_currFrame;

			if (_currFrame == _pSprite->getFramesCount())
				_currFrame = 0;

			_currFrameDuration = _pSprite->getFrameDuration(_currFrame);
		}
	}

	/*============================================================*/
	/*отрисовка спрайта*/
	void AnimatedSprite::render()
	{
		_pSprite->renderSprite(_currFrame);
	}


}