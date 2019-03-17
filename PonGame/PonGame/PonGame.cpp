/**
*
* @author Remus-Ciprian Ghenceanu
*/
#include "Basic.h"
using namespace std;

GLdouble windowWidth = 1200.0;
GLdouble windowHeight = 800.0;
GLdouble halfWindowWidth = windowWidth / 2.0;
GLdouble halfWindowHeight = windowHeight / 2.0;
GLdouble globalAmplifyFactor = 0.0;

void update(void);

bool colliding(Vector2 min1, Vector2 max1, Vector2 min2, Vector2 max2) {
	return !(max1.x < min2.x || max2.x < min1.x ||
		max1.y < min2.y || max2.y < min1.y);
}
GLdouble hitFactor(Vector2 ballPos, Vector2 racketPos, GLdouble racketHeight) {
	GLdouble halfRacketHeight = racketHeight / 2;
	return (ballPos.y - racketPos.y) / halfRacketHeight;
}
void printString(Vector2 position, char *string) {
	//set the position of the text in the window using the x and y coordinates
	glRasterPos2d(position.x, position.y);
	//get the length of the string to display
	int len = (int)strlen(string);

	//loop to display character by character
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
	}
}
void printNumber(Vector2 position, int nr) {
	int i;
	char str[3];
	if (nr != 0) {
		for (i = 0; nr != 0; i++) {
			str[i] = '0' + (nr % 10);
			nr /= 10;
		}
		str[i] = '\0';
		_strrev(str);
	}
	else {
		str[0] = '0';
		str[1] = '\0';
	}

	printString(position, str);
}

class Table {
private:
	GLdouble outsideMargin;
	GLdouble border;
	GLdouble insideMargin;

	Vector2 min;
	Vector2 max;

	GLdouble middleLineStep;

public:
	Table() {}
	Table(GLdouble _outsideMargin, GLdouble _border, GLdouble _insideMargin) {
		outsideMargin = _outsideMargin;
		border = _border;
		insideMargin = _insideMargin;

		min.x = outsideMargin + border + insideMargin;
		min.y = min.x;
		max.x = windowWidth - min.x;
		max.y = windowHeight - min.x;

		middleLineStep = 0.0;
	}

	GLdouble GetOutsideMargin() {
		return outsideMargin;
	}

	GLdouble GetBorder() {
		return border;
	}

	GLdouble GetInsideMargin() {
		return insideMargin;
	}

	Vector2 GetMin() {
		return min;
	}

	Vector2 GetMax() {
		return max;
	}

	void Draw() {
		glColor3f(1.0, 1.0, 1.0);
		glRectd(outsideMargin, outsideMargin, windowWidth - outsideMargin, windowHeight - outsideMargin);
		glColor3f(0.0, 0.0, 0.0);
		glRectd(outsideMargin + border, outsideMargin + border,
			windowWidth - (outsideMargin + border), windowHeight - (outsideMargin + border));
		glColor3f(1.0, 1.0, 1.0);
		GLdouble halfBorder = border / 2;
		Vector2 startPos(halfWindowWidth, windowHeight - outsideMargin - halfBorder);
		for (int i = 0; i < (int)(windowHeight / (2 * border)); i++) {
			glBegin(GL_POLYGON);
			glVertex2d(startPos.x - halfBorder, startPos.y - halfBorder + middleLineStep);
			glVertex2d(startPos.x + halfBorder, startPos.y - halfBorder + middleLineStep);
			glVertex2d(startPos.x + halfBorder, startPos.y + halfBorder + middleLineStep);
			glVertex2d(startPos.x - halfBorder, startPos.y + halfBorder + middleLineStep);
			glEnd();
			middleLineStep -= 2 * border;
		}
		middleLineStep = 0.0;
		glColor3f(0.0, 0.0, 0.0);
		glRectd(0.0, 0.0, windowWidth, outsideMargin);
	}
};

