import java.util.Arrays;
import java.util.Collections;
import java.util.Random;

String[] phrases; //contains all of the phrases
int totalTrialNum = 2; //the total number of phrases to be tested - set this low for testing. Might be ~10 for the real bakeoff!
int currTrialNum = 0; // the current trial number (indexes into trials array above)
float startTime = 0; // time starts when the first letter is entered
float finishTime = 0; // records the time of when the final trial ends
float lastTime = 0; //the timestamp of when the last trial was completed
float lettersEnteredTotal = 0; //a running total of the number of letters the user has entered (need this for final WPM computation)
float lettersExpectedTotal = 0; //a running total of the number of letters expected (correct phrases)
float errorsTotal = 0; //a running total of the number of errors (when hitting next)
String currentPhrase = ""; //the current target phrase
String currentTyped = ""; //what the user has typed so far
final int DPIofYourDeviceScreen = 120; //you will need to look up the DPI or PPI of your device to make sure you get the right scale. Or play around with this value.
final float sizeOfInputArea = DPIofYourDeviceScreen*1; //aka, 1.0 inches square!
PImage watch;
PImage finger;

//Variables for my silly implementation. You can delete this:
char currentLetter = 'a';

boolean submenuOpen = false; // Indicates if a submenu is open
String[] submenuLetters = {}; // Holds letters for the active submenu
float submenuX = 0, submenuY = 0; // Coordinates for the submenu
int lettersPerRow = 3; // Number of letters per row in the submenu

//You can modify anything in here. This is just a basic implementation.
void setup()
{
  //noCursor();
  watch = loadImage("watchhand3smaller.png");
  //finger = loadImage("pngeggSmaller.png"); //not using this
  phrases = loadStrings("phrases2.txt"); //load the phrase set into memory
  Collections.shuffle(Arrays.asList(phrases), new Random()); //randomize the order of the phrases with no seed
  //Collections.shuffle(Arrays.asList(phrases), new Random(100)); //randomize the order of the phrases with seed 100; same order every time, useful for testing
 
  orientation(LANDSCAPE); //can also be PORTRAIT - sets orientation on android device
  size(800, 800); //Sets the size of the app. You should modify this to your device's native size. Many phones today are 1080 wide by 1920 tall.
  textFont(createFont("Arial", 20)); //set the font to arial 24. Creating fonts is expensive, so make difference sizes once in setup, not draw
  noStroke(); //my code doesn't use any strokes
}

