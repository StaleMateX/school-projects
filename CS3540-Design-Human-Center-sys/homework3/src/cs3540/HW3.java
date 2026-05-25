package cs3540;

import java.util.HashMap;

import processing.core.PApplet;
import processing.core.PShape;

/**
 * Your work in this class do not change the name of the class
 * idea from https://processing.org/examples/getchild.html
 * base on democrat vs republican at 2020 user be able to click state to check who has the popular vote in that state
 * @author Jacob Xu
 */

public class HW3 extends PApplet {
	PShape usa; // PShape object to hold the USA map
	HashMap<String, Boolean> stateParty = new HashMap<>(); // Map to store party affiliation (true = Democrat, false = Republican)
    HashMap<String, Integer> stateColors = new HashMap<>(); // Map to store the initial color of each state
													
	String[] stateAbbreviations = { 
			"AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "FL", "GA", 
			"HI", "ID", "IL", "IN", "IA", "KS", "KY", "LA", "ME", "MD", 
			"MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ", 
			"NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC", 
			"SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV", "WI", "WY" };
	
	// Do not change
	public static void main(String[] args) {
		// Tell processing what class we want to run.
		PApplet.main("cs3540.HW3");
	}

	// method for setting the size of the window
	public void settings() {
		size(1000, 600);
	}

	public void setup() {
		usa = loadShape("Blank_US_Map_(states_only).svg");

		if (usa != null) {
			initializeStates(); // Initialize state data
		} else {
			System.out.println("Error: USA map SVG could not be loaded.");
		}
	}

	public void draw() {
		background(255); // Clear the background

        // Draw the title
        drawTitle("democrat vs republican(2020) click to check result!");
        
        // Draw each state with its respective color
        for (String stateAbbr : stateAbbreviations) {
            PShape stateShape = usa.getChild(stateAbbr);
            if (stateShape != null) {
                int stateColor = stateColors.get(stateAbbr); // Get the current color of the state
                drawShape(stateShape, stateColor);
            }
        }
	}

	void drawShape(PShape shape, int stateColor) {
        if (shape != null) {
            shape.disableStyle(); // Disable original style
            fill(stateColor); // Set fill color
            noStroke();
            shape(shape); // Draw the shape
        }
	}

	public void mousePressed() {
        for (String stateAbbr : stateAbbreviations) {
            PShape stateShape = usa.getChild(stateAbbr);
            if (stateShape != null && isMouseInside(stateShape)) {
                // Toggle the color based on the state affiliation
                boolean isDemocrat = stateParty.get(stateAbbr);
                int newColor = isDemocrat ? color(0, 0, 255) : color(255, 0, 0); // Blue for Democrat, Red for Republican
                stateColors.put(stateAbbr, newColor);
            }
        }
	}

	boolean isMouseInside(PShape shape) {
		return shape != null && shape.contains(mouseX, mouseY); // Check if mouse is inside the shape
	}

	void initializeStates() {
        // Sample initial colors for demonstration purposes
        for (String stateAbbr : stateAbbreviations) {
            stateColors.put(stateAbbr, color(random(255), random(255), random(255))); // Initialize with a random color
        }
        
        // Sample party affiliation for demonstration purposes
        String[] democratStates = {
        		"AL", "AZ", "CA", "CO", "CT", "DE", "FL", "GA", "ID", "ME", 
        		"MD", "MA", "MI", "MN", "NV", "NH", "NJ", "NM", "NY", "OR", 
        		"PA", "RI", "VT", "VA", "WA", "WI"};

        // Initialize party affiliation
        for (String stateAbbr : stateAbbreviations) {
            stateParty.put(stateAbbr, java.util.Arrays.asList(democratStates).contains(stateAbbr));
        }
	}
	
    void drawTitle(String title) {
        textSize(32); // Set the font size
        fill(0); // Set the text color to black
        textAlign(CENTER, TOP); // Align text to the center horizontally and top vertically
        text(title, width / 2, 10); // Draw the title at the top center of the canvas
    }
}