enum Type { none, left, right };
enum MovementState { stopped, up, down };

class Racket {
private:
	Type type;
	Vector2 position;
	Vector2 movingPosition;
	GLdouble moveFactor;
	MovementState movementState;

	Vector2 min;
	Vector2 max;

	void SetMinMax() {
		min.x = position.x - Racket::halfWidth;
		min.y = position.y - Racket::halfHeight + moveFactor;
		max.x = position.x + Racket::halfWidth;
		max.y = position.y + Racket::halfHeight + moveFactor;
	}
public:
	static GLdouble gateOffset;
	static GLdouble width;
	static GLdouble height;
	static GLdouble halfWidth;
	static GLdouble halfHeight;
	static GLdouble step;

	Racket() {}
	Racket(Type _type, Table * _table) {
		type = _type;
		switch (type) {
		case Type::left:
			position.x = _table->GetOutsideMargin() + _table->GetBorder() + gateOffset;
			break;
		case Type::right:
			position.x = windowWidth - (_table->GetOutsideMargin() + _table->GetBorder() + gateOffset);
			break;
		}
		position.y = halfWindowHeight;
		movingPosition = position;
		moveFactor = 0.0;
		movementState = MovementState::stopped;
		SetMinMax();
	}

	void MoveUp() {
		moveFactor += Racket::step * globalAmplifyFactor;
		movingPosition.y = position.y + moveFactor;
		SetMinMax();
	}

	void MoveDown() {
		moveFactor -= Racket::step * globalAmplifyFactor;
		movingPosition.y = position.y + moveFactor;
		SetMinMax();
	}

	void Reset() {
		position.y = halfWindowHeight;
		movingPosition = position;
		moveFactor = 0.0;
		movementState = MovementState::stopped;

		SetMinMax();
	}

	void SetMovementState(MovementState _movementState) {
		movementState = _movementState;
	}
	MovementState GetMovementState() {
		return movementState;
	}

	Vector2 GetMin() {
		return min;
	}

	Vector2 GetMax() {
		return max;
	}

	static void SetMargin(GLdouble _gateOffset) {
		Racket::gateOffset = _gateOffset;
	}

	static void SetWidth(GLdouble _width) {
		Racket::width = _width;
		Racket::halfWidth = _width / 2;
	}

	static void SetHeight(GLdouble _height) {
		Racket::height = _height;
		Racket::halfHeight = _height / 2;
	}

	static GLdouble GetHeight() {
		return Racket::height;
	}

	static void SetStep(GLdouble _step) {
		Racket::step = _step;
	}

	Vector2 GetMovingPosition() {
		return movingPosition;
	}

	void Draw() {
		glColor3f(1.0, 1.0, 1.0);

		glPushMatrix();
		glTranslated(0.0, moveFactor, 0.0);

		glRectd(position.x - halfWidth, position.y - halfHeight,
			position.x + halfWidth, position.y + halfHeight);

		glPopMatrix();

		/*glPointSize(5.0);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2d(position.x, position.y);
		glEnd();*/
	}
};

enum RunningState { frozen, running };

class Ball {
private:
	GLdouble size;
	GLdouble halfSize;
	Vector2 position;
	Vector2 runningPosition;
	Vector2 velocity;
	GLdouble speed;
	RunningState runningState;
	Vector2 moveFactor;

	int randomStart;
	Vector2 leftPosition;
	Vector2 rightPosition;

	Vector2 min;
	Vector2 max;

	GLdouble transitionRotateFactor;

	void SetMinMax() {
		min.x = position.x - halfSize + moveFactor.x;
		min.y = position.y - halfSize + moveFactor.y;
		max.x = position.x + halfSize + moveFactor.x;
		max.y = position.y + halfSize + moveFactor.y;
	}

