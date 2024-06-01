#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

class Student {
private:
    string name;
    int id;
    int age;
    string major;

public:
    Student(string n = "", int i = 0, int a = 0, string m = "") : name(n), id(i), age(a), major(m) {}

    string getName() const { return name; }
    void setName(string n) { name = n; }

    int getId() const { return id; }
    void setId(int i) { id = i; }

    int getAge() const { return age; }
    void setAge(int a) { age = a; }

    string getMajor() const { return major; }
    void setMajor(string m) { major = m; }

    void print() const {
        cout << "ID: " << id << ", Name: " << name << ", Age: " << age << ", Major: " << major << endl;
    }

    bool compareById(const Student* other) const {
        return id < other->getId();
    }

    bool compareByName(const Student* other) const {
        return name < other->getName();
    }

    bool compareByAge(const Student* other) const {
        return age < other->getAge();
    }
};

class StudentDatabase {
private:
    Student** students;
    int capacity;
    int count;

    void AddCapacity() {
        capacity *= 2;
        Student** newStudents = new Student * [capacity];
        for (int i = 0; i < count; ++i) {
            newStudents[i] = students[i];
        }
        delete[] students;
        students = newStudents;
    }

public:
    StudentDatabase(int initialCapacity = 10) : capacity(initialCapacity), count(0) {
        students = new Student * [capacity];
    }

    ~StudentDatabase() {
        for (int i = 0; i < count; ++i) {
            delete students[i];
        }
        delete[] students;
    }

    void addStudent(const Student& student) {
        if (count == capacity) {
            AddCapacity();
        }
        students[count++] = new Student(student);
    }

    void removeStudent(int id) {
        for (int i = 0; i < count; ++i) {
            if (students[i]->getId() == id) {
                delete students[i];
                for (int j = i; j < count - 1; ++j) {
                    students[j] = students[j + 1];
                }
                --count;
                return;
            }
        }
        cout << "Student not found." << endl;
    }

    Student* findStudent(int id) {
        for (int i = 0; i < count; ++i) {
            if (students[i]->getId() == id) {
                return students[i];
            }
        }
        return nullptr;
    }

    void listStudents() {
        for (int i = 0; i < count; ++i) {
            students[i]->print();
        }
    }

    void saveToFile(string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (int i = 0; i < count; ++i) {
                file << students[i]->getId() << " "
                    << students[i]->getName() << " "
                    << students[i]->getAge() << " "
                    << students[i]->getMajor() << endl;
            }
            file.close();
        }
        else {
            cout << "Unable to open file." << endl;
        }
    }

    void loadFromFile(string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            count = 0;
            while (!file.eof()) {
                int id, age;
                string name, major;
                file >> id >> name >> age >> major;
                if (file) {
                    addStudent(Student(name, id, age, major));
                }
            }
            file.close();
        }
        else {
            cout << "Unable to open file." << endl;
        }
    }

    void updateStudent(int id, const Student& newInfo) {
        Student* student = findStudent(id);
        if (student) {
            student->setName(newInfo.getName());
            student->setAge(newInfo.getAge());
            student->setMajor(newInfo.getMajor());
        }
        else {
            cout << "Student not found." << endl;
        }
    }

    void filterByMajor(const string& major) {
        for (int i = 0; i < count; ++i) {
            if (students[i]->getMajor() == major) {
                students[i]->print();
            }
        }
    }

    void filterByAge(int age) {
        for (int i = 0; i < count; ++i) {
            if (students[i]->getAge() == age) {
                students[i]->print();
            }
        }
    }

    void sortById() {
        sort(students, students + count, [](const Student* a, const Student* b) {
            return a->compareById(b);
            });
    }

    void sortByName() {
        sort(students, students + count, [](const Student* a, const Student* b) {
            return a->compareByName(b);
            });
    }

    void sortByAge() {
        sort(students, students + count, [](const Student* a, const Student* b) {
            return a->compareByAge(b);
            });
    }

    int getStudentCount() {
        return count;
    }
};

void displayMenu() {
    cout << "\nStudent Database Menu\n";
    cout << "1. Add Student\n";
    cout << "2. Remove Student\n";
    cout << "3. Find Student\n";
    cout << "4. List All Students\n";
    cout << "5. Save to File\n";
    cout << "6. Load from File\n";
    cout << "7. Update Student\n";
    cout << "8. Filter by Major\n";
    cout << "9. Filter by Age\n";
    cout << "10. Sort by ID\n";
    cout << "11. Sort by Name\n";
    cout << "12. Sort by Age\n";
    cout << "13. Get Student Count\n";
    cout << "14. Exit\n";
    cout << "Enter your choice: ";
}

void handleMenuChoice(StudentDatabase& db, int choice) {
    switch (choice) {
    case 1: {
        string name, major;
        int id, age;
        cout << "Enter ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Age: ";
        cin >> age;
        cin.ignore();
        cout << "Enter Major: ";
        getline(cin, major);
        db.addStudent(Student(name, id, age, major));
        break;
    }
    case 2: {
        int id;
        cout << "Enter ID of student to remove: ";
        cin >> id;
        db.removeStudent(id);
        break;
    }
    case 3: {
        int id;
        cout << "Enter ID of student to find: ";
        cin >> id;
        Student* student = db.findStudent(id);
        if (student) {
            student->print();
        }
        else {
            cout << "Student not found." << endl;
        }
        break;
    }
    case 4: {
        db.listStudents();
        break;
    }
    case 5: {
        string filename;
        cout << "Enter filename to save: ";
        getline(cin, filename);
        db.saveToFile(filename);
        break;
    }
    case 6: {
        string filename;
        cout << "Enter filename to load: ";
        getline(cin, filename);
        db.loadFromFile(filename);
        break;
    }
    case 7: {
        int id;
        cout << "Enter ID of student to update: ";
        cin >> id;
        cin.ignore();
        string name, major;
        int age;
        cout << "Enter new Name: ";
        getline(cin, name);
        cout << "Enter new Age: ";
        cin >> age;
        cin.ignore();
        cout << "Enter new Major: ";
        getline(cin, major);
        db.updateStudent(id, Student(name, id, age, major));
        break;
    }
    case 8: {
        string major;
        cout << "Enter major to filter by: ";
        getline(cin, major);
        db.filterByMajor(major);
        break;
    }
    case 9: {
        int age;
        cout << "Enter age to filter by: ";
        cin >> age;
        db.filterByAge(age);
        break;
    }
    case 10: {
        db.sortById();
        cout << "Students sorted by ID." << endl;
        break;
    }
    case 11: {
        db.sortByName();
        cout << "Students sorted by Name." << endl;
        break;
    }
    case 12: {
        db.sortByAge();
        cout << "Students sorted by Age." << endl;
        break;
    }
    case 13: {
        cout << "Total number of students: " << db.getStudentCount() << endl;
        break;
    }
    case 14: {
        cout << "Exiting..." << endl;
        break;
    }
    default: {
        cout << "Invalid choice, please try again." << endl;
    }
    }
}

int main() {
    StudentDatabase db;
    int choice;

    do {
        displayMenu();
        cin >> choice;
        cin.ignore();

        handleMenuChoice(db, choice);

    } while (choice != 14);

}