//You can modify anything in here. This is just a basic implementation.
void draw()
{
  background(255); //clear background
  
   //check to see if the user finished. You can't change the score computation.
  if (finishTime!=0)
  {
    fill(0);
    textAlign(CENTER);
    text("Trials complete!",400,200); //output
    text("Total time taken: " + (finishTime - startTime),400,220); //output
    text("Total letters entered: " + lettersEnteredTotal,400,240); //output
    text("Total letters expected: " + lettersExpectedTotal,400,260); //output
    text("Total errors entered: " + errorsTotal,400,280); //output
    float wpm = (lettersEnteredTotal/5.0f)/((finishTime - startTime)/60000f); //FYI - 60K is number of milliseconds in minute
    text("Raw WPM: " + wpm,400,300); //output
    float freebieErrors = lettersExpectedTotal*.05; //no penalty if errors are under 5% of chars
    text("Freebie errors: " + nf(freebieErrors,1,3),400,320); //output
    float penalty = max(errorsTotal-freebieErrors, 0) * .5f;
    text("Penalty: " + penalty,400,340);
    text("WPM w/ penalty: " + (wpm-penalty),400,360); //yes, minus, because higher WPM is better
    return;
  }
  
  drawWatch();
  
    if (startTime == 0 && !mousePressed) {
    fill(128);
    textAlign(CENTER);
    text("Click to start time!", width / 2, height / 2);
    return;
  }

  if (startTime == 0 && mousePressed) {
    nextTrial();
    return;
  }

  if (startTime != 0) {
    // Draw trial information
    textAlign(LEFT);
    fill(128);
    textSize(16);
    text("Phrase " + (currTrialNum + 1) + " of " + totalTrialNum, 40, 40);
    text("Target:   " + currentPhrase, 40, 70);
    text("Entered:  " + currentTyped + "|", 40, 100);

    if (submenuOpen) {
      drawSubmenu();
    } else {
    // Top buttons (Delete, Apostrophe, Space, Submit)
    float barWidth = sizeOfInputArea / 4;
    float barHeight = 40;
    float barY = height / 2 - sizeOfInputArea / 2 - barHeight - 10;

    // Delete button
    fill(200, 50, 50);
    rect(width / 2 - sizeOfInputArea / 2, height / 2 + 20, barWidth, sizeOfInputArea/3);
    fill(255);
    textAlign(CENTER, CENTER);
    text("X", width / 2 - sizeOfInputArea / 2 + barWidth / 2, height / 2 + 40);

    // Apostrophe button
    fill(200, 200, 50);
    rect(width / 2 - sizeOfInputArea / 2 + barWidth, height / 2 + 20, barWidth, sizeOfInputArea/3);
    fill(0);
    text("'", width / 2 - sizeOfInputArea / 2 + barWidth + barWidth / 2, height / 2 + 40);

    // Space button
    fill(50, 200, 200);
    rect(width / 2 - sizeOfInputArea / 2 + 2 * barWidth, height / 2 + 20, barWidth, sizeOfInputArea/3);
    fill(0);
    text("_", width / 2 - sizeOfInputArea / 2 + 2 * barWidth + barWidth / 2, height / 2 + 40);

    // Submit button
    fill(50, 200, 50);
    rect(width / 2 - sizeOfInputArea / 2 + 3 * barWidth, height / 2 + 20, barWidth, sizeOfInputArea/3);
    fill(255);
    text(" ", width / 2 - sizeOfInputArea / 2 + 3 * barWidth + barWidth / 2, height / 2 + 40);

    // Zones inside the input area
    float zoneWidth = sizeOfInputArea / 3;

    fill(0, 150, 255); // Zone 1
    rect(width / 2 - sizeOfInputArea / 2, height / 2 - sizeOfInputArea / 2, zoneWidth, sizeOfInputArea*2/3);

    fill(0, 255, 150); // Zone 2
    rect(width / 2 - zoneWidth / 2, height / 2 - sizeOfInputArea / 2, zoneWidth, sizeOfInputArea*2/3);

    fill(255, 150, 0); // Zone 3
    rect(width / 2 + sizeOfInputArea / 6, height / 2 - sizeOfInputArea / 2, zoneWidth, sizeOfInputArea*2/3);

    // Labels
    fill(255);
    textSize(14);
    textAlign(CENTER, CENTER);
    text("A-I", width / 2 - sizeOfInputArea / 3, height / 2 - 20);
    text("J-R", width / 2, height / 2 - 20);
    text("S-Z", width / 2 + sizeOfInputArea / 3, height / 2 - 20);
    }
  }

}

//my terrible implementation you can entirely replace
boolean didMouseClick(float x, float y, float w, float h) //simple function to do hit testing
{
  return (mouseX > x && mouseX<x+w && mouseY>y && mouseY<y+h); //check to see if it is in button bounds
}


// Open submenu
void openSubmenu(String[] letters) {
  submenuOpen = true;
  submenuLetters = letters;
  submenuX = width / 2 - sizeOfInputArea / 2 ;
  submenuY = height / 2 - sizeOfInputArea / 2 ;
}

// Draw submenu
void drawSubmenu() {
  int rows = ceil((float)submenuLetters.length / lettersPerRow);
  fill(255);
  rect(submenuX, height / 2 - sizeOfInputArea / 2, sizeOfInputArea, rows * 30 + 30);

  fill(0);
  textAlign(CENTER, CENTER);
  for (int i = 0; i < submenuLetters.length; i++) {
    int row = i / lettersPerRow;
    int col = i % lettersPerRow;
    float x = submenuX + sizeOfInputArea / lettersPerRow * col + sizeOfInputArea / (2 * lettersPerRow);
    float y = submenuY + 20 + row * 30;
    text(submenuLetters[i], x, y);
  }

  // Return button
  fill(255, 100, 0);
  rect(submenuX, submenuY + rows * 30, sizeOfInputArea, 30);
  fill(255);
  text("Return", submenuX + sizeOfInputArea / 2, submenuY + rows * 30 + 15);
}

