
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const string FILE_NAME = "students.txt";

struct Student {
    int id;
    string name;
    int age;
    string course;
    double marks;
};

// ---------------- INPUT HELPERS ----------------

string readText(string prompt) {
    string value;

    while (true) {
        cout << prompt;
        getline(cin, value);

        if (!value.empty() && value.find_first_not_of(' ') != string::npos
            && value.find('|') == string::npos) {
            return value;
        }

        cout << "Invalid input! Enter text without the | symbol.\n";
    }
}

int readInt(string prompt, int minimum, int maximum) {
    string line;
    int value;
    char extra;

    while (true) {
        cout << prompt;
        getline(cin, line);

        stringstream ss(line);

        if (ss >> value && !(ss >> extra) &&
            value >= minimum && value <= maximum) {
            return value;
        }

        cout << "Invalid input! Enter a number between "
             << minimum << " and " << maximum << ".\n";
    }
}

double readMarks(string prompt) {
    string line;
    double value;
    char extra;

    while (true) {
        cout << prompt;
        getline(cin, line);

        stringstream ss(line);

        if (ss >> value && !(ss >> extra) &&
            value >= 0 && value <= 100) {
            return value;
        }

        cout << "Invalid marks! Enter a value from 0 to 100.\n";
    }
}

bool confirmAction(string prompt) {
    string answer;

    while (true) {
        cout << prompt << " (Y/N): ";
        getline(cin, answer);

        if (answer == "Y" || answer == "y") return true;
        if (answer == "N" || answer == "n") return false;

        cout << "Please enter Y or N.\n";
    }
}

// ---------------- STUDENT UTILITIES ----------------

int findStudentIndex(const vector<Student>& students, int id) {
    for (int i = 0; i < (int)students.size(); i++) {
        if (students[i].id == id) {
            return i;
        }
    }
    return -1;
}

string getGrade(double marks) {
    if (marks >= 90) return "A+";
    if (marks >= 80) return "A";
    if (marks >= 70) return "B";
    if (marks >= 60) return "C";
    if (marks >= 50) return "D";
    return "F";
}

string getResult(double marks) {
    return marks >= 40 ? "PASS" : "FAIL";
}

// ---------------- FILE HANDLING ----------------

bool saveStudents(const vector<Student>& students) {
    ofstream file(FILE_NAME.c_str());

    if (!file) {
        cout << "\nERROR: Could not save student records!\n";
        return false;
    }

    for (const Student& s : students) {
        file << s.id << "|"
             << s.name << "|"
             << s.age << "|"
             << s.course << "|"
             << fixed << setprecision(2) << s.marks << "\n";
    }

    file.close();

    if (!file) {
        cout << "\nERROR: Problem writing the student file!\n";
        return false;
    }

    return true;
}

void loadStudents(vector<Student>& students) {
    ifstream file(FILE_NAME.c_str());

    if (!file) return;

    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string idText, name, ageText, course, marksText;

        if (!getline(ss, idText, '|')) continue;
        if (!getline(ss, name, '|')) continue;
        if (!getline(ss, ageText, '|')) continue;
        if (!getline(ss, course, '|')) continue;
        if (!getline(ss, marksText)) continue;

        try {
            Student s;
            s.id = stoi(idText);
            s.name = name;
            s.age = stoi(ageText);
            s.course = course;
            s.marks = stod(marksText);

            if (s.id > 0 && s.age >= 1 && s.age <= 120 &&
                s.marks >= 0 && s.marks <= 100 &&
                findStudentIndex(students, s.id) == -1) {
                students.push_back(s);
            }
        }
        catch (...) {
            continue;
        }
    }

    file.close();
}

// ---------------- DISPLAY FUNCTIONS ----------------

void showBanner() {
    cout << "\n";
    cout << "============================================================\n";
    cout << "              STUDENT MANAGEMENT SYSTEM PRO                 \n";
    cout << "============================================================\n";
    cout << "          ORGANIZE  |  TRACK  |  ACHIEVE                    \n";
    cout << "============================================================\n";
}

