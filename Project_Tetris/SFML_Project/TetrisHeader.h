/**********************************************
************TETRIS GAME HEADERFILE*************
**********************************************/

#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

//Board size
const int length = 20;
const int width = 12;

class Tetris
{
public:
	Tetris();
	~Tetris();
	void Update(sf::RenderWindow* window, float dt);
	void Draw(sf::RenderWindow* window);
	void ButtonControls(sf::Event event);
	void Tick();
	void LineCheck();
	void RotateBlock();
	void SetNextBlock();
	void RandomFirstBlock();
	void SetScore(int value);
	void SetScoreOnFile(int m_score);
	void SetBackground(sf::RenderWindow* window, int n);
	int	 GetScore() { return m_score; }
	int ExtractHighscoreFromFile();
	bool m_IsOutOfBounds();
	bool m_isGameOver();
	
private:

	bool m_Rotate;
	bool m_StartGame = false;

	int m_blockFigures[7][4] =
	{
		1,3,5,7, // I
		2,4,5,7, // Z
		3,5,4,6, // S
		3,5,4,7, // T
		2,3,5,7, // L
		3,5,7,6, // J
		2,3,4,5, // O
	};
	
	struct m_Point
	{
		int x, y;
	}a[4],b[4];

	struct m_temp
	{
		int x, y;
	} tempA[4],tempB[4];

	int m_field[length][width] = { 0 };
	int m_dx;
	int m_colorNum;
	int m_nextColorNum;
	int m_score;
	int m_highscore;
	int m_backgroundChoice;
	double m_timer;
	double m_delay;
	
	sf::Texture m_spriteTexture;
	sf::Texture m_backgroundTexture1;
	sf::Texture m_backgroundTexture2;
	sf::Texture m_backgroundTexture3;
	sf::Sprite m_blockSprite;
	sf::Sprite m_nextBlock;
	sf::RectangleShape m_background;
	sf::RectangleShape m_border;
	sf::Font m_font;
	sf::Text m_scoreText;
	sf::Text m_highScoreText;
	sf::Text m_gameOverText;
	sf::Text m_menuText;
	sf::Text m_backgroundChoiceText;
	sf::Music m_backgroundMusic;
	sf::SoundBuffer m_buffer;
	sf::Sound m_scoreSound;
	
};