	GLdouble RandomSide() {
		srand((int)time(NULL));
		randomStart = rand() % 2;
		if (!randomStart) {
			velocity = Vector2(-1.0, 0.0);
			return leftPosition.x;
		}
		else {
			velocity = Vector2(1.0, 0.0);
			return rightPosition.x;
		}
	}
public:
	Ball() {}
	Ball(GLdouble _size, GLdouble _speed, Table * _table) {
		GLdouble borderSize = _table->GetBorder();
		leftPosition.x = halfWindowWidth - (2 * borderSize + borderSize / 2);
		rightPosition.x = halfWindowWidth + (2 * borderSize + borderSize / 2);
		position.x = RandomSide(); // it has to be here after left/right position calculations and SETS THE VELOCITY, too
		position.y = halfWindowHeight;
		leftPosition.y = position.y;
		rightPosition.y = position.y;
		runningPosition = position;
		size = _size;
		halfSize = _size / 2;
		speed = _speed;
		runningState = RunningState::frozen;
		moveFactor = Vector2(0.0, 0.0);

		transitionRotateFactor = 0.0;

		SetMinMax();
	}

	void Move() {
		moveFactor.x += velocity.x * speed * globalAmplifyFactor;
		moveFactor.y += velocity.y * speed * globalAmplifyFactor;

		runningPosition.x = position.x + moveFactor.x;
		runningPosition.y = position.y + moveFactor.y;

		transitionRotateFactor += velocity.y * speed;

		SetMinMax();
	}

	void Reset(Type _typeWhoScored, RunningState _runningState) {
		switch (_typeWhoScored) {
		case Type::none:
			position.x = RandomSide(); // it sets the velocity, too
			position.y = halfWindowHeight;
			break;
		case Type::left:
			position = leftPosition;
			runningPosition = position;
			velocity = Vector2(-1.0, 0.0);
			break;
		case Type::right:
			position = rightPosition;
			runningPosition = position;
			velocity = Vector2(1.0, 0.0);
			break;
		}


		runningState = _runningState;
		moveFactor = Vector2(0.0, 0.0);

		SetMinMax();
	}

	Vector2 GetMin() {
		return min;
	}

	Vector2 GetMax() {
		return max;
	}

	void SetRunningState(RunningState _runningState) {
		runningState = _runningState;
	}
	RunningState GetRunningState() {
		return runningState;
	}

	void SetVelocity(Vector2 _velocity) {
		velocity = _velocity;
	}
	Vector2 GetVelocity() {
		return velocity;
	}

	void SetSpeed(GLdouble _speed) {
		speed = _speed;
	}

	Vector2 GetRunningPosition() {
		return runningPosition;
	}

	int GetRandomStartValue() {
		return randomStart;
	}

	void Draw() {
		glColor3f(1.0, 1.0, 1.0);

		glPushMatrix();
		glTranslated(moveFactor.x, moveFactor.y, 0.0);

		glRectd(position.x - halfSize, position.y - halfSize,
			position.x + halfSize, position.y + halfSize);

		glPopMatrix();
	}
};

class ScoreBoard {
private:
	Vector2 position;
	GLdouble middleOffset;
	GLdouble halfMiddleOffset;
	int leftPlayerScore;
	int rightPlayerScore;

	int winScore;

	int GetNumberOfDigits(int number) { // 1 or 2
		if (number >= 0 && number <= 9)
			return 1;
		return 2;
	}
public:
	ScoreBoard() {}
	ScoreBoard(int _winScore, GLdouble _middleOffset, Table * _table) {
		position.x = halfWindowWidth;
		position.y = windowHeight - (_table->GetOutsideMargin() + 2 * _table->GetBorder() + _table->GetInsideMargin());
		middleOffset = _middleOffset;
		halfMiddleOffset = _middleOffset / 2;
		leftPlayerScore = 0;
		rightPlayerScore = 0;
		winScore = _winScore;
	}

	void Reset() {
		leftPlayerScore = 0;
		rightPlayerScore = 0;
	}