void showStudent(const Student& s) {
    cout << "\n------------------------------------------------------------\n";
    cout << "STUDENT ID : " << s.id << "\n";
    cout << "NAME       : " << s.name << "\n";
    cout << "AGE        : " << s.age << "\n";
    cout << "COURSE     : " << s.course << "\n";
    cout << "MARKS      : " << fixed << setprecision(2) << s.marks << "/100\n";
    cout << "GRADE      : " << getGrade(s.marks) << "\n";
    cout << "RESULT     : " << getResult(s.marks) << "\n";
    cout << "------------------------------------------------------------\n";
}

void displayStudents(const vector<Student>& students) {
    if (students.empty()) {
        cout << "\nNo student records available yet.\n";
        return;
    }

    cout << "\n====================== STUDENT RECORDS =====================\n";

    cout << left
         << setw(8) << "ID"
         << setw(24) << "NAME"
         << setw(8) << "AGE"
         << setw(20) << "COURSE"
         << setw(10) << "MARKS"
         << setw(8) << "GRADE" << "\n";

    cout << string(78, '-') << "\n";

    for (const Student& s : students) {
        cout << left
             << setw(8) << s.id
             << setw(24) << s.name.substr(0, 22)
             << setw(8) << s.age
             << setw(20) << s.course.substr(0, 18)
             << setw(10) << fixed << setprecision(2) << s.marks
             << setw(8) << getGrade(s.marks) << "\n";
    }

    cout << string(78, '-') << "\n";
    cout << "TOTAL STUDENTS: " << students.size() << "\n";
}

// ---------------- ADD STUDENT ----------------

void addStudent(vector<Student>& students) {
    cout << "\n======================= ADD STUDENT ========================\n";

    Student s;

    s.id = readInt("Enter Student ID: ", 1, 999999999);

    if (findStudentIndex(students, s.id) != -1) {
        cout << "\nA student with this ID already exists!\n";
        return;
    }

    s.name = readText("Enter Full Name: ");
    s.age = readInt("Enter Age: ", 1, 120);
    s.course = readText("Enter Course: ");
    s.marks = readMarks("Enter Marks (0-100): ");

    students.push_back(s);

    if (saveStudents(students)) {
        cout << "\n[SUCCESS] Student added and saved successfully!\n";
    } else {
        students.pop_back();
    }
}

// ---------------- SEARCH STUDENT ----------------

void searchStudent(const vector<Student>& students) {
    cout << "\n====================== SEARCH STUDENT ======================\n";

    int id = readInt("Enter Student ID: ", 1, 999999999);
    int index = findStudentIndex(students, id);

    if (index == -1) {
        cout << "\n[NOT FOUND] No student exists with ID " << id << ".\n";
        return;
    }

    cout << "\n[SUCCESS] Student record found!\n";
    showStudent(students[index]);
}

// ---------------- UPDATE STUDENT ----------------

void updateStudent(vector<Student>& students) {
    cout << "\n====================== UPDATE STUDENT =====================\n";

    int id = readInt("Enter Student ID to update: ", 1, 999999999);
    int index = findStudentIndex(students, id);

    if (index == -1) {
        cout << "\nStudent not found!\n";
        return;
    }

    showStudent(students[index]);

    if (!confirmAction("Do you want to update this record?")) {
        cout << "\nUpdate cancelled.\n";
        return;
    }

    Student updated = students[index];

    cout << "\nEnter the new details:\n";

    updated.name = readText("New Full Name: ");
    updated.age = readInt("New Age: ", 1, 120);
    updated.course = readText("New Course: ");
    updated.marks = readMarks("New Marks (0-100): ");

    Student original = students[index];
    students[index] = updated;

    if (saveStudents(students)) {
        cout << "\n[SUCCESS] Student details updated!\n";
    } else {
        students[index] = original;
    }
}

// ---------------- DELETE STUDENT ----------------

