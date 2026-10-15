#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Grade {
    string assignment;
    double score;
    double possiblePoints;
};

struct Course {
    int id;
    string name;
};

struct Student {
    int id;
    string name;
    vector<int> courseIds;
    vector<Grade> grades;
};

class GradeManager {
private:
    vector<Student> students;
    vector<Course> courses;
    int nextStudentId;
    int nextCourseId;

public:
    GradeManager() : nextStudentId(1), nextCourseId(1) {
        loadSampleData();
    }

    void run() {
        bool running = true;

        while (running) {
            printMenu();
            int choice = readInt("Choose an option: ", 0, 10);

            switch (choice) {
            case 1: listStudents(); break;
            case 2: addStudent(); break;
            case 3: searchStudent(); break;
            case 4: listCourses(); break;
            case 5: addCourse(); break;
            case 6: enrollStudent(); break;
            case 7: addGrade(); break;
            case 8: showStudentReport(); break;
            case 9: showClassAverages(); break;
            case 10: saveData(); break;
            case 0:
                askToSaveBeforeExit();
                running = false;
                break;
            default:
                cout << "That option is not available.\n";
            }
        }

        cout << "\nThanks for using Student Grade Manager!\n";
    }

private:
    void printMenu() const {
        cout << "\n=====================================\n";
        cout << "        STUDENT GRADE MANAGER\n";
        cout << "=====================================\n";
        cout << " 1. List students\n";
        cout << " 2. Add a student\n";
        cout << " 3. Search for a student\n";
        cout << " 4. List classes\n";
        cout << " 5. Add a class\n";
        cout << " 6. Enroll a student in a class\n";
        cout << " 7. Add a grade\n";
        cout << " 8. View a student's report\n";
        cout << " 9. View class averages\n";
        cout << "10. Save data to file\n";
        cout << " 0. Exit\n";
        cout << "-------------------------------------\n";
    }

    void loadSampleData() {
        courses.push_back({ nextCourseId++, "Computer Science" });
        courses.push_back({ nextCourseId++, "English" });
        courses.push_back({ nextCourseId++, "Algebra" });

        Student maya;
        maya.id = nextStudentId++;
        maya.name = "Maya Patel";
        maya.courseIds = { 1, 2 };
        maya.grades.push_back({ "CS Lab 1", 92, 100 });
        maya.grades.push_back({ "CS Quiz", 18, 20 });
        maya.grades.push_back({ "Essay 1", 85, 100 });
        students.push_back(maya);

        Student jordan;
        jordan.id = nextStudentId++;
        jordan.name = "Jordan Lee";
        jordan.courseIds = { 1, 3 };
        jordan.grades.push_back({ "CS Lab 1", 78, 100 });
        jordan.grades.push_back({ "CS Quiz", 16, 20 });
        jordan.grades.push_back({ "Algebra Test", 88, 100 });
        students.push_back(jordan);

        Student sam;
        sam.id = nextStudentId++;
        sam.name = "Sam Rivera";
        sam.courseIds = { 2, 3 };
        sam.grades.push_back({ "Essay 1", 94, 100 });
        sam.grades.push_back({ "Algebra Test", 73, 100 });
        students.push_back(sam);
    }

    int readInt(const string& prompt, int minimum, int maximum) const {
        int value;

        while (true) {
            cout << prompt;
            if (cin >> value && value >= minimum && value <= maximum) {
                clearInputLine();
                return value;
            }

            cout << "Please enter a number from " << minimum
                << " to " << maximum << ".\n";
            cin.clear();
            clearInputLine();
        }
    }

    double readDouble(const string& prompt, double minimum,
        double maximum) const {
        double value;

        while (true) {
            cout << prompt;
            if (cin >> value && value >= minimum && value <= maximum) {
                clearInputLine();
                return value;
            }

            cout << "Please enter a number from " << minimum
                << " to " << maximum << ".\n";
            cin.clear();
            clearInputLine();
        }
    }

