// This is a simple voting system program for a first-year C++ student
// It reads candidate information from a CSV file and lets users vote

#include <iostream>     // For input/output operations
#include <fstream>      // For file operations
#include <string>       // For using strings
#include <vector>       // For using vectors (dynamic arrays)

using namespace std;    // Using standard namespace to avoid writing std:: everywhere

// Main program starts here
int main() {
    // Declare variables we'll need throughout the program
    string username;    // To store the username for login
    string password;    // To store the password for login
    int choice;         // To store menu choice
    bool running = true;    // Flag to keep our program running until user wants to exit
    
    // Arrays to store our data (simple approach for beginners)
    const int MAX_CANDIDATES = 20;    // Maximum number of candidates we can handle
    const int MAX_USERS = 50;         // Maximum number of users we can handle
    
    // Arrays for candidate information
    int candidateIds[MAX_CANDIDATES];         // Array to store candidate IDs
    string candidateNames[MAX_CANDIDATES];    // Array to store candidate names
    string candidateParties[MAX_CANDIDATES];  // Array to store candidate parties
    int candidateVotes[MAX_CANDIDATES];       // Array to store votes for each candidate
    int candidateCount = 0;                   // Counter for how many candidates we have
    
    // Arrays for user information
    string usernames[MAX_USERS];      // Array to store usernames
    string passwords[MAX_USERS];      // Array to store passwords
    bool hasVoted[MAX_USERS];         // Array to track if a user has voted
    int userCount = 0;                // Counter for how many users we have
    
    // Load candidate information from CSV file
    ifstream candidateFile("candidates.csv"); // Open the candidates.csv file
    
    // Check if the file opened successfully
    if (!candidateFile.is_open()) {
        // If the file didn't open, show an error and exit
        cout << "Error: Cannot open candidates.csv file!" << endl;
        cout << "Make sure the file exists in the same folder as this program." << endl;
        return 1;   // Return with error code
    }
    
    // Skip the header line in the CSV
    string line;
    getline(candidateFile, line);  // Read the first line (headers) and ignore it
    
    // Read candidate data line by line
    while (getline(candidateFile, line) && candidateCount < MAX_CANDIDATES) {
        // Variables to track our position in parsing the line
        int pos1 = line.find(',');                       // Find the first comma
        int pos2 = line.find(',', pos1 + 1);             // Find the second comma
        
        // Parse the CSV line into our arrays
        candidateIds[candidateCount] = stoi(line.substr(0, pos1));                     // Get ID
        candidateNames[candidateCount] = line.substr(pos1 + 1, pos2 - pos1 - 1);       // Get name
        candidateParties[candidateCount] = line.substr(pos2 + 1);                      // Get party
        candidateVotes[candidateCount] = 0;                                            // Initialize votes to 0
        
        // Increment our candidate counter
        candidateCount++;
    }
    
    // Close the candidate file after reading
    candidateFile.close();
    
    // Load existing user information from users.txt if it exists
    ifstream userFile("users.txt");
    
    // Check if the user file exists and is open
    if (userFile.is_open()) {
        // Read user data line by line
        while (getline(userFile, line) && userCount < MAX_USERS) {
            // Variables to track our position in parsing the line
            int pos1 = line.find(',');                   // Find the first comma
            int pos2 = line.find(',', pos1 + 1);         // Find the second comma
            
            // Parse the line into our arrays
            usernames[userCount] = line.substr(0, pos1);                         // Get username
            passwords[userCount] = line.substr(pos1 + 1, pos2 - pos1 - 1);       // Get password
            hasVoted[userCount] = (line.substr(pos2 + 1) == "1");                // Get voting status
            
            // Increment our user counter
            userCount++;
        }
        
        // Close the user file after reading
        userFile.close();
    }
    
    // Load existing votes from votes.txt if it exists
    ifstream voteFile("votes.txt");
    
    // Check if the vote file exists and is open
    if (voteFile.is_open()) {
        // Read votes line by line
        while (getline(voteFile, line)) {
            // Variables to track our position in parsing the line
            int pos = line.find(',');                    // Find the comma
            
            // Parse the line to get candidate ID and their votes
            int id = stoi(line.substr(0, pos));          // Get candidate ID
            int votes = stoi(line.substr(pos + 1));      // Get vote count
            
            // Find which candidate this ID belongs to and update their votes
            for (int i = 0; i < candidateCount; i++) {
                if (candidateIds[i] == id) {
                    candidateVotes[i] = votes;           // Update vote count
                    break;                               // Exit the loop once found
                }
            }
        }
        
        // Close the vote file after reading
        voteFile.close();
    }
    
    // Main program loop
    while (running) {
        // Display menu
        cout << "\n========= STUDENT VOTING SYSTEM =========\n";
        cout << "1. Register as a new user\n";
        cout << "2. Login\n";
        cout << "3. View candidates\n";
        cout << "4. View results\n";
        cout << "5. Exit\n";
        cout << "Enter your choice (1-5): ";
        
        // Get user's menu choice
        cin >> choice;
        
        // Process user's choice with a switch statement
        switch (choice) {
            // Case 1: Register a new user
            case 1: {
                // Check if we have room for more users
                if (userCount >= MAX_USERS) {
                    cout << "Error: Maximum number of users reached.\n";
                    break;
                }
                
                // Get registration information
                cout << "\n----- REGISTER -----\n";
                cout << "Enter username: ";
                cin >> username;
                
                // Check if username already exists
                bool usernameExists = false;
                for (int i = 0; i < userCount; i++) {
                    if (usernames[i] == username) {
                        usernameExists = true;
                        break;
                    }
                }
                
                // If username exists, tell the user and go back to menu
                if (usernameExists) {
                    cout << "Username already exists. Please choose another.\n";
                    break;
                }
                
                // Get password
                cout << "Enter password: ";
                cin >> password;
                
                // Store new user information
                usernames[userCount] = username;
                passwords[userCount] = password;
                hasVoted[userCount] = false;
                userCount++;
                
                // Save updated user information to file
                ofstream saveUserFile("users.txt");
                for (int i = 0; i < userCount; i++) {
                    saveUserFile << usernames[i] << "," << passwords[i] << "," 
                                << (hasVoted[i] ? "1" : "0") << endl;
                }
                saveUserFile.close();
                
                cout << "Registration successful!\n";
                break;
            }
            
            // Case 2: Login
            case 2: {
                // Get login information
                cout << "\n----- LOGIN -----\n";
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                
                // Check if credentials are valid
                int currentUser = -1;
                for (int i = 0; i < userCount; i++) {
                    if (usernames[i] == username && passwords[i] == password) {
                        currentUser = i;
                        break;
                    }
                }
                
                // If credentials invalid, tell the user and go back to menu
                if (currentUser == -1) {
                    cout << "Invalid username or password. Try again.\n";
                    break;
                }
                
                cout << "Login successful!\n";
                
                // Check if user has already voted
                if (hasVoted[currentUser]) {
                    cout << "You have already voted in this election.\n";
                    break;
                }
                
                // Show candidates
                cout << "\n----- CANDIDATES -----\n";
                for (int i = 0; i < candidateCount; i++) {
                    cout << candidateIds[i] << ". " << candidateNames[i] 
                         << " (" << candidateParties[i] << ")\n";
                }
                
                // Get voting choice
                int voteChoice;
                cout << "\nEnter the ID of the candidate you want to vote for: ";
                cin >> voteChoice;
                
                // Find which candidate to vote for
                bool validChoice = false;
                for (int i = 0; i < candidateCount; i++) {
                    if (candidateIds[i] == voteChoice) {
                        // Increment vote count
                        candidateVotes[i]++;
                        validChoice = true;
                        break;
                    }
                }
                
                // If choice was invalid, tell the user
                if (!validChoice) {
                    cout << "Invalid candidate ID. Vote not counted.\n";
                    break;
                }
                
                // Mark user as having voted
                hasVoted[currentUser] = true;
                
                // Save updated user information
                ofstream saveUserFile("users.txt");
                for (int i = 0; i < userCount; i++) {
                    saveUserFile << usernames[i] << "," << passwords[i] << "," 
                                << (hasVoted[i] ? "1" : "0") << endl;
                }
                saveUserFile.close();
                
                // Save updated vote information
                ofstream saveVoteFile("votes.txt");
                for (int i = 0; i < candidateCount; i++) {
                    saveVoteFile << candidateIds[i] << "," << candidateVotes[i] << endl;
                }
                saveVoteFile.close();
                
                cout << "Vote successfully cast!\n";
                break;
            }
            
            // Case 3: View candidates
            case 3: {
                // Display all candidates
                cout << "\n----- CANDIDATE LIST -----\n";
                cout << "ID | Name | Party\n";
                cout << "--------------------\n";
                
                for (int i = 0; i < candidateCount; i++) {
                    cout << candidateIds[i] << " | " << candidateNames[i] 
                         << " | " << candidateParties[i] << endl;
                }
                break;
            }
            
            // Case 4: View results
            case 4: {
                // Display all voting results
                cout << "\n----- ELECTION RESULTS -----\n";
                cout << "Candidate | Party | Votes\n";
                cout << "-------------------------\n";
                
                for (int i = 0; i < candidateCount; i++) {
                    cout << candidateNames[i] << " | " << candidateParties[i] 
                         << " | " << candidateVotes[i] << endl;
                }
                break;
            }
            
            // Case 5: Exit program
            case 5: {
                cout << "Thank you for using the Voting System. Goodbye!\n";
                running = false;  // Set running to false to exit the main loop
                break;
            }
            
            // Default case: Invalid choice
            default: {
                cout << "Invalid choice. Please enter a number between 1 and 5.\n";
                break;
            }
        }
    }
    
    // End of program
    return 0;
}