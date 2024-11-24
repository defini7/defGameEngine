#define DGE_APPLICATION
#include "defGameEngine.hpp"

#include <array>

using namespace def;

// If you want to change it then edit
// the RotatePiece function by rewriting indecies there
const def::vi2d TETROMINO_SIZE = { 4, 4 };

// https://tetris.fandom.com/wiki/Tetromino
// Construct tetrominos assuming that each piece is 4x4 in size
const std::array<std::string, 7> TETROMINOS =
{
	"..c...c...c...c.", // def::CYAN
	"..b...b..bb.....", // def::BLUE
	".o...o...oo.....", // def::ORANGE
	".....yy..yy.....", // def::YELLOW
	".g...gg...g.....", // def::GREEN
	"..p..pp...p.....", // def::PURPLE
	"..r..rr..r......", // def::RED
};

def::Pixel GetColor(char c)
{
	switch (c)
	{
	case 'c': return def::CYAN;
	case 'b': return def::BLUE;
	case 'o': return def::ORANGE;
	case 'y': return def::YELLOW;
	case 'g': return def::GREEN;
	case 'p': return def::PURPLE;
	case 'r': return def::RED;
	case '#': return def::DARK_GREY;
	case ' ': return def::WHITE;
	}

	return def::NONE;
}

// Assume that every piece is 4x4 in size,
// so applying simple math we get new indices
// of the pieces in map array
int RotatePiece(const def::vi2d& p, uint8_t r)
{
	switch (r % 4)
	{
	case 0: return p.y * 4 + p.x; // 0 deg
	case 1: return 12 + p.y - p.x * 4; // 90 deg
	case 2: return 15 - p.y * 4 - p.x; // 180 deg
	case 3: return 3 - p.y + p.x * 4; // 270 deg
	}
}

class Tetris : public def::GameEngine
{
public:
	Tetris()
	{
		SetTitle("Tetris");
	}

	// We will update that string variable
	// and match each character to the screen pixels
	// equivalent
	std::string field;

	def::vi2d fieldPos = { 11, 1 };
	def::vi2d fieldSize = { 10, 20 };
	def::vi2d cellSize = { 8, 8 };

	int score = 0;
	int piece = -1;
	int nextPiece = -1;
	int rotation = 0;

	def::vf2d piecePos;

	bool gameOver = false;

	bool DoesPieceFit(const def::vi2d& pos, size_t i, int r)
	{
		if (i >= TETROMINOS.size()) return false;

		for (int y = 0; y < TETROMINO_SIZE.y; y++)
			for (int x = 0; x < TETROMINO_SIZE.x; x++)
			{
				int fi = (pos.y + y) * fieldSize.x + (pos.x + x);

				// Now we are checking each tetromino local cell against
				// it's global analogue
				if (pos.y + y >= 0 && pos.y + y < fieldSize.y)
					if (pos.x + x >= 0 && pos.x + x < fieldSize.x)
					{
						// So if it's not a blank cell we check whether
						// rotated tetromino piece fits
						// in map's cell or not
						if (field[fi] != ' ' && TETROMINOS[i][RotatePiece({ x, y }, r)] != '.')
							return false;
					}
			}

		return true;
	}

