/**********************************************
*********Tetris Game implementation ***********
*********************************************/


#include "TetrisHeader.h"
#include <iostream>
#include <fstream>
#include <string>


//Class Constructor
Tetris::Tetris()
	: m_dx(0)
	, m_colorNum(rand() % 7)
	, m_timer(0)
	, m_delay(0.3)
	, m_Rotate(false)
	, m_blockSprite(m_spriteTexture)
	, m_nextBlock(m_spriteTexture)
	, m_scoreSound(m_buffer)
	
	
{
	
	m_spriteTexture.loadFromFile("images/tiles.png");
	m_font.loadFromFile("Fonts/arial.ttf");
	m_backgroundTexture1.loadFromFile("images/Space.png");
	m_backgroundTexture2.loadFromFile("images/space2.png");
	m_backgroundTexture3.loadFromFile("images/space3.png");
	m_backgroundMusic.openFromFile("Audio/spaceTetris.wav");
	m_buffer.loadFromFile("Audio/Pickup_Coin.wav");

	m_backgroundMusic.play();
	m_backgroundMusic.setLoop(true);
	m_backgroundMusic.setVolume(20);
	m_highscore = ExtractHighscoreFromFile();
	
	RandomFirstBlock();
	SetNextBlock();
	
	
}

//class destructor
Tetris::~Tetris()
{
	std::cout << "Destructing" << std::endl;
}


//call this function to update game per tick

void Tetris::Update(sf::RenderWindow* window,float dt)
{
	if (m_StartGame)
	{
		m_timer += dt;
		// the for loop for checking the blocks position in x direction
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].x += m_dx;
		}

		//check if block is colliding with wall
		if (!m_IsOutOfBounds())
		{
			for (int i = 0; i < 4; i++)
			{
				for (int i = 0; i < 4; i++)
				{
					a[i] = b[i];
				}
			}
		}

		Tick();
		m_dx = 0;
		m_Rotate = 0;
		m_delay = 0.3;

	}
}

//Main drawing function that handles drawing sprites, text, borders and the board itself
void Tetris::Draw(sf::RenderWindow * window)
{
	
	SetBackground(window, m_backgroundChoice);
	

	//drawing the score Text
	m_scoreText.setFont(m_font);
	m_scoreText.setCharacterSize(25);
	m_scoreText.setFillColor(sf::Color::Green);
	m_scoreText.setString("Score: \n " + std::to_string(m_score));
	m_scoreText.setPosition(50, 470);
	window->draw(m_scoreText);


	//drawing the highscore text
	m_highScoreText.setFont(m_font);
	m_highScoreText.setCharacterSize(25);
	m_highScoreText.setFillColor(sf::Color::Yellow);
	m_highScoreText.setString("HighScore \n" + std::to_string(m_highscore));
	m_highScoreText.setPosition(250, 470);
	window->draw(m_highScoreText);


	//drawing gameover text
	m_gameOverText.setFont(m_font);
	m_gameOverText.setCharacterSize(30);
	m_gameOverText.setFillColor(sf::Color(sf::Color(255, rand() % 255, rand() % 255, rand() % 255)));
	m_gameOverText.setString("GAME OVER!");
	m_gameOverText.setPosition(38, 125);
	if (m_isGameOver())
	{
		window->draw(m_gameOverText);
	}

	//drawing the main menu
	m_menuText.setFont(m_font);
	m_menuText.setCharacterSize(20);
	m_menuText.setFillColor(sf::Color::Yellow);
	m_menuText.setString("Press Enter to Start");
	m_menuText.setPosition(40,125);
	if (m_StartGame == false)
	{
		window->draw(m_menuText);
	}

	//drawing the text for background choice
	m_backgroundChoiceText.setFont(m_font);
	m_backgroundChoiceText.setCharacterSize(20);
	m_backgroundChoiceText.setFillColor(sf::Color::Yellow);
	m_backgroundChoiceText.setString("Choose your \nBackground: \n1: Space 1 \n2: Space 2 \n3: Space 3");
	m_backgroundChoiceText.setPosition(280, 280);
	window->draw(m_backgroundChoiceText);
	

	//drawing the border
	m_border.setFillColor(sf::Color::Transparent);
	m_border.setSize(sf::Vector2f(width * 18, length* 18)); //set size based on the board
	//m_border.setSize(sf::Vector2f(216, 360)); // maunally calculated size
	m_border.setPosition(28, 31);
	m_border.setOutlineThickness(10);
	m_border.setOutlineColor(sf::Color::White);
	window->draw(m_border);

	
	if (!m_isGameOver() && m_StartGame)
	{
		//drawing the sprites onto the board
		for (int i = 0; i < length; i++)
		{

			for (int j = 0; j < width; j++)
			{
				if (m_field[i][j] == 0) continue;

				m_blockSprite.setTextureRect(sf::IntRect(m_field[i][j] * 18, 0, 18, 18));
				m_blockSprite.setPosition(j * 18, i * 18);
				m_blockSprite.move(28, 31);
				window->draw(m_blockSprite);

			}
		}

		// drawing the piece as it moves
		for (int i = 0; i < 4; i++)
		{
			m_blockSprite.setTextureRect(sf::IntRect(m_colorNum * 18, 0, 18, 18));
			m_blockSprite.setPosition(a[i].x * 18, a[i].y * 18);
			m_blockSprite.move(28, 12);
			window->draw(m_blockSprite);
		
		}

		// Drawing the next piece
		for (int i = 0; i < 4; i++)
		{
			m_nextBlock.setTextureRect(sf::IntRect(m_nextColorNum * 18, 0, 18, 18));
			m_nextBlock.setPosition(tempA[i].x * 18, tempA[i].y * 18);
			m_nextBlock.move(300, 190);
			window->draw(m_nextBlock);
			
		}
	}

	
}


