#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::remove

using namespace std;

// Node representing an Employee
class Employee {
public:
    string name;
    vector<Employee*> subordinates;  // List of subordinates

    // Constructor to initialize employee with a name
    Employee(string name) {
        this->name = name;
    }

    // Add a subordinate to the employee
    void addSubordinate(Employee* subordinate) {
        subordinates.push_back(subordinate);
    }
};

// Class representing the employee hierarchy tree
class EmployeeHierarchy {
private:
    Employee* CEO;  // Root of the tree (CEO)

public:
    // Constructor to initialize the hierarchy with a CEO
    EmployeeHierarchy(string ceoName) {
        CEO = new Employee(ceoName);
    }

    // Recursive function to find an employee in the hierarchy
    Employee* findEmployee(Employee* root, string name) {
        if (root == NULL) {
            return NULL;
        }

        if (root->name == name) {
            return root;
        }

        // Recursively search for the employee in the subordinates
        for (Employee* subordinate : root->subordinates) {
            Employee* found = findEmployee(subordinate, name);
            if (found != NULL) {
                return found;
            }
        }

        return NULL;
    }

    // Add a new employee under a supervisor
    void addEmployee(string supervisorName, string employeeName) {
        Employee* supervisor = findEmployee(CEO, supervisorName);
        if (supervisor != NULL) {
            Employee* newEmployee = new Employee(employeeName);
            supervisor->addSubordinate(newEmployee);
            cout << "Employee " << employeeName << " added under " << supervisorName << endl;
        } else {
            cout << "Supervisor " << supervisorName << " not found!" << endl;
        }
    }

    // Recursive function to display the hierarchy tree
    void displayHierarchy(Employee* root, int level) {
        if (root == NULL) {
            return;
        }

        // Print the employee name with indentations based on level
        for (int i = 0; i < level; i++) {
            cout << "  ";
        }
        cout << "|--" << root->name << endl;

        // Recursively display all subordinates
        for (Employee* subordinate : root->subordinates) {
            displayHierarchy(subordinate, level + 1);
        }
    }

    // Display the entire hierarchy starting from the CEO
    void showHierarchy() {
        displayHierarchy(CEO, 0);
    }

    // Function to get the total number of employees (including the CEO)
    int getTotalEmployees(Employee* root) {
        if (root == NULL) {
            return 0;
        }

        int count = 1;  // Count the current employee

        // Recursively count all subordinates
        for (Employee* subordinate : root->subordinates) {
            count += getTotalEmployees(subordinate);
        }

        return count;
    }

    // Function to return total number of employees in the company
    int getTotalEmployees() {
        return getTotalEmployees(CEO);
    }

    // Function to find and remove an employee from the hierarchy
    bool removeEmployee(Employee* root, string employeeName, Employee* parent = NULL) {
        if (root == NULL) {
            return false;
        }

        // Check if the current employee is the one to be removed
        if (root->name == employeeName) {
            if (parent != NULL) {
                // Transfer the subordinates of the removed employee to their supervisor
                for (Employee* subordinate : root->subordinates) {
                    parent->addSubordinate(subordinate);
                }

                // Find and remove the employee from the parent's subordinates
                auto& subordinates = parent->subordinates;
                subordinates.erase(std::remove(subordinates.begin(), subordinates.end(), root), subordinates.end());
                delete root;  // Free the memory
                cout << "Employee " << employeeName << " removed successfully." << endl;
            } else {
                cout << "Cannot remove the CEO!" << endl;
            }
            return true;
        }

        // Recursively check all subordinates
        for (Employee* subordinate : root->subordinates) {
            if (removeEmployee(subordinate, employeeName, root)) {
                return true;
            }
        }

        return false;  // Employee not found
    }

    // Public function to remove employee
    void removeEmployee(string employeeName) {
        if (!removeEmployee(CEO, employeeName)) {
            cout << "Employee " << employeeName << " not found!" << endl;
        }
    }
};

int main() {
    // Initialize the hierarchy with the CEO
    EmployeeHierarchy company("CEO");

    int choice;
    string supervisorName, employeeName;

    do {
        cout << "\n--- Employee Hierarchy Management ---\n";
        cout << "1. Add Employee\n";
        cout << "2. Show Hierarchy\n";
        cout << "3. Get Total Employees\n";
        cout << "4. Remove Employee\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Supervisor Name: ";
                cin >> supervisorName;
                cout << "Enter Employee Name: ";
                cin >> employeeName;
                company.addEmployee(supervisorName, employeeName);
                break;

            case 2:
                cout << "\nEmployee Hierarchy:\n";
                company.showHierarchy();
                break;

            case 3:
                cout << "Total Employees: " << company.getTotalEmployees() << endl;
                break;

            case 4:
                cout << "Enter Employee Name to Remove: ";
                cin >> employeeName;
                company.removeEmployee(employeeName);
                break;

            case 5:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
                break;
        }
    } while (choice != 5);

    return 0;
}