	void IncreasePlayerScore(Type _type) {
		switch (_type) {
		case Type::left:
			leftPlayerScore++;
			break;
		case Type::right:
			rightPlayerScore++;
			break;
		}
	}
	int GetPoints(Type _type) {
		switch (_type) {
		case Type::left:
			return leftPlayerScore;
			break;
		case Type::right:
			return rightPlayerScore;
			break;
		}
	}

	Type CheckPotentialWinner() {
		if (leftPlayerScore == winScore)
			return Type::left;
		if (rightPlayerScore == winScore)
			return Type::right;
		return Type::none;
	}

	void Draw() {
		printNumber(Vector2(position.x - halfMiddleOffset - 9 * GetNumberOfDigits(leftPlayerScore), position.y), leftPlayerScore);
		printNumber(Vector2(position.x + halfMiddleOffset, position.y), rightPlayerScore);
	}
};

enum GameState { start, playing, paused, won };
enum TransitionState { inactive, active };

class ControlPanel {
private:
	GameState gameState;

	Table * tableControl;
	Racket * leftRacketControl;
	Racket * rightRacketControl;
	Ball * ballControl;
	ScoreBoard * scoreBoardControl;

	//GLdouble overlayInfoPanelScaleFactor; // OIPSF
	Vector2 panelPosition;
	GLdouble panelWidth;
	GLdouble panelHeight;
	char mainText[256];
	char additionalText[256];

	Type winner;

	TransitionState transitionState;
	GLdouble transitionRotateFactor;
	GLdouble transitionScaleFactor;
	GLdouble initialScaleDivision;

	void displayPanelContainer() {
		glColor3f(1.0, 1.0, 1.0);
		glRectd(panelPosition.x - panelWidth / 2, panelPosition.y - panelHeight / 2,
			panelPosition.x + panelWidth / 2, panelPosition.y + panelHeight / 2);
		glColor3f(0.0, 0.0, 0.0);
		glRectd(panelPosition.x - panelWidth / 2 + tableControl->GetOutsideMargin(), panelPosition.y - panelHeight / 2 + tableControl->GetOutsideMargin(),
			panelPosition.x + panelWidth / 2 - tableControl->GetOutsideMargin(), panelPosition.y + panelHeight / 2 - tableControl->GetOutsideMargin());
	}
	void displayPanelText() {
		glColor3f(1.0, 1.0, 1.0);
		printString(Vector2(panelPosition.x - (strlen(mainText) / 2) * 9, panelPosition.y), mainText);
		printString(Vector2(panelPosition.x - (strlen(additionalText) / 2) * 9, panelPosition.y - 20), additionalText);
	}
public:
	ControlPanel() {}
	ControlPanel(Table * _table, Racket * _leftRacket, Racket * _rightRacket, Ball * _ball, ScoreBoard * _scoreBoard, Vector2 _panelPosition, GLdouble _OIPSF) {
		gameState = GameState::start;

		tableControl = _table;
		leftRacketControl = _leftRacket;
		rightRacketControl = _rightRacket;
		ballControl = _ball;
		scoreBoardControl = _scoreBoard;

		panelPosition = _panelPosition;
		panelWidth = windowWidth / _OIPSF;
		panelHeight = windowHeight / _OIPSF;

		winner = Type::none;

		transitionState = TransitionState::inactive;
		transitionRotateFactor = 0.0;
		transitionScaleFactor = 0.0;
		initialScaleDivision = 30.0;
	}

	void StartTransition() {
		transitionState = TransitionState::active;
		transitionRotateFactor = 0.0;
		transitionScaleFactor = 0.0;
	}

	void Update() {
		if (transitionState == TransitionState::active) {
			transitionRotateFactor += 0.1;
			if (transitionScaleFactor < windowHeight)
				transitionScaleFactor += 2.0;
			else
				transitionState = TransitionState::inactive;
		}
	}

	void Pause() {
		gameState = GameState::paused;
		ballControl->SetRunningState(RunningState::frozen);
		globalAmplifyFactor = 0.0;
	}

