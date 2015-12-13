// Arduino LCD library
#include <TFT.h>  
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// game variables
#define PADDLE_WIDTH  30
#define PADDLE_HEIGHT 5
#define BALL_SIZE 5
#define DEBUG_PORT 7
#define PADDLE_STEP 2
#define BALL_SPEED 5
#define DELAY_MS 5

TFT TFTscreen = TFT(cs, dc, rst);

// variables for the position of the ball and paddle
int paddleX = 0;
int paddleY = 0;
int newPaddleX = 0;
int newPaddleY = 0;
int oldPaddleX, oldPaddleY;
int ballDirectionX = 1;
int ballDirectionY = 1;


int ballX, ballY, oldBallX, oldBallY;  

// save the width and height of the screen
int tftWidth = TFTscreen.width();
int tftHeight = TFTscreen.height();

void setup() {
  // initialize the display
  TFTscreen.begin();
  Serial.begin(9600);
  // black background
  TFTscreen.background(0, 0, 0);

  // Initial paddle's location to the center of the screen
  paddleX = (tftWidth/2) - PADDLE_WIDTH/2;
  paddleY = (tftHeight/2) - PADDLE_HEIGHT/2;
}

void loop() {

  // #update the current position
  newPaddleX = paddleX + map(analogRead(A0), 0, 1023, -PADDLE_STEP, PADDLE_STEP);
  if((newPaddleX >= - PADDLE_WIDTH/2) && (newPaddleX <= tftWidth - PADDLE_WIDTH/2)){
    paddleX = newPaddleX;
  }
  newPaddleY = paddleY + map(analogRead(A1), 0, 1023, -PADDLE_STEP, PADDLE_STEP);
  if((newPaddleY >= - PADDLE_HEIGHT/2) && (newPaddleY <= tftHeight - PADDLE_HEIGHT/2)){
    paddleY = newPaddleY;
  }

  if (digitalRead(DEBUG_PORT) == LOW){
    Serial.println(paddleX);
    Serial.println(paddleY);
  }
    

  // set the fill color to black and erase the previous
  // position of the paddle if different from present
  TFTscreen.fill(0, 0, 0);

  if (oldPaddleX != paddleX || oldPaddleY != paddleY) {
    TFTscreen.rect(oldPaddleX, oldPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT);
  }

  // draw the paddle on screen, save the current position
  // as the previous.
  TFTscreen.fill(255, 255, 255);

  TFTscreen.rect(paddleX, paddleY, PADDLE_WIDTH, PADDLE_HEIGHT);
  oldPaddleX = paddleX;
  oldPaddleY = paddleY;

  // update the ball's position and draw it on screen
  if (millis() % BALL_SPEED < 2) {
    moveBall();
  }

  delay(DELAY_MS);
}

// this function determines the ball's position on screen
void moveBall() {
  // if the ball goes offscreen, reverse the direction:
  if (ballX > TFTscreen.width() || ballX < 0) {
    ballDirectionX = -ballDirectionX;
  }

  if (ballY > TFTscreen.height() || ballY < 0) {
    ballDirectionY = -ballDirectionY;
  }

  // check if the ball and the paddle occupy the same space on screen
  if (inPaddle(ballX, ballY, paddleX, paddleY, PADDLE_WIDTH, PADDLE_HEIGHT)) {
    ballDirectionX = -ballDirectionX;
    ballDirectionY = -ballDirectionY;
  }

  // update the ball's position
  ballX += ballDirectionX;
  ballY += ballDirectionY;

  // erase the ball's previous position
  TFTscreen.fill(0, 0, 0);

  if (oldBallX != ballX || oldBallY != ballY) {
    TFTscreen.rect(oldBallX, oldBallY, BALL_SIZE, BALL_SIZE);
  }


  // draw the ball's current position
  TFTscreen.fill(255, 255, 255);
  TFTscreen.rect(ballX, ballY, BALL_SIZE, BALL_SIZE);

  oldBallX = ballX;
  oldBallY = ballY;

}

// this function checks the position of the ball
// to see if it intersects with the paddle
boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {
  boolean result = false;

  if ((x >= (rectX - BALL_SIZE) && x <= (rectX + rectWidth)) &&
      (y >= (rectY - BALL_SIZE) && y <= (rectY + rectHeight))) {
    result = true;
  }

  return result;
}
