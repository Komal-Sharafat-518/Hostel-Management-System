#include <iostream>
#include <fstream>
#include <limits>
#include <string>
using namespace std;

const int MAX_STUDENTS = 100;

struct Student {
    string id;          // e.g. "2024-CS-599" — kept as text, not a number
    string name;
    int age;
    string roomType;
    bool isPaid;
    string complaint;
    string feedback;
};

Student students[MAX_STUDENTS];
int studentCount = 0;

void ensureFileExists(const string& filePath);
void saveStudentsToFile(const Student students[], int count);
void loadStudentsFromFile(Student students[], int& count);
void viewComplaints(const Student students[], int count);
void viewFeedback(const Student students[], int count);
void showIntro();
void wardenMenu(Student students[], int& count);
void studentMenu(Student students[], int count);
int readMenuChoice();
bool idExists(const Student students[], int count, const string& id);
string readNonEmptyLine(const string& prompt);
int readAge(const string& prompt);

// Makes sure the data file exists so ifstream never opens a missing file.
void ensureFileExists(const string& filePath) {
    ifstream file(filePath);
    if (!file) {
        ofstream outFile(filePath);
        outFile.close();
    }
}

void saveStudentsToFile(const Student students[], int count) {
    ofstream file("students.txt", ios::trunc);
    if (!file) {
        cout << "Error saving students.\n";
        return;
    }
    for (int i = 0; i < count; i++) {
        file << students[i].id << "\n";
        file << students[i].name << "\n";
        file << students[i].age << "\n";
        file << students[i].roomType << "\n";
        file << students[i].isPaid << "\n";
        file << students[i].complaint << "\n";
        file << students[i].feedback << "\n";
    }
    file.close();
}

void loadStudentsFromFile(Student students[], int& count) {
    ifstream file("students.txt");
    if (!file) {
        cout << "No previous data found.\n";
        return;
    }
    count = 0;
    while (file && count < MAX_STUDENTS) {
        string id;
        if (!getline(file, id)) break;   // end of file / nothing left to read
        if (id.empty()) break;           // trailing blank line, stop cleanly

        string name;
        getline(file, name);

        int age;
        file >> age;
        if (file.fail()) break;
        file.ignore();

        string roomType;
        getline(file, roomType);

        bool isPaid;
        file >> isPaid;
        if (file.fail()) break;
        file.ignore();

        string complaint, feedback;
        getline(file, complaint);
        getline(file, feedback);

        // Sanity check: reject clearly corrupted/misaligned records instead
        // of silently filling the array with garbage.
        if (age <= 0 || age > 120) {
            cout << "Warning: students.txt appears corrupted at record "
                 << (count + 1) << ". Stopped loading further records.\n";
            break;
        }

        students[count].id = id;
        students[count].name = name;
        students[count].age = age;
        students[count].roomType = roomType;
        students[count].isPaid = isPaid;
        students[count].complaint = complaint;
        students[count].feedback = feedback;
        count++;
    }
    file.close();
}

// Reads a menu choice safely; re-prompts instead of looping forever on bad input.
int readMenuChoice() {
    int choice;
    while (!(cin >> choice)) {
        cout << "Invalid input. Please enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return choice;
}

// Reads a full line and keeps asking until it's non-empty.
// Assumes any leftover newline in cin has already been cleared by the caller.
string readNonEmptyLine(const string& prompt) {
    string value;
    do {
        cout << prompt;
        getline(cin, value);
    } while (value.empty());
    return value;
}

int readAge(const string& prompt) {
    int age;
    cout << prompt;
    while (!(cin >> age) || age <= 0 || age > 120) {
        cout << "Invalid age. Enter a number between 1 and 120: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return age;
}

bool idExists(const Student students[], int count, const string& id) {
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) return true;
    }
    return false;
}

void viewComplaints(const Student students[], int count) {
    bool hasComplaints = false;
    for (int i = 0; i < count; i++) {
        if (!students[i].complaint.empty()) {
            cout << "ID: " << students[i].id
                 << ", Name: " << students[i].name
                 << ", Complaint: " << students[i].complaint << endl;
            hasComplaints = true;
        }
    }
    if (!hasComplaints) cout << "No complaints found.\n";
}

void viewFeedback(const Student students[], int count) {
    bool hasFeedback = false;
    for (int i = 0; i < count; i++) {
        if (!students[i].feedback.empty()) {
            cout << "ID: " << students[i].id
                 << ", Name: " << students[i].name
                 << ", Feedback: " << students[i].feedback << endl;
            hasFeedback = true;
        }
    }
    if (!hasFeedback) cout << "No feedback submitted.\n";
}

