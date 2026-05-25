package cs3540;


import java.awt.AWTException;
import java.awt.Rectangle;
import java.awt.Robot;
import java.util.ArrayList;
import java.util.Collections;
import processing.core.PApplet;


public class BakeOff1 extends PApplet {
	// when in doubt, consult the Processsing reference:
	// https://processing.org/reference/
	// The argument passed to main must match the class name
	public static void main(String[] args) {
		// Tell processing what class we want to run.
		PApplet.main("cs3540.BakeOff1");
	}

	int margin = 200; // set the margin around the squares
	final int padding = 50; // padding between buttons and also their width/height
	final int buttonSize = 40; // padding between buttons and also their width/height
	ArrayList<Integer> trials = new ArrayList<Integer>(); // contains the order of buttons that activate in the test
	int trialNum = 0; // the current trial number (indexes into trials array above)
	int startTime = 0; // time starts when the first click is captured
	int finishTime = 0; // records the time of the final click
	int hits = 0; // number of successful clicks
	int misses = 0; // number of missed clicks
	Robot robot; // initialized in setup

	int clicks = 0;
	boolean canClick = true; // To disable clicks after 8
	boolean randomColorsActive = false; // Flag for random colors after 8 clicks
	boolean gridFilled = false; // Flag to check if grid is filled
    int fillDuration = 3000; // Duration to fill the grid with cyan
    int fillStartTime; // Time when the fill starts
    
    // Add an array to keep track of button states
    int[] buttonStates = new int[16]; // 0 = neutral, 1 = hit, 2 = miss
    
	int numRepeats = 3; // sets the number of times each button repeats in the test

	/**
	 * https://processing.org/reference/settings_.html#:~:text=The%20settings()%20method%20runs,commands%20in%20the%20Processing%20API.
	 */
	public void settings() {
		size(700, 700);
	}

	/**
	 * // https://processing.org/reference/setup_.html
	 */
	public void setup() {
		// noCursor(); // hides the system cursor if you want
		noStroke(); // turn off all strokes, we're just using fills here (can change this if you
					// want)
		textFont(createFont("Arial", 20)); // sets the font to Arial size 16
		textAlign(CENTER);
		frameRate(60); // normally you can't go much higher than 60 FPS.
		ellipseMode(CENTER); // ellipses are drawn from the center (BUT RECTANGLES ARE NOT!)
		// rectMode(CENTER); //enabling will break the scaffold code, but you might find
		// it easier to work with centered rects

		try {
			robot = new Robot(); // create a "Java Robot" class that can move the system cursor
		} catch (AWTException e) {
			e.printStackTrace();
		}

		// ===DON'T MODIFY MY RANDOM ORDERING CODE==
		for (int i = 0; i < 16; i++) // generate list of targets and randomize the order
			// number of buttons in 4x4 grid
			for (int k = 0; k < numRepeats; k++)
				// number of times each button repeats
				trials.add(i);

		Collections.shuffle(trials); // randomize the order of the buttons
		System.out.println("trial order: " + trials); // print out order for reference

		surface.setLocation(0, 0);// put window in top left corner of screen (doesn't always work)
		
	}
	

	public void draw() {
		background(0); // Set background to black
	    
		if (trialNum >= trials.size()) // check to see if test is over
		{
			float timeTaken = (finishTime - startTime) / 1000f;
			float penalty = constrain(((95f - ((float) hits * 100f / (float) (hits + misses))) * .2f), 0, 100);
			fill(255); // set fill color to white
			// write to screen (not console)
			text("Finished!", width / 2, height / 2);
			text("Hits: " + hits, width / 2, height / 2 + 20);
			text("Misses: " + misses, width / 2, height / 2 + 40);
			text("Accuracy: " + (float) hits * 100f / (float) (hits + misses) + "%", width / 2, height / 2 + 60);
			text("Total time taken: " + timeTaken + " sec", width / 2, height / 2 + 80);
			text("Average time for each button: " + nf((timeTaken) / (float) (hits + misses), 0, 3) + " sec", width / 2,
					height / 2 + 100);
			text("Average time for each button + penalty: "
					+ nf(((timeTaken) / (float) (hits + misses) + penalty), 0, 3) + " sec", width / 2,
					height / 2 + 140);
			return; // return, nothing else to do now test is over
		}
		
		// Draw a colored square underneath the next target
	    if (trialNum + 1 < trials.size()) {
	        // Get the index of the next target
	        int nextTargetIndex = trials.get(trialNum + 1);
	        Rectangle nextTargetBounds = getButtonLocation(nextTargetIndex);
	        
	        float centerX = nextTargetBounds.x + nextTargetBounds.width / 2;
	        float centerY = nextTargetBounds.y + nextTargetBounds.height / 2;

	        // Draw the colored square underneath the next target
	        fill(0, 15, 255); 
	        // Adjust y-coordinate to position it directly below the target square
	        rect(centerX - (buttonSize / 2) - 4, centerY - (buttonSize / 2) - 4, buttonSize + 8, buttonSize + 8);
	    }

	    // Draw regular buttons if the grid is not filled
	    if (randomColorsActive) {
	        for (int i = 0; i < 16; i++) {
	            randomizeSquares(i); // Randomize square colors during this phase
	        }
	    } else {
	        // If randomization is not active, draw the regular buttons
	        for (int i = 0; i < 16; i++) {
	            drawButton(i);
	        }
	    }

        // If the grid was filled, check the time elapsed to clear it
        if (gridFilled) {
            if (millis() - fillStartTime < fillDuration) {
            	canClick = false;
                // Continue showing the filled grid for the specified duration
                for (int i = 0; i < 16; i++) {
                    Rectangle bounds = getButtonLocation(i);
                    fill(0, 255, 255); // Fill with cyan
                    rect(bounds.x, bounds.y, bounds.width, bounds.height);
                }
            } else {
                // After 3 seconds, reset the grid and allow further clicks
                gridFilled = false; // Reset the flag
                clicks = 0; // Reset click count for the next phase
                canClick = true; // Re-enable clicks
            }
        } else {
            // Draw regular buttons if the grid is not filled
    		if (randomColorsActive) { // Randomize squares for the next 8 clicks if randomColorsActive is true
    			for (int i = 0; i < 16; i++) {
    				randomizeSquares(i); // Randomize square colors during this phase
    			}
    		} else {
    			// If randomization is not active, draw the regular buttons
    			for (int i = 0; i < 16; i++) {
    				drawButton(i);
    			}
    		}
        }
        
        if (!canClick) {
            fill(255); // Set fill color to white for instructions
            text("Please wait...", width / 2, 600); // Show wait message
        } else {
            fill(255); // Set fill color to white for instructions
            text("Click the cyan button!", width / 2, 600); // Show click instruction
        }

		
		fill(255); // set fill color to white
		text((trialNum + 1) + " of " + trials.size(), 40, 20); // display what trial the user is on


		fill(255, 0, 0, 200); // set fill color to translucent red
		noStroke();
		ellipse(mouseX, mouseY, 30, 30); // draw user cursor as a circle with a diameter of 20
		

	}