	void Start() {
		gameState = GameState::playing;
		ballControl->SetRunningState(RunningState::running);
		globalAmplifyFactor = 1.0;
	}

	void Reset() {
		gameState = GameState::start;
		globalAmplifyFactor = 0.0;
		leftRacketControl->Reset();
		rightRacketControl->Reset();
		ballControl->Reset(Type::none, RunningState::frozen);
		scoreBoardControl->Reset();

		StartTransition();
	}

	void SetWinner(Type _winner) {
		gameState = GameState::won;
		winner = _winner;
		globalAmplifyFactor = 0.0;
	}

	void SetGameState(GameState _gameState) {
		gameState = _gameState;
	}
	GameState GetGameState() {
		return gameState;
	}

	TransitionState GetTransitionState() {
		return transitionState;
	}

	void DrawTransition(Vector2 _position) {

		for (int i = 0; i < 9; i++) {
			glPushMatrix();
			glTranslated(_position.x, _position.y, 0.0);
			if (i % 2 == 0) {
				glColor3f(0.0, 0.0, 0.0);
				glRotated(transitionRotateFactor, 0.0, 0.0, 1.0);
				glScaled(transitionScaleFactor, transitionScaleFactor, 1.0);
			}
			else {
				glColor3f(1.0, 1.0, 1.0);
				glRotated(-transitionRotateFactor, 0.0, 0.0, 1.0);
				glScaled(transitionScaleFactor, transitionScaleFactor, 1.0);
			}
			glTranslated(-_position.x / initialScaleDivision, -_position.y / initialScaleDivision, 0.0);
			glRectd((_position.x - halfWindowHeight / 2 + i * tableControl->GetBorder()) / initialScaleDivision, (_position.y - halfWindowHeight / 2 + i * tableControl->GetBorder()) / initialScaleDivision,
				(_position.x + halfWindowHeight / 2 - i * tableControl->GetBorder()) / initialScaleDivision, (_position.y + halfWindowHeight / 2 - i * tableControl->GetBorder()) / initialScaleDivision);
			glPopMatrix();
		}

	}

	void Draw() {
		if (gameState != GameState::playing) {

			switch (gameState) {
			case GameState::start:
				displayPanelContainer();

				strcpy_s(mainText, "PRESS SPACE TO START");
				strcpy_s(additionalText, "");

				displayPanelText();
				break;
			case GameState::paused:
				displayPanelContainer();

				strcpy_s(mainText, "GAME PAUSED");
				strcpy_s(additionalText, "R - RESTART GAME");

				displayPanelText();
				break;
			case GameState::won:
				strcpy_s(mainText, "YOU WON!");
				strcpy_s(additionalText, "R - REMATCH :)");

				glColor3f(1.0, 1.0, 1.0);
				switch (winner) {
				case Type::left:
					printString(Vector2(halfWindowWidth - halfWindowWidth / 2.0 - (strlen(mainText) / 2) * 9, halfWindowHeight), mainText);
					printString(Vector2(halfWindowWidth - halfWindowWidth / 2.0 - (strlen(additionalText) / 2) * 9, halfWindowHeight - 20), additionalText);
					break;
				case Type::right:
					printString(Vector2(halfWindowWidth + halfWindowWidth / 2.0 - (strlen(mainText) / 2) * 9, halfWindowHeight), mainText);
					printString(Vector2(halfWindowWidth + halfWindowWidth / 2.0 - (strlen(additionalText) / 2) * 9, halfWindowHeight - 20), additionalText);
					break;
				}
				break;
			}

		}
	}
};

/*
MUST BE SET
*/
GLdouble Racket::gateOffset = 30.0;
GLdouble Racket::width = 10.0;
GLdouble Racket::height = 10.0;
GLdouble Racket::halfWidth;
GLdouble Racket::halfHeight;
GLdouble Racket::step = 0.5;