// Mouse press handling
void mousePressed() {
  if (submenuOpen) {
    handleSubmenuClick();
    return;
  }

  // Top button handling
  float barWidth = sizeOfInputArea / 4;
  float barHeight = 40;
  float barY = height / 2 - sizeOfInputArea / 2 - barHeight - 10;

  // Delete button
  if (mouseX > width / 2 - sizeOfInputArea / 2 && mouseX < width / 2 - sizeOfInputArea / 2 + barWidth &&
      mouseY > height / 2 + sizeOfInputArea / 2 - 40 && mouseY < height / 2 + sizeOfInputArea / 2) {
    if (currentTyped.length() > 0) {
      currentTyped = currentTyped.substring(0, currentTyped.length() - 1);
    }
    return;
  }

  // Apostrophe button
  if (mouseX > width / 2 - sizeOfInputArea / 2 + barWidth && mouseX < width / 2 - sizeOfInputArea / 2 + 2 * barWidth &&
      mouseY > height / 2 + sizeOfInputArea / 2 - 40 && mouseY < height / 2 + sizeOfInputArea / 2) {
    currentTyped += "'";
    return;
  }

  // Space button
  if (mouseX > width / 2 - sizeOfInputArea / 2 + 2 * barWidth && mouseX < width / 2 - sizeOfInputArea / 2 + 3 * barWidth &&
      mouseY > height / 2 + sizeOfInputArea / 2 - 40 && mouseY < height / 2 + sizeOfInputArea / 2) {
    currentTyped += " ";
    return;
  }

  // Submit button
  if (mouseX > width / 2 - sizeOfInputArea / 2 + 3 * barWidth && mouseX < width / 2 - sizeOfInputArea / 2 + 4 * barWidth &&
      mouseY > height / 2 + sizeOfInputArea / 2 - 40 && mouseY < height / 2 + sizeOfInputArea / 2 ) {
    println("Submitted: " + currentTyped);
    nextTrial();
    return;
  }

  // Input area handling for zones
  if (mouseX > width / 2 - sizeOfInputArea / 2 && mouseX < width / 2 + sizeOfInputArea / 2 &&
      mouseY > height / 2 - sizeOfInputArea / 2 && mouseY < height / 2 + sizeOfInputArea / 2 - 40) {
    if (mouseX < width / 2 - sizeOfInputArea / 6) { // Zone 1
      openSubmenu(new String[]{"a", "b", "c", "d", "e", "f", "g", "h", "i"});
    } else if (mouseX < width / 2 + sizeOfInputArea / 6) { // Zone 2
      openSubmenu(new String[]{"j", "k", "l", "m", "n", "o", "p", "q", "r"});
    } else { // Zone 3
      openSubmenu(new String[]{"s", "t", "u", "v", "w", "x", "y", "z"});
    }
  }
}

// Handle submenu clicks
void handleSubmenuClick() {
  int rows = ceil((float)submenuLetters.length / lettersPerRow);
  if (mouseY > submenuY + rows * 30 + 20 && mouseY < submenuY + rows * 30 + 30) {
    submenuOpen = false; // Return to main zones
    return;
  }

  for (int i = 0; i < submenuLetters.length; i++) {
    int row = i / lettersPerRow;
    int col = i % lettersPerRow;
    float x = submenuX + sizeOfInputArea / lettersPerRow * col + sizeOfInputArea / (2 * lettersPerRow);
    float y = submenuY + 20 + row * 30;

    if (mouseX > x - 10 && mouseX < x + 10 && mouseY > y - 10 && mouseY < y + 5 ) {
      currentTyped += submenuLetters[i];
      submenuOpen = false;
      return;
    }
  }
}