	public void mousePressed() // test to see if hit was in target!
	{
		if (canClick && trialNum < trials.size()) {
			if (trialNum >= trials.size()) // check if task is done
				return;

			if (trialNum == 0) // check if first click, if so, record start time
				startTime = millis();

			if (trialNum == trials.size() - 1) // check if final click
			{
				finishTime = millis();
				// write to terminal some output:
				System.out.println("we're all done!");
			}

			Rectangle bounds = getButtonLocation(trials.get(trialNum));
			Rectangle cursorBounds = new Rectangle(mouseX - 15, mouseY - 15, 30, 30);

			// check to see if cursor was inside button
			if (cursorBounds.intersects(bounds)) // test to see if hit was within bounds
			{
				System.out.println("HIT! " + trialNum + " " + (millis() - startTime)); // success
				hits++;
				buttonStates[trials.get(trialNum)] = 1; // Set the state to hit
			} else {
				System.out.println("MISSED! " + trialNum + " " + (millis() - startTime)); // fail
				misses++;
				buttonStates[trials.get(trialNum)] = 2; // Set the state to miss
			}

			trialNum++; // Increment trial number

			clicks++;
		}
		// in this example design, I move the cursor back to the middle after each click
		// Note. When running from eclipse the robot class affects the whole screen not
		// just the GUI, so the mouse may move outside of the GUI.
		// robot.mouseMove(width/2, (height)/2); //on click, move cursor to roughly
		// center of window!
	}

	// probably shouldn't have to edit this method
	public Rectangle getButtonLocation(int i) // for a given button ID, what is its location and size
	{
		int x = (i % 4) * (padding + buttonSize) + margin;
		int y = (i / 4) * (padding + buttonSize) + margin;

		return new Rectangle(x, y, buttonSize, buttonSize);
	}

	// you can edit this method to change how buttons appear
	public void drawButton(int i) {
	    Rectangle bounds = getButtonLocation(i);
	    boolean isCurrentTarget = trials.get(trialNum) == i; // Check if this button is the current target
	    boolean isNextTarget = (trialNum + 1 < trials.size()) && (trials.get(trialNum + 1) == i); // Check if it's the next target

	    // If this button is the current target
	    if (isCurrentTarget) {
	        fill(0, 255, 255); // Fill cyan for the current target
	    } 

	    // For all other buttons
	    else {
	        fill(200); // Fill gray for neutral buttons
	        noStroke(); // No outline for neutral buttons
	    }

	    rect(bounds.x, bounds.y, bounds.width, bounds.height); // Draw the button
	}



	// Randomize squares for the next 8 clicks
	public void randomizeSquares(int i) {
		Rectangle bounds = getButtonLocation(i);

		rect(bounds.x, bounds.y, bounds.width, bounds.height);
	}

	public void mouseMoved() {
		// can do stuff everytime the mouse is moved (i.e., not clicked)
		// https://processing.org/reference/mouseMoved_.html
	}

	public void mouseDragged() {
		// can do stuff everytime the mouse is dragged
		// https://processing.org/reference/mouseDragged_.html
	}

	public void keyPressed() {
		// can use the keyboard if you wish
		// https://processing.org/reference/keyTyped_.html
		// https://processing.org/reference/keyCode.html
	}
}