Table *table = NULL;
Racket *leftRacket = NULL;
Racket *rightRacket = NULL;
Ball *ball = NULL;
ScoreBoard *scoreBoard = NULL;

ControlPanel *controlPanel = NULL;

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
	glShadeModel(GL_FLAT);

	table = new Table(10.0, 20.0, 10.0);

	Racket::SetMargin(70.0);
	Racket::SetWidth(35.0);
	Racket::SetHeight(70.0);
	Racket::SetStep(0.75);

	leftRacket = new Racket(Type::left, table);
	rightRacket = new Racket(Type::right, table);

	ball = new Ball(10.0, 1.15, table);

	scoreBoard = new ScoreBoard(5, 100, table);

	controlPanel = new ControlPanel(table, leftRacket, rightRacket, ball, scoreBoard, Vector2(halfWindowWidth, windowHeight / 1.5), 5.0);

	glutIdleFunc(update);
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	if (controlPanel->GetTransitionState() == TransitionState::inactive) {
		table->Draw();

		leftRacket->Draw();
		rightRacket->Draw();

		ball->Draw();

		scoreBoard->Draw();

		controlPanel->Draw();
	}
	else {
		controlPanel->DrawTransition(Vector2(halfWindowWidth, halfWindowHeight));
	}


	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, windowWidth, 0.0, windowHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void update(void) {
	controlPanel->Update();

	switch (leftRacket->GetMovementState()) {
	case MovementState::up:
		if (leftRacket->GetMax().y + Racket::step < table->GetMax().y) {
			leftRacket->MoveUp();
		}
		break;
	case MovementState::down:
		if (leftRacket->GetMin().y - Racket::step > table->GetMin().y) {
			leftRacket->MoveDown();
		}
		break;
	case MovementState::stopped:
		break;
	}

	switch (rightRacket->GetMovementState()) {
	case MovementState::up:
		if (rightRacket->GetMax().y + Racket::step < table->GetMax().y) {
			rightRacket->MoveUp();
		}
		break;
	case MovementState::down:
		if (rightRacket->GetMin().y - Racket::step > table->GetMin().y) {
			rightRacket->MoveDown();
		}
		break;
	case MovementState::stopped:
		break;
	}

	if (ball->GetRunningState() == RunningState::running) {
		ball->Move();
		/*
		BALL IS COLLIDING WITH LEFT RACKET
		*/
		if (colliding(leftRacket->GetMin(), leftRacket->GetMax(), ball->GetMin(), ball->GetMax())) {
			Vector2 velocity;
			velocity.x = 1.0;
			velocity.y = hitFactor(ball->GetRunningPosition(), leftRacket->GetMovingPosition(), leftRacket->GetHeight());
			velocity.Normalize();

			ball->SetVelocity(velocity);
		}
		/*
		BALL IS COLLIDING WITH RIGHT RACKET
		*/
		if (colliding(rightRacket->GetMin(), rightRacket->GetMax(), ball->GetMin(), ball->GetMax())) {
			Vector2 velocity;
			velocity.x = -1.0;
			velocity.y = hitFactor(ball->GetRunningPosition(), rightRacket->GetMovingPosition(), rightRacket->GetHeight());
			velocity.Normalize();

			ball->SetVelocity(velocity);
		}
		/*
		COLLIDING WITH TOP BORDER
		*/
		if (colliding(Vector2(table->GetOutsideMargin(), windowHeight - (table->GetOutsideMargin() + table->GetBorder())),
			Vector2(windowWidth - table->GetOutsideMargin(), windowHeight - table->GetOutsideMargin()),
			ball->GetMin(), ball->GetMax())) {

			Vector2 velocity = ball->GetVelocity();
			velocity.y *= -1;

			ball->SetVelocity(velocity);
		}
		/*
		COLLIDING WITH BOTTOM BORDER
		*/
		if (colliding(Vector2(table->GetOutsideMargin(), table->GetOutsideMargin()),
			Vector2(windowWidth - table->GetOutsideMargin(), table->GetOutsideMargin() + table->GetBorder()),
			ball->GetMin(), ball->GetMax())) {

			Vector2 velocity = ball->GetVelocity();
			velocity.y *= -1;

			ball->SetVelocity(velocity);
		}
		/*
		COLLIDING WITH LEFT BORDER
		*/
		if (colliding(Vector2(table->GetOutsideMargin(), table->GetOutsideMargin()),
			Vector2(table->GetOutsideMargin() + table->GetBorder(), windowHeight - table->GetOutsideMargin()),
			ball->GetMin(), ball->GetMax())) {
			scoreBoard->IncreasePlayerScore(Type::right);

			if (scoreBoard->CheckPotentialWinner() == Type::none) {
				ball->Reset(Type::right, RunningState::running);
				leftRacket->Reset();
				rightRacket->Reset();
			}
			else {
				ball->SetRunningState(RunningState::frozen);
				controlPanel->SetWinner(scoreBoard->CheckPotentialWinner());
			}
		}
		/*
		COLLIDING WITH RIGHT BORDER
		*/
		if (colliding(Vector2(windowWidth - (table->GetOutsideMargin() + table->GetBorder()), table->GetOutsideMargin()),
			Vector2(windowWidth - table->GetOutsideMargin(), windowHeight - table->GetOutsideMargin()),
			ball->GetMin(), ball->GetMax())) {
			scoreBoard->IncreasePlayerScore(Type::left);

			if (scoreBoard->CheckPotentialWinner() == Type::none) {
				ball->Reset(Type::left, RunningState::running);
				leftRacket->Reset();
				rightRacket->Reset();
			}
			else {
				ball->SetRunningState(RunningState::frozen);
				controlPanel->SetWinner(scoreBoard->CheckPotentialWinner());
			}
		}
	}
}