void deleteStudent(vector<Student>& students) {
    cout << "\n====================== DELETE STUDENT ======================\n";

    int id = readInt("Enter Student ID to delete: ", 1, 999999999);
    int index = findStudentIndex(students, id);

    if (index == -1) {
        cout << "\nStudent not found!\n";
        return;
    }

    showStudent(students[index]);

    if (!confirmAction("Are you sure you want to delete this student?")) {
        cout << "\nDeletion cancelled.\n";
        return;
    }

    Student deleted = students[index];

    students.erase(students.begin() + index);

    if (saveStudents(students)) {
        cout << "\n[SUCCESS] Student record deleted!\n";
    } else {
        students.insert(students.begin() + index, deleted);
    }
}

// ---------------- CLASS STATISTICS ----------------

void showStatistics(const vector<Student>& students) {
    cout << "\n===================== CLASS STATISTICS =====================\n";

    if (students.empty()) {
        cout << "No records available to calculate statistics.\n";
        return;
    }

    double total = 0;
    double highest = students[0].marks;
    double lowest = students[0].marks;
    int passed = 0;
    int failed = 0;

    for (const Student& s : students) {
        total += s.marks;

        if (s.marks > highest) highest = s.marks;
        if (s.marks < lowest) lowest = s.marks;

        if (s.marks >= 40) passed++;
        else failed++;
    }

    double average = total / students.size();

    cout << fixed << setprecision(2);
    cout << "\nTotal Students : " << students.size();
    cout << "\nClass Average  : " << average;
    cout << "\nHighest Marks  : " << highest;
    cout << "\nLowest Marks   : " << lowest;
    cout << "\nPassed         : " << passed;
    cout << "\nFailed         : " << failed;

    cout << "\nPass Percentage: "
         << (passed * 100.0 / students.size()) << "%\n";
}

// ---------------- TOP PERFORMER ----------------

void showTopPerformer(const vector<Student>& students) {
    cout << "\n======================= TOP PERFORMER ======================\n";

    if (students.empty()) {
        cout << "No student records available.\n";
        return;
    }

    vector<Student>::const_iterator top =
        max_element(students.begin(), students.end(),
        [](const Student& a, const Student& b) {
            return a.marks < b.marks;
        });

    cout << "\nCongratulations to our top performer!\n";
    showStudent(*top);
}

// ---------------- SORT STUDENTS ----------------

void sortStudents(vector<Student>& students) {
    if (students.empty()) {
        cout << "\nNo student records available to sort.\n";
        return;
    }

    sort(students.begin(), students.end(),
         [](const Student& a, const Student& b) {
             return a.marks > b.marks;
         });

    cout << "\n[SUCCESS] Students sorted by marks (highest to lowest).\n";
    displayStudents(students);
}

// ---------------- MAIN PROGRAM ----------------

int main() {
    vector<Student> students;

    loadStudents(students);

    int choice;

    while (true) {
        showBanner();

        cout << "\n  1. Add New Student";
        cout << "\n  2. Display All Students";
        cout << "\n  3. Search Student";
        cout << "\n  4. Update Student Details";
        cout << "\n  5. Delete Student";
        cout << "\n  6. Class Statistics";
        cout << "\n  7. View Top Performer";
        cout << "\n  8. Sort Students by Marks";
        cout << "\n  9. Exit";

        cout << "\n\n------------------------------------------------------------\n";

        choice = readInt("  Enter your choice (1-9): ", 1, 9);

        switch (choice) {
            case 1:
                addStudent(students);
                break;

            case 2:
                displayStudents(students);
                break;

            case 3:
                searchStudent(students);
                break;

            case 4:
                updateStudent(students);
                break;

            case 5:
                deleteStudent(students);
                break;

            case 6:
                showStatistics(students);
                break;

            case 7:
                showTopPerformer(students);
                break;

            case 8:
                sortStudents(students);
                break;

            case 9:
                cout << "\n============================================================\n";
                cout << "       Thank you for using STUDENT MANAGEMENT SYSTEM PRO!\n";
                cout << "              Your student records are saved.\n";
                cout << "============================================================\n";
                return 0;
        }

        cout << "\nPress ENTER to return to the main menu...";
        string pause;
        getline(cin, pause);
    }

    return 0;
}
