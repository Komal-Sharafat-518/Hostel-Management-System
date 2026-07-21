#include <iostream>
#include <fstream>
#include <string>
using namespace std;


const int MAX_STUDENTS = 100;


struct Student {
    int id;
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
        file >> students[count].id;
        if (file.fail()) break;  
        file.ignore();
        getline(file, students[count].name);
        file >> students[count].age;
        file.ignore();
        getline(file, students[count].roomType);
        file >> students[count].isPaid;
        file.ignore();
        getline(file, students[count].complaint);
        getline(file, students[count].feedback);
        count++;
    }
    file.close();
}

// Function to view complaints
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

// Function to view feedback
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
        cin >> choice;

        switch (choice) {
            case 1: {
                if (count >= MAX_STUDENTS) {
                    cout << "Student limit reached.\n";
                    break;
                }
                cout << "Enter ID, Name, Age, Room Type: ";
                cin >> students[count].id;
                cin.ignore();
                getline(cin, students[count].name);
                cin >> students[count].age;
                cin.ignore();
                getline(cin, students[count].roomType);
         
                students[count].isPaid = false;
                students[count].complaint = "";
                students[count].feedback = "";
                count++;
                saveStudentsToFile(students, count);
                cout << "Student registered successfully.\n";
                break;
            }
            case 2:
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
                int id;
                cout << "Enter Student ID to update payment: ";
                cin >> id;
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

// Student Menu
void studentMenu(Student students[], int count) {
    int id;
    cout << "Enter your ID: ";
    cin >> id;

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
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Name: " << students[index].name
                     << ", Age: " << students[index].age
                     << ", Room Type: " << students[index].roomType << endl;
                break;
            case 2:
                cin.ignore();
                cout << "Enter Complaint: ";
                getline(cin, students[index].complaint);
                saveStudentsToFile(students, count);
                cout << "Complaint submitted.\n";
                break;
            case 3:
                cin.ignore();
                cout << "Enter Feedback: ";
                getline(cin, students[index].feedback);
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