void processNormalKeysDown(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
	case 'W':
		if (leftRacket != NULL) {
			leftRacket->SetMovementState(MovementState::up);
		}
		break;
	case 's':
	case 'S':
		if (leftRacket != NULL) {
			leftRacket->SetMovementState(MovementState::down);
		}
		break;
	case 'p':
	case 'P':
		if (rightRacket != NULL) {
			rightRacket->SetMovementState(MovementState::up);
		}
		break;
	case 'l':
	case 'L':
		if (rightRacket != NULL) {
			rightRacket->SetMovementState(MovementState::down);
		}
		break;
	case 'r':
	case 'R':
		if (controlPanel->GetGameState() == GameState::paused || controlPanel->GetGameState() == GameState::won) {
			controlPanel->Reset();
		}
		break;
	case ' ':
		if (controlPanel->GetTransitionState() == TransitionState::inactive) {
			switch (controlPanel->GetGameState()) {
			case GameState::start:
				controlPanel->Start();
				break;
			case GameState::paused:
				controlPanel->Start();
				break;
			case GameState::playing:
				controlPanel->Pause();
				break;
			}
		}

		break;
	}
}

void processNormalKeysUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
	case 'W':
		if (leftRacket != NULL) {
			leftRacket->SetMovementState(MovementState::stopped);
		}
		break;
	case 's':
	case 'S':
		if (leftRacket != NULL) {
			leftRacket->SetMovementState(MovementState::stopped);
		}
		break;
	case 'p':
	case 'P':
		if (rightRacket != NULL) {
			rightRacket->SetMovementState(MovementState::stopped);
		}
		break;
	case 'l':
	case 'L':
		if (rightRacket != NULL) {
			rightRacket->SetMovementState(MovementState::stopped);
		}
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize((int)windowWidth, (int)windowHeight);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("PonGame");
	init();

	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(processNormalKeysDown);
	glutKeyboardUpFunc(processNormalKeysUp);

	glutMainLoop();

	return 0;
}