    string readLine(const string& prompt) const {
        string value;

        while (true) {
            cout << prompt;
            getline(cin, value);
            value = trim(value);

            if (!value.empty()) {
                return value;
            }

            cout << "This field cannot be blank. Try again.\n";
        }
    }

    void clearInputLine() const {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string trim(const string& value) const {
        size_t first = value.find_first_not_of(" \t\r\n");

        if (first == string::npos) {
            return "";
        }

        size_t last = value.find_last_not_of(" \t\r\n");
        return value.substr(first, last - first + 1);
    }

    string lowerCase(string value) const {
        for (char& character : value) {
            character = static_cast<char>(
                tolower(static_cast<unsigned char>(character)));
        }
        return value;
    }

    Student* findStudentById(int id) {
        for (Student& student : students) {
            if (student.id == id) {
                return &student;
            }
        }
        return nullptr;
    }

    const Student* findStudentById(int id) const {
        for (const Student& student : students) {
            if (student.id == id) {
                return &student;
            }
        }
        return nullptr;
    }

    Course* findCourseById(int id) {
        for (Course& course : courses) {
            if (course.id == id) {
                return &course;
            }
        }
        return nullptr;
    }

    const Course* findCourseById(int id) const {
        for (const Course& course : courses) {
            if (course.id == id) {
                return &course;
            }
        }
        return nullptr;
    }

    void listStudents() const {
        cout << "\n--- Students ---\n";

        if (students.empty()) {
            cout << "No students have been added yet.\n";
            return;
        }

        cout << left << setw(8) << "ID" << setw(25) << "Name"
            << "Courses\n";
        cout << string(55, '-') << "\n";

        for (const Student& student : students) {
            cout << left << setw(8) << student.id
                << setw(25) << student.name
                << student.courseIds.size() << "\n";
        }
    }

    void addStudent() {
        cout << "\n--- Add Student ---\n";
        string name = readLine("Student's full name: ");

        for (const Student& student : students) {
            if (lowerCase(student.name) == lowerCase(name)) {
                cout << "A student with that name already exists.\n";
                cout << "Student was not added.\n";
                return;
            }
        }

        Student student;
        student.id = nextStudentId++;
        student.name = name;
        students.push_back(student);

        cout << "Added " << student.name << " with ID "
            << student.id << ".\n";
    }

    void searchStudent() const {
        cout << "\n--- Search Student ---\n";
        string searchText = lowerCase(readLine("Enter a name to search for: "));
        bool found = false;

        for (const Student& student : students) {
            if (lowerCase(student.name).find(searchText) != string::npos) {
                printStudentSummary(student);
                found = true;
            }
        }

        if (!found) {
            cout << "No matching student was found.\n";
        }
    }

    void printStudentSummary(const Student& student) const {
        cout << "\nID: " << student.id << "\n";
        cout << "Name: " << student.name << "\n";
        cout << "Enrolled classes: ";

        if (student.courseIds.empty()) {
            cout << "None";
        }
        else {
            for (size_t i = 0; i < student.courseIds.size(); ++i) {
                const Course* course = findCourseById(student.courseIds[i]);
                if (course != nullptr) {
                    if (i > 0) cout << ", ";
                    cout << course->name;
                }
            }
        }

        cout << "\nGrades recorded: " << student.grades.size() << "\n";
        double average = calculateAverage(student);
        if (average >= 0) {
            cout << "Overall average: " << fixed << setprecision(1)
                << average << "% (" << letterGrade(average) << ")\n";
        }
        else {
            cout << "Overall average: No grades yet\n";
        }
    }

    void listCourses() const {
        cout << "\n--- Classes ---\n";

        if (courses.empty()) {
            cout << "No classes have been added yet.\n";
            return;
        }

        cout << left << setw(8) << "ID" << "Class name\n";
        cout << string(40, '-') << "\n";

        for (const Course& course : courses) {
            cout << left << setw(8) << course.id << course.name << "\n";
        }
    }

    void addCourse() {
        cout << "\n--- Add Class ---\n";
        string name = readLine("Class name: ");

        for (const Course& course : courses) {
            if (lowerCase(course.name) == lowerCase(name)) {
                cout << "That class already exists.\n";
                return;
            }
        }

        Course course;
        course.id = nextCourseId++;
        course.name = name;
        courses.push_back(course);

        cout << "Added class \"" << course.name << "\" with ID "
            << course.id << ".\n";
    }

    void enrollStudent() {
        cout << "\n--- Enroll Student ---\n";
        if (students.empty() || courses.empty()) {
            cout << "You need at least one student and one class first.\n";
            return;
        }

        listStudents();
        int studentId = readInt("Enter student ID: ", 1, 1000000);
        Student* student = findStudentById(studentId);

        if (student == nullptr) {
            cout << "Student ID not found.\n";
            return;
        }

        listCourses();
        int courseId = readInt("Enter class ID: ", 1, 1000000);
        Course* course = findCourseById(courseId);

        if (course == nullptr) {
            cout << "Class ID not found.\n";
            return;
        }

        if (containsCourse(*student, courseId)) {
            cout << student->name << " is already enrolled in "
                << course->name << ".\n";
            return;
        }

        student->courseIds.push_back(courseId);
        cout << "Enrolled " << student->name << " in "
            << course->name << ".\n";
    }

    bool containsCourse(const Student& student, int courseId) const {
        return find(student.courseIds.begin(), student.courseIds.end(),
            courseId) != student.courseIds.end();
    }

    void addGrade() {
        cout << "\n--- Add Grade ---\n";
        if (students.empty()) {
            cout << "Add a student before entering grades.\n";
            return;
        }

        listStudents();
        int studentId = readInt("Enter student ID: ", 1, 1000000);
        Student* student = findStudentById(studentId);

        if (student == nullptr) {
            cout << "Student ID not found.\n";
            return;
        }

        string assignment = readLine("Assignment or test name: ");
        double possible = readDouble("Total points possible: ", 1, 1000000);
        double score = readDouble("Points earned: ", 0, possible);

        Grade grade;
        grade.assignment = assignment;
        grade.score = score;
        grade.possiblePoints = possible;
        student->grades.push_back(grade);

        double percentage = (score / possible) * 100.0;
        cout << "Grade added: " << fixed << setprecision(1)
            << percentage << "% (" << letterGrade(percentage) << ").\n";
    }

    void showStudentReport() const {
        cout << "\n--- Student Report ---\n";
        if (students.empty()) {
            cout << "There are no students to report on.\n";
            return;
        }

        listStudents();
        int id = readInt("Enter student ID: ", 1, 1000000);
        const Student* student = findStudentById(id);

        if (student == nullptr) {
            cout << "Student ID not found.\n";
            return;
        }

        cout << "\n=====================================\n";
        cout << "REPORT FOR " << student->name << "\n";
        cout << "Student ID: " << student->id << "\n";
        cout << "=====================================\n";

        printEnrolledCourses(*student);
        printGradeTable(*student);

        double average = calculateAverage(*student);
        if (average < 0) {
            cout << "\nOverall average: No grades recorded\n";
        }
        else {
            cout << "\nOverall average: " << fixed << setprecision(2)
                << average << "%\n";
            cout << "Letter grade: " << letterGrade(average) << "\n";
            printEncouragement(average);
        }
    }

    void printEnrolledCourses(const Student& student) const {
        cout << "\nClasses: ";

        if (student.courseIds.empty()) {
            cout << "Not enrolled in any classes.\n";
            return;
        }

        for (size_t i = 0; i < student.courseIds.size(); ++i) {
            const Course* course = findCourseById(student.courseIds[i]);
            if (course != nullptr) {
                if (i != 0) cout << ", ";
                cout << course->name;
            }
        }

        cout << "\n";
    }

    void printGradeTable(const Student& student) const {
        cout << "\nGrades:\n";

        if (student.grades.empty()) {
            cout << "  No grades have been entered.\n";
            return;
        }

        cout << left << setw(25) << "Assignment"
            << right << setw(12) << "Score"
            << setw(12) << "Possible"
            << setw(12) << "Percent" << "\n";
        cout << string(61, '-') << "\n";

        for (const Grade& grade : student.grades) {
            double percentage = (grade.score / grade.possiblePoints) * 100.0;

            cout << left << setw(25) << grade.assignment
                << right << setw(12) << fixed << setprecision(1)
                << grade.score
                << setw(12) << grade.possiblePoints
                << setw(11) << percentage << "%\n";
        }
    }

    double calculateAverage(const Student& student) const {
        if (student.grades.empty()) {
            return -1.0;
        }

        double totalEarned = 0;
        double totalPossible = 0;

        for (const Grade& grade : student.grades) {
            totalEarned += grade.score;
            totalPossible += grade.possiblePoints;
        }

        if (totalPossible <= 0) {
            return -1.0;
        }

        return (totalEarned / totalPossible) * 100.0;
    }

    string letterGrade(double percentage) const {
        if (percentage >= 90) return "A";
        if (percentage >= 80) return "B";
        if (percentage >= 70) return "C";
        if (percentage >= 60) return "D";
        return "F";
    }

    void printEncouragement(double average) const {
        if (average >= 90) {
            cout << "Status: Excellent work. Keep it up!\n";
        }
        else if (average >= 80) {
            cout << "Status: Good work. Keep practicing.\n";
        }
        else if (average >= 70) {
            cout << "Status: Passing. Review missed questions to improve.\n";
        }
        else if (average >= 60) {
            cout << "Status: Close to passing. Make a study plan.\n";
        }
        else {
            cout << "Status: Consider asking your instructor for help.\n";
        }
    }

    void showClassAverages() const {
        cout << "\n--- Class Averages ---\n";

        if (courses.empty()) {
            cout << "No classes have been created.\n";
            return;
        }

        for (const Course& course : courses) {
            double totalPercentages = 0;
            int studentsWithGrades = 0;

            for (const Student& student : students) {
                if (containsCourse(student, course.id)) {
                    double average = calculateAverageForCourse(student, course);
                    if (average >= 0) {
                        totalPercentages += average;
                        ++studentsWithGrades;
                    }
                }
            }

            cout << "\n" << course.name << "\n";
            if (studentsWithGrades == 0) {
                cout << "  No enrolled students have grades recorded.\n";
            }
            else {
                double average = totalPercentages / studentsWithGrades;
                cout << "  Students with grades: " << studentsWithGrades << "\n";
                cout << "  Average: " << fixed << setprecision(1)
                    << average << "% (" << letterGrade(average) << ")\n";
            }
        }

        cout << "\nNote: class averages use the available student averages.\n";
    }

    double calculateAverageForCourse(const Student& student,
        const Course& course) const {
        (void)course;
        return calculateAverage(student);
    }

    void askToSaveBeforeExit() {
        cout << "Save your changes before exiting? (y/n): ";
        string answer;
        getline(cin, answer);

        if (!answer.empty() &&
            (answer[0] == 'y' || answer[0] == 'Y')) {
            saveData();
        }
    }

    void saveData() const {
        ofstream file("grades_data.txt");

        if (!file) {
            cout << "Error: could not open grades_data.txt for writing.\n";
            return;
        }

        file << "COURSES\n";
        for (const Course& course : courses) {
            file << course.id << "|" << course.name << "\n";
        }

        file << "STUDENTS\n";
        for (const Student& student : students) {
            file << student.id << "|" << student.name << "|";
            for (size_t i = 0; i < student.courseIds.size(); ++i) {
                if (i > 0) file << ",";
                file << student.courseIds[i];
            }
            file << "\n";
        }

        file << "GRADES\n";
        for (const Student& student : students) {
            for (const Grade& grade : student.grades) {
                file << student.id << "|" << grade.assignment << "|"
                    << grade.score << "|" << grade.possiblePoints << "\n";
            }
        }

        file.close();
        cout << "Data saved to grades_data.txt.\n";
    }
};

int main() {
    cout << "Welcome to Student Grade Manager!\n";
    cout << "Some students and classes are already loaded.\n";

    GradeManager manager;
    manager.run();

    return 0;
}