void nextTrial()
{
  if (currTrialNum >= totalTrialNum) //check to see if experiment is done
    return; //if so, just return

  if (startTime!=0 && finishTime==0) //in the middle of trials
  {
    System.out.println("==================");
    System.out.println("Phrase " + (currTrialNum+1) + " of " + totalTrialNum); //output
    System.out.println("Target phrase: " + currentPhrase); //output
    System.out.println("Phrase length: " + currentPhrase.length()); //output
    System.out.println("User typed: " + currentTyped); //output
    System.out.println("User typed length: " + currentTyped.length()); //output
    System.out.println("Number of errors: " + computeLevenshteinDistance(currentTyped.trim(), currentPhrase.trim())); //trim whitespace and compute errors
    System.out.println("Time taken on this trial: " + (millis()-lastTime)); //output
    System.out.println("Time taken since beginning: " + (millis()-startTime)); //output
    System.out.println("==================");
    lettersExpectedTotal+=currentPhrase.trim().length();
    lettersEnteredTotal+=currentTyped.trim().length();
    errorsTotal+=computeLevenshteinDistance(currentTyped.trim(), currentPhrase.trim());
  }

  //probably shouldn't need to modify any of this output / penalty code.
  if (currTrialNum == totalTrialNum-1) //check to see if experiment just finished
  {
    finishTime = millis();
    System.out.println("==================");
    System.out.println("Trials complete!"); //output
    System.out.println("Total time taken: " + (finishTime - startTime)); //output
    System.out.println("Total letters entered: " + lettersEnteredTotal); //output
    System.out.println("Total letters expected: " + lettersExpectedTotal); //output
    System.out.println("Total errors entered: " + errorsTotal); //output

    float wpm = (lettersEnteredTotal/5.0f)/((finishTime - startTime)/60000f); //FYI - 60K is number of milliseconds in minute
    float freebieErrors = lettersExpectedTotal*.05; //no penalty if errors are under 5% of chars
    float penalty = max(errorsTotal-freebieErrors, 0) * .5f;
    
    System.out.println("Raw WPM: " + wpm); //output
    System.out.println("Freebie errors: " + freebieErrors); //output
    System.out.println("Penalty: " + penalty);
    System.out.println("WPM w/ penalty: " + (wpm-penalty)); //yes, minus, becuase higher WPM is better
    System.out.println("==================");

    currTrialNum++; //increment by one so this mesage only appears once when all trials are done
    return;
  }

  if (startTime==0) //first trial starting now
  {
    System.out.println("Trials beginning! Starting timer..."); //output we're done
    startTime = millis(); //start the timer!
  } 
  else
    currTrialNum++; //increment trial number

  lastTime = millis(); //record the time of when this trial ended
  currentTyped = ""; //clear what is currently typed preparing for next trial
  currentPhrase = phrases[currTrialNum]; // load the next phrase!
  //currentPhrase = "abc"; // uncomment this to override the test phrase (useful for debugging)
}

//probably shouldn't touch this - should be same for all teams.
void drawWatch()
{
  float watchscale = DPIofYourDeviceScreen/138.0; //normalizes the image size
  pushMatrix();
  translate(width/2, height/2);
  scale(watchscale);
  imageMode(CENTER);
  image(watch, 0, 0);
  popMatrix();
}

//probably shouldn't touch this - should be same for all teams.
void drawFinger()
{
  float fingerscale = DPIofYourDeviceScreen/150f; //normalizes the image size
  pushMatrix();
  translate(mouseX, mouseY);
  scale(fingerscale);
  imageMode(CENTER);
  image(finger,52,341);
  if (mousePressed)
     fill(0);
  else
     fill(255);
  ellipse(0,0,5,5);

  popMatrix();
  }
  

//=========SHOULD NOT NEED TO TOUCH THIS METHOD AT ALL!==============
int computeLevenshteinDistance(String phrase1, String phrase2) //this computers error between two strings
{
  int[][] distance = new int[phrase1.length() + 1][phrase2.length() + 1];

  for (int i = 0; i <= phrase1.length(); i++)
    distance[i][0] = i;
  for (int j = 1; j <= phrase2.length(); j++)
    distance[0][j] = j;

  for (int i = 1; i <= phrase1.length(); i++)
    for (int j = 1; j <= phrase2.length(); j++)
      distance[i][j] = min(min(distance[i - 1][j] + 1, distance[i][j - 1] + 1), distance[i - 1][j - 1] + ((phrase1.charAt(i - 1) == phrase2.charAt(j - 1)) ? 0 : 1));

  return distance[phrase1.length()][phrase2.length()];
}