// bool function to check for collision of the boards borders
bool Tetris::m_IsOutOfBounds()
{
	for (int i = 0; i < 4; i++)
	{
		if (a[i].x < 0 || a[i].x >= width || a[i].y >= length)
		{
			return false;
		}
		else if (m_field[a[i].y][a[i].x])
		{
			return false;
		}
	}
		
	return true;

}



//bool function to check for when the player reaches the top of the border it ends the game
bool Tetris::m_isGameOver()
{
	//check if blocks  reach top
	for (int i = length - 19; i > 0; i--)
	{
		for (int j = 0; j < width; j++)
		{
			if (m_field[i][j])
			{
				// once game finishes set the current score on text file
				if (m_score > m_highscore)
				{
					SetScoreOnFile(m_score);
				}
				
				return true;
			}
		}
	}

	return false;
}


// main void function to handle tick 
void Tetris::Tick()
{
	if (!m_isGameOver())
	{
		if (m_timer > m_delay)
		{
			// for every tick move the block in the Y direction
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].y += 1;
			}

			if (!m_IsOutOfBounds())
			{
				for (int i = 0; i < 4; i++)
				{
					m_field[b[i].y][b[i].x] = m_colorNum;
				}


				m_colorNum = m_nextColorNum;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = tempA[i].x;
					a[i].y = tempA[i].y;
				}


				SetNextBlock();
			}
		}


		m_timer = 0;
		LineCheck();
	}
}

// player input function
void Tetris::ButtonControls(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Up)
		{
			m_Rotate = true;
			RotateBlock();
		}
		else if (event.key.code == sf::Keyboard::Left)
		{
			m_dx = -1; 
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			m_dx = 1; 
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_delay = 0.05;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		m_StartGame = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		m_backgroundChoice = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		m_backgroundChoice = 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		m_backgroundChoice = 3;
	}
}

void Tetris::LineCheck()
{
	// Check if blocks fill up a line
	int temp = length - 1;
	int scored = 0;
	for (int i = length - 1; i > 0; i--)
	{
		int count = 0;
		
		for (int j = 0; j < width; j++)
		{
			if (m_field[i][j])
			{
				count++;
				
			}
			m_field[temp][j] = m_field[i][j];
		}
		if (count < width)
		{
			temp--;
			
		}
		else // if player scored count how many lines were cleared
		{
			scored++;
			m_scoreSound.play();
		}
		
	
	}

	//add score
	switch (scored)
	{
	case 1:
		SetScore(100);
		break;
	case 2:
		SetScore(200);
		break;
	case 3:
		SetScore(400);
		break;
	case 4:
		SetScore(800);
		break;
	}
	



}

//Call this function to rotate block at its given center point
void Tetris::RotateBlock()
{
	if (m_Rotate)
	{
	
		m_Point p = a[1]; // point of rotation
		for (int i = 0; i < 4; i++)
		{
			int x = a[i].y - p.y;
			int y = a[i].x - p.x;
			a[i].x = p.x - x;
			a[i].y = p.y + y;
		}

		if (!m_IsOutOfBounds())
		{
			for (int i = 0; i < 4; i++)
			{
				a[i] = b[i];
			}
		}
	}
}

//Call this function when setting the next block to spawn
void Tetris::SetNextBlock()
{
	m_nextColorNum = 1 + rand() % 7;
	int n = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		tempA[i].x = m_blockFigures[n][i] % 2;
		tempA[i].y = m_blockFigures[n][i] / 2;
	}
}

//function to call when creating first random block
void Tetris::RandomFirstBlock()
{

	//random firstblock creation
	int n = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		a[i].x = m_blockFigures[n][i] % 2;
		a[i].y = m_blockFigures[n][i] / 2;
	}
}

//Function to call to set/add score to current score
void Tetris::SetScore(int value)
{
	m_score = m_score + value;
}

void Tetris::SetScoreOnFile(int m_score)
{
	std::ofstream highscorefile("Highscore.txt");
	if (highscorefile.is_open())
	{
		highscorefile << m_score << std::endl;
		
	}
	else
	{
		std::cout << "Unable to open file" << std::endl;
	}

}

int Tetris::ExtractHighscoreFromFile()
{
	std::string line;
	int score;
	std::ifstream highscorefile("Highscore.txt");
	if (highscorefile.is_open())
	{
		std::getline(highscorefile, line);
		score = std::stoi(line);

		highscorefile.close();
	}

	return score;
}

void Tetris::SetBackground(sf::RenderWindow* window, int n)
{
	//drawing the background
	
	
	switch (n)
	{
	case 1:
		m_background.setTexture(&m_backgroundTexture1);
		break;
	case 2:
		m_background.setTexture(&m_backgroundTexture2);
		break; 
	case 3:
		m_background.setTexture(&m_backgroundTexture3);
		break;
	default:
		m_background.setTexture(&m_backgroundTexture3);
		break;
	}

	
	m_background.setSize(sf::Vector2f(420, 580)); 
	window->draw(m_background);

}