	/*
	* Draws piece and it's background
	* based on specified flags
	*/
	void DrawPiece(const def::vi2d& pos, size_t i, int rot, bool fillBlank, bool colorised, const def::Pixel& blankCol = def::BLACK)
	{
		def::vi2d p;
		for (p.y = 0; p.y < TETROMINO_SIZE.y; p.y++)
			for (p.x = 0; p.x < TETROMINO_SIZE.x; p.x++)
			{
				def::vi2d gp = (fieldPos + def::vi2d(pos) + p) * cellSize;
				char cell = TETROMINOS[i][RotatePiece(p, rot)];

				if (cell != '.')
					FillRectangle(gp, cellSize, colorised ? GetColor(cell) : def::GREY);
				else if (fillBlank)
					FillRectangle(gp, cellSize, blankCol);
			}
	}

protected:
	bool OnUserCreate() override
	{
		field.resize(fieldSize.x * fieldSize.y, ' ');

		for (int y = 0; y < fieldSize.y; y++)
			for (int x = 0; x < fieldSize.x; x++)
			{
				if (x == 0 || x == fieldSize.x - 1 || y == fieldSize.y - 1)
					field[y * fieldSize.x + x] = '#';
			}

		nextPiece = rand() % TETROMINOS.size();

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (gameOver)
		{
			DrawString(4, cellSize.y * 4, "Game Over!", def::WHITE);
			return true;
		}

		// Pick new piece, put it to the top
		// in the center
		if (piece < 0)
		{
			piece = nextPiece;
			nextPiece = rand() % TETROMINOS.size();
			piecePos = { (float)fieldSize.x * 0.5f, 0.0f };
			rotation = 0;
		}

		if (GetKey(def::Key::SPACE).pressed && DoesPieceFit(piecePos, piece, rotation + 1))
			rotation++;

		if (GetKey(def::Key::LEFT).pressed && DoesPieceFit(piecePos + def::vf2d(-1, 0), piece, rotation))
			piecePos.x--;

		if (GetKey(def::Key::RIGHT).pressed && DoesPieceFit(piecePos + def::vf2d(+1, 0), piece, rotation))
			piecePos.x++;

		if (GetKey(def::Key::DOWN).held && DoesPieceFit(piecePos + def::vf2d(0, 5.0f * deltaTime), piece, rotation))
			piecePos.y += 5.0f * deltaTime;

		// If piece fits, then move it
		// one cell down

		if (DoesPieceFit(piecePos + def::vf2d(0.0f, deltaTime * 5.0f), piece, rotation))
			piecePos.y += deltaTime * 5.0f;
		else
		{
			if (piecePos.y == 0.0f)
				gameOver = true;

			// Otherwise place it into field
			for (int y = 0; y < TETROMINO_SIZE.y; y++)
				for (int x = 0; x < TETROMINO_SIZE.x; x++)
				{
					int ri = RotatePiece({ x, y }, rotation);
					int gi = ((int)piecePos.y + y) * fieldSize.x + ((int)piecePos.x + x);

					// If cell in tetromino space (4x4) isn't blank,
					// then update that cell in the field string

					if (TETROMINOS[piece][ri] != '.')
						field[gi] = TETROMINOS[piece][ri];
				}

			// Reset piece and let program to create new one
			piece = -1;
		}

		// Destruct all fully filled lines
		int destructedLines = 0;

		def::vi2d p;
		for (p.y = 0; p.y < fieldSize.y; p.y++)
		{
			bool fullyFilled = true;

			// Find fully filled line
			for (p.x = 0; p.x < fieldSize.x; p.x++)
			{
				char c = field[p.y * fieldSize.x + p.x];

				if (c == ' ' || (c == '#' && p.x != 0 && p.x != fieldSize.x - 1))
				{
					fullyFilled = false;
					break;
				}
			}

			if (fullyFilled)
			{
				// Now destruct current line and move
				// other pieces that are above one cell down
				for (int y = p.y; y > 0; y--)
				{
					// Grab data from next line
					std::string data = field.substr((y - 1) * fieldSize.x, fieldSize.x - 1);

					// and put it on the current one
					field.replace(y * fieldSize.x, data.size(), data);
				}

				destructedLines++;
			}
		}

		// Add score based on amount of destroyed lines
		switch (destructedLines)
		{
		case 1: score += 40; break;
		case 2: score += 100; break;
		case 3: score += 300; break;
		case 4: score += 1200; break;
		}

		// Draw field on the screen

		for (p.y = 0; p.y < fieldSize.y; p.y++)
			for (p.x = 0; p.x < fieldSize.x; p.x++)
			{
				def::Pixel col = GetColor(field[p.y * fieldSize.x + p.x]);
				FillRectangle((p + fieldPos) * cellSize, cellSize, col);
			}

		// Draw falling piece
		if (piece != -1)
			DrawPiece(piecePos, piece, rotation, false, false);

		// Draw next piece and it's borders
		DrawPiece(fieldPos + def::vi2d(2, fieldSize.y * 0.4f), nextPiece, 0, true, true, def::DARK_GREY);

		// Draw score
		FillRectangle(4, cellSize.y * 4 + 12, 8 * 9, 10, def::BLACK);
		DrawString(4, cellSize.y * 4 + 12, "Score: " + std::to_string(score), def::WHITE);

		return true;
	}

};

int main()
{
	Tetris app;
	app.Construct(256, 240, 4, 4);
	app.Run();
	return 0;
}
