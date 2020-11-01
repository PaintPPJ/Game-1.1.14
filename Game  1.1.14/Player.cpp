#include "stdafx.h"
#include "Player.h"

void Player::initVariables()
{
	this->animState = PLAYER_ANIMATION_STATES::IDLE;
	//this->moving = false;
}

void Player::initTexture()
{
	if (!this->textureSheet.loadFromFile("images/Sprite mix.png"))
	{
		std::cout << "ERROR::PLAYER::Could not load the player sheet!" << "\n";
	}
}

void Player::initSprite()
{
	this->sprite.setTexture(this->textureSheet);
	this->currentFrame = sf::IntRect(0, 0, 200, 300);//SHOULD BE 40 50

	this->sprite.setTextureRect(this->currentFrame);
	this->sprite.setScale(1.0f, 1.0f);
}

void Player::initAnimation()
{
	this->animationTimer.restart();
	this->animationSwitch = true;
}

void Player::initPhysics()
{
	this->velocityMax = 15.f;
	this->velocityMin = 1.f;
	this->acceleration = 2.f;
	this->drag = 0.94f;
	this->gravity = 2.f; //delay filling sprite 2.f slow ,5.f fast
	this->velocityMaxY = 15.f;
}

Player::Player()
{
	this->initTexture();
	this->initSprite();
	this->initSprite();
	this->initAnimation();
	this->initPhysics();
}

Player::~Player()
{

}

const bool& Player::getAnimSwitch()
{
	// TODO: insert return statement here
	bool anim_switch = this->animationSwitch;

	if (animationSwitch)
		this->animationSwitch = false;

	return anim_switch;
}

const sf::FloatRect Player::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}

void Player::setPosition(const float x, const float y)
{
	this->sprite.setPosition(x, y);
}

void Player::resetVelocityY()
{
	this->velocity.y = 0.f;
}

void Player::resetAnimationTimer()
{
	this->animationTimer.restart();
	this->animationSwitch = true;
}

void Player::move(const float dir_x, const float dir_y)
{
	//Accrlrtation
	this->velocity.x += dir_x * this->acceleration;
	//this->velocity.y += dir_y * this->acceleration;

	//Limit velocity
	if (std::abs(this->velocity.x) > this->velocityMax)
	{
		this->velocity.x = this->velocityMax * ((this->velocity.x<0.f) ? -1.f : 1.f);
	}

}

void Player::updatePhysics()
{
	//Gravity
	this->velocity.y += 1.0 * this->gravity;
	if (std::abs(this->velocity.x) > this->velocityMaxY)
	{
		this->velocity.y = this->velocityMaxY * ((this->velocity.y < 0.f) ? -1.f : 1.f);
	}

	//Deceleration
	this->velocity *= this->drag;

	//Linit deceleration
	if (std::abs(this->velocity.x) < this->velocityMin)
		this->velocity.x = 0.f;
	if (std::abs(this->velocity.y) < this->velocityMin)
		this->velocity.y = 0.f;

	this->sprite.move(this->velocity);
}

void Player::updateMovement()
{
	//this->moving = false;
	this->animState = PLAYER_ANIMATION_STATES::IDLE;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))//Left
	{
		this->move(-5.f, 0.f);
		//this->moving = true;
		this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))//Right //void Player::updateAnimations() else if (this->animState == PLAYER_ANIMATION_STATES::MOVING_RIGHT)
	{
		this->move(5.f, 0.f);
		//this->moving = true;
		this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
	}
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))//Top
	{
		this->sprite.move(0.f, -5.f);
		//this->moving = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))//Down
	{
		this->sprite.move(0.f, 5.f);
		//this->moving = true;
	}*/
}

void Player::updateAnimations()
{
	if (this->animState == PLAYER_ANIMATION_STATES::IDLE)
	{
		if (this->animationTimer.getElapsedTime().asSeconds() >= 0.2f||this->getAnimSwitch())//Timer Fix Slow Fast Moving
		{
			this->currentFrame.top = 0.f;
			this->currentFrame.left += 200.f;
			if (this->currentFrame.left >= 200.f)
				this->currentFrame.left = 0.f;

			this->animationTimer.restart();
			this->sprite.setTextureRect(this->currentFrame);
		}
	}
	else if (this->animState == PLAYER_ANIMATION_STATES::MOVING_RIGHT)
	{
		if (this->animationTimer.getElapsedTime().asSeconds() >= 0.2f)
		{
			this->currentFrame.top = 0.f;
			this->currentFrame.left += 200.0f;
			if (this->currentFrame.left >= 1200.f)
			{
				this->currentFrame.left = 0.f;
			}

			this->animationTimer.restart();
			this->sprite.setTextureRect(this->currentFrame);
		}

	}
}

void Player::update()
{
	this->updateMovement();
	this->updateAnimations();
	this->updatePhysics();
}

void Player::render(sf::RenderTarget& targat)
{
	targat.draw(this->sprite);
}