void showIntro() {
    cout << "\nWelcome to Student's Nest Hostel!\n";
    cout << "Where Clean, Fresh, and Refreshing Environment is a Priority.\n";
    cout << "At Student's Nest, we believe that a healthy environment is key to a productive life.\n";
    cout << "Here, studying comes first, but we also care about comfort, safety, and fun.\n";
    cout << "We provide a balanced atmosphere where students can focus on their academics while also enjoying a great community and facilities.\n";
    cout << "Thank you for choosing Student's Nest Hostel!\n\n";
}

void wardenMenu(Student students[], int& count) {
    int choice;
    do {
        cout << "\n--- Warden Menu ---\n";
        cout << "1. Register Student\n";
        cout << "2. View Students\n";
        cout << "3. View Complaints\n";
        cout << "4. View Feedback\n";
        cout << "5. Update Payment Status\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        choice = readMenuChoice();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover newline

        switch (choice) {
            case 1: {
                if (count >= MAX_STUDENTS) {
                    cout << "Student limit reached.\n";
                    break;
                }
                string newId = readNonEmptyLine("Enter ID (e.g. 2024-CS-599): ");
                if (idExists(students, count, newId)) {
                    cout << "A student with this ID already exists.\n";
                    break;
                }
                students[count].id = newId;
                students[count].name = readNonEmptyLine("Enter Name: ");
                students[count].age = readAge("Enter Age: ");
                students[count].roomType = readNonEmptyLine("Enter Room Type: ");

                students[count].isPaid = false;
                students[count].complaint = "";
                students[count].feedback = "";
                count++;
                saveStudentsToFile(students, count);
                cout << "Student registered successfully.\n";
                break;
            }
            case 2:
                if (count == 0) {
                    cout << "No students registered yet.\n";
                    break;
                }
                for (int i = 0; i < count; i++) {
                    cout << "ID: " << students[i].id
                         << ", Name: " << students[i].name
                         << ", Age: " << students[i].age
                         << ", Room Type: " << students[i].roomType
                         << ", Payment: " << (students[i].isPaid ? "Paid" : "Unpaid") << endl;
                }
                break;
            case 3:
                viewComplaints(students, count);
                break;
            case 4:
                viewFeedback(students, count);
                break;
            case 5: {
                string id = readNonEmptyLine("Enter Student ID to update payment: ");
                bool found = false;
                for (int i = 0; i < count; i++) {
                    if (students[i].id == id) {
                        students[i].isPaid = true;
                        saveStudentsToFile(students, count);
                        cout << "Payment updated for " << students[i].name << ".\n";
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Student not found.\n";
                break;
            }
            case 6:
                cout << "Exiting Warden Menu...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 6);
}

void studentMenu(Student students[], int count) {
    string id = readNonEmptyLine("Enter your ID: ");

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Student not found.\n";
        return;
    }

    int choice;
    do {
        cout << "\n--- Student Menu ---\n";
        cout << "1. View Information\n";
        cout << "2. Submit Complaint\n";
        cout << "3. Submit Feedback\n";
        cout << "4. View Complaints\n";
        cout << "5. View Feedback\n";
        cout << "6. Pay Fees\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        choice = readMenuChoice();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover newline

        switch (choice) {
            case 1:
                cout << "Name: " << students[index].name
                     << ", Age: " << students[index].age
                     << ", Room Type: " << students[index].roomType << endl;
                break;
            case 2:
                students[index].complaint = readNonEmptyLine("Enter Complaint: ");
                saveStudentsToFile(students, count);
                cout << "Complaint submitted.\n";
                break;
            case 3:
                students[index].feedback = readNonEmptyLine("Enter Feedback: ");
                saveStudentsToFile(students, count);
                cout << "Feedback submitted.\n";
                break;
            case 4:
                viewComplaints(students, count);
                break;
            case 5:
                viewFeedback(students, count);
                break;
            case 6:
                if (students[index].isPaid) {
                    cout << "Fees already paid.\n";
                } else {
                    students[index].isPaid = true;
                    saveStudentsToFile(students, count);
                    cout << "Fees paid successfully.\n";
                }
                break;
            case 7:
                cout << "Exiting Student Menu...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 7);
}

int main() {
    ensureFileExists("students.txt");
    loadStudentsFromFile(students, studentCount);
    showIntro();

    string role;
    cout << "Enter your role (warden/student): ";
    cin >> role;

    if (role == "warden") {
        wardenMenu(students, studentCount);
    } else if (role == "student") {
        studentMenu(students, studentCount);
    } else {
        cout << "Invalid role.\n";
    }

    return 0;
}
