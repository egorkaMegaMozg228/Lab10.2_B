#include <iostream>
#include <iomanip>
#include <string>
#include <locale>
#include <algorithm>
using namespace std;

// Перелік для спеціальностей
enum Specialization {
    COMPUTER_SCIENCE,
    INFORMATICS,
    MATH_ECONOMICS,
    PHYSICS_INFORMATICS,
    LABOR_EDUCATION
};

// Структура для зберігання даних про студента
struct Student {
    string surname;
    int course;
    Specialization spec;
    double physics;
    double math;

    union {
        double programming;
        double numericalMethods;
        double pedagogy;
    } thirdGrade;
};

// Функція для виведення назви спеціальності
string getSpecializationName(Specialization spec) {
    switch (spec) {
    case COMPUTER_SCIENCE: return "Computer Science";
    case INFORMATICS: return "Informatics";
    case MATH_ECONOMICS: return "Math & Economics";
    case PHYSICS_INFORMATICS: return "Physics & Informatics";
    case LABOR_EDUCATION: return "Labor Education";
    default: return "Unknown";
    }
}

// Функція для введення даних студента
void inputStudentData(Student& student) {
    cout << "Введiть прiзвище студента: ";
    cin >> student.surname;
    cout << "Введiть курс (цiле число): ";
    cin >> student.course;

    int specChoice;
    cout << "Оберiть спецiальнiсть (0 - Computer Science, 1 - Informatics, 2 - Math & Economics, 3 - Physics & Informatics, 4 - Labor Education): ";
    cin >> specChoice;
    student.spec = static_cast<Specialization>(specChoice);

    cout << "Введiть оцiнку з фiзики: ";
    cin >> student.physics;
    cout << "Введiть оцiнку з математики: ";
    cin >> student.math;

    if (student.spec == COMPUTER_SCIENCE) {
        cout << "Введiть оцiнку з програмування: ";
        cin >> student.thirdGrade.programming;
    }
    else if (student.spec == INFORMATICS) {
        cout << "Введiть оцiнку з чисельних методiв: ";
        cin >> student.thirdGrade.numericalMethods;
    }
    else {
        cout << "Введiть оцiнку з педагогiки: ";
        cin >> student.thirdGrade.pedagogy;
    }
    cout << endl;
}

// Функція для створення масиву студентів
void createStudentsArray(Student students[], int n) {
    for (int i = 0; i < n; i++) {
        cout << "Студент №" << i + 1 << ":\n";
        inputStudentData(students[i]);
    }
}

// Функція для виведення таблиці з даними студентів
void printStudentsTable(const Student students[], int n) {
    cout << left << setw(5) << "№"
        << setw(15) << "Прiзвище"
        << setw(10) << "Курс"
        << setw(20) << "Спецiальнiсть"
        << setw(10) << "Фiзика"
        << setw(10) << "Математика"
        << setw(15) << "Програмування"
        << setw(15) << "Чисел. мет."
        << setw(10) << "Педагогiка" << endl;

    cout << string(110, '-') << endl;

    for (int i = 0; i < n; i++) {
        cout << left << setw(5) << (i + 1)
            << setw(15) << students[i].surname
            << setw(10) << students[i].course
            << setw(20) << getSpecializationName(students[i].spec)
            << setw(10) << students[i].physics
            << setw(10) << students[i].math;

        if (students[i].spec == COMPUTER_SCIENCE) {
            cout << setw(15) << students[i].thirdGrade.programming
                << setw(15) << "-"
                << setw(10) << "-";
        }
        else if (students[i].spec == INFORMATICS) {
            cout << setw(15) << "-"
                << setw(15) << students[i].thirdGrade.numericalMethods
                << setw(10) << "-";
        }
        else {
            cout << setw(15) << "-"
                << setw(15) << "-"
                << setw(10) << students[i].thirdGrade.pedagogy;
        }
        cout << endl;
    }
}

// Функція для сортування студентів
void sortStudents(Student students[], int n, int sortType) {
    switch (sortType) {
    case 1:
        // Сортування за спеціальністю, потім за оцінкою з математики, потім за прізвищем
        sort(students, students + n, [](const Student& a, const Student& b) {
            if (a.spec != b.spec) return a.spec < b.spec;
            if (a.math != b.math) return a.math < b.math;
            return a.surname > b.surname;
            });
        break;

    case 2:
        // Сортування лише за оцінкою з математики
        sort(students, students + n, [](const Student& a, const Student& b) {
            return a.math > b.math;
            });
        break;

    case 3:
        // Сортування за прізвищем
        sort(students, students + n, [](const Student& a, const Student& b) {
            return a.surname > b.surname;
            });
        break;

    default:
        cout << "Неправильний вибір сортування.\n";
        break;
    }
}
void createIndexArray(int indexArray[], Student students[], int n) {
    for (int i = 0; i < n; i++) indexArray[i] = i;

    sort(indexArray, indexArray + n, [students](int i, int j) {
        if (students[i].spec != students[j].spec) return students[i].spec < students[j].spec;
        if (students[i].math != students[j].math) return students[i].math < students[j].math;
        return students[i].surname < students[j].surname;
        });
}

// Функція для бінарного пошуку студента
int binarySearchByIndex(const Student students[], const int indexArray[], int n, const string& surname, Specialization spec, double math) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int realIndex = indexArray[mid];
        const Student& student = students[realIndex];

        if (student.spec == spec && student.math == math && student.surname == surname) {
            return realIndex;  // Повертаємо реальний індекс у масиві студентів
        }
        else if ((student.spec < spec) ||
            (student.spec == spec && student.math < math) ||
            (student.spec == spec && student.math == math && student.surname < surname)) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return -1;
}


int main() {
    setlocale(LC_CTYPE, "ukr");
    int n;
    cout << "Введiть кiлькiсть студентiв: ";
    cin >> n;

    Student* students = new Student[n];
    createStudentsArray(students, n);

    // Меню для вибору типу сортування
    int sortType;
    cout << "\nОберiть тип сортування:\n";
    cout << "1 - За спецiальнiстю, оцiнкою з математики та прiзвищем\n";
    cout << "2 - За оцiнкою з математики\n";
    cout << "3 - За прiзвищем\n";
    cout << "Ваш вибiр: ";
    cin >> sortType;

    // Сортування масиву студентів згідно з обраним типом
    sortStudents(students, n, sortType);

    // Виведення таблиці після сортування
    cout << "\nТаблиця студентiв пiсля сортування:\n";
    printStudentsTable(students, n);
    int* indexArray = new int[n];
    createIndexArray(indexArray, students, n);

    string surname;
    int specChoice;
    double math;
    cout << "\nВведiть прiзвище для пошуку: ";
    cin >> surname;
    cout << "Оберiть спецiальнiсть для пошуку (0 - Computer Science, 1 - Informatics, 2 - Math & Economics, 3 - Physics & Informatics, 4 - Labor Education): ";
    cin >> specChoice;
    cout << "Введiть оцiнку з математики для пошуку: ";
    cin >> math;

    // Виконуємо бінарний пошук у впорядкованому індексному масиві
    int foundIndex = binarySearchByIndex(students, indexArray, n, surname, static_cast<Specialization>(specChoice), math);
    if (foundIndex != -1) {
        cout << "Студент знайдений на позицiї " << foundIndex + 1 << " у впорядкованому масивi.\n";
    }
    else {
        cout << "Студента не знайдено.\n";
    }

    delete[] students;
    delete[] indexArray;
    return 0;
}